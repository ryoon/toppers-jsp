/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H_
#define _CPU_RENAME_H_

/*
 * cpu_support.S
 */
#define activate_r		_kernel_activate_r
#define ret_int			_kernel_ret_int
#define ret_exc			_kernel_ret_exc
#define no_reg_exception	_kernel_no_reg_exception

/*
 * cpu_config.c
 */
#define task_intmask		_kernel_task_intmask
#define int_intmask		_kernel_int_intmask
#define intnest			_kernel_intnest
#define BASE_VBR		_kernel_BASE_VBR
#define cpu_experr		_kernel_cpu_experr

#ifdef LABEL_ASM

/*
 * cpu_support.S
 */
#define _activate_r		__kernel_activate_r
#define _ret_int		__kernel_ret_int
#define _ret_exc		__kernel_ret_exc
#define _no_reg_exception	__kernel_no_reg_exception

/*
 * cpu_config.c
 */
#define _task_intmask		__kernel_task_intmask
#define _int_intmask		__kernel_int_intmask
#define _intnest		__kernel_intnest
#define _BASE_VBR		__kernel_BASE_VBR
#define _cpu_experr		__kernel_cpu_experr

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
