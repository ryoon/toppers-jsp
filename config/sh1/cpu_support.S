/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: cpu_support.S,v 1.8 2003/12/18 06:34:40 honda Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（SH1用）
 *　　　　　カーネル内部で使用する定義
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

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


#if TCB_enatex > 15
 ここでアセンブルエラー
#endif

#if TCB_enatex_mask > 0x80
 ここでアセンブルエラー
#endif

#if !CHECK_IMMEDIATE_ADDRESSING(TCB_texptn)
 ここでアセンブルエラー
#endif

/*
 *  タスクディスパッチャ
 *
 *    　dispatch は、割込み／CPU例外ネストカウンタ = 0,割込み禁止状態
 *    で呼び出さなければならない．exit_and_dispatch も，割込みネスト
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
	bra     dispatcher
	mov.l	r1,@(TCB_pc, r0)	/*  遅延スロット   		*/
#else	/*  TCB_SHORT  */
	mov    #TCB_sp,r9
	mov.l  r15,@(r0,r9)     /* タスクスタックを保存                 */
	mov    #TCB_pc,r8
	mov.l  dispatch_r_k,r1  /* 実行再開番地を保存                   */
	bra    dispatcher
	mov.l  r1,@(r0,r8)	/*  遅延スロット  			*/
#endif	/* TCB_SHORT */

	/*
	 * dispatch_r:
	 * 　タスクコントコンテキストでディスパッチャ呼び出しをした場合の
	 * 　実行再開番地
	 * 　dispatcherから呼ばれるので、r7にruntskが代入されている
	 * 
	 * 　　レジスタ割り当て
	 * 　　　r7 ：runtsk
	 * 　　　r0 ：runtsk->enatex
	 * 　　　     mov.b @(imm, r),r0命令はオペランドがr0に
	 * 　　　     固定されている
	 * 　　　r1 ：runtsk->texptn
	 * 　　　r2 ：call_texrtn()の先頭アドレス
	 * 
	 */
dispatch_r:
	lds.l  @r15+,pr			/* レジスタを復帰               */
	mov.l  @r15+,r14
	mov.l  @r15+,r13
	mov.l  @r15+,r12
	mov.l  @r15+,r11
	mov.l  @r15+,r10
	mov.l  @r15+,r9
	mov.l  @r15+,r8
	mov.b  @(TCB_enatex,r7),r0	/*  オペランドがr0に固定 (c)  */
	mov.l  @(TCB_texptn,r7),r1
	and    #TCB_enatex_mask,r0
	tst    r0,r0			/*  runtsk->enatexのチェック 	*/
	bt     _dispatch_r_1
	tst    r1,r1			/*  runtsk->texptnのチェック  	*/
	bt     _dispatch_r_1
	mov.l  _call_texrtn_dis,r2 /* タスク例外処理ルーチン起動	*/
	jmp    @r2		/*（dispatch()呼び出し元にリターンする）*/
	nop			/*  遅延スロット　			*/
_dispatch_r_1:
	rts			/*  dispatch()呼び出し元へのリターン  	*/
	nop			/*  遅延スロット　			*/


	.global _exit_and_dispatch
_exit_and_dispatch:
				/*  割込みネストカウンタをクリア  	*/
	mov.l  _intnest_dis, r1
	mov    #0,r0
	mov.l  r0,@r1

/*
 *  ディスパッチャ本体
 *  
 */
dispatcher:
	/*
	 * ここには割り込み禁止で来ること
	 */
	mov.l  _schedtsk_dis,r1
	mov.l  _runtsk_dis,r2
	mov.l  @r1,r7		/* r7 <- schedtsk  */
	mov    #0, r0		/* (b) 　　　　　　*/

	/*
	 * ここでruntskにschedtskを代入するのは２つの意味がある。
	 * 　(1) schedtsk != NULLの場合
	 * 　　　　通常のタスク切り替えを行う。
	 * 　(2) schedtsk == NULLの場合
	 * 　　　　runtskにNULLを代入しておく。
	 * 　　　　（dispatcher_1以降の割込み待ちで割込みが入り、その中で
	 * 　　　　　iget_tid()がコールされたときに正しくTSK_NONEを返すため
	 * 　　　　　には、実行状態のタスクがない時に、runtskをNULLにして
	 * 　　　　　おる必要がある。）
	 */
	mov.l  r7,@r2               /* runtsk←schedtsk　               */

	cmp/eq r7,r0                /* schedtsk があるか？ (a) 末尾参照 */
	bt     dispatcher_1         /* 無ければジャンプ                 */
	/*  遅延スロットなし  */

	/*
	 * タスクコントロールブロックTCBから実行再開番地を取り出して
	 * 分岐する。
	 * 分岐先は以下の3つ
	 * 　dispatch_r：タスクコンテキストからのディスパッチ呼び出し
	 * 　ret_int_r ：割込みの出口処理
	 * 　activate_r：起動直後
	 * 
	 * 　分岐先ではr7にruntsk(=schedtsk)がロードされているとして
	 * 　使用して良い
	 */
