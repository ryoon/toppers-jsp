/* This file is generated from jsp_rename.def by genrename. */

#ifndef _JSP_RENAME_H
#define _JSP_RENAME_H

/*
 *  startup.c
 */
#define system_start				_kernel_system_start
#define iniflg						_kernel_iniflg

/*
 *  banner.c
 */
#define print_banner				_kernel_print_banner

/*
 *  cpu_config.c, cpu_support.S, sys_config.c, sys_support.S
 */
#define dispatch					_kernel_dispatch
#define dispatch_and_migrate		_kernel_dispatch_and_migrate
#define exit_and_dispatch			_kernel_exit_and_dispatch
#define dispatch_and_migratecpu_initialize	_kernel_dispatch_and_migratecpu_initialize
#define cpu_terminate				_kernel_cpu_terminate
#define sys_initialize				_kernel_sys_initialize
#define sys_exit					_kernel_sys_exit
#define sys_putc					_kernel_sys_putc

/*
 *  task.c
 */
#define p_pcb_table					_kernel_p_pcb_table
#define tprc_tpcb					_kernel_tprc_tpcb
#define giant_lock					_kernel_giant_lock
#define task_initialize				_kernel_task_initialize
#define search_schedtsk				_kernel_search_schedtsk
#define make_runnable				_kernel_make_runnable
#define make_non_runnable			_kernel_make_non_runnable
#define make_dormant				_kernel_make_dormant
#define make_active					_kernel_make_active
#define exit_task					_kernel_exit_task
#define change_priority				_kernel_change_priority
#define rotate_ready_queue			_kernel_rotate_ready_queue
#define call_texrtn					_kernel_call_texrtn
#define calltex						_kernel_calltex

/*
 *  wait.c
 */
#define make_wait_tmout				_kernel_make_wait_tmout
#define wait_complete				_kernel_wait_complete
#define wait_tmout					_kernel_wait_tmout
#define wait_tmout_ok				_kernel_wait_tmout_ok
#define wait_cancel					_kernel_wait_cancel
#define wait_release				_kernel_wait_release
#define wobj_make_wait				_kernel_wobj_make_wait
#define wobj_make_wait_tmout		_kernel_wobj_make_wait_tmout
#define wobj_change_priority		_kernel_wobj_change_priority

/*
 *  task_manage.c
 */
#define migrate_self				_kernel_migrate_self

/*
 *  time_event.c
 */
#define p_tevtcb					_kernel_p_tevtcb
#define p_tevtcb_table				_kernel_p_tevtcb_table
#define tmevt_initialize			_kernel_tmevt_initialize
#define tmevt_up					_kernel_tmevt_up
#define tmevt_down					_kernel_tmevt_down
#define tmevtb_insert				_kernel_tmevtb_insert
#define tmevtb_delete				_kernel_tmevtb_delete

/*
 *  syslog.c
 */
#define syslogcb					_kernel_syslogcb
#define syslog_logmask				_kernel_syslog_logmask
#define syslog_lowmask				_kernel_syslog_lowmask
#define syslog_initialize			_kernel_syslog_initialize
#define syslog_terminate			_kernel_syslog_terminate

/*
 *  semaphore.c
 */
#define semaphore_initialize		_kernel_semaphore_initialize

/*
 *  eventflag.c
 */
#define eventflag_initialize		_kernel_eventflag_initialize
#define eventflag_cond				_kernel_eventflag_cond

/*
 *  dataqueue.c
 */
#define dataqueue_initialize		_kernel_dataqueue_initialize
#define enqueue_data				_kernel_enqueue_data
#define force_enqueue_data			_kernel_force_enqueue_data
#define dequeue_data				_kernel_dequeue_data
#define send_data_rwait				_kernel_send_data_rwait
#define receive_data_swait			_kernel_receive_data_swait

/*
 *  mailbox.c
 */
#define mailbox_initialize			_kernel_mailbox_initialize

/*
 *  mempfix.c
 */
#define mempfix_initialize			_kernel_mempfix_initialize
#define mempfix_get_block			_kernel_mempfix_get_block

/*
 *  cyclic.c
 */
#define cyclic_initialize			_kernel_cyclic_initialize
#define tmevtb_enqueue_cyc			_kernel_tmevtb_enqueue_cyc
#define call_cychdr					_kernel_call_cychdr

/*
 *  spinlock.c
 */
#define spinlock_initialize			_kernel_spinlock_initialize
#define get_spncb					_kernel_get_spncb

/*
 *  interrupt.c
 */
#define interrupt_initialize		_kernel_interrupt_initialize
#define configint_initialize		_kernel_configint_initialize

