/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id$
 */

/*
 *  CPU依存タイマモジュール（SH7047用）
 *  CMT0を使用
 *
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <s_services.h>
#include "crb_h3.h"

/*
 *  コンペアマッチタイマスタートレジスタCMSTRのビット設定
 */
#define CMSTR_STR1	0x0002	/* カウントスタート1 */
#define CMSTR_STR0  0x0001	/* カウントスタート0 */

/*
 *  コンペアマッチタイマコントロール／ステータスレジスタCMCSRのビット設定
 */
#define CMCSR_CMF   0x0080	/* コンペアマッチフラグ */
#define CMCSR_CMIE  0x0040	/* コンペアマッチ割込みイネーブル */
#define CMCSR_CKS8  0x0000	/* 8分周クロックセレクト */

/*
 *  タイマ割込みハンドラのベクタ番号
 */
#define	INHNO_TIMER	CMI0

/*
 *   タイマに供給されるクロック周波数[kHz]
 *   	40.000MHz/8 =5.0000MHz =5000.0kHz
 */
#define TIMER_CLOCK     (CLOCK_RATE/8000L)

#ifndef _MACRO_ONLY
/*
 *  タイマ値の内部表現の型
 */
typedef UH CLOCK;

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 *
 */
#define	TO_CLOCK(nume, deno)	(TIMER_CLOCK * (nume) / (deno))
#define	TO_USEC(clock)		((clock) * 1000 / TIMER_CLOCK)
#define CLOCK_PER_TICK	((CLOCK) TO_CLOCK(TIC_NUME, TIC_DENO))

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define	MAX_CLOCK	((CLOCK) 0xffff)

/*
 *  タイマの現在値を割込み発生前の値とみなすかの判断
 */
#define	GET_TOLERANCE	100		/* 処理遅れの見積り値（単位は内部表現） */
#define	BEFORE_IREQ(clock) \
		((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)


/*  割込みレベルを設定レジスタに代入するときのシフト数  */
#define FRT_BIT_SHIFT	4

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize()
{
	/* タイマデバイス自体の初期化 */
	sil_wrh_mem((VP)IOREG_CMSTR_BASE, 0x0000);	/* タイマストップ */
												/* カウンタ値設定 */
	sil_wrh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCOR_OFFSET), TO_CLOCK(TIC_NUME, TIC_DENO));

	/* 割り込みレベル設定   */
	define_int_plevel ((VH*)(IOREG_INTC_BASE+IOREG_IPRG_OFFSET), TIMER_INTLVL, FRT_BIT_SHIFT);

	/*  割込み設定と要求をクリアとクロックセット */
	sil_wrh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCSR_OFFSET), (CMCSR_CMIE|CMCSR_CKS8));
	sil_wrh_mem((VP)IOREG_CMSTR_BASE, CMSTR_STR0);	/* タイマスタート */
}

/*
 *  タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear()
{
	sil_wrh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCSR_OFFSET),
		(sil_reh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCSR_OFFSET)) & ~CMCSR_CMF));
}

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
Inline void
hw_timer_terminate()
{
	sil_wrh_mem((VP)IOREG_CMSTR_BASE,			/* タイマの停止 */
		(sil_reh_mem((VP)IOREG_CMSTR_BASE) & ~CMSTR_STR0));
	sil_wrh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCSR_OFFSET), CMCSR_CKS8);/* 割込み停止と要因のクリア */
}

/*
 *  タイマの現在値の読出し
 *　　　タイマの動作を一時的に停止し，タイマ値を読み出す．
 */
Inline CLOCK
hw_timer_get_current(void)
{
	return sil_reh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCNT_OFFSET));
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return ((sil_reh_mem((VP)(IOREG_CMT0_BASE+IOREG_CMCSR_OFFSET)) & CMCSR_CMIE) != 0);
}

#endif /* _MACRO_ONLY */
#endif /* _HW_TIMER_H_ */
