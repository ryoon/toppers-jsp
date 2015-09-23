/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: sys_config.h,v 1.2 2002/04/10 10:40:53 honda Exp $
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "h8_3048f.h"

/*
 *  起動メッセージのターゲット名
 */

#define	TARGET_NAME	"AKI-H8/3048F"

/*
 *  vgxet_tim をサポートするかどうかの定義
 */

#define	SUPPORT_VXGET_TIM

/*
 *   スタック領域の定義
 */

#define STACKTOP    	0x000fff10		/* タスク独立部用スタックの初期値 */
#define str_STACKTOP	_TO_STRING(STACKTOP)

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

extern void cpu_putc(char c);

Inline void
sys_putc(char c)
{
	cpu_putc(c);
	};

#endif /* _MACRO_ONLY */

/*
 *  (1) シリアルポートの設定
 *  (2) タイマーの設定
 *  (3) 外部アドレス空間制御
 */
 
/*
 *  シリアル割り込みが入力/出力で異なるかどうかの定義
 */

#define SEPARATE_SIO_INT

/*
 *  サポートするシリアルディバイスの数、最大 2
 */

#define NUM_PORT	2

#define SYSTEM_SCI		H8SCI1

#define SYSTEM_SCI_IPR		H8IPRB
#define SYSTEM_SCI_IP_BIT	H8IPR_SCI1_BIT

#define BAUD_RATE		9600			/* bps */

#define H8SMR_CKS		(H8SMR_CKS1|H8SMR_CKS0)	/* clock / 8 */

#define H8BRR_RATE		((UB)((CPU_CLOCK/(32*(BAUD_RATE)))-1))
#define SCI_SETUP_COUNT		((CPU_CLOCK)/(BAUD_RATE)/5)

#define	CONSOLE_PORTID		SYSTEM_PORTID	/* コンソールに用いるシリアルポート番号     */
#define	LOGTASK_PORTID		SYSTEM_PORTID	/* システムログを出力するシリアルポート番号 */

#if NUM_PORT == 1

#define	SYSTEM_PORTID		PORT_ID1

#define HWPORT1_ADDR		SYSTEM_SCI

#define INHNO_SERIAL_ERROR	IRQ_ERI1
#define INHNO_SERIAL_IN		IRQ_RXI1
#define INHNO_SERIAL_OUT	IRQ_TXI1

#elif NUM_PORT == 2	/* of #if NUM_PORT == 1 */

#define USER_SCI		H8SCI0

#define USER_SCI_IPR		H8IPRB
#define USER_SCI_IP_BIT		H8IPR_SCI0_BIT

#define	USER_PORTID		PORT_ID1
#define	SYSTEM_PORTID		PORT_ID2

#define HWPORT1_ADDR		USER_SCI
#define HWPORT2_ADDR		SYSTEM_SCI

#define INHNO_SERIAL_ERROR	IRQ_ERI0
#define INHNO_SERIAL_IN		IRQ_RXI0
#define INHNO_SERIAL_OUT	IRQ_TXI0

#define INHNO_SERIAL_ERROR2	IRQ_ERI1
#define INHNO_SERIAL_IN2	IRQ_RXI1
#define INHNO_SERIAL_OUT2	IRQ_TXI1

#else	/* of #if NUM_PORT == 1 */

#error NUM_PORT <= 2

#endif	/* of #if NUM_PORT == 1 */

/*
 *  タイマの設定
 */

#define SYSTEM_TIMER		H8ITU0

#define SYSTEM_TIMER_CNT	(SYSTEM_TIMER + H8TCNT)
#define SYSTEM_TIMER_TCR	(SYSTEM_TIMER + H8TCR)
#define SYSTEM_TIMER_TIOR	(SYSTEM_TIMER + H8TIOR)
#define SYSTEM_TIMER_IER	(SYSTEM_TIMER + H8TIER)
#define SYSTEM_TIMER_IFR	(SYSTEM_TIMER + H8TSR)
#define SYSTEM_TIMER_TSTR	H8ITU_TSTR
#define SYSTEM_TIMER_GR		(SYSTEM_TIMER + H8GRA)

#define SYSTEM_TIMER_STR	H8TSTR_STR0
#define SYSTEM_TIMER_STR_BIT	H8TSTR_STR0_BIT
#define SYSTEM_TIMER_IE		H8TIER_IMIEA		/* interrupt mask */
#define SYSTEM_TIMER_IE_BIT	H8TIER_IMIEA_BIT
#define SYSTEM_TIMER_IF		H8TSR_IMIFA		/* match flag */
#define SYSTEM_TIMER_IF_BIT	H8TSR_IMIFA_BIT

#define INHNO_TIMER		IRQ_IMIA0

#define SYSTEM_TIMER_TCR_BIT	(H8TCR_CCLR0 | H8TCR_TPSC1 | H8TCR_TPSC0)
#define SYSTEM_TIMER_TIOR_BIT	(0)

#define TIMER_CLOCK		((CPU_CLOCK)/8000)	/* 16MHz / 8 = 2MHz = 2000KHz */

/*
 *  外部アドレス空間制御
 */

#if 0

#define	ENABLE_LOWER_DATA

#define	ENABLE_P8_CS		(H8P8DDR_CS0|H8P8DDR_CS1|H8P8DDR_CS2|H8P8DDR_CS3)
#define	ENABLE_PA_CS		(H8PADDR_CS4|H8PADDR_CS5|H8PADDR_CS6)
#define ENABLE_PB_CS		 H8PBDDR_CS7

#endif	/* of #if 0 */

#endif /* _SYS_CONFIG_H_ */
