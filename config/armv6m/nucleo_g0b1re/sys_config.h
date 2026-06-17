/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: sys_config.h 1952 2026-03-22 21:06:04Z roi $
 */

/*
 *		ターゲット依存モジュール（NUCLEO-G0B1RE用）
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
#define SysFreHCLK	SystemFrequency	/* HCLK = MasterClock / 1 */
#define SysFrePCLK1	SystemFrequency	/* PCLK1 = HCLK / 1 */
#define SYS_CLOCK	(SystemFrequency)

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
#define IRQ_VECTOR_SYSTICK          -1	/* SYSTICC */
#define IRQ_VECTOR_WWDG             0	/* Window WatchDog Interrupt */
#define IRQ_VECTOR_PVD              1	/* PVD through EXTI Line detection Interrupt */
#define IRQ_VECTOR_RTC_WKUP         2	/* RTC Wakeup interrupt through the EXTI line */
#define IRQ_VECTOR_FLASH            3	/* FLASH global Interrupt */
#define IRQ_VECTOR_RCC              4	/* RCC global Interrupt */
#define IRQ_VECTOR_EXTI0_1          5	/* EXTI Line0 and 1 Interrupt */
#define IRQ_VECTOR_EXTI2_3          6	/* EXTI Line2 and 3 Interrupt */
#define IRQ_VECTOR_EXTI4_15         7	/* EXTI Line4 to 15 Interrupt */
#define IRQ_VECTOR_USB_UCPD1_2      8	/* USB UPD1 and 2 global Interrupts */
#define IRQ_VECTOR_DMA1_CH1         9	/* DMA1 Channel 1 global Interrupt */
#define IRQ_VECTOR_DMA1_CH_LOW      10	/* DMA1 Channel 2 and 3 global Interrupt */
#define IRQ_VECTOR_DMA1_CH_HIGH     11	/* DMA1 Channel 4 to 7 global Interrupt */
#define IRQ_VECTOR_ADC1_COMP        12	/* ADC1, COMP1 and COMP2 Interrupts */
#define IRQ_VECTOR_LPTIM1           13	/* LPTIM1 Interrupt */
#define IRQ_VECTOR_USART4_5         14	/* USART4 and USART5 Interrupt */
#define IRQ_VECTOR_TIM2             15	/* TIM2 global Interrupt */
#define IRQ_VECTOR_TIM3             16	/* TIM3 global Interrupt */
#define IRQ_VECTOR_TIM6_DAC         17	/* TIM6 global and DAC channel underrun error Interrupt */
#define IRQ_VECTOR_TIM7             18	/* TIM7 global Interrupt */
#define IRQ_VECTOR_TIM21            20	/* TIM21 global Interrupt */
#define IRQ_VECTOR_I2C3             21	/* I2C3 global Interrupt */
#define IRQ_VECTOR_TIM22            22	/* TIM22 global Interrupt */
#define IRQ_VECTOR_I2C1             23	/* I2C1 global Interrupt  */
#define IRQ_VECTOR_I2C2             24	/* I2C2 global Interrupt */
#define IRQ_VECTOR_SPI1             25	/* SPI1 global Interrupt */
#define IRQ_VECTOR_SPI2             26	/* SPI2 global Interrupt */
#define IRQ_VECTOR_USART1           27	/* USART1 global Interrupt */
#define IRQ_VECTOR_USART2_LPUART2   28	/* USART2 and LPUART2 global Interrupt */
#define IRQ_VECTOR_USART3_4_5_6_LPUART1 29	/* USART3,4,5,6 and LPUART1 global Interrupt */
#define IRQ_VECTOR_CEC              30	/* CEC Interrupt(combined with EXTI 27) */

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
#ifdef TOPPERS_RAM_EXEC
#define SIL_DLY_TIM2    64
#else
#define SIL_DLY_TIM2    70
#endif


#ifndef _MACRO_ONLY

/*
 *  マルチプロセッサ処理モデルの実現
 *
 *  コア番号は標準実装．
 *  その他はシングルコアの場合、実装は不要．
 */

/*
 *  コア番号を取り出す
 */
#define	x_prc_index()	(0)

#define t_prc_index()	x_prc_index()
#define i_prc_index()	x_prc_index()

/*
 *  ベクターテーブル
 */
extern const FP vector_table[][TMAX_INTNO + 16 + 1];

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
 *  起動クロックの取り出し
 */
extern UW sysclock_init_value(void);

/*
 *  システムクロックコンフィギュレーション
 */
extern ER sysclock_config(int mode);

/*
 *  システムクロックの変更
 */
extern ER sysclock_change(UB range);

#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
