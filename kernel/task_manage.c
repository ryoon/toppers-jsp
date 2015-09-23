/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: task_manage.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	タスク管理機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "wait.h"

/*
 *  タスクの起動
 */
SYSCALL ER
act_tsk(ID tskid)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (TSTAT_DORMANT(tcb->tstat)) {
		if (make_active(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (!(tcb->actcnt)) {
		tcb->actcnt = TRUE;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスクの起動（非タスクコンテキスト用）
 */
SYSCALL ER
iact_tsk(ID tskid)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	i_lock_cpu();
	if (TSTAT_DORMANT(tcb->tstat)) {
		if (make_active(tcb)) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	else if (!(tcb->actcnt)) {
		tcb->actcnt = TRUE;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	i_unlock_cpu();
	return(ercd);
}

/*
 *  タスク起動要求のキャンセル
 */
SYSCALL ER_UINT
can_act(ID tskid)
{
	TCB	*tcb;
	UINT	actcnt;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	actcnt = tcb->actcnt ? 1 : 0;
	tcb->actcnt = FALSE;
	t_unlock_cpu();
	return(actcnt);
}

/*
 *  自タスクの終了
 */
SYSCALL void
ext_tsk()
{
#ifdef ACTIVATED_STACK_SIZE
	/*
	 *  create_context と activate_context で，使用中のスタック領
	 *  域を破壊しないように，スタック上にダミー領域を確保する．
	 */
	(void) alloca(ACTIVATED_STACK_SIZE);
#endif /* ACTIVATED_STACK_SIZE */

	if (sense_context()) {
		/*
		 *  非タスクコンテキストから ext_tsk が呼ばれた場合，
		 *  強制的にタスクコンテキストに切り換えて実行を継続す
		 *  るが，ターゲットによっては非タスクコンテキスト用の
		 *  スタックにゴミが残る場合がある．
		 */
		syslog(LOG_ALERT, "ext_tsk reports E_CTX.");
	}
	if (t_sense_lock()) {
		/*
		 *  CPUロック状態で ext_tsk が呼ばれた場合は，CPUロッ
		 *  クを解除してからタスクを終了する．実装上は，サービ
		 *  スコール内でのCPUロックを省略すればよいだけ．
		 */
		syslog(LOG_WARNING, "ext_tsk reports E_CTX.");
	}
	else {
		t_lock_cpu();
	}
	if (!(enadsp)) {
		/*
		 *  ディスパッチ禁止状態で ext_tsk が呼ばれた場合は，
		 *  ディスパッチ許可状態にしてからタスクを終了する．
		 */
		syslog(LOG_WARNING, "ext_tsk reports E_CTX.");
		enadsp = TRUE;
	}
	exit_task();
}

/*
 *  タスクの強制終了
 */
SYSCALL ER
ter_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);
	CHECK_NONSELF(tcb);

	t_lock_cpu();
	if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else {
		if (TSTAT_RUNNABLE(tstat)) {
			make_non_runnable(tcb);
		}
		else if (TSTAT_WAITING(tstat)) {
			wait_cancel(tcb);
		}
		make_dormant(tcb);
		if (tcb->actcnt) {
			tcb->actcnt = FALSE;
			if (make_active(tcb)) {
				dispatch();
			}
		}
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスク優先度の変更
 */
SYSCALL ER
chg_pri(ID tskid, PRI tskpri)
{
	TCB	*tcb;
	UINT	newpri;
	UINT	tstat;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	CHECK_TPRI_INI(tskpri);
	tcb = get_tcb_self(tskid);
	newpri = (tskpri == TPRI_INI) ? tcb->tinib->ipriority
					: INT_PRIORITY(tskpri);

	t_lock_cpu();
	if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else if (TSTAT_RUNNABLE(tstat)) {
		if (change_priority(tcb, newpri)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		tcb->priority = newpri;
		if ((tstat & TS_WAIT_WOBJCB) != 0) {
			wobj_change_priority(((WINFO_WOBJ *)(tcb->winfo))
							->wobjcb, tcb);
		}
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスク優先度の参照
 */
SYSCALL ER
get_pri(ID tskid, PRI *p_tskpri)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (TSTAT_DORMANT(tcb->tstat)) {
		ercd = E_OBJ;
	}
	else {
		*p_tskpri = EXT_TSKPRI(tcb->priority);
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}
