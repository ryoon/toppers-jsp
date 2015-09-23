/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: microblaze.h,v 1.1 2002/04/05 08:47:14 honda Exp $
 */

#ifndef _MICROBLAZE_H_
#define _MICROBLAZE_H_

#include "sys_config.h"

#ifndef _MACRO_ONLY
typedef unsigned char   byte;
typedef volatile byte   IOREG;          
typedef unsigned short  word;
typedef volatile word   HIOREG;
typedef volatile int    LIOREG;
#endif /* _MACRO_ONLY */

/*
 *  MSRのビット
 */
#define MSR_IE 0x02
#define MSR_BE 0x01


/*
 * INTC Registers
 */
#define INTC_INT_STATUS_REG    0x00
#define INTC_INT_PENDING_REG   0x04
#define INTC_INT_ENABLE_REG    0x08
#define INTC_INT_ACK_REG       0x0c
#define INTC_SET_INT_ENABLE    0x10
#define INTC_CLEAR_INT_ENABLE  0x14
#define INTC_INT_VECTOR_REG    0x18
#define INTC_MASTER_ENABLE_REG 0x1c

/* 
 * Masks for the INTC Registers
 */
#define INTC_HIE_MASK 0x2
#define INTC_ME_MASK  0x1


#ifndef _MACRO_ONLY
#define INTC_ISR ((LIOREG *)(INTC_BASE + INTC_INT_STATUS_REG))
#define INTC_IPR ((LIOREG *)(INTC_BASE + INTC_INT_PENDING_REG))
#define INTC_IER ((LIOREG *)(INTC_BASE + INTC_INT_ENABLE_REG))
#define INTC_IAR ((LIOREG *)(INTC_BASE + INTC_INT_ACK_REG))
#define INTC_SIE ((LIOREG *)(INTC_BASE + INTC_SET_INT_ENABLE))
#define INTC_CIE ((LIOREG *)(INTC_BASE + INTC_CLEAR_INT_ENABLE))
#define INTC_IVR ((LIOREG *)(INTC_BASE + INTC_INT_VECTOR_REG))
#define INTC_MER ((LIOREG *)(INTC_BASE + INTC_MASTER_ENABLE_REG))

/*
 *  INTC操作関数
 */
Inline void
intc_start(void){
  *INTC_MER = INTC_HIE_MASK | INTC_ME_MASK;
}

Inline void
intc_disable_allinterrupt(){
  *INTC_MER = 0;
}

Inline void
intc_enable_interrupt(UW mask)
{
  *INTC_SIE = mask;
}

Inline void
intc_disable_interrupt(UW mask)
{
  *INTC_CIE = mask;
}

Inline void
intc_ack_interrupt(UW mask)
{
  *INTC_IAR = mask;
}

#endif /* _MACRO_ONLY */



/*
 * タイマドライバ用のマクロ定義
 */


/*
 * Masks for the control/status register
 */
#define TIMER_ENABLE_ALL    0x400
#define TIMER_PWM           0x200
#define TIMER_INTERRUPT     0x100
#define TIMER_ENABLE        0x080
#define TIMER_ENABLE_INTR   0x040
#define TIMER_RESET         0x020
#define TIMER_RELOAD        0x010
#define TIMER_EXT_CAPTURE   0x008
#define TIMER_EXT_COMPARE   0x004
#define TIMER_DOWN_COUNT    0x002
#define TIMER_CAPTURE_MODE  0x001

#define TIMER_CONTROL_STATUS_0  0x0
#define TIMER_COMPARE_CAPTURE_0 0x4
#define TIMER_COUNTER_0         0x8
#define TIMER_CONTROL_STATUS_1  0x10
#define TIMER_COMPARE_CAPTURE_1 0x14
#define TIMER_COUNTER_1         0x18


#ifndef _MACRO_ONLY
#define TIMER_TCSR0 ((LIOREG *)(TIMER_BASE + TIMER_CONTROL_STATUS_0))
#define TIMER_TCCR0 ((LIOREG *)(TIMER_BASE + TIMER_COMPARE_CAPTURE_0))
#define TIMER_TCR0  ((LIOREG *)(TIMER_BASE + TIMER_COUNTER_0))
#define TIMER_TCSR1 ((LIOREG *)(TIMER_BASE + TIMER_CONTROL_STATUS_1))
#define TIMER_TCCR1 ((LIOREG *)(TIMER_BASE + TIMER_COMPARE_CAPTURE_1))
#define TIMER_TCR1  ((LIOREG *)(TIMER_BASE + TIMER_COUNTER_1))
#endif  /* _MACRO_ONLY */



/*
 *  UARTドライバ用のマクロ定義
 */


/*
 * Error condition masks 
 */
#define UARTLITE_PAR_ERROR     0x80
#define UARTLITE_FRAME_ERROR   0x40
#define UARTLITE_OVERRUN_ERROR 0x20


/*
 * Other status bit masks
 */
#define UARTLITE_INTR_ENABLED       0x10
#define UARTLITE_TX_FIFO_FULL       0x08
#define UARTLITE_TX_FIFO_EMPTY      0x04
#define UARTLITE_RX_FIFO_FULL       0x02
#define UARTLITE_RX_FIFO_VALID_DATA 0x01

/*
 * Control bit masks 
 */
#define UARTLITE_ENABLE_INTR  0x10
#define UARTLITE_RST_RX_FIFO  0x02
#define UARTLITE_RST_TX_FIFO  0x01

/*
 * UARTLITE Registers
 */
#define UARTLITE_RECEIVE_REG  0x0
#define UARTLITE_TRANSMIT_REG 0x4
#define UARTLITE_STATUS_REG   0x8
#define UARTLITE_CONTROL_REG  0xc



#ifndef _MACRO_ONLY
#define UARTLITE_RXREG ((LIOREG *)(UART_BASE + UARTLITE_RECEIVE_REG))
#define UARTLITE_TXREG ((LIOREG *)(UART_BASE + UARTLITE_TRANSMIT_REG))
#define UARTLITE_SRREG ((LIOREG *)(UART_BASE + UARTLITE_STATUS_REG))
#define UARTLITE_CTREG ((LIOREG *)(UART_BASE + UARTLITE_CONTROL_REG))


Inline void
uart_putc(unsigned char c){
  while(*UARTLITE_SRREG & UARTLITE_TX_FIFO_FULL);
  *UARTLITE_TXREG = c;
}

Inline unsigned char
uart_getc(void){
  while(!(*UARTLITE_SRREG & UARTLITE_RX_FIFO_VALID_DATA));
  return (char)(*UARTLITE_RXREG);
}

Inline void
uart_ena_int(void){
  *UARTLITE_CTREG |= UARTLITE_ENABLE_INTR;
}

Inline void
uart_dis_int(void){
  *UARTLITE_CTREG &= ~(UARTLITE_ENABLE_INTR);
}
#endif  /* _MACRO_ONLY */

#endif /* _MICROBLAZE_H_*/

