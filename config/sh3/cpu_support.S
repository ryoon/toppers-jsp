/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: cpu_support.S,v 1.13 2001/02/27 07:10:47 honda Exp $
 */


#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include "sys_config.h"
#include "cpu_config.h"
			
			
#define TRA         0xFFFFFFD0          /* TRAP interrupt reg. */
#define EXPEVT      0xFFFFFFD4          /* TRAP interrupt reg. */
#define INTEVT      0xFFFFFFD8          /* TRAP interrupt reg. */
#define INTEVT2     0xA4000000          /* TRAP interrupt reg. */
		
/*
 *  例外/割り込み出入り口処理ルーチン
 *  SH3はベクタテーブルを持たず、例外,TRAPAが発生するとVBR+0x100番地を、
 *  内部/外部/NMI割り込みが発生すると、VBR+0x600番地を実行する。
 */
/*	.little */
	.section .text
	.align 2
/*
 *	VBR に設定する値
 */	
	.global _BASE_VBR
_BASE_VBR:   
	nop 
	nop
	nop

/*
 *  Exception 100 vector
 *  アドレスエラー,スロット不当命令例外、Trapa例外等が発生すると実行される。
 *
 *  CPU例外ハンドラは、非タスクコンテキストで実行する、そのため、CPU例外ハン
 *  ドラを呼び出す前に例外/割り込みを示すレジスタバンク0の r7 レジスタをイン
 *  クメントし、リターンしてきた後にデクリメントする。CPU例外がタスクコンテキ
 *  ストで発生し、regflg が TRUE になった時に、 ret_exc へ分岐する。
 *  regflg をチェックする前に割り込みを禁止しないと、reqflg をチェック後に起
 *  動された割り込みハンドラ内でディスパッチが要求された場合に、ディスパッチ
 *  されない。
 *
 *		
 *  例外が発生すると、その例外が発生した番地をスキップするため、spcを2インク
 *  リメントする。次に例外発生元のコンテキストであるspc,pr,ssr,r0〜r7を退避
 *  する。例外が発生した時点でのコンテキストを調べ非タスクコンテキストなら
 *  _exception_from_intから実行を開始する。例外の要因はEXPEVTレジスタ読み、
 *  exc_tableからCPU例外ハンドラの開始番地を読み込む。発生した例外に対して
 *  CPU例外ハンドラが登録されていなければ、_no_reg_exceptionにブランチして、
 *  ssr,sr,spc,prの内容を出力して停止する。CPU例外ハンドラからリターンして
 *  きたら、reqflgを調べ0以外ならrec_exc()に飛び、0なら元の処理にリターンす
 *  る。
 *				 		
 */	
			

	.org	0x0100            /*  _BASE_VBR + 0x0400番地に配置  */
	.align 2
	.global _general_exception
_general_exception:
	stc    spc,r0             /* spcを2インクリメント            */
	add    #0x02,r0       
	mov.l  r0,@-r15           /* spc,pr,ssr をスタックに保存する */
	sts.l  pr,@-r15               
	stc.l  ssr,@-r15   
	stc.l  r0_bank,@-r15
	stc.l  r1_bank,@-r15
	stc.l  r2_bank,@-r15
	stc.l  r3_bank,@-r15
	stc.l  r4_bank,@-r15
	stc.l  r5_bank,@-r15
	stc.l  r6_bank,@-r15    
	stc.l  r7_bank,@-r15
	tst    r7,r7	         /* 例外発生元のコンテキストを判定            */
	bf/s   _exception_from_int /* 例外/割り込みハンドラならジャンプ       */
	add    #0x01,r7	         /* 例外/割り込みネスト回数をインクリメント   */
	mov    r15,r1            /* 戻り先が割り込みハンドラでないのでスタック*/
	mov.l  _stacktop_exc,r15 /* を入れ替え元のスタックポインタを保存      */  
	mov.l  r1,@-r15            
	mov.l  _expevt,r0        /* 割り込み許可すると消えるレジスタを保存    */
	mov.l  @r0,r5            /* EXPEVTレジスタを取得		              */
	ldc    r1,r4_bank        /* CPU例外ハンドラへの引数                   */
	ldc    r5,r5_bank	     
	mov.l  _unmak_rb_bl_exc,r2 /* BLビットを0に，レジスタバンクを切り替え */
	stc    sr,r3               
	and    r3,r2			
	ldc    r2,sr               
	shlr2  r5                /* EXPECTを3ビット右シフト	              */
	shlr   r5                
	mov.l  _exc_table_k,r0   /* exc_tableのアドレスをr0に                 */
	mov.l  @(r0,r5),r2       /* ハンドラのアドレスを取得                  */
	tst    r2,r2             /* ハンドラが登録されていない例外なら        */
	bt     _no_reg_exception /* no_reg_exceptionへジャンプ                */
	jsr    @r2               /* CPU例外ハンドラへ                         */
	nop
	mov.l  _mask_md_ipm_exc,r0
	ldc    r0,sr	         /* 割り込み禁止                              */
	stc    r7_bank,r7        /* 例外/割り込みのネスト回数をデクリメント   */
	dt     r7
	ldc    r7,r7_bank
	mov.l  _reqflg_exc,r2    /* reqflgのチェック                          */
	mov.l  @r2,r1
	tst    r1,r1             /* reqflgがFALSEならret_totask_excに飛び     */
	bt/s   _ret_to_task_exc  /* タスクに戻る                              */
	mov.l  @r15,r15          /* 戻り先がタスクなのでスタックを戻す        */
	xor    r0,r0	         /* reqflgをクリア                            */
	bra    ret_exc
	mov.l  r0,@r2              	


