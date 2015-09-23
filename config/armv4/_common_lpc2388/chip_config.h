/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *                2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2009 by Suikan
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation
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
 */


/*
 *  ターゲットチップ依存モジュール（LPC2388用）
 *
 *  このインクルードファイルは，sys_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */


#ifndef _CHIP_CONFIG_H_
#define _CHIP_CONFIG_H_


/*
 * 割込みハンドラの最大数
 */
#define MAX_INT_NUM 31

/*
 *  vxget_tim をサポートするかどうかの定義
 */
#define  SUPPORT_VXGET_TIM



/*
 *  割込み待ち状態での実行ルーチン
 */
#ifdef ARM920T_CORE
#define WAIT_INTERRUPT  mcr p15, 0, r3, c7, c0, 4
#else
#define WAIT_INTERRUPT  nop
#endif

/*
 *  プロセッサのエンディアン
 */
#define SIL_ENDIAN  SIL_ENDIAN_LITTLE  /* リトルエンディアン */


/*
 * ----------------------- 割り込みベクトル宣言 --------------------
 * INTNO_UART0 ではなく INTO_U0 を使うのは、UARTをTOPPERS/JSP内部のデバイス名として
 * 使っており、UARTの番号とLPC2388の非同期通信デバイスの番号が一致しないためである。
 *
 */
#define	INTNO_WDT		0
/*reserverved interrupt 1*/
#define	INTNO_ARM0		2
#define	INTNO_ARM1		3
#define	INTNO_TIMER0	4
#define	INTNO_TIMER1	5
#define	INTNO_U0		6
#define	INTNO_U1		7
#define	INTNO_PWM1		8
#define	INTNO_I2C0		9
#define	INTNO_SSP0		10
#define	INTNO_SSP1		11
#define	INTNO_PLL		12
#define	INTNO_RTC		13
#define	INTNO_EINT0		14
#define	INTNO_EINT1		15
#define	INTNO_EINT2		16
#define	INTNO_EINT3		17
#define	INTNO_ADC0		18
#define	INTNO_I2C1		19
#define	INTNO_BOD		20
#define	INTNO_ETHERNET	21
#define	INTNO_USB		22
#define	INTNO_CAN		23
#define	INTNO_SD		24
#define	INTNO_GPDMA		25
#define	INTNO_TIMER2	26
#define	INTNO_TIMER3	27
#define	INTNO_U2		28
#define	INTNO_U3		29
#define	INTNO_I2C2		30
#define	INTNO_I2S		31

#define	INHNO_WDT		0
/*reserverved interrupt 1*/
#define	INHNO_ARM0		2
#define	INHNO_ARM1		3
#define	INHNO_TIMER0	4
#define	INHNO_TIMER1	5
#define	INHNO_U0		6
#define	INHNO_U1		7
#define	INHNO_PWM1		8
#define	INHNO_I2C0		9
#define	INHNO_SSP0		10
#define	INHNO_SSP1		11
#define	INHNO_PLL		12
#define	INHNO_RTC		13
#define	INHNO_EINT0		14
#define	INHNO_EINT1		15
#define	INHNO_EINT2		16
#define	INHNO_EINT3		17
#define	INHNO_ADC0		18
#define	INHNO_I2C1		19
#define	INHNO_BOD		20
#define	INHNO_ETHERNET	21
#define	INHNO_USB		22
#define	INHNO_CAN		23
#define	INHNO_SD		24
#define	INHNO_GPDMA		25
#define	INHNO_TIMER2	26
#define	INHNO_TIMER3	27
#define	INHNO_U2		28
#define	INHNO_U3		29
#define	INHNO_I2C2		30
#define	INHNO_I2S		31

/*
 * SIO1,2,3にはU0,U2,U3を割り当てる
 */
#define INHNO_SIO		INHNO_U0
#define INHNO_SIO2		INHNO_U2
#define INHNO_SIO3		INHNO_U3

/*
 * ----------------------- UART 設定 ---------------------------
 */

/*
 *     UARTのうち、幾つのポートをPDICとして使用するか宣言する。
 *     LPC2388版では最大3
 */
#define TNUM_SIOP_UART	TNUM_SIOP

/*
 *    UARTのレジスタが何バイト境界に配置されているかを指定する。指定できる値は1,2,4のいずれか。
 */
#define UART_BOUNDARY	4

/*
 * UART レジスタへのアクセスにsil_xxb_iop()使う場合にはこのマクロを宣言する。
 * 値は不要。宣言しない場合、sil_xxb_mem()によってアクセスする。LPC2388実装
 * ではmemアクセスを使用する。
 */
/* #define UART_IOP_ACCESS */

/*
 *     UARTのベース・アドレスを指定する。ベースアドレスとはTHRのアドレスのこと。
 *     U0,2,3を使うのは、LPC2388においてU1だけ別扱いのため
 */
#define UART0_ADDRESS 0xE000C000
#define UART1_ADDRESS 0xE0078000	/*U2*/
#define UART2_ADDRESS 0xE007C000	/*U3*/

/*
 * UART内部の分周回路の設定値を指定する。16ビットの値。lpc2388uart.c内部で
 * 上位と下位に分割してDLM/DLLレジスタに設定される。
 *
 * UARTのデバイザの出力は所望するボーレートの16倍でなければならない。
 * U0,2,3を使うのは、LPC2388においてU1だけ別扱いのため
 */
#define UART0_DIVISOR (LP2388_SYSTEM_UART0_CLOCK/16/UART0_BAUD_RATE)
#define UART1_DIVISOR (LP2388_SYSTEM_UART1_CLOCK/16/UART2_BAUD_RATE) /*U2*/
#define UART2_DIVISOR (LP2388_SYSTEM_UART1_CLOCK/16/UART3_BAUD_RATE) /*U3*/


/*
 * ----------------------- UART 設定おわり ---------------------------
 */


#ifndef _MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void    sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．ROMモニタ呼出しで実現することを想定し
 *  ている．
 */
extern void    sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */
extern void    sys_putc(char c);



/*
 *  IRQハンドラ(sys_support.S)
 */
extern void IRQ_Handler(void);

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)  extern void inthdr(void)

#define INT_ENTRY(inthdr) inthdr

/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
extern void define_inh(INHNO inhno, FP inthdr);

extern void undef_interrupt();

extern void init_IRQ();
extern void init_vector();
extern void init_uart0();

/*
 * 割り込みのイネーブル、ディセーブル
 */

extern ER dis_int(INTNO intno);
extern ER ena_int(INTNO intno);
extern ER chg_ims(unsigned int enableMask);
extern ER get_ims(unsigned int *p_enableMask);

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
