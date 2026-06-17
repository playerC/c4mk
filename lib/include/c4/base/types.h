#ifndef _C4_BASE__TYPES_H_
#define _C4_BASE__TYPES_H_

#include <stddef.h>

/* >= 0 : success.
 * < 0 : errno. */
typedef int c4_errno_t;

/* 0 : false, fail.
 * !=0 : true, success. */
typedef unsigned char c4_bool_t;

#endif
