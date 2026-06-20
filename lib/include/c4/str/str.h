#ifndef _C4_STR__STR_H_
#define _C4_STR__STR_H_

#include <c4/base/cdefs.h>
#include <c4/base/types.h>

struct c4_str;
struct c4_str_view;

/* utf8 encoding str. */
struct c4_str {
        char *buf;
        size_t byte_length;
        size_t char_length;
        size_t buf_size;
};

__BEGIN_DECLS

c4_errno_t
c4_str_init(struct c4_str *, char *, size_t);

void
c4_str_clear(struct c4_str *);

c4_errno_t
c4_str_set_realloc_buf(struct c4_str *, char *, size_t);

c4_errno_t
c4_str_cat(struct c4_str *, const struct c4_str_view *);

c4_errno_t
c4_str_insert(struct c4_str *, size_t, const struct c4_str_view *);

c4_errno_t
c4_str_replace(struct c4_str *, size_t, size_t, const struct c4_str_view *);

/* please this format use to cut that string.
   - (`%.*s`, size_t, char *) */
c4_errno_t
c4_str_printf(struct c4_str *, const char *, ...);

c4_errno_t
c4_str_substr(struct c4_str *, size_t, size_t, struct c4_str_view __out *);

__END_DECLS

#endif