#ifdef TCB_SHORT
	/*
	 * タスクコントロールブロックTCB中のsp,pcが先頭から60バイトの
	 * 範囲内にあり、アラインメントが4バイト境界になっていれば、
	 * イミディエイト相対アドレッシングでアクセスできる
	 */
					/* 実行再開番地を復帰           */
	mov.l	@(TCB_pc, r7),r1
	jmp     @r1
	mov.l	@(TCB_sp, r7),r15      /*  遅延スロット		*/
					/* タスクスタックポインタを復元 */
#else	/*  TCB_SHORT  */
	mov    #TCB_pc,r8
	mov.l  @(r7,r8),r1          	/* 実行再開番地を復帰		*/
	mov    #TCB_sp,r9
	jmp    @r1
	mov.l  @(r7,r9),r15         	/* 遅延スロット			*/
					/* タスクスタックポインタを復元 */
#endif	/* TCB_SHORT */


dispatcher_1:
	/*
	 *  ここで割込みモードに切り換えるのは，ここで発生する割込み処理
	 *  にどのスタックを使うかという問題の解決と，割込みハンドラ内で
	 *  のタスクディスパッチの防止という二つの意味がある．
	 */


	/* イミディエイト値を毎回読み込むのは効率が悪いので事前に用意する
	 * 	r0  : 0x00
	 * 	r1  : 0x01
	 * 	r8  : 割り込み禁止中のSRの値   
	 * 	r9  : 割り込み許可中のSRの値
	 * 	r10 : reqflgの番地
	 * 	r11 : 割込みネストカウンタintnestの番地       
	 */
	mov    #0,r0
	mov    #1,r1
	mov.l  _mask_ipm_dis,r8

#ifdef SUPPORT_CHG_IPM
	mov.l  _task_intmask_dis,r2  /*  タスクコンテキストのIPM  */
	mov.l  @r2,r9
#else /* SUPPORT_CHG_IPM */
	mov    #0,r9
#endif /* SUPPORT_CHG_IPM */

	mov.l  _reqflg_dis,r10
	mov.l  _intnest_dis,r11
	    			
	mov.l  _stacktop_dis,r15  /* スタックを割込みスタックに切替え */
	mov.l  r1,@r11		  /* 割込みネストカウンタを１にする   */
dispatcher_2:
	ldc    r9,sr		  /* 割り込み許可                     */

	/*
	 *　既知の不具合
	 *　
	 *　割込み許可後にsleep命令を実行しているため、割込み許可命令
	 *　の実行前に割込み要求が入っている場合（あるいは割込み許可直
	 *　後、sleep命令実行前に割込み要求が入った場合）、割込み許可
	 *　命令の実行と共に割込みが受け付けられ、その復帰後にsleepし
	 *　たままになってしまう。
	 *　（reqflgのチェックに進まない。）
	 *　この問題は，割込みの許可とsleep 状態への移行がアトミックに
	 *　実行できないことに起因する。
	 *　SH3以降ではIPMとは別にSRのBLビットを使って割込みの禁止/許
	 *　可を制御することにより、この問題を回避できるが、SH1/2では
	 *　うまい解決方法が見つかっていない。
	 *　（SH1ではIPMを設定する以外に割込みを禁止／許可する方法がな
	 *　く、割込み許可（割込み待ちのIPM設定）とsleepをアトミックに
	 *　行う方法がない。）
	 *
	 *　1msec以内にタイマ割込みが入るため、実際にはsleepしたままと
	 *　いうことはないが、この1msecも要求仕様に合わない場合は
	 *　OMIT_POWER_CONTROLマクロを定義することでsleep命令をスキッ
	 *　プできる。
	 */
#ifndef OMIT_POWER_CONTROL
	sleep    		    /* 割込み待ち                       */
#else	/*  OMIT_POWER_CONTROL  */
	nop
#endif	/*  OMIT_POWER_CONTROL  */
	ldc    r8,sr		    /* 割り込み禁止                     */
	mov.l  @r10,r2		    /* reqflgのチェック   		*/
	tst    r2,r2		    /* FALSEならば、もう1度割り込み待ち  */
	bt     dispatcher_2
	/*  遅延スロットなし  */
	mov.l  r0,@r10		    /* reqflgのクリア   		*/
	bra    dispatcher
	mov.l  r0,@r11		    /*  遅延スロット  */
				    /*  割込みネストカウンタをクリア 	*/

	/*
	 *　割込み待ちの直前に行ったスタック切替の戻しを行っていないが、
	 *　ディスパッチャの出口でTCBから取り出した値をスタックポイン
	 *　タに設定するので問題ない。
	 */


	.align 4
