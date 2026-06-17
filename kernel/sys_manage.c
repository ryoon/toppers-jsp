/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: sys_manage.c,v 1.8 2003/07/01 13:30:35 hiro Exp $
 */

/*
 *	システム状態管理機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

#ifndef TOPPERS_ROTRDQ_GLOBAL
#define	NUM_BROADCAST	1
#else
#define	NUM_BROADCAST	TNUM_PRCID
#endif

/*
 *  タスクコンテキストでの自タスクに対するマイグレーション処理
 *
 *  CPUロック，移動元と移動先のタスクロックの取得，コンテキスト
 *  を保存したのち，非タスクコンテキストのスタックで来ること． 
 */
#ifdef __migrate_self
#if TNUM_PRCID > 1

void
migrate_self(ID prcid)
{
	TCB		*tcb;
	TPCB	*t_tpcb;
	TPCB	*f_tpcb;

	f_tpcb = get_my_tpcb();
	tcb = f_tpcb->runtsk;

	/* 移動先のプロセッサのPCBを取得 */
	t_tpcb = get_pure_tpcb(INDEX_PRC(prcid));

	if (TSTAT_RUNNABLE(tcb->tstat)) {
		/* レディーキューから外す */
		make_non_runnable(tcb);
		/* pcb の書き換え */
		tcb->tpcb = t_tpcb;
		/* 移行先のプロセッサでmake_runnable する*/
		if (make_runnable(tcb)) {
			reqest_task_event(t_tpcb, IPI_EVENT_DISPATCH);
		}
	}
	else {
		/*
		 * CPUロックかつタスクロックを取得しているため
		 * 他の状態にはならない
		 */
		assert(0);
	}
	x_release_pure_glock();

	/* ディスパッチャーへ */
	exit_and_dispatch();
	assert(0);
}

#endif /* TNUM_PRCID > 1 */
#endif /* ___migrate_self */

/*
 *  タスクの優先順位の回転
 */
#ifdef __rot_rdq

SYSCALL ER
rot_rdq(PRI tskpri)
{
	UINT	pri, idx;
	ER	ercd;
	TPCB	*tpcb;
	UINT	n = NUM_BROADCAST;

	LOG_ROT_RDQ_ENTER(tskpri);
	CHECK_TSKCTX_UNL();
	CHECK_TPRI_SELF(tskpri);

	t_acquire_glock();
	pri = (tskpri == TPRI_SELF) ? get_my_tpcb()->runtsk->priority : INT_PRIORITY(tskpri);
	idx = get_prc_index();
	do{
		tpcb = get_pure_tpcb(idx);
		if (rotate_ready_queue(pri, tpcb)) {
			multi_core_dispatch(tpcb);
		}
		if (++idx >= TNUM_PRCID) {
			idx = 0;
		}
	}while(--n > 0);
	ercd = E_OK;
	t_release_glock();

    exit:
	LOG_ROT_RDQ_LEAVE(ercd);
	return(ercd);
}

#endif /* __rot_rdq */

/*
 *  タスクの優先順位の回転（非タスクコンテキスト用）
 */
#ifdef __irot_rdq

SYSCALL ER
irot_rdq(PRI tskpri)
{
	UINT	idx;
	ER	ercd;
	TPCB	*tpcb;
	UINT	n = NUM_BROADCAST;

	LOG_IROT_RDQ_ENTER(tskpri);
	CHECK_INTCTX_UNL();
	CHECK_TPRI(tskpri);

	i_acquire_glock();
	idx = get_prc_index();
	do{
		tpcb = get_pure_tpcb(idx);
		if (rotate_ready_queue(INT_PRIORITY(tskpri), tpcb)) {
			reqest_task_event(tpcb, IPI_EVENT_DISPATCH);
		}
		if (++idx >= TNUM_PRCID) {
			idx = 0;
		}
	}while(--n > 0);
	ercd = E_OK;
	i_release_glock();

    exit:
	LOG_IROT_RDQ_LEAVE(ercd);
	return(ercd);
}

#endif /* __irot_rdq */

/*
 *  実行状態のタスクIDの参照
 */
#ifdef __get_tid

