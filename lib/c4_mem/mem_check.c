#include "c4/mem/mem_check.h"
#include "c4/base/cdefs.h"
#include "c4/base/queue.h"
#include "c4/base/types.h"
#include "c4/log/log.h"
#include "c4/mem/mem.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEM_VERSION (0xC4)
#define MEM_TAIL_SIZE (1)
#define MEM_CLEAR_MASK (0xCE)

struct _mem_blk;
TAILQ_HEAD(_mem_blk_list, _mem_blk);

struct _mem_db;

struct _mem_db {
        struct _mem_blk_list mem_list;
        pthread_mutex_t mem_list_lock;

        struct _mem_blk_list free_list;
        pthread_mutex_t free_list_lock;

        int free_timeout_s;

        /* use free_list_lock lock this. */
        time_t next_free_at;
};

struct _mem_blk {
        size_t blk_size;

        const char *file;
        int line;

        time_t free_at;

        TAILQ_ENTRY(_mem_blk) list_entry;
        unsigned char version;
};

static struct _mem_db g_mem_db = {0};

static pthread_once_t g_mem_db_init_once = PTHREAD_ONCE_INIT;

static void *
_alloc(size_t);

static void *
_calloc(size_t, size_t);

static void *
_realloc(void *, size_t);

static void
_free(void *);

static void
_tag(void *, const char *, int);

struct c4_mem_cfg g_mem_check_cfg = {_alloc, _calloc, _realloc, _free,
                                     _tag};

static size_t
_mem_blk_size(size_t req_size)
{
        return sizeof(struct _mem_blk) + req_size + MEM_TAIL_SIZE;
}
static void *
_mem_blk_to_user(const struct _mem_blk *self)
{
        return (void *)(self + 1);
}

static c4_bool_t
_mem_blk_is_tail_valid(const struct _mem_blk *self)
{
        const char *tail_version =
            (const char *)_mem_blk_to_user(self);
        return tail_version[self->blk_size] == (char)MEM_VERSION;
}

static c4_bool_t
_mem_blk_is_valid(const struct _mem_blk *self)
{
        return self->version == MEM_VERSION &&
               _mem_blk_is_tail_valid(self);
}

static struct _mem_blk *
_mem_blk_from_user(const void *mem)
{
        struct _mem_blk *blk = ((struct _mem_blk *)mem) - 1;

        const c4_bool_t is_valid = _mem_blk_is_valid(blk);

        assert(is_valid == 1);

        if (is_valid) {
                return blk;
        }

        c4_log_errx(EXIT_FAILURE, "%s:%d mem is not valid.\n",
                    __FILE__, __LINE__);

        return NULL;
}

static c4_errno_t
_mem_blk_init(struct _mem_blk *self, size_t req_size)
{
        self->version = MEM_VERSION;
        self->blk_size = req_size;

        {
                char *tail_version = NULL;

                tail_version = (char *)_mem_blk_to_user(self);
                assert(tail_version != NULL);
                tail_version[req_size] = MEM_VERSION;
        }
        return 0;
}

static void
_mem_blk_clear(struct _mem_blk *self)
{
        void *mem = _mem_blk_to_user(self);

        assert(mem != NULL);
        memset(mem, MEM_CLEAR_MASK, self->blk_size);
        self->version = MEM_CLEAR_MASK;
}

static struct _mem_blk *
g_mem_db_add_blk_ts(size_t req_size)
{
        const size_t alloc_size = _mem_blk_size(req_size);
        struct _mem_blk *blk = NULL;

        blk = calloc(1, alloc_size);
        assert(blk != NULL);
        if (blk == NULL) {
                return NULL;
        }

        _mem_blk_init(blk, req_size);

        pthread_mutex_lock(&g_mem_db.mem_list_lock);
        {
                TAILQ_INSERT_TAIL(&g_mem_db.mem_list, blk,
                                  list_entry);
        }
        pthread_mutex_unlock(&g_mem_db.mem_list_lock);
        return blk;
}

static c4_errno_t
g_mem_db_mv_blk_to_free_ts(struct _mem_blk *blk)
{
        c4_errno_t ex = 0;
        time_t free_at;

        time(&free_at);
        free_at += g_mem_db.free_timeout_s;

        blk->free_at = free_at;
        pthread_mutex_lock(&g_mem_db.mem_list_lock);
        {
                TAILQ_REMOVE(&g_mem_db.mem_list, blk, list_entry);
        }
        pthread_mutex_unlock(&g_mem_db.mem_list_lock);

        _mem_blk_clear(blk);

        pthread_mutex_lock(&g_mem_db.free_list_lock);
        {
                TAILQ_INSERT_TAIL(&g_mem_db.free_list, blk,
                                  list_entry);
        }
        pthread_mutex_unlock(&g_mem_db.free_list_lock);

        return ex;
}

