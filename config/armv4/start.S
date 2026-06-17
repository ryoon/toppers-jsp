/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: start.S,v 1.19 2007/01/05 02:02:38 honda Exp $
 */
                
/*
 *    カーネル用のスタートアップモジュール(ARMv4用)
 */
#define _MACRO_ONLY
#include <armv4.h>
#include <sys_defs.h>
        
#include <t_config.h>
 


        /*
         * Vectorテーブルとジャンプテーブル
         * サイズは合計0x3c
         */
        .section .vector,"a"
        .code 32
        .align 0
        .global vector_table
#ifdef _MINDSTORMSNXT_
		.global _kernel_arm_vector_add
#endif  /* _MINDSTORMSNXT_ */
vector_table:
        ldr pc, reset_vector       /* リセット             */
        ldr pc, undef_vector       /* 未定義命令           */
        ldr pc, swi_vector         /* ソフトウェア割込み   */
        ldr pc, prefech_vector     /* プリフェッチアボード */
        ldr pc, data_abort_vector  /* データアボード       */
        ldr pc, reserved_vector
#ifdef _MINDSTORMSNXT_
        ldr   pc,[pc,#-0xF20]	@ IRQ : read the AIC
        ldr   pc,[pc,#-0xF24]	@ FIQ : read the AIC
#else  /* _MINDSTORMSNXT_ */
        ldr pc, irq_vector         /* IRQ                  */
        ldr pc, fiq_vector         /* IRQ                  */
#endif  /* _MINDSTORMSNXT_ */

#ifndef __thumb__
#define undef_handler			undef_exception
#define swi_handler				swi_exception
#define prefetch_abort_handler	prefetch_exception
#define data_abort_handler		data_abort_exception
#define reserved_handler		start
#endif

/*
 *  ベクターテーブル
 */
_kernel_arm_vector_add:
reset_vector:
        .long   start
undef_vector:
        .long   undef_handler
swi_vector:
        .long   swi_handler
prefech_vector:
        .long   prefetch_abort_handler
data_abort_vector:
        .long   data_abort_handler
reserved_vector:
		.long   reserved_handler
#ifndef _MINDSTORMSNXT_
#ifdef VECTOR_IN_ROM
irq_vector:
        .long   IRQ_Handler
fiq_vector:
        .long   FIQ_Handler
#else /* VECTOR_IN_ROM */
irq_vector:
        .long   irq_exception
fiq_vector:
        .long   fiq_exception
#endif /* VECTOR_IN_ROM */
#endif /* _MINDSTORMSNXT_ */




        .text
        .code 32
        .align     0
        .global    start
start:
        mov  r1,  #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT) /* To System mode */
		msr  cpsr, r1

#ifdef _MINDSTORMSNXT_

@
@ Do the remapping, if required, so that RAM is at address 0.
@
@  We read what is at address 0 and 2MB
@ We then invert what is at 2MB and read at 0 again
@ If address 0 has changed, then RAM is already at 0, so there is
@ no need to remap

        ldr  r0,=0x000000
        ldr  r2,=0x200000
        ldr  r3,[r0]
        ldr  r4,[r2]
        ldr  r5,=0xFFFFFFFF
        eor  r5,r4,r5
        str  r5,[r2]
        ldr  r5,[r0]
        str  r4,[r2]

        cmp  r3,r5
        bne  skip_remap_toggle

        ldr  r0,=(TADR_MC_BASE+TOFF_MC_RCR)
        ldr  r1,=1
        str  r1,[r0]

skip_remap_toggle:
        b    continue_boot

continue_boot:

@   Set up stacks etc.
@   We set up the irq and system stacks
@   We switch to system mode for further execution
@   NB Stacks should be 8-byte aligned for APCS
@   They should already be aligned, but we mask the values to make sure.
@
@ Set up Interrupt stack
        msr  CPSR_c,#0xD2 @ IRQ mode, IRQ, FIQ off
        ldr  r0, =__irq_stack__
        mov  sp,r0
        bic  sp,sp,#7    @ make sure it is 8-byte aligned

        msr  CPSR_c,#(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT) /* To System mode */

#endif	/* _MINDSTORMSNXT_ */

        /*
         * スタックポインタと，フレームポインタの初期化
         * SL(スタック上限レジスタr10)は設定する必要があるか?
         * gccはディフォルトでは-mapcsなので必要ない
         */
        ldr  r3, =STACKTOP
        mov  sp, r3
        mov  r11, #0        /* Armモード用フレームポインタ   */         
        mov  r7, #0         /* Thubmモード用フレームポインタ */

        /*
         * hardware_init_hookの呼び出し （0でない場合）
         *
         * ターゲットハードウェアに依存して必要な初期化処理がある場合
         * は，hardware_init_hook という関数を用意する
         */
        ldr  r0, =hardware_init_hook
        cmp  r0, #0x00
        blne hardware_init_hook


        /*
         *  bssセクションのクリア
         */        
start_1:
        ldr  r1, =__bss_start
        ldr  r2, =__bss_end
        cmp  r1,r2
        bhs  start_3
        mov  r0,#0
start_2:
        str  r0,[r1],#4
        cmp  r1, r2
        blo  start_2
start_3:
#if defined(NXTBIOS)
        /*
         *  vectorセクションの初期化(NXTBIOS用)
         *  __vectors_load_start__ から __vectors_load_end__ までを，__vectors_ram_start__ 以降に
         *  コピーする
         */
        ldr  r1, =__vectors_load_start__
        ldr  r3, =__vectors_load_end__
        cmp  r1, r3
        bhs  start_32
        ldr  r2, =__vectors_ram_start__
start_31:
        ldr  r0, [r1],#4
        str  r0, [r2],#4
        cmp  r1, r3
        blo  start_31
start_32:
#endif /* NXTBIOS */
#if defined(VECTOR_IN_ROM) || defined(NXTBIOS)
        /*
         *  dataセクションの初期化(ROM化対応)
         *  __idata_start から __idata_end までを，__data_start 以降に
         *  コピーする
         */
        ldr  r1, =__idata_start
        ldr  r3, =__idata_end
        cmp  r1, r3
        bhs  start_5
        ldr  r2, =__data_start
start_4:        
        ldr  r0, [r1],#4
        str  r0, [r2],#4
        cmp  r1, r3
        blo  start_4
#endif /* VECTOR_IN_ROM || NXTBIOS */
        /*
         *  software_init_hook を呼出し（0 でない場合）
         *
         *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
         *  理がある場合は，software_init_hook という関数を用意すれば
         *  よい．
         */
start_5:
        ldr   r0, =software_init_hook
        cmp   r0, #0x00
        blne  software_init_hook

start_6:
        ldr r0, =kernel_start
        mov lr, pc
        bx  r0

#ifdef __thumb__

        /*
         *  デフォルトエクセプション関数のthumb対応ハンドラ
         */
        .global undef_handler
        .global swi_handler
        .global prefetch_abort_handler
        .global data_abort_handler
        .global reserved_handler

        .text
        .code 32
        .align 	0

undef_handler:
        msr CPSR_c,#0xDF		@ System mode , I and F bits set (interrupts disabled)
        ldr r0,=undef_exception
        mov lr,pc
        bx  r0
undef_exception_returned:
        b   undef_exception_returned

swi_handler:
        msr CPSR_c,#0xDF		@ System mode , I and F bits set (interrupts disabled)
        ldr r0,=swi_exception
        mov lr,pc
        bx  r0
swi_exception_returned:
        b   swi_exception_returned

prefetch_abort_handler:
        msr CPSR_c,#0xDF		@ System mode , I and F bits set (interrupts disabled)
        ldr r0,=prefetch_exception
        mov lr,pc
        bx  r0
prefetch_exception_returned:
        b   prefetch_exception_returned

data_abort_handler:
        msr CPSR_c,#0xDF		@ System mode , I and F bits set (interrupts disabled)
        ldr r0,=data_abort_exception
        mov lr,pc
        bx  r0
data_abort_exception_returned:
        b   data_abort_exception_returned

reserved_handler:
        b   reserved_handler

#endif

