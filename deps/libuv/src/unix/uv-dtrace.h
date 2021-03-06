/*
 * Generated by dtrace(1M).
 */

#ifndef	_UV_DTRACE_H
#define	_UV_DTRACE_H

#include <unistd.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define UV_STABILITY "___dtrace_stability$uv$v1$1_1_0_1_1_0_1_1_0_1_1_0_1_1_0"

#define UV_TYPEDEFS "___dtrace_typedefs$uv$v2"

#if !defined(DTRACE_PROBES_DISABLED) || !DTRACE_PROBES_DISABLED

#define	UV_TICK_START(arg0, arg1) \
do { \
	__asm__ volatile(".reference " UV_TYPEDEFS); \
	__dtrace_probe$uv$tick__start$v1$766f6964202a$696e74(arg0, arg1); \
	__asm__ volatile(".reference " UV_STABILITY); \
} while (0)
#define	UV_TICK_START_ENABLED() \
	({ int _r = __dtrace_isenabled$uv$tick__start$v1(); \
		__asm__ volatile(""); \
		_r; })
#define	UV_TICK_STOP(arg0, arg1) \
do { \
	__asm__ volatile(".reference " UV_TYPEDEFS); \
	__dtrace_probe$uv$tick__stop$v1$766f6964202a$696e74(arg0, arg1); \
	__asm__ volatile(".reference " UV_STABILITY); \
} while (0)
#define	UV_TICK_STOP_ENABLED() \
	({ int _r = __dtrace_isenabled$uv$tick__stop$v1(); \
		__asm__ volatile(""); \
		_r; })


extern void __dtrace_probe$uv$tick__start$v1$766f6964202a$696e74(const void *, int);
extern int __dtrace_isenabled$uv$tick__start$v1(void);
extern void __dtrace_probe$uv$tick__stop$v1$766f6964202a$696e74(const void *, int);
extern int __dtrace_isenabled$uv$tick__stop$v1(void);

#else

#define	UV_TICK_START(arg0, arg1) \
do { \
	} while (0)
#define	UV_TICK_START_ENABLED() (0)
#define	UV_TICK_STOP(arg0, arg1) \
do { \
	} while (0)
#define	UV_TICK_STOP_ENABLED() (0)

#endif /* !defined(DTRACE_PROBES_DISABLED) || !DTRACE_PROBES_DISABLED */


#ifdef	__cplusplus
}
#endif

#endif	/* _UV_DTRACE_H */
