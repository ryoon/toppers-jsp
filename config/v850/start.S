/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: start.S,v 1.2 2000/11/27 15:08:07 takayuki Exp $
 */

#define	_MACRO_ONLY
#include "sys_config.h"
#include "v850asm.inc"
#include "offset.h"

		/* 標準割込み処理とスぺーサー */
	.macro StandardInterruptEntry name
Function \name
	jr	vector_handler
	.rept 6
		nop
	.endr
	.endm

/*
 * 割込みハンドラ部分の定義
 * 		とりあえず一箇所に集める。
 */

	.section	".vectors","ax"

Function __reset

	#Addr : 0x00000000 RESET
	jr	start
	.rept 6
		nop		/*スペーサ*/
	.endr

	StandardInterruptEntry __NMI
	StandardInterruptEntry __INTWDT
	StandardInterruptEntry __RESERVED3
	StandardInterruptEntry __TRAP0
	StandardInterruptEntry __TRAP1
	StandardInterruptEntry __ILGOP
	StandardInterruptEntry __RESERVED7
	StandardInterruptEntry __INTWDTM
	StandardInterruptEntry __INTP0
	StandardInterruptEntry __INTP1
	StandardInterruptEntry __INTP2
	StandardInterruptEntry __INTP3
	StandardInterruptEntry __INTP4
	StandardInterruptEntry __INTP5
	StandardInterruptEntry __INTP6
	StandardInterruptEntry __INTWTI
	StandardInterruptEntry __INTTM00
	StandardInterruptEntry __INTTM01
	StandardInterruptEntry __INTTM10
	StandardInterruptEntry __INTTM11
	StandardInterruptEntry __INTTM2
	StandardInterruptEntry __INTTM3
	StandardInterruptEntry __INTTM4
	StandardInterruptEntry __INTTM5
	StandardInterruptEntry __INTCSIO
	StandardInterruptEntry __INTSER0
	StandardInterruptEntry __INTCSI1
	StandardInterruptEntry __INTST0
	StandardInterruptEntry __INTCSI2
	StandardInterruptEntry __INTSER1
	StandardInterruptEntry __INTSR1
	StandardInterruptEntry __INTST1
	StandardInterruptEntry __INTAD
	StandardInterruptEntry __INTDMA0
	StandardInterruptEntry __INTDMA1
	StandardInterruptEntry __INTDMA2
	StandardInterruptEntry __INTWT

/*
 *	V850用スタートアップファイル
 */

	Function start

	di

	/*
	 * スタックポインタの初期化
	 */
	mov		r0, r2
	Lea		STACKTOP, r3

	/*
	 *  hardware_init_hook の呼出し（0 でない場合）
	 *
	 *  ターゲットハードウェアに依存して必要な初期化処理がある場合
	 *  は，hardware_init_hook という関数を用意する．
	 */

	Lea		hardware_init_hook, r1
	cmp		r1, r0
	bz		1f
	jarl	hardware_init_hook, r31
1:

	/*
	 *  bssセクションのクリア
	 *   アラインされているので4ずつ行く
	 */

	Lea		__bss_start, ep
	Lea		_end, r6
	Lea		1f, r31
1:
	cmp		ep, r6
	bl		1f
	sst.w	r0, 0[ep]
	add		4, ep
	jmp		r31
1:


	/*
	 *  dataセクションの初期化（ROM化対応）
	 *
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする．
	 */
	Lea		__idata_start, r6
	Lea		__idata_end, r7
	Lea		__data_start, r8
	Lea		1f, r31
1:
	cmp		r6, r7
	bl		1f
	ld.w	0[r6], r9
	st.w	r9, 0[r8]
	add		4, r6
	add		4, r8
	jmp		r31
1:

	/*
	 *  software_init_hook を呼出し（0 でない場合）
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */

	Lea		software_init_hook, r1
	cmp		r1, r1
	bz		1f
	jarl	software_init_hook, r31
1:

	/*
	 *  カーネルを起動する．
	 */
	jarl _kernel_start, r31
	halt




.extern _InterruptHandlerEntry
.extern __kernel_runtsk
.extern __kernel_calltex

