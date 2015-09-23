/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H_
#define _CPU_RENAME_H_

#define activate_r		_kernel_activate_r
#define interrupt_entry		_kernel_interrupt_entry
#define cpu_exception_entry	_kernel_cpu_exception_entry
#define task_intmask		_kernel_task_intmask
#define int_intmask		_kernel_int_intmask
#define intnest			_kernel_intnest

#ifdef LABEL_ASM

#define _activate_r		_kernel_activate_r
#define _interrupt_entry	_kernel_interrupt_entry
#define _cpu_exception_entry	_kernel_cpu_exception_entry
#define _task_intmask		_kernel_task_intmask
#define _int_intmask		_kernel_int_intmask
#define _intnest		_kernel_intnest

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
