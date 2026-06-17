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
 *  @(#) $Id: sys_config.h 1952 2025-11-02 10:21:41Z roi $
 */

/*
 *		ターゲット依存モジュール（NUCLEO-F767ZI用）
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
#define SystemFrequency     216000000
#define SysFreHCLK          216000000	/* HCLK = MasterClock / 1 */
#define SysFrePCLK1         54000000	/* PCLK1 = HCLK / 4 */
#define SysFrePCLK2         108000000	/* PCLK2 = HCLK / 2 */
#define HSE_VALUE           8000000
#define HSI_VALUE           16000000
#define LSE_VALUE           32768

#define SYS_CLOCK		    (SystemFrequency)

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK		(SYS_CLOCK / 1000)

/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO      (109)

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
#define IRQ_VECTOR_PVD              1	/* PVD through EXTI Line detection Interrupt */
#define IRQ_VECTOR_TAMP_STAMP       2	/* Tamper and TimeStamp interrupts through the EXTI line */
#define IRQ_VECTOR_RTC_WKUP         3	/* RTC Wakeup interrupt through the EXTI line */
#define IRQ_VECTOR_FLASH            4	/* FLASH global Interrupt */
#define IRQ_VECTOR_RCC              5	/* RCC global Interrupt */
#define IRQ_VECTOR_EXTI0            6	/* EXTI Line0 Interrupt */
#define IRQ_VECTOR_EXTI1            7	/* EXTI Line1 Interrupt */
#define IRQ_VECTOR_EXTI2            8	/* EXTI Line2 Interrupt */
#define IRQ_VECTOR_EXTI3            9	/* EXTI Line3 Interrupt */
#define IRQ_VECTOR_EXTI4            10	/* EXTI Line4 Interrupt */
#define IRQ_VECTOR_DMA1_STREAM0     11	/* DMA1 Stream 0 global Interrupt */
#define IRQ_VECTOR_DMA1_STREAM1     12	/* DMA1 Stream 1 global Interrupt */
#define IRQ_VECTOR_DMA1_STREAM2     13	/* DMA1 Stream 2 global Interrupt */
#define IRQ_VECTOR_DMA1_STREAM3     14	/* DMA1 Stream 3 global Interrupt */
#define IRQ_VECTOR_DMA1_STREAM4     15	/* DMA1 Stream 4 global Interrupt */
#define IRQ_VECTOR_DMA1_STREAM5     16	/* DMA1 Stream 5 global Interrupt */
#define IRQ_VECTOR_DMA1_STREAM6     17	/* DMA1 Stream 6 global Interrupt */
#define IRQ_VECTOR_ADC              18	/* ADC1, ADC2 and ADC3 global Interrupts */
#define IRQ_VECTOR_CAN1_TX          19	/* CAN1 TX Interrupt */
#define IRQ_VECTOR_CAN1_RX0         20	/* CAN1 RX0 Interrupt */
#define IRQ_VECTOR_CAN1_RX1         21	/* CAN1 RX1 Interrupt */
#define IRQ_VECTOR_CAN1_SCE         22	/* CAN1 SCE Interrupt */
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
#define IRQ_VECTOR_OTG_FS_WKUP      42	/* USB OTG FS Wakeup through EXTI line interrupt */
#define IRQ_VECTOR_TIM8_BRK_TIM12   43	/* TIM8 Break Interrupt and TIM12 global interrupt */
#define IRQ_VECTOR_TIM8_UP_TIM13    44	/* TIM8 Update Interrupt and TIM13 global interrupt */
#define IRQ_VECTOR_TIM8_TRG_COM_TIM14 45	/* TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
#define IRQ_VECTOR_TIM8_CC          46	/* TIM8 Capture Compare Interrupt */
#define IRQ_VECTOR_DMA1_STREAM7     47	/* DMA1 Stream7 Interrupt */
#define IRQ_VECTOR_FMC              48	/* FMC global Interrupt */
#define IRQ_VECTOR_SDMMC            49	/* SDMMC global Interrupt */
#define IRQ_VECTOR_TIM5             50	/* TIM5 global Interrupt */
#define IRQ_VECTOR_SPI3             51	/* SPI3 global Interrupt */
#define IRQ_VECTOR_UART4            52	/* UART4 global Interrupt */
#define IRQ_VECTOR_UART5            53	/* UART5 global Interrupt */
#define IRQ_VECTOR_TIM6_DAC         54	/* TIM6 global and DAC1&2 underrun error interrupts */
#define IRQ_VECTOR_TIM7             55	/* TIM7 global interrupt */
#define IRQ_VECTOR_DMA2_STREAM0     56	/* DMA2 Stream 0 global Interrupt */
#define IRQ_VECTOR_DMA2_STREAM1     57	/* DMA2 Stream 1 global Interrupt */
#define IRQ_VECTOR_DMA2_STREAM2     58	/* DMA2 Stream 2 global Interrupt */
#define IRQ_VECTOR_DMA2_STREAM3     59	/* DMA2 Stream 3 global Interrupt */
#define IRQ_VECTOR_DMA2_STREAM4     60	/* DMA2 Stream 4 global Interrupt */
#define IRQ_VECTOR_ETH              61	/* Ethernet global Interrupt */
#define IRQ_VECTOR_ETH_WKUP         62	/* Ethernet Wakeup through EXTI line Interrupt */
#define IRQ_VECTOR_CAN2_TX          63	/* CAN2 TX Interrupt */
#define IRQ_VECTOR_CAN2_RX0         64	/* CAN2 RX0 Interrupt */
#define IRQ_VECTOR_CAN2_RX1         65	/* CAN2 RX1 Interrupt */
#define IRQ_VECTOR_CAN2_SCE         66	/* CAN2 SCE Interrupt */
#define IRQ_VECTOR_OTG_FS           67	/* USB OTG FS global Interrupt */
#define IRQ_VECTOR_DMA2_STREAM5     68	/* DMA2 Stream 5 global interrupt */
#define IRQ_VECTOR_DMA2_STREAM6     69	/* DMA2 Stream 6 global interrupt */
#define IRQ_VECTOR_DMA2_STREAM7     70	/* DMA2 Stream 7 global interrupt */
#define IRQ_VECTOR_USART6           71	/* USART6 global interrupt */
#define IRQ_VECTOR_I2C3_EV          72	/* I2C3 event interrupt */
#define IRQ_VECTOR_I2C3_ER          73	/* I2C3 error interrupt */
#define IRQ_VECTOR_OTG_HS_EP1_OUT   74	/* USB OTG HS End Point 1 Out global interrupt */
#define IRQ_VECTOR_OTG_HS_EP1_IN    75	/* USB OTG HS End Point 1 In global interrupt */
#define IRQ_VECTOR_OTG_HS_WKUP      76	/* USB OTG HS Wakeup through EXTI interrupt */
#define IRQ_VECTOR_OTG_HS           77	/* USB OTG HS global interrupt */
#define IRQ_VECTOR_DCMI             78	/* DCMI global interrupt */
#define IRQ_VECTOR_CRYP             79	/* CRYP crypto global interrupt */
#define IRQ_VECTOR_HASH_RNG         80	/* Hash and Rng global interrupt */
#define IRQ_VECTOR_FPU              81	/* FPU global interrupt */
#define IRQ_VECTOR_UART7            82	/* UART7 global interrupt */
#define IRQ_VECTOR_UART8            83	/* UART8 global interrupt */
#define IRQ_VECTOR_SPI4             84	/* SPI4 global Interrupt */
#define IRQ_VECTOR_SPI5             85	/* SPI5 global Interrupt */
#define IRQ_VECTOR_SPI6             86	/* SPI6 global Interrupt */
#define IRQ_VECTOR_SAI1             87	/* SAI1 global Interrupt */
#define IRQ_VECTOR_LTDC             88	/* LTDC global Interrupt */
#define IRQ_VECTOR_LTDC_ER          89	/* LTDC Error global Interrupt */
#define IRQ_VECTOR_DMA2D            90	/* DMA2D global Interrupt */
#define IRQ_VECTOR_SAI2             91	/* SAI2 global Interrupt */
#define IRQ_VECTOR_QUADSPI          92	/* Quad SPI global interrupt */
#define IRQ_VECTOR_LPTIM1           93	/* LP TIM1 interrupt */
#define IRQ_VECTOR_CEC              94	/* HDMI-CEC global Interrupt */
#define IRQ_VECTOR_I2C4_EV          95	/* I2C4 Event Interrupt */
#define IRQ_VECTOR_I2C4_ER          96	/* I2C4 Error Interrupt */
#define IRQ_VECTOR_SPDIF_RX         97	/* SPDIF-RX global Interrupt */
#define IRQ_DFSDM1_FLT0	            99	/* DFSDM1 Filter 0 global Interrupt */
#define IRQ_DFSDM1_FLT1	            100	/* DFSDM1 Filter 1 global Interrupt */
#define IRQ_DFSDM1_FLT2	            101	/* DFSDM1 Filter 2 global Interrupt */
#define IRQ_DFSDM1_FLT3             102	/* DFSDM1 Filter 3 global Interrupt */
#define IRQ_VECTOR_SDMMC2           103	/* SDMMC2 global Interrupt */
#define IRQ_VECTOR_CAN3_TX          104	/* CAN3 TX Interrupt */
#define IRQ_VECTOR_CAN3_RX0         105	/* CAN3 RX0 Interrupt */
#define IRQ_VECTOR_CAN3_RX1         106	/* CAN3 RX1 Interrupt */
#define IRQ_VECTOR_CAN3_SCE         107	/* CAN3 SCE Interrupt */
#define IRQ_VECTOR_JPEG             108	/* JPEG global Interrupt */
#define IRQ_VECTOR_MDIOS            109	/* MDIO Slave global Interrupt */

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
#define SIL_DLY_TIM1    100
#define SIL_DLY_TIM2    9


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