/*
 *   exception_from_int
 *   CPU例外発生時のコンテキストが非タスクコンテキストの場合の処理
 *   タスクコンテキストの場合との処理の違いは、とCPU例外ハンドラの
 *   処理が終了すると必ず元の処理に戻ることである。
 */
	
_exception_from_int:
	mov.l   _expevt,r0       /* 割り込み許可すると消えるレジスタを保存    */
	mov.l   @r0,r5           /* EXPECTレジスタを取得                      */
	ldc     r15,r4_bank      /* CPU例外ハンドラへの引数                   */
	ldc     r5,r5_bank	     
	mov.l   _unmak_rb_bl_exc,r2 /* BLビットを0に，レジスタバンクを切り替え */
	stc     sr,r3
	and     r3,r2			
	ldc     r2,sr               
	shlr2   r5               /* EXPECTを3ビット右シフト                   */
	shlr    r5                	
	mov.l   _exc_table_k,r0  /* exc_tableのアドレスをr0に                 */
	mov.l   @(r0,r5),r2      /* ハンドラのアドレスを取得                  */
	tst     r2,r2            /* ハンドラが登録されていない例外なら        */
	bt      _no_reg_exception   /* no_reg_exceptionへジャンプ             */
	jsr     @r2              /* CPU例外ハンドラへ                         */
	nop
	mov.l  _mask_md_ipm_exc,r0
	ldc    r0,sr	         /* 割り込み禁止                              */
	stc    r7_bank,r7        /* 例外/割り込みのネスト回数をデクリメント   */
	dt     r7
	ldc    r7,r7_bank
_ret_to_task_exc:		
	mov.l  @r15+,r7          /* r0〜r1,ssr,pr,spcを復帰しタスクに戻る     */
	mov.l  @r15+,r6
	mov.l  @r15+,r5
	mov.l  @r15+,r4
	mov.l  @r15+,r3
	mov.l  @r15+,r2
	mov.l  @r15+,r1
	mov.l  @r15+,r0
	ldc.l  @r15+,ssr            
	lds.l  @r15+,pr
	ldc.l  @r15+,spc
	rte
	nop


/*	
 *  no_reg_exception()
 *  CPU例外として登録されていない例外が発生すると呼び出される
 *  例外コード、例外が発生した時点のpc,sr,prを出力してカーネル
 *  を停止する。
 */					
_no_reg_exception:
	mov.l  _expevt,r5
	mov.l  @r5,r4
	stc    spc,r5
	stc    ssr,r6
	sts    pr,r7
	mov.l  _cpu_expevt_k,r1
	jsr    @r1
	nop

	.align  4
_stacktop_exc:	
	.long  STACKTOP            /* タスク独立部のスタックの初期値  */
_expevt:	
	.long  EXPEVT
_exc_table_k:	
	.long  _exc_table
_unmak_rb_bl_exc:		
	.long  0x4fffffff
_mask_md_ipm_exc:	
	.long  0x40000000 + MAX_IPM << 4	
_reqflg_exc:
	.long  _reqflg
_cpu_expevt_k:	
	.long  _cpu_expevt	




	
