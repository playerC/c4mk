#include "c4/str/str_view.h"

#include <assert.h>
#include <errno.h>
#include <string.h>

c4_errno_t
c4_str_view_init(struct c4_str_view *self, const char *s)
{
        return c4_str_view_init_n(self, s, strlen(s));
}

c4_errno_t
c4_str_view_init_n(struct c4_str_view *self, const char *s,
                   size_t len)
{
        c4_errno_t ex = 0;

        assert(self != NULL);

        self->ch = s;
        self->byte_length = len;
        ex = c4_str_view_char_length(s, self->byte_length,
                                     &self->char_length);
        return ex;
}

c4_errno_t
c4_str_view_findl(struct c4_str_view *self,
                  const struct c4_str_view *mv, size_t __out *i)
{
        size_t ti = 0;
        int eq = 0;
        size_t tlen = 0;
        char *tp = NULL;

        assert(self != NULL);
        assert(mv != NULL);

        if (mv->byte_length > self->byte_length) {
                return -EPERM;
        }

        tlen = (self->byte_length - mv->byte_length);
        for (; ti < tlen; ++ti) {
                tp = self->ch + ti;
                if (*tp == mv->ch[0]) {
                        eq = memcmp(tp, mv->ch, mv->byte_length);

                        if (eq == 0) {
                                *i = ti;
                                return 0;
                        }
                }
        }
        return -ENOENT;
}

c4_errno_t
c4_str_view_findr(struct c4_str_view *self,
                  const struct c4_str_view *mv, size_t __out *i)
{
        size_t ti = 0;
        int eq = 0;
        size_t tlen = 0;
        char *tp = NULL;

        assert(self != NULL);
        assert(mv != NULL);

        if (mv->byte_length > self->byte_length) {
                return -EPERM;
        }

        tlen = (self->byte_length - mv->byte_length);
        for (ti = tlen - 1; ti >= 0; --ti) {
                tp = self->ch + ti;
                if (*tp == mv->ch[0]) {
                        eq = memcmp(tp, mv->ch, mv->byte_length);

                        if (eq == 0) {
                                *i = ti;
                                return 0;
                        }
                }
        }
        return -ENOENT;
}

c4_errno_t
c4_str_view_char_length(const char *s, size_t len,
                        size_t __out *ch_len)
{
        int step = 0;
        size_t tmp_len = 0;
        size_t i = 0;

        assert(ch_len != NULL);

        for (; i < len; i += step) {
                step = c4_str_view_char_bytes(s[i]);
                if (C4_HAS_ERROR(step)) {
                        return step;
                }
                ++tmp_len;
        }

        *ch_len = tmp_len;
        return 0;
}

c4_errno_t
c4_str_view_index_from_char_index(const char *s, size_t len,
                                  size_t ch_i, size_t __out *byte_i)
{
        int step = 0;
        size_t tmp_len = 0;
        size_t i = 0;

        assert(byte_i != NULL);

        for (; i < len && tmp_len != ch_i; i += step) {
                step = c4_str_view_char_bytes(s[i]);
                if (C4_HAS_ERROR(step)) {
                        return step;
                }
                ++tmp_len;
        }

        *byte_i = i;
        return 0;
}

struct c4_str_view
c4_str_view_sub(struct c4_str_view *self, size_t si, size_t len)
{
        struct c4_str_view sub = {0};

        sub.ch = self->ch + si;
        sub.byte_length = len;
        c4_str_view_char_length(sub.ch, sub.byte_length,
                                &sub.char_length);
        return sub;
}

#define UTF8_M1 (0x80)
#define UTF8_S1 (0x00)

#define UTF8_M2 (0XE0)
#define UTF8_S2 (0xC0)

#define UTF8_M3 (0xF0)
#define UTF8_S3 (0xE0)

#define UTF8_M4 (0xF8)
#define UTF8_S4 (0xF0)

#define UTF8_MATCH(v, n) if (((v) & (UTF8_M##n)) == (UTF8_S##n))

c4_errno_t
c4_str_view_char_bytes(char c)
{
        UTF8_MATCH(c, 1) { return 1; }
        UTF8_MATCH(c, 2) { return 2; }
        UTF8_MATCH(c, 3) { return 3; }
        UTF8_MATCH(c, 4) { return 4; }
        return -EPERM;
}
