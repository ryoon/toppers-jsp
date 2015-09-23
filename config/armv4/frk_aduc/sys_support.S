/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2006 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2006 by Logic Research Co., Ltd.
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
 */


#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "frk_aduc.h"


        /*
         * Vectorテーブルとジャンプテーブル
         * サイズは合計0x3c
         */
        .section .data.vctsram,"a"
        .code 32
        .align 0
        .global vct_tb_sram
vct_tb_sram:
        ldr pc, reset_vector       /* リセット             */
        ldr pc, undef_vector       /* 未定義命令           */
        ldr pc, swi_vector         /* ソフトウェア割込み   */
        ldr pc, prefech_vector     /* プリフェッチアボード */
        ldr pc, data_abort_vector  /* データアボード       */
         nop
        ldr pc, irq_vector         /* IRQ                  */
        ldr pc, fiq_vector         /* IRQ                  */

/*
 *  ベクターテーブル
 */
reset_vector:
        .long   start
undef_vector:
        .long   undef_exception
swi_vector:
        .long   swi_exception
prefech_vector:
        .long   prefetch_exception
data_abort_vector:
        .long   data_abort_exception
irq_vector:
        .long   irq_exception
fiq_vector:
        .long   fiq_exception




/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．          
 */       

        .text
        .align 2
        .global hardware_init_hook
hardware_init_hook: 
        mov     pc, lr


        .text
        .align 2
        .global software_init_hook
software_init_hook:
        mov     pc, lr

        .text
        .align 2
        .global software_term_hook
software_term_hook:
        mov     pc, lr


/*
 *
 *  割込みの出入り口処理
 *         
 */        
        .text
        .align 4    
        .global IRQ_Handler
IRQ_Handler:

        /* 
         * 割込みモード 
         * 
         * cpsrがspsr_irqに復帰先がr14_irq(lp)に入り，
         * spsr_irqとr14_irqとr13(sp)_irqが r14,r13となる．
         */

        /* 
         *  タスクの動作時モード(スーパーバイザーモード)へ
         */
        mov   sp,#(CPSR_SVC | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 
        msr   cpsr_all, sp
        stmfd sp!, {r0-r3,ip,lr,pc} /* pcはダミー */

 
  
        /*
         * spsrと戻り番地を取得するためにIRQモードへ
         */
        mov   r0,#(CPSR_IRQ | CPSR_FIQ_BIT | CPSR_IRQ_BIT)
        msr   cpsr,r0
        sub   r0,lr,#4
        mrs   r1,spsr
   
        /*
         *  スーパーバイザーモードに
         */
        and   r2,  r1, #CPSR_FIQ_BIT /* FIQビットの継承 */
        orr   r2,  r2, #(CPSR_SVC|CPSR_IRQ_BIT)        
        msr   cpsr_all, r2
        str   r0, [sp,#0x18] /* Store pc */
        stmfd sp!,{r1}       /* spsr */


        /*
         *  多重割り込みか判定
         */
        ldr     r2, =interrupt_count
        ldr     r3, [r2]
        add     r0,r3,#1
        str     r0, [r2]
        cmp     r3, #0x00

        moveq   r2,sp            /* ネスト割り込みでない場合 */
        ldreq   sp,=STACKTOP     /* スタックの変更    */
        stmeqfd sp!,{r2}         /* タスクスタックの保存 */


        /*
         *  割り込み要因の判定．
         */
        ldr     r1, =IRQSTA

		ldr     r0, =IRQEN
		ldr     r2, [r0]
		stmfd   sp!, {r2}			/* マスクビットの退避 */

        ldr     r2, [r1]
        mov     r0, #1
		mov		r3, #1
find_bit_loop:
        mov     r1, r3, lsl r0
        tst     r2, r1
        bne     find_bit_end   
        add     r0, r0, #1
        bpl     find_bit_loop
find_bit_end:
        mov     r3, r0
        ldr     r0,=int_mask_table    /* 割込みマスクテーブルの呼び出し*/
        ldr     r0,[r0,r3,lsl #2]     /* r0<-割込みマスク              */    
        ldr     r1, =IRQCLR			  /* 割込みレジスタへのセット      */
        str     r0,[r1]

        ldr     r0,  =int_table       /* 割込み疑似テーブルの読み出し  */
        ldr     r0,  [r0,r3,lsl #2]   /* r0<-割込みハンドラ            */    


        /*
         *  未定義の割込みかチェック
         */
        cmp     r0, #0x00
        beq     undefined_interrupt
        
        /* 
         * 割り込み許可 
         */                
        mrs     r2, cpsr
        and     r2,  r2, #~CPSR_IRQ_BIT /* 割込み許可 */
        msr     cpsr,r2

        /* 
         * Call Handler  
         */        
        mov     lr,  pc   
        bx      r0

        /* 
         * 割り込み禁止 
         */
        mrs     r2, cpsr
        and     r2,  r2, #CPSR_FIQ_BIT /* FIQビットの継承 */         
        orr     r2,  r2, #(CPSR_SVC|CPSR_IRQ_BIT)
        msr     cpsr,r2

        ldmfd   sp!, {r0}                /* マスクビットの復帰 */
        ldr     r1,  =IRQEN        /*割り込み許可*/
        str     r0,  [r1]

        /* 
         * 割込みネスト回数(interrupt_count) デクリメント 
         */
        ldr     r2, =interrupt_count
        ldr     r1, [r2]        
        sub     r3, r1, #1                        
        str     r3, [r2]
        cmp     r3, #0x00        
        bne     return_to_task_irq
        
        /* 
         * タスクスタックの復帰 
         */
        ldmfd   sp!,{r0}         
        mov     sp, r0        
                
        ldr     r1, =reqflg    /* Check reqflg */
        ldr     r0, [r1]
        cmp     r0, #0
        beq     return_to_task_irq
        mov     r0, #0
        str     r0, [r1]       /* Clear reqflg */
        b       ret_int

return_to_task_irq:
        /*
         *  復帰処理
         *  割り込み許可となるがタスクコンテキスト上に保存しているため，
         *  問題はない
         */
        ldmfd   sp!,{r1}           /* CPSRの復帰処理 */
        mrs     r2, cpsr           /* FIQを継承            */
        and     r2, r2, #CPSR_FIQ_BIT
        and     r1, r1, #~CPSR_FIQ_BIT
        orr     r1, r1, r2                 
        msr     spsr, r1           /* 割り込み許可   */
        ldmfd   sp!,{r0-r3,ip,lr,pc}^ /*タスク復帰 + 割込み許可 */

        /*
         *  未定義の割込みが入ったときに呼び出す
         */    
undefined_interrupt:
         b      undef_interrupt
    