SYSCALL ER
get_tid(ID *p_tskid)
{
	TPCB	*my_tpcb;
	ER	ercd;

	LOG_GET_TID_ENTER(p_tskid);
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	my_tpcb = get_my_tpcb();
	*p_tskid = TSKID(my_tpcb->runtsk);
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	LOG_GET_TID_LEAVE(ercd, *p_tskid);
	return(ercd);
}

#endif /* __get_tid */

/*
 *  実行状態のタスクIDの参照（非タスクコンテキスト用）
 */
#ifdef __iget_tid

SYSCALL ER
iget_tid(ID *p_tskid)
{
	TPCB	*my_tpcb;
	ER	ercd;

	LOG_IGET_TID_ENTER(p_tskid);
	CHECK_INTCTX_UNL();

	i_lock_cpu();
	my_tpcb = get_my_tpcb();
	*p_tskid = (my_tpcb->runtsk == NULL) ? TSK_NONE : TSKID(my_tpcb->runtsk);
	ercd = E_OK;
	i_unlock_cpu();

    exit:
	LOG_IGET_TID_LEAVE(ercd, *p_tskid);
	return(ercd);
}

#endif /* __iget_tid */

/*
 *  (休止状態の)タスクのプロセッサの変更
 */
#ifdef __chg_pid

ER
chg_pid(ID tskid, ID prcid)
{
	TCB	*tcb;
	TPCB	*t_tpcb, *f_tpcb, *my_tpcb;
	ER	ercd;

	LOG_CHG_PID_ENTER(tskid, prcid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	CHECK_PRCID_INI(prcid);

	t_acquire_glock();
	tcb = get_tcb_self(tskid, get_my_tpcb());
	prcid = (prcid == TPRC_INI) ? GET_INI_PRCID(tcb->tinib->tskatr) : prcid;
	t_tpcb = get_pure_tpcb(INDEX_PRC(prcid));
	f_tpcb = tcb->tpcb;
	my_tpcb = get_my_tpcb();
	if (TSTAT_RUNNABLE(tcb->tstat)) {
#ifdef TOPPERS_CHGPID_MIGATE
		if (f_tpcb != my_tpcb) {
			/*
			 * 自タスクと同じプロセッサに割り付けられているタスクでなけれ
			 * ばエラー． mig_tsk を呼び出したタスクがシステムコール呼出し後，
			 * マイグレートされた場合にも，ここでエラーとなる
			 */
			ercd = E_OBJ;
		}
		/* 実行可能状態 */
		else if(tcb == my_tpcb->runtsk) {
			/* 自タスクに対して発行 */
			if (!(my_tpcb->enadsp)) {
				/* ディスパッチ禁止中ならエラー */
				ercd = E_CTX;
			}
			else if (t_tpcb == my_tpcb) {
				ercd = E_OK;
			}
			else {
#if TNUM_PRCID > 1
				/* マイグレーション要求を処理 */
				x_release_pure_glock();
				dispatch_and_migrate(prcid);
				/* ここに戻ってくる時にはロックは解放されている */
				t_acquire_glock();
#endif /* TNUM_PRCID > 1 */
				ercd = E_OK;
			}
		} 
		else {
			/* 他タスクの場合 */
			if (t_tpcb == my_tpcb) {
				ercd = E_OK; 
			}
			else {
				/* 異なるプロセッサを指定 */
				/* レディーキューから外す */
				make_non_runnable(tcb);
				/* pcb の書き換え */
				tcb->tpcb = t_tpcb;
				/* 移行先のプロセッサでmake_runnable する*/
				if (make_runnable(tcb)) {
					reqest_task_event(t_tpcb, IPI_EVENT_DISPATCH);
				}
				ercd = E_OK;
			}
		}
#else /* TOPPERS_CHGPID_MIGATE */
		ercd = E_OBJ;
#endif /* TOPPERS_CHGPID_MIGATE */
	}
	else if (TSTAT_DORMANT(tcb->tstat)) {
		/* 休止状態 */
		tcb->tpcb = t_tpcb;
		ercd = E_OK;
	}
	else {
		/* 待ち状態 */
#ifdef TOPPERS_CHGPID_MIGATE
#ifndef TOPPERS_SYSTIM_GLOBAL
		/*
		 *  ローカルタイマ方式で時間待ちの場合
		 */
		if (TSTAT_WAITING(tcb->tstat) && tcb->winfo->tmevtb != NULL) {
			/*
			 * 時間待ちの場合 グローバルタイマ方式 なら必要なし
			 */
			TMEVTB	*tmevtb = tcb->winfo->tmevtb;
			EVTTIM	time;
			RELTIM	left_time;
			/* キューから削除 */
			time = TMEVT_NODE(f_tpcb->tevtcb, tmevtb->index).time;
			left_time = (RELTIM)(time - f_tpcb->tevtcb->current_time);
			tmevtb_dequeue(f_tpcb->tevtcb, tmevtb);
			/* 移動先のプロセッサのキューに挿入 */
			tmevtb_insert(t_tpcb->tevtcb, tmevtb, base_time(t_tpcb->tevtcb) + left_time);
		}
#endif	/* TOPPERS_SYSTIM_GLOBAL */
		tcb->tpcb = t_tpcb;
		ercd = E_OK;
#else /* TOPPERS_CHGPID_MIGATE */
		ercd = E_OBJ;
#endif /* TOPPERS_CHGPID_MIGATE */
	}
	t_release_glock();

	exit:
	LOG_CHG_PID_LEAVE(ercd);
	return(ercd);
}

#endif /* __chg_pid */

/*
 *  実行状態のプロセッサIDの参照
 */
#ifdef __get_pid

ER
get_pid(ID tskid, ID *p_prcid)
{
	TCB	*tcb;
	ER	ercd;

	LOG_GET_PID_ENTER(tskid, p_prcid);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid, get_my_tpcb());
	*p_prcid = tcb->tpcb->prcid;
	ercd = E_OK;

	exit:
	LOG_GET_PID_LEAVE(ercd, *p_prcid);
	return(ercd);
}

