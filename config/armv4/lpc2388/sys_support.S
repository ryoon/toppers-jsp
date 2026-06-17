/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2004 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2006-2010 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: sys_support.S,v 1.2 2006/01/11 11:45:55 roi Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "cpu_asm.inc"
#include "offset.h"
#include <lpc23xx.h>

#define SCS_Val         0x00000020

/* Constants */
#define OSCRANGE        (1<<4)			/* Oscillator Range Select */
#define OSCEN           (1<<5)			/* Main oscillator Enable */
#define OSCSTAT         (1<<6)			/* Main Oscillator Status */
#define PLLCON_PLLE     (1<<0)			/* PLL Enable */
#define PLLCON_PLLC     (1<<1)			/* PLL Connect */
#define PLLSTAT_M       (0x7FFF<<0)		/* PLL M Value */
#define PLLSTAT_N       (0xFF<<16)		/* PLL N Value */
#define PLLSTAT_PLOCK   (1<<26)			/* PLL Lock Status */

#define CLKSRCSEL_Val   0x00000001
#define PLLCFG_Val      0x0000000B
#define CCLKCFG_Val     0x00000005
#define USBCLKCFG_Val   0x00000005
#define PCLKSEL0_Val    0x00000000
#define PCLKSEL1_Val    0x01000000

#define MAMTIM_Val      0x00000004
#define MAMCR_Val       0x00000002

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリ初期化の前に呼び出される．
 */

        .text
        .align 2
        .global hardware_init_hook
