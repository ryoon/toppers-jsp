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
 *  @(#) $Id: cpu_support.S,v 1.1 2000/11/14 14:44:14 hiro Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（68040用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

/*
 *  タスクディスパッチャ
 *
 *  _dispatch は，マスタモード・割込み禁止状態で呼び出さなければならな
 *  い．_exit_and_dispatch も，マスタモード・割込み禁止状態で呼び出す
 *  のが原則であるが，カーネル起動時に対応するため，割込みモードで呼び
 *  出した場合にも対応している．
 */
	.text
	.globl dispatch
	.globl exit_and_dispatch
dispatch:
	movem.l %d2-%d7/%a2-%a6, -(%sp)	/* レジスタを保存 */
	move.l runtsk, %a0		/* A0 ← runtsk */
	move.l %sp, TCB_msp(%a0)	/* タスクスタックを保存 */
	move.l #dispatch_r, TCB_pc(%a0)	/* 実行再開番地を保存 */
	jbra dispatcher_1

dispatch_r:
	movem.l (%sp)+, %d2-%d7/%a2-%a6	/* レジスタを復帰 */
	jmp calltex			/* タスク例外処理ルーチンの呼出し */

exit_and_dispatch:
	or.w #0x1000, %sr		/* マスタモード */
dispatcher_1:
	/*
	 *  ここではマスタモード・割込み禁止状態でなければならない．
	 */
	move.l schedtsk, %a0
	move.l %a0, runtsk		/* schedtsk を runtsk に */
	jbeq dispatcher_2		/* runtsk があるか？ */
	move.l TCB_msp(%a0), %sp	/* タスクスタックを復帰 */
	move.l TCB_pc(%a0), %a1		/* 実行再開番地を復帰 */
	jmp (%a1)
dispatcher_2:
	stop #0x2000			/* 割込み待ち（割込みモード） */
	/*
	 *  ここで割込みモードに切り換えるのは，ここで発生する割込み処理
	 *  にどのスタックを使うかという問題の解決と，割込みハンドラ内で
	 *  のタスクディスパッチの防止という二つの意味がある．
	 */
	or.w #0x1700, %sr		/* マスタモード・割込み禁止 */
	jbra dispatcher_1
	
/*
 *  タスク起動時処理
 */
	.text
	.globl activate_r
activate_r:
#ifdef SUPPORT_CHG_IPM
	move.w %sr, %d0			/* 割込みマスクを task_intmask に */
	and.w #~0x0700, %d0
	or.w task_intmask, %d0
	move.w %d0, %sr
#else /* SUPPORT_CHG_IPM */
	and.w #~0x0700, %sr		/* 割込み許可 */
#endif /* SUPPORT_CHG_IPM */
	move.l (%sp)+, %a0		/* タスクの起動番地を a0 に */
	jmp (%a0)

/*
 *  割込みハンドラ／CPU例外ハンドラ出口処理
 *
 *  ret_int は割込みモード・割込み禁止状態で，ret_exc はマスタモード・
 *  割込み禁止状態で呼び出さなければならない．また ret_exc は，スクラッ
 *  チレジスタを保存した状態で呼び出すこと．
 */
	.text
	.globl ret_int
	.globl ret_exc
ret_int:
	addq.l #8, %sp			/* スローアウェイフレームを捨てる */
	or.w #0x1000, %sr		/* マスタモード */
	movem.l %d0-%d1/%a0-%a1, -(%sp)	/* スクラッチレジスタを保存 */
ret_exc:
	clr.l reqflg
	move.l runtsk, %a0		/* A0 ← runtsk */
	tst.l enadsp			/* enadsp のチェック */
	jbeq ret_int_1
	cmp.l schedtsk, %a0		/* runtsk と schedtsk を比較 */
	jbeq ret_int_1
	movem.l %d2-%d7/%a2-%a6, -(%sp)	/* 残りのレジスタを保存 */
	move.l %sp, TCB_msp(%a0)	/* タスクスタックを保存 */
	move.l #ret_int_r, TCB_pc(%a0)	/* 実行再開番地を保存 */
	jbra dispatcher_1

ret_int_r:
	movem.l (%sp)+, %d2-%d7/%a2-%a6	/* レジスタを復帰 */
ret_int_1:
	jsr calltex			/* タスク例外処理ルーチンの呼出し */
#ifdef SUPPORT_CHG_IPM
	move.w 16(%sp), %d0		/* 戻り先の割込みマスクを */
	and.w #~0x0700, %d0		/*        task_intmask に */
	or.w task_intmask, %d0
	move.w %d0, 16(%sp)
#endif /* SUPPORT_CHG_IPM */
	movem.l (%sp)+, %d0-%d1/%a0-%a1	/* スクラッチレジスタを復帰 */
	rte
