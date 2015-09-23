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
#undef cpu_putc

/*
 * cpu_support.src
 */
#undef activate_r
#undef no_reg_exception
#undef current_ccr
#undef set_ccr
#undef current_exr
#undef set_exr

/*
 * start.src
 */
#undef start

#ifdef LABEL_ASM

/*
 * cpu_config.c
 */
#undef _intnest
#undef _iscpulocked
#undef _task_intmask
#undef _int_intmask
#undef _cpu_experr
#undef _cpu_putc

/*
 * cpu_support.src
 */
#undef _activate_r
#undef _no_reg_exception
#undef _current_ccr
#undef _set_ccr
#undef _current_exr
#undef _set_exr

/*
 * start.src
 */
#undef _start

#endif /* LABEL_ASM */
#endif /* _CPU_UNRENAME_H_ */
