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
 *  @(#) $Id: sys_support.S,v 1.4 2001/11/09 12:08:41 honda Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "evaluator7t.h"
        

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．          
 */       
#define SYSCFG 0x03ff0000

        
        
        
#define INTOFFSET   0x03ff0000+0x4024
#define INTPND      0x03ff0000+0x4004
#define INTMASK     0x03ff0000+0x4008        
#define LEDDATA     0x03ff0000+0x5008        


        .text
        .align 2
        .global hardware_init_hook
hardware_init_hook: 
        ldr  r2, syscfg_k /* キャッシュON */
        ldr  r1, [r2,#0]
        bic  r1, r1,#0x32
        orr  r1, r1,#0x12
        str  r1, [r2,#0]
        mov  pc, lr
        .align 2
syscfg_k:       
        .long SYSCFG


/*
 *
 *  割込みの出入り口処理
 *         
 */        
        .text
        .align 4	
        .global IRQ_Handler
IRQ_Handler:
        ldr   sp,   .int_stack
        sub   lr,lr,#4
        /*
         *    一時的にint_stackに待避
         */
        stmfd sp!, {r0 - r3,lr}

        /*
         *  割り込み要因の判定，システムモードになると読めない
         *  (KS32C50100依存)
         */
        ldr   r0,  intoffset_k 
        ldr   r1,  [r0]

        
       /* 
        * システムモード スタックを入れ替えるため割り込みは禁止
        * コンテキストをタスクスタックに積み直す．
        */
        mrs   r0, spsr
        mov   r3, sp
        mov   r2,#(CPSR_SYS|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
        msr   cpsr,r2
        ldr   r2,[r3,#0x10]     /* load  PC         */
        stmfd sp!,{r2}          /* Store PC         */
        ldr   r2,[r3,#0x0C]     /* load  r3         */                
        stmfd sp!,{r2,ip,lr}    /* Store r3,ip,lr   */
        ldmfd r3!,{r2,ip,lr}	/* load  r0,r1,r2   */
        stmfd sp!,{r0,r2,ip,lr} /* Store SPSR,r0,r1,r2 */


        /*
         *  多重割り込みか判定
         */
        ldr  r2, .interrupt_count
        ldr  r3, [r2]
        add  r0,r3,#1
        str  r0, [r2]
        cmp  r3, #0x00

        moveq   r2,sp            /* ネスト割り込みでない場合 */
        ldreq   sp,stack         /* スタックの変更    */
        stmeqfd sp!,{r2}         /* タスクスタックの保存 */
        
        ldr   r2,  int_table_k /* 割込み疑似テーブルの読み出し  */
        ldr   r0,  [r2,r1]            
        mov   r2,r1,lsr #2
        mov   r1,#1
        mov   r3,r1,lsl r2
        ldr   r1,intpnd_k      /* ペンディングビットのクリア */
        str   r3,[r1] 
        stmfd sp!,{r3}         /* マスクビットを保存 */
        ldr   r2,.int_mask     /* 同じ割り込みのネストを防ぐため */
        ldr   r1,[r2]          /* マスクする                     */
        orr   r1,r1,r3
        str   r1,[r2]      

        
        /* 割り込み許可 */                
        mov   r2,#CPSR_SYS     
        msr   cpsr,r2


        /* Call Handler  */        
        mov   lr,  pc   
        mov   pc,  r0
        
        
        mov   r2,#(CPSR_SYS|CPSR_FIQ_BIT|CPSR_IRQ_BIT) /* 割り込み禁止 */
        msr   cpsr,r2
        

        ldmfd sp!,{r3}         /* ペンディングビットの復帰 */
        ldr   r2,.int_mask     /* 割り込み許可             */
        ldr   r1,[r2]
        bic   r1,r1,r3
        str   r1,[r2]
                

        /* interrupt_count デクリメント */
        ldr   r2,.interrupt_count
        ldr   r1, [r2]        
        sub   r3,r1,#1                        
        str   r3, [r2]
        cmp   r3,#0x00        
        bne   return_to_task_irq

        
        /* タスクスタックの復帰 */
        ldmfd sp!,{r0}         
        mov   sp, r0        

                
        ldr   r1, reqflg_k    /* Check reqflg */
        ldr   r0,[r1]
        cmp   r0,#0
        beq   return_to_task_irq
        mov   r0,#0
        str   r0,[r1]         /* Clear reqflg */
        b     ret_int

return_to_task_irq:
        /*
         *  復帰処理
         *  割り込み許可となるがタスクコンテキスト上に保存しているため，
         *  問題はない
         */
        ldmfd sp!,{r1}           /* CPSRの復帰処理 */
        msr   cpsr, r1           /* 割り込み許可   */
        ldmfd sp!,{r0-r3,ip,lr,pc}
        
        .align 4
.int_stack:
        .long int_stack + 6 * 4        
intoffset_k:    
        .long     INTOFFSET
int_table_k:
        .long     int_table            
reqflg_k:
        .long     reqflg
intpnd_k:       
        .long INTPND
stack:
        .long   STACKTOP
.interrupt_count:       
        .long   interrupt_count
.int_mask:              
        .long   INTMASK
        