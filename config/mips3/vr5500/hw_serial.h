/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2000-2003 by Industrial Technology Institute,
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
 */

/*
 *	���ꥢ��I/O�ǥХ�����SIO�˥ɥ饤�С�RTE-VR5500-CB�ѡ�
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include <tl16pir552.h>		/* siopcb, tl16pir552_openflag */

/*
 *  SIO�ɥ饤�Фν�����롼����
 */
#define	sio_initialize	tl16pir552_initialize

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ����ץ�
 */
Inline SIOPCB *
sio_opn_por(ID siopid, VP_INT exinf)
{
	SIOPCB	*siopcb;
	BOOL	openflag;
	UB	temp, mask = 0;

	/*
	 *  �����ץ󤷤��ݡ��Ȥ����뤫�� openflag ���ɤ�Ǥ�����
	 */
	openflag = tl16pir552_openflag();

	/*
	 *  �ǥХ�����¸�Υ����ץ������
	 */
	siopcb = tl16pir552_opn_por(siopid, exinf);

	/*
	 *  ���ꥢ��I/O����ߤγ���ߥ�٥�����ꤷ���ޥ����������롥
	 */
	if (!openflag) {
		temp = icu_read( (VP) ICU_INT0M );
#if TNUM_PORT >= 2
		switch(siopid) {
		case 1 :	/* �ݡ��ȣ�; ���ꥢ�룰 */
			all_set_ilv( INTNO_SERIAL0, &((IPM) IPM_SCC0) );
							/* ����ߥ�٥����� */
			mask = SERIAL0;
			break;
		case 2 :	/* �ݡ��ȣ�; ���ꥢ�룱 */
			all_set_ilv( INTNO_SERIAL1, &((IPM) IPM_SCC1) );
							/* ����ߥ�٥����� */
			mask = SERIAL1;
			break;
		}
#else	/* TNUM_PORT >= 2 */
		all_set_ilv( INTNO_SERIAL0, &((IPM) IPM_SCC0) );
							/* ����ߥ�٥����� */
		mask = SERIAL0;
#endif	/* TNUM_PORT >= 2 */
		icu_write( (VP) ICU_INT0M, temp | mask );
							/* �ޥ���������� */
	}

	return(siopcb);
}

/*
 *  ���ꥢ��I/O�ݡ��ȤΥ�������
 */
Inline void
sio_cls_por(SIOPCB *siopcb)
{
	UB	temp;

	/*
	 *  �ǥХ�����¸�Υ�������������
	 */
	tl16pir552_cls_por(siopcb);

	/*
	 *  ���ꥢ��I/O����ߤ�ޥ������롥(�ݡ��ȣ�������ξ��)
	 */
	if (!tl16pir552_openflag) {
		temp = icu_read( (VP) ICU_INT0M );	/* �ޥ���������� */
		icu_write( (VP) ICU_INT0M, temp & ~(SERIAL0 | SERIAL1) );
	}
}

/*
 *  SIO�γ���ߥϥ�ɥ�
 */
#define	scc0_handler	tl16pir552_uart0_isr
#if TNUM_PORT >= 2
#define scc1_handler	tl16pir552_uart1_isr
#endif /* TNUM_PORT */

/*
 *  ���ꥢ��I/O�ݡ��Ȥؤ�ʸ������
 */
#define	sio_snd_chr	tl16pir552_snd_chr

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����ʸ������
 */
#define	sio_rcv_chr	tl16pir552_rcv_chr

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ε���
 */
#define	sio_ena_cbr	tl16pir552_ena_cbr

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���Υ�����Хå��ζػ�
 */
#define	sio_dis_cbr	tl16pir552_dis_cbr

/*
 *  ���ꥢ��I/O�ݡ��Ȥ����������ǽ������Хå�
 */
#define	sio_ierdy_snd	tl16pir552_ierdy_snd

/*
 *  ���ꥢ��I/O�ݡ��Ȥ���μ������Υ�����Хå�
 */
#define	sio_ierdy_rcv	tl16pir552_ierdy_rcv

#endif /* _HW_SERIAL_H_ */