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
 *  @(#) $Id: semaphore.c,v 1.2 2001/09/06 12:39:51 hiro Exp $
 */

/*
 *	セマフォ機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "semaphore.h"

/*
 *  セマフォIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_semid;

/*
 *  セマフォ初期化ブロックのエリア（kernel_cfg.c）
 */
extern const SEMINIB	seminib_table[];

/*
 *  セマフォ管理ブロックのエリア（kernel_cfg.c）
 */
extern SEMCB	semcb_table[];

/*
 *  セマフォIDからセマフォ管理ブロックを取り出すためのマクロ
 */
#define INDEX_SEM(semid)	((semid) - TMIN_SEMID)
#define get_semcb(semid)	(&(semcb_table[INDEX_SEM(semid)]))

/* 
 *  セマフォ機能の初期化
 */
void
semaphore_initialize()
{
	INT	i;
	SEMCB	*semcb;

	for (semcb = semcb_table, i = 0; i < tmax_semid; semcb++, i++) {
		queue_initialize(&(semcb->wait_queue));
		semcb->seminib = &(seminib_table[i]);
		semcb->semcnt = semcb->seminib->isemcnt;
	}
}

/*
 *  セマフォ資源の返却
 */
SYSCALL ER
sig_sem(ID semid)
{
	SEMCB	*semcb;
	TCB	*tcb;
	ER	ercd;
    
	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	semcb = get_semcb(semid);

	t_lock_cpu();
	if (!(queue_empty(&(semcb->wait_queue)))) {
		tcb = (TCB *) queue_delete_next(&(semcb->wait_queue));
		if (wait_complete(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (semcb->semcnt < semcb->seminib->maxsem) {
		semcb->semcnt += 1;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  セマフォ資源の返却（非タスクコンテキスト用）
 */
SYSCALL ER
isig_sem(ID semid)
{
	SEMCB	*semcb;
	TCB	*tcb;
	ER	ercd;
    
	CHECK_INTCTX_UNL();
	CHECK_SEMID(semid);
	semcb = get_semcb(semid);

	i_lock_cpu();
	if (!queue_empty(&(semcb->wait_queue))) {
		tcb = (TCB *) queue_delete_next(&(semcb->wait_queue));
		if (wait_complete(tcb)) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	else if (semcb->semcnt < semcb->seminib->maxsem) {
		semcb->semcnt += 1;
		ercd = E_OK;
	}
	else {
		ercd = E_QOVR;
	}
	i_unlock_cpu();
	return(ercd);
}

/*
 *  セマフォ資源の獲得
 */
SYSCALL ER
wai_sem(ID semid)
{
	SEMCB	*semcb;
	WINFO_WOBJ winfo;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_SEMID(semid);
	semcb = get_semcb(semid);

	t_lock_cpu();
	if (semcb->semcnt >= 1) {
		semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else {
		wobj_make_wait((WOBJCB *) semcb, &winfo);
		dispatch();
		ercd = winfo.winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  セマフォ資源の獲得（ポーリング）
 */
SYSCALL ER
pol_sem(ID semid)
{
	SEMCB	*semcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	semcb = get_semcb(semid);

	t_lock_cpu();
	if (semcb->semcnt >= 1) {
		semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  セマフォ資源の獲得（タイムアウトあり）
 */
SYSCALL ER
twai_sem(ID semid, TMO tmout)
{
	SEMCB	*semcb;
	WINFO_WOBJ winfo;
	TMEVTB	tmevtb;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_SEMID(semid);
	CHECK_TMOUT(tmout);
	semcb = get_semcb(semid);

	t_lock_cpu();
	if (semcb->semcnt >= 1) {
		semcb->semcnt -= 1;
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		wobj_make_wait_tmout((WOBJCB *) semcb, &winfo, &tmevtb, tmout);
		dispatch();
		ercd = winfo.winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}
