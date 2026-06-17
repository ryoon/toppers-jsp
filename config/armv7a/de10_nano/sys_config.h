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
 *  @(#) $Id: sys_config.h 1952 2026-03-09 16:39:19Z roi $
 */

/*
 *		ターゲット依存モジュール（DE10-Nano用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  コア固有データ定義
 */
#include <cpu_insn.h>

/*
 *  CPUクロック設定
 */
#define SOCFPGA_CORE_CLK	800000000U
#define SOCFPGA_PERIPHCLK	(SOCFPGA_CORE_CLK/4000000U)

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK		(SOCFPGA_PERIPHCLK * 1000u)

/*
 *  プライベートタイマのプレスケーラ
 */
#define MPCORE_TM_PS_1MS	0u

/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO		(211)

/*
 *  割込み優先度の範囲
 */
#define TMIN_INTPRI		(0)		/* 割込み優先度の最小値（最高値）*/
#define TMAX_INTPRI		(31)	/* 割込み優先度の最大値（最低値）*/

/*
 *  システムタイマ割込み優先度
 */
#define INTPRI_TIMER    9

/*
 *  割込みベクター定義
 */
/*
 *  INTERRUPT NUMBER
 */
#define IREQ_SGI0                       0	/* Software Interrupt #0 */
#define IREQ_SGI1                       1	/* Software Interrupt #1 */
#define IREQ_SGI2                       2	/* Software Interrupt #2 */
#define IREQ_SGI3                       3	/* Software Interrupt #3 */
#define IREQ_SGI4                       4	/* Software Interrupt #4 */
#define IREQ_SGI5                       5	/* Software Interrupt #5 */
#define IREQ_SGI6                       6	/* Software Interrupt #6 */
#define IREQ_SGI7                       7	/* Software Interrupt #7 */
#define IREQ_SGI8                       8	/* Software Interrupt #8 */
#define IREQ_SGI9                       9	/* Software Interrupt #9 */
#define IREQ_SGI10                      10	/* Software Interrupt #10 */
#define IREQ_SGI11                      11	/* Software Interrupt #11 */
#define IREQ_SGI12                      12	/* Software Interrupt #12 */
#define IREQ_SGI13                      13	/* Software Interrupt #13 */
#define IREQ_SGI14                      14	/* Software Interrupt #14 */
#define IREQ_SGI15                      15	/* Software Interrupt #15 */

#define IREQ_PPI0                       16	/* Private Peripheral Interrupts #0 */
#define IREQ_PPI_TIMER_GLOBAL           27	/* Private Peripheral Interrupts Global Timer */
#define IREQ_PPI_TIMER_PRIVATE          29	/* Private Peripheral Interrupts Private Timer */
#define IREQ_PPI_TIMER_WATCHDOG         30	/* Private Peripheral Interrupts watchdog */

#define IREQ_CPU0_PARITYFAIL            32	/* CPU0 PARITYFAIL */
#define IREQ_CPU0_PARITYFAIL_BTAC       33	/* CPU0 PARITYFAIL BTAC */
#define IREQ_CPU0_PARITYFAIL_GHB        34	/* CPU0 PARITYFAIL GHB */
#define IREQ_CPU0_PARITYFAIL_I_TAG      35	/* CPU0 PARITYFAIL I_TAG */
#define IREQ_CPU0_PARITYFAIL_I_DATA     36	/* CPU0 PARITYFAIL I_DATA */
#define IREQ_CPU0_PARITYFAIL_TLB        37	/* CPU0 PARITYFAIL TLB */
#define IREQ_CPU0_PARITYFAIL_D_OUTER    38 	/* CPU0 PARITYFAIL D_OUTER */
#define IREQ_CPU0_PARITYFAIL_D_TAG      39	/* CPU0 PARITYFAIL D_TAG */
#define IREQ_CPU0_PARITYFAIL_D_DATA     40	/* CPU0 PARITYFAIL D_DATA  */
#define IREQ_CPU0_DEFLAGS0              41	/* CPU0 DEFFLAG0 */
#define IREQ_CPU0_DEFLAGS1              42	/* CPU0 DEFFLAG1 */
#define IREQ_CPU0_DEFLAGS2              43	/* CPU0 DEFFLAG2 */
#define IREQ_CPU0_DEFLAGS3              44	/* CPU0 DEFFLAG3 */
#define IREQ_CPU0_DEFLAGS4              45	/* CPU0 DEFFLAG4 */
#define IREQ_CPU0_DEFLAGS5              46	/* CPU0 DEFFLAG5 */
#define IREQ_CPU0_DEFLAGS6              47	/* CPU0 DEFFLAG6 */

#define IREQ_CPU1_PARITYFAIL            48	/* CPU1 PARITYFAIL */
#define IREQ_CPU1_PARITYFAIL_BTAC       49	/* CPU1 PARITYFAIL BTAC */
#define IREQ_CPU1_PARITYFAIL_GHB        50	/* CPU1 PARITYFAIL GHB */
#define IREQ_CPU1_PARITYFAIL_I_TAG      51	/* CPU1 PARITYFAIL I_TAG */
#define IREQ_CPU1_PARITYFAIL_I_DATA     52	/* CPU1 PARITYFAIL I_DATA */
#define IREQ_CPU1_PARITYFAIL_TLB        53	/* CPU1 PARITYFAIL TLB */
#define IREQ_CPU1_PARITYFAIL_D_OUTER    54	/* CPU1 PARITYFAIL D_OUTER */
#define IREQ_CPU1_PARITYFAIL_D_TAG      55	/* CPU1 PARITYFAIL D_TAG */
#define IREQ_CPU1_PARITYFAIL_D_DATA     56	/* CPU1 PARITYFAIL D_DATA */
#define IREQ_CPU1_DEFLAGS0              57	/* CPU1 DEFFLAG0 */
#define IREQ_CPU1_DEFLAGS1              58	/* CPU1 DEFFLAG1 */
#define IREQ_CPU1_DEFLAGS2              59	/* CPU1 DEFFLAG2 */
#define IREQ_CPU1_DEFLAGS3              60	/* CPU1 DEFFLAG3 */
#define IREQ_CPU1_DEFLAGS4              61	/* CPU1 DEFFLAG4 */
#define IREQ_CPU1_DEFLAGS5              62	/* CPU1 DEFFLAG5 */
#define IREQ_CPU1_DEFLAGS6              63	/* CPU1 DEFFLAG6 */

/*
 *  Interrupts sourced from the Snoop Control Unit (SCU).
 */
#define IREQ_SCU_PARITYFAIL0            64
#define IREQ_SCU_PARITYFAIL1            65
#define IREQ_SCU_EV_ABORT               66

/*
 *  Interrupts sourced from the L2 Cache Controller.
 */
#define IREQ_L2_ECC_BYTE_WR_IRQ         67
#define IREQ_L2_ECC_CORRECTED_IRQ       68
#define IREQ_L2_ECC_UNCORRECTED_IRQ     69
#define IREQ_L2_COMBINED_IRQ            70

/*
 *  Interrupts sourced from the SDRAM Controller.
 */
#define IREQ_DDR_ECC_ERROR_IRQ          71

/*
 *  Interrupt request from the FPGA logic, 0 - 63.
 */
#define IREQ_F2S_FPGA_IRQ0              72
#define IREQ_F2S_FPGA_IRQ1              73
#define IREQ_F2S_FPGA_IRQ2              74
#define IREQ_F2S_FPGA_IRQ3              75
#define IREQ_F2S_FPGA_IRQ4              76
#define IREQ_F2S_FPGA_IRQ5              77
#define IREQ_F2S_FPGA_IRQ6              78
#define IREQ_F2S_FPGA_IRQ7              79
#define IREQ_F2S_FPGA_IRQ8              80
#define IREQ_F2S_FPGA_IRQ9              81
#define IREQ_F2S_FPGA_IRQ10             82
#define IREQ_F2S_FPGA_IRQ11             83
#define IREQ_F2S_FPGA_IRQ12             84
#define IREQ_F2S_FPGA_IRQ13             85
#define IREQ_F2S_FPGA_IRQ14             86
#define IREQ_F2S_FPGA_IRQ15             87
#define IREQ_F2S_FPGA_IRQ16             88
#define IREQ_F2S_FPGA_IRQ17             89
#define IREQ_F2S_FPGA_IRQ18             90
#define IREQ_F2S_FPGA_IRQ19             91
#define IREQ_F2S_FPGA_IRQ20             92
#define IREQ_F2S_FPGA_IRQ21             93
#define IREQ_F2S_FPGA_IRQ22             94
#define IREQ_F2S_FPGA_IRQ23             95
#define IREQ_F2S_FPGA_IRQ24             96
#define IREQ_F2S_FPGA_IRQ25             97
#define IREQ_F2S_FPGA_IRQ26             98
#define IREQ_F2S_FPGA_IRQ27             99
#define IREQ_F2S_FPGA_IRQ28             100
#define IREQ_F2S_FPGA_IRQ29             101
#define IREQ_F2S_FPGA_IRQ30             102
#define IREQ_F2S_FPGA_IRQ31             103
#define IREQ_F2S_FPGA_IRQ32             104
#define IREQ_F2S_FPGA_IRQ33             105
#define IREQ_F2S_FPGA_IRQ34             106
#define IREQ_F2S_FPGA_IRQ35             107
#define IREQ_F2S_FPGA_IRQ36             108
#define IREQ_F2S_FPGA_IRQ37             109
#define IREQ_F2S_FPGA_IRQ38             110
#define IREQ_F2S_FPGA_IRQ39             111
#define IREQ_F2S_FPGA_IRQ40             112
#define IREQ_F2S_FPGA_IRQ41             113
#define IREQ_F2S_FPGA_IRQ42             114
#define IREQ_F2S_FPGA_IRQ43             115
#define IREQ_F2S_FPGA_IRQ44             116
#define IREQ_F2S_FPGA_IRQ45             117
#define IREQ_F2S_FPGA_IRQ46             118
#define IREQ_F2S_FPGA_IRQ47             119
#define IREQ_F2S_FPGA_IRQ48             120
#define IREQ_F2S_FPGA_IRQ49             121
#define IREQ_F2S_FPGA_IRQ50             122
#define IREQ_F2S_FPGA_IRQ51             123
#define IREQ_F2S_FPGA_IRQ52             124
#define IREQ_F2S_FPGA_IRQ53             125
#define IREQ_F2S_FPGA_IRQ54             126
#define IREQ_F2S_FPGA_IRQ55             127
#define IREQ_F2S_FPGA_IRQ56             128
#define IREQ_F2S_FPGA_IRQ57             129
#define IREQ_F2S_FPGA_IRQ58             130
#define IREQ_F2S_FPGA_IRQ59             131
#define IREQ_F2S_FPGA_IRQ60             132
#define IREQ_F2S_FPGA_IRQ61             133
#define IREQ_F2S_FPGA_IRQ62             134
#define IREQ_F2S_FPGA_IRQ63             135

/*
 *  Interrupts sourced from the DMA Controller.
 */
#define IREQ_DMA_IRQ0                   136
#define IREQ_DMA_IRQ1                   137
#define IREQ_DMA_IRQ2                   138
#define IREQ_DMA_IRQ3                   139
#define IREQ_DMA_IRQ4                   140
#define IREQ_DMA_IRQ5                   141
#define IREQ_DMA_IRQ6                   142
#define IREQ_DMA_IRQ7                   143
#define IREQ_DMA_IRQ_ABORT              144
#define IREQ_DMA_ECC_CORRECTED_IRQ      145
#define IREQ_DMA_ECC_UNCORRECTED_IRQ    146

/*
 *  Interrupts sourced from the Ethernet MAC 0 (EMAC0).
 */
#define IREQ_EMAC0_IRQ                  147
#define IREQ_EMAC0_TX_ECC_CORRECTED_IRQ 148
#define IREQ_EMAC0_TX_ECC_UNCORRECTED_IRQ 149
#define IREQ_EMAC0_RX_ECC_CORRECTED_IRQ 150
#define IREQ_EMAC0_RX_ECC_UNCORRECTED_IRQ 151

/*
 *  Interrupts sourced from the Ethernet MAC 1 (EMAC1).
 */
#define IREQ_EMAC1_IRQ                  152
#define IREQ_EMAC1_TX_ECC_CORRECTED_IRQ 153
#define IREQ_EMAC1_TX_ECC_UNCORRECTED_IRQ 154
#define IREQ_EMAC1_RX_ECC_CORRECTED_IRQ 155
#define IREQ_EMAC1_RX_ECC_UNCORRECTED_IRQ 156

/*
 *  Interrupts sourced from the USB OTG 0.
 */
#define IREQ_USB0_IRQ                   157
#define IREQ_USB0_ECC_CORRECTED         158
#define IREQ_USB0_ECC_UNCORRECTED       159

/*
 *  Interrupts sourced from the USB OTG 1.
 */
#define IREQ_USB1_IRQ                   160
#define IREQ_USB1_ECC_CORRECTED         161
#define IREQ_USB1_ECC_UNCORRECTED       162

/*
 *  Interrupts sourced from the CAN Controller 0.
 */
#define IREQ_CAN0_STS_IRQ               163
#define IREQ_CAN0_MO_IRQ                164
#define IREQ_CAN0_ECC_CORRECTED_IRQ     165
#define IREQ_CAN0_ECC_UNCORRECTED_IRQ   166,

/*
 *  Interrupts sourced from the CAN Controller 1.
 */
#define IREQ_CAN1_STS_IRQ               167
#define IREQ_CAN1_MO_IRQ                168
#define IREQ_CAN1_ECC_CORRECTED_IRQ     169
#define IREQ_CAN1_ECC_UNCORRECTED_IRQ   170

/*
 *  Interrupts sourced from the SDMMC Controller.
 */
#define IREQ_SDMMC_IRQ                  171
#define IREQ_SDMMC_PORTA_ECC_CORRECTED  172
#define IREQ_SDMMC_PORTA_ECC_UNCORRECTED 173
#define IREQ_SDMMC_PORTB_ECC_CORRECTED  174
#define IREQ_SDMMC_PORTB_ECC_UNCORRECTED 175

/*
 *  Interrupts sourced from the NAND Controller.
 */
#define IREQ_NAND_IRQ                   176
#define IREQ_NANDR_ECC_CORRECTED_IRQ    177
#define IREQ_NANDR_ECC_UNCORRECTED_IRQ  178
#define IREQ_NANDW_ECC_CORRECTED_IRQ    179
#define IREQ_NANDW_ECC_UNCORRECTED_IRQ  180
#define IREQ_NANDE_ECC_CORRECTED_IRQ    181
#define IREQ_NANDE_ECC_UNCORRECTED_IRQ  182

/*
 *  Interrupts sourced from the QSPI Controller.
 */
#define IREQ_QSPI_IRQ                   183
#define IREQ_QSPI_ECC_CORRECTED_IRQ     184
#define IREQ_QSPI_ECC_UNCORRECTED_IRQ   185

/*
 *  Interrupts sourced from the SPI Controllers 0 - 3.
 *  SPI0_IRQ corresponds to SPIM0. SPI1_IRQ corresponds to SPIM1.
 *  SPI2_IRQ corresponds to SPIS0. SPI3_IRQ corresponds to SPIS1.
 */
#define IREQ_SPI0_IRQ                   186
#define IREQ_SPI1_IRQ                   187
#define IREQ_SPI2_IRQ                   188
#define IREQ_SPI3_IRQ                   189

/*
 *  Interrupts sourced from the I2C Controllers 0 - 3.
 */
#define IREQ_I2C0_IRQ                   190
#define IREQ_I2C1_IRQ                   191
#define IREQ_I2C2_IRQ                   192
#define IREQ_I2C3_IRQ                   193

/*
 *  Interrupts sourced from the UARTs 0 - 1.
 */
#define IREQ_UART0                      194
#define IREQ_UART1                      195

/*
 *  Interrupts sourced from the GPIO 0 - 2.
 */
#define IREQ_GPIO0                      196
#define IREQ_GPIO1                      197
#define IREQ_GPIO2                      198

/*
 *  Interrupts sourced from the Timer controllers.
 */
#define IREQ_TIMER_L4SP_0_IRQ           199
#define IREQ_TIMER_L4SP_1_IRQ           200
#define IREQ_TIMER_OSC1_0_IRQ           201
#define IREQ_TIMER_OSC1_1_IRQ           202

/*
 *  Interrupts sourced from the Watchdog Timers 0 - 1.
 */
#define IREQ_WDOG0_IRQ                  203
#define IREQ_WDOG1_IRQ                  204

/*
 *  Interrupts sourced from the Clock Manager.
 */
#define IREQ_CLKMGR_IRQ                 205

/*
 *  Interrupts sourced from the Clock Manager MPU Wakeup.
 */
#define IREQ_MPUWAKEUP_IRQ              206

/*
 *  Interrupts sourced from the FPGA Manager.
 */
#define IREQ_FPGA_MAN_IRQ               207

/*
 *  Interrupts sourced from the CoreSight for CPU0 and CPU1's CTI.
 */
#define IREQ_NCTIIRQ0                   208
#define IREQ_NCTIIRQ1                   209

/*
 *  Interrupts sourced from the On-chip RAM.
 */
#define IREQ_RAM_ECC_CORRECTED_IRQ      210
#define IREQ_RAM_ECC_UNCORRECTED_IRQ    211

/*
 *  GIC 割込みの定義
 */
#define GIC_INTNO_TEX		IREQ_SGI0
#define GIC_INTNO_DIS		IREQ_SGI1
#define GIC_INTNO_EXT		IREQ_SGI2
#define GIC_INTNO_PPI0		IREQ_PPI0
#define GIC_INTNO_SPI0		IREQ_CPU0_PARITYFAIL


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
#define SIL_DLY_TIM1    1
#define SIL_DLY_TIM2    2

/*
 *  ベクタの数（アラインを考慮）
 */
#define	NUM_VECTORS		212

/*
 *  メモリのベースアドレスとサイズ
 */
#define DDR_ADDR		0x00000000
#define DDR_SIZE		0x40000000 /* 1000MB */

/*
 *  MPCoreのメモリベースアドレス
 */
#define MPCORE_PMR_BASE	0xFFFEC000

/*
 *  GIC Base Address
 */
#define GICC_BASE		(MPCORE_PMR_BASE + 0x0100)
#define GICD_BASE		(MPCORE_PMR_BASE + 0x1000)

/*
 *  L3REGS
 */
#define L3REGS_BASE 0xFF800000
#define L3REGS_REMAP (L3REGS_BASE + 0x00)

#define L3REGS_REMAP_MPUZERO     0x01
#define L3REGS_REMAP_NONMPUZERO  0x02
#define L3REGS_REMAP_HPS2FPGA    0x08
#define L3REGS_REMAP_LWHP2FPGA   0x10

/*
 *  RSTMGR
 */
#define RSTMGR_BASE		0xFFD05000
#define RSTMGR_MPUMODRST  (RSTMGR_BASE + 0x10)
#define RSTMGR_PERMODRST  (RSTMGR_BASE + 0x14)
#define RSTMGR_BRGMODRST  (RSTMGR_BASE + 0x1C)

#define RSTMGR_MPUMODRST_CPU1  0x02

#define RSTMGR_PERMODRST_L4WD0 0x40

#define RSTMGR_BRGMODRST_H2F   0x01
#define RSTMGR_BRGMODRST_LWH2F 0x02 
#define RSTMGR_BRGMODRST_F2H   0x04

/*
 *  SYSMGR
 */
#define SYSMGR_BASE 0xFFD08000
#define SYSMGR_CPU1STARTADDR (SYSMGR_BASE + 0xC4)

/*
 *  MPUL2
 */
#define MPUL2_BASE 0xFFFEF000

#define MPUL2_ADRFILTER (MPUL2_BASE  + 0xC00)


#ifndef _MACRO_ONLY

/*
 *  マルチプロセッサ処理モデルの実現
 *
 *  コア番号は標準実装．
 *  その他はシングルコアの場合、実装は不要．
 */

/*
 *  ロック取得解放関数（マルチコアのみ必要）
 */

/*
 *  最大使用可能なスピンロックの数
 */
#define	TNUM_SPINLOCKID	(64)

/*
 *  カーネル部使用のスピンロック定義
 */
#define KERNEL_GLOCK_SPNLOCK	(0)
#define KERNEL_TOOL_SPNLOCK		(0)

/*
 *  スピンロックの初期化
 */
Inline void
x_initialize_spin(ID spnid, LOCK *p_spn_lock)
{
	*p_spn_lock = 0;
}

/*
 *  ジャイアントロックの初期化（G_LOCKの場合）
 */
Inline void
x_initialize_giant_lock(LOCK *p_giant_lock)
{
	*p_giant_lock = 0;
}

/*
 *  ロック取得失敗時の割込み待ち
 *
 *  ロック取得失敗時にスピンを繰り返すのではなく，wfeにより，
 *  イベント待ちとすることが推奨されている．
 *  wfeによるイベント待ちの際，割込み応答性を低下させないため，割込みを許
 *  可した状態て，イベント待ちとするべきである．wfe命令自信は，割込みを禁
 *  止・許可を操作しないため，別途msr命令により割込みを許可する．
 *
 *  その際，msr実行により割込みが許可された直後(wfeを実行する前に)に割込
 *  みが入り，その後割込みハンドラからのリターン後にwfeになり，イベントを
 *  逃す可能性が考えられる．
 *
 *  しかしながら， 割込みハンドラからのリターン(正確には例外一般からのリタ
 *  ーン)を行うと，イベントレジスタがセットされ，wfe実行時にイベントレジス
 *  タがセットされていると，クリアしてwfeから即座にリターンするための問題
 *  ない．
 */

/*
 *  1段目のロック取得（タスク・非タスクコンテキスト共用）
 */
Inline void
x_acquire_lock(LOCK *p_lock)
{
	int		locked;

	while(TRUE) {
		Asm("   mov      r2, #0x01   \n"
			"\t ldrex    r1, [%1]    \n"
			"\t cmp      r1, #0x00   \n"
			"\t msrne    cpsr_c, %2  \n" /* 割込みの許可(bit0-7のみ書き換え) */
			#ifndef TOPPERS_OMIT_USE_WFE
			"\t wfene                \n"
			#endif /* TOPPERS_OMIT_USE_WFE */
			"\t msrne    cpsr_c, %3  \n" /* 割込みの禁止(bit0-7のみ書き換え) */
			"\t strexeq  r1, r2,[%1] \n"
			"\t mov      %0, r1      \n"
			:"=r"(locked)
			:"r"(p_lock),"I"(CPSR_SVC|CPSR_ALWAYS_SET),"I"(CPSR_SVC|CPSR_CPULOCK|CPSR_ALWAYS_SET)
			:"r1","r2", "cc");

		if(locked == 0){
			/* ロック取得成功 */
			CP15_DATA_MEMORY_BARRIER();
			Asm("":::"memory");
			return;
		}
	}
}

#define t_acquire_lock(p_lock) x_acquire_lock(p_lock)
#define i_acquire_lock(p_lock) x_acquire_lock(p_lock)

/*
 *  ロックの解放（タスク・非タスクコンテキスト共用）
 */
Inline void
x_release_lock(LOCK *p_lock)
{
	Asm("":::"memory");
	CP15_DATA_MEMORY_BARRIER();
	*p_lock = 0;
	CP15_DATA_MEMORY_BARRIER();
	Asm("sev");
}

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
 *  プロセッサ間割込み要求
 */
extern void x_ipi_raise(ID prcid, UH event);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */

extern void	sys_putc(char c) throw();

/*
 *  コア１の実行
 */
extern void mprc_initialize(void);

/*
 *  MMUの初期化：システムレベル
 */
extern void sys_mmu_init(void);

/*
 *  PPRC2 boot code(sys_support.S)
 */
extern UB prc2_boot;
extern UB prc2_boot_end;

#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
