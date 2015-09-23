/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2005 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: h8_sil.c,v 1.6 2005/11/07 01:49:53 honda Exp $
 */

/*
 * �����ƥ।�󥿥ե������쥤����ɲ���ʬ��
 *����I/O�ݡ��ȤΥǡ������ǥ��쥯����󡦥쥸����DDR�ؤΥ�������
 *����H8��DDR�Ͻ񤭹������ѤǤ��ꡢ���ΤޤޤǤϽ�˾�ΥӥåȤ�����
 *�����ѹ����뤳�Ȥ��Ǥ��ʤ�����bset,bclr̿��Ǥ�����Բġ�
 *�������Τ��ᡢ�ܼ����Ǥϡ������˥ƥ�ݥ����Ѱդ��ơ�DDR��
 *���������ͤ��ݻ�������ˡ��ΤäƤ��롣
 *����
 *�����ʤ����ݡ���7���������Ѥ�DDR�쥸�������ʤ����ᡢ��ά���Ƥ��롣
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include <h8_sil.h>

#define	DUMMY	0

/*  �����Υƥ�ݥ���ΰ�  */
static UB ddr_tmp[] = {
        /* 1         2         3         4         5      */
        H8P1DDR0, H8P2DDR0, H8P3DDR0, H8P4DDR0, H8P5DDR0, 
        /* 6         8         9         A         B      */
        H8P6DDR0, H8P8DDR0, H8P9DDR0, H8PADDR0, H8PBDDR0
};

/*  �ƥݡ��ȤΥ��ɥ쥹  */
static const VP ddr_adr[] = {
        (VP)H8P1DDR, (VP)H8P2DDR, (VP)H8P3DDR, (VP)H8P4DDR, (VP)H8P5DDR, 
        (VP)H8P6DDR, (VP)H8P8DDR, (VP)H8P9DDR, (VP)H8PADDR, (VP)H8PBDDR
};


/*
 *  ���������Ԥ�
 *  ���ץ��ȥ����������sil.h��ˤ���
 */
void sil_dly_nse(UINT dlytim) {
	/* er0 = dlytim */
	Asm("		sub.l	%0, er0" :: "g"(SIL_DLY_TIM1) );
	Asm("		ble	sil_dly_nse_2");
				/* dlytim < SIL_DLY_TIM1 �ʤ�롼�׽�λ */
	Asm("	sil_dly_nse_1:");
	Asm("		sub.l	%0, er0" :: "g"(SIL_DLY_TIM2) );
	Asm("		bgt	sil_dly_nse_1");
				/* dlytim > SIL_DLY_TIM2 �ʤ�롼��³�� */
	Asm("	sil_dly_nse_2:");
}

/*
 *      DDR���ɤ߽Ф�
 */
UB sil_reb_ddr(UINT port)
{
	assert((IO_PORT1 <= port) && (port <= IO_PORTB));
        return ddr_tmp[port];
}

/*
 *      DDR�ν񤭹���
 */
void sil_wrb_ddr(UINT port, UB data)
{
	assert((IO_PORT1 <= port) && (port <= IO_PORTB));
        ddr_tmp[port] = data;
        sil_wrb_mem(ddr_adr[port], (VB)data);
}

/*
 *      DDR��AND�黻
 */
void sil_anb_ddr(UINT port, UB data)
{
        UB ddr = sil_reb_ddr(port);
        
        ddr &= data;
        sil_wrb_ddr(port, ddr);
}

/*
 *      DDR��OR�黻
 */
void sil_orb_ddr(UINT port, UB data)
{
        UB ddr = sil_reb_ddr(port);

        ddr |= data;
        sil_wrb_ddr(port, ddr);
}
