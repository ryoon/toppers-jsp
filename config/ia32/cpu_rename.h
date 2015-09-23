/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H_
#define _CPU_RENAME_H_

#define activate_r		_kernel_activate_r
#define define_exc		_kernel_define_exc
#define dispatch_loop		_kernel_dispatch_loop
#define exc_table		_kernel_exc_table
#define exception0		_kernel_exception0
#define exception1		_kernel_exception1
#define exception2		_kernel_exception2
#define exception3		_kernel_exception3
#define exception4		_kernel_exception4
#define exception5		_kernel_exception5
#define exception6		_kernel_exception6
#define exception7		_kernel_exception7
#define exception8		_kernel_exception8
#define exception9		_kernel_exception9
#define exception10		_kernel_exception10
#define exception11		_kernel_exception11
#define exception12		_kernel_exception12
#define exception13		_kernel_exception13
#define exception14		_kernel_exception14
#define exception16		_kernel_exception16
#define exception17		_kernel_exception17
#define exception18		_kernel_exception18
#define exception19		_kernel_exception19
#define nest			_kernel_nest
#define set_gate_descriptor	_kernel_set_gate_descriptor
#define set_segment_descriptor	_kernel_set_segment_descriptor

#ifdef LABEL_ASM

#define _activate_r		__kernel_activate_r
#define _define_exc		__kernel_define_exc
#define _dispatch_loop		__kernel_dispatch_loop
#define _exc_table		__kernel_exc_table
#define _exception0		__kernel_exception0
#define _exception1		__kernel_exception1
#define _exception2		__kernel_exception2
#define _exception3		__kernel_exception3
#define _exception4		__kernel_exception4
#define _exception5		__kernel_exception5
#define _exception6		__kernel_exception6
#define _exception7		__kernel_exception7
#define _exception8		__kernel_exception8
#define _exception9		__kernel_exception9
#define _exception10		__kernel_exception10
#define _exception11		__kernel_exception11
#define _exception12		__kernel_exception12
#define _exception13		__kernel_exception13
#define _exception14		__kernel_exception14
#define _exception16		__kernel_exception16
#define _exception17		__kernel_exception17
#define _exception18		__kernel_exception18
#define _exception19		__kernel_exception19
#define _nest			__kernel_nest
#define _set_gate_descriptor	__kernel_set_gate_descriptor
#define _set_segment_descriptor	__kernel_set_segment_descriptor

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
