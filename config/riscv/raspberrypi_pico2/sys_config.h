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
 *  @(#) $Id: sys_config.h 1952 2026-01-13 21:12:44Z roi $
 */

/*
 *		ターゲット依存モジュール（Raspberry Pi Pico2 RISC-V用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  RISC-Vマクロの定義
 */
#include <cpu_config.h>

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
#define SYS_CLOCK		    (SystemFrequency)

#endif /* _MACRO_ONLY */

/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO      (51)

/*
 *  カーネル管理の割込み優先度の範囲
 */
#ifndef TMIN_INTPRI
#define TMIN_INTPRI     (0)		/* 割込み優先度の最小値（最低値）*/
#endif /* TMIN_INTPRI */
#define TMAX_INTPRI     (15)	/* 割込み優先度の最大値（最高値） */

/*
 *  MACHINE INTERRUPT NUMBER
 */
#define IRQ_MACHINE_SOFTWARE    3
#define IRQ_MACHIE_TIMER        7
#define IRQ_MACHINE_EXTERNAL    11

/*
 *  INTERRUPT NUMBER
 */
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
 *  MACHINE EXCEPTION NUMBER
 */
#define EXC_INSTRUCTION_ADDRESS_MISALIGNED  0
#define EXC_INSTRUCTION_ADDRESS_FAULT       1
#define EXC_ILLEGAL_INSTRUCTION             2
#define EXC_BREAKPOINT                      3
#define EXC_LOAD_ADDRESS_MISALIGNED         4
#define EXC_LOAD_ADDRESS_FAULT              5
#define EXC_STORE_AMO_ADDRESS_MISALIGNED    6
#define EXC_STORE_AMO_ACCESS_FAUT           7
#define EXC_ENVIRONMENT_CALL_FROM_UMODE     8
#define EXC_ENVIRONMENT_CALL_FROM_MMODE     11

/*
 *  HAZARD3割込み定義
 */
#define hazard3_irqarray_set(csr, index, data) (set_csr(csr, (index) | ((UW)(data) << 16)))
#define hazard3_irqarray_clear(csr, index, data) (clear_csr(csr, (index) | ((UW)(data) << 16)))
#define hazard3_irqarray_read(csr, index) (set_csr(csr, (index)) >> 16)
#define hazard3_set_csr(csr, data)  (set_csr((csr), (data)))
#define hazard3_clear_csr(csr, data)  (clear_csr((csr), (data)))
#define hazard3_read_csr(csr)  (read_csr((csr)))

#define RVCSR_MEIEA         0x00000BE0
#define RVCSR_MEIPA         0x00000BE1
#define RVCSR_MEIFA         0x00000BE2
#define RVCSR_MEIPRA        0x00000BE3
#define RVCSR_MEINEXT       0x00000BE4
#define RVCSR_MEINEXT_UPDATE      0x00000001
#define RVCSR_MEICONTEXT    0x00000BE5
#define RVCSR_MEICONTEXT_CLEARTS  0x00000002

/*
 *  PSM
 */
#define TADR_PSM_FRCE_ON    0x40018000	/* (RW) Force block out of reset (i.e. power it on) */
  #define PSM_FRCE_ON_PROC1     0x01000000	/* proc1 (0) */
#define TADR_PSM_DONE       0x4001800C	/* (R)  Indicates the peripheral's registers are ready to access. */
  #define PSM_DONE_PROC1        0x01000000	/* proc1 (0) */

/*
 *  SINGLE-CYCLE IO BLOCK
 */
#define TADR_SIO_FIFO_ST    0xD0000050
  #define SIO_FIFO_ST_VLD        0x00000001	/* VLD (0): Value is 1 if this core's RX FIFO is not empty (i */
  #define SIO_FIFO_ST_RDY        0x00000002	/* RDY (1): Value is 1 if this core's TX FIFO is not full (i */
  #define SIO_FIFO_ST_WOF        0x00000004	/* WOF (0): Sticky flag indicating the TX FIFO was written when full */
  #define SIO_FIFO_ST_ROE        0x00000008	/* ROE (0): Sticky flag indicating the RX FIFO was read when empty */
#define TADR_SIO_FIFO_WR    0xD0000054	/* (W)  Write access to this core's TX FIFO */
#define TADR_SIO_FIFO_RD    0xD0000058	/* (R)  Read access to this core's RX FIFO */
#define TADR_SIO_SPNLOCK    0xD0000100

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
#define	SUPPORT_VXGET_TIM

/*
 *  デバッグシリアルポート数の定義 (RP2350A中のUART0を使用する)
 */
#ifndef TNUM_PORT
#define TNUM_PORT	2u	/* サポートするシリアルポートの数 */
#endif
#define	TNUM_SIOP	2u	/* サポートするシリアルI/Oポートの数 */

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
#define SIL_DLY_TIM1    30
#define SIL_DLY_TIM2    12


#ifndef _MACRO_ONLY

/*
 *  タイマ用時間調整データ
 */
extern UD compare_time[];
extern W  molecule[];

/*
 *  HARZARD3割込みベクタ
 */
extern UW ivector[];

/*
 *  割込みハンドラテーブル
 */
extern FP* const p_int_table[];

/*
 *  割込み有効化マップ
 */
extern UD	interrupt_map[];

/*
 *  PLIC THRESHOLD（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline UW
current_ithreshold(void)
{
	UW threshold = hazard3_read_csr(RVCSR_MEICONTEXT);

	threshold = threshold >> 16 & 0x1F;
	if(threshold > 0)
		threshold--;
	return threshold;
}

/*
 *  PLIC THRESHOLD（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_ithreshold(UW threshold)
{
	if(threshold > 0)
		threshold++;
	hazard3_clear_csr(RVCSR_MEICONTEXT, (0x1F << 16));
	hazard3_set_csr(RVCSR_MEICONTEXT, (threshold << 16));
}

/*
 *  PLIC 割込み優先度（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline UW
current_ipriority(INTNO intno)
{
	UH priority_row = (UH) hazard3_irqarray_read(RVCSR_MEIPRA, intno / 4);
	return  (priority_row >> (4 * (intno % 4))) & 0x0F;
}

/*
 *  PLIC 割込み優先度（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_ipriority(INTNO intno, UW priority)
{
	hazard3_irqarray_clear(RVCSR_MEIPRA, intno / 4, 0xFU << (4 * (intno % 4)));
	hazard3_irqarray_set(RVCSR_MEIPRA, intno / 4, (priority & 0xFU) << (4 * (intno % 4)));
}

/*
 *  割込み要求禁止フラグのセット
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをセットしようとした場合には，falseを返す．
 */
Inline BOOL
disable_int(INTNO intno)
{
	if(intno > TMAX_INTNO && (interrupt_map[x_prc_index()] & (1 << intno)) == 0){
		return FALSE;
	}
	hazard3_irqarray_clear(RVCSR_MEIFA, (intno / 16), (1 << (intno % 16)));
	return TRUE;
}

/*
 *  割込み要求禁止フラグのクリア
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline BOOL
enable_int(INTNO intno)
{
	if(intno > TMAX_INTNO && (interrupt_map[x_prc_index()] & (1 << intno)) == 0){
		return FALSE;
	}
	hazard3_irqarray_clear(RVCSR_MEIFA, (intno / 16), (1 << (intno % 16)));
	hazard3_irqarray_set(RVCSR_MEIEA, (intno / 16), (1 << (intno % 16)));
	return TRUE;
}

/*
 *  割込み要求のクリア
 */
Inline void
clear_int(INTNO intno)
{
}

/*
 *  割込み要求のチェック
 */
Inline BOOL
probe_int(INTNO intno)
{
	UH penddata = (UH) hazard3_irqarray_read(RVCSR_MEIPA, (intno / 16));
	return  (penddata & (1 << (intno % 16))) != 0;
}

/*
 *  割込みハンドラの設定
 */
Inline void
define_inh(INHNO inhno, FP int_entry)
{
	FP *vector_table = (FP *)p_int_table[x_prc_index()];

	if(inhno > TMAX_INTNO)
		timer_inh(int_entry);
	else
		vector_table[inhno] = int_entry;
}


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
			Asm("fence rw, rw" : : : "memory");
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
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)  extern void inthdr(void)

#define INT_ENTRY(inthdr) inthdr

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
 *  イレギュラーな割込み処理
 */
extern void default_hazard3_handler(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */

extern void	sys_putc(char c) throw();

/*
 *  割込み要求ラインの属性の設定
 *
 *  PICO2では，優先度0～15までの設定．
 */
extern void	x_config_int(INTNO intno, BOOL active, PRI intpri);

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
