/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: h8s_2350.h,v 1.2 2002/04/13 13:15:41 imai Exp $
 */

#ifndef _H8S_2350_H_
#define _H8S_2350_H_

/*
 *  H8S/2350 用定義
 */

/*
 *  数値データ文字列化用マクロ
 */

#define TO_STRING(arg)	#arg
#define _TO_STRING(arg)	TO_STRING(arg)

/* コンディションコードレジスタ(CCR) */

#define H8SCCR_I_BIT		7
#define H8SCCR_UI_BIT		6
#define H8SCCR_H_BIT		5
#define H8SCCR_U_BIT		4
#define H8SCCR_N_BIT		3
#define H8SCCR_Z_BIT		2
#define H8SCCR_V_BIT		1
#define H8SCCR_C_BIT		0

#define H8SCCR_I			(1<<H8SCCR_I_BIT)
#define H8SCCR_UI			(1<<H8SCCR_UI_BIT)
#define H8SCCR_H			(1<<H8SCCR_H_BIT)
#define H8SCCR_U			(1<<H8SCCR_U_BIT)
#define H8SCCR_N			(1<<H8SCCR_N_BIT)
#define H8SCCR_Z			(1<<H8SCCR_Z_BIT)
#define H8SCCR_V			(1<<H8SCCR_V_BIT)
#define H8SCCR_C			(1<<H8SCCR_C_BIT)

/*  エクステンドレジスタ(EXR) */

#define H8SEXR_T_BIT		7
#define H8SEXR_I2_BIT		2
#define H8SEXR_I1_BIT		1
#define H8SEXR_I0_BIT		0

#define H8SEXR_T			(1<<H8SEXR_T_BIT)
#define H8SEXR_I2			(1<<H8SEXR_I2_BIT)
#define H8SEXR_I1			(1<<H8SEXR_I1_BIT)
#define H8SEXR_I0			(1<<H8SEXR_I0_BIT)


/* 例外処理ベクタ番号 */

#define IRQ_NMI			7		/* NMI */

#define IRQ_EXT0		16		/* IRQ0 */
#define IRQ_EXT1		17		/* IRQ1 */
#define IRQ_EXT2		18		/* IRQ2 */
#define IRQ_EXT3		19		/* IRQ3 */
#define IRQ_EXT4		20		/* IRQ4 */
#define IRQ_EXT5		21		/* IRQ5 */
#define IRQ_EXT6		22		/* IRQ6 */
#define IRQ_EXT7		23		/* IRQ7 */

#define IRQ_SWDTEND		24		/* DTC */

#define IRQ_WOVI		25		/* Match Dog Timer */

#define IRQ_CMI 		26		/* Refresh Controller */

#define IRQ_ADI	     		28		/* A/D */

#define IRQ_TGI0A		32		/* TPU0 TGI0A */
#define IRQ_TGI0B		33		/* TPU0 TGI0B */
#define IRQ_TGI0C		34		/* TPU0 TGI0C */
#define IRQ_TGI0D		35		/* TPU0 TGI0D */
#define IRQ_TCI0V		36		/* TPU0 TCI0V */

#define IRQ_TGI1A		40		/* TPU1 TGI1A */
#define IRQ_TGI1B		41		/* TPU1 TGI1B */
#define IRQ_TCI1V		42		/* TPU1 TCI1V */
#define IRQ_TCI1U		43		/* TPU1 TCI1U */

#define IRQ_TGI2A		44		/* TPU2 TGI2A */
#define IRQ_TGI2B		45		/* TPU2 TGI2B */
#define IRQ_TCI2V		46		/* TPU2 TCI2V */
#define IRQ_TCI2U		47		/* TPU2 TCI2U */

#define IRQ_TGI3A		48		/* TPU3 TGI3A */
#define IRQ_TGI3B		49		/* TPU3 TGI3B */
#define IRQ_TGI3C		50		/* TPU3 TGI3C */
#define IRQ_TGI3D		51		/* TPU3 TGI3D */
#define IRQ_TCI3V		52		/* TPU3 TCI3V */

