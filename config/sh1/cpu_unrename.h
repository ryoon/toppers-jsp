/* This file is generated from cpu_rename.def by genrename. */

#ifdef _CPU_RENAME_H_
#undef _CPU_RENAME_H_

/*
 * cpu_support.S
 */
#undef activate_r
#undef ret_int
#undef ret_exc
#undef no_reg_exception

/*
 * cpu_config.c
 */
#undef task_intmask
#undef int_intmask
#undef intnest
#undef BASE_VBR
#undef cpu_experr

#ifdef LABEL_ASM

/*
 * cpu_support.S
 */
#undef _activate_r
#undef _ret_int
#undef _ret_exc
#undef _no_reg_exception

/*
 * cpu_config.c
 */
#undef _task_intmask
#undef _int_intmask
#undef _intnest
#undef _BASE_VBR
#undef _cpu_experr

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
