/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: $
 */

#ifndef _SH7047_H_
#define _SH7047_H_

/*
 *	7047.h -- definitions for SH2/7047F 
 */

/*	Values	*/

#define CLOCK_RATE		40000000L

/*
 *  割込みベクタ番号定義
 */
#define GII             4		/* 一般不当命令:General Illegal Instruction  */
#define SII             6		/* スロット不当命令:Slot Illegal Instruction */
#define CAE             9		/* CPUアドレスエラー:CPU Address Error       */
#define DTC             10		/* DTCアドレエラー */
#define DAE             DTC

#define TGIA_0          88		/* MTU0 */
#define TGIB_0          89		/* MTU0 */
#define TGIC_0          90		/* MTU0 */
#define TGID_0          91		/* MTU0 */
#define TCIV_0          92		/* MTU0 */
#define TGIA_1          96		/* MTU1 */
#define TGIB_1          97		/* MTU1 */
#define TCIV_1          100		/* MTU1 */
#define TCIU_1          101		/* MTU1 */
#define TGIA_2          104		/* MTU2 */
#define TGIB_2          105		/* MTU2 */
#define TCIV_2          108		/* MTU2 */
#define TCIU_2          109		/* MTU2 */
#define TGIA_3          112		/* MTU3 */
#define TGIB_3          113		/* MTU3 */
#define TGIC_3          114		/* MTU3 */
#define TGID_3          115		/* MTU3 */
#define TCIV_3          116		/* MTU3 */
#define TGIA_4          120		/* MTU4 */
#define TGIB_4          121		/* MTU4 */
#define TGIC_4          122		/* MTU4 */
#define TGID_4          123		/* MTU4 */
#define TCIV_4          124		/* MTU4 */

#define CMI0            144		/* CMT0 */
#define CMI1            145		/* CMT1 */

#define ERI_2           168		/* SCI2 */
#define RXI_2           169		/* SCI2 */
#define TXI_2           170		/* SCI2 */
#define TEI_2           171		/* SCI2 */
#define ERI_3           172		/* SCI3 */
#define RXI_3           173		/* SCI3 */
#define TXI_3           174		/* SCI3 */
#define TEI_3           175		/* SCI3 */
#define ERI_4           176		/* SCI4 */
#define RXI_4           177		/* SCI4 */
#define TXI_4           178		/* SCI4 */
#define TEI_4           179		/* SCI4 */

/*	Serial Channels		*/

#define IOREG_SCI_BASE		0xffff81C0
#define IOREG_SCI_WINDOW	16
#define IOREG_SMR_OFFSET	0x0
#define IOREG_BRR_OFFSET	0x1
#define IOREG_SCR_OFFSET	0x2
#define IOREG_TDR_OFFSET	0x3
#define IOREG_SSR_OFFSET	0x4
#define IOREG_RDR_OFFSET	0x5
#define IOREG_SDCR_OFFSET   0x6

/*	Status bits in SSR	*/

#define SSR_TDRE		0x80
#define SSR_RDRF		0x40
#define SSR_OPER		0x20
#define SSR_FER			0x10
#define SSR_PER			0x08
#define SSR_TEND		0x04
#define SSR_MPB			0x02
#define SSR_MPBT		0x01

/*	Status bits in SCR	*/

#define SCR_TIE			0x80
#define SCR_RIE			0x40
#define SCR_TE			0x20
#define SCR_RE			0x10
#define SCR_MPIE		0x08
#define SCR_TEIE		0x04
#define SCR_CKE			0x03

/* Status bits in SDCR */

#define SDCR_DIR		0x08