#define IRQ_TGI4A		56		/* TPU4 TGI4A */
#define IRQ_TGI4B		57		/* TPU4 TGI4B */
#define IRQ_TCI4V		58		/* TPU4 TCI4V */
#define IRQ_TCI4U		59		/* TPU4 TCI4U */

#define IRQ_TGI5A		60		/* TPU5 TGI5A */
#define IRQ_TGI5B		61		/* TPU5 TGI5B */
#define IRQ_TCI5V		62		/* TPU5 TCI5V */
#define IRQ_TCI5U		63		/* TPU5 TCI5U */

#define IRQ_DEND0A		72		/* DMAC */
#define IRQ_DEND0B		73		/* DMAC */
#define IRQ_DEND1A		74		/* DMAC */
#define IRQ_DEND1B		75		/* DMAC */

#define IRQ_ERI0		80		/* SCI0 ERI */
#define IRQ_RXI0		81		/* SCI0 RXI */
#define IRQ_TXI0		82		/* SCI0 TXI */
#define IRQ_TEI0		83		/* SCI0 TEI */

#define IRQ_ERI1		84		/* SCI1 ERI */
#define IRQ_RXI1		85		/* SCI1 RXI */
#define IRQ_TXI1		86		/* SCI1 TXI */
#define IRQ_TEI1		87		/* SCI1 TEI */


/* レジスタアドレス */

/*  モジュールストップコントロールレジスタ  */
#define MSTPCR			(*(volatile UH *)0xffff3c)
#define MSTPCR_TPU		0x2000	/*  タイマ  */
#define MSTPCR_SCI0		0x0020	/*  SCI0  */
#define MSTPCR_SCI1		0x0040	/*  SCI1  */


/* ---I/Oポート-------------------------------------------- */

/* port1 */

#define H8SP1DDR		0xfffeb0
#define H8SP1DR			0xffff60
#define H8SPORT1		0xffff50

/* port2 */

#define H8SP2DDR		0xfffeb1
#define H8SP2DR			0xffff61
#define H8SPORT2		0xffff51

/* port3 */

#define H8SP3DDR		0xfffeb2
#define H8SP3DR			0xffff62
#define H8SPORT3     		0xffff52
#define H8SP3ODR		0xffff76

/* port4 */

#define H8SPORT4		0xffff53

/* port5 */

#define H8SP5DDR		0xfffeb4
#define H8SP5DR			0xffff64
#define H8SPORT5		0xffff54

/* port6 */

#define H8P6DDR			0xfffeb5
#define H8P6DR			0xffff65
#define H8SPORT6 		0xffff55

/*  portA  */
#define H8SPADDR		0xfffeb9
#define H8SPADR			0xffff69
#define H8SPORTA		0xffff59
/*  H8S/2351のみ  */
#define H8SPAPCR  		0xffff70
#define H8SPAODR		0xffff77

/*  portB  */
/*  H8S/2351のみ  */
#define H8SPBDDR		0xfffeba
#define H8SPBDR			0xffff6a
#define H8SPORTB		0xffff5a
#define H8SPBPCR		0xffff71

/*  portC  */
/*  H8S/2351のみ  */
#define H8SPCDDR		0xfffebb
#define H8SPCDR			0xffff6b
#define H8SPORTC		0xffff5b
#define H8SPCPCR		0xffff72

/*  portD  */
/*  H8S/2351のみ  */
#define H8SPDDDR		0xfffebc
#define H8SPDDR			0xffff6c
#define H8SPORTD		0xffff5c
#define H8SPDPCR		0xffff73

/*  portE  */
#define H8SPEDDR		0xfffebd
#define H8SPEDR			0xffff6d
#define H8SPORTE		0xffff5d
#define H8SPEPCR		0xffff74

/*  portF  */
#define H8SPFDDR		0xfffebe
#define H8SPFDR			0xffff6e
#define H8SPORTF		0xffff5e