/*
 *  タスクディスパッチャ
 *
 *  _dispatch は、r7_bank0 = 0,割込み禁止状態で呼び出さなければならな
 *  い．_exit_and_dispatch も，r7_bank0 = 0・割込み禁止状態で呼び出す
 *  のが原則であるが，カーネル起動時に対応するため，r7_bank = 1で呼び
 *  出した場合にも対応している．
 */

	.text
	.align 2
	.global _dispatch
_dispatch:
	sts.l  pr,@-r15         /* pr,r8〜r15 をスタックに保存           */
	mov.l  r14,@-r15        /* r0〜r7は呼び出し元で保存しているため  */    
	mov.l  r13,@-r15        /* 保存する必要が無い                    */    
	mov.l  r12,@-r15
	mov.l  r11,@-r15
	mov.l  r10,@-r15
	mov.l  r9, @-r15
	mov.l  r8, @-r15
	mov.l  _runtsk_dis,r2   /* r0 <- runtsk                          */
	mov.l  @r2,r0
	mov.l  r15,@(TCB_sp,r0) /* タスクスタックをTCBに保存 60以下ならOK*/		
	mov.l  dispatch_r_k,r1  /* 実行再開番地を保存                    */
	mov.l  r1,@(TCB_pc,r0)  /* 実行再開番地をTCBに保存  60以下ならOK */
	bra    dispatcher_1  
    nop

dispatch_r:	
	mov.l  @r15+,r8         /* レジスタを復帰                       */
	mov.l  @r15+,r9
	mov.l  @r15+,r10
	mov.l  @r15+,r11
	mov.l  @r15+,r12
	mov.l  @r15+,r13
	mov.l  @r15+,r14
	lds.l  @r15+,pr
	mov.l  _calltex_dis,r1 /* タスク例外ルーチンの呼び出し          */
	jmp    @r1
	nop

	.global _exit_and_dispatch
_exit_and_dispatch:
	mov.l  _mask_md_ipm_dis,r9  /* 割り込み禁止                     */
	ldc    r9,sr               
	xor    r1,r1	            /* r7_bank0を0クリア                */
	ldc    r1,r7_bank
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
	mov.l  @(TCB_sp,r0),r15     /* TCBからタスクスタックを復帰      */
	mov.l  @(TCB_pc,r0),r1      /* TCBから実行再開番地を復帰        */
	jmp    @r1 
	nop
dispatcher_2:
	/*
	 *  ここで割込みモードに切り換えるのは，ここで発生する割込み処理
	 *  にどのスタックを使うかという問題の解決と，割込みハンドラ内で
	 *  のタスクディスパッチの防止という二つの意味がある．
	 */	
	mov.l  _stacktop_dis,r15    /* スタックを割り込みスタックに     */
	mov    #0x01,r10
	ldc    r10,r7_bank          /* r7_bank0 を1にして割り込み状態に	*/
	mov.l  _mask_md_dis,r9      /* 割り込み許可                     */
	ldc    r9,sr
	sleep    		    /* 割込み待ち                       */
	mov.l  _mask_md_ipm_dis,r8  /* 割り込み禁止                     */
	ldc    r8,sr	
	dt     r10                  /* r7_bank0 をクリア                */
	ldc    r10,r7_bank
	bra    dispatcher_1
	nop	
	.align 4
_runtsk_dis:	
	.long _runtsk
_schedtsk_dis:	
	.long _schedtsk	
_calltex_dis:	
	.long _calltex
_mask_md_ipm_dis:	
	.long 0x40000000 + MAX_IPM << 4
_mask_md_dis:	
	.long 0x40000000	
dispatch_r_k:	
	.long dispatch_r
_stacktop_dis:	
	.long STACKTOP            /* タスク独立部のスタックの初期値	*/


/* 
 *  割り込みハンドラ/CPU例外ハンドラ出口処理
 *
 * 戻り先がタスクでreqflgがセットされている場合のみここにくる。
 * r7_bank = 0,割り込み禁止状態,スクラッチレジスタを保存した
 * 状態で呼び出すこと。 
 *	
 */
	.text
	.align 2
	.globl ret_int
	.globl ret_exc