_runtsk_dis:
	.long _runtsk
_schedtsk_dis:
	.long _schedtsk
_call_texrtn_dis:
	.long _call_texrtn
_mask_ipm_dis:
	.long MAX_IPM << 4	/* 割込み禁止時のSRの値  		*/
dispatch_r_k:
	.long dispatch_r
_stacktop_dis:
	.long STACKTOP		/* タスク独立部のスタックの初期値	*/

_intnest_dis:	  		/* 割込み／CPU例外ネストカウンタ  	*/
	.long _intnest
_task_intmask_dis:		/* タスクコンテキストの割込みマスク   	*/
	.long _task_intmask
_reqflg_dis:
	.long _reqflg
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
	.globl _ret_int
_ret_int:
	mov.l  _runtsk_ret,r1   /* r7 <- runtsk                    */
	mov.l  @r1,r7
	mov.l  _enadsp_ret,r2   /* enadspのチェック                */
	mov.l  @r2,r3
	tst    r3,r3		/* ディスパッチ禁止ならret_int_1へ */
	bt     ret_int_1
	/*  遅延スロットなし  */

	mov.l  _schedtsk_ret,r4 /* r5 <- schedtsk                  */
	mov.l  @r4,r5
	cmp/eq r7,r5            /* runtsk と schedtsk を比較       */
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
	mov.l  r15,@(TCB_sp, r7)
	mov.l  ret_int_r_k,r1  	/* 実行再開番地             	  */
	bra    dispatcher
	mov.l  r1,@(TCB_pc, r7)	/*  遅延スロット　*/

#else	/*  TCB_SHORT  */
	mov    #TCB_sp,r1      	/* タスクスタックを保存           */
	mov.l  r15,@(r7,r1)
	mov.l  ret_int_r_k,r1  	/* 実行再開番地を保存             */
	mov    #TCB_pc,r2	/*  ディスパッチャからの戻り先を  */
				/*  ret_int_rに設定  		　*/
	bra    dispatcher
	mov.l  r1,@(r7,r2)	/*  遅延スロット 		  */
#endif	/* TCB_SHORT */

/*
 *  割込みの出口でディスパッチャからここに戻ってくる
 *    r7にruntskが代入された状態でここに来る
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

	/*
	 * ディスパッチャからret_int_rに分岐した場合、
	 * ret_intから直接ここに分岐した場合、
	 * いずれの場合も、r7にruntskが代入されている
	 *
	 *　 レジスタ割り当て
	 * 	r7  : runtsk
	 * 	r0  : runtsk->enatex
	 * 　　　     mov.b @(imm, r),r0命令はオペランドがr0に
	 * 　　　     固定されている
	 * 	r2  : runtsk->texptn
	 * 	r3  : call_texrtn()の先頭番地
	 */
ret_int_1:
	mov.b  @(TCB_enatex,r7),r0	/*  オペランドがr0に固定 (d) */
	and    #TCB_enatex_mask,r0
	tst    r0,r0			/*  runtsk->enatexのチェック  	*/
	bt     ret_int_2
	mov.l  @(TCB_texptn,r7),r2	/*  runtsk->texptnのチェック  	*/
	tst    r2,r2
	bt     ret_int_2
	mov.l  _call_texrtn_ret,r3 	/* タスク例外処理ルーチン起動	*/
	jsr    @r3
	nop				/*  遅延スロット		*/

ret_int_2:
#ifdef SUPPORT_CHG_IPM
	/*
	 *　SH1では割込みからのリターン命令でハードウェアがスタック上
	 *　のデータを読み出してSRを復元する。
	 *　割込みによって別のタスクに切り替わった場合は、タスクコンテ
	 *　キストのIPMが変更されている可能性があるのでスタック上のSR
	 *　のコピーに含まれるIPMを更新しておく必要がある。
	 */
	mov    #40,r0		/*  	スタックの先頭からsrまでの  	*/
				/*  	オフセット （下記参照）		*/
	mov.l  @(r0,r15),r1	/*  	r1←スタックに積まれたsr  	*/
	mov.l  _unmask_ipm,r2
	mov.l  _task_intmask_k,r3
	and    r2,r1
	mov.l  @r3,r4
	or     r4,r1
	mov.l  r1,@(r0,r15)	/*  スタックに積んであるsrに上書き  	*/
#endif /* SUPPORT_CHG_IPM */

				/* pr,スクラッチレジスタを復帰 		*/
				/*  スタックの先頭からのオフセット  	*/
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
_call_texrtn_ret:
	.long _call_texrtn
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
#endif /* SUPPORT_CHG_IPM */


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



