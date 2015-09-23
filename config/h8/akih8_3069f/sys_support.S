/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001-2003 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Kunihiko Ohnaka
 * 
 *  �嵭����Ԥϡ��ʲ��� (1)��(4) �ξ�狼��Free Software Foundation 
 *  �ˤ�äƸ�ɽ����Ƥ��� GNU General Public License �� Version 2 �˵�
 *  �Ҥ���Ƥ���������������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ�����
 *  ����Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����
 *  �ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ
 *  ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: sys_support.S,v 1.5 2003/12/11 07:00:10 honda Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include <tinet_sys_config.h>

	.h8300h

/*
 * ����ߥ٥��������
 */

	.section	.vectors
	.align 4

	.global _vectors
_vectors:

#ifdef AKI_MONITOR

	.long	_start				/*  0, 0x0000: reset	*/
	.long	0				/*  1, 0x0004: reserved	*/
	.long	0				/*  2, 0x0008: reserved	*/
	.long	0				/*  3, 0x000c: reserved	*/
	.long	0				/*  4, 0x0010: reserved	*/
	.long	0				/*  5, 0x0014: reserved	*/
	.long	0				/*  6, 0x0018: reserved	*/
	.long	0				/*  7, 0x001c: NMI	*/
	.long	0				/*  8, 0x0020: trap	*/
	.long	0				/*  9, 0x0024: trap	*/
	.long	0				/* 10, 0x0028: trap	*/
	.long	0				/* 11, 0x002c: trap	*/
	.long	0				/* 12, 0x0030: IRQ0	*/
	.long	0				/* 13, 0x0034: IRQ1	*/
	.long	0				/* 14, 0x0038: IRQ2	*/
	.long	0				/* 15, 0x003c: IRQ3	*/
	.long	0				/* 16, 0x0040: IRQ4	*/

#if defined(SUPPORT_ETHER)

	.long	_if_ed_handler_entry		/* 17, 0x0044: IRQ5	*/

#else	/* #if defined(SUPPORT_ETHER) */

	.long	0				/* 17, 0x0044: IRQ5	*/

#endif	/* #if defined(SUPPORT_ETHER) */

	.long	0				/* 18, 0x0048: reserved	*/
	.long	0				/* 19, 0x004c: reserved	*/
	.long	0				/* 20, 0x0050: WOVI	*/
	.long	0				/* 21, 0x0054: CMI	*/
	.long	0				/* 22, 0x0058: reserved	*/
	.long	0				/* 23, 0x005c: ADI	*/
	.long	_timer_handler_entry		/* 24, 0x0060: IMIA0	*/
	.long	0				/* 25, 0x0064: IMIB0	*/
	.long	0				/* 26, 0x0068: OVI0	*/
	.long	0				/* 27, 0x006c: reserved	*/
	.long	0				/* 28, 0x0070: IMIA1	*/
	.long	0				/* 29, 0x0074: IMIB1	*/
	.long	0				/* 30, 0x0078: OVI1	*/
	.long	0				/* 31, 0x007c: reserved	*/
	.long	0				/* 32, 0x0080: IMIA2	*/
	.long	0				/* 33, 0x0084: IMIB2	*/
	.long	0				/* 34, 0x0088: OVI2	*/
	.long	0				/* 35, 0x008c: reserved	*/
	.long	0				/* 36, 0x0090: CIMIA0	*/
	.long	0				/* 37, 0x0094: CIMIB0	*/
	.long	0				/* 38, 0x0098: CIMIA1/CIMIB1 */
	.long	0				/* 39, 0x009c: TOVI0/TOVI1   */
	.long	0				/* 40, 0x00a0: CIMIA2	*/
	.long	0				/* 41, 0x00a4: CIMIA2	*/
	.long	0				/* 42, 0x00a8: CIMIA3/CIMIB3 */
	.long	0				/* 43, 0x00ac: TOVI2/TOVI3   */
	.long	0				/* 44, 0x00b0: DEND0A	*/
	.long	0				/* 45, 0x00b4: DEND0B	*/
	.long	0				/* 46, 0x00b8: DEND1A	*/
	.long	0				/* 47, 0x00bc: DEND1B	*/
	.long	0				/* 48, 0x00c0: reserved	*/
	.long	0				/* 49, 0x00c4: reserved	*/
	.long	0				/* 50, 0x00c8: reserved	*/
	.long	0				/* 51, 0x00cc: reserved	*/

#if TNUM_PORT == 1

	.long	0				/* 52, 0x00d0: ERI0	*/
	.long	0				/* 53, 0x00d4: RXI0	*/
	.long	0				/* 54, 0x00d8: TXI0	*/
	.long	0				/* 55, 0x00ec: TEI0	*/

#ifdef H8_CFG_SCI_ERR_HANDLER

	.long	_sio_err_handler_entry		/* 56, 0x00e0: ERI1	*/

