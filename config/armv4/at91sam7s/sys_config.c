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
 *  @(#) $Id$
 */

/*
 *    ターゲットシステム依存モジュール（At91sam7s用）
 */

#include "jsp_kernel.h"
#include <at91sam7s.h>


/*interrupt mask*/
UW int_mode_table[MAX_INT_NUM]={
    0,
    0,
    0,
    0,
    0,
    0,
    SMR_US0,
    SMR_US1,
    0,
    0,
    0,
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

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
    unsigned int  baud = ((MCK * 10) / (115200 * 16));
    unsigned long brgr;

#ifndef VECTOR_IN_ROM
    /*
     *  ARMのベクタテーブルへ割込みハンドラを登録
     */
    arm_install_handler(IRQ_Number,IRQ_Handler);
#endif

    /*
     *  sys_putc が可能になるようにUARTを初期化
     */
    brgr = baud / 10;
    if((baud % 10) >= 5)
        brgr = (baud / 10) + 1;
    sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_PDR), (1<<9)|(1<<10));
    sil_wrw_mem((VP)(TADR_DBGU_BASE+TOFF_US_IDR), 0xFFFFFFFF);
    sil_wrw_mem((VP)(TADR_DBGU_BASE+TOFF_US_CR), US_RSTRX|US_RSTTX|US_RXDIS|US_TXDIS);
    sil_wrw_mem((VP)(TADR_DBGU_BASE+TOFF_US_BRGR), brgr);
    sil_wrw_mem((VP)(TADR_DBGU_BASE+TOFF_US_MR), 4<<9);
    sil_wrw_mem((VP)(TADR_DBGU_BASE+TOFF_US_CR), US_TXEN|US_RXEN);
}


/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
    syslog(LOG_EMERG, "End Kernel.....!");
    while(1);
}


/*
 *  ターゲットシステムの文字出力
 */

void
sys_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
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
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_IDCR), 1<<inhno);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SVR+inhno*4), (UW)inthdr);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_SMR+inhno*4), int_mode_table[inhno]);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_ICCR), 1<<inhno);
	sil_wrw_mem((VP)(TADR_AIC_BASE+TOFF_AIC_IECR), 1<<inhno);
}

/*
 *  未定義の割込みが入った場合の処理
 */
void
undef_interrupt(){
    syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
    while(1);
}