#endif /* __get_pid */

/*
 *  実行状態のプロセッサIDの参照（非タスクコンテキスト用）
 */
#ifdef __iget_pid

ER_ID
iget_pid(ID *p_prcid)
{
	ER_ID	ercd;

	LOG_IGET_PID_ENTER(p_prcid);
	CHECK_INTCTX_UNL();

	*p_prcid = get_my_tpcb()->prcid;
	ercd = E_OK;

	exit:
	LOG_IGET_PID_LEAVE(ercd, *p_prcid);
	return(ercd);
}

#endif /* __iget_pid */

/*
 *  CPUロック状態への移行
 */
#ifdef __loc_cpu

SYSCALL ER
loc_cpu(void)
{
	ER	ercd;

	LOG_LOC_CPU_ENTER();
	CHECK_TSKCTX();

	if (!(t_sense_lock())) {
		t_lock_cpu();
	}
	ercd = E_OK;

    exit:
	LOG_LOC_CPU_LEAVE(ercd);
	return(ercd);
}

#endif /* __loc_cpu */

/*
 *  CPUロック状態への移行（非タスクコンテキスト用）
 */
#ifdef __iloc_cpu

SYSCALL ER
iloc_cpu(void)
{
	ER	ercd;

	LOG_ILOC_CPU_ENTER();
	CHECK_INTCTX();

	if (!(i_sense_lock())) {
		i_lock_cpu();
	}
	ercd = E_OK;

    exit:
	LOG_ILOC_CPU_LEAVE(ercd);
	return(ercd);
}

#endif /* __iloc_cpu */

/*
 *  CPUロック状態の解除
 *
 *  CPUロック中は，ディスパッチが必要となるサービスコールを呼び出すこ
 *  とはできないため，CPUロック状態の解除時にディスパッチャを起動する
 *  必要はない．
 */
#ifdef __unl_cpu

SYSCALL ER
unl_cpu(void)
{
	ER	ercd;

	LOG_UNL_CPU_ENTER();
	CHECK_TSKCTX();

	if (t_sense_lock() && ((get_my_tpcb())->locspnid == 0)) {
		t_unlock_cpu();
	}
	ercd = E_OK;

    exit:
	LOG_UNL_CPU_LEAVE(ercd);
	return(ercd);
}

