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
 *  @(#) $Id: sys_config.c,v 1.1 2010/09/05 16:18:56 roi Exp $
 */

/*
 *    ターゲットシステム依存モジュール（lpc23xx用）
 */

#include "jsp_kernel.h"
#include <lpc23xx.h>


#define DEFAULT_PCLK    12000000
#define DEFAULT_DLL     0x00000003
#define DEFAULT_FDR     0x00000067

/*interrupt mask*/
UB int_mode_table[MAX_INT_NUM]={
    0,			/* 00: wdt */
    0,
    0,
    0,
    14,			/* 04: timer #0 lowest system timer */
    15,			/* 05: timer #1 */
    7,			/* 06: uart #0 console 1 */
    7,			/* 07: uart #1 console 2 */
    15,
    15,
    15,
    15,
    15,
    12,			/* 13: RTC */
    15,
    15,
    15,
    15,
    15,
    15,
    15,
    8,			/* 21: ETHER */
    15,
    15,
    3,			/* 24: MMC */
    9,			/* 25: GPDMA */
    15,			/* 26: timer #2 */
    15,			/* 27: timer #3 */
    15,			/* 28: uart #2 */
    15,			/* 29: uart #3 */
    15,
    15
};

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	unsigned long tmp;
	unsigned int  i;

   /* initialize VIC */
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_INTENCLR), 0xffffffff);
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_VECTADDR), 0x00000000);
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_INTSELECT), 0x00000000);

	for(i = 0 ; i < MAX_INT_NUM ; i++){
		sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_VECTADDRB+i*4), 0x00000000);
		sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_VECTPRIO+i*4), 0x0000000f);
	}

   /* Configure UART0 for 115200 baud. */
	tmp  = sil_rew_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL0));
	tmp &= ~0x000000f0;
	tmp |= 0x00000050;                   /* Enable RxD0 TxD0 pin         */
	sil_wrw_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL0), tmp);

	sil_wrw_mem((VP)(TADR_UART0_BASE+TOFF_UART_LCR), 0x00000083);	/* 8 bits, no Parity, 1 Stop bit*/
#if DEF_BAUDRATE == 115200
	sil_wrw_mem((VP)(TADR_UART0_BASE+TOFF_UART_DLL), DEFAULT_DLL);	/* for 12MHz PCLK Clock         */
	sil_wrw_mem((VP)(TADR_UART0_BASE+TOFF_UART_FDR), DEFAULT_FDR);	/* Fractional Divider           */
#else
	tmp = (DEFAULT_PCLK * 10) / 16 / DEF_BAUDRATE;
	tmp = (tmp+5) / 10;
	sil_wrw_mem((VP)(TADR_UART0_BASE+TOFF_UART_DLL), tmp & 0xff);	/* set divisor */
	sil_wrw_mem((VP)(TADR_UART0_BASE+TOFF_UART_DLM), tmp >> 8);
#endif
	sil_wrw_mem((VP)(TADR_UART0_BASE+TOFF_UART_LCR), 0x00000003);	/* DLAB = 0                     */

   /* Configure UART1 for 115200 baud. */
	tmp  = sil_rew_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL0));
	tmp &= ~0xC0000000;
	tmp |= 0x40000000;                   /* Enable TxD1 pin              */
	sil_wrw_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL0), tmp);
	tmp  = sil_rew_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL1));
	tmp &= ~0x00000003;
	tmp |= 0x00000001;                   /* Enable RxD1 pin              */

	sil_wrw_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL1), tmp);

	sil_wrw_mem((VP)(TADR_UART1_BASE+TOFF_UART_LCR), 0x00000083);	/* 8 bits, no Parity, 1 Stop bit*/
#if DEF_BAUDRATE == 115200
	sil_wrw_mem((VP)(TADR_UART1_BASE+TOFF_UART_DLL), DEFAULT_DLL);	/* for 12MHz PCLK Clock         */
	sil_wrw_mem((VP)(TADR_UART1_BASE+TOFF_UART_FDR), DEFAULT_FDR);	/* Fractional Divider           */
#else
	tmp = (DEFAULT_PCLK * 10) / 16 / DEF_BAUDRATE;
	tmp = (tmp+5) / 10;
	sil_wrw_mem((VP)(TADR_UART1_BASE+TOFF_UART_DLL), tmp & 0xff);	/* set divisor */
	sil_wrw_mem((VP)(TADR_UART1_BASE+TOFF_UART_DLM), tmp >> 8);
#endif
	sil_wrw_mem((VP)(TADR_UART1_BASE+TOFF_UART_LCR), 0x00000003);	/* DLAB = 0                     */

	/* MMC pin seklect (MCI poweron pin to GPIO) */
	tmp  = sil_rew_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL1));
	tmp &= ~0x00003fc0;
	tmp |= 0x00002A80;
	sil_wrw_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL1), tmp);
	tmp  = sil_rew_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL4));
	tmp &= ~0x0fc00000;
	tmp |= 0x0A800000;
	sil_wrw_mem((VP)(TADR_PINSEL_BASE+TOFF_PINSEL4), tmp);

#if 0	/* ROI DEBUG */
	sil_wrw_mem((VP)(TADR_FIO_BASE+TOFF_FIO2_CLR), 0x000000aa);		/* led on */
#endif	/* ROI DEBUG */
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
        lpc2300_uart_putc('\r');
    }
    lpc2300_uart_putc(c);
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
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_INTENCLR), 1<<inhno);
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_VECTADDRB+inhno*4), (UW)inthdr);
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_VECTPRIO+inhno*4), (UW)int_mode_table[inhno]);
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_SOFTINTCLR), 1<<inhno);
	sil_wrw_mem((VP)(TADR_VIC_BASE+TOFF_VIC_INTENABLE), 1<<inhno);
}

/*
 *  未定義の割込みが入った場合の処理
 */
void
undef_interrupt(){
    syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
    while(1);
}

