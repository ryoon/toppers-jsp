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
 *  @(#) $Id: time_manage.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	システム時刻管理機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "time_event.h"

/*
 *  システム時刻のオフセット
 *
 *  systim_offset が 0 に初期化されることを仮定している．0 に初期化さ
 *  れない環境では，set_tim を呼び出して systim_offset を初期化するこ
 *  とが必要．
 */
static SYSTIM	systim_offset;

/*
 *  システム時刻の設定
 */
SYSCALL ER
set_tim(SYSTIM *pk_systim)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	systim_offset = *pk_systim - current_time;
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  システム時刻の参照
 */
SYSCALL ER
get_tim(SYSTIM *pk_systim)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*pk_systim = systim_offset + current_time;
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  性能評価用システム時刻の参照
 */
#ifdef SUPPORT_VXGET_TIM
#include "hw_timer.h"

SYSCALL ER
vxget_tim(SYSUTIM *pk_sysutim)
{
	SYSUTIM	utime;
	SYSTIM	time;
#if TIC_DENO != 1
	INT	subtime;
#endif /* TIC_DENO != 1 */
	CLOCK	clock;
	BOOL	ireq;
	BOOL	locked;

	CHECK_TSKCTX();

	locked = sense_lock();
	if (!(locked)) {
		t_lock_cpu();
	}
	time = systim_offset + next_time;
#if TIC_DENO != 1
	subtime = (INT) next_subtime;
#endif /* TIC_DENO != 1 */
	clock = hw_timer_get_current();
	ireq = hw_timer_fetch_interrupt();
	if (!(locked)) {
		t_unlock_cpu();
	}

	utime = ((SYSUTIM) time) * 1000;
#if TIC_DENO != 1
	utime += subtime * 1000 / TIC_DENO
#endif /* TIC_DENO != 1 */
	if (!(ireq) || clock >= (TO_CLOCK(TIC_NUME, TIC_DENO)
					- GET_TOLERANCE)) {
		utime -= TIC_NUME * 1000 / TIC_DENO;
	}
	utime += clock * 1000 / TIMER_CLOCK;
	*pk_sysutim = utime;
	return(E_OK);
}

#endif /* SUPPORT_VXGET_TIM */
