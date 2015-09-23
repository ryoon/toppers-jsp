/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: task_sync.c,v 1.3 2002/03/26 08:19:38 hiro Exp $
 */

/*
 *	タスク付属同期機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "wait.h"

/*
 *  起床待ち
 */
SYSCALL ER
slp_tsk()
{
	WINFO	winfo;
	ER	ercd;

	CHECK_DISPATCH();

	t_lock_cpu();
	if (runtsk->wupcnt) {
		runtsk->wupcnt = FALSE;
		ercd = E_OK;
	}
	else {
		runtsk->tstat = (TS_WAITING | TS_WAIT_SLEEP);
		make_wait(&winfo);
		dispatch();
		ercd = winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  起床待ち（タイムアウトあり）
 */
SYSCALL ER
tslp_tsk(TMO tmout)
{
	WINFO	winfo;
	TMEVTB	tmevtb;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_TMOUT(tmout);

	t_lock_cpu();
	if (runtsk->wupcnt) {
		runtsk->wupcnt = FALSE;
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		runtsk->tstat = (TS_WAITING | TS_WAIT_SLEEP);
		make_wait_tmout(&winfo, &tmevtb, tmout);
		dispatch();
		ercd = winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスクの起床
 */
SYSCALL ER
wup_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else if ((tstat & TS_WAIT_SLEEP) != 0) {
		if (wait_complete(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (!(tcb->wupcnt)) {
		tcb->wupcnt = TRUE;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスクの起床（非タスクコンテキスト用）
 */
SYSCALL ER
iwup_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	i_lock_cpu();
	if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else if ((tstat & TS_WAIT_SLEEP) != 0) {
		if (wait_complete(tcb)) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	else if (!(tcb->wupcnt)) {
		tcb->wupcnt = TRUE;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	i_unlock_cpu();
	return(ercd);
}

/*
 *  タスク起床要求のキャンセル
 */
SYSCALL ER_UINT
can_wup(ID tskid)
{
	TCB	*tcb;
	ER_UINT	wupcnt;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (TSTAT_DORMANT(tcb->tstat)) {
		wupcnt = E_OBJ;
	}
	else {
		wupcnt = tcb->wupcnt ? 1 : 0;
		tcb->wupcnt = FALSE;
	}
	t_unlock_cpu();
	return(wupcnt);
}

/*
 *  待ち状態の強制解除
 */
SYSCALL ER
rel_wai(ID tskid)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	t_lock_cpu();
	if (!(TSTAT_WAITING(tcb->tstat))) {
		ercd = E_OBJ;
	}
	else {
		if (wait_release(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  待ち状態の強制解除（非タスクコンテキスト用）
 */
SYSCALL ER
irel_wai(ID tskid)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	i_lock_cpu();
	if (!(TSTAT_WAITING(tcb->tstat))) {
		ercd = E_OBJ;
	}
	else {
		if (wait_release(tcb)) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	i_unlock_cpu();
	return(ercd);
}

/*
 *  強制待ち状態への移行
 */
SYSCALL ER
sus_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (tcb == runtsk && !(enadsp)) {
		ercd = E_CTX;
	}
	else if (TSTAT_DORMANT(tstat = tcb->tstat)) {
		ercd = E_OBJ;
	}
	else if (TSTAT_RUNNABLE(tstat)) {
		/*
		 *  実行できる状態から強制待ち状態への遷移
		 */
		tcb->tstat = TS_SUSPENDED;
		if (make_non_runnable(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (TSTAT_SUSPENDED(tstat)) {
		ercd = E_QOVR;
	}
	else {
		/*
		 *  待ち状態から二重待ち状態への遷移
		 */
		tcb->tstat |= TS_SUSPENDED;
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  強制待ち状態からの再開
 */
SYSCALL ER
rsm_tsk(ID tskid)
{
	TCB	*tcb;
	UINT	tstat;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID(tskid);
	tcb = get_tcb(tskid);

	t_lock_cpu();
	if (!(TSTAT_SUSPENDED(tstat = tcb->tstat))) {
		ercd = E_OBJ;
	}
	else if (!(TSTAT_WAITING(tstat))) {
		/*
		 *  強制待ち状態から実行できる状態への遷移
		 */
		if (make_runnable(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		/*
		 *  二重待ち状態から待ち状態への遷移
		 */
		tcb->tstat &= ~TS_SUSPENDED;
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  強制待ち状態からの強制再開
 *
 *  JSPカーネルでは，frsm_tsk と rsm_tsk は同一の処理となる．
 */
#ifdef LABEL_ALIAS
LABEL_ALIAS(frsm_tsk, rsm_tsk)
#else /* LABEL_ALIAS */

SYSCALL ER
frsm_tsk(ID tskid)
{
	return(rsm_tsk(tskid));
}

#endif /* LABEL_ALIAS */

/*
 *  自タスクの遅延
 */
SYSCALL ER
dly_tsk(RELTIM dlytim)
{
	WINFO	winfo;
	TMEVTB	tmevtb;

	CHECK_DISPATCH();
	CHECK_PAR(dlytim <= TMAX_RELTIM);

	t_lock_cpu();
	runtsk->tstat = TS_WAITING;
	make_non_runnable(runtsk);
	runtsk->winfo = &winfo;
	winfo.tmevtb = &tmevtb;
	tmevtb_enqueue(&tmevtb, dlytim, (CBACK) wait_tmout_ok, (VP) runtsk);
	dispatch();
	t_unlock_cpu();
	return(winfo.wercd);
}
