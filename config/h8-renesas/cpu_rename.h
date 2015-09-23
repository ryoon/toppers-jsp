/* This file is generated from cpu_rename.def by genrename. */

#ifndef _CPU_RENAME_H_
#define _CPU_RENAME_H_

/*
 * cpu_config.c
 */
#define intnest                 _kernel_intnest
#define iscpulocked             _kernel_iscpulocked
#define task_intmask            _kernel_task_intmask
#define int_intmask             _kernel_int_intmask
#define cpu_experr              _kernel_cpu_experr
#define cpu_putc                _kernel_cpu_putc
/*
 * TIMER_IRC
 */

/*
 * cpu_support.src
 */
#define activate_r              _kernel_activate_r
#define no_reg_exception        _kernel_no_reg_exception
#define current_ccr             _kernel_current_ccr
#define set_ccr                 _kernel_set_ccr
#define disint                  _kernel_disint
#define enaint                  _kernel_enaint

/*
 * start.src
 */
#define start                   _kernel_start
#define start_1                 _kernel_start_1

#ifdef LABEL_ASM

/*
 * cpu_config.c
 */
#define _intnest                __kernel_intnest
#define _iscpulocked            __kernel_iscpulocked
#define _task_intmask           __kernel_task_intmask
#define _int_intmask            __kernel_int_intmask
#define _cpu_experr             __kernel_cpu_experr
#define _cpu_putc               __kernel_cpu_putc
/*
 * TIMER_IRC
 */

/*
 * cpu_support.src
 */
#define _activate_r             __kernel_activate_r
#define _no_reg_exception       __kernel_no_reg_exception
#define _current_ccr            __kernel_current_ccr
#define _set_ccr                __kernel_set_ccr
#define _disint                 __kernel_disint
#define _enaint                 __kernel_enaint

/*
 * start.src
 */
#define _start                  __kernel_start
#define _start_1                __kernel_start_1

#endif /* LABEL_ASM */
#endif /* _CPU_RENAME_H_ */
