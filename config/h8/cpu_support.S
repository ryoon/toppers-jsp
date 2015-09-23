/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: cpu_support.S,v 1.2 2001/11/12 15:23:41 abe Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include "sys_config.h"
#include "cpu_config.h"

	.h8300h

	.text
	.align 2

/*
 *  タスクディスパッチャ
 *
 *    　_dispatch は、割込み外ネストカウンタ = 0,割込み禁止状態
 *    で呼び出さなければならない．_exit_and_dispatch も，割込みネスト
 *    カウンタ = 0・割込み禁止状態で呼び出すのが原則であるが，カーネル
 *    起動時に対応するため，割込みネストカウンタ = 1で呼び出した場合に
 *    も対応している．
 */

	.global _dispatch
_dispatch:
	push.l	er6			/*  er2〜6を保存			*/
	push.l	er5
	push.l	er4
	push.l	er3
	push.l	er2
	mov.l	@_runtsk, er0		/* er0 <- runtsk		*/
	mov.l	sp, @(TCB_sp, er0)	/* タスクスタックポインタを保存	*/
	mov.l	#dispatch_r, er1	/* 実行再開番地			*/
	mov.l	er1, @(TCB_pc, er0)
	bra     dispatcher_1

dispatch_r:
	pop.l	er2			/*  er2〜6を復元			*/
	pop.l	er3
	pop.l	er4
	pop.l	er5
	pop.l	er6
	jmp	@_calltex

	.global	_exit_and_dispatch
_exit_and_dispatch:
	orc	#H8INT_DIS_ALL, ccr	/* 割り込み禁止			*/
	sub.l	er0, er0		/* 割込みネストカウンタをクリア	*/
	mov.l	er0, @_intnest

dispatcher_1:
	/*
	 * ここには割り込み禁止で来ること
	 */
	mov.l	@_schedtsk, er0		/* er0 <- schedtsk		*/
	or.l	er0, er0		/* schedtsk があるか？		*/
	beq	dispatcher_2		/* 無ければジャンプ		*/
	mov.l	er0, @_runtsk		/* schedtskをruntskに		*/
	mov.l	@(TCB_sp, er0), sp
	mov.l	@(TCB_pc, er0), er0	/* 実行再開番地を復帰		*/
	jmp	@er0

dispatcher_2:
	/*
	 *  ここで割込みモードに切り換えるのは，ここで発生する割込み処理
	 *  にどのスタックを使うかという問題の解決と，割込みハンドラ内で
	 *  のタスクディスパッチの防止という二つの意味がある．
	 */
	    			/* スタックを割り込みスタックに切替え	*/
	mov.l	#STACKTOP, sp
				/*  割込みネストカウンタを１にする		*/
	sub.l	er0, er0
	inc.l	#1, er0
	mov.l	er0, @_intnest

	andc	#H8INT_ENA_ALL, ccr	/* 割り込み許可			*/
	sleep				/* 割込み待ち			*/
	orc	#H8INT_DIS_ALL, ccr	/* 割り込み禁止			*/
	sub.l	er0, er0		/* 割込みネストカウンタをクリア	*/
	mov.l	er0, @_intnest
	bra	dispatcher_1

/*
 *  割込みハンドラ出口処理
 *
 * 戻り先がタスクでreqflgがセットされている場合のみここにくる。
 * 割込みネストカウンタ = 0,割込み禁止状態,スクラッチレジスタを
 * 保存した状態で呼び出すこと。
 *
 */

	.globl ret_int
ret_int:

	mov.l	@_enadsp, er0		/* enadspのチェック		*/
	or.l	er0, er0	/* ディスパッチ禁止ならret_int_rへ	*/
	beq	ret_int_r

	mov.l	@_runtsk, er0		/* er0 <- runtsk		*/
	mov.l	@_schedtsk, er1		/* er1 <- schedtsk		*/
	cmp.l	er0, er1		/* runtsk と schedtsk を比較	*/
	beq	ret_int_r		/* 同じならret_int_rへ		*/
					/* タスクスタックポインタを保存	*/
	mov.l	sp, @(TCB_sp, e0)
	mov.l	#ret_int_r, er1  	/* 実行再開番地			*/
	mov.l	er1, @(TCB_pc, er0)
	bra	dispatcher_1

/*
 *  割込みの出口でディスパッチャからここに戻ってくる
 */

ret_int_r:
	jsr	@_calltex		/* タスク例外処理ルーチン起動	*/
	pop.l	er6			/* レジスタを復帰		*/
	pop.l	er5
	pop.l	er4
	pop.l	er3
	pop.l	er2
	pop.l	er1
	pop.l	er0
	rte

/*
 *  no_reg_exception()
 *  CPU例外として登録されていない例外が発生すると呼び出される。
 *  例外が発生した時点のPC,ER0〜7を出力してカーネル
 *  を停止する。
 */
	.globl _no_reg_exception
_no_reg_exception:

	push.l	er6		/*  ER0〜6を保存				*/
	push.l	er5
	push.l	er4
	push.l	er3
	push.l	er2
	push.l	er1
	push.l	er0

	mov.l	sp, er0		/*  引数設定				*/
				/*  SP は -32 で、発生時点の値		*/
	jsr	@_cpu_experr	/*  cpu_experr()の呼び出し		*/

/*
 *  タスク起動処理
 */

	.globl _activate_r
_activate_r:

	mov.l	@_runtsk, er2
	mov.l	@(TCB_tinib, er2), er2
	mov.l	@(TINIB_task, er2), er1	/*  タスク起動番地		*/
					/*  拡張情報（タスクへの引数）	*/
	mov.l	@(TINIB_exinf, er2), er0
	andc	#H8INT_ENA_ALL, ccr	/*  割込み許可			*/
	jmp	@er1

/*
 *  ハードウェア割り込み許可
 */

/*
 * ITU -- 何もしない。
 */

	.globl _timer_handler_enable_int
_timer_handler_enable_int:
	rts

/*
 * SCI -- 割込み優先レジスタの優先度を上げる。
 */

	.globl _serial_in_handler_enable_int
	.globl _serial_out_handler_enable_int
_serial_in_handler_enable_int:
_serial_out_handler_enable_int:
	mov.b	@SYSTEM_SCI_IPR, r0l
	bset	#SYSTEM_SCI_IP_BIT, r0l
	mov.b	r0l, @SYSTEM_SCI_IPR
	rts

/*
 *  ハードウェア割り込み禁止
 */

/*
 *  ITU -- Compare Match フラグをクリアーする。
 */

	.globl _timer_handler_disable_int
_timer_handler_disable_int:
	mov.l	#(SYSTEM_ITU + H8TSR), er0
	bclr	#SYSTEM_ITU_IF_BIT, @er0
	rts

/*
 * SCI -- 割込み優先レジスタの優先度を下げる。
 */

	.globl _serial_in_handler_disable_int
	.globl _serial_out_handler_disable_int
_serial_in_handler_disable_int:
_serial_out_handler_disable_int:
	mov.b	@SYSTEM_SCI_IPR, r0l
	bclr	#SYSTEM_SCI_IP_BIT, r0l
	mov.b	r0l, @SYSTEM_SCI_IPR
	rts
