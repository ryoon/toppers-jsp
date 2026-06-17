/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  @(#) $Id: hw_timer.h 1218 2026-03-11 11:44:23Z roi $
 */

/*
 *  タイマドライバ（DE10-Nano用）
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>		/* デバイスドライバ用標準インクルードファイル */
#include "mpcore.h"

/*
 *  タイマ割込みハンドラ登録のための定数
 */
/*
 *  ローカルタイマ方式用の定義
 */
#define INHNO_TIMER		IREQ_PPI_TIMER_PRIVATE	/* 割込みハンドラ番号 */
#define INTNO_TIMER		IREQ_PPI_TIMER_PRIVATE	/* 割込み番号 */


#ifndef _MACRO_ONLY

/*
 *  タイマ値の内部表現の型
 */
typedef UW	CLOCK;

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 *  1μ秒毎(1MHz) にカウントダウンする
 */
#define TO_CLOCK(nume, deno) (TIMER_CLOCK * (nume) / (deno))
#define TO_USEC(clock)       (((SYSUTIM) clock) * 1000U / TIMER_CLOCK)

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK        ((CLOCK) 0xffffffffU)

/*
 *  タイマの現在値を割込み発生前の値とみなすかの判断
 */
#define GET_TOLERANCE    100U    /* 処理遅れの見積り値（単位は内部表現）*/
#define BEFORE_IREQ(clock) \
            ((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear(void)
{
	/* ペンディングビットをクリア */
	sil_wrw_mem((void *)MPCORE_TM_ISR, MPCORE_TM_ISR_SCBIT);
}

/*
 *  タイマの起動処理
 *    タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize(void)
{
	CLOCK    cyc = TO_CLOCK(TIC_NUME, TIC_DENO);

	/* タイマ停止 */
	sil_wrw_mem((void *)MPCORE_TM_CNT, 0x00);

	/* ペンディングビットクリア */
	hw_timer_int_clear();

	/* ロード値を設定 */
	sil_wrw_mem((void *)MPCORE_TM_LR, cyc - 1);

	/*
	 * タイマースタート(割込み許可)
	 * 1MHzでカウントするようにプリスケーラーを設定
	 */
	sil_wrw_mem((void *)MPCORE_TM_CNT,
				(MPCORE_TM_PS_1MS << MPCORE_TM_CNT_PS_OFFSET)
				| MPCORE_TM_CNT_IEN | MPCORE_TM_CNT_AR | MPCORE_TM_CNT_EN);
}

/*
 *  タイマの停止処理
 *  タイマの動作を停止させる．
 */
Inline void
hw_timer_terminate(void)
{
	/* 割込みクリア */
	hw_timer_int_clear();

	/* タイマ停止 */
	sil_wrw_mem((void *)MPCORE_TM_CNT, 0x00);
}

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
hw_timer_get_current(void)
{
	return(TO_CLOCK(TIC_NUME, TIC_DENO) - sil_rew_mem((void *)MPCORE_TM_COUNT));
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	if((sil_rew_mem((void *)MPCORE_TM_ISR) & MPCORE_TM_ISR_SCBIT)
	   == MPCORE_TM_ISR_SCBIT){
		return TRUE;
	}
	else {
		return FALSE;
	}
}

#endif /* _MACRO_ONLY */
#endif /* _HW_TIMER_H_ */