/*  portG  */
#define H8SPGDDR		0xfffebf
#define H8SPGDR			0xffff6f
#define H8SPORTG		0xffff5f


/* --- MCU動作モード----------------------------------------  */
#define H8SMDCR			0xffff3b
#define H8SSYSCR		0xffff39

/*  モードコントロールレジスタ(MDCR)  */

/*  システムコントロールレジスタ(SYSCR)  */
#define H8SSYSCR_INTM1_BIT 	5
#define H8SSYSCR_INTM0_BIT	4
#define H8SSYSCR_NMIEG_BIT 	3
#define H8SSYSCR_RNAME_BIT	0

#define H8SSYSCR_INTM1		(1<<H8SSYSCR_INTM1_BIT)
#define H8SSYSCR_INTM0		(1<<H8SSYSCR_INTM0_BIT)
#define H8SSYSCR_NMIEG		(1<<H8SSYSCR_NMIEG_BIT)
#define H8SSYSCR_RNAME		(1<<H8SSYSCR_RNAME_BIT)


/*  ---割込みコントローラ------------------------------------  */

#define H8SSYSCR		0xffff39
#define H8SISCRH		0xffff2c
#define H8SISCRL		0xffff2d
#define H8SIER			0xffff2e
#define H8SISR			0xffff2f
#define H8SIPRA			0xfffec4
#define H8SIPRB 		0xfffec5
#define H8SIPRC			0xfffec6
#define H8SIPRD			0xfffec7
#define H8SIPRE			0xfffec8
#define H8SIPRF			0xfffec9
#define H8SIPRG			0xfffeca
#define H8SIPRH			0xfffecb
#define H8SIPRI			0xfffecc
#define H8SIPRJ			0xfffecd
#define H8SIPRK			0xfffece

/*  割込みコントローラ　システムコントロールレジスタ(SYSCR)  */
/* 「MCU動作モード」にて設定  */


/* ---16ビットタイマパルスユニット(TPU)----------------------- */

/*  共通  */
#define H8STPU_TSTR			0xffffc0
#define H8STPU_TSYR			0xffffc1
#define H8STPU_MSTPCR	 		0xffff3c

/*  ベースアドレス  */
#define H8STPU0			0xffffd0
#define H8STPU1			0xffffe0
#define H8STPU2			0xfffff0
#define H8STPU3			0xfffe80
#define H8STPU4			0xfffe90
#define H8STPU5			0xfffea0

/*  アドレスオフセット
 *  (注意：チャネル1，2，4，5では、タイマジェネラルレジスタC，Dはサポート
 *  しておらず、それに伴い、タイマI/Oコントロールレジスタは１本のみである。
 *  また、チャネル0，3用レジスタとチャネル1，2，4，5用レジスタでは、各レジスタ
 *　内のビットの使用方法に多少の違いがある。)
 */
#define H8STCR  	  	0
#define H8STMDR 		1
#define H8STIORH		2
#define H8STIORL		3
#define H8STIER 		4
#define H8STSR			5
#define H8STCNT 		6
#define H8STGRA			8
#define H8STGRB			a
#define H8STGRC 		c
#define H8STGRD 		e

/* TPU タイマスタートレジスタ (TSTR) */
#define H8STSTR_CST5_BIT	5
#define H8STSTR_CST4_BIT	4
#define H8STSTR_CST3_BIT	3
#define H8STSTR_CST2_BIT	2
#define H8STSTR_CST1_BIT	1
#define H8STSTR_CST0_BIT	0

#define H8STSTR_CST5		(1<<H8STSTR_CST5_BIT)
#define H8STSTR_CST4		(1<<H8STSTR_CST4_BIT)
#define H8STSTR_CST3		(1<<H8STSTR_CST3_BIT)
#define H8STSTR_CST2		(1<<H8STSTR_CST2_BIT)
#define H8STSTR_CST1		(1<<H8STSTR_CST1_BIT)
#define H8STSTR_CST0		(1<<H8STSTR_CST0_BIT)