hardware_init_hook:
									/* Setup Clock */
        ldr     r0, =TADR_SCB_BASE
        mov     r1, #0xAA
        mov     r2, #0x55
									/* Configure and Enable PLL */
        ldr     r3, =SCS_Val			/* Enable main oscillator */
        str     r3, [r0, #TOFF_SCB_SCS]

osc_loop:
        ldr     r3, [r0, #TOFF_SCB_SCS]	/* Wait for main osc stabilize */
        ands    r3, r3, #OSCSTAT
        beq     osc_loop

        ldr     r3, =CLKSRCSEL_Val		/* Select PLL source clock */
        str     r3, [r0, #TOFF_SCB_CLKSRCSEL]
        ldr     r3, =PLLCFG_Val
        str     r3, [r0, #TOFF_SCB_PLLCFG]
        str     r1, [r0, #TOFF_SCB_PLLFEED]
        str     r2, [r0, #TOFF_SCB_PLLFEED]
        mov     r3, #PLLCON_PLLE
        str     r3, [r0, #TOFF_SCB_PLLCON]
        str     r1, [r0, #TOFF_SCB_PLLFEED]
        str     r2, [r0, #TOFF_SCB_PLLFEED]

									/* Wait until PLL Locked */
pll_loop:
        ldr     r3, [r0, #TOFF_SCB_PLLSTAT]
        ands    r3, r3, #PLLSTAT_PLOCK
        beq     pll_loop

m_n_lock:
        ldr     r3, [r0, #TOFF_SCB_PLLSTAT]
        ldr     r4, =(PLLSTAT_M | PLLSTAT_N)
        and     r3, r3, r4
        ldr     r4, =PLLCFG_Val
        eors    r3, r3, r4
        bne     m_n_lock

									/* Setup CPU clock divider */
        mov     r3, #CCLKCFG_Val
        str     r3, [r0, #TOFF_SCB_CCLKCFG]
									/* Setup USB clock divider */
        ldr     r3, =USBCLKCFG_Val
        str     r3, [R0, #TOFF_SCB_USBCLKCFG]
									/* Setup Peripheral Clock */
        ldr     r3, =PCLKSEL0_Val
        str     r3, [r0, #TOFF_SCB_PCLKSEL0]
        ldr     r3, =PCLKSEL1_Val
        str     r3, [r0, #TOFF_SCB_PCLKSEL1]
									/* Switch to PLL Clock */
        mov     r3, #(PLLCON_PLLE | PLLCON_PLLC)
        str     r3, [r0, #TOFF_SCB_PLLCON]
        str     r1, [r0, #TOFF_SCB_PLLFEED]
        str     r2, [r0, #TOFF_SCB_PLLFEED]

									/* Setup MAM */
        ldr     r0, =TADR_PINSEL_BASE
        mov     r3, #0
        str     r3, [r0, #TOFF_PINSEL10]
        ldr     r0, =TADR_SCB_BASE
        mov     r1, #MAMTIM_Val
        str     r1, [r0, #TOFF_SCB_MAMTIM]
        mov     r1, #MAMCR_Val
        str     r1, [r0, #TOFF_SCB_MAMCR]

									/* Setup LED */
        ldr     r0, =(TADR_FIO_BASE)
        ldr     r3, =0xffffffff
        str     r3, [r0, #TOFF_FIO2_DIR]
        ldr     r3, =0x000000ff
        str     r3, [r0, #TOFF_FIO2_SET]

									/* Setup the stack for each mode */
        mov     r0, sp
        msr     cpsr_c, #(CPSR_FIQ|CPSR_IRQ_BIT|CPSR_FIQ_BIT)	/* Set up Fast Interrupt Mode and set FIQ Mode Stack */
        ldr     r8, =TADR_VIC_BASE                      /* Init the FIQ register */
        mov     r13, r0

        msr     cpsr_c, #(CPSR_IRQ|CPSR_IRQ_BIT|CPSR_FIQ_BIT)	/* Set up Interrupt Mode and set IRQ Mode Stack */
        mov     r13, r0                                 /* Init stack IRQ */
        msr     cpsr_c, #(CPSR_SVC|CPSR_IRQ_BIT|CPSR_FIQ_BIT)   /* Return Supervisor Mode and set Supervisor Mode */

init_done:
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
        mov     sp,#(CPSR_SVC | CPSR_FIQ_BIT | CPSR_IRQ_BIT) 
        msr     cpsr_all, sp
        stmfd   sp!, {r0-r3,ip,lr,pc} /* pcはダミー */


        /*
         * spsrと戻り番地を取得するためにIRQモードへ
         */
        mov     r0,#(CPSR_IRQ | CPSR_FIQ_BIT | CPSR_IRQ_BIT)
        msr     cpsr,r0
        sub     r0,lr,#4
        mrs     r1,spsr

        /*
         *  スーパーバイザーモードに
         */
        and     r2,  r1, #CPSR_FIQ_BIT /* FIQビットの継承 */
        orr     r2,  r2, #(CPSR_SVC|CPSR_IRQ_BIT)
        msr     cpsr_all, r2
        str     r0, [sp,#0x18] /* Store pc */
        stmfd   sp!,{r1}       /* spsr */


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
        ldr     r3, =TADR_VIC_BASE
        ldr     r0, [r3, #TOFF_VIC_VECTADDR]
        cmp     r0, #0x00
        bne     IRQ_callhandler
        ldr     r0, =irq_exception

        /*
         * 割り込み許可
         */
IRQ_callhandler:
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

        /*
         * 割込みクリア
         */
        ldr     r3, =TADR_VIC_BASE
        ldr     r0, =0x00000000
        str     r0, [r3, #TOFF_VIC_VECTADDR]

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

        my_tpcb r1, r0			/* runtskを読み込む */
        ldr     r0, [r1, #TPCB_reqflg]	/* Check reqflg */
        cmp     r0, #0
        beq     return_to_task_irq
        mov     r0, #0
        str     r0, [r1, #TPCB_reqflg]	/* Clear reqflg */
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


        .text
        .align 4
        .global FIQ_Handler
FIQ_Handler:
         /* Switch in SVC/User Mode to allow User Stack access for C code */
         /* because the FIQ is not yet acknowledged */
         /* Save and r0 in FIQ_Register */
         mov    r9, r0
         ldr    r0, [r8, #TOFF_VIC_VECTADDR]
         msr    cpsr_c,#(CPSR_SVC|CPSR_IRQ_BIT|CPSR_FIQ_BIT)
         /* Save scratch/used registers and LR in User Stack */
         ldr    sp, =(STACKTOP+FIQ_DATA_SIZE)
         stmfd  sp!, { r1-r3, r12, lr}
         /* Branch to the routine pointed by the AIC_FVR */
         mov    r14, pc
         bx     r0
         /* Restore scratch/used registers and LR from User Stack */
         ldmia  sp!, { r1-r3, r12, lr}
         /* Leave Interrupts disabled and switch back in FIQ mode */
         msr    cpsr_c, #(CPSR_FIQ|CPSR_IRQ_BIT|CPSR_FIQ_BIT)
         /* Restore the R0 ARM_MODE_SVC register */
         mov    r0, r9
         /* Restore the Program Counter using the LR_fiq directly in the PC */
         subs   pc, lr, #4

