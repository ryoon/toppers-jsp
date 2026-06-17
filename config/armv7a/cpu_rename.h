/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H
#define _CPU_RENAME_H

#define activate_r					_kernel_activate_r
#define ret_int						_kernel_ret_int
#define ret_exc						_kernel_ret_exc
#define default_exc_handler			_kernel_default_exc_handler
#define default_undef_handler		_kernel_default_undef_handler
#define default_swi_handler			_kernel_default_swi_handler
#define default_prefetchabort_handler	_kernel_default_prefetchabort_handler
#define default_dataabort_handler	_kernel_default_dataabort_handler
#define spin_lock_error_handler		_kernel_spin_lock_error_handler
#define start						_kernel_start
#define x_config_int				_kernel_x_config_int
#define p_exch_table				_kernel_p_exch_table
#define p_inth_table				_kernel_p_inth_table
#define p_tspcb_table				_kernel_p_tspcb_table
#define vector_table				_kernel_vector_table

#ifdef LABEL_ASM

#define _activate_r					__kernel_activate_r
#define _ret_int					__kernel_ret_int
#define _ret_exc					__kernel_ret_exc
#define _default_exc_handler		__kernel_default_exc_handler
#define _default_undef_handler		__kernel_default_undef_handler
#define _default_swi_handler		__kernel_default_swi_handler
#define _default_prefetchabort_handler	__kernel_default_prefetchabort_handler
#define _default_dataabort_handler	__kernel_default_dataabort_handler
#define _spin_lock_error_handler	__kernel_spin_lock_error_handler
#define _start						__kernel_start
#define _x_config_int				__kernel_x_config_int
#define _p_exch_table				__kernel_p_exch_table
#define _p_inth_table				__kernel_p_inth_table
#define _p_tspcb_table				__kernel_p_tspcb_table
#define _vector_table				__kernel_vector_table

#endif /* LABEL_ASM */


#endif /* _CPU_RENAME_H */