/* MTU */
#define IOREG_MTU_BASE      0xffff8200
#define IOREG_TCR3_OFFSET   0x0000
#define IOREG_TCR4_OFFSET   0x0001
#define IOREG_TMDR3_OFFSET  0x0002
#define IOREG_TMDR4_OFFSET  0x0003
#define IOREG_TIORH3_OFFSET 0x0004
#define IOREG_TIORL3_OFFSET 0x0005
#define IOREG_TIORH4_OFFSET 0x0006
#define IOREG_TIORL4_OFFSET 0x0007
#define IOREG_TIER3_OFFSET  0x0008
#define IOREG_TIER4_OFFSET  0x0009
#define IOREG_TCNT3_OFFSET  0x0010
#define IOREG_TCNT4_OFFSET  0x0012
#define IOREG_TGRA3_OFFSET  0x0018
#define IOREG_TGRB3_OFFSET  0x001a
#define IOREG_TGRA4_OFFSET  0x001c
#define IOREG_TGRB4_OFFSET  0x001e
#define IOREG_TGRC3_OFFSET  0x0024
#define IOREG_TGRD3_OFFSET  0x0026
#define IOREG_TGRC4_OFFSET  0x0028
#define IOREG_TGRD4_OFFSET  0x002a
#define IOREG_TSR3_OFFSET   0x002c
#define IOREG_TSR4_OFFSET   0x002d
#define IOREG_TSTR_OFFSET   0x0040
#define IOREG_TSYR_OFFSET   0x0041
#define IOREG_MTU0_BASE     0xffff8260
#define IOREG_TCR_OFFSET    0x0000
#define IOREG_TMDR_OFFSET   0x0001
#define IOREG_TIOR_OFFSET   0x0002
#define IOREG_TIORH0_OFFSET 0x0002
#define IOREG_TIORL0_OFFSET 0x0003
#define IOREG_TIER_OFFSET   0x0004
  #define TIER_TTGE         0x80
  #define TIER_TCIEU        0x20
  #define TIER_TCIEV        0x10
  #define TIER_TGIED        0x08
  #define TIER_TGIEC        0x04
  #define TIER_TGIEB        0x02
  #define TIER_TGIEA        0x01
#define IOREG_TSR_OFFSET    0x0005
#define IOREG_TCNT_OFFSET   0x0006
#define IOREG_TGRA_OFFSET   0x0008
#define IOREG_TGRB_OFFSET   0x000a
#define IOREG_TGRC_OFFSET   0x000c
#define IOREG_TGRD_OFFSET   0x000e
#define IOREG_MTU1_BASE     0xffff8280
#define IOREG_MTU2_BASE     0xffff82a0

/* Interrupt controller */

#define IOREG_INTC_BASE		0xffff8348
#define IOREG_IPRA_OFFSET	0x0000
#define IOREG_IPRD_OFFSET	0x0006
#define IOREG_IPRE_OFFSET	0x0008
#define IOREG_IPRF_OFFSET	0x000a
#define IOREG_IPRG_OFFSET	0x000c
#define IOREG_IPRH_OFFSET	0x000e
#define IOREG_ICR1_OFFSET	0x0010
#define IOREG_ISR_OFFSET	0x0012
#define IOREG_IPRI_OFFSET	0x0014
  #define ISR_SCI2_SHIFT	12
  #define ISR_SCR3_SHIFT	8
  #define ISR_SCR4_SHIFT	4
#define IOREG_IPRK_OFFSET	0x0018
#define IOREG_ICR2_OFFSET	0x001e

/* Parallel Port A Data Register */
#define IOREG_PADRL_BASE    0xffff8382

/* Parallel Port Controller */
#define IOREG_PFC_BASE		0xffff8386
#define IOREG_PAIORL_OFFSET	0x0000
#define IOREG_PACRL3_OFFSET	0x0004
#define IOREG_PACRL1_OFFSET	0x0006
#define	IOREG_PACRL2_OFFSET	0x0008
#define IOREG_PBIOR_OFFSET	0x000e
#define IOREG_PBCR1_OFFSET	0x0012
#define IOREG_PBCR2_OFFSET	0x0014
#define IOREG_PDIORL_OFFSET	0x0020
#define IOREG_PDCRL1_OFFSET	0x0026
#define IOREG_PDCRL2_OFFSET	0x0028
#define IOREG_PEIORL_OFFSET	0x002e
#define IOREG_PEIORH_OFFSET	0x0030
#define	IOREG_PECRL1_OFFSET	0x0032
#define IOREG_PECRL2_OFFSET	0x0034
#define IOREG_PECRH_OFFSET	0x0036

/* Compare Match Timer */
#define IOREG_CMSTR_BASE    0xffff83d0
#define IOREG_CMT0_BASE     0xffff83d2
#define IOREG_CMCSR_OFFSET  0x0000
#define IOREG_CMCNT_OFFSET  0x0002
#define IOREG_CMCOR_OFFSET  0x0004
#define IOREG_CMT1_BASE     0xffff83d8


/* Module Standby Controller */

#define IOREG_MST_BASE		0xffff861c
#define IOREG_CR1_OFFSET	0x0000
#define IOREG_CR2_OFFSET	0x0002
  #define MST_MSTP13        0x2000

/* Bass State Controller */

#define IOREG_BSC_BASE		0xffff8620
#define IOREG_BCR1_OFFSET	0x0000
#define IOREG_BCR2_OFFSET	0x0002
#define	IOREG_WCR1_OFFSET	0x0004

#endif	/* of #ifndef _SH7047_H_ */