/* TPU タイマシンクロレジスタ(TSYR) */
#define H8STSYR_SYNC5_BIT	5
#define H8STSYR_SYNC4_BIT	4
#define H8STSYR_SYNC3_BIT	3
#define H8STSYR_SYNC2_BIT	2
#define H8STSYR_SYNC1_BIT	1
#define H8STSYR_SYNC0_BIT	0

#define H8STSYR_SYNC5		(1<<H8STSYR_SYNC5_BIT)
#define H8STSYR_SYNC4		(1<<H8STSYR_SYNC4_BIT)
#define H8STSYR_SYNC3		(1<<H8STSYR_SYNC3_BIT)
#define H8STSYR_SYNC2		(1<<H8STSYR_SYNC2_BIT)
#define H8STSYR_SYNC1		(1<<H8STSYR_SYNC1_BIT)
#define H8STSYR_SYNC0		(1<<H8STSYR_SYNC0_BIT)


/* TPU ストップモジュールコントロールレジスタ(MSTPCR) */
#define H8SMSTPCR_H15_BIT	15
#define H8SMSTPCR_H14_BIT	14
#define H8SMSTPCR_H13_BIT	13
#define H8SMSTPCR_H12_BIT	12
#define H8SMSTPCR_H11_BIT	11
#define H8SMSTPCR_H10_BIT	10
#define H8SMSTPCR_H9_BIT	9
#define H8SMSTPCR_H8_BIT	8
#define H8SMSTPCR_L7_BIT	7
#define H8SMSTPCR_L6_BIT	6
#define H8SMSTPCR_L5_BIT	5
#define H8SMSTPCR_L4_BIT	4
#define H8SMSTPCR_L3_BIT	3
#define H8SMSTPCR_L2_BIT	2
#define H8SMSTPCR_L1_BIT	1
#define H8SMSTPCR_L0_BIT	0
/*  省略あり  */

/*  TPU  タイマコントロールレジスタ(TCR) 
 * (注意：チャネル1，2，4，5では、ビット7はリザーブビット)
 */
#define H8STCR_CCLR2_BIT 	7
#define H8STCR_CCLR1_BIT	6
#define H8STCR_CCLR0_BIT	5
#define H8STCR_CKEG1_BIT	4
#define H8STCR_CKEG0_BIT	3
#define H8STCR_TPSC2_BIT	2
#define H8STCR_TPSC1_BIT	1
#define H8STCR_TPSC0_BIT	0

#define H8STCR_CCLR2 		(1<<H8STCR_CCLR2_BIT)
#define H8STCR_CCLR1		(1<<H8STCR_CCLR1_BIT)
#define H8STCR_CCLR0		(1<<H8STCR_CCLR0_BIT)
#define H8STCR_CKEG1		(1<<H8STCR_CKEG1_BIT)
#define H8STCR_CKEG0		(1<<H8STCR_CKEG0_BIT)
#define H8STCR_TPSC2		(1<<H8STCR_TPSC2_BIT)
#define H8STCR_TPSC1		(1<<H8STCR_TPSC1_BIT)
#define H8STCR_TPSC0		(1<<H8STCR_TPSC0_BIT)


/*  TPU タイマモードレジスタ(TMDR) 
 *  (注意：チャネル1，2，4，5では、ビット5，4はリザーブビット)
 */
#define H8STMDR_BFB_BIT	5
#define H8STMDR_BFA_BIT	4
#define H8STMDR_MD3_BIT	3
#define H8STMDR_MD2_BIT	2
#define H8STMDR_MD1_BIT	1
#define H8STMDR_MD0_BIT	0

#define H8STMDR_BFB		(1<<H8STMDR_BFB_BIT)
#define H8STMDR_BFA		(1<<H8STMDR_BFA_BIT)
#define H8STMDR_MD3		(1<<H8STMDR_MD3_BIT)
#define H8STMDR_MD2		(1<<H8STMDR_MD2_BIT)
#define H8STMDR_MD1		(1<<H8STMDR_MD1_BIT)
#define H8STMDR_MD0		(1<<H8STMDR_MD0_BIT)

