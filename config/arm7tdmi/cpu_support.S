/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: cpu_support.S,v 1.9 2002/04/05 07:43:51 honda Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（ARM7TDMI用）
 */
#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include "arm7tdmi.h"

        
/*
 *  タスクディスパッチャ
 *
 *  dispatchは，
 *  dispatch は，システムモード・割込み禁止状態で呼び出さなければならな
 *  い．_exit_and_dispatch も，システムモード・割込み禁止状態で呼び出す
 *  のが原則であるが，カーネル起動時に対応するため，IRQモードで呼び出した
 *	場合にも対応している．
 */

        .text
        .align 4	
        .globl dispatch
        .globl exit_and_dispatch

dispatch:
        stmfd sp!, {r4 - r11,lr}   /* レジスタの保存 */
        ldr   r0,runtsk_dis        /* runtskを読み込む */
        ldr   r1,[r0]
        str   sp,[r1,#TCB_sp]      /* タスクスタックを保存 */
        adr   r2,dispatch_r
        str   r2,[r1,#TCB_pc]      /* 実行再開番地を保存 */
        ldr   r6,interrupt_count_dis /* r6 <-interrupt_count */
        mov r5,#0x9f       /* 割り込み禁止(システムモード) */
        mov r4,#0x1f       /* 割り込み許可(システムモード) */                
        b     dispatcher_1

dispatch_r:
        ldmfd sp!,{r4 - r11,lr}
        /*
         * タスク例外処理ルーチンの起動
         * dispatch_r は dispatcher_1 から呼び出されるため，
         * tcbのアドレスはr1に入っている
         */
        ldrb  r0,[r1,#TCB_enatex]
        tst   r0,#TCB_enatex_mask
        beq   dispatch_r_1          /* enatex が FALSE ならリターン */
        ldr   r0,[r1,#TCB_texptn]   /* texptnをロード               */
        tst   r0,r0                 /* texptn が0で無ければ         */
        bne   call_texrtn           /* タスク例外ルーチンの呼び出し */
dispatch_r_1:   
        mov   pc,lr        



exit_and_dispatch:
        ldr   r6,interrupt_count_dis /* interrupt_countを0クリア */
        mov   r3,#0        
        str   r3, [r6]                                        
        mov r5,#0x9f       /* 割り込み禁止(システムモード) */
        mov r4,#0x1f       /* 割り込み許可(システムモード) */
		msr cpsr, r5	   /* システムモード */
dispatcher_1:
    	/*
         *  ここではシステムモード・割込み禁止状態でなければならない．
    	 */
        ldr r0, schedtsk_k /* schedtsk を読み込む */
        ldr r1, [r0]
        cmp r1, #0
        beq dispatcher_2
        ldr r2, runtsk_dis  /* schedtsk を runtskに */
        str r1, [r2]
dispatcher_3:           
        ldr sp, [r1,#TCB_sp] /* タスクスタックを復帰 */
        ldr pc, [r1,#TCB_pc] /* 実行再開番地を復帰   */
dispatcher_2:
        mov   r3,#1
        str   r3, [r6]
        /* sleepモードを持つCPUなら書き換える */
        msr cpsr, r4		/* 割込み待ち      */
        msr cpsr, r5		/* 割込み禁止      */
        mov   r3,#0
        str   r3, [r6]        
        b   dispatcher_1
runtsk_dis:
        .long runtsk
schedtsk_k:     
        .long schedtsk
interrupt_count_dis:       
        .long   interrupt_count

                
/*
 *  タスク起動時処理
 */
        .text
        .globl activate_r
activate_r:
        mov   r1,#0x1F         /* 割り込み許可(システムモード) */
        msr   cpsr, r1         /* 割込み許可 */
        ldr   lr, ext_tsk_k    /* 戻り番地設定 */
        ldmfd sp!, {r0,pc}     /* 引数,PC設定  */
ext_tsk_k:
        .long ext_tsk


		
/*
 *  割込みハンドラ／CPU例外ハンドラ出口処理
 *
 *  ret_int はIRQモード・IRQ割込み禁止状態で呼び出す．
 */
        .text
        .globl ret_int
        .globl ret_exc 
ret_int:
ret_exc:                
        /*
         *   システムモードで来ること
         */
		ldr   r2, runtsk_ret    /* runtsk を読み込む */
        ldr   r0,[r2]
        ldr   r2, enadsp_ret
        ldr   r1,[r2]
        cmp   r1,#0
        beq   ret_int_1
        ldr   r2,schedtsk_ret
        ldr   r1,[r2]
        cmp   r0,r1             /* runtsk と schedtskを比較 */
        beq   ret_int_1
        stmfd sp!, {r4-r11}     /* 残りのレジスタを保存 */
        str   sp,[r0,#TCB_sp]   /* タスクスタックを保存 */
        adr   r1, ret_int_r     /* 実行再開番地を保存   */
        str   r1,[r0,#TCB_pc]
        b     dispatcher_1
ret_int_r:
        ldmfd sp!, {r4-r11} /* レジスタの復帰 */
ret_int_1:
        /*
         * タスク例外処理ルーチンの起動
         * dispatch_r は dispatcher_1 から呼び出されるため，
         * tcbのアドレスはr1に入っている
         */
        ldrb  r0,[r1,#TCB_enatex]
        tst   r0,#TCB_enatex_mask
        beq   ret_int_2             /* enatex が FALSE ならリターン */
        ldr   r0,[r1,#TCB_texptn]   /* texptnをロード               */
        tst   r0,r0                 /* texptn が0で無ければ         */
        blne   call_texrtn          /* タスク例外ルーチンの呼び出し */        
ret_int_2:              
        ldmfd sp!, {r0}       /* spsr を復帰 */
        msr   cpsr, r0      
        ldmfd sp!, {r0 - r3,ip,lr,pc} /* タスクへ戻る */
        

runtsk_ret:
        .long runtsk
schedtsk_ret:   
        .long schedtsk
enadsp_ret:     
        .long enadsp                
