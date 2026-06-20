#ifndef _C4_STR__STR_VIEW_H_
#define _C4_STR__STR_VIEW_H_

#include <c4/base/cdefs.h>
#include <c4/base/types.h>

struct c4_str_view {
        const char *ch;
        size_t byte_length;
        size_t char_length;
};

__BEGIN_DECLS

c4_errno_t
c4_str_view_init(struct c4_str_view *, const char *);

c4_errno_t
c4_str_view_init_n(struct c4_str_view *, const char *, size_t);

c4_errno_t
c4_str_view_findl(struct c4_str_view *, const struct c4_str_view *,
                  size_t __out *);

c4_errno_t
c4_str_view_findr(struct c4_str_view *, const struct c4_str_view *,
                  size_t __out *);

c4_errno_t
c4_str_view_char_length(const char *, size_t, size_t __out *);

c4_errno_t
c4_str_view_index_from_char_index(const char *, size_t, size_t,
                                  size_t __out *);

struct c4_str_view
c4_str_view_sub(struct c4_str_view *, size_t ,size_t);

c4_errno_t
c4_str_view_char_bytes(char);

__END_DECLS

#endif
