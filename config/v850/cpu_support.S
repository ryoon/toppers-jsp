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
 *  @(#) $Id: cpu_support.S,v 1.1 2000/11/24 03:41:29 takayuki Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（68040用）
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
	mov		sp, ep
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
	Lea		__kernel_runtsk, ep
	sld.w	0[ep], ep
	sst.w	sp, TCB_sp[ep]
	Lea		dispatch_r, r10
	sst.w	r10, TCB_pc[ep]

		/*
		 * 次のタスクの準備 __kernel_exit_and_dispatch
		 *
		 *  特にexit固有の処理はしてないので 強制ディスパッチにも使ってる
		 */
Label __kernel_exit_and_dispatch

		/* 割込みスタックの退避 */
	cmp		r2, r0
	bnz		1f
	mov		r3, r2

		/* 次に起動すべきタスクの読み出し */
1:
	Lea		__kernel_schedtsk, r10
	ld.w	0[r10], r10
	cmp		r10,r0
	bne		1f

		/* 次に起動すべきタスクがない */
	ei
	halt
	di

	jr		1b
1:

		/* runtsk = schedtsk; */
	Lea		__kernel_runtsk, r11
	st.w	r10, 0[r11]

		/* スタックと次の起動番地を復帰 */
	mov		r10, ep
	sld.w	TCB_pc[ep], r31
	sld.w	TCB_sp[ep], r3
	ei
	jmp		r31

		/* ディスパッチャの復帰ルーチン */
Label dispatch_r

		/* タスク例外 */
	jarl	__kernel_calltex, r31
	
		/* 退避したコンテキストの復帰 */
	mov		sp, ep
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

	Pop		ep
	jmp		r31

/*
 *  タスク起動時処理
 */

Function __kernel_activate_r
	Lea		__kernel_runtsk,	ep
	ld.w	0[ep], ep
	ld.w	TCB_tinib[ep], ep
	ld.w	TINIB_exinf[ep], r6
	ld.w	TINIB_task[ep], r10
	Lea		_ext_tsk, r31
	
	jmp		r10

/*
 *  割込みハンドラ／CPU例外ハンドラ出口処理
 *
 *  ret_int は割込みモード・割込み禁止状態で，ret_exc はマスタモード・
 *  割込み禁止状態で呼び出さなければならない．また ret_exc は，スクラッ
 *  チレジスタを保存した状態で呼び出すこと．
 */


/*
 * CPUロック状態の取得
 *
 *   割込み禁止ならCPUロック状態
 */

Function _sense_lock
	stsr	psw, r10
	shr		5, r10
	andi	1, r10, r10
	jmp		r31


/*
 * コンテキストモードの取得
 *
 *  スタックポインタがRAM領域の最後尾_end以降にあれば非タスクコンテキスト
 */

Function _sense_context
	mov		r0, r10
	Lea		_end, r6
	cmp		r6, r3
	bnh		1f
	mov		1, r10
1:
	jmp		r31


/*
 * 評価用高性能タイマ
 */

Function _vxget_tim
	
	mov		r0, r10
	cmp		r6, r0
	be		1f

	Lea		TIMERCOUNTER, r7
	ld.w	0[r7], r7
	st.w	r7, 0[r6]
1:
	jmp		r31




