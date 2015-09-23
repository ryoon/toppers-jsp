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
 *  @(#) $Id: task_except.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	タスク例外処理機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

/*
 *  タスク例外処理の要求
 */
SYSCALL ER
ras_tex(ID tskid, TEXPTN rasptn)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	CHECK_PAR(rasptn != 0);
	tcb = get_tcb_self(tskid);

	t_lock_cpu();
	if (TSTAT_DORMANT(tcb->tstat) || tcb->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else if (tcb == runtsk && runtsk->enatex) {
		/*
		 *  この状態では，runtsk->texptn は 0 になっている．な
		 *  ぜなら，もしそうでなければ，これ以前にタスク例外処
		 *  理ルーチンが呼び出されているはずである．
		 */
		assert(runtsk->texptn == 0);
		runtsk->enatex = FALSE;
		t_unlock_cpu();
		(*runtsk->tinib->texrtn)(rasptn, runtsk->tinib->exinf);
		t_lock_cpu();

		call_texrtn();
		ercd = E_OK;
	}
	else {
		tcb->texptn |= rasptn;
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスク例外処理の要求（非タスクコンテキスト用）
 */
SYSCALL ER
iras_tex(ID tskid, TEXPTN rasptn)
{
	TCB	*tcb;
	ER	ercd;

	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	CHECK_PAR(rasptn != 0);
	tcb = get_tcb(tskid);

	i_lock_cpu();
	if (TSTAT_DORMANT(tcb->tstat) || tcb->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else {
		tcb->texptn |= rasptn;
		if (tcb == runtsk && runtsk->enatex) {
			reqflg = TRUE;
		}
		ercd = E_OK;
	}
	i_unlock_cpu();
	return(ercd);
}

/*
 *  タスク例外処理の禁止
 */
SYSCALL ER
dis_tex()
{
	ER	ercd;

	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	if (runtsk->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else {
		runtsk->enatex = FALSE;
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスク例外処理の許可
 */
SYSCALL ER
ena_tex()
{
	ER	ercd;

	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	if (runtsk->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else {
		/*
		 *  この状態では，runtsk->texptn が 0 か，
		 *  runtsk->enatex が FALSE のいずれかが成り立つ．なぜ
		 *  なら，もしそうでなければ，これ以前にタスク例外処理
		 *  ルーチンが呼び出されているはずである．
		 */
		call_texrtn();
		ercd = E_OK;
	}
	t_unlock_cpu();
	return(ercd);
}

/*
 *  タスク例外処理禁止状態の参照
 */
SYSCALL BOOL
sns_tex()
{
	return((runtsk != NULL && runtsk->enatex) ? FALSE : TRUE);
}
