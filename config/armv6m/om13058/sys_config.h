/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: sys_config.h 1952 2025-12-12 18:51:15Z roi $
 */

/*
 *		ターゲット依存モジュール（OM13058用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>


#ifndef _MACRO_ONLY

/*
 *  システムクロックの定義
 */
extern  UW SystemFrequency;
#define SYS_CLOCK		(SystemFrequency)

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK		(SYS_CLOCK / 1000)

#endif /* _MACRO_ONLY */

/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO      (30)

/*
 *  カーネル管理の割込み優先度の範囲
 */
#define TMIN_INTPRI     (0)		/* 割込み優先度の最小値（最高値）*/
#ifndef TMAX_INTPRI
#define TMAX_INTPRI     (3)		/* 割込み優先度の最大値（最低値） */
#endif /* TMAX_INTPRI */

/*
 *  システムタイマ割込み優先度
 */
#define INTPRI_TIMER    2

/*
 *  割込みベクター定義
 */
#define IRQ_VECTOR_SYSTICK      -1	/* SYSTICC */
#define IRQ_VECTOR_PIN_INT0     0	/* Pin Interrupt 0 */
#define IRQ_VECTOR_PIN_INT1     1	/* Pin Interrupt 1 */
#define IRQ_VECTOR_PIN INT2     2	/* Pin Interrupt 2 */
#define IRQ_VECTOR_PIN_INT3     3	/* Pin Interrupt 3 */
#define IRQ_VECTOR_PIN_INT4     4	/* Pin Interrupt 4 */
#define IRQ_VECTOR_PIN_INT5     5	/* Pin Interrupt 5 */
#define IRQ_VECTOR_PIN_INT6     6	/* Pin Interrupt 6 */
#define IRQ_VECTOR_PIN_INT7     7	/* Pin Interrupt 7 */
#define IRQ_VECTOR_GINT0        8	/* GPIO interrupt status of port 0 */
#define IRQ_VECTOR_GINT1        9	/* GPIO interrupt status of port 1 */
#define IRQ_VECTOR_I2C1         10	/* I2C1 Interrupt */
#define IRQ_VECTOR_USART1_4     11	/* Combined USART1 and USART4 interrupts  */
#define IRQ_VECTOR_USART2_3     12	/* Combined USART2 and USART3 interrupts */
#define IRQ_VECTOR_SCT0_1       13	/* Combined SCT0 and SCT1 interrupts  */
#define IRQ_VECTOR_SSP1         14	/* SSP1 Interrupt  */
#define IRQ_VECTOR_I2C0         15	/* I2C0 Interrupt */
#define IRQ_VECTOR_TIMER_16_0   16	/* 16-bit Timer0 Interrupt  */
#define IRQ_VECTOR_TIMER_16_1   17	/* 16-bit Timer1 Interrupt  */
#define IRQ_VECTOR_TIMER_32_0   18	/* 32-bit Timer0 Interrupt */
#define IRQ_VECTOR_TIMER_32_1   19	/* 32-bit Timer1 Interrupt */
#define IRQ_VECTOR_SSP0         20	/* SSP0 Interrupt */
#define IRQ_VECTOR_USART0       21	/* USART0 interruptt */
#define IRQ_VECTOR_USB0         22	/* USB IRQ interrupt */
#define IRQ_VECTOR_USB0_FIQ     23	/* USB FIQ interrupt  */
#define IRQ_VECTOR_ADV_A        24	/* ADC_A Converter Interrupt */
#define IRQ_VECTOR_RTC          25	/* RTC Interrupt  */
#define IRQ_VECTOR_BOD_WDT      26	/* Shared Brown Out Detect(BOD) and WDT Interrupts */
#define IRQ_VECTOR_FMC          27	/* FLASH Interrupt */
#define IRQ_VECTOR_DMA          28	/* USART3 ang LPUART1 global Interrupt */
#define IRQ_VECTOR_ADC_B        29	/* ADC_B Interrupt */
#define IRQ_VECTOR_USB_WAKEUP   30	/* USB wake-up interrupt Interrupt */

/*
 *  ターゲット定義のオブジェクト属性
 */
#define TARGET_INHATR	TA_NONKERNEL	/* カーネル管理外の割込み */

/*
 *  chg_ipmをサポートするかどうかの定義
 */
#define SUPPORT_CHG_IPM

/*
 *  vgxet_tim をサポートするかどうかの定義
 */
#define SUPPORT_VXGET_TIM

/*
 *  デバッグシリアルポート数の定義 (RP2040中のUART0を使用する)
 */
#define TNUM_PORT	1u	/* サポートするシリアルポートの数 */
#define TNUM_SIOP	1u	/* サポートするシリアルI/Oポートの数 */

/*
 *  システムタスクに関する定義
 */
#define	LOGTASK_PORTID	1u	/* システムログを出力するシリアルポート番号 */

/*
 *  シリアルのデフォルトボーレート定義
 */
#define DEF_BAUDRATE    115200

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    50
#define SIL_DLY_TIM2    84


#ifndef _MACRO_ONLY

/*
 *  ベクターテーブル
 */
extern const FP vector_table[];

/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */

extern void	sys_putc(char c) throw();

/*
 * WDTクロックレイトを算出する
 */
extern UW get_WDTrate(UW reg);

/*
 * PLLソースクロックを取得する
 */
extern UW get_PLLinclockrate(void);

/*
 *  起動クロックの取り出し
 */
extern UW sysclock_init_value(void);

#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
