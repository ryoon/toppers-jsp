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
 *  @(#) $Id: sys_config.c,v 1.3 2004/10/04 12:18:45 honda Exp $
 */

#include "jsp_kernel.h"
#include <sil.h>
#ifndef GDB_STUB
#include "sh7615scif1.h"
#else	/*  GDB_STUB  */
#include "sh7615scif2.h"
#endif	/*  GDB_STUB  */
#include "hsb7616it.h"

/*
 *  シリアルI/Oポートの初期化 banner出力のためカーネルの初期化と無関係に行う
 */
Inline void
Sci_Initialize ()
{
	sil_wrh_mem (SCIF_SCSCR,
				 sil_reh_mem (SCIF_SCSCR) & ~(SCSCR_TE | SCSCR_RE));
#ifndef GDB_STUB
	sil_wrh_mem (PBCR, sil_reh_mem (PBCR) | (PFC_TXD | PFC_RXD));
#else	/*  GDB_STUB  */
	sil_wrh_mem (PBCR2, sil_reh_mem (PBCR2) | (PFC_TXD | PFC_RXD));
#endif	/*  GDB_STUB  */
	sil_wrb_mem (SCIF_SCFCR, (VB) (SCFCR_TFRST | SCFCR_RFRST));
	sil_wrb_mem (SCIF_SCSMR, 0x00);
	sil_wrb_mem (SCIF_SCBRR, (VB) SCI_BRR);
	sil_dly_nse (sh2scif_DELAY);
	sil_wrb_mem (SCIF_SCFCR, 0x00);
	sil_wrh_mem (SCIF_SC1SSR, sil_reh_mem (SCIF_SC1SSR) & ~SC1SSR_ER);
	sil_wrb_mem (SCIF_SCSCR, (VB) (SCSCR_RIE | SCSCR_TE | SCSCR_RE));
}

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize ()
{
	//バナーを出力するためシリアルポートを初期化しておく
	Sci_Initialize ();

#ifndef GDB_STUB
	/* ベクタ番号の初期化 */

	sil_wrw_mem (DMA_VCRDMA0, 72);
	sil_wrw_mem (DMA_VCRDMA1, 73);

	sil_wrh_mem (VCRWDT, 74 * 0x100 + 75);
	sil_wrh_mem (VCRA, 76 * 0x100);
	sil_wrh_mem (VCRC, 77 * 0x100 + 78);
	sil_wrh_mem (VCRD, 79 * 0x100);
	sil_wrh_mem (VCRE, 80 * 0x100 + 81);
	sil_wrh_mem (VCRF, 82 * 0x100 + 83);
	sil_wrh_mem (VCRG, 84 * 0x100);
	sil_wrh_mem (VCRH, 85 * 0x100 + 86);
	sil_wrh_mem (VCRI, 87 * 0x100 + 88);
	sil_wrh_mem (VCRJ, 89 * 0x100 + 90);
	sil_wrh_mem (VCRK, 91 * 0x100 + 92);
	sil_wrh_mem (VCRL, 93 * 0x100 + 94);
	sil_wrh_mem (VCRM, 95 * 0x100 + 96);
	sil_wrh_mem (VCRN, 97 * 0x100 + 98);
	sil_wrh_mem (VCRO, 99 * 0x100 + 100);
	sil_wrh_mem (VCRP, 101 * 0x100 + 102);
	sil_wrh_mem (VCRQ, 103 * 0x100 + 104);
	sil_wrh_mem (VCRR, 105 * 0x100 + 106);
	sil_wrh_mem (VCRS, 107 * 0x100 + 108);
	sil_wrh_mem (VCRT, 109 * 0x100 + 110);
	sil_wrh_mem (VCRU, 111 * 0x100 + 112);
#endif	/*  GDB_STUB  */

}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit()
{
	sh2_exit();
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		sh2_putc('\r');
	}
	sh2_putc(c);
}

/* ハードウェアの設定 */
void
hardware_init_hook (void)
{
}
