#define C4MK_LOG_LEVEL 0
#include <c4mk/log.h>

int
main(int argc, char **argv)
{
	c4mk_log_debug("this is debug\n");
	c4mk_log_debug("this is debug %s\n", "sample arg");
	c4mk_log_info("this is info\n");
	c4mk_log_warn("this is warn\n");
	c4mk_log_fatal("this is fatal, and exit here.\n");
	c4mk_log_info("this is info never show.\n");
	return 0;
}
