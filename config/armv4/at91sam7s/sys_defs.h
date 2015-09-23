/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *                2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2005 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  ターゲットシステムに依存する定義（At91sam7s用）
 *
 *  このインクルードファイルは，kernel.h と sil.h でインクルードされる．
 *  他のファイルから直接インクルードすることはない．このファイルをイン
 *  クルードする前に，t_stddef.h と itron.h がインクルードされるので，
 *  それらに依存してもよい．
 */

#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

/*
 *  起動メッセージのターゲットシステム名
 */
#define TARGET_NAME "ARM - AT91SAM7S"
#define AT91SAM7S

/* カーネル起動時のメッセージ   */
#define COPYRIGHT_SYS \
"Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN\n"

#ifdef VECTOR_IN_ROM
#define RAM_START       0x00200000
#else  /* VECTOR_IN_ROM */
#define RAM_START       0x00000000
#endif /* VECTOR_IN_ROM */

#ifndef RAM_SIZE
#if defined (__AT91SAM7S128__)
#define RAM_SIZE        (32*1024)
#elif defined (__AT91SAM7S256__)
#define RAM_SIZE        (64*1024)
#endif
#endif

#define FIQ_DATA_SIZE   256
#define STACKTOP        (RAM_START+RAM_SIZE-FIQ_DATA_SIZE)  /* 非タスクコンテキスト用のスタックの初期値 */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK     5990

/*
 *  VECTOR MODE
 */
//#define VECTOR_IN_ROM

/*
 *  ターゲットシステム依存のサービスコール
 */
#ifndef _MACRO_ONLY


/*
 *  INTNO型と割込みの禁止/許可
 */
typedef    UINT        INTNO;        /* 割込み番号 */
typedef    UINT        INHNO;        /* 割込みハンドラ番号 */


typedef    UD  SYSUTIM;    /* 性能評価用システム時刻 */
extern ER   vxget_tim(SYSUTIM *pk_sysutim) throw();

#endif /* _MACRO_ONLY */


/*
 *  タイムティックの定義
 */
#define    TIC_NUME    1        /* タイムティックの周期の分子 */
#define    TIC_DENO    1        /* タイムティックの周期の分母 */

/*
 *  微少時間待ちのための定義
 */
#define    SIL_DLY_TIM1     20
#define    SIL_DLY_TIM2     10

/*
 *  プロセッサのエンディアン
 */
#define    SIL_ENDIAN    SIL_ENDIAN_LITTLE    /* リトルエンディアン */

#ifndef _MACRO_ONLY
/*
 * システムの中断処理
 */

Inline void
kernel_abort()
{
   while(1);
}

#endif /* _MACRO_ONLY */

#endif /* _SYS_DEFS_H_ */