#endif /* __unl_cpu */

/*
 *  CPUロック状態の解除（非タスクコンテキスト用）
 *
 *  CPUロック中は，ディスパッチが必要となるサービスコールを呼び出すこ
 *  とはできないため，CPUロック状態の解除時にディスパッチャの起動を要
 *  求する必要はない．
 */
#ifdef __iunl_cpu

SYSCALL ER
iunl_cpu(void)
{
	ER	ercd;

	LOG_IUNL_CPU_ENTER();
	CHECK_INTCTX();

	if (i_sense_lock() && ((get_my_tpcb())->locspnid == 0)) {
		i_unlock_cpu();
	}
	ercd = E_OK;

    exit:
	LOG_IUNL_CPU_LEAVE(ercd);
	return(ercd);
}

#endif /* __iunl_cpu */

/*
 *  ディスパッチの禁止
 */
#ifdef __dis_dsp

SYSCALL ER
dis_dsp(void)
{
	TPCB	*my_tpcb;
	ER	ercd;

	LOG_DIS_DSP_ENTER();
	CHECK_TSKCTX_UNL();

	t_acquire_glock();
	my_tpcb->enadsp = FALSE;
	ercd = E_OK;
	t_release_glock();

    exit:
	LOG_DIS_DSP_LEAVE(ercd);
	return(ercd);
}

#endif /* __dis_dsp */

/*
 *  ディスパッチの許可
 */
#ifdef __ena_dsp

SYSCALL ER
ena_dsp(void)
{
	TPCB	*my_tpcb;
	ER	ercd;

	LOG_ENA_DSP_ENTER();
	CHECK_TSKCTX_UNL();

	t_acquire_glock();
	my_tpcb = get_my_tpcb();
	my_tpcb->enadsp = TRUE;
	if (my_tpcb->runtsk != my_tpcb->schedtsk) {
		multi_core_dispatch(my_tpcb);
	}
	ercd = E_OK;
	t_release_glock();

    exit:
	LOG_ENA_DSP_LEAVE(ercd);
	return(ercd);
}

#endif /* __ena_dsp */

/*
 *  コンテキストの参照
 */
#ifdef __sns_ctx

SYSCALL BOOL
sns_ctx(void)
{
	BOOL	state;

	LOG_SNS_CTX_ENTER();
	state = sense_context() ? TRUE : FALSE;
	LOG_SNS_CTX_LEAVE(state);
	return(state);
}

#endif /* __sns_ctx */

/*
 *  CPUロック状態の参照
 */
#ifdef __sns_loc

SYSCALL BOOL
sns_loc(void)
{
	BOOL	state;

	LOG_SNS_LOC_ENTER();
	state = sense_lock() ? TRUE : FALSE;
	LOG_SNS_LOC_LEAVE(state);
	return(state);
}

#endif /* __sns_loc */

/*
 *  ディスパッチ禁止状態の参照
 */
#ifdef __sns_dsp

SYSCALL BOOL
sns_dsp(void)
{
	BOOL	state;

	LOG_SNS_DSP_ENTER();
	state = !(get_enadsp()) ? TRUE : FALSE;
	LOG_SNS_DSP_LEAVE(state);
	return(state);
}

#endif /* __sns_dsp */

/*
 *  ディスパッチ保留状態の参照
 */
#ifdef __sns_dpn

SYSCALL BOOL
sns_dpn(void)
{
	TPCB	*my_tpcb;
	BOOL	state;

	LOG_SNS_DPN_ENTER();
	my_tpcb = get_my_tpcb();
	state = (sense_context() || sense_lock() || !(my_tpcb->enadsp)) ? TRUE : FALSE;
	LOG_SNS_DPN_LEAVE(state);
	return(state);
}

#endif /* __sns_dpn */

/*
 *  カーネル動作状態の参照
 */
#ifdef __vsns_ini

SYSCALL BOOL
vsns_ini(void)
{
	BOOL	state;

	LOG_VSNS_INI_ENTER();
	state = !(iniflg) ? TRUE : FALSE;
	LOG_VSNS_INI_LEAVE(state);
	return(state);
}

#endif /* __vsns_ini */
