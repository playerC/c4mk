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
        if (g_c4_mem_cfg.alloc_cb == NULL) {
                return malloc(s);
        }
        return g_c4_mem_cfg.alloc_cb(s);
}

void *
c4_mem_calloc(size_t item_count, size_t item_size)
{
        if (g_c4_mem_cfg.calloc_cb == NULL) {
                return calloc(item_count, item_size);
        }
        return g_c4_mem_cfg.calloc_cb(item_count, item_size);
}

void *
c4_mem_realloc(void *mem, size_t ns)
{
        if (g_c4_mem_cfg.realloc_cb == NULL) {
                return realloc(mem, ns);
        }
        return g_c4_mem_cfg.realloc_cb(mem, ns);
}

void
c4_mem_free(void *mem)
{
        if (g_c4_mem_cfg.free_cb == NULL) {
                free(mem);
                return;
        }

        g_c4_mem_cfg.free_cb(mem);
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

        if (mem == NULL) {
                return NULL;
        }

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
        if (mem == NULL) {
                return;
        }

        memset(mem, FREE_MASK, s);
        c4_mem_free(mem);
}