/*
 * 割込みハンドラ
 *
 *  ハンドラ起動前に返却値格納レジスタとスクラッチレジスタを退避する．
 *  割込みハンドラ内でタスクスイッチが発生したときのみ全てのレジスタ
 *  を退避する．
 *
 * それぞれ状況に応じて次のような処理を行う
 * ・ハンドラがない場合
 *    作業域作成 -> ハンドラ読出 -> 作業域破棄
 *
 * ・ハンドラがあり、多重割込みである場合
 *    作業域作成 -> ハンドラ読出 -> レジスタA群退避 -> ハンドラ起動
 *     -> レジスタA群復帰 -> 作業域破棄
 *
 * ・ハンドラがあり、タスクから割込みがかかり、ハンドラ内で切替がない場合
 *    作業域作成 -> ハンドラ読出 -> レジスタA群退避 -> スタック切替
 *     -> ハンドラ起動 -> タスク例外起動 -> レジスタA群退避
 *     -> 作業域破棄
 *
 * ・ハンドラがあり、タスクから割込みがかかり、ハンドラ内で切替がある場合
 *    作業域作成 -> ハンドラ読出 -> レジスタA群退避 -> スタック切替
 *     -> ハンドラ起動 -> レジスタB群退避 -> ディスパッチャ
 *     -> レジスタB群復帰 -> タスク例外起動 -> レジスタA群退避
 *     -> 作業域破棄
 *
 * 退避レジスタ格納順序 (アドレス上位から順)
 *  R10 R11                             (A:作業域/リターン用レジスタ)
 *  R12 R13 R14 R15 R16 R17 R18 R19     (A:スクラッチレジスタ)
 *  R6 R7 R8 R9 R10                     (A:引数用レジスタ)
 *  EIPC EIPSW                          (A:割込み時PC, PSW)
 *  SP[現在値]                          (A:スタックポインタ)
 *  R30 R20 R21 R22 R23 R24 R26 R27     (B:その他退避対象)
 *  R28 R1  R4  R5  R6  R7  R8  R9
 */

	Function vector_handler

		/* 作業領域の作成 */
	st.w	r10, -4[sp]
	st.w	r11, -8[sp]
	add		-8, sp
	
		/*
		 * ハンドラ起動番地の読み出し
		 *    R10 : ハンドラの番地
		 */
	stsr	ecr, r10
	shr		2, r10
	add		-4, r10
	Lea		_InterruptHandlerEntry, r11
	add		r10, r11
	ld.w	0[r11], r10

		/* ハンドラが無ければ即終了 */
	cmp		0,r6
	be		end_handler

	/*
	 * レジスタ退避 
	 *
	 * 格納順序 : (10 11) ^ 12..19 6..9 31 eipc eipsw | (sp)
	 */

	mov		r30, r11		/* epを使いたいのでちょっと退避 */
	addi	-60, r3, r30
	sst.w	r12, 56[ep]
	sst.w	r13, 52[ep]
	sst.w	r14, 48[ep]
	sst.w	r15, 44[ep]
	sst.w	r16, 40[ep]
	sst.w	r17, 36[ep]
	sst.w	r18, 32[ep]
	sst.w	r19, 28[ep]
	sst.w	r6,  24[ep]
	sst.w	r7,  20[ep]
	sst.w	r8,  16[ep]
	sst.w	r9,  12[ep]
	sst.w	r31,  8[ep]
	stsr	eipc, r6		/* 多重割込み対策 */
	sst.w	r6 , 4[ep]
	stsr	eipsw, r6
	sst.w	r6 ,0[ep]
	mov		r30, r3			/* スタックに反映 */
	mov		r11, r30		/* ep復帰 */


		/*
		 * コンテキストチェックとスタック切り替え
		 */

		/* スタックチェック */
	Lea		_end, r11
	cmp		r11, r3
	bnh		1f
	
		/* 多重割込み */
	st.w	r3, -4[r3]
	add		-4, r3
	jr		2f
1:
		/* タスクコンテキスト上での割込み */

		/* 現在のスタック位置を割り込みスタック上に保存 */
	st.w	r3, -4[r2]

		/* スタック切り替え */
	addi	-4,r2, r3

2:

		/*
		 * ハンドラ起動
		 */

	Lea		ret_int, r31
	ei
	jmp		r10

	/*
	 * 割り込みからの復帰処理
	 */
Label	ret_int
	di

		/* スタックの復帰 */
	ld.w	0[r3],sp


		/* どこへ飛んでくのかチェック */
	Lea		_end, r10
	cmp		r10, r3
	bh		recover_from_int

		/* 
		 * タスクへ飛んでく場合ならreqflgをチェック
		 * 要求があれば全レジスタを保存してタスクディスパッチャへ
		 */
	Lea		__kernel_reqflg, r10
	ld.w	0[r10], r11
	cmp		r0, r11
	bnz		getting_ready_for_contextswitch

