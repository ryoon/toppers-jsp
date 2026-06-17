/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H
#define _CPU_RENAME_H

#define activate_r					_kernel_activate_r
#define ret_int						_kernel_ret_int
#define ret_exc						_kernel_ret_exc
#define handle_trap					_kernel_handle_trap
#define trap_entry					_kernel_trap_entry
#define trap_nest					_kernel_trap_nest
#define m_interrupt_handlers		_kernel_m_interrupt_handlers
#define x_config_int				_kernel_x_config_int
#define lock_flag					_kernel_lock_flag
#define inest_lvl					_kernel_inest_lvl
#define kernel_mie					_kernel_kernel_mie

#ifdef LABEL_ASM

#define _activate_r					__kernel_activate_r
#define _ret_int					__kernel_ret_int
#define _ret_exc					__kernel_ret_exc
#define _handle_trap				__kernel_handle_trap
#define _trap_entry					__kernel_trap_entry
#define _trap_nest					__kernel_trap_nest
#define _m_interrupt_handlers		__kernel_m_interrupt_handlers
#define _x_config_int				__kernel_x_config_int
#define _lock_flag					__kernel_lock_flag
#define _inest_lvl					__kernel_inest_lvl
#define _kernel_mie					__kernel_kernel_mie

#endif /* LABEL_ASM */


#endif /* _CPU_RENAME_H */
