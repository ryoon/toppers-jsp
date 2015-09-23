/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2006-2010 by Witz Corporation, JAPAN
 * 
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 * 
 *  (3) When this software is redistributed in the forms unusable for the
 *      development of other software, such as the case when the software
 *      is embedded in a piece of equipment, either of the following two
 *      conditions must be satisfied:
 * 
 *    (a) The above copyright notice, this use conditions, and the
 *           disclaimer shown below must be shown without modification in
 *        the document provided with the redistributed software, such as
 *        the user manual.
 * 
 *    (b) How the software is to be redistributed must be reported to the
 *        TOPPERS Project according to the procedure described
 *        separately.
 * 
 *  (4) The above copyright holders and the TOPPERS Project are exempt
 *      from responsibility for any type of damage directly or indirectly
 *      caused from the use of this software and are indemnified by any
 *      users or end users of this software from any and all causes of
 *      action whatsoever.
 * 
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 * 
 *  @(#) $Id: hw_timer.h,v 1.1 2006/04/10 08:19:25 honda Exp $
 */

/*
 *	タイマドライバ（Zup-F16拡張ボード用）
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>
#include <sil.h>
#include <tmp91cy22.h>

/*
 *  タイマ割込みハンドラのベクタ番号
 */
#define	INHNO_TIMER		INT_NO_TA0

/*
 *  タイマーの割り込みレベル
 */
#define INT_LEVEL_TIMER	0x04

#ifndef _MACRO_ONLY

/*
 *  1msのタイマー割り込みを使用するのでそのクロックを定義する．
 */
#define TIMER_CLOCK		192


/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
#pragma inline hw_timer_initialize
void
hw_timer_initialize()
{
	/* タイマ0停止	*/
	sil_wrb_mem((VP)TADR_SFR_TA01RUN,
		(VB)(sil_reb_mem((VP)TADR_SFR_TA01RUN) & ~TBIT_TA0RUN));
	/* モード設定	 IDLE2:動作,8bitモード,ソースクロック/16,1ms	*/
	sil_wrb_mem((VP)TADR_SFR_TA01RUN,
		(sil_reb_mem((VP)TADR_SFR_TA01RUN) | TBIT_I2TA01));
	sil_wrb_mem((VP)TADR_SFR_TA01MOD,
		(sil_reb_mem((VP)TADR_SFR_TA01MOD) | TBIT_TA0CLK16));
	sil_wrb_mem((VP)TADR_SFR_TA0REG, (VB)(TIMER_CLOCK - 1) );
	/* 割込み設定	割込み要求クリア,割込みレベル5	*/
	sil_wrb_mem((VP)TADR_SFR_INTCLR, TBIT_TA0_CLR );
	sil_wrb_mem((VP)TADR_SFR_INTETA01,
		(VB)((sil_reb_mem((VP)TADR_SFR_INTETA01) & ~TBIT_INTM_L) | INT_LEVEL_TIMER));
	/* タイマ0およびプリスケーラ起動	*/
	sil_wrb_mem((VP)TADR_SFR_TA01RUN,
		(VB)(sil_reb_mem((VP)TADR_SFR_TA01RUN) | (TBIT_TA01PRUN | TBIT_TA0RUN)));
}

/*
 *  タイマ割込み要求のクリア
 */
#pragma inline hw_timer_int_clear
void
hw_timer_int_clear()
{
	/* 割込み要求クリア	*/
	sil_wrb_mem((VP)TADR_SFR_INTCLR, TBIT_TA0_CLR );
}

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
#pragma inline hw_timer_terminate
void
hw_timer_terminate()
{
	/* タイマ0停止	*/
	sil_wrb_mem((VP)TADR_SFR_TA01RUN,
		(VB)(sil_reb_mem((VP)TADR_SFR_TA01RUN) & ~TBIT_TA0RUN));
}

/*
 *  タイマ割込み要求のチェック
 */
#pragma inline hw_timer_fetch_interrupt
BOOL
hw_timer_fetch_interrupt(void)
{
	return (sil_reb_mem((VP)TADR_SFR_INTETA01) & TBIT_INTC_L);
}

#endif /* _MACRO_ONLY */
#endif /* _HW_TIMER_H_ */
