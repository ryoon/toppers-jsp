/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H
#define _CPU_RENAME_H

#define activate_r					_kernel_activate_r
#define ret_int						_kernel_ret_int
#define ret_exc						_kernel_ret_exc
#define cpu_exc_entry				_kernel_cpu_exc_entry
#define cpu_int_entry				_kernel_cpu_int_entry
#define pendsvc_handler				_kernel_pendsvc_handler
#define start						_kernel_start
#define enable_exc					_kernel_enable_exc
#define disable_exc					_kernel_disable_exc
#define x_config_int				_kernel_x_config_int
#define int_handler_table			_kernel_int_handler_table

#ifdef LABEL_ASM

#define _activate_r					__kernel_activate_r
#define _ret_int					__kernel_ret_int
#define _ret_exc					__kernel_ret_exc
#define _cpu_exc_entry				__kernel_cpu_exc_entry
#define _cpu_int_entry				__kernel_cpu_int_entry
#define _pendsvc_handler			__kernel_pendsvc_handler
#define _start						__kernel_start
#define _enable_exc					__kernel_enable_exc
#define _disable_exc				__kernel_disable_exc
#define _x_config_int				__kernel_x_config_int
#define _int_handler_table			__kernel_int_handler_table

#endif /* LABEL_ASM */


#endif /* _CPU_RENAME_H */
