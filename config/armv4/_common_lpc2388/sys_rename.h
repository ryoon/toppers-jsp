/* This file is generated from sys_rename.def by genrename. */

#ifndef _SYS_RENAME_H_
#define _SYS_RENAME_H_

#define IRQ_Handler		_kernel_IRQ_Handler
#define init_vector		_kernel_init_vector
#define init_IRQ		_kernel_init_IRQ
#define init_uart0		_kernel_init_uart0
#define vector_table		_kernel_vector_table
#define undef_interrupt		_kernel_undef_interrupt
#define swi_exception		_kernel_swi_exception
#define irq_exception		_kernel_irq_exception
#define frq_exception		_kernel_frq_exception
#define data_abort_exception	_kernel_data_abort_exception
#define prefetch_exception	_kernel_prefetch_exception

#ifdef LABEL_ASM

#define _IRQ_Handler		__kernel_IRQ_Handler
#define _init_vector		__kernel_init_vector
#define _init_IRQ		__kernel_init_IRQ
#define _init_uart0		__kernel_init_uart0
#define _vector_table		__kernel_vector_table
#define _undef_interrupt	__kernel_undef_interrupt
#define _swi_exception		__kernel_swi_exception
#define _irq_exception		__kernel_irq_exception
#define _frq_exception		__kernel_frq_exception
#define _data_abort_exception	__kernel_data_abort_exception
#define _prefetch_exception	__kernel_prefetch_exception

#endif /* LABEL_ASM */
#endif /* _SYS_RENAME_H_ */
