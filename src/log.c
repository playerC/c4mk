#include "c4mk/log.h"

#include <stdarg.h>
#include <stdio.h>

void
c4mk_log_printf(int level, const char *fname, int lno, const char *fmt, ...)
{
	const char *lstr = NULL;
	va_list args;

	switch (level) {
	case C4MK_LOG_DEBUG:
		lstr = "###";
		break;
	case C4MK_LOG_INFO:
		lstr = "+++";
		break;
	case C4MK_LOG_WARN:
		lstr = "***";
		break;
	case C4MK_LOG_FATAL:
		lstr = "!!!";
		break;
	default:
		lstr = "???";
		break;
	}

	printf("[%s] ", lstr);

	switch (level) {
	case C4MK_LOG_INFO:
	case C4MK_LOG_WARN:
	default:
		break;
	case C4MK_LOG_DEBUG:
	case C4MK_LOG_FATAL:
		printf("loc:[%s:%d] ", fname, lno);
		break;
	}

	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);

	fflush(stdout);
}
