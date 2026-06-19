#include "c4/log/log.h"

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

enum C4_LOG_LEVEL g_c4_log_level = C4_LOG_LEVEL__DEBUG;
void (*g_c4_log_cb)(enum C4_LOG_LEVEL, const char *, va_list) = NULL;

#define TIME_STR_BUFSIZE (128)

#define __DO_LOG(level)                                              \
        va_list vl;                                                  \
                                                                     \
        if (g_c4_log_level <= (level)) {                             \
                va_start(vl, fmt);                                   \
                if (g_c4_log_cb == NULL) {                           \
                        _vlog((level), fmt, vl);                     \
                } else {                                             \
                        g_c4_log_cb((level), fmt, vl);               \
                }                                                    \
                va_end(vl);                                          \
        }

static void
_vlog(enum C4_LOG_LEVEL v, const char *fmt, va_list vl)
{
        FILE *log_file = stdout;
        const char *prefix = "+++";
        char time_str[TIME_STR_BUFSIZE + 1] = {0};
        time_t now;
        struct tm *tm_info;
        int ax;

        switch (v) {
        case C4_LOG_LEVEL__DEBUG:
                prefix = "###";
                log_file = stdout;
                break;
        case C4_LOG_LEVEL__INFO:
                prefix = "+++";
                log_file = stdout;
                break;
        case C4_LOG_LEVEL__WARN:
                prefix = "***";
                log_file = stderr;
                break;
        case C4_LOG_LEVEL__ERRX:
                prefix = "!!!";
                log_file = stderr;
                break;
        default:
                break;
        }

        time(&now);
        tm_info = localtime(&now);
        assert(tm_info != NULL);

        ax = strftime(time_str, TIME_STR_BUFSIZE, "", tm_info);
        assert(ax != 0);

        fprintf(log_file, "%s %s ", prefix, time_str);
        vfprintf(log_file, fmt, vl);
}

void
c4_log_set_level(const struct c4_log_cfg *cfg)
{
        if (cfg == NULL) {
                return;
        }
        g_c4_log_level = cfg->level;
        g_c4_log_cb = cfg->log_cb;
}

void
c4_log_debug(const char *fmt, ...)
{
        __DO_LOG(C4_LOG_LEVEL__DEBUG);
}

void
c4_log_info(const char *fmt, ...)
{
        __DO_LOG(C4_LOG_LEVEL__INFO);
}

void
c4_log_warn(const char *fmt, ...)
{
        __DO_LOG(C4_LOG_LEVEL__WARN);
}

__dead void
c4_log_errx(int code, const char *fmt, ...)
{
        __DO_LOG(C4_LOG_LEVEL__ERRX);
        fflush(stdout);
        fflush(stderr);
        exit(code);
}
