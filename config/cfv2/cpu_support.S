/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *  @(#) $Id: cpu_support.S,v 1.16 2005/11/12 14:58:46 hiro Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（MCF52235用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

/*
 *  タスクディスパッチャ
 *
 *  dispatch は，マスタモード・割込み禁止状態で呼び出さなければならな
 *  い．exit_and_dispatch も，マスタモード・割込み禁止状態で呼び出すの
 *  が原則であるが，カーネル起動時に対応するため，割込みモードで呼び出
 *  した場合にも対応している．
 */
	.text
	.globl dispatch
	.globl exit_and_dispatch
dispatch:
	lea  -44(%sp), %sp
	movem.l %d2-%d7/%a2-%a6, (%sp)	/* レジスタを保存 */
	move.l runtsk, %a0		/* A0 を runtsk に */
	move.l %sp, TCB_msp(%a0)	/* タスクスタックを保存 */
	move.l #dispatch_r, %d2
	move.l %d2, TCB_pc(%a0)	/* 実行再開番地を保存 */
	jbra dispatcher

dispatch_r:
	movem.l (%sp), %d2-%d7/%a2-%a6	/* レジスタを復帰 */
	lea 44(%sp), %sp
	btst.b #TCB_enatex_bit, TCB_enatex(%a0)
	jbeq dispatch_r_1		/* enatex が FALSE ならリターン */
	tst.l TCB_texptn(%a0)		/* texptn が 0 でなければ           */
	jbne call_texrtn		/*   タスク例外処理ルーチンの呼出し */
dispatch_r_1:
	rts

exit_and_dispatch:
	clr.l	intnest

dispatcher:
	/*
	 *  ここではマスタモード・割込み禁止状態でなければならない．
	 */
	move.l schedtsk, %a0
	move.l %a0, runtsk		/* schedtsk を runtsk に */
	jbeq   dispatcher_1		/* runtsk があるか？ */
	move.l TCB_msp(%a0), %sp	/* タスクスタックを復帰 */
	move.l TCB_pc(%a0),  %a1	/* 実行再開番地を復帰 */
	jmp (%a1)
dispatcher_1:
	addq.l  #1, intnest
	move.l  #STACKTOP, %sp
	stop #0x2000			/* 割込み待ち（割込みモード） */
	/*
	 *  ここで割込みモードに切り換えるのは，割込みハンドラ内で
	 *  のタスクディスパッチの防止という2つの意味がある．
	 *
	 *  この stop命令は，IPM を 0 にするが，本来は task_intmask に
	 *  設定すべきである．M68040 では，stop 命令のパラメータに定数
	 *  しかとれないため，やむをえず 0 にしている（stop 命令を 8つ
	 *  並べて，task_intmask の値で飛び分ける手はあるが，そこまで
	 *  やる意義はないと考えた）．
	 *
	 *  プロセッサを待ちモードに移行させる処理と，割込み許可とは，
	 *  不可分に行なう必要がある（M68040 では stop命令で両方行なう
	 *  ので問題ない）．これを不可分に行なわない場合，割込みを許可
	 *  した直後に割込みが入り，その中でタスクが実行可能状態になる
	 *  と，実行すべきタスクがあるにもかかわらずプロセッサが待ちモー
	 *  ドになってしまう．
	 *
	 *  割込みを待つ間は，runtsk を NULL（=0）に設定しなければなら
	 *  ない．このように設定しないと，割込みハンドラから iget_tid 
	 *  を呼び出した際の動作が仕様に合致しなくなる．
	 */
	move.w #0x2700, %sr		/* 割込み禁止 */
	clr.l intnest
	tst.l reqflg			/* reqflg が FALSE なら */
	jbeq dispatcher_1		/*      dispatcher_1 へ */
	clr.l reqflg			/* reqflg を FALSE に */
	jbra dispatcher

/*
 *  no_reg_exception()
 *  CPU例外として登録されていない例外が発生すると呼び出される
 *  例外が発生した時点のpc,sr,pr,r0〜15を出力してカーネル
 *  を停止する。
 */
	.text
	.align 2
	.globl no_reg_exception
no_reg_exception:
	move.l  %a7, %sp@-
	move.l  %a6, %sp@-
	move.l  %a5, %sp@-
	move.l  %a4, %sp@-
	move.l  %a3, %sp@-
	move.l  %a2, %sp@-
	move.l  %a1, %sp@-
	move.l  %a0, %sp@-
	move.l  %d7, %sp@-
	move.l  %d6, %sp@-
	move.l  %d5, %sp@-
	move.l  %d4, %sp@-
	move.l  %d3, %sp@-
	move.l  %d2, %sp@-
	move.l  %d1, %sp@-
	move.l  %d0, %sp@-
	move.l %sp, %sp@-
	jbsr  cpu_experr

/*
 *  タスク起動時処理
 */
	.text
	.globl activate_r
activate_r:
	/*
	 *  タスク起動直後はタスク例外処理が禁止されているため，ここでタ
	 *  スク例外処理ルーチンを呼び出す条件は成り立たない．
	 */
#ifdef SUPPORT_CHG_IPM			/* t_unlock_cpu 相当の処理 */
	move.w %sr, %d0			/* 割込みマスクを task_intmask に */
	and.l  #~0x00000700, %d0
	clr.l	%d1
	move.w task_intmask, %d1
	or.l   %d1, %d0
	move.w %d0, %sr
#else /* SUPPORT_CHG_IPM */
	move.w %sr, %d0			
	and.l #~0x00000700, %d0		/* 割込み許可 */
	move.w %d0, %sr
#endif /* SUPPORT_CHG_IPM */
	move.l (%sp)+, %a0		/* タスクの起動番地を a0 に */
	jmp (%a0)

/**
 *
 *  CPU例外の入口処理の続き
 *
 *  CPU例外ハンドラは，非タスクコンテキストで実行する．そのため，CPU例
 *  外ハンドラを呼び出す前に割込みモードに移行し，リターンしてきた後に
 *  元のモードに戻す．元のモードに戻すために，割込みモードに移行する前
 *  の SR を割込みスタック上に保存する．CPU例外がタスクコンテキストで
 *  発生し，reqflg が TRUE になった時に，ret_exc へ分岐する．
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 *  　
 *  一般不当命令の場合は戻り番地を2バイト進める必要があるが
 *  対応していない
 *  （GDB stubがブレークポイントとして使用する。）
 *  　
 * 　　CPU例外要因毎に展開されるルーチンでr0,r1を保存し、
 * 　　割込み禁止にした後、
 *　　　　d1：割込み受付直後のSRのコピー
 *　　　　a0：C言語ルーチンの先頭アドレス
 * 　　の状態でここに来る。
 * 　　
 * 　　レジスタ割当
 * 　　　r4：C言語ルーチンの引数
 * 　　　　　スタック上に積まれたSRのアドレス
 * 　　　r7：タスクスタックポインタ
 */
	.text
	.align 2
	.globl cpu_exception_entry
cpu_exception_entry:
	/*  割込み／CPU例外ネストカウンタのチェック 	*/
	tst.l	intnest         /* CPU例外発生時のコンテキストを判定  	*/
	bne	_exc_from_int   /* 多重例外ならジャンプ   		*/
	addq.l  #1, intnest

        /* 	初段のCPU例外の場合					*/
				/* スタック入れ替え前の			*/
				/* タスクスタックポインタを保存        	*/
	move.l  %sp, %a1
				/* 割込みスタックに切り替え		*/
	move.l  #STACKTOP, %sp
	move.l	%a1,-(%sp) 	/*       　  			*/
	move.w 	%d1, %sr	/*  割込み許可                          */
				/*    タスクスタックポインタを		*/
				/*    割込みスタックに積む  		*/
	jsr	(%a0)		/* C言語ルーチン呼び出し		*/
									
        move.l  #(MAX_IPM << 8), %d0 /*  割込み禁止		*/
        ori.l   #0x00003000, %d0
	move.w	%d0, %sr
				/* 割込み/CPU例外ネストカウンタをクリア */
	clr.l	intnest
	move.l	(%sp),%sp	/* スタック切替え  			*/
				/* reqflgのチェック                    	*/
	tst.l	reqflg
	jbne	ret_exc
	movem.l (%sp), %d0-%d1/%a0-%a1
	adda.l  #16,%sp
	rte

/*
 *  割込みの口処理の続き
 *
 * 　　割込み要因毎に展開されるルーチンでr0,r1を保存し、
 * 　　割込み禁止にした後、
 *　　　　d1：割込み受付直後のSRのコピー
 *　　　　a0:C言語ルーチンの先頭アドレス
 * 　　の状態でここに来る。
 * 　　
 * 　　レジスタ割当
 * 　　　r7：タスクスタックポインタ
 */
	.text
	.align 2
	.globl interrupt_entry
interrupt_entry:
	/*  割込み／CPU例外ネストカウンタのチェック 	*/
	tst.l	intnest		/* 多重割込みならジャンプ   		*/
	bne	_interrupt_from_int
	addq.l	#1, intnest	/* 割込みネストカウンタをインクリメント */

                                /* 初段の割込みの場合		*/
				/* スタック入れ替え前の			*/
				/* タスクスタックポインタを保存        	*/
	move.l  %sp, %a1	/* 割込みスタックに切り替え		*/
	move.l  #STACKTOP, %sp 
	move.l	%a1,-(%sp) 	/*    タスクスタックポインタを		*/
	move.w  %d1, %sr	/*  割込み許可  */
				/*    割込みスタックに積む  		*/
	jsr	(%a0)		/* C言語ルーチン呼び出し		*/
				/*  割込み禁止		*/
        move.l  #(MAX_IPM << 8), %d0
        ori.l   #0x00002000, %d0
	move.w	%d0, %sr	/*  割込み禁止			*/
				/* 割込み／CPU例外ネストカウンタをクリア*/
	clr.l	intnest
	move.l	(%sp),%sp	/* スタック切替え  			*/
				/* reqflgのチェック                    	*/
	tst.l	reqflg
	jbne	ret_int
	movem.l (%sp), %d0-%d1/%a0-%a1
	adda.l  #16,%sp
	rte

/* 多重割込みの場合 							*/
/* 	割込み発生時のコンテキストを判別後、 				*/
/*   		割込み禁止 						*/
/*   		a0：C言語ルーチンの先頭アドレス				*/
/*   		d0：割り込みネスト回数			                */
/*　　　　      d1：割込み受付直後のSRのコピー                          */
/* 	の状態でここに飛んでくる 					*/
_exc_from_int:
_interrupt_from_int:
	addq.l	#1, intnest		/* 割込みネストカウンタをインクリメント */
	move.w	%d1,%sr			/*  割込み許可	*/
	
	jsr	(%a0)			/*  C言語ルーチン呼び出し 	*/
	
        move.l  #(MAX_IPM << 8), %d0
        ori.l   #0x00002000, %d0
	move.w	%d0, %sr	/*  割込み禁止			*/
	/* 割込み／CPU例外ネストカウンタをディクリメント	*/
	subq.l	#1, intnest
	movem.l (%sp), %d0-%d1/%a0-%a1
	adda.l  #16,%sp
	rte

/*
 *  割込みハンドラ／CPU例外ハンドラ出口処理
 *
 *  ret_int は割込み禁止状態で，ret_exc はマスタモード・
 *  割込み禁止状態で呼び出さなければならない．また ret_exc は，スクラッ
 *  チレジスタを保存した状態で呼び出すこと．
 */
	.text
	.align 2
ret_int:
ret_exc:
	clr.l reqflg			/* reqflg を FALSE に */
	move.l runtsk, %a0		/* A0 ← runtsk */
	tst.l enadsp			/* enadsp が FALSE なら */
	jbeq  ret_int_1			/*         ret_int_1 へ */
	cmp.l schedtsk, %a0		/* runtsk と schedtsk が同じなら */
	jbeq ret_int_1			/*                  ret_int_1 へ */
	lea	-44(%sp), %sp
	movem.l %d2-%d7/%a2-%a6, (%sp)	/* 残りのレジスタを保存 */
	move.l %sp, TCB_msp(%a0)	/* タスクスタックを保存 */
	move.l #ret_int_r, %d2
	move.l %d2, TCB_pc(%a0)	/* 実行再開番地を保存 */
	jbra dispatcher
ret_int_r:
	movem.l (%sp), %d2-%d7/%a2-%a6	/* レジスタを復帰 */
	lea  44(%sp), %sp
ret_int_1:
	btst.b #TCB_enatex_bit, TCB_enatex(%a0)
	jbeq ret_int_2			/* enatex が FALSE ならリターン */
	tst.l TCB_texptn(%a0)		/* texptn が 0 ならリターン */
	jbeq ret_int_2
	jsr call_texrtn			/* タスク例外処理ルーチンの呼出し */
ret_int_2:
#ifdef SUPPORT_CHG_IPM
	move.l  16(%sp),  %d0	               /* 戻り先の割込みマスクを */
	and.l   #~0x00000700, %d0		/*        task_intmask に */
	clr.l	%d1
	move.w	task_intmask, %d1
	or.l	%d1, %d0
	move.l  %d0, 16(%sp)
#endif /* SUPPORT_CHG_IPM */
	movem.l (%sp), %d0-%d1/%a0-%a1	/* スクラッチレジスタを復帰 */
	adda.l  #16, %sp
	rte

/*
 *  微少時間待ち
 */
	.globl _sil_dly_nse
_sil_dly_nse:
	subi.l #SIL_DLY_TIM1, %d0	/* D0 から SIL_DLY_TIM1 を引く */
	jbhi _sil_dly_nse_1		/* 結果が 0 以下ならリターン */
	rts
_sil_dly_nse_1:
	subi.l #SIL_DLY_TIM2, %d0	/* D0 から SIL_DLY_TIM2 を引く */
	jbhi _sil_dly_nse_1		/* 結果が 0 より大きければループ */
	rts
