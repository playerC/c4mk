#ifndef _C4_MEM__MEM_CHECK_H_
#define _C4_MEM__MEM_CHECK_H_

#include <c4/base/cdefs.h>
#include <c4/base/types.h>

__BEGIN_DECLS

extern struct c4_mem_cfg g_mem_check_cfg;

c4_errno_t
c4_mem_check(void);

__END_DECLS

#endif
