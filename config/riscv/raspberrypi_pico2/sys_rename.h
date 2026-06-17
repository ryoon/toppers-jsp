/* This file is generated from sys_rename.def by genrename. */

#ifndef _SYS_RENAME_H
#define _SYS_RENAME_H

#define ivector						_kernel_ivector
#define p_int_table					_kernel_p_int_table
#define default_hazard3_handler		_kernel_default_hazard3_handler
#define sys_putc					_kernel_sys_putc
#define compare_time				_kernel_compare_time
#define molecule					_kernel_molecule
#define interrupt_map				_kernel_interrupt_map

#ifdef LABEL_ASM

#define _ivector					__kernel_ivector
#define _p_int_table				__kernel_p_int_table
#define _default_hazard3_handler	__kernel_default_hazard3_handler
#define _sys_putc					__kernel_sys_putc
#define _compare_time				__kernel_compare_time
#define _molecule					__kernel_molecule
#define _interrupt_map				__kernel_interrupt_map

#endif /* LABEL_ASM */


#endif /* _SYS_RENAME_H */
