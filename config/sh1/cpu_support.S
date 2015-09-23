/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: cpu_support.S,v 1.1 2001/05/04 09:07:50 imai Exp $
 */


#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include "sys_config.h"
#include "cpu_config.h"

/*
 * タスクコントロールブロックTCB中のsp,pcが先頭から60バイトの
 * 範囲内にあり、アラインメントが4バイト境界になっていれば、
 * イミディエイト相対アドレッシングでアクセスできる
 * TCB_pc,TCB_spはoffset.hで定義されている
 * (make時に生成される）
 */

/*  イミディエイト相対アドレッシングが可能かチェック  */
#define CHECK_IMMEDIATE_ADDRESSING(dst)			\
			((dst <= 60) && ((dst % 4) == 0))

#if CHECK_IMMEDIATE_ADDRESSING(TCB_pc) && 		\
	CHECK_IMMEDIATE_ADDRESSING(TCB_sp)
#define TCB_SHORT
#endif

/*
 * タスクコントロールブロックTCB中のsp,pcが先頭から128バイト
 * 以降に配置されていると
 * mov    #TCB_sp,r9
 * 等の符号拡張を含む命令は期待通りに動作しない
 */
#if (TCB_pc > 127) || (TCB_sp > 127)
 ここでアセンブルエラー
#endif


/*
 *  タスクディスパッチャ
 *
 *    　_dispatch は、割込み／CPU例外ネストカウンタ = 0,割込み禁止状態
 *    で呼び出さなければならない．_exit_and_dispatch も，割込みネスト
 *    カウンタ = 0・割込み禁止状態で呼び出すのが原則であるが，カーネル
 *    起動時に対応するため，割込みネストカウンタ = 1で呼び出した場合に
 *    も対応している．
 */

	.text
	.align 2
	.global _dispatch
_dispatch:
				/* pr,r8〜r15 をスタックに保存           */
	mov.l  r8, @-r15       	/* r0〜r7は呼び出し元で保存しているため  */
	mov.l  r9, @-r15        /* 保存する必要が無い                    */
	mov.l  r10,@-r15
	mov.l  r11,@-r15
	mov.l  r12,@-r15
	mov.l  r13,@-r15
	mov.l  r14,@-r15
	sts.l  pr,@-r15
	mov.l  _runtsk_dis,r2   /* r0 <- runtsk                          */
	mov.l  @r2,r0
	/*
	 * タスクコントロールブロックTCB中のsp,pcが先頭から60バイトの
	 * 範囲内にあり、アラインメントが4バイト境界になっていれば、
	 * イミディエイト相対アドレッシングでアクセスできる
	 */
#ifdef TCB_SHORT
				/*  タスクスタックポインタを保存 	*/
	mov.l	r15,@(TCB_sp, r0)
	mov.l   dispatch_r_k,r1  /* 実行再開番地	                */
	bra     dispatcher_1
	mov.l	r1,@(TCB_pc, r0)	/*  遅延スロット   		*/
#else	/*  TCB_SHORT  */
	mov    #TCB_sp,r9
	mov.l  r15,@(r0,r9)     /* タスクスタックを保存                 */
	mov    #TCB_pc,r8
	mov.l  dispatch_r_k,r1  /* 実行再開番地を保存                   */
	bra    dispatcher_1
	mov.l  r1,@(r0,r8)	/*  遅延スロット  			*/
#endif	/* TCB_SHORT */

dispatch_r:
	         		/* レジスタを復帰                       */
	lds.l  @r15+,pr
	mov.l  @r15+,r14
	mov.l  @r15+,r13
	mov.l  @r15+,r12
	mov.l  @r15+,r11
	mov.l  @r15+,r10
	mov.l  @r15+,r9
	mov.l  _calltex_dis,r1 /* タスク例外ルーチンの呼び出し          */
	jmp    @r1
	mov.l  @r15+,r8		/*  遅延スロット　			*/

	.global _exit_and_dispatch
_exit_and_dispatch:
	mov.l  _mask_ipm_dis,r9  /* 割り込み禁止                     	*/
	ldc    r9,sr
				/*  割込みネストカウンタをクリア  	*/
	mov.l  _intnest_dis, r1
	mov    #0,r0
	mov.l  r0,@r1

dispatcher_1:
	/*
	 * ここには割り込み禁止で来ること
	 */
	mov.l  _schedtsk_dis,r12    /* r0 <- schedtsk                   */
	mov.l  @r12,r0
	cmp/eq #0,r0                /* schedtsk があるか？              */
	bt     dispatcher_2         /* 無ければジャンプ                 */
	mov.l  _runtsk_dis,r2
	mov.l  r0,@r2               /* schedtskをruntskに               */

	/*
	 * タスクコントロールブロックTCB中のsp,pcが先頭から60バイトの
	 * 範囲内にあり、アラインメントが4バイト境界になっていれば、
	 * イミディエイト相対アドレッシングでアクセスできる
	 */
#ifdef TCB_SHORT
					/* 実行再開番地を復帰           */
	mov.l	@(TCB_pc, r0),r1
	jmp     @r1
	mov.l	@(TCB_sp, r0),r15 /*  遅延スロット		*/
					/* タスクスタックポインタを復元 */
