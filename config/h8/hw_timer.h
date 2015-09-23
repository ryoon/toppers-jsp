/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: hw_timer.h,v 1.1 2001/11/12 13:38:30 abe Exp $
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

/*
 *  CPU依存タイマモジュール（H8用）
 *  ITU0を使用
 */

#include "sys_defs.h"
#include "sys_config.h"
#include "cpu_config.h"

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
 *	TCRレジスタのIMFAビットは1回読み出した後に０を書き込む
 */

Inline void
hw_timer_int_clear(void)
{
    /* GRAコンペアマッチの割り込み要求フラグをクリア */
    /*outb(SYSTEM_ITU + H8TSR, inb(SYSTEM_ITU + H8TSR) & ~H8TSR_IMIFA);*/
    UW addr = SYSTEM_ITU + H8TSR;

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(H8TSR_IMIFA_BIT));
#undef  BITCLR
}

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */

Inline void
hw_timer_initialize()
{
    UW addr = H8ITU_TSTR;

    /* タイマ停止 */
    /*outb(H8ITU_TSTR, inb(H8ITU_TSTR) & ~SYSTEM_STR);*/

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(SYSTEM_STR_BIT));
#undef  BITCLR

    /*  GRAコンペアマッチでカウンタをクリア、分周比設定 */
    outb(SYSTEM_ITU + H8TCR, H8TCR_CCLR | H8TCR_CKEG | H8TCR_TPSC);

    /*  GRAコンペアマッチによる割込み要求を許可 */
    outb(SYSTEM_ITU + H8TIER, SYSTEM_ITU_IE);

    /*  GRAコンペアマッチによる端子出力禁止 */
    outb(SYSTEM_ITU + H8TIOR, H8TIOR_IOB | H8TIOR_IOA);

    /*  GRAレジスタ設定（カウンタ目標値）  	*/
    outw(SYSTEM_ITU + SYSTEM_GR, CLOCK_PER_TICK);

    outw(SYSTEM_ITU + H8TCNT, 0);	/* カウンタをクリア	*/

    hw_timer_int_clear();		/*  割込み要求をクリア  */

    /* タイマスタート */
    /*outb(H8ITU_TSTR, inb(H8ITU_TSTR) | SYSTEM_STR);*/

#define BITSET(bit)	Asm("bset #" bit ", @%0" : : "r"(addr))
	BITSET(_TO_STRING(SYSTEM_STR_BIT));
#undef  BITSET
}

/*
 *  タイマの停止処理
 */

Inline void
hw_timer_terminate()
{
    UW addr = H8ITU_TSTR;

    /* タイマ停止 */
    /*outb(H8ITU_TSTR, inb(H8ITU_TSTR) & ~SYSTEM_STR);*/

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(SYSTEM_STR_BIT));
#undef  BITCLR

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
	return(CLOCK_PER_TICK - (CLOCK)(inw(SYSTEM_ITU + H8TCNT)));
}

/*
 *  タイマ割込み要求のチェック
 */

Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return(inb(SYSTEM_ITU + H8TSR) & H8TSR_IMIFA);
}

#endif /* _HW_TIMER_H_ */
