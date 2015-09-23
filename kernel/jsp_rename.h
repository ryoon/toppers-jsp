/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: jsp_rename.h,v 1.7 2001/10/11 18:58:01 honda Exp $
 */

/*
 *	カーネルの内部識別名のリネーム
 */

#ifndef _JSP_RENAME_H_
#define _JSP_RENAME_H_

#ifndef OMIT_RENAME

/*
 *  banner.c
 */
#define print_banner		_kernel_print_banner

/*
 *  cpu_config.c, cpu_support.S, sys_config.c, sys_support.S
 */
#define dispatch		_kernel_dispatch
#define exit_and_dispatch	_kernel_exit_and_dispatch
#define cpu_initialize		_kernel_cpu_initialize
#define cpu_terminate		_kernel_cpu_terminate
#define sys_initialize		_kernel_sys_initialize
#define sys_exit		_kernel_sys_exit
#define sys_putc		_kernel_sys_putc

/*
 *  task.c
 */
#define task_initialize		_kernel_task_initialize
#define make_runnable		_kernel_make_runnable
#define make_non_runnable	_kernel_make_non_runnable
#define make_dormant		_kernel_make_dormant
#define make_active		_kernel_make_active
#define exit_task		_kernel_exit_task
#define change_priority		_kernel_change_priority
#define rotate_ready_queue	_kernel_rotate_ready_queue
#define call_texrtn		_kernel_call_texrtn
#define calltex			_kernel_calltex
#define runtsk			_kernel_runtsk
#define reqflg			_kernel_reqflg
#define schedtsk		_kernel_schedtsk
#define enadsp			_kernel_enadsp

/*
 *  wait.c
 */
#define wait_complete		_kernel_wait_complete
#define wait_tmout		_kernel_wait_tmout
#define wait_tmout_ok		_kernel_wait_tmout_ok
#define wait_release		_kernel_wait_release
#define wobj_make_wait		_kernel_wobj_make_wait
#define wobj_make_wait_tmout	_kernel_wobj_make_wait_tmout
#define wobj_change_priority	_kernel_wobj_change_priority
#define make_wait_tmout		_kernel_make_wait_tmout
#define wait_cancel		_kernel_wait_cancel

/*
 *  time_event.c
 */
#define tmevt_initialize	_kernel_tmevt_initialize
#define tmevtb_insert		_kernel_tmevtb_insert
#define tmevtb_delete		_kernel_tmevtb_delete
#define systim_offset		_kernel_systim_offset
#define current_time		_kernel_current_time
#define next_time		_kernel_next_time

/*
 *  syslog.c
 */
#define syslog_initialize	_kernel_syslog_initialize
#define syslog_terminate	_kernel_syslog_terminate

/*
 *  kernel_cfg.c
 */
#define object_initialize	_kernel_object_initialize
#define call_inirtn		_kernel_call_inirtn
#define tmax_tskid		_kernel_tmax_tskid
#define tinib_table		_kernel_tinib_table
#define tcb_table		_kernel_tcb_table
#define tmax_semid		_kernel_tmax_semid
#define seminib_table		_kernel_seminib_table
#define semcb_table		_kernel_semcb_table
#define tmax_flgid		_kernel_tmax_flgid
#define flginib_table		_kernel_flginib_table
#define flgcb_table		_kernel_flgcb_table
#define tmax_dtqid		_kernel_tmax_dtqid
#define dtqcb_table		_kernel_dtqcb_table
#define dtqinib_table		_kernel_dtqinib_table
#define tmax_mbxid		_kernel_tmax_mbxid
#define mbxcb_table		_kernel_mbxcb_table
#define mbxinib_table		_kernel_mbxinib_table
#define tmax_mpfid		_kernel_tmax_mpfid
#define mpfinib_table		_kernel_mpfinib_table
#define mpfcb_table		_kernel_mpfcb_table
#define tmax_cycid		_kernel_tmax_cycid
#define cycinib_table		_kernel_cycinib_table
#define cyccb_table		_kernel_cyccb_table
#define tnum_inhno		_kernel_tnum_inhno
#define inhinib_table		_kernel_inhinib_table
#define tnum_excno		_kernel_tnum_excno
#define excinib_table		_kernel_excinib_table
#define tmevt_heap		_kernel_tmevt_heap

/*
 *  semaphore.c, eventflag.c, dataqueue.c, mailbox.c
 *  mempfix.c, cyclic.c, interrupt.c, exception.c
 */
#define semaphore_initialize	_kernel_semaphore_initialize
#define eventflag_initialize	_kernel_eventflag_initialize
#define dataqueue_initialize	_kernel_dataqueue_initialize
#define mailbox_initialize	_kernel_mailbox_initialize
#define mempfix_initialize	_kernel_mempfix_initialize
#define cyclic_initialize	_kernel_cyclic_initialize
#define interrupt_initialize	_kernel_interrupt_initialize
#define exception_initialize	_kernel_exception_initialize

