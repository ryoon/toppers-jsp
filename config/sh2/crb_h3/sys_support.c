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


#include "jsp_kernel.h"
#include "sh7047.h"
#include <sil.h>

void hardware_init_hook(void)
{
	unsigned long *p = (unsigned long *)SRAM_START;

	/* initial Module Standby register */
	sil_wrh_mem((VP)(IOREG_MST_BASE+IOREG_CR1_OFFSET), 0xf023);	/* sci4,sci3,sci2	*/
	sil_wrh_mem((VP)(IOREG_MST_BASE+IOREG_CR2_OFFSET), 0xe0f0);	/* cmt */

	/* initial base state */
	sil_wrh_mem((VP)(IOREG_BSC_BASE+IOREG_BCR1_OFFSET), 0x600e);/* CS0:8bit */
	sil_wrh_mem((VP)(IOREG_BSC_BASE+IOREG_BCR2_OFFSET), 0xffff);/* CS0:idle cycle 3*/
	sil_wrh_mem((VP)(IOREG_BSC_BASE+IOREG_WCR1_OFFSET), 0xfff2);/* CS0 2Wait */

	/* ＩＯポート設定 */
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PACRL1_OFFSET), 0x1115);	/*	0001 0001 0001 0101	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PACRL2_OFFSET), 0x4000);	/*	0100 0000 0000 0000	*/	/*	PA6	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PACRL3_OFFSET), 0x03bf);	/*	0000 0011 1011 1111	*/	/*	PA6	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PAIORL_OFFSET), 0x5ebf);	/*	0101 1110 1011 1111	*/

	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PBCR1_OFFSET), 0x2c00);	/*	0010 1100 0000 0000	*/	/*	TXD4,RXD4	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PBCR2_OFFSET), 0x04f5);	/*	0000 0100 1111 0101	*/	/*	TXD4,RXD4	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PBIOR_OFFSET), 0x0023);	/*	0000 0000 0010 0011	*/	/*	TXD4,RXD4	*/

	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PDCRL1_OFFSET), 0x00ff);	/*	0000 0000 1111 1111	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PDCRL2_OFFSET), 0x0000);	/*	0000 0000 0000 0000	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PDIORL_OFFSET), 0x0000);	/*	0000 0000 0000 0000	*/

	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PECRH_OFFSET), 0x0fff);	/*	0000 1111 1111 1111	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PECRL1_OFFSET), 0x0000);	/*	0000 0000 0000 0000	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PECRL2_OFFSET), 0xff00);	/*	1111 1111 0000 0000	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PEIORH_OFFSET), 0x003f);	/*	0000 0000 0011 1111	*/
	sil_wrh_mem((VP)(IOREG_PFC_BASE+IOREG_PEIORL_OFFSET), 0x00f0);	/*	0000 0000 1111 0000	*/

	/* SRAM clear */
	while(p < (unsigned long *)SRAM_END){
		*p++ = 0L;
	}
}

/*
 * 割り込みコントローラの初期化
 */
void sh2_init_intcontorller (void)
{
	sil_wrh_mem((VP)(IOREG_INTC_BASE+IOREG_IPRA_OFFSET), 0x0000);
	sil_wrh_mem((VP)(IOREG_INTC_BASE+IOREG_IPRD_OFFSET), 0x0000);
	sil_wrh_mem((VP)(IOREG_INTC_BASE+IOREG_IPRE_OFFSET), 0x0000);
	sil_wrh_mem((VP)(IOREG_INTC_BASE+IOREG_IPRF_OFFSET), 0x0000);
	sil_wrh_mem((VP)(IOREG_INTC_BASE+IOREG_IPRG_OFFSET), 0x0000);
	sil_wrh_mem((VP)(IOREG_INTC_BASE+IOREG_IPRH_OFFSET), 0x0000);
}

