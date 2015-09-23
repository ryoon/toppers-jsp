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
 *  @(#) $Id: task_except.c,v 1.5 2002/03/26 08:19:38 hiro Exp $
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
	else {
		tcb->texptn |= rasptn;
		if (tcb == runtsk && runtsk->enatex) {
			call_texrtn();
		}
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
		runtsk->enatex = TRUE;
		if (runtsk->texptn != 0) {
			call_texrtn();
		}
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
