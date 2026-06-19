#include <c4/mem/mem.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define FREE_MASK (0xFE)

struct c4_mem_cfg g_c4_mem_cfg = {0};

void
c4_mem_set_cfg(const struct c4_mem_cfg *cfg)
{
        if (cfg == NULL) {
                return;
        }

        g_c4_mem_cfg = *cfg;
}

void *
c4_mem_alloc(size_t s)
{
        assert(s > 0);

        if (g_c4_mem_cfg.alloc_cb == NULL) {
                return malloc(s);
        }
        return g_c4_mem_cfg.alloc_cb(s);
}

void *
c4_mem_calloc(size_t item_count, size_t item_size)
{
        assert(item_count > 0 && item_size > 0);

        if (g_c4_mem_cfg.calloc_cb == NULL) {
                return calloc(item_count, item_size);
        }
        return g_c4_mem_cfg.calloc_cb(item_count, item_size);
}

void *
c4_mem_realloc(void *mem, size_t ns)
{
        assert(mem != NULL);
        assert(ns > 0);

        if (g_c4_mem_cfg.realloc_cb == NULL) {
                return realloc(mem, ns);
        }
        return g_c4_mem_cfg.realloc_cb(mem, ns);
}

void
c4_mem_free(void *mem)
{
        assert(mem != NULL);

        if (g_c4_mem_cfg.free_cb == NULL) {
                free(mem);
                return;
        }

        g_c4_mem_cfg.free_cb(mem);
}

void
__c4_mem_tag(void *mem, const char *file, int line)
{
        assert(mem != NULL);
        assert(file != NULL);

        if (g_c4_mem_cfg.tag_cb == NULL) {
                return;
        }
        g_c4_mem_cfg.tag_cb(mem, file, line);
}

void *
c4_mem_alloc_zero(size_t s)
{
        return c4_mem_calloc(1, s);
}

void *
c4_mem_realloc_mask(void *mem, size_t s, size_t ns)
{
        void *n = NULL;
        size_t cp_size = s;

        assert(mem != NULL);

        n = c4_mem_alloc_zero(ns);
        if (n == NULL) {
                return NULL;
        }

        cp_size = ns > cp_size ? cp_size : ns;
        memcpy(n, mem, cp_size);
        c4_mem_free_mask(mem, s);
        return n;
}

void
c4_mem_free_mask(void *mem, size_t s)
{
        assert(mem != NULL);

        memset(mem, FREE_MASK, s);
        c4_mem_free(mem);
}
