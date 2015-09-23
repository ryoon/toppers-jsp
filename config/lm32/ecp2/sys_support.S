/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: sys_support.S,v 1.14 2007/07/27 11:28:44 honda Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
 
         .text
        .align 4    
        .global Interrupt_exception_handler
        .global _save_all
        .global _restore_all_and_bret
        .global _restore_all_and_eret
        .global _process_exception

_process_exception:
	addi    sp, sp, -4
	sw      (sp+4), ra
/*
	rcsr    r2, ie
	ori     r3, r0, 1
	and     r2, r2, r3
	be      r2, r0, _normal_process
bug:
	bi      bug
*/
_normal_process:		
	mvhi    r2, hi(exception_count)
	ori     r2, r2, lo(exception_count)
	lw      r3, (r2+0)
    addi    r4, r3, 1
	sw      (r2+0), r4
	bne     r3, r0,	nest_exception

	mv      r2, sp
	mvhi    r3, hi(_fstack)
	ori     r3, r3, lo(_fstack)
    mv      sp, r3
	addi    sp, sp, -4
	sw      (sp+4), r2
	
nest_exception:

	ori     r2, r0, 6
	be      r1, r2, _process_interrupt
    rcsr    r2, ie
    ori     r2, r2, 0x1
	wcsr    ie, r2		
	
_process_interrupt:	

	addi    sp, sp, -4
	sw      (sp+4), r1

	sli     r3, r1, 2
	mvhi    r2, hi(exc_table)
	ori     r2, r2, lo(exc_table)
	add     r3, r3, r2
	lw      r2, (r3+0)
	call    r2

	addi    sp, sp, 4
	lw      r1, (sp+0)
	
	ori     r2, r0, 6
	be      r1, r2, _process_interrupt_exit
    rcsr    r2, ie
    andi    r2, r2, 0x6
	wcsr    ie, r2		
	
_process_interrupt_exit:	
		
	mvhi    r1, hi(exception_count)
	ori     r1, r1, lo(exception_count)
	lw      r2, (r1+0)
	addi    r2, r2, -1
	sw      (r1+0), r2
	bne     r2, r0, return_to_task_exc

	addi    sp, sp, 4
	lw      r1, (sp+0)
	mv      sp, r1

    mvhi    r2, hi(reqflg)
	ori     r2, r2, lo(reqflg)
	lw      r1, (r2+0)
	be      r1, r0, return_to_task_exc
	sw      (r2+0), r0
	bi      ret_exc

return_to_task_exc:
	addi    sp, sp, 4
	lw      ra, (sp+0)
	ret    
    		
_save_all:
    addi    sp, sp, -128
    sw      (sp+4),   r1
    sw      (sp+8),   r2
    sw      (sp+12),  r3
    sw      (sp+16),  r4
    sw      (sp+20),  r5
    sw      (sp+24),  r6
    sw      (sp+28),  r7
    sw      (sp+32),  r8
    sw      (sp+36),  r9
    sw      (sp+40),  r10
    sw      (sp+44),  r11
    sw      (sp+48),  r12
    sw      (sp+52),  r13
    sw      (sp+56),  r14
    sw      (sp+60),  r15
    sw      (sp+64),  r16
    sw      (sp+68),  r17
    sw      (sp+72),  r18
    sw      (sp+76),  r19
    sw      (sp+80),  r20
    sw      (sp+84),  r21
    sw      (sp+88),  r22
    sw      (sp+92),  r23
    sw      (sp+96),  r24
    sw      (sp+100), r25
    sw      (sp+104), r26
    sw      (sp+108), r27    
    
	sw      (sp+120), ea
	sw      (sp+124), ba

    lw      r1, (sp+128)
    sw      (sp+116), r1
    mv      r1, sp
    addi    r1, r1, 128
    sw      (sp+112), r1	

	mvhi    r1, hi(exception_registers)
	ori     r1, r1, lo(exception_registers)
	sw      (r1+0), r0
	sw      (r1+8), r2 
    sw      (r1+12), r3
    sw      (r1+16), r4
    sw      (r1+20), r5
    sw      (r1+24), r6
    sw      (r1+28), r7
    sw      (r1+32), r8
    sw      (r1+36), r9
    sw      (r1+40), r10
    sw      (r1+44), r11
    sw      (r1+48), r12
    sw      (r1+52), r13
    sw      (r1+56), r14
    sw      (r1+60), r15
    sw      (r1+64), r16
    sw      (r1+68), r17
    sw      (r1+72), r18
    sw      (r1+76), r19
    sw      (r1+80), r20
    sw      (r1+84), r21
    sw      (r1+88), r22
    sw      (r1+92), r23
    sw      (r1+96), r24
    sw      (r1+100), r25
    sw      (r1+104), r26
    sw      (r1+108), r27
    sw      (r1+120), ea
    sw      (r1+124), ba
	
    lw      r2, (sp+4)
	sw      (r1+4), r2
	lw      r2, (sp+56)
	sw      (r1+116), r2	
	addi    r2, sp, 56
    sw      (r1+112), r2
    rcsr    r2, ie
    sw      (r1+128), r2
    rcsr    r2, im
    sw      (r1+132), r2
    rcsr    r2, ip
    sw      (r1+138), r2
	rcsr    r2, icc
    sw      (r1+140), r2
	rcsr    r2, dcc
    sw      (r1+144), r2
	rcsr    r2, cc
    sw      (r1+148), r2
	rcsr    r2, cfg
    sw      (r1+152), r2    
	rcsr    r2, eba
    sw      (r1+158), r2    
             	
	ret
    