/*  TPU タイマI/Oコントロールレジスタ(TIORH)  */
#define H8STIORH_IOB3_BIT	7
#define H8STIORH_IOB2_BIT	6
#define H8STIORH_IOB1_BIT	5
#define H8STIORH_IOB0_BIT	4
#define H8STIORH_IOA3_BIT	3
#define H8STIORH_IOA2_BIT	2
#define H8STIORH_IOA1_BIT	1
#define H8STIORH_IOA0_BIT	0

#define H8STIORH_IOB3		(1<<H8STIORH_IOB3_BIT)
#define H8STIORH_IOB2		(1<<H8STIORH_IOB2_BIT)
#define H8STIORH_IOB1		(1<<H8STIORH_IOB1_BIT)
#define H8STIORH_IOB0		(1<<H8STIORH_IOB0_BIT)
#define H8STIORH_IOA3		(1<<H8STIORH_IOA3_BIT)
#define H8STIORH_IOA2		(1<<H8STIORH_IOA2_BIT)
#define H8STIORH_IOA1		(1<<H8STIORH_IOA1_BIT)
#define H8STIORH_IOA0		(1<<H8STIORH_IOA0_BIT)

/*
 *  TPU タイマI/Oコントロールレジスタ(TIORL) 
 *  (注意：チャネル1，2，4，5では、サポートしない。)
 */
#define H8STIORL_IOD3_BIT	7
#define H8STIORL_IOD2_BIT	6
#define H8STIORL_IOD1_BIT	5
#define H8STIORL_IOD0_BIT	4
#define H8STIORL_IOC3_BIT	3
#define H8STIORL_IOC2_BIT	2
#define H8STIORL_IOC1_BIT	1
#define H8STIORL_IOC0_BIT	0

#define H8STIORL_IOD3		(1<<H8STIORL_IOD3_BIT)
#define H8STIORL_IOD2		(1<<H8STIORL_IOD2_BIT)
#define H8STIORL_IOD1		(1<<H8STIORL_IOD1_BIT)
#define H8STIORL_IOD0		(1<<H8STIORL_IOD0_BIT)
#define H8STIORL_IOC3		(1<<H8STIORL_IOC3_BIT)
#define H8STIORL_IOC2		(1<<H8STIORL_IOC2_BIT)
#define H8STIORL_IOC1		(1<<H8STIORL_IOC1_BIT)
#define H8STIORL_IOC0		(1<<H8STIORL_IOC0_BIT)


/*
 *  TPU タイマインタラプトイネーブルレジスタ(TIER)  
 *  (注意：チャネル1，2，4，5では、ビット3，2はリザーブビット)
 */
#define H8STIER_TTGE_BIT	7
#define H8STIER_TCIEV_BIT	4
#define H8STIER_TGIED_BIT	3
#define H8STIER_TGIEC_BIT	2
#define H8STIER_TGIEB_BIT	1
#define H8STIER_TGIEA_BIT	0

#define H8STIER_TTGE		(1<<H8STIER_TTGE_BIT)
#define H8STIER_TCIEV		(1<<H8STIER_TCIEV_BIT)
#define H8STIER_TGIED		(1<<H8STIER_TGIED_BIT)
#define H8STIER_TGIEC		(1<<H8STIER_TGIEC_BIT)
#define H8STIER_TGIEB		(1<<H8STIER_TGIEB_BIT)
#define H8STIER_TGIEA		(1<<H8STIER_TGIEA_BIT)

/*
 *   TPU タイマステータスレジスタ(TSR)
 *  (注意：チャネル1，2，4，5では、ビット3，2はリザーブビット)
 */
#define H8STSR_TCFV_BIT		4
#define H8STSR_TGFD_BIT		3
#define H8STSR_TGFC_BIT		2
#define H8STSR_TGFB_BIT		1
#define H8STSR_TGFA_BIT		0

