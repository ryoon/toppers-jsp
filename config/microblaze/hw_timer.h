/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: hw_timer.h,v 1.1 2002/04/05 08:47:14 honda Exp $
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

/*
 * CPU依存タイマモジュール（MicroBlaze用）
 */ 
 
#include "sys_defs.h"
#include "cpu_config.h"
#include "sys_config.h"
#include "microblaze.h"

/*
 * タイマ割込みハンドラの割込み番号
 */
#define INHNO_TIMER TIMINTLVL

/*
 * タイマ値の内部表現の型
 */
typedef UW CLOCK;

#define TO_CLOCK(nume, deno)  (TIMER_CLOCK * (nume) / (deno))

/*
 *  設定できる最大タイマ周期（単位は内部表現）
 */
#define MAX_CLOCK ((CLOCK) 0xffffffff)

/*
 *  性能評価用システム時刻を取り出す際の処理時間の見積り値（単位は内部表現）
 */
#define GET_TOLERANCE	100



/*
 * タイマの起動処理
 *
 * タイマを初期化し，周期的なタイマ割込みを発生させる
 */ 
Inline void
hw_timer_initialize(void)
{
  int dummy;
  
  CLOCK c = TO_CLOCK(TIC_NUME, TIC_DENO);

  *TIMER_TCSR0 = TIMER_RESET;  /* リセット */
  
  assert(c <= MAX_CLOCK);		     /* タイマ上限値のチェック */
  *TIMER_TCCR0 = c;            /* カウンターセット */
  
  /*
   * タイマー0イネーブル，オートリロード，ダウンカウント　割込み許可　
   */
  *TIMER_TCSR0 = TIMER_ENABLE | TIMER_RELOAD
                   | TIMER_DOWN_COUNT | TIMER_ENABLE_INTR;

  /*
   *  INTC割込み許可
   */
  dummy = 1 << (TIMINTLVL - 1);
  intc_enable_interrupt(dummy);
}



/*
 *   タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear()
{
  *TIMER_TCSR0 = *TIMER_TCSR0; 
}



/*
 *   タイマの禁止処理
 */
Inline void
hw_timer_terminate()
{
  int dummy;
  
  *TIMER_TCSR0 = ~TIMER_ENABLE; /* タイマ停止         */
  *TIMER_TCSR0 = *TIMER_TCSR0;  /* 割込み要求のクリア */
  
  /*
   * すでに割込みが入っている場合があるので割込み要求をクリア
   */
  dummy = 1 << (TIMINTLVL - 1);
  intc_ack_interrupt(dummy);
  intc_disable_interrupt(dummy);  /* INTCの受け付け禁止 */
}



/*
 *   タイマの現在値の読み出し
 *
 *   割込み禁止区間中で呼び出すこと
 */
Inline CLOCK
hw_timer_get_current(void)
{
  return(TO_CLOCK(TIC_NUME, TIC_DENO) - *TIMER_TCR0);
}



/*
 *   割込みが発生しているかの判定
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
  return(*TIMER_TCSR0 & TIMER_INTERRUPT);
}

#endif /* _HW_TIMER_H_ */