/*  イミディエイト相対アドレッシングが可能かチェック  */
#if !(CHECK_IMMEDIATE_ADDRESSING(TCB_tinib) 			\
	&& CHECK_IMMEDIATE_ADDRESSING(TINIB_task)		\
	&& CHECK_IMMEDIATE_ADDRESSING(TINIB_exinf) )

ここでアセンブル・エラー
#endif


/*
 *  タスク起動処理
 *  　　dispatcherから呼ばれるので、r7にruntskが代入されている
 *  
 */
	.text
	.align 2
	.globl _activate_r
_activate_r:
				/* 割り込み許可の準備 		*/
#ifndef SUPPORT_CHG_IPM		/*  （t_unlock_cpu相当の処理） 	*/
	mov   #0,r1		/*  IPM以外のビットも破壊される */
#else  /* SUPPORT_CHG_IPM */
        mov.l _task_intmask_act,r2
	mov.l @r2,r1
#endif /* SUPPORT_CHG_IPM */
	ldc   r1,sr		/*  割込み許可  		*/
	mov.l _ext_tsk_act,r3
	lds   r3,pr		/*  タスクからの戻り番地を設定  */

	/*
	 *  ここで割込みが入ってruntskが書き換わっても、
	 *  このタスクに戻ってくるときにはruntskの値も
	 *  当然、元に戻っている
	 * 
	 *   レジスタ割り当て
	 * 　　r7：runtsk
	 * 　　r1：&(runtsk->tinib)
	 * 　　r2：runtsk->tinib->task
	 * 　　r4：タスクの拡張情報（引数）
	 */
	mov.l @(TCB_tinib,r7),r1
	mov.l @(TINIB_task,r1),r2	/*  タスク起動番地  	*/
	jmp   @r2		/* jsr命令ではprを破壊してしまう*/
	mov.l @(TINIB_exinf,r1),r4	/*  遅延スロット  	*/
				/*  拡張情報（タスクへの引数）  */
	.align 4
_ext_tsk_act:
	.long _ext_tsk

#ifdef SUPPORT_CHG_IPM
_task_intmask_act:
	.long _task_intmask
#endif /* SUPPORT_CHG_IPM */

/*
 *  微少時間待ち
 */
	.globl _sil_dly_nse
_sil_dly_nse:

    mov.l  _sil_dly_tim1, r1	/* r4から SIL_DLY_TIM1 を引く */
    add    r1, r4
    cmp/pl r4			/* 結果が 0 以下ならリターン */
    bt     _sil_dly_nse1
    rts
    nop
_sil_dly_nse1:            
    mov.l  _sil_dly_tim2, r1	/* r4から SIL_DLY_TIM2 を引く */
    add    r1, r4
    cmp/pl r4			/* 結果が 0 以下ならリターン */
    bt     _sil_dly_nse1
    rts
    nop   
    .align 4
_sil_dly_tim1:
    .long  -SIL_DLY_TIM1    
_sil_dly_tim2:                         
    .long  -SIL_DLY_TIM2


/*
 * 備忘録
 * 　　dispatcher内(a)行でcmp/eq imm,r0命令を使わない理由
 * 　　
 *　　　dispatcherでr7にruntskの値を代入し、分岐先のdispatch_r、
 *　　　ret_int_r、activate_rでは無駄なロードをしないようにしてい
 *　　　る。
 *　　　schedtskの有無をチェックするときのcmp/eq imm,r0命令（(a)
 *　　　行）がオペランドをr0に固定されているため、それに合わせて、
 *　　　最初はruntskにr0を割り当てていた。
 *　　　しかし、r0は各分岐先にあるenatexの読み出し(c),(d)行のmov.b
 *　　　命令でも予約しているため、できれば空けておきたい。
 *　　　（mov.b @(imm, r),r0命令はオペランドがr0に固定されている。）
 *　　　従って、runtskはr7に割り当て、(a)行はcmp/eq imm,r0命令から
 *　　　cmp/eq rn,rm命令に変更した。そのため、レジスタに0を代入する
 *　　　命令（(b)行）が１つ余分に入っている。
 *
 *　　　一方、mov.l @(imm, r),r0命令はオペランドがr0に固定されて
 *　　　いない。
 *　　　SH3版のようにmakeoffset.cでラベルBIT_BWを指定すれば、4バ
 *　　　イト単位でオフセットとマスクが生成されるので、mov.l命令が
 *　　　使え、レジスタ割付が楽になるが、enatex判定時のマスクデー
 *　　　タがand命令のイミディエート値に収まり切らなくなるため、
 *　　　ロード命令が１つ余分に入る。
 *　　　どちらも一長一短があるが、いずれにしろ、runtskを再ロード
 *　　　するよりはマシと思われる。
 */
