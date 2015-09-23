/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  Copyright (C) 2005 by Freelines CO.,Ltd
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
 *  @(#) $Id: cpu_support.S,v 1.8 2005/12/12 09:08:16 honda Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（V850ES用）
 */

#define	_MACRO_ONLY
#include "v850asm.inc"
#include "jsp_kernel.h"
#include "offset.h"

/*
 *  タスクディスパッチャ
 */

Function	__kernel_dispatch

		/*
		 * コンテキストの退避 
		 *  30 20 21 22 23 24 25 26 27 28 29 31
		 */

	Push	ep

		/* 保存対象レジスタの保存 */
	addi	-44, sp, sp
	mov	sp, ep
	sst.w	r20, 40[ep]
	sst.w	r21, 36[ep]
	sst.w	r22, 32[ep]
	sst.w	r23, 28[ep]
	sst.w	r24, 24[ep]
	sst.w	r25, 20[ep]
	sst.w	r26, 16[ep]
	sst.w	r27, 12[ep]
	sst.w	r28, 8[ep]
	sst.w	r29, 4[ep]
	sst.w	r31, 0[ep]
	
		/* スタックと次の起動番地を保存 */
	Lea	__kernel_runtsk, ep
	sld.w	0[ep], ep
	sst.w	sp, TCB_sp[ep]
	Lea	dispatch_r, r10
	sst.w	r10, TCB_pc[ep]
	stsr	psw, r10
	sst.w	r10, TCB_psw[ep]

		/*
		 * 次のタスクの準備 __kernel_exit_and_dispatch
		 *
		 *  特にexit固有の処理はしてないので 強制ディスパッチにも使ってる
		 */
Label __kernel_exit_and_dispatch

		/* intnestをクリア */
	Lea	_intnest, r10
	st.w	r0, 0[r10]

		/* 割込みスタックの退避 */
	cmp	r2, r0
	bnz	1f
	mov	r3, r2

		/* 次に起動すべきタスクの読み出し */
1:
	Lea	__kernel_schedtsk, r10
	ld.w	0[r10], r10
	cmp	r10,r0
	bne	1f

		/* 次に起動すべきタスクがない */
	mov	r2, r3

		/* intnestを１にする */
	mov	r0, r10
	add	1, r10
	Lea	_intnest, r11
	st.w	r10, 0[r11]

		/* 割り込み待ち */
	ei
	nop
	nop
	nop
	nop
	nop
	di

		/* intnestをクリアする */
	Lea	_intnest, r10
	st.w	r0, 0[r10]

	jr	1b
1:

		/* runtsk = schedtsk; */
	Lea	__kernel_runtsk, r11
	st.w	r10, 0[r11]

		/* スタックと次の起動番地を復帰 */
	mov	r10, ep
	sld.w	TCB_pc[ep], r31
	sld.w	TCB_sp[ep], r3
	sld.w	TCB_psw[ep], r10
	ldsr	r10, psw
	jmp	r31

		/* ディスパッチャの復帰ルーチン */
Label dispatch_r

		/* タスク例外 */
	jarl	__kernel_calltex, r31
	
		/* 退避したコンテキストの復帰 */
	mov	sp, ep
	sld.w	 40[ep],r20
	sld.w	 36[ep],r21
	sld.w	 32[ep],r22
	sld.w	 28[ep],r23
	sld.w	 24[ep],r24
	sld.w	 20[ep],r25
	sld.w	 16[ep],r26
	sld.w	 12[ep],r27
	sld.w	 8[ep],r28
	sld.w	 4[ep],r29
	sld.w	 0[ep],r31
	addi	44, sp, sp

	Pop	ep
	jmp	r31

/*
 *  タスク起動時処理
 */

Function __kernel_activate_r

	Lea	__kernel_runtsk, ep
	ld.w	0[ep], ep
	ld.w	TCB_tinib[ep], ep
	ld.w	TINIB_exinf[ep], r6
	ld.w	TINIB_task[ep], r10
	Lea	_ext_tsk, r31
	
	ei

	jmp	r10

/*
 *  割込みハンドラ／CPU例外ハンドラ出口処理
 *
 *  ret_int は割込みモード・割込み禁止状態で，ret_exc はマスタモード・
 *  割込み禁止状態で呼び出さなければならない．また ret_exc は，スクラッ
 *  チレジスタを保存した状態で呼び出すこと．
 */

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
 *  R10 R11
 *  R1-R9
 *  R11-R19
 *  R31
 *  EIPC EIPSW
 */

	.data
	.align 4
__intcount:	.long 0

	Function vector_handler

		/* 作業領域の作成 */
	add	-8,   sp
	st.w	r10, 4[sp]
	st.w	r11, 0[sp]

		/* intnestをインクリメント */
	Lea	_intnest, r11
	ld.w	0[r11], r10
	add	1, r10
	st.w	r10, 0[r11]

		/*
		 * ハンドラ起動番地の読み出し
		 *    R10 : ハンドラの番地
		 */
	stsr	ecr, r10
	shr	2, r10
	add	-4, r10
	Lea	_InterruptHandlerEntry, r11
	add	r10, r11
	ld.w	0[r11], r10

		/* ハンドラが無ければ即終了 */
	cmp	0,r10
	bne	1f

	stsr	ecr, r6
	jarl	_cpu_experr, r31
	be	end_handler

1:
	/*
	 * レジスタ退避 
	 *
	 * 格納順序 : (10 11) ^ 12..19 6..9 31 eipc eipsw | (sp)
	 */

	mov	r30, r11		/* epを使いたいのでちょっと退避 */
	addi	-80, r3, r30
	mov	r30, r3			/* スタックに反映 */
	sst.w	r1,  76[ep]
	sst.w	r2,  72[ep]
	sst.w	r3,  68[ep]
	sst.w	r4,  64[ep]
	sst.w	r5,  60[ep]
	sst.w	r6,  56[ep]
	sst.w	r7,  52[ep]
	sst.w	r8,  48[ep]
	sst.w	r9,  44[ep]
	sst.w	r12, 40[ep]
	sst.w	r13, 36[ep]
	sst.w	r14, 32[ep]
	sst.w	r15, 28[ep]
	sst.w	r16, 24[ep]
	sst.w	r17, 20[ep]
	sst.w	r18, 16[ep]
	sst.w	r19, 12[ep]
	sst.w	r31,  8[ep]
	stsr	eipc, r6		/* 多重割込み対策 */
	sst.w	r6 ,  4[ep]
	stsr	eipsw, r6
	sst.w	r6 ,  0[ep]
	mov	r11,  r30		/* ep復帰 */

		/*
		 * コンテキストチェックとスタック切り替え
		 */

		/* 多重割り込みチェック */
	Lea	_intnest, r11
	ld.w	0[r11], r11
	cmp	1, r11
	ble	1f
	
		/* 多重割込み */
	st.w	r3, -4[r3]
	add	-4, r3
	jr	2f
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

	Lea	ret_int, r31

	ei

	jmp	r10

	/*
	 * 割り込みからの復帰処理
	 */
Label	ret_int
	di

		/* intnestをディクリメント */
	Lea	_intnest, r11
	ld.w	0[r11], r10
	mov	1, r11
	sub	r11, r10
	Lea	_intnest, r11
	st.w	r10, 0[r11]

		/* スタックの復帰 */
	ld.w	0[r3],sp


		/* どこへ飛んでくのかチェック */
	Lea	_intnest, r11
	ld.w	0[r11], r10
	cmp	r0, r10
	bne	recover_from_int

		/* 
		 * タスクへ飛んでく場合ならreqflgをチェック
		 * 要求があれば全レジスタを保存してタスクディスパッチャへ
		 */
	Lea	__kernel_reqflg, r10
	ld.w	0[r10], r11
	cmp	r0, r11
	bnz	getting_ready_for_contextswitch

recover_from_int_with_tskctx:
		/* タスクに飛んでいく前にタスク例外 */
	jarl	__kernel_calltex, r31

recover_from_int:
	/*
	 * レジスタ復帰 
	 *
	 * 格納順序 : (10 11) 12..19 6..9 31 eipc eipsw ^ | (sp)
	 */

	mov	r30, r11		/* epを使いたいのでちょっと退避 */
	mov	r3,  r30
	sld.w	4[ep],r10
	ldsr	r10, eipc
	sld.w	0[ep],r10
	ldsr	r10, eipsw

	sld.w	76[ep], r1
	sld.w	72[ep], r2
	sld.w	68[ep], r3
	sld.w	64[ep], r4
	sld.w	60[ep], r5
	sld.w	56[ep], r6
	sld.w	52[ep], r7
	sld.w	48[ep], r8
	sld.w	44[ep], r9
	sld.w	40[ep], r12
	sld.w	36[ep], r13
	sld.w	32[ep], r14
	sld.w	28[ep], r15
	sld.w	24[ep], r16
	sld.w	20[ep], r17
	sld.w	16[ep], r18
	sld.w	12[ep], r19
	sld.w	 8[ep], r31

	addi	80, r30, r3
	mov	r11, r30


end_handler:
	/*
	 * 作業領域の復帰
	 *
	 * 格納順序 : 10 11 ^ (12..19 6..9 31 eipc eipsw | sp)
	 */

	add	8, sp
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
	Lea	__kernel_runtsk, ep
	sld.w	0[ep], ep				/* ep = _kernel_runtsk */
	
	sst.w	r3, TCB_sp[ep]
	Lea	recover_from_contextswitch, r10
	sst.w	r10, TCB_pc[ep]
	stsr	psw, r10
	sst.w	r10, TCB_psw[ep]
	
		/* タスクディスパッチャへ */
	Lea	__kernel_exit_and_dispatch, r10
	ldsr	r10, eipc
	/* カーネル内で割込み禁止にすべきなのに割込み許可になっている */
	/* 箇所があり、割込みのタイミングによってはシステムが破綻します。*/
	stsr	eipsw, r10
	ori	0x20,r10,r10
	ldsr	r10, eipsw
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
 * CPUロック状態の取得
 *
 *   割込み禁止ならCPUロック状態
 */

Function _sense_lock
	stsr	psw, r10
	shr	5, r10
	andi	1, r10, r10
	jmp	r31


