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
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2007-     by Monami Software Limited Partnership, JAPAN
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
 *  @(#) $Id: sys_config.c,v 1.1 2006/06/11 11:18:56 roi Exp $
 */

/*
 *    ターゲットシステム依存モジュール（mindstormsnxt用）
 */

#include "jsp_kernel.h"
#include <mindstormsnxt.h>


/*interrupt mask*/
UW int_mode_table[MAX_INT_NUM]={
    0,
    SMR_SYSIRQ,
    SMR_PIOA,
    0,
    0,
    SMR_SPI,
    SMR_US0,
    SMR_US1,
    SMR_SSC,
    SMR_TWI,
    SMR_PMW,
    0,
    SMR_TC0,
    SMR_TC1,
    SMR_TC2,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0
};


/* Default handlers for the three general kinds of interrupts that the
 * ARM core has to handle. These are defined in irq.s, and just freeze
 * the board in an infinite loop.
 */
extern void default_isr(void);
extern void default_fiq(void);
extern void spurious_isr(void);
extern void nxt_toppers_stop(void);

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	int i;

   /* If we're coming from a warm boot, the AIC may be in a weird
    * state. Do some cleaning up to bring the AIC back into a known
    * state:
    *  - All interrupt lines disabled,
    *  - No interrupt lines handled by the FIQ handler,
    *  - No pending interrupts,
    *  - AIC idle, not handling an interrupt.
    */
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_IDCR), 0xFFFFFFFF);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_FFDR), 0xFFFFFFFF);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_ICCR), 0xFFFFFFFF);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_EOICR), 1);

   /* Enable debug protection. This is necessary for JTAG debugging, so
    * that the hardware debugger can read AIC registers without
    * triggering side-effects.
    */
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_DCR), 1);

   /* Set default handlers for all interrupt lines. */
	for (i = 0; i < 32; i++) {
		sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SMR+i*4), 0);
		sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SVR+i*4), (unsigned long)default_isr);
	}
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SVR+IRQ_FIQ_PID*4), (unsigned long)default_fiq);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SVR+TOFF_AIC_SPU), (unsigned long)spurious_isr);
}


/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
    nxt_toppers_stop();
    while(1);
}


/*
 *  ターゲットシステムの文字出力
 */

void
sys_putc(char c)
{
}


/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
void
define_inh(INHNO inhno, FP inthdr)
{
    assert(inhno < MAX_INT_NUM);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_IDCR), (1 << inhno));
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SMR+inhno*4), int_mode_table[inhno]);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SVR+inhno*4), (VW)inthdr);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_IECR), (1 << inhno));
}

/*
 *  未定義の割込みが入った場合の処理
 */
void
undef_interrupt(){
    syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
    while(1);
}