#define H8STSR_TCFV		(1<<H8STSR_TCFV_BIT)
#define H8STSR_TGFD		(1<<H8STSR_TGFD_BIT)
#define H8STSR_TGFC		(1<<H8STSR_TGFC_BIT)
#define H8STSR_TGFB		(1<<H8STSR_TGFB_BIT)
#define H8STSR_TGFA		(1<<H8STSR_TGFA_BIT)

/*  TPU タイマカウンタ(TCNT)  */
/*  TPU タイマジェネラルレジスタ(TGR0A,TGR0B,TGR0C,TGR0D)  */


/*  ---シリアルコミュニケーションインターフェース(SCI)-------  */
/*  ベースアドレス  */
#define H8SSCI0		0xffff78
#define H8SSCI1		0xffff80

/*  アドレスオフセット  */
#define H8SSMR		0
#define H8SBRR		1
#define H8SSCR		2
#define H8STDR		3
#define H8SSSR		4
#define H8SRDR		5
#define H8SSCMR		6

/*  共通  */
#define H8SMSTPCR	0xffff3c

/*  SCI レシーブシフトレジスタ(RSR) */
/*  SCI レシーブデータレジスタ(RDR) */
/*  SCI トランスミットシフトレジスタ(TSR)  */
/*  SCI トランスミットデータレジスタ(TDR)  */

/*  SCI シリアルモードレジスタ(SMR)  */
#define H8SSMR_CA_BIT		7
#define H8SSMR_CHR_BIT		6
#define H8SSMR_PE_BIT		5
#define H8SSMR_OE_BIT		4
#define H8SSMR_STOP_BIT		3
#define H8SSMR_MP_BIT		2
#define H8SSMR_CKS1_BIT		1
#define H8SSMR_CKS0_BIT		0

#define H8SSMR_CA		(1<<H8SSMR_CA_BIT)
#define H8SSMR_CHR		(1<<H8SSMR_CHR_BIT)
#define H8SSMR_PE		(1<<H8SSMR_PE_BIT)
#define H8SSMR_OE		(1<<H8SSMR_OE_BIT)
#define H8SSMR_STOP		(1<<H8SSMR_STOP_BIT)
#define H8SSMR_MP		(1<<H8SSMR_MP_BIT)
#define H8SSMR_CKS1		(1<<H8SSMR_CKS1_BIT)
#define H8SSMR_CKS0		(1<<H8SSMR_CKS0_BIT)

/*  SCI シリアルコントロールレジスタ(SCR)  */
#define H8SSCR_TIE_BIT		7
#define H8SSCR_RIE_BIT		6
#define H8SSCR_TE_BIT		5
#define H8SSCR_RE_BIT		4
#define H8SSCR_MPIE_BIT		3
#define H8SSCR_TEIE_BIT		2
#define H8SSCR_CKE1_BIT		1
#define H8SSCR_CKE0_BIT		0

#define H8SSCR_TIE		(1<<H8SSCR_TIE_BIT)
#define H8SSCR_RIE		(1<<H8SSCR_RIE_BIT)
#define H8SSCR_TE		(1<<H8SSCR_TE_BIT)
#define H8SSCR_RE		(1<<H8SSCR_RE_BIT)
#define H8SSCR_MPIE		(1<<H8SSCR_MPIE_BIT)
#define H8SSCR_TEIE		(1<<H8SSCR_TEIE_BIT)
#define H8SSCR_CKE1		(1<<H8SSCR_CKE1_BIT)
#define H8SSCR_CKE0		(1<<H8SSCR_CKE0_BIT)

/*  SCI シリアルステータスレジスタ(SSR)  */
#define H8SSSR_TDRE_BIT		7
#define H8SSSR_RDRF_BIT		6
#define H8SSSR_ORER_BIT		5
#define H8SSSR_FER_BIT 		4
#define H8SSSR_PER_BIT 		3
#define H8SSSR_TEND_BIT		2
#define H8SSSR_MPB_BIT 		1
#define H8SSSR_MPBT_BIT		0

