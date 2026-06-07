#include <c4mk/getopt.h>

#include <stdio.h>

int
main(int argc, char **argv)
{
	struct c4mk_getopt_ctx ctx = {0};

	while (c4mk_getopt(argc, argv, "S:B:v", &ctx)) {
		printf("opt:[%s], arg:[%s]\n", ctx.opt, ctx.opt_arg);
	}

	return 0;
}
