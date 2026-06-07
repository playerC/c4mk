#ifndef _C4MK_TYPES_H_
#define _C4MK_TYPES_H_

#include <stddef.h>

/* >= 0 : success.
 * < 0 : errno. */
typedef int c4mk_errno_t;

/* 0 : false, fail.
 * !=0 : true, success. */
typedef unsigned char c4mk_bool_t;

#endif
