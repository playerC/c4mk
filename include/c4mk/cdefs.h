#ifndef _C4MK_CDEFS_H_
#define _C4MK_CDEFS_H_

#ifndef __BEGIN_DECLS

#if defined(__cplusplus)
#define __BEGIN_DECLS extern "C" {
#define __END_DECLS }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif /* ~ if __cplusplus */

#endif /* ~ ifndef */

/* meta for functions exit the program. */
#ifndef __dead
#define __dead
#endif

/* meta for input/output type of arguments. */
#define __IN
#define __OUT
#define __INOUT

#endif /* ~ header */