ret_exc:
ret_int:
	mov.l  _runtsk_ret,r1   /* r0 <- runtsk                    */
	mov.l  @r1,r0           
	mov.l  _enadsp_ret,r2   /* enadspのチェック                */
	mov.l  @r2,r3
	tst    r3,r3           
	bt     ret_int_1           
	mov.l  _schedtsk_ret,r4 /* r5 <- schedtsk                  */ 
	mov.l  @r4,r5
	cmp/eq r0,r5            /* runtsk と schedtsk を比較       */
	bt     ret_int_1
    mov.l  r14,@-r15	/* 残りのレジスタを保存            */
    mov.l  r13,@-r15					
    mov.l  r12,@-r15					
    mov.l  r11,@-r15					
    mov.l  r10,@-r15					
    mov.l  r9,@-r15					
    mov.l  r8,@-r15
	sts.l  mach,@-r15					
	sts.l  macl,@-r15					
	stc.l  gbr,@-r15					
	mov    #TCB_sp,r1      /* タスクスタックを保存            */
	mov.l  r15,@(r0,r1)    
	mov.l  ret_int_r_k,r1  /* 実行再開番地を保存              */
	mov    #TCB_pc,r2	
	mov.l  r1,@(r0,r2)  
	bra    dispatcher_1        
	nop	
ret_int_r:
	ldc.l  @r15+,gbr       /* レジスタを復帰                  */
	lds.l  @r15+,macl
	lds.l  @r15+,mach
	mov.l  @r15+,r8		
	mov.l  @r15+,r9		
	mov.l  @r15+,r10		
	mov.l  @r15+,r11		
	mov.l  @r15+,r12		
	mov.l  @r15+,r13		
	mov.l  @r15+,r14		
ret_int_1:
	mov.l  _calltex_ret,r2    /* タスク例外処理ルーチン起動      */
	jsr    @r2             
	nop
#ifdef SUPPORT_CHG_IPM
	mov    #32,r0
	mov.l  @(r0,r15),r1
	mov.l  _unmask_ipm,r2
	and    r2,r1
	mov.l  _task_intmask_k,r2
	mov.l  @r2,r3
	or     r3,r1
	mov.l  r1,@(r0,r15)
#endif /* SUPPORT_CHG_IMP */
	mov.l  @r15+,r7       /* spc,pr,ssr,スクラッチレジスタを復帰 */
	mov.l  @r15+,r6        
	mov.l  @r15+,r5
	mov.l  @r15+,r4
	mov.l  @r15+,r3
	mov.l  @r15+,r2
	mov.l  @r15+,r1
	mov.l  @r15+,r0
	ldc.l  @r15+,ssr    
	lds.l  @r15+,pr
	ldc.l  @r15+,spc
	rte
	nop
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

	
/*
 *  タスク起動処理
 */ 

	.text
	.align 2
	.globl _activate_r
_activate_r:
	stc   sr,r0               /* 割り込み許可 */
	mov.l _unmask_ipm,r1
	and   r1,r0	
#ifdef SUPPORT_CHG_IPM
        mov.l _task_intmask_k,r1
	mov.l @r1,r2
	or    r2,r0
#endif /* SUPPORT_CHG_IPM */
	ldc   r0,sr
	mov.l @r15+,r1
	lds.l @r15+,pr
	mov.l @r15+,r4
	jmp   @r1
	nop
	.align 4
_unmask_ipm:
	.long  0xffffff0f
#ifdef SUPPORT_CHG_IPM		
_task_intmask_k:
	.long _task_intmask
#endif /* SUPPORT_CHG_IPM */
		
		
/*
 *  Exception 600 vector
 *  割り込みが発生すると実行される。		
 *
 *
 *  SH3は割り込みが発生するとすべてVBR+0x600番地からプログラムを実行するため、
 *  ここに配置するルーチンでまずスタックの切り替え、レジスタの保存,IPMの設定,
 *  割り込み要因の判定を行いその後BLビットを0にして割り込みハンドラを呼ぶ必要
 *  がある。
 *  割り込み要因に対応した割り込みハンドラの開始番地は配列int_table[]に登録し
 *  て呼び出す。オフセットの計算は、割り込み要因レジスタを右に2ビットシフトし
 *  て行う。割り込み要因レジスタはSH7708ではINVENTレジスタにセットされるが、
 *  SH7709およびSH7709AではINVENT2にセットされるため、ifdefにより切り分けてい
 *  る。また、一部のディバイスについて割り込み要因レジスタで渡される値とIPMと
 *  の間には関係がないため、各割り込み要因に対応したIPMを持つSRの内容を配列
 *  int_plevel_table[]に登録してそれを読み出す。
 *
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 */

	.org	0x0600            /* _BASE_VBR + 0x0600番地に配置 */
	.align 2
	.global _interrupt