#else	/*  TCB_SHORT  */
	mov    #TCB_pc,r8
	mov.l  @(r0,r8),r1          	/* 実行再開番地を復帰		*/
	mov    #TCB_sp,r9
	jmp    @r1
	mov.l  @(r0,r9),r15         	/* 遅延スロット			*/
					/* タスクスタックポインタを復元 */
#endif	/* TCB_SHORT */


dispatcher_2:
	/*
	 *  ここで割込みモードに切り換えるのは，ここで発生する割込み処理
	 *  にどのスタックを使うかという問題の解決と，割込みハンドラ内で
	 *  のタスクディスパッチの防止という二つの意味がある．
	 */
	    			/* スタックを割り込みスタックに切替え	*/
	mov.l  _stacktop_dis,r15
				    /*  割込みネストカウンタを１にする 	*/
	mov.l  _intnest_dis,r0
	mov    #1,r1
	mov.l  r1,@r0

	mov    #0,r9      	    /* 割り込み許可                     */
	ldc    r9,sr
	sleep    		    /* 割込み待ち                       */
	mov.l  _mask_ipm_dis,r8     /* 割り込み禁止                     */
	ldc    r8,sr
	bra    dispatcher_1
	mov.l  r9,@r0		    /*  遅延スロット  */
				    /*  割込みネストカウンタをクリア 	*/
	.align 4
_runtsk_dis:
	.long _runtsk
_schedtsk_dis:
	.long _schedtsk
_calltex_dis:
	.long _calltex
_mask_ipm_dis:
	.long MAX_IPM << 4	/* 割込み禁止時のSRの値  		*/
dispatch_r_k:
	.long dispatch_r
_stacktop_dis:
	.long STACKTOP		/* タスク独立部のスタックの初期値	*/

_intnest_dis:	  		/* 割込み／CPU例外ネストカウンタ  	*/
	.long _intnest


/*
 *  割込みハンドラ/CPU例外ハンドラ出口処理
 *
 * 戻り先がタスクでreqflgがセットされている場合のみここにくる。
 * 割込みネストカウンタ = 0,割込み禁止状態,スクラッチレジスタを
 * 保存した状態で呼び出すこと。
 *
 */
	.text
	.align 2
	.globl ret_int
ret_int:
	mov.l  _runtsk_ret,r1   /* r0 <- runtsk                    */
	mov.l  @r1,r0
	mov.l  _enadsp_ret,r2   /* enadspのチェック                */
	mov.l  @r2,r3
	tst    r3,r3		/* ディスパッチ禁止ならret_int_1へ */
	bt     ret_int_1
	/*  遅延スロットなし  */

	mov.l  _schedtsk_ret,r4 /* r5 <- schedtsk                  */
	mov.l  @r4,r5
	cmp/eq r0,r5            /* runtsk と schedtsk を比較       */
	bt     ret_int_1	/*  同じならret_int_1へ  	   */
	/*  遅延スロットなし  */

        mov.l  r8,@-r15        	/* 残りのレジスタを保存            */
        mov.l  r9,@-r15
        mov.l  r10,@-r15
        mov.l  r11,@-r15
        mov.l  r12,@-r15
        mov.l  r13,@-r15
        mov.l  r14,@-r15
	sts.l  mach,@-r15
	sts.l  macl,@-r15
	stc.l  gbr,@-r15
	/*
	 * タスクコントロールブロックTCB中のsp,pcが先頭から60バイトの
	 * 範囲内にあり、アラインメントが4バイト境界になっていれば、
	 * イミディエイト相対アドレッシングでアクセスできる
	 */
#ifdef TCB_SHORT
				/*  タスクスタックポインタを保存  */
	mov.l	r15,@(TCB_sp, r0)
	mov.l  ret_int_r_k,r1  	/* 実行再開番地             	  */
	bra    dispatcher_1
	mov.l	r1,@(TCB_pc, r0)	/*  遅延スロット　*/

#else	/*  TCB_SHORT  */
	mov    #TCB_sp,r1      	/* タスクスタックを保存           */
	mov.l  r15,@(r0,r1)
	mov.l  ret_int_r_k,r1  	/* 実行再開番地を保存             */
	mov    #TCB_pc,r2	/*  ディスパッチャからの戻り先を  */
				/*  ret_int_rに設定  		　*/
	bra    dispatcher_1
	mov.l  r1,@(r0,r2)	/*  遅延スロット 		  */
#endif	/* TCB_SHORT */

/*
 *  割込みの出口でディスパッチャからここに戻ってくる
 */
ret_int_r:
	ldc.l  @r15+,gbr       /* レジスタを復帰                  */
	lds.l  @r15+,macl
	lds.l  @r15+,mach
	mov.l  @r15+,r14
	mov.l  @r15+,r13
	mov.l  @r15+,r12
	mov.l  @r15+,r11
	mov.l  @r15+,r10
	mov.l  @r15+,r9
	mov.l  @r15+,r8
