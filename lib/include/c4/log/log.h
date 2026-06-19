#ifndef _C4_LOG__LOG_H_
#define _C4_LOG__LOG_H_

#include <c4/base/cdefs.h>

#include <stdarg.h>

__BEGIN_DECLS

enum C4_LOG_LEVEL {
        C4_LOG_LEVEL__DEBUG,
        C4_LOG_LEVEL__INFO,
        C4_LOG_LEVEL__WARN,
        C4_LOG_LEVEL__ERRX
};

struct c4_log_cfg {
        enum C4_LOG_LEVEL level;
        void (*log_cb)(enum C4_LOG_LEVEL, const char *, va_list);
};

void
c4_log_set_cfg(const struct c4_log_cfg *);

void
c4_log_debug(const char *, ...);

void
c4_log_info(const char *, ...);

void
c4_log_warn(const char *, ...);

__dead void
c4_log_errx(int, const char *, ...);

__END_DECLS
#endif
