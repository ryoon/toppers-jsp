/* This file is generated from cpu_rename.def by genrename. */

/* This file is included only when cpu_rename.h has been included. */
#ifdef _CPU_RENAME_H
#undef _CPU_RENAME_H

#undef activate_r
#undef ret_int
#undef ret_exc
#undef handle_trap
#undef trap_entry
#undef trap_nest
#undef m_interrupt_handlers
#undef x_config_int
#undef spin_lock_error_handler
#undef p_tspcb_table

#ifdef LABEL_ASM

#undef _activate_r
#undef _ret_int
#undef _ret_exc
#undef _handle_trap
#undef _trap_entry
#undef _trap_nest
#undef _m_interrupt_handlers
#undef _x_config_int
#undef _spin_lock_error_handler
#undef _p_tspcb_table

#endif /* LABEL_ASM */


#endif /* _CPU_RENAME_H */
