#ifndef _C4_MEM__MEM_H_
#define _C4_MEM__MEM_H_

#include <c4/base/cdefs.h>

#include <stddef.h>

__BEGIN_DECLS

struct c4_mem_cfg {
        void *(*alloc_cb)(size_t);
        void *(*calloc_cb)(size_t, size_t);
        void *(*realloc_cb)(void *, size_t);
        void (*free_cb)(void *);
};

void
c4_mem_set_cfg(const struct c4_mem_cfg *);

void *
c4_mem_alloc(size_t);

void *
c4_mem_calloc(size_t, size_t);

void *
c4_mem_realloc(void *, size_t);

void
c4_mem_free(void *);

#define c4_mem_free_null(mem)                                        \
        do {                                                         \
                c4_mem_free(mem);                                    \
                mem = NULL;                                          \
        } while (0)

void *
c4_mem_alloc_zero(size_t);

void *
c4_mem_realloc_mask(void *, size_t, size_t);

void
c4_mem_free_mask(void *, size_t);

__END_DECLS

#endif
