/* This file is generated from sys_rename.def by genrename. */

#ifndef _SYS_RENAME_H_
#define _SYS_RENAME_H_

#define sys_initialize \
		_kernel_sys_initialize

#define sys_exit \
		_kernel_sys_exit

#define hardware_init_hook \
	_kernel_hardware_init_hook

#define sys_putc \
		_kernel_sys_putc


#ifdef LABEL_ASM

#define _sys_initialize \
	__kernel_sys_initialize

#define _sys_exit \
		__kernel_sys_exit

#define _hardware_init_hook \
	__kernel_hardware_init_hook

#define _sys_putc \
		__kernel_sys_putc


#endif /* LABEL_ASM */
#endif /* _SYS_RENAME_H_ */
