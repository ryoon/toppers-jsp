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
 *  @(#) $Id: cyclic.c,v 1.5 2001/09/13 15:20:43 hiro Exp $
 */

/*
 *	周期ハンドラ機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "cyclic.h"

/*
 *  周期ハンドラIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_cycid;

/*
 *  周期ハンドラ初期化ブロックのエリア（kernel_cfg.c）
 */
extern const CYCINIB	cycinib_table[];

/*
 *  周期ハンドラ管理ブロックのエリア（kernel_cfg.c）
 */
extern CYCCB	cyccb_table[];

/*
 *  周期ハンドラIDから周期ハンドラ管理ブロックを取り出すためのマクロ
 */
#define INDEX_CYC(cycid)	((cycid) - TMIN_CYCID)
#define get_cyccb(cycid)	(&(cyccb_table[INDEX_CYC(cycid)]))

/*
 *  周期ハンドラ起動ルーチン (前方参照のための定義)
 */
static void	call_cychdr(CYCCB *cyccb);

/*
 *  周期ハンドラ起動のためのタイムイベントブロックの登録
 */
static void
tmevtb_enqueue_cyc(CYCCB *cyccb, EVTTIM evttim)
{
	tmevtb_enqueue_evttim(&(cyccb->tmevtb), evttim,
				(CBACK) call_cychdr, (VP) cyccb);
	cyccb->evttim = evttim;
}

/*
 *  周期ハンドラ機能の初期化
 */
void
cyclic_initialize()
{
	INT	i;
	CYCCB	*cyccb;

	for (cyccb = cyccb_table, i = 0; i < tmax_cycid; cyccb++, i++) {
		cyccb->cycinib = &(cycinib_table[i]);
		if ((cyccb->cycinib->cycatr & TA_STA) != 0) {
			cyccb->cycsta = TRUE;
			tmevtb_enqueue_cyc(cyccb, cyccb->cycinib->cycphs);
		}
		else {
			cyccb->cycsta = FALSE;
		}
	}
}

/*
 *  周期ハンドラの動作開始
 */
SYSCALL ER
sta_cyc(ID cycid)
{
	CYCCB	*cyccb;

	CHECK_TSKCTX_UNL();
	CHECK_CYCID(cycid);
	cyccb = get_cyccb(cycid);

	t_lock_cpu();
	if (cyccb->cycsta) {
		tmevtb_dequeue(&(cyccb->tmevtb));
	}
	else {
		cyccb->cycsta = TRUE;
	}
	tmevtb_enqueue_cyc(cyccb, base_time + cyccb->cycinib->cyctim);
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  周期ハンドラの動作停止
 */
SYSCALL ER
stp_cyc(ID cycid)
{
	CYCCB	*cyccb;

	CHECK_TSKCTX_UNL();
	CHECK_CYCID(cycid);
	cyccb = get_cyccb(cycid);

	t_lock_cpu();
	if (cyccb->cycsta) {
		cyccb->cycsta = FALSE;
		tmevtb_dequeue(&(cyccb->tmevtb));
	}
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  周期ハンドラ起動ルーチン
 */
static void
call_cychdr(CYCCB *cyccb)
{
	/*
	 *  次回の起動のためのタイムイベントブロックを登録する．
	 *
	 *  同じタイムティックで周期ハンドラを再度起動すべき場合には，
	 *  この関数から isig_tim に戻った後に，再度この関数が呼ばれる
	 *  ことになる．
	 */
	tmevtb_enqueue_cyc(cyccb, cyccb->evttim + cyccb->cycinib->cyctim);

	/*
	 *  周期ハンドラを，CPUロック解除状態で呼び出す．
	 */
	i_unlock_cpu();
	(*cyccb->cycinib->cychdr)(cyccb->cycinib->exinf);
	i_lock_cpu();
}
