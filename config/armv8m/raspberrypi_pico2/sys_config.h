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
 *  @(#) $Id: sys_config.h 1952 2025-11-24 15:56:19Z roi $
 */

/*
 *		ターゲット依存モジュール（Raspberry Pi Pico2 Cortex-M33用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  クロックの定義
 */
#define CLK_GPOUT0      0	/* GPIO Muxing 0 */
#define CLK_GPOUT1      1	/* GPIO Muxing 1 */
#define CLK_GPOUT2      2	/* GPIO Muxing 2 */
#define CLK_GPOUT3      3	/* GPIO Muxing 3 */
#define CLK_REF         4	/* Watchdog and timers reference clock */
#define CLK_SYS         5	/* Processors, bus fabric, memory, memory mapped registers */
#define CLK_PERI        6	/* Peripheral clock for UART and SPI */
#define CLK_HSTX        7	/* HSTX clock */
#define CLK_USB         8	/* USB clock */
#define CLK_ADC         9	/* ADC clock */
#define NUM_CLK         10
#define CLK_HW_SIZE     12

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
#define TMAX_INTNO      (51)
#define TBITW_IPRI		4

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
#define INTPRI_TIMER    9

/*
 *  割込みベクター定義
 */
#define IRQ_VECTOR_SYSTICK      -1	/* SYSTICC */
#define IRQ_VECTOR_TIMER0_0     0	/* TIMER0_IRQ_0 */
#define IRQ_VECTOR_TIMER0_1     1	/* TIMER0_IRQ_1 */
#define IRQ_VECTOR_TIMER0_2     2	/* TIMER0_IRQ_2 */
#define IRQ_VECTOR_TIMER0_3     3	/* TIMER0_IRQ_3 */
#define IRQ_VECTOR_TIMER1_0     4	/* TIMER1_IRQ_0 */
#define IRQ_VECTOR_TIMER1_1     5	/* TIMER1_IRQ_1 */
#define IRQ_VECTOR_TIMER1_2     6	/* TIMER1_IRQ_2 */
#define IRQ_VECTOR_TIMER1_3     7	/* TIMER1_IRQ_3 */
#define IRQ_VECTOR_PWM_WRAP_0   8	/* PWM_IRQ_WRAP_0 */
#define IRQ_VECTOR_PWM_WRAP_1   9	/* PWM_IRQ_WRAP_1 */
#define IRQ_VECTOR_DMA0         10	/* DMA_IRQ_0  */
#define IRQ_VECTOR_DMA1         11	/* DMA_IRQ_1  */
#define IRQ_VECTOR_DMA2         12	/* DMA_IRQ_2  */
#define IRQ_VECTOR_DMA3         13	/* DMA_IRQ_3  */
#define IRQ_VECTOR_USBCTRL      14	/* USBCTRL_IRQ */
#define IRQ_VECTOR_PIO0_0       15	/* PIO0_IRQ_0 */
#define IRQ_VECTOR_PIO0_1       16	/* PIO0_IRQ_1 */
#define IRQ_VECTOR_PIO1_0       17	/* PIO1_IRQ_0 */
#define IRQ_VECTOR_PIO1_1       18	/* PIO1_IRQ_1 */
#define IRQ_VECTOR_PIO2_0       19	/* PIO2_IRQ_0 */
#define IRQ_VECTOR_PIO2_1       20	/* PIO2_IRQ_1 */
#define IRQ_VECTOR_IO_BANK0     21	/* IO_IRQ_BANK0  */
#define IRQ_VECTOR_IO_BANK0_NS  22	/* IO_IRQ_BANK0 NONE SECURE  */
#define IRQ_VECTOR_QSPI         23	/* IO_IRQ_QSPI */
#define IRQ_VECTOR_QSPI_NS      24	/* IO_IRQ_QSPI NONE SECURE */
#define IRQ_VECTOR_SIO_FIFO     25	/* SIO_IRQ_FIFO */
#define IRQ_VECTOR_SIO_BELL     26	/* SIO_IRQ_BELL */
#define IRQ_VECTOR_SIO_FIFO_NS  27	/* SIO_IRQ_FIFO NONE SECURE */
#define IRQ_VECTOR_SIO_BELL_NS  28	/* SIO_IRQ_BELL NONE SECURE */
#define IRQ_VECTOR_MTIMECMP     29	/* SIO_IRQ_MTIMECMP */
#define IRQ_VECTOR_CLOCKS       30	/* CLOCKS_IRQ */
#define IRQ_VECTOR_SPI0         31	/* SPI0_IRQ */
#define IRQ_VECTOR_SPI1         32	/* SPI1_IRQ */
#define IRQ_VECTOR_UART0        33	/* UART0_IRQ */
#define IRQ_VECTOR_UART1        34	/* UART1_IRQ */
#define IRQ_VECTOR_ADC_FIFO     35	/* ADC_IRQ_FIFO */
#define IRQ_VECTOR_I2C0         36	/* I2C0_IRQ  */
#define IRQ_VECTOR_I2C1         37	/* I2C1_IRQ */
#define IRQ_VECTOR_OTP          38	/* OTP_IRQ */
#define IRQ_VECTOR_TRNG         39	/* TRNG_IRQ */
#define IRQ_VECTOR_PROC0_CTI    40	/* PROC0_IRQ_CTI */
#define IRQ_VECTOR_PROC1_CTI    41	/* PROC1_IRQ_CTI */
#define IRQ_VECTOR_PLL_SYS      42	/* PLL_SYS_IRQ */
#define IRQ_VECTOR_USB_SYS      43	/* PLL_USB_IRQ */
#define IRQ_VECTOR_POWMAN_POW   44	/* POWMAM_IRQ_POW */
#define IRQ_VECTOR_POWMAN_TIMER 45	/* POWMAM_IRQ_TIMER */
#define IRQ_VECTOR_SPAREIRQ_0   46	/* SPAREIRQ_IRQ_0 */
#define IRQ_VECTOR_SPAREIRQ_1   47	/* SPAREIRQ_IRQ_1 */
#define IRQ_VECTOR_SPAREIRQ_2   48	/* SPAREIRQ_IRQ_2 */
#define IRQ_VECTOR_SPAREIRQ_3   49	/* SPAREIRQ_IRQ_3 */
#define IRQ_VECTOR_SPAREIRQ_4   50	/* SPAREIRQ_IRQ_4 */
#define IRQ_VECTOR_SPAREIRQ_5   51	/* SPAREIRQ_IRQ_5 */

/*
 *  PSM
 */
#define TADR_PSM_FRCE_ON        0x40018000	/* (RW) Force block out of reset (i.e. power it on) */
  #define PSM_FRCE_ON_PROC1         0x01000000	/* proc1 (0) */
#define TADR_PSM_DONE           0x4001800C	/* (R)  Indicates the peripheral's registers are ready to access. */
  #define PSM_DONE_PROC1            0x01000000	/* proc1 (0) */

/*
 *  SINGLE-CYCLE IO BLOCK
 */
#define TADR_SIO_FIFO_ST        0xD0000050
  #define SIO_FIFO_ST_VLD           0x00000001	/* VLD (0): Value is 1 if this core's RX FIFO is not empty (i */
  #define SIO_FIFO_ST_RDY           0x00000002	/* RDY (1): Value is 1 if this core's TX FIFO is not full (i */
#define TADR_SIO_FIFO_WR        0xD0000054	/* (W)  Write access to this core's TX FIFO */
#define TADR_SIO_FIFO_RD        0xD0000058	/* (R)  Read access to this core's RX FIFO */
#define TADR_SIO_SPNLOCK        0xD0000100

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
 *  デバッグシリアルポート数の定義 (RP2350A中のUART0を使用する)
 */
#ifndef TNUM_PORT
#define TNUM_PORT	2u	/* サポートするシリアルポートの数 */
#endif
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
 *  150MHz用に設定、クロックを下げた場合、時間は増大する．
 */
#define SIL_DLY_TIM1    30
#define SIL_DLY_TIM2    27


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
 *  クロック値を取得
 */
extern UW get_Clock(UB index);

/*
 *  実行コアを取得する
 */
extern UW get_CoreNo(void);

/*
 *  バイナリインフォのアドレスを取り出す
 */
extern const void *get_Binaryinfo(void);

/*
 *  コア１の実行
 */
extern void mprc_initialize(FP entry, UW *sp, UW vec);

#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