static c4_errno_t
g_mem_db_free_blk_if_need_ts()
{
        time_t now;

        time(&now);
        if (difftime(now, g_mem_db.next_free_at) < 0) {
                return 0;
        }

        pthread_mutex_lock(&g_mem_db.free_list_lock);
        {
                struct _mem_blk *item, *tmp;
                time_t next_free_at;

                if (difftime(now, g_mem_db.next_free_at) < 0) {
                        goto clean;
                }

                next_free_at = now + g_mem_db.free_timeout_s;
                TAILQ_FOREACH_SAFE(item, &g_mem_db.free_list,
                                   list_entry, tmp)
                {
                        if (difftime(item->free_at, now) > 0) {
                                continue;
                        }
                        TAILQ_REMOVE(&g_mem_db.free_list, item,
                                     list_entry);

                        _mem_blk_clear(item);

                        free(item);
                }

                if (!TAILQ_EMPTY(&g_mem_db.free_list)) {
                        next_free_at =
                            TAILQ_FIRST(&g_mem_db.free_list)->free_at;
                }
                g_mem_db.next_free_at = next_free_at;
        }
clean:
        pthread_mutex_unlock(&g_mem_db.free_list_lock);

        return 0;
}

static void
g_mem_db_init(void)
{
        {
                pthread_mutexattr_t ma = NULL;
                pthread_mutexattr_init(&ma);
                pthread_mutexattr_settype(&ma,
                                          PTHREAD_MUTEX_RECURSIVE);

                pthread_mutex_init(&g_mem_db.mem_list_lock, &ma);
                pthread_mutex_init(&g_mem_db.free_list_lock, &ma);

                pthread_mutexattr_destroy(&ma);
        }

        TAILQ_INIT(&g_mem_db.mem_list);
        TAILQ_INIT(&g_mem_db.free_list);

        g_mem_db.free_timeout_s = 7;
}

static void
ensure_mem_db_init(void)
{
        pthread_once(&g_mem_db_init_once, g_mem_db_init);
}

static void *
_alloc(size_t s)
{
        void *mem = NULL;
        struct _mem_blk *blk = NULL;

        ensure_mem_db_init();

        blk = g_mem_db_add_blk_ts(s);
        if (blk == NULL) {
                return NULL;
        }

        mem = _mem_blk_to_user(blk);
        return mem;
}

static void *
_calloc(size_t n, size_t s)
{
        return _alloc(n * s);
}

static void *
_realloc(void *m, size_t ns)
{
        struct _mem_blk *blk = NULL;
        struct _mem_blk *nblk = NULL;
        void *nm = NULL;

        ensure_mem_db_init();

        g_mem_db_free_blk_if_need_ts();

        blk = _mem_blk_from_user(m);
        if (blk == NULL) {
                return NULL;
        }

        if (blk->blk_size > ns) {
                return m;
        }

        nm = _calloc(1, ns);
        if (nm == NULL) {
                return NULL;
        }

        memcpy(nm, m, ns);

        g_mem_db_mv_blk_to_free_ts(blk);
        return nm;
}

static void
_free(void *m)
{
        struct _mem_blk *blk = NULL;

        ensure_mem_db_init();

        g_mem_db_free_blk_if_need_ts();

        blk = _mem_blk_from_user(m);
        if (blk == NULL) {
                return;
        }

        g_mem_db_mv_blk_to_free_ts(blk);
}

static void
_tag(void *mem, const char *file, int line)
{
        struct _mem_blk *blk = NULL;

        ensure_mem_db_init();

        blk = _mem_blk_from_user(m);
        if (blk == NULL) {
                return;
        }

        blk->file = file;
        blk->line = line;
}

c4_errno_t
c4_mem_check(void)
{
        struct _mem_blk *item = NULL;

        ensure_mem_db_init();
        if (TAILQ_EMPTY(&g_mem_db.mem_list)) {
                return 0;
        }

        TAILQ_FOREACH(item, &g_mem_db.mem_list, list_entry)
        {
                if (item->file != NULL) {
                        c4_log_warn("mem not freed: size:[%ld], "
                                    "alloc at:[%s:%d]\n",
                                    item->blk_size, item->file,
                                    item->line);
                } else {
                        c4_log_warn("mem not freed: size:[%ld]\n",
                                    item->blk_size);
                }
        }
}
