#ifndef _C4_BASE__TYPES_H_
#define _C4_BASE__TYPES_H_

#include <stddef.h>

/* >= 0 : success.
 * < 0 : errno. */
typedef int c4_errno_t;

#define C4_HAS_ERROR(x) ((x) < 0)

/* 0 : false, fail.
 * !=0 : true, success. */
typedef unsigned char c4_bool_t;

#define C4_IS_TRUE(x) （(x) != 0)
#define C4_IS_FALSE(x) ((x) == 0)

#ifndef __out
#define __out
#endif

#ifndef __inout
#define __inout
#endif

#endif
