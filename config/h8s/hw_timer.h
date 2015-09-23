/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: hw_timer.h,v 1.2 2002/04/13 13:14:45 imai Exp $
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

/*
 *  CPU依存タイマモジュール（H8S用）
 *  TPU0を使用
 */

#include "sys_defs.h"
#include "sys_config.h"
#include "cpu_config.h"


#pragma inline(hw_timer_int_clear, hw_timer_initialize, hw_timer_terminate) 
#pragma inline(hw_timer_get_current, hw_timer_fetch_interrupt)


/*
 *  タイマ値の内部表現の型
 */

typedef UH	CLOCK;


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
 *	TSRレジスタのIMFAビットは1回読み出した後に０を書き込む
 */

Inline void
hw_timer_int_clear(void)
{
    /* GRAコンペアマッチの割り込み要求フラグをクリア */
    UW addr = SYSTEM_TPU + H8STSR;
    
    bitclr(addr, SYSTEM_TPU_IF_BIT);
}

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */

Inline void
hw_timer_initialize(void)
{
    UW addr = H8STPU_TSTR;

    /*  モジュールストップモード解除  */
    MSTPCR &= ~MSTPCR_TPU;

    /* タイマ停止 */
    bitclr(addr, SYSTEM_STR_BIT);
    
    /*  GRAコンペアマッチでカウンタをクリア、分周比設定 */
    outb(SYSTEM_TPU + H8STCR, H8STCR_CCLR | H8STCR_CKEG | H8STCR_TPSC);

    /*  GRAコンペアマッチによる割込み要求を許可 */
    outb(SYSTEM_TPU + H8STIER, SYSTEM_TPU_IE);

    /*  GRAコンペアマッチによる端子出力禁止 */
    outb(SYSTEM_TPU + H8STIORH, H8STIOR_IOB | H8STIOR_IOA);
    outb(SYSTEM_TPU + H8STIORL, H8STIOR_IOC | H8STIOR_IOD);
    /*  GRAレジスタ設定（カウンタ目標値）  	*/
    outw(SYSTEM_TPU + SYSTEM_GR, CLOCK_PER_TICK);

    outw(SYSTEM_TPU + H8STCNT, 0);	/* カウンタをクリア	*/

	/*
	 *  割込み関連の設定
	 */
    					/* 割り込みレベル設定 	*/
    outb(SYSTEM_TPU_IPR,(UB)((inb(SYSTEM_TPU_IPR)&~(0x7<<TPU_BIT_SHIFT)) | (TPU_INTLVL<<TPU_BIT_SHIFT)));
    hw_timer_int_clear();		/*  割込み要求をクリア  */

    /* タイマスタート */
        bitset(addr, SYSTEM_STR_BIT);
}

/*
 *  タイマの停止処理
 */

Inline void
hw_timer_terminate(void)
{
    UW addr = H8STPU_TSTR;

    /* タイマ停止 */
        bitclr(addr, SYSTEM_STR_BIT);

    	hw_timer_int_clear();		/* 割り込み要求をクリア */
}

/*
 *  タイマの現在値の読み出し
 *
 *  割込み禁止区間中で呼び出すこと．
 */

Inline CLOCK
hw_timer_get_current(void)
{
	return(CLOCK_PER_TICK - (CLOCK)(inw(SYSTEM_TPU + H8STCNT)));
}

/*
 *  タイマ割込み要求のチェック
 */

Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return(inb(SYSTEM_TPU + H8STSR) & H8STSR_TGFA);
}

#endif /* _HW_TIMER_H_ */
