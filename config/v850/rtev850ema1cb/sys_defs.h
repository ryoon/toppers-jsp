/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: sys_defs.h,v 1.2 2001/11/15 13:18:32 takayuki Exp $
 */

/*
 *	ターゲットシステムに依存する定義
 *    RTE-V850E/MA1-CB uPD70F3107 50MHz
 */

#include "sys_config.h"

#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

#define RTEV850EMA1CB

/*
 * メモリマップドレジスタ
 */

#define ISPR  0xfffff1fa

#define PHCMD 0x0ffff800
#define CKC   0x0ffff822

#define IMR0  0x0ffff100
#define IMR1  0x0ffff102
#define IMR2  0x0ffff104
#define IMR3  0x0ffff106

#define SRIC0 0x0ffff15c
#define STIC0 0x0ffff15e
#define ASIM0 0x0ffffa00
#define RXB0  0x0ffffa02
#define ASIS0 0x0ffffa03
#define TXB0  0x0ffffa04
#define ASIF0 0x0ffffa05
#define CKSR0 0x0ffffa06
#define BRGC0 0x0ffffa07

#define LED1  0x07802000
#define LED2  0x07802001

#define BCT0  0x0ffff480
#define BCT1  0x0ffff482
#define DWC0  0x0ffff484
#define DWC1  0x0ffff486
#define BCC   0x0ffff488
#define VSWC  0x0ffff06e
#define BCP   0x0ffff48c
#define ASC   0x0ffff48a
#define CSC0  0x0ffff060
#define CSC1  0x0ffff062
#define BSC   0x0ffff066
#define BEC   0x0ffff068
#define SCR1  0x0ffff4a4
#define RFS1  0x0ffff4a6

#define PMC4  0x0ffff448
#define PFC4  0x0ffff468

	/* タイマ関連レジスタ */
#define TMD(x)   (0x0ffff540 + ((x) * 0x10))
#define CMD(x)   (0x0ffff540 + ((x) * 0x10) + 2)
#define TMCD(x)  (0x0ffff540 + ((x) * 0x10) + 4)
#define CMICD(x) (0x0ffff148 + ((x) * 2))

	/* シリアル関連レジスタ */
#define ASIM(x)  (0x0ffffa00 + ((x) * 0x10))
#define ASIS(x)  (0x0ffffa00 + ((x) * 0x10) + 3)
#define ASIF(x)  (0x0ffffa00 + ((x) * 0x10) + 5)
#define RXB(x)   (0x0ffffa00 + ((x) * 0x10) + 2)
#define TXB(x)   (0x0ffffa00 + ((x) * 0x10) + 4)
#define CKSR(x)  (0x0ffffa00 + ((x) * 0x10) + 6)
#define BRGC(x)  (0x0ffffa00 + ((x) * 0x10) + 7)
#define SRIC(x)  (0x0ffff15c + ((x) * 8))
#define STIC(x)  (0x0ffff15c + ((x) * 8) + 1)

/*
 *  INTNO型と割込みの禁止/許可
 */
#ifndef _MACRO_ONLY

typedef	UINT		INTNO;		/* 割込み番号 */

extern ER	dis_int(INTNO intno);
extern ER	ena_int(INTNO intno);

#endif /* _MACRO_ONLY */

/*
 *  ターゲットシステム依存のサービスコール
 */
#ifndef _MACRO_ONLY

typedef	UD		SYSUTIM;	/* 性能評価用システム時刻 */

extern ER	vxget_tim(SYSUTIM *pk_sysutim);

#endif /* _MACRO_ONLY */

/*
 *  タイムティックの定義
 */
#define	TIC_NUME	1		/* タイムティックの周期の分子 */
#define	TIC_DENO	1		/* タイムティックの周期の分母 */

#endif /* _SYS_DEFS_H_ */
