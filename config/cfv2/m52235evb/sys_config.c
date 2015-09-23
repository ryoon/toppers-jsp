/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *  @(#) $Id: sys_config.c,v 1.7 2003/07/08 14:57:15 hiro Exp $
 */

/*
 *	ターゲットシステム依存モジュール（MCF52235EVB用）
 */

#include "jsp_kernel.h"
#include <sil.h>
#include "mcfuart.h"

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
    SIOPCB *siopcb;

    sil_wrb_mem (MCF_GPIO_PUAPAR, (MCF_GPIO_PUAPAR_RXD0_RXD0|MCF_GPIO_PUAPAR_TXD0_TXD0));
    sil_wrb_mem (MCF_GPIO_PUBPAR, (MCF_GPIO_PUBPAR_RXD1_RXD1|MCF_GPIO_PUBPAR_TXD1_TXD1));
    mcfuart_initialize ();
    siopcb = mcfuart_opn_por (LOGTASK_PORTID, 0);
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit()
{
	cfv2_exit();
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		cfv2_putc(LOGTASK_PORTID, '\r');
	}
	cfv2_putc(LOGTASK_PORTID, c);
}

/* ハードウェアの設定 */
#ifndef GDB_STUB


static void init_clock_config (void)
{
    sil_wrb_mem ((VP)MCF_CLK_CCHR,  (UB)MCF_CLK_CCHR_PFD(0x4));
    
    sil_wrh_mem ((VP)MCF_CLK_SYNCR,  (UH)0x4103);
    while (!(sil_reb_mem((VP)MCF_CLK_SYNSR) & 0x08));
    
    sil_wrh_mem ((VP)MCF_CLK_SYNCR,  (UH)0x4003);
    
    while (!(sil_reb_mem((VP)MCF_CLK_SYNSR) & 0x08));
    
    sil_wrh_mem ((VP)MCF_CLK_SYNCR,  (UH)0x4007);
}

void
hardware_init_hook (void)
{
  init_clock_config ();
  sil_wrw_mem ((VP) MCF_INTC_IMRH(MCF_INTC0), ~MCF_INTC_IMRH_ALL);
  sil_wrw_mem ((VP) MCF_INTC_IMRL(MCF_INTC0), ~MCF_INTC_IMRL_ALL);
  sil_wrw_mem ((VP) MCF_INTC_IMRH(MCF_INTC1), ~MCF_INTC_IMRH_ALL);
  sil_wrw_mem ((VP) MCF_INTC_IMRL(MCF_INTC1), ~MCF_INTC_IMRL_ALL);
  sil_wrw_mem ((VP) MCF_SCM_RAMBAR, (MCF_SCM_RAMBAR_BA(0x20000000) | MCF_SCM_RAMBAR_BDE));  
  return;
}
#else
void
hardware_init_hook (void)
{
}
#endif
