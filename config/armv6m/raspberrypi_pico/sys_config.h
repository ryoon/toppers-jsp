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
 *  @(#) $Id: sys_config.h 1952 2026-01-01 15:49:16Z roi $
 */

/*
 *		ターゲット依存モジュール（Raspberry Pi Pico用）
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
#define CLK_GPOUT0          0	/* GPIO Muxing 0 */
#define CLK_GPOUT1          1	/* GPIO Muxing 1 */
#define CLK_GPOUT2          2	/* GPIO Muxing 2 */
#define CLK_GPOUT3          3	/* GPIO Muxing 3 */
#define CLK_REF             4	/* Watchdog and timers reference clock */
#define CLK_SYS             5	/* Processors, bus fabric, memory, memory mapped registers */
#define CLK_PERI			6	/* Peripheral clock for UART and SPI */
#define CLK_USB             7	/* USB clock */
#define CLK_ADC             8	/* ADC clock */
#define CLK_RTC				9	/* Real time clock */
#define NUM_CLK             10
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
#define TMAX_INTNO      (25)

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
#define IRQ_VECTOR_TIMER0       0	/* TIMER_IRQ_0 */
#define IRQ_VECTOR_TIMER1       1	/* TIMER_IRQ_1 */
#define IRQ_VECTOR_TIMER2       2	/* TIMER_IRQ_2 */
#define IRQ_VECTOR_TIMER3       3	/* TIMER_IRQ_3 */
#define IRQ_VECTOR_PWM_WRAP     4	/* PWM_IRQ_WRAP */
#define IRQ_VECTOR_USBCTRL      5	/* USBCTRL_IRQ */
#define IRQ_VECTOR_XIP          6	/* XIP_IRQ */
#define IRQ_VECTOR_PIO0_0       7	/* PIO0_IRQ_0 */
#define IRQ_VECTOR_PIO0_1       8	/* PIO0_IRQ_1 */
#define IRQ_VECTOR_PIO1_0       9	/* PIO1_IRQ_0 */
#define IRQ_VECTOR_PIO1_1       10	/* PIO1_IRQ_1 */
#define IRQ_VECTOR_DMA0         11	/* DMA_IRQ_0  */
#define IRQ_VECTOR_DMA1         12	/* DMA_IRQ_1 */
#define IRQ_VECTOR_IO_BANK0     13	/* IO_IRQ_BANK0  */
#define IRQ_VECTOR_IO_QSPI      14	/* IO_IRQ_QSPI */
#define IRQ_VECTOR_SIO_PROC0    15	/* SIO_IRQ_PROC0 */
#define IRQ_VECTOR_SIO_PROC1    16	/* SIO_IRQ_PROC1 */
#define IRQ_VECTOR_CLOCKS       17	/* CLOCKS_IRQ */
#define IRQ_VECTOR_SPI0         18	/* SPI0_IRQ */
#define IRQ_VECTOR_SPI1         19	/* SPI1_IRQ */
#define IRQ_VECTOR_UART0        20	/* UART0_IRQ */
#define IRQ_VECTOR_UART1        21	/* UART1_IRQ */
#define IRQ_VECTOR_ADC_FIFO     22	/* ADC0_IRQ_FIFO */
#define IRQ_VECTOR_I2C0         23	/* I2C0_IRQ  */
#define IRQ_VECTOR_I2C1         24	/* I2C1_IRQ */
#define IRQ_VECTOR_RTC          25	/* RTC Interrupt  */

/*
 *  マルチプロセッサ制御使用するSIOの定義
 */
#define TADR_SIO_FIFO_ST        0xD0000050	/* (R)  Status register for inter-core FIFOs (mailboxes) */
  #define SIO_FIFO_ST_VLD       0x00000001	/* VLD (0): Value is 1 if this core's RX FIFO is not empty (i */
  #define SIO_FIFO_ST_RDY       0x00000002	/* RDY (1): Value is 1 if this core's TX FIFO is not full (i */
  #define SIO_FIFO_ST_WOF       0x00000004	/* WOF (0): Sticky flag indicating the TX FIFO was written when full */
  #define SIO_FIFO_ST_ROE       0x00000008	/* ROE (0): Sticky flag indicating the RX FIFO was read when empty */
#define TADR_SIO_FIFO_WR        0xD0000054	/* (W)  Write access to this core's TX FIFO */
#define TADR_SIO_FIFO_RD        0xD0000058	/* (R)  Read access to this core's RX FIFO */
#define TADR_SIO_SPNLOCK        0xD0000100

/*
 *  割込みクリアペンディングレジスタ
 */
#define TADR_NVIC_ICER0         0xE000E280	/* IRQ 0 to 31 Clear-Pending Register */

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
#ifndef TNUM_PORT
#define TNUM_PORT	2u	/* サポートするシリアルポートの数 */
#endif
#ifndef TNUM_SIOP
#define TNUM_SIOP	2u	/* サポートするシリアルI/Oポートの数 */
#endif

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
#define SIL_DLY_TIM1    40
#define SIL_DLY_TIM2    32

/*
 *  ベクタの数（アラインを考慮）
 */
#define	NUM_VECTORS		64

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
Inline UINT
x_prc_index(void)
{
	UINT index = *((volatile UW *)(TADR_SIO_CPUID));
	return ((index == 0) ? 0 : 1);
}

#define t_prc_index()	x_prc_index()
#define i_prc_index()	x_prc_index()

/*
 *  ロック取得解放関数（マルチコアのみ必要）
 */

/*
 *  最大使用可能なスピンロックの数
 */
#define	TNUM_SPINLOCKID	30

/*
 *  カーネル部使用のスピンロック定義
 */
#define KERNEL_GLOCK_SPNLOCK	(TADR_SIO_SPNLOCK+(TNUM_SPINLOCKID+1)*4)
#define KERNEL_TOOL_SPNLOCK		(TADR_SIO_SPNLOCK+TNUM_SPINLOCKID*4)

/*
 *  スピンロックの初期化
 */
Inline void
x_initialize_spin(ID spnid, LOCK *p_spn_lock)
{
	*p_spn_lock = (LOCK)(TADR_SIO_SPNLOCK+(spnid-1)*4);
}

/*
 *  ジャイアントロックの初期化（G_LOCKの場合）
 */
Inline void
x_initialize_giant_lock(LOCK *p_giant_lock)
{
	*p_giant_lock = (LOCK)KERNEL_GLOCK_SPNLOCK;
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
		locked = *((volatile int *)*p_lock);

		if(locked != 0){
			Asm("dmb":::"memory");
			/* ロック取得成功 */
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
	*((volatile int *)*p_lock) = 0;
}

/*
 *  プロセッサ間通信関数（マルチコアのみ必要）
 */

/*
 *  割込み要求のクリア
 */
Inline void
x_ipi_clear_int(INTNO intno)
{
	*((volatile int *)TADR_NVIC_ICER0 + (intno >> 5)) = (1 << (intno & 0x1f));
}

/*
 *  ターゲットプロセッサへの割込み要求（タスク・非タスクコンテキスト共用）
 */
Inline void
x_ipi_raise(ID prcid, UH event)
{
	*((volatile int *)TADR_SIO_FIFO_WR) = ((x_prc_index()+1) << 24) | (prcid << 16) | event;
}

/*
 *  プロセッサ間通信イベントを取り出す
 */
Inline UW
x_ipi_event(void)
{
	return *((volatile UW *)TADR_SIO_FIFO_RD);
}

/*
 *  ベクターテーブル
 */
extern const FP vector_table[][NUM_VECTORS];

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
 *  コア１の実行
 */
extern void mprc_initialize(FP entry, UW *sp, UW vec);

#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