ret_int_1:
	mov.l  _calltex_ret,r2    /* タスク例外処理ルーチン起動      */
	jsr    @r2
	nop
#ifdef SUPPORT_CHG_IPM		/*  chg_ipm()をサポートする場合 */
	mov    #40,r0		/*  スタックの先頭からsrまでの  */
				/*  オフセット （下記参照）	*/
	mov.l  @(r0,r15),r1	/*  r1←スタックに積まれたsr  	*/
	mov.l  _unmask_ipm,r2
	and    r2,r1
	mov.l  _task_intmask_k,r2
	mov.l  @r2,r3
	or     r3,r1
	mov.l  r1,@(r0,r15)	/*  スタックに積んであるsrに上書き  */
#endif /* SUPPORT_CHG_IMP */
				/* pr,スクラッチレジスタを復帰 */
				/*  スタックの先頭からのオフセット  */
	lds.l  @r15+,pr		/*   +0:PR  */
	mov.l  @r15+,r7		/*   +4:r7  */
	mov.l  @r15+,r6		/*   +8:r6  */
	mov.l  @r15+,r5		/*  +12:r5  */
	mov.l  @r15+,r4		/*  +16:r4  */
	mov.l  @r15+,r3		/*  +20:r3  */
	mov.l  @r15+,r2		/*  +24:r2  */
	mov.l  @r15+,r1		/*  +28:r1  */
	mov.l  @r15+,r0		/*  +32:r0  */
				/*  +36:PC  */
				/*  +40:SR  */
	rte
	nop			/*  遅延スロット  */
	.align 4
_calltex_ret:
	.long _calltex
_runtsk_ret:
	.long _runtsk
_schedtsk_ret:
	.long _schedtsk
_enadsp_ret:
	.long _enadsp
ret_int_r_k:
	.long ret_int_r

#ifdef SUPPORT_CHG_IPM		/*  chg_ipm()をサポートする場合 */
_unmask_ipm:
	.long  ~0xf0		/*  ipmビット以外すべて１  */
_task_intmask_k:
	.long _task_intmask
#endif /* SUPPORT_CHG_IMP */


/*
 *  no_reg_exception()
 *  CPU例外として登録されていない例外が発生すると呼び出される
 *  例外が発生した時点のpc,sr,pr,r0〜15を出力してカーネル
 *  を停止する。
 */
	.text
	.align 2
	.globl _no_reg_exception
_no_reg_exception:
				/*  pr,r0〜15を保存  */
	sts.l  pr, @-r15
	mov.l  r15,@-r15
	mov.l  r14,@-r15
	mov.l  r13,@-r15
	mov.l  r12,@-r15
	mov.l  r11,@-r15
	mov.l  r10,@-r15
	mov.l  r9, @-r15
	mov.l  r8, @-r15
	mov.l  r7, @-r15
	mov.l  r6, @-r15
	mov.l  r5, @-r15
	mov.l  r4, @-r15
	mov.l  r3, @-r15
	mov.l  r2, @-r15
	mov.l  r1, @-r15
	mov.l  r0, @-r15

				/*  cpu_experr()の呼び出し  	*/
	mov.l  _cpu_experr_k,r1	/*  (cpu_config.c)  		*/
	jsr    @r1
	mov    r15, r4		/*  引数設定（遅延スロット）	*/

	.align  4
_cpu_experr_k:
	.long  _cpu_experr


/*
 *  タスク起動処理
 */

/*  イミディエイト相対アドレッシングが可能かチェック  */
#if !(CHECK_IMMEDIATE_ADDRESSING(TCB_tinib) 			\
	&& CHECK_IMMEDIATE_ADDRESSING(TINIB_task)		\
	&& CHECK_IMMEDIATE_ADDRESSING(TINIB_exinf) )

ここでアセンブル・エラー
#endif


	.text
	.align 2
	.globl _activate_r
_activate_r:
				/* 割り込み許可の準備 		*/
#ifndef SUPPORT_CHG_IPM
	mov   #0,r1		/*  IPM以外のビットも破壊される */
#else  /* SUPPORT_CHG_IPM */
        mov.l _task_intmask_act,r0
	mov.l @r0,r1
#endif /* SUPPORT_CHG_IPM */
	ldc   r1,sr		/*  割込み許可  		*/
	mov.l _ext_tsk_act,r0
	lds   r0,pr		/*  タスクからの戻り番地を設定  */
	mov.l _runtsk_act,r1
	mov.l @r1,r2
	mov.l @(TCB_tinib,r2),r3
	mov.l @(TINIB_task,r3),r0	/*  タスク起動番地  	*/
	jmp   @r0		/* jsr命令ではprを破壊してしまう*/
	mov.l @(TINIB_exinf,r3),r4	/*  遅延スロット  	*/
				/*  拡張情報（タスクへの引数）  */
	.align 4
_ext_tsk_act:
	.long _ext_tsk
_runtsk_act:
	.long _runtsk

#ifdef SUPPORT_CHG_IPM
_task_intmask_act:
	.long _task_intmask
#endif /* SUPPORT_CHG_IPM */
