/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: hw_timer.h,v 1.2 2001/10/22 02:16:43 imai Exp $
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

/*
 *  CPU依存タイマモジュール（SH1用）
 *  TMU0を使用
 */

#include "sh1.h"
#include "sys_defs.h"
#include "cpu_config.h"

/*
 *  タイマ割込みハンドラのベクタ番号
 *	jsp/systask/timer.cfgで割込みハンドラの登録に使用
 */
#define	INHNO_TIMER	IMIA0

/*
 *  タイマ値の内部表現の型
 */
typedef UH	CLOCK;

/*
 *  タイマーの割り込みレベル
 */
#define TINTLVL0	4

/*  割込みレベルを設定レジスタに代入するときのシフト数  */
#define ITU_BIT_SHIFT	4

/*
 *  タイマ周期の単位を内部表現に変換
 *  	jsp/kernel/time_manage.cでも使用
 */
#define TO_CLOCK(nume, deno)	(TIMER_CLOCK * (nume) / (deno))

/*
 *  タイマ周期の単位を内部表現に変換
 */
#define CLOCK_PER_TICK	((CLOCK) TO_CLOCK(TIC_NUME, TIC_DENO))

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define MAX_CLOCK	((CLOCK) 0xffff)

/*
 *  性能評価用システム時刻を取り出す際の処理時間の見積り値
 *	（単位は内部表現）
 *	注意（本当は分周クロックに依存）
 */
#define GET_TOLERANCE	100


/*
 *  タイマ割込み要求のクリア
 *	TCRレジスタのIMFAビットは1回読み出した後に０を書き込む
 */


Inline void
hw_timer_int_clear()
{
    ITU.TSR0 &= ~TSR_IMFA;	/* 割り込み要求をクリア 	*/
}				/*  （GRAコンペアマッチフラグ） */


/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize()
{
	/*
	 *  タイマ関連の設定
	 */
    ITU.TSTR &= ~TSTR_STR0;	/* タイマ停止 */
    		     		/* タイマ上限値のチェック */
    assert(CLOCK_PER_TICK <= MAX_CLOCK);
			/*  GRAコンペアマッチでカウンタをクリア */
			/*  分周比設定  			*/
    ITU.TCR0 = (TCR_CCLR_GRA | TCR_TPSC);
			/*  GRAコンペアマッチによる割込み要求を許可 */
    ITU.TIER0 = TIER_RESERVE | TIER_IMIEA;
			/*  GRAコンペアマッチによる端子出力禁止 */
    ITU.TIOR0 = TIOR_RESERVE | TIOR_IOA;
			/*  GRAレジスタ設定（カウンタ目標値）  	*/
    ITU.GRA0 = CLOCK_PER_TICK;
    ITU.TCNT0 = 0;	/* カウンタをクリア 			*/

	/*
	 *  割込み関連の設定
	 */
    					/* 割り込みレベル設定 	*/
    define_int_plevel(IPRC, TINTLVL0, ITU_BIT_SHIFT);
    hw_timer_int_clear();		/*  割込み要求をクリア  */


    ITU.TSTR  |= TSTR_STR0;		/* タイマスタート    	*/
}


/*
 *  タイマの停止処理
 */
Inline void
hw_timer_terminate()
{
    ITU.TSTR &= ~TSTR_STR0;		  /* タイマを停止 	  */
    hw_timer_int_clear();		  /* 割り込み要求をクリア */
}

/*
 *  タイマの現在値の読み出し
 *
 *  割込み禁止区間中で呼び出すこと．
 */
Inline CLOCK
hw_timer_get_current(void)
{
	return(CLOCK_PER_TICK - (CLOCK)(ITU.TCNT0));
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return(ITU.TSR0 & TSR_IMFA);
}

#endif /* _HW_TIMER_H_ */