_interrupt:
	stc.l   spc,@-r15         /* 多重割り込みが入ると消えてしまうので */
	sts.l   pr,@-r15          /* spc,pr,ssr,r0〜r7をスタックに保存    */  
	stc.l   ssr,@-r15   
	stc.l   r0_bank,@-r15
	stc.l   r1_bank,@-r15
	stc.l   r2_bank,@-r15
	stc.l   r3_bank,@-r15
	stc.l   r4_bank,@-r15
	stc.l   r5_bank,@-r15
	stc.l   r6_bank,@-r15    
	stc.l   r7_bank,@-r15
	tst     r7,r7               /* 割り込み発生時のコンテキストを判定  */  
	bf/s    _interrupt_from_int /* 例外/割り込みハンドラならジャンプ   */
	add     #0x01,r7            /* 割り込みのネスト回数をインクリメント*/
	mov     r15,r1              /* スタックを入れ替え元のスタックポイ  */
	mov.l   _stacktop_k,r15	    /* ンタを保存                          */
	mov.l   r1,@-r15            
	mov.l   _intevt,r0          /* 例外要因レジスタを取得              */
	mov.l   @r0,r4              
	shlr2   r4                  /* 3ビット右シフトしオフセットを求める */
	shlr    r4                  
	mov.l   _int_plevel_table_k,r0 
	mov.l   @(r0,r4),r5         /* 割り込みの優先度を取得	           */
	mov.l   _int_table_k,r0    
	mov.l   @(r0,r4),r2         /* 割り込みハンドラのアドレスを取得    */
	ldc     r2,r2_bank	    /* バンク1に切り替えるためコピー       */	
	ldc     r5,sr               /* これ以降割り込みを受け付ける。      */
	jsr     @r2                 /* 割り込みハンドラへ                  */
	nop
	mov.l  _mask_md_ipm_ret,r0
	ldc    r0,sr	         /* 割り込み禁止                           */	
	stc    r7_bank,r7        /* 例外/割り込みのネスト回数をデクリメント*/
	dt     r7
	ldc    r7,r7_bank
	mov.l  _reqflg_k,r4      /* reqflgのチェック                       */
    mov.l  @r4,r1
	tst    r1,r1             /* reqflgがFALSEならret_to_task_intに飛ぶ */
	bt/s   _ret_to_task_int  /* タスクに戻る                           */
	mov.l  @r15,r15          /* 戻り先がタスクなのでスタックを戻す     */	
	xor    r0,r0
	mov.l  r0,@r4            /* reqflgをクリア	                   */
	bra    ret_int
	nop

_interrupt_from_int:
	mov.l   _intevt,r0             /* 例外要因INTEVT2レジスタを取得    */
	mov.l   @r0,r4
	shlr2   r4                     /* オフセットを求める               */
	shlr    r4                     /* オフセットを求める               */	
	mov.l   _int_plevel_table_k,r0 
	mov.l   @(r0,r4),r5            /* 割り込みの優先度を取得	   */
	mov.l   _int_table_k,r0        
	mov.l   @(r0,r4),r2            /* 割り込みハンドラのアドレスを取得 */
	ldc     r2,r2_bank             /* バンク1に切り替えるためコピー    */
	ldc     r5,sr                  /* これ以降割り込みを受け付ける。   */
	jsr     @r2                    /* 割り込みハンドラへ               */
	nop
	mov.l  _mask_md_ipm_ret,r0
	ldc    r0,sr	         /* 割り込み禁止                           */
	stc    r7_bank,r7        /* 例外/割り込みのネスト回数をデクリメント*/
	dt     r7
	ldc    r7,r7_bank
_ret_to_task_int:	         
	mov.l  @r15+,r7          /* r0〜r1,ssr,pr,spcを復帰し割り込み元に戻る */
	mov.l  @r15+,r6
	mov.l  @r15+,r5
	mov.l  @r15+,r4
	mov.l  @r15+,r3
	mov.l  @r15+,r2
	mov.l  @r15+,r1
	mov.l  @r15+,r0
	ldc.l  @r15+,ssr
	lds.l  @r15+,pr
	ldc.l  @r15+,spc
	rte
	nop
	.align  4
_stacktop_k:	
	.long  STACKTOP            /* タスク独立部のスタックの初期値  */
_intevt:		
#ifdef SH7708	
	.long  INTEVT
#else
	.long  INTEVT2
#endif	
_int_table_k:	
	.long    _int_table       
_int_plevel_table_k:	
	.long	_int_plevel_table 
_reqflg_k:
	.long  _reqflg
_mask_md_ipm_ret:	
	.long  0x40000000 + MAX_IPM << 4



	
