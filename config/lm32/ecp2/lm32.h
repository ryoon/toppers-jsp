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
 *  @(#) $Id: lm32.h,v 1.14 2007/07/27 11:28:44 honda Exp $
 */

#ifndef _LM32_H_
#define _LM32_H_

#define MICO32_CPU_CLOCK_HZ     (25000000)
//#define MICO32_CPU_CLOCK_HZ   (100000000)

#define DEFAULT_UART_BAUDRATE  (115200)

#define TIMER_BASE_REG          (0x80000100)
#define UART1_BASE_REG          (0x80000180)

/*
 * Timer Registers
 */
#define TIMER_STATUS            (TIMER_BASE_REG)
#define TIMER_CONTROL           (TIMER_BASE_REG + 0x04)
#define TIMER_PERIOD            (TIMER_BASE_REG + 0x08)
#define TIMER_SNAPSHOT          (TIMER_BASE_REG + 0x0C)

#define TIMER_STATUS_TO         (0x01)
#define TIMER_CONTROL_ITO       (0x01)
#define TIMER_CONTROL_CONT      (0x02)
#define TIMER_CONTROL_START     (0x04)
#define TIMER_CONTROL_STOP      (0x08)

/*
 * UART0 Registers
 */
#define UART1_RX_TX    (UART1_BASE_REG)
#define UART1_IER      (UART1_BASE_REG + 0x04)
#define UART1_IIR      (UART1_BASE_REG + 0x08)
#define UART1_LCR      (UART1_BASE_REG + 0x0C)
#define UART1_MCR      (UART1_BASE_REG + 0x10)
#define UART1_LSR      (UART1_BASE_REG + 0x14)
#define UART1_MSR      (UART1_BASE_REG + 0x18)
#define UART1_DIV      (UART1_BASE_REG + 0x1C)

/*
 * UART1 Registers
 */
#define UART2_RX_TX    (UART2_BASE_REG)
#define UART2_IER      (UART2_BASE_REG + 0x04)
#define UART2_IIR      (UART2_BASE_REG + 0x08)
#define UART2_LCR      (UART2_BASE_REG + 0x0C)
#define UART2_MCR      (UART2_BASE_REG + 0x10)
#define UART2_LSR      (UART2_BASE_REG + 0x14)
#define UART2_MSR      (UART2_BASE_REG + 0x18)
#define UART2_DIV      (UART2_BASE_REG + 0x1C)

#define UART_IER_RX_INT_MASK    (0x01)
#define UART_IER_TX_INT_MASK    (0x02)
#define UART_LSR_RX_RDY_MASK    (0x01)
#define UART_LSR_TX_RDY_MASK    (0x20)
#define UART_IIR_RXRDY          (0x04)
#define UART_IIR_TXRDY          (0x02)

#define MAX_INT_NUM     32

#define INHNO_TIMER     0
#define INHNO_SIO1		1

#define TNUM_PORT   1
#define TNUM_SIOP   1

#ifndef _MACRO_ONLY

extern void uart1_isr(void);
extern void uart2_isr(void);

typedef struct sio_port_initialization_block 
{
    volatile VP rxtx;
    volatile VP ier;
    volatile VP iir;
    volatile VP lcr;
    volatile VP mcr;
    volatile VP lsr;
    volatile VP msr;
    volatile VP div;
}
SIOPINIB;

typedef struct sio_port_control_block 
{
    const SIOPINIB  *siopinib;
    VP_INT          exinf;
    BOOL            openflag;
    BOOL            sendflag;
    BOOL            getready;
    BOOL            putready;
    UW              ier_snapshot;
}SIOPCB;

#define SIO_ERDY_SND    1u
#define SIO_ERDY_RCV    2u

extern void      uart_putc(char c);
extern void      uart_initialize(void);
extern BOOL      uart_openflag(void);
extern SIOPCB   *uart_opn_por(ID siopid, VP_INT exinf);
extern void      uart_cls_por(SIOPCB *siopcb);
extern BOOL      uart_snd_chr(SIOPCB *siopcb, char c);
extern INT       uart_rcv_chr(SIOPCB *siopcb);
extern void      uart_ena_cbr(SIOPCB *siopcb, UINT cbrtn);
extern void      uart_dis_cbr(SIOPCB *siopcb, UINT cbrtn);
extern void      uart_in_isr(void);
extern void      uart_out_isr(void);
extern void      uart_ierdy_snd(VP_INT exinf);
extern void      uart_ierdy_rcv(VP_INT exinf);

#endif /* _MACRO_ONLY */
#endif /* _LM32_H_ */
