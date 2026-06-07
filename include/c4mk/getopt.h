#ifndef _C4MK_GETOPT_H_
#define _C4MK_GETOPT_H_

#include <c4mk/cdefs.h>
#include <c4mk/types.h>

__BEGIN_DECLS

#define C4MK_GETOPT_DSC_MAX_SIZE (1024)

struct c4mk_getopt_ctx {
	char *opt;
	char *opt_arg;
	int i;
	size_t dsc_len;
};

/* return: remain arg count. */
c4mk_errno_t
c4mk_getopt(int, char **, const char *, __INOUT struct c4mk_getopt_ctx *);

__END_DECLS
#endif
