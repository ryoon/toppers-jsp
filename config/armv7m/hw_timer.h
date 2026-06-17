/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 */

/*
 *   タイマドライバ（Cortex-M用）
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>		/* デバイスドライバ用標準インクルードファイル */

/*
 *  タイマ割込みの割込み番号
 */
#define INHNO_TIMER		IRQ_VECTOR_SYSTICK
#define INTNO_TIMER		INHNO_TIMER


#ifndef _MACRO_ONLY

/*
 *  タイマ値の内部表現の型
 */
typedef UW	CLOCK;

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 */
#define TO_CLOCK(nume, deno)	((CLOCK)(TIMER_CLOCK * (nume) / (deno)))
#define TO_USEC(clock)			(((SYSUTIM) clock) * 1000U / TIMER_CLOCK)

/*
 *  タイマの現在値を割込み発生前の値とみなすかの判断
 */
#define	GET_TOLERANCE		100u	/* 処理遅れの見積り値(単位は内部表現) */
#define	BEFORE_IREQ(clock) \
		((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK	((CLOCK)0x00ffffffU)

/*
 *  タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear(void)
{
	sil_rew_mem((void *)SYSTIC_CONTROL_STATUS);
}

/*
 *  タイマの起動処理
 *    タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize(void)
{
	CLOCK	cyc;
	UW	tmp;

#ifdef SYSTIC_USE_CALIBRATION
	/* CALIBRATIONレジスタの値を使用 */
	cyc = (sil_rew_mem((VP)SYSTIC_CALIBRATION) & SYSTIC_TENMS) / 10;
#else 
	cyc = TO_CLOCK(TIC_NUME, TIC_DENO) - 1;
#endif /* SYSTIC_USE_CALIBRATION */

	/* 停止 */
	tmp = sil_rew_mem((VP)SYSTIC_CONTROL_STATUS);
	tmp &= ~SYSTIC_ENABLE;
	sil_wrw_mem((VP)SYSTIC_CONTROL_STATUS, tmp);

	sil_wrw_mem((VP)SYSTIC_RELOAD_VALUE, cyc);
	sil_wrw_mem((VP)SYSTIC_CURRENT_VALUE, cyc);

	tmp = sil_rew_mem((VP)SYSTIC_CONTROL_STATUS);

#ifdef SYSTIC_USE_STCLK
	/* 外部クロックの使用 */
	tmp |= SYSTIC_ENABLE;
#else
	/* プロセッサクロックの使用 */
	tmp |= SYSTIC_ENABLE|SYSTIC_CLKSOURCE;
#endif /* SYSTIC_USE_STCLK */

	sil_wrw_mem((VP)SYSTIC_CONTROL_STATUS, tmp);
}

/*
 *  タイマの停止処理
 *  タイマの動作を停止させる．
 */
Inline void
hw_timer_terminate(void)
{
	/* タイマを停止 */
	sil_wrw_mem((VP)SYSTIC_CONTROL_STATUS, 0x00);
}

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
hw_timer_get_current(void)
{
	CLOCK	cyc;
	UW	tmp;

#ifdef SYSTIC_USE_CALIBRATION
	/* CALIBRATIONレジスタの値を使用 */
	cyc = (sil_rew_mem((VP)SYSTIC_CALIBRATION) & SYSTIC_TENMS) / 10;
#else 
	cyc = TO_CLOCK(TIC_NUME, TIC_DENO) - 1;
#endif /* SYSTIC_USE_CALIBRATION */
	return cyc - sil_rew_mem((VP)SYSTIC_CURRENT_VALUE);
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return ((sil_rew_mem((VP)NVIC_ICSR) & NVIC_PENDSTSET) == NVIC_PENDSTSET);
}

#endif /* _MACRO_ONLY */
#endif /* _HW_TIMER_H_ */
