#include "c4mk/getopt.h"

#include <errno.h>
#include <string.h>

c4mk_errno_t
c4mk_getopt(int argc, char **argv, const char *dsc,
            __INOUT struct c4mk_getopt_ctx *ctx)
{
	c4mk_errno_t err = 0;

	if (argv == NULL || dsc == NULL || ctx == NULL || argc == 0) {
		return -EFAULT;
	}

	if (ctx->i == 0) {
		ctx->dsc_len = strnlen(dsc, C4MK_GETOPT_DSC_MAX_SIZE);
	}

	if (ctx->i >= argc) {
		return 0;
	}

	ctx->opt_arg = argv[ctx->i];

	if (ctx->opt_arg[0] == '-') {
		ctx->opt = strchr(dsc, ctx->opt_arg[1]);
		if (ctx->opt == NULL) {
			ctx->opt = "?";

			goto fin;
		} else {
			if (ctx->opt[1] == ':') {
				ctx->i++;
				ctx->opt_arg = argv[ctx->i];
			}
		}
	} else {
		ctx->opt = "?";
	}

fin:
	err = argc - ctx->i;
	ctx->i++;
	return err;
}