#define H8SSSR_TDRE		(1<<H8SSSR_TDRE_BIT)
#define H8SSSR_RDRF		(1<<H8SSSR_RDRF_BIT)
#define H8SSSR_ORER		(1<<H8SSSR_ORER_BIT)
#define H8SSSR_FER		(1<<H8SSSR_FER_BIT)
#define H8SSSR_PER		(1<<H8SSSR_PER_BIT)
#define H8SSSR_TEND		(1<<H8SSSR_TEND_BIT)
#define H8SSSR_MPB		(1<<H8SSSR_MPB_BIT)
#define H8SSSR_MPBT		(1<<H8SSSR_MPBT_BIT)

/*  SCI ビットレートレジスタ(BRR)  */

/*  SCI スマートカードモードレジスタ(SCMR)  */
#define H8SSCMR_SDIR_BIT	3
#define H8SSCMR_SINV_BIT	2
#define H8SSCMR_SMIF_BIT	0

#define H8SSCMR_SDIR		(1<<H8SSCMR_SDIR_BIT)
#define H8SSCMR_SINV		(1<<H8SSCMR_SINV_BIT)
#define H8SSCMR_SMIF		(1<<H8SSCMR_SMIF_BIT)

/*  SCI モジュールストップコントロールレジスタ(MSTPCR)  */
#define H8SMSTPCR_H15_BIT	15
#define H8SMSTPCR_H14_BIT	14
#define H8SMSTPCR_H13_BIT	13
#define H8SMSTPCR_H12_BIT	12
#define H8SMSTPCR_H11_BIT	11
#define H8SMSTPCR_H10_BIT	10
#define H8SMSTPCR_H9_BIT	9
#define H8SMSTPCR_H8_BIT	8
#define H8SMSTPCR_L7_BIT	7
#define H8SMSTPCR_L6_BIT	6
#define H8SMSTPCR_L5_BIT	5
#define H8SMSTPCR_L4_BIT	4
#define H8SMSTPCR_L3_BIT	3
#define H8SMSTPCR_L2_BIT	2
#define H8SMSTPCR_L1_BIT	1
#define H8SMSTPCR_L0_BIT	0

/* ---バスコントローラ(BSC)--------------------------- */
#define H8SABWCR	0xfffed0
#define H8SASTCR	0xfffed1
#define H8SWCRH		0xfffed2
#define H8SWCRL      	0xfffed3
#define H8SBCRH		0xfffed4
#define H8SBCRL		0xfffed5
#define H8SMCR		0xfffed6
#define H8SDRAMCR	0xfffed7
#define H8SRTCNT	0xfffed8
#define H8SRTCOR	0xfffed9

/*  BSC バス幅コントロールレジスタ(ABWCR)  */

/*  BSC アクセスステートコントロールレジスタ(ASTCR)  */
#define H8SASTCR_AST7_BIT	7
#define H8SASTCR_AST6_BIT	6
#define H8SASTCR_AST5_BIT	5
#define H8SASTCR_AST4_BIT	4
#define H8SASTCR_AST3_BIT	3
#define H8SASTCR_AST2_BIT	2
#define H8SASTCR_AST1_BIT	1
#define H8SASTCR_AST0_BIT	0

#define H8SASTCR_AST7 		(1<<H8SASTCR_AST7_BIT)
#define H8SASTCR_AST6 		(1<<H8SASTCR_AST6_BIT)
#define H8SASTCR_AST5 		(1<<H8SASTCR_AST5_BIT)
#define H8SASTCR_AST4 		(1<<H8SASTCR_AST4_BIT)
#define H8SASTCR_AST3 		(1<<H8SASTCR_AST3_BIT)
#define H8SASTCR_AST2 		(1<<H8SASTCR_AST2_BIT)
#define H8SASTCR_AST1 		(1<<H8SASTCR_AST1_BIT)
#define H8SASTCR_AST0 		(1<<H8SASTCR_AST0_BIT)

