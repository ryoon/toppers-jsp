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
 * 
 *  @(#) $Id: hw_timer.h 889 2025-11-04 17:02:38Z roi $
 */

/*
 *   タイマドライバ（SAPPHIRESOC用）
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>		/* デバイスドライバ用標準インクルードファイル */

/*
 *  CLINT周波数定義
 */
#define CLINT_FREQ      SYSTEM_CLINT_HZ

#define TOFF_CLINT_MTIMECMP 0x4000
#define TOFF_CLINT_MTIME    0xBFF8

/*
 *  タイマ割込みの割込み番号
 */
#define INHNO_TIMER	32	/* TMAX_INTNO */
#define	INTNO_TIMER	INHNO_TIMER

#ifndef _MACRO_ONLY

/*
 *  タイマ値の内部表現の型
 */
typedef UD		CLOCK;

#endif /* _MACRO_ONLY */

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 */
#define DENOMINATOR(nume, deno) (1000 * (nume) / (deno))
#define REMAINDER(nume, deno)   (((CLINT_FREQ * (nume)) / (deno)) % DENOMINATOR((nume), (deno)))
#define TIMER_CLOCK				(CLINT_FREQ)
#define TO_CLOCK(nume, deno)	((CLOCK)((TIMER_CLOCK * (nume) / (deno)) / 1000u))
#define TO_USEC(clock)			((clock) * 1000000U / TIMER_CLOCK)

/*
 *  タイマの現在値を割込み発生前の値とみなすかの判断
 */
#define	GET_TOLERANCE		100u	/* 処理遅れの見積り値(単位は内部表現) */
#define	BEFORE_IREQ(clock) \
		((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK		((CLOCK) 0xffffffffffffffffU)


#ifndef _MACRO_ONLY

/*
 *  累積値の読み出し
 */
Inline CLOCK
hw_current_timer_value(void)
{
	while (1) {
		UW hi = sil_rew_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIME+4));
		UW lo = sil_rew_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIME));
		if(hi == sil_rew_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIME+4)))
			return ((CLOCK)hi << 32) | lo;
	}
}

/*
 *  ターゲット値の書き込み
 */
Inline void
hw_set_target_timer_value(CLOCK mtimecmp)
{
	sil_wrw_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIMECMP+4), (UW)(mtimecmp >> 32));
	sil_wrw_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIMECMP), (UW)mtimecmp);
	compare_time = mtimecmp;
}

/*
 *  タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear(void)
{
	UD	mtimecmp;
	W   difference;

	mtimecmp = compare_time;
	do{
		mtimecmp += TO_CLOCK(TIC_NUME, TIC_DENO);
		if(molecule >= DENOMINATOR(TIC_NUME, TIC_DENO)){
			mtimecmp++;
			molecule -= DENOMINATOR(TIC_NUME, TIC_DENO);
		}
		molecule += REMAINDER(TIC_NUME, TIC_DENO);
		difference = mtimecmp - hw_current_timer_value();
	}while(difference <= 0);
	hw_set_target_timer_value(mtimecmp);
}

/*
 *  タイマの起動処理
 *    タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize(void)
{
	CLOCK	mtimecmp;

	/*
	 *  タイマ周期を設定し，タイマの動作を開始する．
	 */
	mtimecmp = hw_current_timer_value() + TO_CLOCK(TIC_NUME, TIC_DENO);
	hw_set_target_timer_value(mtimecmp);
	molecule = REMAINDER(TIC_NUME, TIC_DENO);
}

/*
 *  タイマの停止処理
 *  タイマの動作を停止させる．
 */
Inline void
hw_timer_terminate(void)
{
	/*
	 *  タイマの動作を停止する．
	 */
	clear_csr(mie, MIP_MTIP);
}

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
hw_timer_get_current(void)
{
	CLOCK	clk;

	clk = hw_current_timer_value() - (compare_time - TO_CLOCK(TIC_NUME, TIC_DENO));
	return(clk);
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return hw_current_timer_value() >= compare_time;
}

#endif /* _MACRO_ONLY */
#endif /* _HW_TIMER_H_ */
