/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: sys_support.S,v 1.4 2002/04/10 11:17:16 takayuki Exp $
 */

/*
 *	ターゲットシステム依存モジュール アセンブリ言語部（RTE-V850E/MA1-CB用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "v850asm.inc"

#define Set(x,y,z) Lea x, r6; Lea y, r7; st.##z r7, 0[r6]

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */

	.text
	.globl hardware_init_hook
hardware_init_hook:	

		/* 動作クロックを50MHzへ変更 */
	stsr	psw,	r10
	addi	0x80,	r10,	r6
	ldsr	r6,		psw
	mov		0x7,	r6
	addi	-0x800, r0, 	r7
	st.b	r6,		0[r7]
	st.b	r6,		0x22[r7]
	nop
	nop
	nop
	nop
	nop
	ldsr	r10,	psw

	Set(BCT0, 0x88b8, h)
	Set(DWC0, 0x1111, h)
	Set(VSWC, 0x14,   b)
	Set(BCC,  0xffc0, h)
	Set(ASC,  0x5555, h)
	Set(BCP,  0x0,    b)
	Set(CSC0, 0xfcf3, h)
	Set(CSC1, 0x2c11, h)
	Set(SCR1, 0x2096, h)
	Set(RFS1, 0x8017, h)

	jmp		r31



		/* 標準割込み処理とスぺーサー */
	.macro StandardInterruptEntry name
	.globl \name
\name:
	jr	vector_handler
	nop;nop;nop;nop;nop;nop
	.endm

	.macro ReservedInterruptEntry
	nop;nop;nop;nop;nop;nop;nop;nop
	.endm

/*
 * 割込みハンドラ部分の定義
 * 		とりあえず一箇所に集める。
 */

	.section	".vectors","ax"
	.align      4
	.globl      __reset
__reset:
	jr	start
	nop;nop;nop;nop;nop;nop

	StandardInterruptEntry NMI
	ReservedInterruptEntry
	ReservedInterruptEntry
	StandardInterruptEntry TRAP0
	StandardInterruptEntry TRAP1
	StandardInterruptEntry DBG
	ReservedInterruptEntry
	StandardInterruptEntry INTOV00
	StandardInterruptEntry INTOV01
	StandardInterruptEntry INTOV02
	StandardInterruptEntry INTOV03
	StandardInterruptEntry INTTM000
	StandardInterruptEntry INTTM001
	StandardInterruptEntry INTTM010
	StandardInterruptEntry INTTM011
	StandardInterruptEntry INTTM020
	StandardInterruptEntry INTTM021
	StandardInterruptEntry INTTM030
	StandardInterruptEntry INTTM031
	StandardInterruptEntry INTP100
	StandardInterruptEntry INTP101
	StandardInterruptEntry INTP102
	StandardInterruptEntry INTP103
	StandardInterruptEntry INTP110
	StandardInterruptEntry INTP111
	StandardInterruptEntry INTP112
	StandardInterruptEntry INTP113
	StandardInterruptEntry INTP120
	StandardInterruptEntry INTP121
	StandardInterruptEntry INTP122
	StandardInterruptEntry INTP123
	StandardInterruptEntry INTP130
	StandardInterruptEntry INTP131
	StandardInterruptEntry INTP132
	StandardInterruptEntry INTP133
	StandardInterruptEntry INTCMD0
	StandardInterruptEntry INTCMD1
	StandardInterruptEntry INTCMD2
	StandardInterruptEntry INTCMD3
	StandardInterruptEntry INTDMA0
	StandardInterruptEntry INTDMA1
	StandardInterruptEntry INTDMA2
	StandardInterruptEntry INTDMA3
	StandardInterruptEntry INTCSI0
	StandardInterruptEntry INTSER0
	StandardInterruptEntry INTSR0
	StandardInterruptEntry INTST0
	StandardInterruptEntry INTCSI1
	StandardInterruptEntry INTSER1
	StandardInterruptEntry INTSR1
	StandardInterruptEntry INTST1
	StandardInterruptEntry INTCSI2
	StandardInterruptEntry INTSER2
	StandardInterruptEntry INTSR2
	StandardInterruptEntry INTST2
	StandardInterruptEntry INTAD

/*
 * 評価用高性能?タイマ
 *  (今回は評価用フリーランニングタイマを作らずに直接タイマカウンタをのぞく)
 */
Function _vxget_tim
	Lea		TMD(TIMER_PORT), r7
	ld.h	0[r7], r8
	st.w	r8, 0[r6]
	mov		r0, r10
	jmp		r31
