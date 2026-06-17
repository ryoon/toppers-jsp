/*
 *  rommon Simple Monitor
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2006-2015 by GJ Business Division
 *                   RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2016-2022 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: $
 */

#include "sys_defs.h"
#include <sil.h>
#include "comdefs.h"
#include "serial.h"

/*
 *  SIL関数のマクロ定義
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))

#define UART_A_SAMPLE_PER_BAUD  8

#define STOP_1                  0
#define STOP_2                  1
#define PARITY_NONE             0
#define PARITY_EVEN             1
#define PARITY_ODD              2
#define CONFIG_VALUE            ((STOP_1<<16) | (PARITY_NONE<<8) | (8-1))


static const uint32_t sio_base_table[] = {
	TADR_UART0_BASE
};

/*
 *  UART0 Interrupt Handler
 */
void uart_0_handler(void)
{
	uint32_t base = sio_base_table[0];
	uint32_t status = sil_rew_mem((uint32_t *)(base+TOFF_UART_STATUS));
	if((status & UART_RXFIFO_NOTEMPTY_INTERRUPT) != 0)
		ComInterRX(0);
#ifndef NOINT_UARTTX
	if((status & UART_TXFIFO_EMPTY_INTERRUPT) != 0 && (status & UART_WRITEAVAILABILITY) != 0){
		ComInterTX(0);
	}
#endif
};


/*
 *  Get RX Data
 *  Input: id: Serial Channel #
 *  output: c: Rx Data
 */
unsigned short GetRxData(int id)
{
	uint32_t base = sio_base_table[id];
	return sil_rew_mem((uint32_t *)(base+TOFF_UART_DATA)) | CHKRXDATA;
}

#ifndef NOINT_UARTTX
/*
 *  Enable Tx interrupt
 *  Input: id: Serial Channel #
 */
void EnableITX(int id)
{
	uint32_t base = sio_base_table[id];
	sil_orw_mem((uint32_t *)(base+TOFF_UART_STATUS), UART_TX_EMPTY_INTERRUPT_ENABLE);
}

/*
 *  Disable Tx interrupt
 *  Input: id: Serial Channel #
 */
void DisableITX(int id)
{
	uint32_t base = sio_base_table[id];
	sil_andw_mem((uint32_t *)(base+TOFF_UART_STATUS), UART_TX_EMPTY_INTERRUPT_ENABLE);
}
#endif	/* NOINT_UARTTX */

/*
 *  Send Tx in interrupt
 *  Input: id: Serial Channel #
 *         c : Send charactor
 */
void SendTX(int id, char c)
{
	uint32_t base = sio_base_table[id];
	sil_wrw_mem((uint32_t *)(base+TOFF_UART_DATA), c);
}

/*
 * Send Tx wait
 * Inputs: id: Serial Channel #
 */
void SendTXWait(int id)
{
	uint32_t base = sio_base_table[id];
	while((sil_rew_mem((uint32_t *)(base+TOFF_UART_STATUS)) & UART_WRITEAVAILABILITY) == 0);
}

/*
 *  none-interrupt putc().
 */
void sys_putc(char c)
{
#ifdef NOINT_UARTTX
	SendTXWait(DEF_MON_SERIAL);
	SendTX(DEF_MON_SERIAL, c);
	if(c == '\n'){
		SendTXWait(DEF_MON_SERIAL);
		SendTX(DEF_MON_SERIAL, '\r');
	}
#else
	SendChar(DEF_MON_SERIAL, c);
	if(c == '\n'){
		SendChar(DEF_MON_SERIAL, '\r');
	}
#endif
}

/*
 *  none-interrupt print the string.
 */
int sys_printf(const char *s)
{
	while(*s > 0)
		sys_putc((char)*s++);
	return 0;
}

/*
 *  interrupt putc().
 */
void apl_putchar(char c)
{
	SendChar(DEF_MON_SERIAL, c);
	if(c == '\n'){
		SendChar(DEF_MON_SERIAL, '\r');
	}
}

/*
 *  interrupt puthex().
 */
void apl_puthex(unsigned int c, unsigned int val)
{
	while(c-- > 0){
		uint32_t v = (val >> (c * 4)) & 0xf;
		if(v >= 10)
			v += 'A' - 10;
		else
			v += '0';
		apl_putchar(v);
	}
}

/*
 *  interrupt print the string.
 */
int apl_printf(const char *s)
{
	while(*s > 0)
		apl_putchar((char)*s++);
	return 0;
}

void InitSIO(int id)
{
	uint32_t base = sio_base_table[id];
	uint32_t divider = SYSCLK_FREQUENCY/(DEFAULT_SPEED*UART_A_SAMPLE_PER_BAUD)-1;

	/*
	 *  Configurate UART0 DEFAULT_SPEED Baud Rate
	 */
	sil_wrw_mem((uint32_t *)(base+TOFF_UART_CLOCK_DIVIDER), divider);
	sil_wrw_mem((uint32_t *)(base+TOFF_UART_FRAME_CONFIG), CONFIG_VALUE);
#if 0

	/*
	 * Configurate Interrupt
	 */
	clear_csr(mie, MIP_MEIP);
	clear_csr(mie, MIP_MTIP);

	plic_init();
#endif

	plic_set_vector(IRQ_VECTOR_UART0, uart_0_handler);
	plic_set_threshold(0);
	plic_enable_interrupt(IRQ_VECTOR_UART0);
	plic_set_priority(IRQ_VECTOR_UART0, 1);
	sil_orw_mem((uint32_t *)(base+TOFF_UART_STATUS), UART_RX_NOTEMPTY_INTERRUPT_ENABLE);

	/*
	 *  Enable the Machine-Interrupt
	 */
	set_csr(mie, MIP_MEIP);

	/*
	 *  Enable PLIC-Interrupt
	 */
	Enable();
}

