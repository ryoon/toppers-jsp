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
 *  @(#) $Id: mempfix.c,v 1.2 2000/12/22 07:53:11 hiro Exp $
 */

/*
 *	固定長メモリプール機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "mempfix.h"

/*
 *  固定長メモリプールIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_mpfid;

/*
 *  固定長メモリプール初期化ブロックのエリア（kernel_cfg.c）
 */
extern const MPFINIB	mpfinib_table[];

/*
 *  固定長メモリプール管理ブロックのエリア（kernel_cfg.c）
 */
extern MPFCB	mpfcb_table[];

/*
 *  固定長メモリプールIDから固定長メモリプール管理ブロックを取り出すた
 *  めのマクロ
 */
#define INDEX_MPF(mpfid)	((mpfid) - TMIN_MPFID)
#define get_mpfcb(mpfid)	(&(mpfcb_table[INDEX_MPF(mpfid)]))

/*
 *  固定長メモリプール待ち情報ブロックの定義
 */
typedef struct fixed_memorypool_waiting_information {
	WINFO	winfo;		/* 標準の待ち情報ブロック */
	WOBJCB	*wobjcb;	/* 待ちオブジェクトのコントロールブロック */
	VP	blk;		/* 獲得したメモリブロック */
} WINFO_MPF;

/* 
 *  固定長メモリプール機能の初期化
 */
void
mempfix_initialize()
{
	INT	i;
	MPFCB	*mpfcb;

	for (mpfcb = mpfcb_table, i = 0; i < tmax_mpfid; mpfcb++, i++) {
		queue_initialize(&(mpfcb->wait_queue));
		mpfcb->mpfinib = &(mpfinib_table[i]);
		mpfcb->unused = mpfcb->mpfinib->mpf;
		mpfcb->freelist = NULL;
	}
}

/*
 *  固定長メモリプールからブロックを獲得
 */
static BOOL
get_block(MPFCB *mpfcb, VP *p_blk)
{
	FREEL	*free;

	if (mpfcb->freelist != NULL) {
		free = mpfcb->freelist;
		mpfcb->freelist = free->next;
		*p_blk = (VP) free;
		return(TRUE);
	}
	else if (mpfcb->unused < mpfcb->mpfinib->limit) {
		*p_blk = mpfcb->unused;
		mpfcb->unused = (VB *)(mpfcb->unused) + mpfcb->mpfinib->blksz;
		return(TRUE);
	}
	return(FALSE);
}

/*
 *  固定長メモリブロックの獲得
 */
SYSCALL ER
get_mpf(ID mpfid, VP *p_blk)
{
	MPFCB	*mpfcb;
	WINFO_MPF winfo;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_MPFID(mpfid);
	mpfcb = get_mpfcb(mpfid);

	t_lock_cpu();
	if (get_block(mpfcb, p_blk)) {
		ercd = E_OK;
	}
	else {
		wobj_make_wait((WOBJCB *) mpfcb, (WINFO_WOBJ *) &winfo);
		dispatch();
		*p_blk = winfo.blk;
		ercd = winfo.winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  固定長メモリブロックの獲得（ポーリング）
 */
SYSCALL ER
pget_mpf(ID mpfid, VP *p_blk)
{
	MPFCB	*mpfcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_MPFID(mpfid);
	mpfcb = get_mpfcb(mpfid);

	t_lock_cpu();
	if (get_block(mpfcb, p_blk)) {
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  固定長メモリブロックの獲得（タイムアウトあり）
 */
SYSCALL ER
tget_mpf(ID mpfid, VP *p_blk, TMO tmout)
{
	MPFCB	*mpfcb;
	WINFO_MPF winfo;
	TMEVTB	tmevtb;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_MPFID(mpfid);
	CHECK_TMOUT(tmout);
	mpfcb = get_mpfcb(mpfid);

	t_lock_cpu();
	if (get_block(mpfcb, p_blk)) {
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		wobj_make_wait_tmout((WOBJCB *) mpfcb, (WINFO_WOBJ *) &winfo,
						&tmevtb, tmout);
		dispatch();
		*p_blk = winfo.blk;
		ercd = winfo.winfo.wercd;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  固定長メモリブロックの返却
 */
SYSCALL ER
rel_mpf(ID mpfid, VP blk)
{
	MPFCB	*mpfcb;
	TCB	*tcb;
	FREEL	*free;
	ER	ercd;
    
	CHECK_TSKCTX_UNL();
	CHECK_MPFID(mpfid);
	mpfcb = get_mpfcb(mpfid);

	t_lock_cpu();
	if (!(mpfcb->mpfinib->mpf <= blk && blk < mpfcb->mpfinib->limit
			&& (((VB *) blk) - ((VB *)(mpfcb->mpfinib->mpf)))
					% mpfcb->mpfinib->blksz == 0)) {
		ercd = E_PAR;
	}
	else if (!(queue_empty(&(mpfcb->wait_queue)))) {
		tcb = (TCB *) queue_delete_next(&(mpfcb->wait_queue));
		((WINFO_MPF *)(tcb->winfo))->blk = blk;
		if (wait_complete(tcb)) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		free = (FREEL *) blk;
		free->next = mpfcb->freelist;
		mpfcb->freelist = free;
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}
