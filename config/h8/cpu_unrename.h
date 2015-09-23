/* This file is generated from cpu_rename.def by genrename. */

#ifdef _CPU_UNRENAME_H_
#undef _CPU_UNRENAME_H_

/*
 * cpu_config.c
 */
#undef intnest
#undef iscpulocked
#undef task_intmask
#undef int_intmask
#undef cpu_experr

/*
 * cpu_support.S
 */
#undef activate_r
#undef no_reg_exception
#undef common_interrupt_process

/*
 * vector.S
 */
#undef vector

#ifdef LABEL_ASM

/*
 * cpu_config.c
 */
#undef _intnest
#undef _iscpulocked
#undef _task_intmask
#undef _int_intmask
#undef _cpu_experr

/*
 * cpu_support.S
 */
#undef _activate_r
#undef _no_reg_exception
#undef _common_interrupt_process

/*
 * vector.S
 */
#undef _vector

#endif /* LABEL_ASM */
#endif /* _CPU_UNRENAME_H_ */
