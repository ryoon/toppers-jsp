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
 *  @(#) $Id: eventflag.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	イベントフラグ機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "eventflag.h"

/*
 *  イベントフラグIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_flgid;

/*
 *  イベントフラグ初期化ブロックのエリア（kernel_cfg.c）
 */
extern const FLGINIB	flginib_table[];

/*
 *  イベントフラグ管理ブロックのエリア（kernel_cfg.c）
 */
extern FLGCB	flgcb_table[];

/*
 *  イベントフラグIDからイベントフラグ管理ブロックを取り出すためのマクロ
 */
#define INDEX_FLG(flgid)	((flgid) - TMIN_FLGID)
#define get_flgcb(flgid)	(&(flgcb_table[INDEX_FLG(flgid)]))

/*
 *  イベントフラグ待ち情報ブロックの定義
 *
 *  flgptn は，waiptn および wfmode と同時に使うことはないため，union 
 *  を使えばメモリを節約することが可能である．
 */
typedef struct eventflag_waiting_information {
	WINFO	winfo;		/* 標準の待ち情報ブロック */
	WOBJCB	*wobjcb;	/* 待ちオブジェクトのコントロールブロック */
	FLGPTN	waiptn;		/* 待ちパターン */
	MODE	wfmode;		/* 待ちモード */
	FLGPTN	flgptn;		/* 待ち解除時のパターン */
} WINFO_FLG;

/*
 *  イベントフラグ機能の初期化
 */
void
eventflag_initialize(void)
{
	INT	i;
	FLGCB	*flgcb;

	for(flgcb = flgcb_table, i = 0; i < tmax_flgid; flgcb++, i++) {
		queue_initialize(&(flgcb->wait_queue));
		flgcb->flginib = &(flginib_table[i]);
		flgcb->flgptn = flgcb->flginib->iflgptn;
	}
}

/*
 *  イベントフラグ待ち解除条件のチェック
 */
static BOOL
eventflag_cond(FLGCB *flgcb, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	if ((wfmode & TWF_ORW) != 0 ? (flgcb->flgptn & waiptn) != 0
				: (flgcb->flgptn & waiptn) == waiptn) {
		*p_flgptn = flgcb->flgptn;
		if ((flgcb->flginib->flgatr & TA_CLR) != 0) {
			flgcb->flgptn = 0;
		}
		return(TRUE);
	}
	return(FALSE);
}

/*
 *  イベントフラグのセット
 */
SYSCALL ER
set_flg(ID flgid, FLGPTN setptn)
{
	FLGCB	*flgcb;
	TCB	*tcb;
	WINFO_FLG *winfo;

	CHECK_TSKCTX_UNL();
	CHECK_FLGID(flgid);
	flgcb = get_flgcb(flgid);

	t_lock_cpu();
	flgcb->flgptn |= setptn;
	if (!(queue_empty(&(flgcb->wait_queue)))) {
		tcb = (TCB *)(flgcb->wait_queue.next);
		winfo = (WINFO_FLG *)(tcb->winfo);
		if (eventflag_cond(flgcb, winfo->waiptn,
					winfo->wfmode, &(winfo->flgptn))) {
			queue_delete(&(tcb->task_queue));
			if (wait_complete(tcb)) {
				dispatch();
			}
		}
	}
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  イベントフラグのセット（非タスクコンテキスト用）
 */
SYSCALL ER
iset_flg(ID flgid, FLGPTN setptn)
{
	FLGCB	*flgcb;
	TCB	*tcb;
	WINFO_FLG *winfo;

	CHECK_INTCTX_UNL();
	CHECK_FLGID(flgid);
	flgcb = get_flgcb(flgid);

	i_lock_cpu();
	flgcb->flgptn |= setptn;
	if (!(queue_empty(&(flgcb->wait_queue)))) {
		tcb = (TCB *)(flgcb->wait_queue.next);
		winfo = (WINFO_FLG *)(tcb->winfo);
		if (eventflag_cond(flgcb, winfo->waiptn,
					winfo->wfmode, &(winfo->flgptn))) {
			queue_delete(&(tcb->task_queue));
			if (wait_complete(tcb)) {
				reqflg = TRUE;
			}
		}
	}
	i_unlock_cpu();
	return(E_OK);
}

/*
 *  イベントフラグのクリア
 */
SYSCALL ER
clr_flg(ID flgid, FLGPTN clrptn)
{
	FLGCB	*flgcb;

	CHECK_TSKCTX_UNL();
	CHECK_FLGID(flgid);
	flgcb = get_flgcb(flgid);

	t_lock_cpu();
	flgcb->flgptn &= clrptn; 
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  イベントフラグ待ち
 */
SYSCALL ER
wai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	FLGCB	*flgcb;
	WINFO_FLG winfo;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_FLGID(flgid);
	CHECK_PAR(waiptn != 0);
	CHECK_PAR((wfmode & ~TWF_ORW) == 0);
	flgcb = get_flgcb(flgid);

	t_lock_cpu();
	if (!(queue_empty(&(flgcb->wait_queue)))) {
		ercd = E_ILUSE;
	}
	else if (eventflag_cond(flgcb, waiptn, wfmode, p_flgptn)) {
		ercd = E_OK;
	}
	else {
		winfo.waiptn = waiptn;
		winfo.wfmode = wfmode;
		wobj_make_wait((WOBJCB *) flgcb, (WINFO_WOBJ *) &winfo);
		dispatch();
		*p_flgptn = winfo.flgptn;
		ercd = winfo.winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  イベントフラグ待ち（ポーリング）
 */
SYSCALL ER
pol_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn)
{
	FLGCB	*flgcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_FLGID(flgid);
	CHECK_PAR(waiptn != 0);
	CHECK_PAR((wfmode & ~TWF_ORW) == 0);
	flgcb = get_flgcb(flgid);

	t_lock_cpu();
	if (!(queue_empty(&(flgcb->wait_queue)))) {
		ercd = E_ILUSE;
	}
	else if (eventflag_cond(flgcb, waiptn, wfmode, p_flgptn)) {
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  イベントフラグ待ち（タイムアウトあり）
 */
SYSCALL ER
twai_flg(ID flgid, FLGPTN waiptn, MODE wfmode, FLGPTN *p_flgptn, TMO tmout)
{
	FLGCB	*flgcb;
	WINFO_FLG winfo;
	TMEVTB	tmevtb;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_FLGID(flgid);
	CHECK_PAR(waiptn != 0);
	CHECK_PAR((wfmode & ~TWF_ORW) == 0);
	CHECK_TMOUT(tmout);
	flgcb = get_flgcb(flgid);

	t_lock_cpu();
	if (!(queue_empty(&(flgcb->wait_queue)))) {
		ercd = E_ILUSE;
	}
	else if (eventflag_cond(flgcb, waiptn, wfmode, p_flgptn)) {
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		winfo.waiptn = waiptn;
		winfo.wfmode = wfmode;
		wobj_make_wait_tmout((WOBJCB *) flgcb, (WINFO_WOBJ *) &winfo,
						&tmevtb, tmout);
		dispatch();
		*p_flgptn = winfo.flgptn;
		ercd = winfo.winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}
