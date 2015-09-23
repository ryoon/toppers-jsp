/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: hw_timer.h,v 1.9 2004/09/03 15:39:07 honda Exp $
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>

#ifndef _MACRO_ONLY

/*
 *  CPU依存タイマモジュール（H8用）
 */

/*
 *  タイマ値の内部表現の型
 */

typedef UH	CLOCK;

/*
 *  タイマ周期の単位を内部表現に変換
 *  	jsp/kernel/time_manage.cでも使用
 */

#define TO_CLOCK(nume, deno)	(TIMER_CLOCK * (nume) / (deno))
#define TO_USEC(clock)		((clock) * 1000 / TIMER_CLOCK)

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
#define	BEFORE_IREQ(clock) \
		((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  タイマ割込み要求のクリア
 *	TCRレジスタのIMFAビットは1回読み出した後に０を書き込む
 */

Inline void
hw_timer_int_clear(void)
{
    /* GRAコンペアマッチの割り込み要求フラグをクリア */
    /*sil_wrb_mem((VP)SYSTEM_TIMER_IFR,
                  sil_reb_mem((VP)SYSTEM_TIMER_IFR) & ~SYSTEM_TIMER_IF);*/
    UW addr = SYSTEM_TIMER_IFR;

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(SYSTEM_TIMER_IF_BIT));
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
    UW addr = SYSTEM_TIMER_TSTR;

    /* タイマ停止 */
    /*sil_wrb_mem((VP)SYSTEM_TIMER_TSTR,
                  sil_reb_mem((VP)SYSTEM_TIMER_TSTR) & ~SYSTEM_TIMER_STR);*/

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(SYSTEM_TIMER_STR_BIT));
#undef  BITCLR

    /*  GRAコンペアマッチでカウンタをクリア、分周比設定 */
    sil_wrb_mem((VP)SYSTEM_TIMER_TCR, SYSTEM_TIMER_TCR_BIT);

    /*  GRAコンペアマッチによる割込み要求を許可 */
    sil_wrb_mem((VP)SYSTEM_TIMER_IER, SYSTEM_TIMER_IE);

    /*  GRAコンペアマッチによる端子出力禁止 */
    sil_wrb_mem((VP)SYSTEM_TIMER_TIOR, SYSTEM_TIMER_TIOR_BIT);

    /*  GRAレジスタ設定（カウンタ目標値）  	*/
    sil_wrh_mem((VP)SYSTEM_TIMER_GR, CLOCK_PER_TICK);

    sil_wrh_mem((VP)SYSTEM_TIMER_CNT, 0);	/* カウンタをクリア	*/

    hw_timer_int_clear();			/*  割込み要求をクリア	*/

    /* タイマスタート */
    /*sil_wrb_mem((VP)SYSTEM_TIMER_TSTR,
                  sil_reb_mem((VP)SYSTEM_TIMER_TSTR) | SYSTEM_TIMER_STR);*/

#define BITSET(bit)	Asm("bset #" bit ", @%0" : : "r"(addr))
	BITSET(_TO_STRING(SYSTEM_TIMER_STR_BIT));
#undef  BITSET
}

/*
 *  タイマの停止処理
 */

Inline void
hw_timer_terminate()
{
    UW addr = SYSTEM_TIMER_TSTR;

    /* タイマ停止 */
    /*sil_wrb_mem((VP)SYSTEM_TIMER_TSTR,
                  sil_reb_mem((VP)SYSTEM_TIMER_TSTR) & ~SYSTEM_TIMER_STR);*/

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(SYSTEM_TIMER_STR_BIT));
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
	return((CLOCK)(sil_reh_mem((VP)SYSTEM_TIMER_CNT)));
}

/*
 *  タイマ割込み要求のチェック
 */

Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return(sil_reb_mem((VP)SYSTEM_TIMER_IFR) & SYSTEM_TIMER_IF);
}

#endif	/* of #ifndef _MACRO_ONLY */

#endif /* _HW_TIMER_H_ */
