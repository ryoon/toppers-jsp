/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: hw_timer.h,v 1.3 2002/04/10 11:08:33 hiro Exp $
 */

/*
 *	ターゲット依存タイマモジュール（DVE68K/40用）
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include "dve68k.h"

/*
 *  タイマ割込みハンドラのベクタ番号
 */
#define	INHNO_TIMER	TT0_VEC

/*
 *  タイマ値の内部表現の型
 */
typedef UW	CLOCK;

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 *
 *  DVE68K/40 CPUボードでは，タイマは1μ秒毎にカウントアップする．
 */
#define TIMER_CLOCK		1000
#define TO_CLOCK(nume, deno)	(TIMER_CLOCK * (nume) / (deno))

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK	((CLOCK) 0xffffff)

/*
 *  性能評価用システム時刻を取り出す際の処理時間の見積り値（単位は内部表現）
 */
#define GET_TOLERANCE	100

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize()
{
	CLOCK	c = TO_CLOCK(TIC_NUME, TIC_DENO);

	/*
	 *  タイマ関連の設定
	 */
	assert(c <= MAX_CLOCK);		/* タイマ周期の上限値のチェック */
	*DGA_CSR12 = 0x80000000 | c;	/* 上限値を設定，タイマスタート */

	/*
	 *  割込み関連の設定
	 */
	dga_set_ilv(DGA_CSR25, TT0IL_BIT, IRQ_LEVEL4);
					/* 割込みレベル設定 */
	*DGA_CSR23 = TT0_BIT;		/* 割込み要求をクリア */
	*DGA_CSR21 |= TT0_BIT;		/* 割込みマスク解除 */
}

/*
 *  タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear()
{
	*DGA_CSR23 = TT0_BIT;		/* タイマ割込み要求をクリア */
}

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
Inline void
hw_timer_terminate()
{
	*DGA_CSR12 &= 0x7fffffff;	/* タイマを停止 */
	*DGA_CSR21 &= ~TT0_BIT;		/* 割込みマスク設定 */
	*DGA_CSR23 = TT0_BIT;		/* 割込み要求をクリア */
}

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
hw_timer_get_current(void)
{
	CLOCK	c;

	*DGA_CSR12 &= 0x7fffffff;	/* タイマを停止 */
	cpu_wait_io();			/* タイマが停止するのを待つ */
	cpu_wait_io();
	c = *DGA_CSR13 & 0x00ffffff;
	*DGA_CSR12 |= 0x80000000;	/* タイマスタート */
	return(c);
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return(*DGA_CSR20 & TT0_BIT);
}

#endif /* _HW_TIMER_H_ */