/*  BSC ウェイトコントロールレジスタH(WCRH)  */
#define H8SWCRH_W71_BIT 	7
#define H8SWCRH_W70_BIT 	6
#define H8SWCRH_W61_BIT 	5
#define H8SWCRH_W60_BIT 	4
#define H8SWCRH_W51_BIT 	3
#define H8SWCRH_W50_BIT 	2
#define H8SWCRH_W41_BIT 	1
#define H8SWCRH_W40_BIT 	0

#define H8SWCRH_W71 		(1<<H8SWCRH_W71_BIT)
#define H8SWCRH_W70 		(1<<H8SWCRH_W70_BIT)
#define H8SWCRH_W61 		(1<<H8SWCRH_W61_BIT)
#define H8SWCRH_W60 		(1<<H8SWCRH_W60_BIT)
#define H8SWCRH_W51 		(1<<H8SWCRH_W51_BIT)
#define H8SWCRH_W50 		(1<<H8SWCRH_W50_BIT)
#define H8SWCRH_W41 		(1<<H8SWCRH_W41_BIT)
#define H8SWCRH_W40 		(1<<H8SWCRH_W40_BIT)


/*  BSC ウェイトコントロールレジスタL(WCRL)  */
#define H8SWCRL_W31_BIT 	7
#define H8SWCRL_W30_BIT 	6
#define H8SWCRL_W21_BIT 	5
#define H8SWCRL_W20_BIT 	4
#define H8SWCRL_W11_BIT 	3
#define H8SWCRL_W10_BIT 	2
#define H8SWCRL_W01_BIT 	1
#define H8SWCRL_W00_BIT 	0

#define H8SWCRL_W31 		(1<<H8SWCRL_W31_BIT)
#define H8SWCRL_W30 		(1<<H8SWCRL_W30_BIT)
#define H8SWCRL_W21 		(1<<H8SWCRL_W21_BIT)
#define H8SWCRL_W20		(1<<H8SWCRL_W20_BIT)
#define H8SWCRL_W11 		(1<<H8SWCRL_W11_BIT)
#define H8SWCRL_W10 		(1<<H8SWCRL_W10_BIT)
#define H8SWCRL_W01 		(1<<H8SWCRL_W01_BIT)
#define H8SWCRL_W00 		(1<<H8SWCRL_W00_BIT)

/*  BSC バスコントロールレジスタ(BCRH)  */
#define H8SBCRH_ICIS1_BIT	7
#define H8SBCRH_ICIS0_BIT	6
#define H8SBCRH_BRSTRM_BIT	5
#define H8SBCRH_BRSTS1_BIT	4
#define H8SBCRH_BRSTS0_BIT	3
#define H8SBCRH_RMTS2_BIT	2
#define H8SBCRH_RMTS1_BIT	1
#define H8SBCRH_RMTS0_BIT	0

#define H8SBCRHL_ICIS1 		(1<<H8SBCRHL_ICIS1_BIT)
#define H8SBCRHL_ICIS0 		(1<<H8SBCRHL_ICIS0_BIT)
#define H8SBCRHL_BRSTRM 	(1<<H8SBCRHL_BRSTRM_BIT)
#define H8SBCRHL_BRSTS1 	(1<<H8SBCRHL_BRSTS1_BIT)
#define H8SBCRHL_BRSTS0 	(1<<H8SBCRHL_BRSTS0_BIT)
#define H8SBCRHL_RMTS2 		(1<<H8SBCRHL_RMTS2_BIT)
#define H8SBCRHL_RMTS1 		(1<<H8SBCRHL_RMTS1_BIT)
#define H8SBCRHL_RMTS0 		(1<<H8SBCRHL_RMTS0_BIT)

/* I/Oポートレジスタへのアクセス */

#define inb(p)			(*(volatile UB*)(p))
#define inw(p)			(*(volatile UH*)(p))
#define outb(p,d)		(*(volatile UB*)(p)=(d))
#define outw(p,d)		(*(volatile UH*)(p)=(d))

#endif /* _H8S_2350_H_ */