#else	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_sio_in_handler_entry		/* 56, 0x00e0: ERI1	*/

#endif	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_sio_in_handler_entry		/* 57, 0x00e4: RXI1	*/
	.long	_sio_out_handler_entry		/* 58, 0x00e8: TXI1	*/
	.long	0				/* 59, 0x00ec: TEI1	*/
	.long	0				/* 60, 0x00f0: ERI2	*/
	.long	0				/* 61, 0x00f4: RXI2	*/
	.long	0				/* 62, 0x00f8: TXI2	*/
	.long	0				/* 63, 0x00fc: TEI2	*/

#elif TNUM_PORT == 2	/* of #if TNUM_PORT == 1 */

#ifdef H8_CFG_SCI_ERR_HANDLER

	.long	_sio_err_handler_entry		/* 52, 0x00d0: ERI0	*/
	.long	_sio_in_handler_entry		/* 53, 0x00d4: RXI0	*/
	.long	_sio_out_handler_entry		/* 54, 0x00d8: TXI0	*/
	.long	0				/* 55, 0x00ec: TEI0	*/
	.long	_sio_err2_handler_entry		/* 56, 0x00e0: ERI1	*/
	.long	_sio_in2_handler_entry		/* 57, 0x00e4: RXI1	*/
	.long	_sio_out2_handler_entry		/* 58, 0x00e8: TXI1	*/

#else	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_sio_in_handler_entry		/* 52, 0x00d0: ERI0	*/
	.long	_sio_in_handler_entry		/* 53, 0x00d4: RXI0	*/
	.long	_sio_out_handler_entry		/* 54, 0x00d8: TXI0	*/
	.long	0				/* 55, 0x00ec: TEI0	*/
	.long	_sio_in2_handler_entry		/* 56, 0x00e0: ERI1	*/
	.long	_sio_in2_handler_entry		/* 57, 0x00e4: RXI1	*/
	.long	_sio_out2_handler_entry		/* 58, 0x00e8: TXI1	*/

#endif	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	0				/* 59, 0x00ec: TEI1	*/
	.long	0				/* 60, 0x00f0: ERI2	*/
	.long	0				/* 61, 0x00f4: RXI2	*/
	.long	0				/* 62, 0x00f8: TXI2	*/
	.long	0				/* 63, 0x00fc: TEI2	*/

#endif	/* of #if TNUM_PORT == 1 */

#else	/* of #ifdef AKI_MONITOR */

	.long	_start				/*  0, 0x0000: reset	*/
	.long	_no_reg_exception		/*  1, 0x0004: reserved	*/
	.long	_no_reg_exception		/*  2, 0x0008: reserved	*/
	.long	_no_reg_exception		/*  3, 0x000c: reserved	*/
	.long	_no_reg_exception		/*  4, 0x0010: reserved	*/
	.long	_no_reg_exception		/*  5, 0x0014: reserved	*/
	.long	_no_reg_exception		/*  6, 0x0018: reserved	*/
	.long	_no_reg_exception		/*  7, 0x001c: NMI	*/
	.long	_no_reg_exception		/*  8, 0x0020: trap	*/
	.long	_no_reg_exception		/*  9, 0x0024: trap	*/
	.long	_no_reg_exception		/* 10, 0x0028: trap	*/
	.long	_no_reg_exception		/* 11, 0x002c: trap	*/
	.long	_no_reg_exception		/* 12, 0x0030: IRQ0	*/
	.long	_no_reg_exception		/* 13, 0x0034: IRQ1	*/
	.long	_no_reg_exception		/* 14, 0x0038: IRQ2	*/
	.long	_no_reg_exception		/* 15, 0x003c: IRQ3	*/
	.long	_no_reg_exception		/* 16, 0x0040: IRQ4	*/

#if defined(SUPPORT_ETHER)

	.long	_if_ed_handler_entry		/* 17, 0x0044: IRQ5	*/

#else	/* #if defined(SUPPORT_ETHER) */

	.long	_no_reg_exception		/* 17, 0x0044: IRQ5	*/

