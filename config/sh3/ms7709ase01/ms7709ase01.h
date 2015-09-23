/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: ms7709ase01.h,v 1.5 2001/10/20 15:47:34 honda Exp $
 */

#ifndef _MS7709ASE01_ 
#define _MS7709ASE01_ 


#include "sh3.h"

/*
 *  MS7709ASE01のハードウェア資源の定義
 */

/*
 *  FPGA1(U17)により生成される割込みのINTEVTコード
 */
 
#define SLOT_IRQ8  0x200
#define KBC0_IRQ0  0x220
#define PCIC_IRQ2  0x240
#define UART_IRQ1  0x260
#define SLOT_IRQ6  0x280
#define KBC_IRQ1   0x2A0
#define PCIC_IRQ1  0x2C0
#define UART_IRQ0  0x2E0
#define SLOT_IRQ4  0x300
#define SLOT_IRQ3  0x320
#define PRN_IRQ    0x340
#define SLOT_IRQ2  0x360
#define LAN_IRQ    0x380
#define FDC_IRQ    0x3A0
#define PCIC_IRQ0  0x3C0


/* SMSC Super I/O の設定 */ 

#define BASE_ADDR 0xB0400000

#define SMSC_CONFIG_PORT ((HIOREG *)(BASE_ADDR + 0x7E0))
#define SMSC_INDEX_PORT  ((HIOREG *)(BASE_ADDR + 0x7E0))
#define SMSC_DATA_PORT   ((HIOREG *)(BASE_ADDR + 0x7E2))

/* Super I/O 上のSCI0 */
#define SMSC_SCI0_BASE_ADDR  0x03f8

#define SMSC_SCI0_RBR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x0))
#define SMSC_SCI0_THR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x0))
#define SMSC_SCI0_DLL ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x0))
#define SMSC_SCI0_IER ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x2))
#define SMSC_SCI0_DLM ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x2))
#define SMSC_SCI0_IIR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x4))
#define SMSC_SCI0_FCR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x4))  
#define SMSC_SCI0_LCR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x6))
#define SMSC_SCI0_MCR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0x8))
#define SMSC_SCI0_LSR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0xA))
#define SMSC_SCI0_MSR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0xC))
#define SMSC_SCI0_SCR ((HIOREG *)(BASE_ADDR + SMSC_SCI0_BASE_ADDR * 2 + 0xE))

Inline void
smsc_config_write(word index, word data)
{
    *SMSC_INDEX_PORT = (index << 8);
    *SMSC_DATA_PORT  = (data << 8);
}

Inline word
smsc_config_read(word index)
{
    *SMSC_INDEX_PORT = (index << 8);
    return(*SMSC_DATA_PORT >> 8);
}



#ifdef PARTNER_J

#define PARTNER_OUT   0xfffffe00
#define PARTNER_IN    0xfffffe01

/*
 *  PARTNERへの文字の出力
 */
void partner_putc(char c);

#endif /* PARTNER_J */


#endif /*  _MS7709ASE01_ */



