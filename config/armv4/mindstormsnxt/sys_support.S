/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2009      by GJ Designing Center Print Business Group
 *                             RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id: sys_support.S,v 1.1 2009/08/11 19:22:15 honda Exp $
 */

/*
 *	ターゲットシステム依存モジュール アセンブリ言語部
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"

        .text
        .code 32
        .align     0
        .global hardware_init_hook
        .global nxt_toppers_stop

hardware_init_hook:
@   MC_FMR:
@   Set up flash memory accessing and memory for 48 MHz FMCN = 50)
@   2 wait states (FWS = 1)
@   No special flags
@

        ldr r0,=(TADR_MC_BASE+TOFF_MC_FMR)
        ldr r1,=((72 << MC_FMR_FMCN_SHIFT) | MC_FMR_FWS_1FWS)
        str r1,[r0]

@
@    Disable watchdog
@    Set MDT_MR disable bit. 
@    Note: probably want to set this differently at some stage...
@
        ldr r0,=(TADR_WDT_BASE+TOFF_WDT_MR)
        ldr r1,=WDT_MR_WDDIS
        str r1,[r0]

@
@    Set up PLL & clock source
@    The main crystal is 18.432 MHz
@    We want as close as we can get to 48MHz
@    We use the PLL to generate 18432000 / 14 * 73 = 96109714
@    We then divide this by 2

@ Start main osc

        ldr r0,=(TADR_PMC_BASE+TOFF_CKGR_MOR)
        ldr r1,=((6 << CKGR_MOR_OSCOUNT_SHIFT) | CKGR_MOR_MOSCEN)
        str r1,[r0]

        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_SR)
wait_MOSCS:
        ldr r1,[r0]
        ands r1,r1,#PMC_SR_MOSCS
        beq wait_MOSCS

	@ set up PLL
        ldr r0,=(TADR_PMC_BASE+TOFF_CKGR_PLLR)
        ldr r1,=0x00481C0E
        str r1,[r0]

        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_SR)
wait_pll_lock:
        ldr r1,[r0]
        ands r1,r1,#PMC_SR_LOCK
        beq wait_pll_lock

        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_SR)
wait_MCLK_Ready0:
        ldr r1,[r0]
        ands r1,r1,#8
        beq  wait_MCLK_Ready0

@ Set up master clock to use clock div 2
        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_MCKR)
        ldr r1,=PMC_MCKR_PRES_CLK_2
        str r1,[r0]

        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_SR)
wait_MCLK_Ready1:
        ldr r1,[r0]
        ands r1,r1,#8
        beq  wait_MCLK_Ready1

@ Set up master clock to use PLL clock div 2
        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_MCKR)
        ldr r1,=(PMC_MCKR_PRES_CLK_2 | PMC_MCKR_CSS_PLL_CLOCK)
        str r1,[r0]

        ldr r0,=(TADR_PMC_BASE+TOFF_PMC_SR)
wait_MCLK_Ready2:
        ldr r1,[r0]
        ands r1,r1,#8
        beq  wait_MCLK_Ready2

        bx    lr


nxt_toppers_stop:
        msr cpsr, #(CPSR_IRQ_BIT | CPSR_FIQ_BIT | CPSR_SVC)
        ldr r0, =TADR_PIT_BASE
        mov r1, #0
        str r1, [r0, #TOFF_PIT_MR]
        b start

@
@	ISRs
@
@
        .global spurious_isr
        .global default_isr
        .global default_fiq
default_fiq:
spurious_isr:
default_isr:
        b default_isr