#endif	/* #if defined(SUPPORT_ETHER) */

	.long	_no_reg_exception		/* 18, 0x0048: reserved	*/
	.long	_no_reg_exception		/* 19, 0x004c: reserved	*/
	.long	_no_reg_exception		/* 20, 0x0050: WOVI	*/
	.long	_no_reg_exception		/* 21, 0x0054: CMI	*/
	.long	_no_reg_exception		/* 22, 0x0058: reserved	*/
	.long	_no_reg_exception		/* 23, 0x005c: ADI	*/
	.long	_timer_handler_entry		/* 24, 0x0060: IMIA0	*/
	.long	_no_reg_exception		/* 25, 0x0064: IMIB0	*/
	.long	_no_reg_exception		/* 26, 0x0068: OVI0	*/
	.long	_no_reg_exception		/* 27, 0x006c: reserved	*/
	.long	_no_reg_exception		/* 28, 0x0070: IMIA1	*/
	.long	_no_reg_exception		/* 29, 0x0074: IMIB1	*/
	.long	_no_reg_exception		/* 30, 0x0078: OVI1	*/
	.long	_no_reg_exception		/* 31, 0x007c: reserved	*/
	.long	_no_reg_exception		/* 32, 0x0080: IMIA2	*/
	.long	_no_reg_exception		/* 33, 0x0084: IMIB2	*/
	.long	_no_reg_exception		/* 34, 0x0088: OVI2	*/
	.long	_no_reg_exception		/* 35, 0x008c: reserved	*/
	.long	_no_reg_exception		/* 36, 0x0090: CIMIA0	*/
	.long	_no_reg_exception		/* 37, 0x0094: CIMIB0	*/
	.long	_no_reg_exception		/* 38, 0x0098: CIMIA1/CIMIB1 */
	.long	_no_reg_exception		/* 39, 0x009c: TOVI0/TOVI1   */
	.long	_no_reg_exception		/* 40, 0x00a0: CIMIA2	*/
	.long	_no_reg_exception		/* 41, 0x00a4: CIMIA2	*/
	.long	_no_reg_exception		/* 42, 0x00a8: CIMIA3/CIMIB3 */
	.long	_no_reg_exception		/* 43, 0x00ac: TOVI2/TOVI3   */
	.long	_no_reg_exception		/* 44, 0x00b0: DEND0A	*/
	.long	_no_reg_exception		/* 45, 0x00b4: DEND0B	*/
	.long	_no_reg_exception		/* 46, 0x00b8: DEND1A	*/
	.long	_no_reg_exception		/* 47, 0x00bc: DEND1B	*/
	.long	_no_reg_exception		/* 48, 0x00c0: reserved	*/
	.long	_no_reg_exception		/* 49, 0x00c4: reserved	*/
	.long	_no_reg_exception		/* 50, 0x00c8: reserved	*/
	.long	_no_reg_exception		/* 51, 0x00cc: reserved	*/

#if TNUM_PORT == 1

	.long	_no_reg_exception		/* 52, 0x00d0: ERI0	*/
	.long	_no_reg_exception		/* 53, 0x00d4: RXI0	*/
	.long	_no_reg_exception		/* 54, 0x00d8: TXI0	*/
	.long	_no_reg_exception		/* 55, 0x00ec: TEI0	*/

#ifdef H8_CFG_SCI_ERR_HANDLER

	.long	_sio_err_handler_entry		/* 56, 0x00e0: ERI1	*/

#else	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_sio_in_handler_entry		/* 56, 0x00e0: ERI1	*/

#endif	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_sio_in_handler_entry		/* 57, 0x00e4: RXI1	*/
	.long	_sio_out_handler_entry		/* 58, 0x00e8: TXI1	*/
	.long	_no_reg_exception		/* 59, 0x00ec: TEI1	*/
	.long	_no_reg_exception		/* 60, 0x00f0: ERI2	*/
	.long	_no_reg_exception		/* 61, 0x00f4: RXI2	*/
	.long	_no_reg_exception		/* 62, 0x00f8: TXI2	*/
	.long	_no_reg_exception		/* 63, 0x00fc: TEI2	*/

#elif TNUM_PORT == 2	/* of #if TNUM_PORT == 1 */

#ifdef H8_CFG_SCI_ERR_HANDLER

	.long	_sio_err_handler_entry		/* 52, 0x00d0: ERI0	*/
	.long	_sio_in_handler_entry		/* 53, 0x00d4: RXI0	*/
	.long	_sio_out_handler_entry		/* 54, 0x00d8: TXI0	*/
	.long	_no_reg_exception		/* 55, 0x00ec: TEI0	*/
	.long	_sio_err2_handler_entry		/* 56, 0x00e0: ERI1	*/
	.long	_sio_in2_handler_entry		/* 57, 0x00e4: RXI1	*/
	.long	_sio_out2_handler_entry		/* 58, 0x00e8: TXI1	*/

#else	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_sio_in_handler_entry		/* 52, 0x00d0: ERI0	*/
	.long	_sio_in_handler_entry		/* 53, 0x00d4: RXI0	*/
	.long	_sio_out_handler_entry		/* 54, 0x00d8: TXI0	*/
	.long	_no_reg_exception		/* 55, 0x00ec: TEI0	*/
	.long	_sio_in2_handler_entry		/* 56, 0x00e0: ERI1	*/
	.long	_sio_in2_handler_entry		/* 57, 0x00e4: RXI1	*/
	.long	_sio_out2_handler_entry		/* 58, 0x00e8: TXI1	*/

