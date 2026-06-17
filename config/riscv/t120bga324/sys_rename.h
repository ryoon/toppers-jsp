/* This file is generated from target_rename.def by genrename. */

#ifndef _SYS_RENAME_H_
#define _SYS_RENAME_H_

#define vector_table		_kernel_vector_table
#define default_plic_handler	_kernel_default_plic_handler
#define sys_putc			_kernel_sys_putc
#define compare_time		_kernel_compare_time
#define molecule			_kernel_molecule

#ifdef LABEL_ASM

#define _vector_table		__kernel_vector_table
#define _default_plic_handler	__kernel_default_plic_handler
#define _sys_putc			__kernel_sys_putc
#define _compare_time		__kernel_compare_time
#define _molecule			__kernel_molecule

#endif /* LABEL_ASM */
#endif /* _SYS_RENAME_H_ */