recover_from_int_with_tskctx:
		/* タスクに飛んでいく前にタスク例外 */
	jarl	__kernel_calltex, r31

recover_from_int:
	/*
	 * レジスタ復帰 
	 *
	 * 格納順序 : (10 11) 12..19 6..9 31 eipc eipsw ^ | (sp)
	 */

	mov		r30, r11		/* epを使いたいのでちょっと退避 */
	mov		r3,  r30
	sld.w	4[ep],r10
	ldsr	r10, eipc
	sld.w	0[ep],r10
	ldsr	r10, eipsw

	sld.w	56[ep], r12
	sld.w	52[ep], r13
	sld.w	48[ep], r14
	sld.w	44[ep], r15
	sld.w	40[ep], r16
	sld.w	36[ep], r17
	sld.w	32[ep], r18
	sld.w	28[ep], r19
	sld.w	24[ep], r6
	sld.w	20[ep], r7
	sld.w	16[ep], r8
	sld.w	12[ep], r9
	sld.w	 8[ep], r31

	addi	60, r30, r3
	mov		r11, r30


end_handler:
	/*
	 * 作業領域の復帰
	 *
	 * 格納順序 : 10 11 ^ (12..19 6..9 31 eipc eipsw | sp)
	 */
	add		8, sp
	ld.w	-4[sp], r10
	ld.w	-8[sp], r11
	
	reti


	/*
	 * タスクスイッチ発生に伴う全レジスタ退避
	 *
	 * 格納順序 : (10 11 12..19 6..9 31 eipc eipsw | sp) ^ 
	 *            30 20..29 1 4..9
	 */
getting_ready_for_contextswitch:

		/* reqflg = 0 */
	st.w	r0, 0[r10]
	
	st.w	ep, -4[sp]		/*epを使いたいので真っ先に退避 */
	addi	-68, sp, ep
	sst.w	r20, 64[ep]
	sst.w	r21, 60[ep]
	sst.w	r22, 56[ep]
	sst.w	r23, 52[ep]
	sst.w	r24, 48[ep]
	sst.w	r25, 44[ep]
	sst.w	r26, 40[ep]
	sst.w	r27, 36[ep]
	sst.w	r28, 32[ep]
	sst.w	r29, 28[ep]
	sst.w	 r1, 24[ep]
	sst.w	 r4, 20[ep]
	sst.w	 r5, 16[ep]
	sst.w	 r6, 12[ep]
	sst.w	 r7,  8[ep]
	sst.w	 r8,  4[ep]
	sst.w	 r9,  0[ep]
	mov		ep, sp

		/* TCBの中身を修正して復帰情報を設定 */
	Lea		__kernel_runtsk, ep
	sld.w	0[ep], ep				/* ep = _kernel_runtsk */
	
	sst.w	r3, TCB_sp[ep]
	Lea		recover_from_contextswitch, r10
	sst.w	r10, TCB_pc[ep]
	stsr	psw, r10
	sst.w	r10, TCB_psw[ep]
	
		/* タスクディスパッチャへ */
	Lea		__kernel_exit_and_dispatch, r10
	ldsr	r10, eipc
	reti


	/*
	 * 割込み脱出時コンテキストスイッチの復帰ルーチン
	 *
	 * 格納順序 : (10 11 12..19 6..9 31 eipc eipsw | sp 
	 *             30 20..29 1 4..9) ^
	 */
recover_from_contextswitch:
	di

		/* 退避したレジスタの一部復帰 */
	mov		sp, ep
	sld.w	64[ep], r20
	sld.w	60[ep], r21
	sld.w	56[ep], r22
	sld.w	52[ep], r23
	sld.w	48[ep], r24
	sld.w	44[ep], r25
	sld.w	40[ep], r26
	sld.w	36[ep], r27
	sld.w	32[ep], r28
	sld.w	28[ep], r29
	sld.w	24[ep], r1
	sld.w	20[ep], r4
	sld.w	16[ep], r5
	sld.w	12[ep], r6
	sld.w	 8[ep], r7
	sld.w	 4[ep], r8
	sld.w	 0[ep], r9
	addi	68, sp, sp

		/* おおもとの復帰処理へ飛ばす */
	jr recover_from_int_with_tskctx

/*
 * 例外ハンドラ (TRAP1)
 */