#endif	/* of #ifdef H8_CFG_SCI_ERR_HANDLER */

	.long	_no_reg_exception		/* 59, 0x00ec: TEI1	*/
	.long	_no_reg_exception		/* 60, 0x00f0: ERI2	*/
	.long	_no_reg_exception		/* 61, 0x00f4: RXI2	*/
	.long	_no_reg_exception		/* 62, 0x00f8: TXI2	*/
	.long	_no_reg_exception		/* 63, 0x00fc: TEI2	*/

#endif	/* of #if TNUM_PORT == 1 */

#endif	/* of #ifdef AKI_MONITOR */

	.text
	.align 2

/*
 *  ��꡼���Ǥϡ����� DRAM ͭ���ˤ��롣
 */

	.global	_hardware_init_hook
_hardware_init_hook:

	/* CCR �� UI �ӥåȤ�����ߥޥ����ӥåȤȤ��ƻ��Ѥ��롣*/

	orc	#H8CCR_UI, ccr
	mov.l	#H8SYSCR, er0
	mov.b	@er0, r1h
	bclr	#H8SYSCR_UE_BIT, r1h
	mov.b	r1h, @er0

	/* �������ߤ�ͥ���٤�夲�롣*/

	mov.b	#0xff, r1l

	add.b	#(H8IPRA - H8SYSCR), r0l
	mov.b	r1l, @er0
	add.b	#(H8IPRB - H8IPRA), r0l
	mov.b	r1l, @er0

	/* ���� DRAM ��ͭ���ˤ��롣*/

	mov.l	#H8P1DDR, er0
	mov.b	r1l, @er0	/* enable A0  - A7  */

	add.b	#(H8P2DDR - H8P1DDR), r0l
	mov.b	r1l, @er0	/* enable A8  - A15 */

	add.b	#(H8P3DDR - H8P2DDR), r0l
	mov.b	r1l, @er0	/* enable D8  - D15 */

	mov.b	#H8P5DDR_A16, r0h
	mov.b	r0h, @H8P5DDR	/*  Enable A16		*/

	mov.b	#H8P8DDR_CS1|H8P8DDR_CS2, r0h
	mov.b	r0h, @H8P8DDR	/*  Enable CS1, CS2	*/

	mov.b	#H8PBDDR_UCAS, r0h
	mov.b	r0h, @H8PBDDR	/*  Enable UCAS		*/

		/*  DRAM: 2,048/32ms: 32ms / 2,048 = 15.625us	*/
		/*  20MHz / 8 / 39 = 1 / 15.6us			*/
	mov.b	#(32*(CPU_CLOCK/1000))/(8*2048), r0h
	mov.b	r0h, @H8RTCOR

	mov.b	#H8RTMCSR_CKS1, r0h
	mov.b	r0h, @H8RTMCSR	/*  clock / 8				*/

	mov.b	#H8DRCRB_RCYCE|H8DRCRB_MXC1, r0h
	mov.b	r0h, @H8DRCRB	/*  column address: 10 bit, A23 - A10	*/

	mov.b	#H8DRCRA_RDM|H8DRCRA_BE|H8DRCRA_DRAS0, r0h
	mov.b	r0h, @H8DRCRA

	mov.l	#CPU_CLOCK / 1000, er0	/*  Wait for DRAM stable	*/
_hw_init_hock_1:
	dec.l	#1, er0
	bne	_hw_init_hock_1

#if defined(SUPPORT_ETHER)

	/* NIC �γ����ߤ���Ĥ��롣*/

	mov.l	#H8IER, er0
	mov.b	@er0, r1l
	bset	#ED_IER_IP_BIT, r1l
	mov.b	r1l, @er0

#endif	/* #if defined(SUPPORT_ETHER) */

	rts

#if defined(SUPPORT_ETHER)

/*
 *  �ϡ��ɥ����������ߵ���
 */

/* NIC �ϡ��ɥ����������ߵ��� */

	.globl _if_ed_handler_enable_int
	.globl _ed_ena_inter
_if_ed_handler_enable_int:
_ed_ena_inter:
	mov.b	@ED_IPR, r0l
	bset	#ED_IPR_IP_BIT, r0l
	mov.b	r0l, @ED_IPR
	rts

/*
 *  �ϡ��ɥ����������߶ػ�
 */

/* NIC �ϡ��ɥ����������߶ػ� */

	.globl _if_ed_handler_disable_int
	.globl _ed_dis_inter
_if_ed_handler_disable_int:
_ed_dis_inter:
	mov.b	@ED_IPR, r0l
	bclr	#ED_IPR_IP_BIT, r0l
	mov.b	r0l, @ED_IPR
	rts

#endif	/* #if defined(SUPPORT_ETHER) */