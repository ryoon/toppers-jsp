/* This file is generated from cpu_rename.def by genrename. */

/* This file is included only when cpu_rename.h has been included. */
#ifdef _CPU_RENAME_H
#undef _CPU_RENAME_H

#undef activate_r
#undef ret_int
#undef ret_exc
#undef default_exc_handler
#undef default_undef_handler
#undef default_swi_handler
#undef default_prefetchabort_handler
#undef default_dataabort_handler
#undef spin_lock_error_handler
#undef start
#undef x_config_int
#undef p_exch_table
#undef p_inth_table
#undef p_tspcb_table
#undef vector_table

#ifdef LABEL_ASM

#undef _activate_r
#undef _ret_int
#undef _ret_exc
#undef _default_exc_handler
#undef _default_undef_handler
#undef _default_swi_handler
#undef _default_prefetchabort_handler
#undef _default_dataabort_handler
#undef _spin_lock_error_handler
#undef _start
#undef _x_config_int
#undef _p_exch_table
#undef _p_inth_table
#undef _p_tspcb_table
#undef _vector_table

#endif /* LABEL_ASM */


#endif /* _CPU_RENAME_H */
