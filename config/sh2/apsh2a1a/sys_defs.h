/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
 *  Copyright (C) 2010 by Cronus Computer Works, JAPAN
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
 *  @(#) $Id
 */

/*
 *	ターゲットシステムに依存する定義（APSH2A1A用）
 *　　　　　アプリケーションが必要とする定義
 *
 *  このインクルードファイルは，kernel.h と sil.h でインクルードされる．
 *  他のファイルから直接インクルードすることはない．このファイルをイン
 *  クルードする前に，t_stddef.h と itron.h がインクルードされるので，
 *  それらに依存してもよい．
 */

#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

#define APSH2A1A			/* システム略称 */

#define COPYRIGHT_SYS \
"Copyright (C) 2010 by Cronus Computer Works, JAPAN\n\n"

#ifndef _MACRO_ONLY

/*
 *  割込み番号の型と割込みの禁止／許可
 */
#if 0 /* 現時点ではサポートしていない */

typedef	UINT	INTNO;		/* 割込み番号 */

extern ER	dis_int(INTNO intno) throw();
extern ER	ena_int(INTNO intno) throw();

#endif /* 0 */

#endif /* _MACRO_ONLY */

/*
 *  キャッシュモードの設定
 */
//#define CCR_MODE      0x00000101    /* ライトバックモード */
#define CCR_MODE      0x00000103    /* ライトスルーモード */

/*
 *  周辺モジュールへの供給クロックの周波数
 *  APSH2A1Aでは、PΦクロックは33.333MHz
 */
#define PCLOCK          33333000

/*
 *  タイムティックの定義
 */
#define	TIC_NUME	1u	/* タイムティックの周期の分子 */
#define	TIC_DENO	1u	/* タイムティックの周期の分母 */

/*
 *   タイマへの入力クロックの分周比設定
 *
 *   APSH2A1Aでは、PΦクロックが33.333Mhz
 *   0: PΦ/8   4.166MHz  tick 4166  0.24 μsec
 *   1: PΦ/32  1.041MHz  tick 1041  0.96 μsec
 *   2: PΦ/128 260.4KHz  tick 260   3.84 μsec
 *   3: PΦ/512 65.103KHz tick 65    15.36 μsec
 */
#define TCR_CKS         0

/*
 *  SDRAMモードレジスタのアドレス
 *  ※アドレスがモード設定値
 */
#define SDRAM_MODE  ((VH *)0xfffc58c0)


#ifndef _MACRO_ONLY

/*
 *  システムの停止処理
 */
#ifdef GDB_STUB

Inline void
kernel_abort()
{
	/* テストしていない */
	Asm("trapa #0x20"::);
}

#else /* GDB_STUB */

Inline void
kernel_abort()
{
	while(1);
}

#endif /* GDB_STUB */
#endif /* _MACRO_ONLY */
#endif /* _SYS_DEFS_H_ */
