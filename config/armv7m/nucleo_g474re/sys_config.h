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
 *  @(#) $Id: sys_config.h 1952 2025-11-01 11:22:45Z roi $
 */

/*
 *		ターゲット依存モジュール（NUCLEO-G474RE用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>


/*
 *  システムクロックの定義
 */
#define SystemFrequency     170000000
#define SysFreHCLK          SystemFrequency	/* HCLK = MasterClock / 1 */
#define SysFrePCLK1         SystemFrequency	/* PCLK1 = HCLK / 1 */
#define SysFrePCLK2         SystemFrequency	/* PCLK2 = HCLK / 1 */

#define SYS_CLOCK		    (SystemFrequency)

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK		(SYS_CLOCK / 1000)

/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO      (101)

/*
 *  カーネル管理の割込み優先度の範囲
 */
#define TMIN_INTPRI     (0)		/* 割込み優先度の最小値（最高値）*/
#ifndef TMAX_INTPRI
#define TMAX_INTPRI     (15)	/* 割込み優先度の最大値（最低値） */
#endif /* TMAX_INTPRI */

/*
 *  システムタイマ割込み優先度
 */
#define INTPRI_TIMER    4

/*
 *  割込みベクター定義
 */
#define IRQ_VECTOR_SYSTICK          -1	/* SYSTICC */
#define IRQ_VECTOR_WWDG             0	/* Window WatchDog Interrupt */
#define IRQ_VECTOR_PVD_PVM          1	/* PVD/PVM1/PVM2/PVM3/PVM4 through EXTI Line detection Interrupts */
#define IRQ_VECTOR_RTC_TAMP_LSECSS  2	/* Tamper and TimeStamp interrupts through the EXTI line */
#define IRQ_VECTOR_RTC_WKUP         3	/* RTC Wakeup interrupt through the EXTI line */
#define IRQ_VECTOR_FLASH            4	/* FLASH global Interrupt */
#define IRQ_VECTOR_RCC              5	/* RCC global Interrupt */
#define IRQ_VECTOR_EXTI0            6	/* EXTI Line0 Interrupt */
#define IRQ_VECTOR_EXTI1            7	/* EXTI Line1 Interrupt */
#define IRQ_VECTOR_EXTI2            8	/* EXTI Line2 Interrupt */
#define IRQ_VECTOR_EXTI3            9	/* EXTI Line3 Interrupt */
#define IRQ_VECTOR_EXTI4            10	/* EXTI Line4 Interrupt */
#define IRQ_VECTOR_DMA1_CH1         11	/* DMA1 Channel 1 global Interrupt */
#define IRQ_VECTOR_DMA1_CH2         12	/* DMA1 Channel 2 global Interrupt */
#define IRQ_VECTOR_DMA1_CH3         13	/* DMA1 Channel 3 global Interrupt */
#define IRQ_VECTOR_DMA1_CH4         14	/* DMA1 Channel 4 global Interrupt */
#define IRQ_VECTOR_DMA1_CH5         15	/* DMA1 Channel 5 global Interrupt */
#define IRQ_VECTOR_DMA1_CH6         16	/* DMA1 Channel 6 global Interrupt */
#define IRQ_VECTOR_DMA1_CH7         17	/* DMA1 Channel 7 global Interrupt */
#define IRQ_VECTOR_ADC1_2           18	/* ADC1, ADC2 SAR global Interrupts */
#define IRQ_VECTOR_USB_HP           19	/* USB HP Interrupt */
#define IRQ_VECTOR_USB_LP           20	/* USB LP Interrupt */
#define IRQ_VECTOR_FDCAN1_IT0       21	/* FDCAN1 IT0 Interrupt */
#define IRQ_VECTOR_FDCAN1_IT1       22	/* FDCAN1 IT1 Interrupt */
#define IRQ_VECTOR_EXTI9_5          23	/* External Line[9:5] Interrupts */
#define IRQ_VECTOR_TIM1_BRK_TIM9    24	/* TIM1 Break interrupt and TIM9 global interrupt */
#define IRQ_VECTOR_TIM1_UP_TIM10    25	/* TIM1 Update Interrupt and TIM10 global interrupt */
#define IRQ_VECTOR_TIM1_TRG_COM_TIM11 26	/* TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
#define IRQ_VECTOR_TIM1_CC          27	/* TIM1 Capture Compare Interrupt */
#define IRQ_VECTOR_TIM2             28	/* TIM2 global Interrupt */
#define IRQ_VECTOR_TIM3             29	/* TIM3 global Interrupt */
#define IRQ_VECTOR_TIM4             30	/* TIM4 global Interrupt */
#define IRQ_VECTOR_I2C1_EV          31	/* I2C1 Event Interrupt */
#define IRQ_VECTOR_I2C1_ER          32	/* I2C1 Error Interrupt */
#define IRQ_VECTOR_I2C2_EV          33	/* I2C2 Event Interrupt */
#define IRQ_VECTOR_I2C2_ER          34	/* I2C2 Error Interrupt */
#define IRQ_VECTOR_SPI1             35	/* SPI1 global Interrupt */
#define IRQ_VECTOR_SPI2             36	/* SPI2 global Interrupt */
#define IRQ_VECTOR_USART1           37	/* USART1 global Interrupt */
#define IRQ_VECTOR_USART2           38	/* USART2 global Interrupt */
#define IRQ_VECTOR_USART3           39	/* USART3 global Interrupt */
#define IRQ_VECTOR_EXTI15_10        40	/* External Line[15:10] Interrupts */
#define IRQ_VECTOR_RTC_ALARM        41	/* RTC Alarm (A and B) through EXTI Line Interrupt */
#define IRQ_VECTOR_USB_WAKEUP       42	/* USB Wakeup through EXTI line Interrupt */
#define IRQ_VECTOR_TIM8_BRK         43	/* TIM8 Break Interrupt */
#define IRQ_VECTOR_TIM8_UP          44	/* TIM8 Update Interrupt */
#define IRQ_VECTOR_TIM8_TRG_COM     45	/* TIM8 Trigger and Commutation Interrupt */
#define IRQ_VECTOR_TIM8_CC          46	/* TIM8 Capture Compare Interrupt */
#define IRQ_VECTOR_ADC3             47	/* ADC3 global Interrupt */
#define IRQ_VECTOR_FMC              48	/* FMC global Interrupt */
#define IRQ_VECTOR_LPTIM1           49	/* LP TIM1 Interrupt */
#define IRQ_VECTOR_TIM5             50	/* TIM5 global Interrupt */
#define IRQ_VECTOR_SPI3             51	/* SPI3 global Interrupt */
#define IRQ_VECTOR_UART4            52	/* UART4 global Interrupt */
#define IRQ_VECTOR_UART5            53	/* UART5 global Interrupt */
#define IRQ_VECTOR_TIM6_DAC         54	/* TIM6 global and DAC1&2 underrun error interrupts */
#define IRQ_VECTOR_TIM7_DAC         55	/* TIM7 global and DAC1&2 underrun error interrupts */
#define IRQ_VECTOR_DMA2_CH1         56	/* DMA2 Channel 1 global Interrupt */
#define IRQ_VECTOR_DMA2_CH2         57	/* DMA2 Channel 2 global Interrupt */
#define IRQ_VECTOR_DMA2_CH3         58	/* DMA2 Channel 3 global Interrupt */
#define IRQ_VECTOR_DMA2_CH4         59	/* DMA2 Channel 4 global Interrupt */
#define IRQ_VECTOR_DMA2_CH5         60	/* DMA2 Channel 5 global Interrupt */
#define IRQ_VECTOR_ADC4             61	/* ADC4 global Interrupt */
#define IRQ_VECTOR_ADC5             62	/* ADC5 global Interrupt */
#define IRQ_VECTOR_UCPD1            63	/* UCPD global Interrupt */
#define IRQ_VECTOR_COMP1_2_3        64	/* COMP1, COMP2 and COMP3 Interrupts */
#define IRQ_VECTOR_COMP4_5_6        65	/* COMP4, COMP5 and COMP6 Interrupts */
#define IRQ_VECTOR_COMP7            66	/* COMP7 Interrupt */
#define IRQ_VECTOR_HRTIM1_MASTER    67	/* HRTIM Master Timer global Interrupt */
#define IRQ_VECTOR_HRTIM1_TIMA      68	/* HRTIM Timer A global Interrupt */
#define IRQ_VECTOR_HRTIM1_TIMB      69	/* HRTIM Timer B global Interrupt */
#define IRQ_VECTOR_HRTIM1_TIMC      70	/* HRTIM Timer C global Interrupt */
#define IRQ_VECTOR_HRTIM1_TIMD      71	/* HRTIM Timer D global Interrupt */
#define IRQ_VECTOR_HRTIM1_TIME      72	/* HRTIM Timer E global Interrupt */
#define IRQ_VECTOR_HRTIM1_FLT       73	/* HRTIM Fault global Interrupt */
#define IRQ_VECTOR_HRTIM1_TIMF      74	/* HRTIM Timer F global Interrupt */
#define IRQ_VECTOR_CRS              75	/* CRS global interrupt */
#define IRQ_VECTOR_SAI1             76	/* Serial Audio Interface global interrupt */
#define IRQ_VECTOR_TIM20_BRK        77	/* TIM20 Break, Transition error and Index error Interrupt */
#define IRQ_VECTOR_TIM20_UP         78	/* TIM20 Update interrupt */
#define IRQ_VECTOR_TIM20_TRG_COM    79	/* TIM20 Trigger, Commutation, Direction change and Index Interrupt */
#define IRQ_VECTOR_TIM20_CC         80	/* TIM20 Capture Compare interrupt */
#define IRQ_VECTOR_FPU              81	/* FPU global interrupt */
#define IRQ_VECTOR_I2C4_EV          82	/* I2C4 Event interrupt */
#define IRQ_VECTOR_I2C4_ER          83	/* I2C4 Error interrupt */
#define IRQ_VECTOR_SPI4             84	/* SPI4 Event interrupt */
#define IRQ_VECTOR_FDCAN2_IT0       86	/* FDCAN2 interrupt line 0 interrupt */
#define IRQ_VECTOR_FDCAN2_IT1       87	/* FDCAN2 interrupt line 1 interrupt */
#define IRQ_VECTOR_FDCAN3_IT0       88	/* FDCAN3 interrupt line 0 interrupt */
#define IRQ_VECTOR_FDCAN3_IT1       89	/* FDCAN3 interrupt line 1 interrupt */
#define IRQ_VECTOR_RNG              90	/* RNG global interrupt */
#define IRQ_VECTOR_LPUART1          91	/* LP UART 1 Interrupt */
#define IRQ_VECTOR_I2C3_EV          92	/* I2C3 Event Interrupt */
#define IRQ_VECTOR_I2C3_ER          93	/* I2C3 Error interrupt */
#define IRQ_VECTOR_DMAMUX_OVR       94	/* DMAMUX overrun global interrupt */
#define IRQ_VECTOR_QUADSPI          95	/* QUADSPI interrupt */
#define IRQ_VECTOR_DMA1_CH8         96	/* DMA1 Channel 8 interrupt */
#define IRQ_VECTOR_DMA2_CH6         97	/* DMA2 Channel 6 interrupt */
#define IRQ_VECTOR_DMA2_CH7         98	/* DMA2 Channel 7 interrupt */
#define IRQ_VECTOR_DMA2_CH8         99	/* DMA2 Channel 8 interrupt */
#define IRQ_VECTOR_CORDIC           100	/* CORDIC global Interrupt  */
#define IRQ_VECTOR_FMAC             101	/* FMAC global Interrupt */

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
 *  デバッグシリアルポート数の定義 (SOC中のUSARTを使用する)
 */
#define TNUM_PORT	2u	/* サポートするシリアルポートの数 */
#define TNUM_SIOP	2u	/* サポートするシリアルI/Oポートの数 */

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
#define SIL_DLY_TIM1    44
#define SIL_DLY_TIM2    33


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
 *  システムクロックコンフィギュレーション
 */
extern ER sysclock_config(UW mode);

#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
