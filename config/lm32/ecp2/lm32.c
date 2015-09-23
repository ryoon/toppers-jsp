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
 *  @(#) $Id: lm32.c,v 1.14 2007/07/27 11:28:44 honda Exp $
 */

#include <s_services.h>
#include <lm32.h>

const SIOPINIB siopinib_table[TNUM_SIOP] = {
    {
        (VP)UART1_RX_TX,
        (VP)UART1_IER,
        (VP)UART1_IIR,
        (VP)UART1_LCR,
        (VP)UART1_MCR,
        (VP)UART1_LSR,
        (VP)UART1_MSR,
        (VP)UART1_DIV
    }
#if TNUM_SIOP >= 2
    ,
    {
        (VP)UART2_RX_TX,
        (VP)UART2_IER,
        (VP)UART2_IIR,
        (VP)UART2_LCR,
        (VP)UART2_MCR,
        (VP)UART2_LSR,
        (VP)UART2_MSR,
        (VP)UART2_DIV
    }
#endif /* TNUM_SIOP >= 2 */
};

SIOPCB  siopcb_table[TNUM_SIOP];

#define INDEX_SIOP(siopid)  ((UINT)((siopid) - 1))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))

Inline BOOL
uart_getready(SIOPCB *siopcb)
{
    return(sil_rew_mem((VP)(siopcb->siopinib->lsr)) & UART_LSR_RX_RDY_MASK);
}

Inline BOOL
uart_putready(SIOPCB *siopcb)
{
    return(sil_rew_mem((VP)(siopcb->siopinib->lsr)) & UART_LSR_TX_RDY_MASK);
}

Inline UB
uart_getchar(SIOPCB *siopcb)
{
    return(sil_rew_mem((VP)(siopcb->siopinib->rxtx)));
}

Inline void
uart_putchar(SIOPCB *siopcb, UB c)
{
    sil_wrw_mem((VP)(siopcb->siopinib->rxtx), c);
}

Inline void
uart_enable_send(SIOPCB *siopcb)
{
	const SIOPINIB *siopinib;
	siopinib = siopcb->siopinib;
	siopcb->ier_snapshot |= UART_IER_TX_INT_MASK;
    sil_wrw_mem((VP)siopinib->ier, siopcb->ier_snapshot);	
}

Inline void
uart_disable_send(SIOPCB *siopcb)
{
	const SIOPINIB *siopinib;
	siopinib = siopcb->siopinib;	
	siopcb->ier_snapshot &= (~UART_IER_TX_INT_MASK);
    sil_wrw_mem((VP)siopinib->ier, siopcb->ier_snapshot);	
}

Inline void
uart_enable_rcv(SIOPCB *siopcb)
{
	const SIOPINIB *siopinib;
	siopinib = siopcb->siopinib;	
	siopcb->ier_snapshot |= UART_IER_RX_INT_MASK;
    sil_wrw_mem((VP)siopinib->ier, siopcb->ier_snapshot);	
}

Inline void
uart_disable_rcv(SIOPCB *siopcb)
{
	const SIOPINIB *siopinib;
	siopinib = siopcb->siopinib;
	siopcb->ier_snapshot &= (~UART_IER_RX_INT_MASK);
    sil_wrw_mem((VP)siopinib->ier, siopcb->ier_snapshot);	
}

void
init_uart(void)
{
    volatile unsigned int divisor;
    UW sr;

    sil_wrw_mem((VP)UART1_IER, 0);

    /* 8Data, 1Stop, No Parity*/
    sil_wrw_mem((VP)UART1_LCR, 0x3);

    /* 115200bps */
    divisor = ((1048576.0 * (DEFAULT_UART_BAUDRATE/100)) / (MICO32_CPU_CLOCK_HZ/100)) + 0.5;
    //divisor = 0x12e0;
    sil_wrw_mem((VP)UART1_DIV, divisor);

    sil_wrw_mem((VP)UART1_IER, UART_IER_RX_INT_MASK);
}

void
uart_putc(char c)
{
	volatile unsigned int lsr;
	
	while(1)
	{
		lsr = sil_rew_mem((VP)(UART1_LSR));
		if(lsr & UART_LSR_TX_RDY_MASK)		
		{
			sil_wrw_mem((VP)UART1_RX_TX, (VW)c);
			return;
		}
	}
}

void
uart_initialize()
{
    SIOPCB  *siopcb;
    UINT    i;

    for (siopcb = siopcb_table, i = 0; i < TNUM_SIOP; siopcb++, i++) {
        siopcb->siopinib = &(siopinib_table[i]);
        siopcb->openflag = FALSE;
        siopcb->sendflag = FALSE;
    }
}

BOOL
uart_openflag(void)
{
    return(siopcb_table[0].openflag);
}

SIOPCB *
uart_opn_por(ID siopid, VP_INT exinf)
{
    SIOPCB *siopcb;
    const SIOPINIB *siopinib;
    volatile unsigned int divisor;
    UW sr;

    siopcb = get_siopcb(siopid);
    siopinib = siopcb->siopinib;

    sil_wrw_mem((VP)siopinib->ier, 0);

    /* 8Data, 1Stop, No Parity*/
    sil_wrw_mem((VP)siopinib->lcr, 0x3);

    /* 115200bps */
    divisor = ((1048576.0 * (DEFAULT_UART_BAUDRATE/100)) / (MICO32_CPU_CLOCK_HZ/100)) + 0.5;
    //divisor = 0x12e0;
    sil_wrw_mem((VP)siopinib->div, divisor);

    siopcb->ier_snapshot = UART_IER_RX_INT_MASK;
    sil_wrw_mem((VP)siopinib->ier, siopcb->ier_snapshot);
    
    Asm("rcsr %0, im" : "=r"(sr));
    sr |= (1 << INHNO_SIO1);
    Asm("wcsr im, %0" : : "r"(sr) );    
    
    siopcb->exinf = exinf;
    siopcb->getready = siopcb->putready = FALSE;
    siopcb->openflag = TRUE;
    
    return(siopcb);
}

void
uart_cls_por(SIOPCB *siopcb)
{
	const SIOPINIB *siopinib;
	siopinib = siopcb->siopinib;
    sil_wrw_mem((VP)siopinib->ier, 0);
    siopcb->openflag = FALSE;
}

BOOL
uart_snd_chr(SIOPCB *siopcb, char c)
{
    if (uart_putready(siopcb)){
        uart_putchar(siopcb, c);
        return(TRUE);
    }
    return(FALSE);
}

INT
uart_rcv_chr(SIOPCB *siopcb)
{
    if (uart_getready(siopcb)) {
        return((INT)(UB) uart_getchar(siopcb));
    }
    return(-1);
}

void
uart_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{
    switch (cbrtn) {
        case SIO_ERDY_SND:
            uart_enable_send(siopcb);
            break;
        case SIO_ERDY_RCV:
            uart_enable_rcv(siopcb);
            break;
    }
}

void
uart_dis_cbr(SIOPCB *siopcb, UINT cbrtn)
{
    switch (cbrtn) {
        case SIO_ERDY_SND:
            uart_disable_send(siopcb);
            break;
        case SIO_ERDY_RCV:
            uart_disable_rcv(siopcb);
            break;
    }
}

static void
uart_isr_siop(SIOPCB *siopcb)
{
	volatile UW iir;
	
	iir = sil_rew_mem((VP)(siopcb->siopinib->iir));
	
    if (uart_getready(siopcb)) {
        uart_ierdy_rcv(siopcb->exinf);
    }
    if (uart_putready(siopcb)) {
        uart_ierdy_snd(siopcb->exinf);
    }
}

void
uart_isr0()
{
    uart_isr_siop(&(siopcb_table[0]));
}

#if TNUM_SIOP >= 2
void
uart_isr1()
{
    uart_isr_siop(&(siopcb_table[1]));
}
#endif

