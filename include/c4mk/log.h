#ifndef _C4MK_LOG_H_
#define _C4MK_LOG_H_

#include <c4mk/cdefs.h>

/* exit() use this. */
#include <stdlib.h>

__BEGIN_DECLS
#define C4MK_LOG_DEBUG (0)
#define C4MK_LOG_INFO (1)
#define C4MK_LOG_WARN (2)
#define C4MK_LOG_FATAL (3)

#ifndef C4MK_LOG_LEVEL
#define C4MK_LOG_LEVEL (C4MK_LOG_INFO)
#endif

#if C4MK_LOG_LEVEL <= C4MK_LOG_DEBUG
#define c4mk_log_debug(fmt, ...)                                               \
	(void)c4mk_log_printf(C4MK_LOG_DEBUG, __FILE__, __LINE__, fmt,         \
	                      ##__VA_ARGS__)
#else
#define c4mk_log_debug(fmt, ...)
#endif

#if C4MK_LOG_LEVEL <= C4MK_LOG_INFO
#define c4mk_log_info(fmt, ...)                                                \
	(void)c4mk_log_printf(C4MK_LOG_INFO, __FILE__, __LINE__, fmt,          \
	                      ##__VA_ARGS__)
#else
#define c4mk_log_info(fmt, ...)
#endif

#if C4MK_LOG_LEVEL <= C4MK_LOG_WARN
#define c4mk_log_warn(fmt, ...)                                                \
	(void)c4mk_log_printf(C4MK_LOG_WARN, __FILE__, __LINE__, fmt,          \
	                      ##__VA_ARGS__)
#else
#define c4mk_log_warn(fmt, ...)
#endif

#if C4MK_LOG_LEVEL <= C4MK_LOG_FATAL
/* when fatal calls exit. */
#define c4mk_log_fatal(fmt, ...)                                               \
	do {                                                                   \
		(void)c4mk_log_printf(C4MK_LOG_FATAL, __FILE__, __LINE__, fmt, \
		                      ##__VA_ARGS__);                          \
		exit(1);                                                       \
	} while (0)
#else
#define c4mk_log_fatal(fmt, ...) exit(1)
#endif

void
c4mk_log_printf(int, const char *, int, const char *, ...);

__END_DECLS
#endif
