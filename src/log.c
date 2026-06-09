#include "c4mk/log.h"

#include <stdarg.h>
#include <stdio.h>

void
c4mk_log_printf(int level, const char *fname, int lno, const char *fmt, ...)
{
        const char *lstr = NULL;
        FILE *outf = stdout;
        va_list args;

        switch (level) {
        case C4MK_LOG_DEBUG:
                lstr = "###";
                outf = stdout;
                break;
        case C4MK_LOG_INFO:
                lstr = "+++";
                outf = stdout;
                break;
        case C4MK_LOG_WARN:
                lstr = "***";
                outf = stdout;
                break;
        case C4MK_LOG_FATAL:
                lstr = "!!!";
                outf = stderr;
                break;
        default:
                lstr = "???";
                outf = stdout;
                break;
        }

        fprintf(outf, "[%s] ", lstr);

        switch (level) {
        case C4MK_LOG_INFO:
        case C4MK_LOG_WARN:
        default:
                break;
        case C4MK_LOG_DEBUG:
        case C4MK_LOG_FATAL:
                fprintf(outf, "loc:[%s:%d] ", fname, lno);
                break;
        }

        va_start(args, fmt);
        vfprintf(outf, fmt, args);
        va_end(args);

        fflush(outf);
}
