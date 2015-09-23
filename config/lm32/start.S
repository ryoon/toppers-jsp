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
 *  @(#) $Id: start.S,v 1.14 2007/07/27 11:28:44 honda Exp $
 */

#define _MACRO_ONLY

#include "jsp_kernel.h"	

	.section ".boot","ax"
	.global reset
reset:
_reset_handler:
	xor     r0, r0, r0
	wcsr    IE, r0
    mvhi    r1, hi(_reset_handler)
    ori     r1, r1, lo(_reset_handler)
    wcsr    EBA, r1
    calli   _start
	nop
	nop
_breakpoint_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 1
	calli   _process_exception
	bi      _restore_all_and_bret
	nop
	nop
	nop
_instruction_bus_error_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 2
	calli   _process_exception
	bi      _restore_all_and_eret
	nop 
	nop
	nop
_watchpoint_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 3
	calli   _process_exception
	bi      _restore_all_and_bret
	nop 
	nop
	nop
_data_bus_error_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 4
	calli   _process_exception
	bi      _restore_all_and_eret
	nop 
	nop
	nop
_divide_by_zero_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 5
	calli   _process_exception
	bi      _restore_all_and_eret
	nop 
	nop
	nop
_interrupt_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 6
	calli   _process_exception
	bi      _restore_all_and_eret
	nop 
	nop
	nop
_system_call_handler:
	sw      (sp+0), ra
	calli   _save_all
	ori     r1, r0, 7
	calli   _process_exception
	bi      _restore_all_and_eret
	nop 
	nop
	nop

	.global _start
_start:
    /* Clear r0 */
    xor     r0, r0, r0
    /* Initialize the cache */
    wcsr    icc, r1
    nop
    nop
    nop
    nop
    wcsr    dcc, r1
    nop
    nop
    nop
    nop
    /* clear the interrupt */
    wcsr    ie, r0
    wcsr    im, r0
    mvhi    r1, 0xFFFF
    ori     r1, r1, 0xFFFF
    wcsr    ip, r1
    /* Setup stack and global pointer */
    mvhi    sp, hi(_fstack)
    ori     sp, sp, lo(_fstack)
    mvhi    gp, hi(_gp)
    ori     gp, gp, lo(_gp)
    /* Clear BSS */
    mvhi    r1, hi(_fbss)
    ori     r1, r1, lo(_fbss)
    mvhi    r3, hi(_ebss)
    ori     r3, r3, lo(_ebss)
clear_bss:
    be      r1, r3, start_kernel
    sw      (r1+0), r0
    addi    r1, r1, 4
    bi      clear_bss
start_kernel:	        
	calli kernel_start
