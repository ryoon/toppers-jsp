/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: cpu_support.S,v 1.27 2007/01/05 01:02:31 honda Exp $
 */

/*
 *    プロセッサ依存モジュール アセンブリ言語部（ARMv4用）
 */
#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include <armv4.h>
#include <t_config.h>
        
/*
 *  タスクディスパッチャ
 *
 *  dispatch は，スーパーバイザーモード・割込み禁止状態で呼び出さなければ
 *  ならない．
 *  _exit_and_dispatch も，スーパーバイザーモード・割込み禁止状態で呼び出す
 *  のが原則であるが，カーネル起動時に対応するため，IRQモードで呼び出した
 *  場合にも対応している．
 */

        .text
        .align 4    
        .globl dispatch
        .globl exit_and_dispatch
dispatch:
        stmfd sp!, {r4 - r11,lr}   /* レジスタの保存 */
        ldr   r0, =runtsk          /* runtskを読み込む */
        ldr   r1, [r0]
        str   sp, [r1,#TCB_sp]      /* タスクスタックを保存 */
        adr   r2, dispatch_r
        str   r2, [r1,#TCB_pc]      /* 実行再開番地を保存 */
        ldr   r6, =interrupt_count  /* r6 <-interrupt_count */
        mov   r5, #(CPSR_SVC|CPSR_IRQ_BIT)  /* 割り込み禁止(スーパーバイザーモード) */
        mov   r4, #(CPSR_SVC)               /* 割り込み許可(スーパーバイザーモード) */
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
        ldr   r6, =interrupt_count /* interrupt_countを0クリア */
        mov   r3, #0        
        str   r3, [r6]                                        
        /*
         *  FIQは常に禁止する．
         */
        mov   r5, #(CPSR_SVC|CPSR_IRQ_BIT) /* 割り込み禁止(スーパーバイザーモード) */
        mov   r4, #(CPSR_SVC)              /* 割り込み許可(スーパーバイザーモード) */
        mrs   r0, cpsr                     /* FIQを継承            */
        and   r0, r0, #CPSR_FIQ_BIT
        orr   r0, r0, r5
        msr   cpsr, r0          /* スーパーバイザーモード */
dispatcher_1:
        /*
         *  ここではスーパーバイザーモード・割込み禁止状態でなければならない．
         */
        ldr   r0, =schedtsk   /* schedtsk を読み込む */
        ldr   r1, [r0]
        ldr   r2, =runtsk     /* schedtsk を runtskに */
        str   r1, [r2]        /* schedtsk がいない場合はruntskをNULLに */
        cmp   r1, #0
        beq   dispatcher_2        
dispatcher_3:           
        ldr   sp, [r1,#TCB_sp] /* タスクスタックを復帰 */
        ldr   pc, [r1,#TCB_pc] /* 実行再開番地を復帰   */
dispatcher_2:
        mov   r3,#1
        str   r3, [r6]         /* interupt_count = 1 */
        ldr   sp, =STACKTOP
        /* sleepモードを持つCPUなら書き換える */
        mrs   r0, cpsr        /* FIQを継承           */
        and   r0, r0, #CPSR_FIQ_BIT
        orr   r0, r0, r4
        msr   cpsr, r0        /* 割込み待ち          */
        WAIT_INTERRUPT
        mrs   r0, cpsr        /* FIQを継承           */
        and   r0, r0, #CPSR_FIQ_BIT
        orr   r0, r0, r5
        msr   cpsr, r0        /* 割込み禁止          */        
        mov   r3,#0
        str   r3, [r6]        /* interrupt_count = 0 */   
        b     dispatcher_1


                
/*
 *  タスク起動時処理
 */
        .text
        .globl activate_r
activate_r:
        mov   r1,#(CPSR_SVC|CPSR_FIQ_BIT) /* 割り込み許可(スーパーバイザーモード) */
        mrs   r2, cpsr         /* FIQを継承   */
        and   r2, r2, #CPSR_FIQ_BIT
        orr   r1, r1, r2            
        msr   cpsr, r1         /* 割込み許可 */
        ldr   lr, =ext_tsk     /* 戻り番地設定 */
        ldmfd sp!, {r0,pc}     /* 引数,PC設定  */


        
/*
 *  割込みハンドラ／CPU例外ハンドラ出口処理
 *
 *  ret_int はスーパーバイザーモード・IRQ割込み禁止状態で呼び出す．
 */
        .text
        .globl ret_int
        .globl ret_exc 
ret_int:
ret_exc:                
        /*
         *   スーパーバイザーモードで来ること
         */
        ldr   r2, =runtsk       /* runtsk を読み込む */
        ldr   r1, [r2]
        ldr   r2, =enadsp
        ldr   r0, [r2]
        cmp   r0, #0
        beq   ret_int_1
        ldr   r2, =schedtsk
        ldr   r0, [r2]
        cmp   r0, r1            /* schedtsk と runtsk を比較 */
        beq   ret_int_1
        stmfd sp!, {r4-r11}     /* 残りのレジスタを保存 */
        str   sp, [r1,#TCB_sp]  /* タスクスタックを保存 */
        adr   r0, ret_int_r     /* 実行再開番地を保存   */
        str   r0, [r1,#TCB_pc]
        b     dispatcher_1
ret_int_r:
        ldmfd sp!, {r4-r11}     /* レジスタの復帰 */
ret_int_1:
        /*
         * タスク例外処理ルーチンの起動
         * dispatch_r は dispatcher_1 から呼び出されるため，
         * tcbのアドレスはr1に入っている
         * ret_int_1 は ret_exe から呼び出される
         */
        ldrb  r0, [r1,#TCB_enatex]
        tst   r0, #TCB_enatex_mask
        beq   ret_int_2              /* enatex が FALSE ならリターン */
        ldr   r0, [r1,#TCB_texptn]   /* texptnをロード               */
        tst   r0, r0                 /* texptn が0で無ければ         */
        blne  call_texrtn            /* タスク例外ルーチンの呼び出し */        
ret_int_2:              
        ldmfd sp!, {r0}       /* spsr を復帰 */
        mrs   r2, cpsr        /* FIQを継承            */
        and   r2, r2, #CPSR_FIQ_BIT
        and   r0, r0, #~CPSR_FIQ_BIT
        orr   r0, r0, r2              
        msr     spsr, r0              /* 戻り先のcpsrをspsrに設定 */
        ldmfd   sp!,{r0-r3,ip,lr,pc}^ /* タスクに復帰 ^付きなので、cpsr <- spsr */
        
        /*
         *  微少時間待ち
         */
	.globl sil_dly_nse
sil_dly_nse:
        sub   r0, r0, #SIL_DLY_TIM1
        cmp   r0, #0
        bgt   _sil_dly_nse1
        movle pc, lr
_sil_dly_nse1:
        sub   r0, r0, #SIL_DLY_TIM2
        cmp   r0, #0
        bgt   _sil_dly_nse1
        movle pc, lr
