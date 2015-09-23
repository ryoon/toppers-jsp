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
 *  @(#) $Id: sys_config.h,v 1.2 2002/04/13 13:15:41 imai Exp $
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "h8S_2350.h"

/*
 *  起動メッセージのターゲット名
 */

#define	TARGET_NAME	"MISPO-H8S/2350 EVA"

/*
 *  vgxet_tim をサポートするかどうかの定義
 */

#define	SUPPORT_VXGET_TIM

/*
 *  JSPカーネル動作時のメモリマップ
 *      0x000000 - 0x01ffff     コード領域(128kB)
 *      0x200000 -              データ領域(128kB)
 *               - 0x21ffff     タスク独立部用スタック，メモリ終了
 */

/*
 *   スタック領域の定義
 */

#define STACKTOP    	0x21fffc		/* タスク独立部用スタックの初期値 */
#define str_STACKTOP	_TO_STRING(STACKTOP)
 
/*
 *  シリアル割り込みが入力/出力で異なるかどうかの定義
 */

#define SEPARATE_SIO_INT

/*
 *  システムタスクに関する定義
 */

#define	CONSOLE_PORTID	1	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORTID	1	/* システムログを出力するシリアルポート番号 */

/*
 *  システムタスクが使用するライブラリに関する定義
 */

#define NEWLIB			/* newlib を用いる */

#ifndef _MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */

extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．ROMモニタ呼出しで実現することを想定し
 *  ている．
 */

extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */

extern void sys_putc(char c);

#endif /* _MACRO_ONLY */

/*
 *  (1) CPU のクロック周波数
 *  (2) SCI の設定
 *  (3) ITU の設定
 *  (4) 外部アドレス空間制御
 */

#ifdef CONFIG_20MHz

#define CPU_CLOCK	20000000		/* MHz */

#endif	/* of #ifdef CONFIG_20MHz */

/*
 *  シリアルコミュニケーションインターフェース(SCI) の設定
 */

/*   クロック20(MHz)
 *　 ビットレート9600(bps)
 *   
 *　 n=0（分周なし）
 *　 N=64，誤差0.16(%)
 *   従って、シリアルモードレジスタ(SMR)のクロックセレクトビット
 *　 CKS1=0，CKS0=0
 *　 ビットレートレジスタBRR=64
 */

#define SYSTEM_SCI		H8SSCI0
#define SYSTEM_SCI_IPR		H8SIPRJ
/*  以下の2項目はH8版では定義していない。  */
#define SCI_INTLVL		3   	/*  SCIチャネル0の割り込みレベル  */
#define SCI_BIT_SHIFT		0       /*  割込みレベルを設定レジスタに代入するときのシフト数  */

#define BAUD_RATE		9600			/* bps */

#define INHNO_SERIAL_IN		IRQ_RXI0
#define INHNO_SERIAL_OUT	IRQ_TXI0

#define H8SSMR_CKS		(H8SSMR_CKS0)  /*  不要  */

#define H8SBRR_RATE		64	/*  N=64  */

/*  
 *  最初の１ビット分の待ち時間の設定
 *　クロック20MHzの場合の値を流用
 *　ループ1回に付き5命令(=5クロック)
 */

#define SCI_SETUP_COUNT		((CPU_CLOCK)/(BAUD_RATE)/5) 
/*
 *  タイマパルスユニット(TPU) の設定
 */

#define SYSTEM_TPU		H8STPU0
#define SYSTEM_STR		H8STSTR_CST0
#define SYSTEM_STR_BIT		H8STSTR_CST0_BIT
#define SYSTEM_GR		H8STGRA
#define SYSTEM_TPU_IE		H8STIER_TGIEA		/* interrupt mask */
#define SYSTEM_TPU_IE_BIT	H8STIER_TGIEA_BIT
#define SYSTEM_TPU_IF		H8STSR_TGFA		/* match flag */
#define SYSTEM_TPU_IF_BIT	H8STSR_TGFA_BIT

#define SYSTEM_TPU_IPR		H8SIPRF  /*  TPUチャネル0のIPR  */
/*  H8版では以下の2項目の定義はない  */
 				/*　TPUチャネル0の割り込みレベル　*/
#define TPU_INTLVL		5
#define LEVEL0			0
				 /*  割込みレベルを設定レジスタに代入するときのシフト数  */
#define TPU_BIT_SHIFT		4

#define INHNO_TIMER		IRQ_TGI0A

#define H8STCR_CCLR		H8STCR_CCLR0		/* clear on GRA */
#define H8STCR_CKEG		(0)			/* positive edge */
							/* clock / 4 */
#define H8STCR_TPSC		(H8STCR_TPSC0)
#define H8STIOR_IOB		(0)			/* no output */
#define H8STIOR_IOA		(0)			/* no output */
#define H8STIOR_IOC		(0)			/* no output */
#define H8STIOR_IOD		(0)			/* no output */

#define TIMER_CLOCK		((CPU_CLOCK)/4000)	/* 20MHz / 4 = 5MHz = 5000KHz */

/*
 *  外部アドレス空間制御
 */

#if 0

#define	ENABLE_LOWER_DATA

#define	ENABLE_P8_CS		(H8P8DDR_CS0_BIT|H8P8DDR_CS1_BIT|H8P8DDR_CS2_BIT|H8P8DDR_CS3_BIT)
#define	ENABLE_PA_CS		(H8PADDR_CS4_BIT|H8PADDR_CS5_BIT|H8PADDR_CS6_BIT)
#define ENABLE_PB_CS		 H8PBDDR_CS7_BIT

#endif	/* of #if 0 */

#endif /* _SYS_CONFIG_H_ */
