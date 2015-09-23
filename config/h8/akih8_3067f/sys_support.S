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
 *  Copyright (C) 2001,2002 by Kunihiko Ohnaka
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
 *  @(#) $Id: sys_support.S,v 1.2 2002/04/11 11:55:11 hiro Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"

	.h8300h

/*
 * 割込みベクタの定義
 */

	.section	.vectors
	.align 4

	.global _vectors
_vectors:

#ifdef AKI_MONITOR

	.long	_start				/*  0, 0x0000: reset	*/
	.long	0				/*  1, 0x0004: reserved	*/
	.long	0				/*  2, 0x0008: reserved	*/
	.long	0				/*  3, 0x000c: reserved	*/
	.long	0				/*  4, 0x0010: reserved	*/
	.long	0				/*  5, 0x0014: reserved	*/
	.long	0				/*  6, 0x0018: reserved	*/
	.long	0				/*  7, 0x001c: NMI	*/
	.long	0				/*  8, 0x0020: trap	*/
	.long	0				/*  9, 0x0024: trap	*/
	.long	0				/* 10, 0x0028: trap	*/
	.long	0				/* 11, 0x002c: trap	*/
	.long	0				/* 12, 0x0030: IRQ0	*/
	.long	0				/* 13, 0x0034: IRQ1	*/
	.long	0				/* 14, 0x0038: IRQ2	*/
	.long	0				/* 15, 0x003c: IRQ3	*/
	.long	0				/* 16, 0x0040: IRQ4	*/
	.long	0				/* 17, 0x0044: IRQ5	*/
	.long	0				/* 18, 0x0048: reserved	*/
	.long	0				/* 19, 0x004c: reserved	*/
	.long	0				/* 20, 0x0050: WOVI	*/
	.long	0				/* 21, 0x0054: CMI	*/
	.long	0				/* 22, 0x0058: reserved	*/
	.long	0				/* 23, 0x005c: ADI	*/
	.long	_timer_handler_entry		/* 24, 0x0060: IMIA0	*/
	.long	0				/* 25, 0x0064: IMIB0	*/
	.long	0				/* 26, 0x0068: OVI0	*/
	.long	0				/* 27, 0x006c: reserved	*/
	.long	0				/* 28, 0x0070: IMIA1	*/
	.long	0				/* 29, 0x0074: IMIB1	*/
	.long	0				/* 30, 0x0078: OVI1	*/
	.long	0				/* 31, 0x007c: reserved	*/
	.long	0				/* 32, 0x0080: IMIA2	*/
	.long	0				/* 33, 0x0084: IMIB2	*/
	.long	0				/* 34, 0x0088: OVI2	*/
	.long	0				/* 35, 0x008c: reserved	*/
	.long	0				/* 36, 0x0090: CIMIA0	*/
	.long	0				/* 37, 0x0094: CIMIB0	*/
	.long	0				/* 38, 0x0098: CIMIA1/CIMIB1 */
	.long	0				/* 39, 0x009c: TOVI0/TOVI1   */
	.long	0				/* 40, 0x00a0: CIMIA2	*/
	.long	0				/* 41, 0x00a4: CIMIA2	*/
	.long	0				/* 42, 0x00a8: CIMIA3/CIMIB3 */
	.long	0				/* 43, 0x00ac: TOVI2/TOVI3   */
	.long	0				/* 44, 0x00b0: DEND0A	*/
	.long	0				/* 45, 0x00b4: DEND0B	*/
	.long	0				/* 46, 0x00b8: DEND1A	*/
	.long	0				/* 47, 0x00bc: DEND1B	*/
	.long	0				/* 48, 0x00c0: reserved	*/
	.long	0				/* 49, 0x00c4: reserved	*/
	.long	0				/* 50, 0x00c8: reserved	*/
	.long	0				/* 51, 0x00cc: reserved	*/

#if NUM_PORT == 1

	.long	0				/* 52, 0x00d0: ERI0	*/
	.long	0				/* 53, 0x00d4: RXI0	*/
	.long	0				/* 54, 0x00d8: TXI0	*/
	.long	0				/* 55, 0x00ec: TEI0	*/
	.long	_serial_in_handler_entry	/* 56, 0x00e0: ERI1	*/
	.long	_serial_in_handler_entry	/* 57, 0x00e4: RXI1	*/
	.long	_serial_out_handler_entry	/* 58, 0x00e8: TXI1	*/
	.long	0				/* 59, 0x00ec: TEI1	*/
	.long	0				/* 60, 0x00f0: ERI2	*/
	.long	0				/* 61, 0x00f4: RXI2	*/
	.long	0				/* 62, 0x00f8: TXI2	*/
	.long	0				/* 63, 0x00fc: TEI2	*/

#elif NUM_PORT == 2	/* of #if NUM_PORT == 1 */

	.long	_serial_in_handler_entry	/* 52, 0x00d0: ERI0	*/
	.long	_serial_in_handler_entry	/* 53, 0x00d4: RXI0	*/
	.long	_serial_out_handler_entry	/* 54, 0x00d8: TXI0	*/
	.long	0				/* 55, 0x00ec: TEI0	*/
	.long	_serial_in_handler_entry	/* 56, 0x00e0: ERI1	*/
	.long	_serial_in_handler_entry	/* 57, 0x00e4: RXI1	*/
	.long	_serial_out_handler_entry	/* 58, 0x00e8: TXI1	*/
	.long	0				/* 59, 0x00ec: TEI1	*/
	.long	0				/* 60, 0x00f0: ERI2	*/
	.long	0				/* 61, 0x00f4: RXI2	*/
	.long	0				/* 62, 0x00f8: TXI2	*/
	.long	0				/* 63, 0x00fc: TEI2	*/

#endif	/* of #if NUM_PORT == 1 */

#else	/* of #ifdef AKI_MONITOR */

	.long	_start				/*  0, 0x0000: reset	*/
	.long	_no_reg_exception		/*  1, 0x0004: reserved	*/
	.long	_no_reg_exception		/*  2, 0x0008: reserved	*/
	.long	_no_reg_exception		/*  3, 0x000c: reserved	*/
	.long	_no_reg_exception		/*  4, 0x0010: reserved	*/
	.long	_no_reg_exception		/*  5, 0x0014: reserved	*/
	.long	_no_reg_exception		/*  6, 0x0018: reserved	*/
	.long	_no_reg_exception		/*  7, 0x001c: NMI	*/
	.long	_no_reg_exception		/*  8, 0x0020: trap	*/
	.long	_no_reg_exception		/*  9, 0x0024: trap	*/
	.long	_no_reg_exception		/* 10, 0x0028: trap	*/
	.long	_no_reg_exception		/* 11, 0x002c: trap	*/
	.long	_no_reg_exception		/* 12, 0x0030: IRQ0	*/
	.long	_no_reg_exception		/* 13, 0x0034: IRQ1	*/
	.long	_no_reg_exception		/* 14, 0x0038: IRQ2	*/
	.long	_no_reg_exception		/* 15, 0x003c: IRQ3	*/
	.long	_no_reg_exception		/* 16, 0x0040: IRQ4	*/
	.long	_no_reg_exception		/* 17, 0x0044: IRQ5	*/
	.long	_no_reg_exception		/* 18, 0x0048: reserved	*/
	.long	_no_reg_exception		/* 19, 0x004c: reserved	*/
	.long	_no_reg_exception		/* 20, 0x0050: WOVI	*/
	.long	_no_reg_exception		/* 21, 0x0054: CMI	*/
	.long	_no_reg_exception		/* 22, 0x0058: reserved	*/
	.long	_no_reg_exception		/* 23, 0x005c: ADI	*/
	.long	_timer_handler_entry		/* 24, 0x0060: IMIA0	*/
	.long	_no_reg_exception		/* 25, 0x0064: IMIB0	*/
	.long	_no_reg_exception		/* 26, 0x0068: OVI0	*/
	.long	_no_reg_exception		/* 27, 0x006c: reserved	*/
	.long	_no_reg_exception		/* 28, 0x0070: IMIA1	*/
	.long	_no_reg_exception		/* 29, 0x0074: IMIB1	*/
	.long	_no_reg_exception		/* 30, 0x0078: OVI1	*/
	.long	_no_reg_exception		/* 31, 0x007c: reserved	*/
	.long	_no_reg_exception		/* 32, 0x0080: IMIA2	*/
	.long	_no_reg_exception		/* 33, 0x0084: IMIB2	*/
	.long	_no_reg_exception		/* 34, 0x0088: OVI2	*/
	.long	_no_reg_exception		/* 35, 0x008c: reserved	*/
	.long	_no_reg_exception		/* 36, 0x0090: CIMIA0	*/
	.long	_no_reg_exception		/* 37, 0x0094: CIMIB0	*/
	.long	_no_reg_exception		/* 38, 0x0098: CIMIA1/CIMIB1 */
	.long	_no_reg_exception		/* 39, 0x009c: TOVI0/TOVI1   */
	.long	_no_reg_exception		/* 40, 0x00a0: CIMIA2	*/
	.long	_no_reg_exception		/* 41, 0x00a4: CIMIA2	*/
	.long	_no_reg_exception		/* 42, 0x00a8: CIMIA3/CIMIB3 */
	.long	_no_reg_exception		/* 43, 0x00ac: TOVI2/TOVI3   */
	.long	_no_reg_exception		/* 44, 0x00b0: DEND0A	*/
	.long	_no_reg_exception		/* 45, 0x00b4: DEND0B	*/
	.long	_no_reg_exception		/* 46, 0x00b8: DEND1A	*/
	.long	_no_reg_exception		/* 47, 0x00bc: DEND1B	*/
	.long	_no_reg_exception		/* 48, 0x00c0: reserved	*/
	.long	_no_reg_exception		/* 49, 0x00c4: reserved	*/
	.long	_no_reg_exception		/* 50, 0x00c8: reserved	*/
	.long	_no_reg_exception		/* 51, 0x00cc: reserved	*/

#if NUM_PORT == 1

	.long	_no_reg_exception		/* 52, 0x00d0: ERI0	*/
	.long	_no_reg_exception		/* 53, 0x00d4: RXI0	*/
	.long	_no_reg_exception		/* 54, 0x00d8: TXI0	*/
	.long	_no_reg_exception		/* 55, 0x00ec: TEI0	*/
	.long	_serial_error_handler_entry	/* 56, 0x00e0: ERI1	*/
	.long	_serial_in_handler_entry	/* 57, 0x00e4: RXI1	*/
	.long	_serial_out_handler_entry	/* 58, 0x00e8: TXI1	*/
	.long	_no_reg_exception		/* 59, 0x00ec: TEI1	*/
	.long	_no_reg_exception		/* 60, 0x00f0: ERI2	*/
	.long	_no_reg_exception		/* 61, 0x00f4: RXI2	*/
	.long	_no_reg_exception		/* 62, 0x00f8: TXI2	*/
	.long	_no_reg_exception		/* 63, 0x00fc: TEI2	*/

#elif NUM_PORT == 2	/* of #if NUM_PORT == 1 */

	.long	_serial_in_handler_entry	/* 52, 0x00d0: ERI0	*/
	.long	_serial_in_handler_entry	/* 53, 0x00d4: RXI0	*/
	.long	_serial_out_handler_entry	/* 54, 0x00d8: TXI0	*/
	.long	_no_reg_exception		/* 55, 0x00ec: TEI0	*/
	.long	_serial_in_handler_entry	/* 56, 0x00e0: ERI1	*/
	.long	_serial_in_handler_entry	/* 57, 0x00e4: RXI1	*/
	.long	_serial_out_handler_entry	/* 58, 0x00e8: TXI1	*/
	.long	_no_reg_exception		/* 59, 0x00ec: TEI1	*/
	.long	_no_reg_exception		/* 60, 0x00f0: ERI2	*/
	.long	_no_reg_exception		/* 61, 0x00f4: RXI2	*/
	.long	_no_reg_exception		/* 62, 0x00f8: TXI2	*/
	.long	_no_reg_exception		/* 63, 0x00fc: TEI2	*/

#endif	/* of #if NUM_PORT == 1 */

#endif	/* of #ifdef AKI_MONITOR */

	.text
	.align 2

/*
 *  リリースでは、外部 RAM 有効にする。
 */

	.global	_hardware_init_hook
_hardware_init_hook:

	/* CCR の UI ビットを割り込みマスクビットとして使用する。*/

	orc	#H8CCR_UI, ccr
	mov.l	#H8SYSCR, er0
	mov.b	@er0, r1h
	bclr	#H8SYSCR_UE_BIT, r1h
	mov.b	r1h, @er0

	/* 全割り込みの優先度を上げる。*/

	mov.b	#0xff, r1l

	add.b	#(H8IPRA - H8SYSCR), r0l
	mov.b	r1l, @er0
	add.b	#(H8IPRB - H8IPRA), r0l
	mov.b	r1l, @er0

	/* 外部 RAM を有効にする。*/

	mov.l	#H8P1DDR, er0
	mov.b	r1l, @er0	/* enable A0  - A7  */

	add.b	#(H8P2DDR - H8P1DDR), r0l
	mov.b	r1l, @er0	/* enable A8  - A15 */

	add.b	#(H8P3DDR - H8P2DDR), r0l
	mov.b	r1l, @er0	/* enable D8  - D15 */

	add.b	#(H8P5DDR - H8P3DDR), r0l
	mov.b	r1l, @er0	/* enable A16 - A19 */

	add.b	#(H8PADDR - H8P5DDR), r0l
	mov.b	r1l, @er0	/* enable A20 - A23 */

#ifdef ENABLE_LOWER_DATA

	mov.l	#H8P4DDR, er0
	mov.b	r1l, @er0	/* enable D0  - D7  */

#endif	/* #if ENABLE_LOWER_DATA */

#ifdef ENABLE_P8_CS

	mov.l	#H8P8DDR, er0
	mov.b	#ENABLE_P8_CS, r1l
	mov.b	r1l, @er0	/* enable CS0 - CS3 */

#endif	/* #ifdef ENABLE_P8_CS */

#ifdef ENABLE_PB_CS

	mov.l	#H8PBDDR, er0
	mov.b	#ENABLE_PB_CS, r1l
	mov.b	r1l, @er0	/* enable CS4 - CS7 */

#endif	/* #ifdef ENABLE_PB_CS */

	rts
