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
 *  @(#) $Id: wait.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	待ち状態管理モジュール
 */

#include "jsp_kernel.h"
#include "wait.h"

/*
 *  待ち状態への移行（タイムアウト指定）
 *
 */
inline void
make_wait_tmout(WINFO *winfo, TMEVTB *tmevtb, TMO tmout)
{
	make_non_runnable(runtsk);
	runtsk->winfo = winfo;
	if (tmout > 0) {
		winfo->tmevtb = tmevtb;
		tmevtb_enqueue(tmevtb, (RELTIM) tmout,
					(CBACK) wait_tmout, (VP) runtsk);
	}
	else {
		assert(tmout == TMO_FEVR);
		winfo->tmevtb = NULL;
	}
}

/*
 *  待ち解除のためのタスク状態の更新
 *
 *  tcb で指定されるタスクを，待ち解除するようタスク状態を更新する．待
 *  ち解除するタスクが実行できる状態になる場合は，レディキューにつなぐ．
 *  また，ディスパッチが必要な場合には TRUE を返す．
 */
Inline BOOL
make_non_wait(TCB *tcb)
{
	assert(TSTAT_WAITING(tcb->tstat));

	if (!(TSTAT_SUSPENDED(tcb->tstat))) {
		/*
		 *  待ち状態から実行できる状態への遷移
		 */
		return(make_runnable(tcb));
	}
	else {
		/*
		 *  二重待ち状態から強制待ち状態への遷移
		 */
		tcb->tstat = TS_SUSPENDED;
		return(FALSE);
	}
}

/*
 *  待ち解除
 */
BOOL
wait_complete(TCB *tcb)
{
	if (tcb->winfo->tmevtb != NULL) {
		tmevtb_dequeue(tcb->winfo->tmevtb);
	}
	tcb->winfo->wercd = E_OK;
	return(make_non_wait(tcb));
}

/*
 *  タイムアウトに伴う待ち解除
 */
void
wait_tmout(TCB *tcb)
{
	if ((tcb->tstat & TS_WAIT_WOBJ) != 0) {
		queue_delete(&(tcb->task_queue));
	}
	tcb->winfo->wercd = E_TMOUT;
	if (make_non_wait(tcb)) {
		reqflg = TRUE;
	}
}

void
wait_tmout_ok(TCB *tcb)
{
	tcb->winfo->wercd = E_OK;
	if (make_non_wait(tcb)) {
		reqflg = TRUE;
	}
}

/*
 *  待ち状態の強制解除
 */
inline void
wait_cancel(TCB *tcb)
{
	if (tcb->winfo->tmevtb != NULL) {
		tmevtb_dequeue(tcb->winfo->tmevtb);
	}
	if ((tcb->tstat & TS_WAIT_WOBJ) != 0) {
		queue_delete(&(tcb->task_queue));
	}
}

BOOL
wait_release(TCB *tcb)
{
	wait_cancel(tcb);
	tcb->winfo->wercd = E_RLWAI;
	return(make_non_wait(tcb));
}

/*
 *  タスクの優先度順の待ちキューへの挿入
 */
Inline void
queue_insert_tpri(TCB *tcb, QUEUE *queue)
{
	QUEUE	*entry;
	UINT	priority = tcb->priority;

	for (entry = queue->next; entry != queue; entry = entry->next) {
		if (priority < ((TCB *) entry)->priority) {
			break;
		}
	}
	queue_insert_prev(entry, &(tcb->task_queue));
}

/*
 *  実行中のタスクの同期・通信オブジェクトの待ちキューへの挿入
 */
Inline void
wobj_queue_insert(WOBJCB *wobjcb)
{
	if ((wobjcb->wobjinib->wobjatr & TA_TPRI) != 0) {
		queue_insert_tpri(runtsk, &(wobjcb->wait_queue));
	}
	else {
		queue_insert_prev(&(wobjcb->wait_queue),
					&(runtsk->task_queue));
	}
}

/*
 *  同期・通信オブジェクトに対する待ち状態への移行
 */
void
wobj_make_wait(WOBJCB *wobjcb, WINFO_WOBJ *winfo)
{
	runtsk->tstat = (TS_WAITING | TS_WAIT_WOBJ | TS_WAIT_WOBJCB);
	make_wait(&(winfo->winfo));
	wobj_queue_insert(wobjcb);
	winfo->wobjcb = wobjcb;
}

void
wobj_make_wait_tmout(WOBJCB *wobjcb, WINFO_WOBJ *winfo,
					TMEVTB *tmevtb, TMO tmout)
{
	runtsk->tstat = (TS_WAITING | TS_WAIT_WOBJ | TS_WAIT_WOBJCB);
	make_wait_tmout(&(winfo->winfo), tmevtb, tmout);
	wobj_queue_insert(wobjcb);
	winfo->wobjcb = wobjcb;
}

/*
 *  タスクの優先度変更時の処理
 */
void
wobj_change_priority(WOBJCB *wobjcb, TCB *tcb)
{
	if ((wobjcb->wobjinib->wobjatr & TA_TPRI) != 0) {
		queue_delete(&(tcb->task_queue));
		queue_insert_tpri(tcb, &(wobjcb->wait_queue));
	}
}