_restore_all_and_eret:
    lw      r1,  (sp+4)
    lw      r2,  (sp+8) 
    lw      r3,  (sp+12) 
    lw      r4,  (sp+16) 
    lw      r5,  (sp+20) 
    lw      r6,  (sp+24) 
    lw      r7,  (sp+28) 
    lw      r8,  (sp+32) 
    lw      r9,  (sp+36) 
    lw      r10, (sp+40)
    lw      r11, (sp+44)
    lw      r12, (sp+48)
    lw      r13, (sp+52)
    lw      r14, (sp+56)
    lw      r15, (sp+60)
    lw      r16, (sp+64)
    lw      r17, (sp+68)
    lw      r18, (sp+72)
    lw      r19, (sp+76)
    lw      r20, (sp+80)
    lw      r21, (sp+84)
    lw      r22, (sp+88)
    lw      r23, (sp+92)
    lw      r24, (sp+96)
    lw      r25, (sp+100)
    lw      r26, (sp+104)
    lw      r27, (sp+108)    
    lw      ra,  (sp+116)
    lw      ea,  (sp+120)
    lw      ba,  (sp+124)
    lw      sp,  (sp+112)
	eret

_restore_all_and_bret:
    lw      r1,  (sp+4)
    lw      r2,  (sp+8) 
    lw      r3,  (sp+12) 
    lw      r4,  (sp+16) 
    lw      r5,  (sp+20) 
    lw      r6,  (sp+24) 
    lw      r7,  (sp+28) 
    lw      r8,  (sp+32) 
    lw      r9,  (sp+36) 
    lw      r10, (sp+40)
    lw      r11, (sp+44)
    lw      r12, (sp+48)
    lw      r13, (sp+52)
    lw      r14, (sp+56)
    lw      r15, (sp+60)
    lw      r16, (sp+64)
    lw      r17, (sp+68)
    lw      r18, (sp+72)
    lw      r19, (sp+76)
    lw      r20, (sp+80)
    lw      r21, (sp+84)
    lw      r22, (sp+88)
    lw      r23, (sp+92)
    lw      r24, (sp+96)
    lw      r25, (sp+100)
    lw      r26, (sp+104)
    lw      r27, (sp+108)    
    lw      ra,  (sp+116)
    lw      ea,  (sp+120)
    lw      ba,  (sp+124)
    lw      sp,  (sp+112)
	bret
        
Interrupt_exception_handler:
	addi    sp, sp, -4
	sw      (sp+4), ra

    rcsr    r1, ip
    rcsr    r2, im
    and     r1, r1, r2
   
    addi    r2, r0, 1
	addi    r4, r0, 0

find_bit_loop:
    and     r3, r1, r2
	sli     r2, r2, 1
	addi    r4, r4, 1
    be      r3, r0, find_bit_loop
    addi    r4, r4, -1
        
save_intr_mask:
	rcsr    r1, im
	addi    sp, sp, -4
	sw      (sp+4), r1

    mvhi    r1, hi(int_mask_table)
	ori     r1, r1, lo(int_mask_table)
    sli     r5, r4, 2
	add     r1, r1, r5
    lw      r1, (r1+0)
    rcsr    r2, im
    and     r1, r1, r2
    wcsr    im, r1
    
    mvhi    r2, hi(int_table)
    ori     r2, r2, lo(int_table)
    sli     r5, r4, 2
    add     r2, r2, r5
    lw      r2, (r2+0)
    be      r2, r0, undefined_interrupt

	addi    sp, sp, -4
	sw      (sp+4), r4    
    
    /* Enable Interrupt */
    rcsr    r1, ie
    ori     r1, r1, 0x1
	wcsr    ie, r1	

call_user_isr:
    call    r2

    /* Disable Interrupt */
	rcsr    r1, ie
	andi    r1, r1, 0x6
	wcsr    ie, r1	 

clear_intr_pending:	
	addi    sp, sp, 4
	lw      r4, (sp+0)

	addi    r1, r0, 1
    sl      r2, r1, r4
    wcsr    ip, r2
	
restore_intr_mask:
	addi    sp, sp, 4
	lw      r1, (sp+0)
	wcsr    im, r1
	
	addi    sp, sp, 4
	lw      ra, (sp+0)
	ret
	nop

undefined_interrupt:
    bi      undef_interrupt
