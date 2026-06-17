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
 *  @(#) $Id: task_except.c,v 1.8 2003/06/04 01:46:16 hiro Exp $
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
#ifdef __ras_tex

SYSCALL ER
ras_tex(ID tskid, TEXPTN rasptn)
{
	TCB	*tcb;
	ER	ercd;
	TPCB	*tpcb;

	LOG_RAS_TEX_ENTER(tskid, rasptn);
	CHECK_TSKCTX_UNL();
	CHECK_TSKID_SELF(tskid);
	CHECK_PAR(rasptn != 0);
	tcb = get_tcb_self(tskid, get_my_tpcb());

	t_acquire_glock();
	tpcb = tcb->tpcb;
	if (TSTAT_DORMANT(tcb->tstat) || tcb->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else {
		tcb->texptn |= rasptn;
		if (reqest_task_event(tcb->tpcb, IPI_EVENT_TEXCEPT)) {
			if (tcb == tpcb->runtsk && tpcb->runtsk->enatex) {
				call_texrtn();
				tpcb = get_my_tpcb();
			}
		}
		ercd = E_OK;
	}
	t_release_glock();

    exit:
	LOG_RAS_TEX_LEAVE(ercd);
	return(ercd);
}

#endif /* __ras_tex */

/*
 *  タスク例外処理の要求（非タスクコンテキスト用）
 */
#ifdef __iras_tex

SYSCALL ER
iras_tex(ID tskid, TEXPTN rasptn)
{
	TCB	*tcb;
	ER	ercd;

	LOG_IRAS_TEX_ENTER(tskid, rasptn);
	CHECK_INTCTX_UNL();
	CHECK_TSKID(tskid);
	CHECK_PAR(rasptn != 0);
	tcb = get_tcb(tskid);

	i_acquire_glock();
	if (TSTAT_DORMANT(tcb->tstat) || tcb->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else {
		TPCB *tpcb = tcb->tpcb;
		tcb->texptn |= rasptn;
		if (tcb == tpcb->runtsk && tpcb->runtsk->enatex) {
			reqest_task_event(tcb->tpcb, IPI_EVENT_DISPATCH);
		}
		ercd = E_OK;
	}
	i_release_glock();

    exit:
	LOG_IRAS_TEX_LEAVE(ercd);
	return(ercd);
}

#endif /* __iras_tex */

/*
 *  タスク例外処理の禁止
 */
#ifdef __dis_tex

SYSCALL ER
dis_tex()
{
	TCB	*runtsk;
	ER	ercd;

	LOG_DIS_TEX_ENTER();
	CHECK_TSKCTX_UNL();

	t_acquire_glock();
	runtsk = get_my_tpcb()->runtsk;
	if (runtsk->tinib->texrtn == NULL) {
		ercd = E_OBJ;
	}
	else {
		runtsk->enatex = FALSE;
		ercd = E_OK;
	}
	t_release_glock();

    exit:
	LOG_DIS_TEX_LEAVE(ercd);
	return(ercd);
}

#endif /* __dis_tex */

/*
 *  タスク例外処理の許可
 */
#ifdef __ena_tex

SYSCALL ER
ena_tex()
{
	ER	ercd;
	TCB	*runtsk;
	TPCB	*my_tpcb;

	LOG_ENA_TEX_ENTER();
	CHECK_TSKCTX_UNL();

	t_acquire_glock();
	my_tpcb = get_my_tpcb();
	runtsk = my_tpcb->runtsk;
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
	t_release_glock();

    exit:
	LOG_ENA_TEX_LEAVE(ercd);
	return(ercd);
}

#endif /* __ena_tex */

/*
 *  タスク例外処理禁止状態の参照
 */
#ifdef __sns_tex

SYSCALL BOOL
sns_tex()
{
	TCB	*runtsk;
	BOOL	state;

	LOG_SNS_TEX_ENTER();
	runtsk = get_my_tpcb()->runtsk;
	state = (runtsk != NULL && runtsk->enatex) ? FALSE : TRUE;
	LOG_SNS_TEX_LEAVE(state);
	return(state);
}

#endif /* __sns_tex */