/*
 *  exception.c
 */
#define exception_initialize		_kernel_exception_initialize

/*
 *  kernel_cfg.c
 */
#define object_initialize			_kernel_object_initialize
#define call_inirtn					_kernel_call_inirtn
#define call_terrtn					_kernel_call_terrtn
#define tmax_tskid					_kernel_tmax_tskid
#define tinib_table					_kernel_tinib_table
#define torder_table				_kernel_torder_table
#define tcb_table					_kernel_tcb_table
#define tmax_semid					_kernel_tmax_semid
#define seminib_table				_kernel_seminib_table
#define semcb_table					_kernel_semcb_table
#define tmax_flgid					_kernel_tmax_flgid
#define flginib_table				_kernel_flginib_table
#define flgcb_table					_kernel_flgcb_table
#define tmax_dtqid					_kernel_tmax_dtqid
#define dtqcb_table					_kernel_dtqcb_table
#define dtqinib_table				_kernel_dtqinib_table
#define tmax_mbxid					_kernel_tmax_mbxid
#define mbxcb_table					_kernel_mbxcb_table
#define mbxinib_table				_kernel_mbxinib_table
#define tmax_mpfid					_kernel_tmax_mpfid
#define mpfinib_table				_kernel_mpfinib_table
#define mpfcb_table					_kernel_mpfcb_table
#define tmax_cycid					_kernel_tmax_cycid
#define cycinib_table				_kernel_cycinib_table
#define cyccb_table					_kernel_cyccb_table
#define tnum_inhno					_kernel_tnum_inhno
#define spninib_table				_kernel_spninib_table
#define spncb_table					_kernel_spncb_table
#define tmax_spnid					_kernel_tmax_spnid
#define inhinib_table				_kernel_inhinib_table
#define tnum_excno					_kernel_tnum_excno
#define excinib_table				_kernel_excinib_table
#define tnum_cfgintno				_kernel_tnum_cfgintno
#define cfgintinib_table			_kernel_cfgintinib_table
#define tnum_prcid					_kernel_tnum_prcid
#define tnum_port					_kernel_tnum_port
#define tmevt_heap					_kernel_tmevt_heap

#ifdef LABEL_ASM

/*
 *  startup.c
 */
#define _system_start				__kernel_system_start
#define _iniflg						__kernel_iniflg

/*
 *  banner.c
 */
#define _print_banner				__kernel_print_banner

/*
 *  cpu_config.c, cpu_support.S, sys_config.c, sys_support.S
 */
#define _dispatch					__kernel_dispatch
#define _dispatch_and_migrate		__kernel_dispatch_and_migrate
#define _exit_and_dispatch			__kernel_exit_and_dispatch
#define _dispatch_and_migratecpu_initialize	__kernel_dispatch_and_migratecpu_initialize
#define _cpu_terminate				__kernel_cpu_terminate
#define _sys_initialize				__kernel_sys_initialize
#define _sys_exit					__kernel_sys_exit
#define _sys_putc					__kernel_sys_putc

/*
 *  task.c
 */
#define _p_pcb_table				__kernel_p_pcb_table
#define _tprc_tpcb					__kernel_tprc_tpcb
#define _giant_lock					__kernel_giant_lock
#define _task_initialize			__kernel_task_initialize
#define _search_schedtsk			__kernel_search_schedtsk
#define _make_runnable				__kernel_make_runnable
#define _make_non_runnable			__kernel_make_non_runnable
#define _make_dormant				__kernel_make_dormant
#define _make_active				__kernel_make_active
#define _exit_task					__kernel_exit_task
#define _change_priority			__kernel_change_priority
#define _rotate_ready_queue			__kernel_rotate_ready_queue
#define _call_texrtn				__kernel_call_texrtn
#define _calltex					__kernel_calltex

/*
 *  wait.c
 */
#define _make_wait_tmout			__kernel_make_wait_tmout
#define _wait_complete				__kernel_wait_complete
#define _wait_tmout					__kernel_wait_tmout
#define _wait_tmout_ok				__kernel_wait_tmout_ok
#define _wait_cancel				__kernel_wait_cancel
#define _wait_release				__kernel_wait_release
#define _wobj_make_wait				__kernel_wobj_make_wait
#define _wobj_make_wait_tmout		__kernel_wobj_make_wait_tmout
#define _wobj_change_priority		__kernel_wobj_change_priority

/*
 *  task_manage.c
 */
#define _migrate_self				__kernel_migrate_self

/*
 *  time_event.c
 */
