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
#include "encoding.h"
#include <string.h>

/*
 *  SIL関数のマクロ定義
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_orb_mem(a, b)		sil_wrb_mem((a), sil_reb_mem(a) | (b))
#define sil_andb_mem(a, b)		sil_wrb_mem((a), sil_reb_mem(a) & ~(b))

#define BSP_LED_MASK            0x00f0
#define BSP_BUTTON2             0x0001

#define mtime_low_value()       sil_rew_mem((uint32_t *)(TADR_CLIC_BASE+TOFF_CLINT_MTIME))
#define mtime_high_value()      sil_rew_mem((uint32_t *)(TADR_CLIC_BASE+TOFF_CLINT_MTIME+4))


function_ptr_t plic_interrupt_handlers[1];


#ifdef REQ_PLIC_INIT
/*
 *  INIT PLIC
 */
void plic_init(void)
{
	unsigned long hart_id = read_csr(mhartid);
	uint32_t i, off, len;

	/*
	 *  Disable all interrupts
	 */
	off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE);
	len = (MAX_INT_NUM + 8) / 8;
	for(i = 0 ; i < len ; i += 4){
		sil_wrw_mem((uint32_t *)(TADR_PLIC_BASE + off + i), 0);
	}

	/*
	 *  Set all priorities to 0 (equal priority -- don't assume that these are reset).
	 */
	len = (MAX_INT_NUM + 1) * sizeof(uint32_t);
	for(i = 0 ; i < len ; i++){
		sil_wrb_mem((uint8_t *)(TADR_PLIC_BASE+TOFF_PLIC_PRIORITY+i), 0);
	}

	/*
	 *  Set the threshold to 0.
	 */
	off = (TOFF_PLIC_THRESHOLD + (hart_id * NUM_PLIC_THRESHOLD));
	sil_wrw_mem((uint32_t *)(TADR_PLIC_BASE+off), 0);

	for(i = 0; i < MAX_INT_NUM ; i++){
		plic_interrupt_handlers[i] = NULL;
	}
}
#endif

/*
 *  SETUP PRIORITY THRESHOLD
 */
void plic_set_threshold(unsigned long threshold)
{
	unsigned long hart_id = read_csr(mhartid);
	uint32_t off = TOFF_PLIC_THRESHOLD + (hart_id * NUM_PLIC_THRESHOLD);

	sil_wrw_mem((uint32_t *)(TADR_PLIC_BASE+off), threshold);
}

/*
 *  ENABLE PLIC INTERRUPT
 */
void plic_enable_interrupt(unsigned int source)
{
	unsigned long hart_id = read_csr(mhartid);
	uint32_t off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE) + (source >> 3);

	sil_orb_mem((uint8_t *)(TADR_PLIC_BASE+off), (1 << (source & 7)));
}

#if 0
/*
 *  DISABLE PLIC INTERRUPT
 */
void plic_disable_interrupt(unsigned int source)
{
	unsigned long hart_id = read_csr(mhartid);
	uint32_t off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE) + (source >> 3);

	sil_andb_mem((uint8_t *)(TADR_PLIC_BASE+off), (1 << (source & 7)));
}
#endif

/*
 *  SETUP PLIC INTERRUPT PRIORITY
 */
void plic_set_priority(unsigned int source, unsigned int priority)
{
	uint32_t off;
	if(priority < MAX_PRIORITY && source < MAX_INT_NUM) {
		off = TOFF_PLIC_PRIORITY + (source * sizeof(uint32_t));
		sil_wrb_mem((uint8_t *)(TADR_PLIC_BASE+off), priority);
	}
}

#if 0	/* ROI DEBUG */
/*
 *  GET CLINT TIMER VALUE
 */
unsigned long long get_timer_value(void)
{
	while (1) {
		uint32_t hi = mtime_high_value();
		uint32_t lo = mtime_low_value();
		if(hi == mtime_high_value())
			return ((uint64_t)hi << 32) | lo;
	}
}
#endif	/* ROI DEBUG */


/*
 *  PLIC Interrupt Handler
 */
void m_external_interrupt_handler(void)
{
	unsigned long hart_id = read_csr(mhartid);
	uint32_t int_num;
	int_num = sil_rew_mem((uint32_t *)(TADR_PLIC_BASE+TOFF_PLIC_CLAIM+(hart_id * NUM_PLIC_THRESHOLD)));

	if(int_num >= 1 && int_num < MAX_INT_NUM){
		if(int_num == IRQ_VECTOR_UART0)
			plic_interrupt_handlers[0]();
		else
			sys_printf("m_external_interrupt_handler none handler !\n");
	}
	else{
		sys_printf("m_external_interrupt_handler Illegal global interrupt !\n");
	}
	sil_wrw_mem((uint32_t *)(TADR_PLIC_BASE+TOFF_PLIC_CLAIM+(hart_id * NUM_PLIC_THRESHOLD)), int_num);
}

/*
 *  TRAP ENTRY
 */
void trap(void)
{
	long mcause = read_csr(mcause);

    /*
	 *  Handle External Machine-Level plic interrupt
	 */
	if((mcause & MCAUSE_INT) && ((mcause & MCAUSE_CAUSE) == IRQ_M_EXT)){
		m_external_interrupt_handler();
	}
	/*
	 *  Handle Machine-Level exception
	 */
	else if((mcause & MCAUSE_INT) == 0){
		sys_printf("handle_trap Exception occurs !\n");
	}
	else{
		sys_printf("handle_trap Illegal machine interrupt !\n");
	}
}

void BeforeInit(void)
{
	write_csr(mtvec, &trap_entry);
#ifdef REQ_PLIC_INIT
	plic_init();
#endif
	/*
	 *  LED OFF
	 */
	sil_wrw_mem((uint32_t *)(TADR_GPIO0_BASE+TOFF_GPIO_OUTPUT_EN), BSP_LED_MASK);
	sil_wrw_mem((uint32_t *)(TADR_GPIO0_BASE+TOFF_GPIO_OUTPUT), 0x00000000);
}