/*
 *	カーネルの内部識別名のリネーム（"_" を付けるアセンブラ用）
 */

#ifdef LABEL_ASM

/*
 *  banner.c
 */
#define _print_banner		__kernel_print_banner

/*
 *  cpu_config.c, cpu_support.S, sys_config.c, sys_support.S
 */
#define _dispatch		__kernel_dispatch
#define _exit_and_dispatch	__kernel_exit_and_dispatch
#define _cpu_initialize		__kernel_cpu_initialize
#define _cpu_terminate		__kernel_cpu_terminate
#define _sys_initialize		__kernel_sys_initialize
#define _sys_exit		__kernel_sys_exit
#define _sys_putc		__kernel_sys_putc

/*
 *  task.c
 */
#define _task_initialize	__kernel_task_initialize
#define _make_runnable		__kernel_make_runnable
#define _make_non_runnable	__kernel_make_non_runnable
#define _make_dormant		__kernel_make_dormant
#define _make_active		__kernel_make_active
#define _exit_task		__kernel_exit_task
#define _change_priority	__kernel_change_priority
#define _rotate_ready_queue	__kernel_rotate_ready_queue
#define _call_texrtn		__kernel_call_texrtn
#define _calltex		__kernel_calltex
#define _runtsk			__kernel_runtsk
#define _reqflg			__kernel_reqflg
#define _schedtsk		__kernel_schedtsk
#define _enadsp			__kernel_enadsp

/*
 *  wait.c
 */
#define _wait_complete		__kernel_wait_complete
#define _wait_tmout		__kernel_wait_tmout
#define _wait_tmout_ok		__kernel_wait_tmout_ok
#define _wait_release		__kernel_wait_release
#define _wobj_make_wait		__kernel_wobj_make_wait
#define _wobj_make_wait_tmout	__kernel_wobj_make_wait_tmout
#define _wobj_change_priority	__kernel_wobj_change_priority
#define _make_wait_tmout	__kernel_make_wait_tmout
#define _wait_cancel		__kernel_wait_cancel

/*
 *  time_event.c
 */
#define _tmevt_initialize	__kernel_tmevt_initialize
#define _tmevtb_insert		__kernel_tmevtb_insert
#define _tmevtb_delete		__kernel_tmevtb_delete
#define _systim_offset		__kernel_systim_offset
#define _current_time		__kernel_current_time
#define _next_time		__kernel_next_time

/*
 *  syslog.c
 */
#define _syslog_initialize	__kernel_syslog_initialize
#define _syslog_terminate	__kernel_syslog_terminate

/*
 *  kernel_cfg.c
 */
#define _object_initialize	__kernel_object_initialize
#define _call_inirtn		__kernel_call_inirtn
#define _tmax_tskid		__kernel_tmax_tskid
#define _tinib_table		__kernel_tinib_table
#define _tcb_table		__kernel_tcb_table
#define _tmax_semid		__kernel_tmax_semid
#define _seminib_table		__kernel_seminib_table
#define _semcb_table		__kernel_semcb_table
#define _tmax_flgid		__kernel_tmax_flgid
#define _flginib_table		__kernel_flginib_table
#define _flgcb_table		__kernel_flgcb_table
#define _tmax_dtqid		__kernel_tmax_dtqid
#define _dtqcb_table		__kernel_dtqcb_table
#define _dtqinib_table		__kernel_dtqinib_table
#define _tmax_mbxid		__kernel_tmax_mbxid
#define _mbxcb_table		__kernel_mbxcb_table
#define _mbxinib_table		__kernel_mbxinib_table
#define _tmax_mpfid		__kernel_tmax_mpfid
#define _mpfinib_table		__kernel_mpfinib_table
#define _mpfcb_table		__kernel_mpfcb_table
#define _tmax_cycid		__kernel_tmax_cycid
#define _cycinib_table		__kernel_cycinib_table
#define _cyccb_table		__kernel_cyccb_table
#define _tnum_inhno		__kernel_tnum_inhno
#define _inhinib_table		__kernel_inhinib_table
#define _tnum_excno		__kernel_tnum_excno
#define _excinib_table		__kernel_excinib_table
#define _tmevt_heap		__kernel_tmevt_heap

/*      
 *  semaphore.c, eventflag.c, dataqueue.c, mailbox.c
 *  mempfix.c, cyclic.c, interrupt.c, exception.c
 */
#define _semaphore_initialize	__kernel_semaphore_initialize
#define _eventflag_initialize	__kernel_eventflag_initialize
#define _dataqueue_initialize	__kernel_dataqueue_initialize
#define _mailbox_initialize	__kernel_mailbox_initialize
#define _mempfix_initialize	__kernel_mempfix_initialize
#define _cyclic_initialize	__kernel_cyclic_initialize
#define _interrupt_initialize	__kernel_interrupt_initialize
#define _exception_initialize	__kernel_exception_initialize

#endif /* LABEL_ASM */
#endif /* OMIT_RENAME */
#endif /* _JSP_RENAME_H_ */
