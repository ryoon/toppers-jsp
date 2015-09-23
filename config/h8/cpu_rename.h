/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H_
#define _CPU_RENAME_H_

/*
 * cpu_config.c
 */
#define intnest			_kernel_intnest
#define iscpulocked		_kernel_iscpulocked
#define task_intmask		_kernel_task_intmask
#define int_intmask		_kernel_int_intmask
#define cpu_experr		_kernel_cpu_experr

/*
 * cpu_support.S
 */
#define activate_r		_kernel_activate_r
#define no_reg_exception	_kernel_no_reg_exception
#define common_interrupt_process	_kernel_common_interrupt_process

/*
 * vector.S
 */
#define vector			_kernel_vector

#ifdef LABEL_ASM

/*
 * cpu_config.c
 */
#define _intnest		__kernel_intnest
#define _iscpulocked		__kernel_iscpulocked
#define _task_intmask		__kernel_task_intmask
#define _int_intmask		__kernel_int_intmask
#define _cpu_experr		__kernel_cpu_experr

/*
 * cpu_support.S
 */
#define _activate_r		__kernel_activate_r
#define _no_reg_exception	__kernel_no_reg_exception
#define _common_interrupt_process	__kernel_common_interrupt_process

/*
 * vector.S
 */
#define _vector			__kernel_vector

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