#define _p_tevtcb					__kernel_p_tevtcb
#define _p_tevtcb_table				__kernel_p_tevtcb_table
#define _tmevt_initialize			__kernel_tmevt_initialize
#define _tmevt_up					__kernel_tmevt_up
#define _tmevt_down					__kernel_tmevt_down
#define _tmevtb_insert				__kernel_tmevtb_insert
#define _tmevtb_delete				__kernel_tmevtb_delete

/*
 *  syslog.c
 */
#define _syslogcb					__kernel_syslogcb
#define _syslog_logmask				__kernel_syslog_logmask
#define _syslog_lowmask				__kernel_syslog_lowmask
#define _syslog_initialize			__kernel_syslog_initialize
#define _syslog_terminate			__kernel_syslog_terminate

/*
 *  semaphore.c
 */
#define _semaphore_initialize		__kernel_semaphore_initialize

/*
 *  eventflag.c
 */
#define _eventflag_initialize		__kernel_eventflag_initialize
#define _eventflag_cond				__kernel_eventflag_cond

/*
 *  dataqueue.c
 */
#define _dataqueue_initialize		__kernel_dataqueue_initialize
#define _enqueue_data				__kernel_enqueue_data
#define _force_enqueue_data			__kernel_force_enqueue_data
#define _dequeue_data				__kernel_dequeue_data
#define _send_data_rwait			__kernel_send_data_rwait
#define _receive_data_swait			__kernel_receive_data_swait

/*
 *  mailbox.c
 */
#define _mailbox_initialize			__kernel_mailbox_initialize

/*
 *  mempfix.c
 */
#define _mempfix_initialize			__kernel_mempfix_initialize
#define _mempfix_get_block			__kernel_mempfix_get_block

/*
 *  cyclic.c
 */
#define _cyclic_initialize			__kernel_cyclic_initialize
#define _tmevtb_enqueue_cyc			__kernel_tmevtb_enqueue_cyc
#define _call_cychdr				__kernel_call_cychdr

/*
 *  spinlock.c
 */
#define _spinlock_initialize		__kernel_spinlock_initialize
#define _get_spncb					__kernel_get_spncb

/*
 *  interrupt.c
 */
#define _interrupt_initialize		__kernel_interrupt_initialize
#define _configint_initialize		__kernel_configint_initialize

/*
 *  exception.c
 */
#define _exception_initialize		__kernel_exception_initialize

/*
 *  kernel_cfg.c
 */
#define _object_initialize			__kernel_object_initialize
#define _call_inirtn				__kernel_call_inirtn
#define _call_terrtn				__kernel_call_terrtn
#define _tmax_tskid					__kernel_tmax_tskid
#define _tinib_table				__kernel_tinib_table
#define _torder_table				__kernel_torder_table
#define _tcb_table					__kernel_tcb_table
#define _tmax_semid					__kernel_tmax_semid
#define _seminib_table				__kernel_seminib_table
#define _semcb_table				__kernel_semcb_table
#define _tmax_flgid					__kernel_tmax_flgid
#define _flginib_table				__kernel_flginib_table
#define _flgcb_table				__kernel_flgcb_table
#define _tmax_dtqid					__kernel_tmax_dtqid
#define _dtqcb_table				__kernel_dtqcb_table
#define _dtqinib_table				__kernel_dtqinib_table
#define _tmax_mbxid					__kernel_tmax_mbxid
#define _mbxcb_table				__kernel_mbxcb_table
#define _mbxinib_table				__kernel_mbxinib_table
#define _tmax_mpfid					__kernel_tmax_mpfid
#define _mpfinib_table				__kernel_mpfinib_table
#define _mpfcb_table				__kernel_mpfcb_table
#define _tmax_cycid					__kernel_tmax_cycid
#define _cycinib_table				__kernel_cycinib_table
#define _cyccb_table				__kernel_cyccb_table
#define _tnum_inhno					__kernel_tnum_inhno
#define _spninib_table				__kernel_spninib_table
#define _spncb_table				__kernel_spncb_table
#define _tmax_spnid					__kernel_tmax_spnid
#define _inhinib_table				__kernel_inhinib_table
#define _tnum_excno					__kernel_tnum_excno
#define _excinib_table				__kernel_excinib_table
#define _tnum_cfgintno				__kernel_tnum_cfgintno
#define _cfgintinib_table			__kernel_cfgintinib_table
#define _tnum_prcid					__kernel_tnum_prcid
#define _tnum_port					__kernel_tnum_port
#define _tmevt_heap					__kernel_tmevt_heap

#endif /* LABEL_ASM */


#endif /* _JSP_RENAME_H */
