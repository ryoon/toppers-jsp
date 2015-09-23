/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
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

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

/*
 *  �ץ����å��˰�¸���������H8S�ѡ�
 *
 *  ���Υ��󥯥롼�ɥե�����ϡ�kernel.h �� sil.h �ǥ��󥯥롼�ɤ���롥
 *  ¾�Υե����뤫��ľ�ܥ��󥯥롼�ɤ��뤳�ȤϤʤ������Υե�����򥤥�
 *  ���롼�ɤ������ˡ�t_stddef.h �� itron.h �����󥯥롼�ɤ����Τǡ�
 *  �����˰�¸���Ƥ�褤��
 */

#define H8S

/* �����ͥ뵯ư���Υ�å����� */
#define COPYRIGHT_CPU \
"Copyright (C) 2001-2005 by Industrial Technology Institute,\n" \
"                            Miyagi Prefectural Government, JAPAN\n" \
"Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering,\n" \
"                 Tomakomai National College of Technology, JAPAN\n"

#ifndef _MACRO_ONLY

/*
 *  ����ߡ�CPU�㳰�ϥ�ɥ��ֹ�η�
 */
typedef	UINT	INHNO;			/* ����ߥϥ�ɥ��ֹ� */
typedef	UINT	EXCNO;			/* CPU�㳰�ϥ�ɥ��ֹ� */

/*
 *  ����ߥޥ����η��ȳ���ߥޥ������ѹ�������
 */

/* ����ߥޥ����η� */
typedef	UB	IPM;			/* ����ߥޥ��� */

/* ����ߥޥ�������Ѥδؿ� */
extern ER	chg_ipm(IPM ipm) throw();	/* ����ߥޥ������ѹ� */
extern ER	get_ipm(IPM *p_ipm) throw();	/* ����ߥޥ����μ��� */

#if 0		/*  �������Ǥϥ��ݡ��Ȥ��Ƥ��ʤ�  */
/*
 *  INTNO���ȳ���ߤζػ�/����
 */
typedef	UINT	INTNO;			/* ������ֹ� */

extern ER	dis_int(INTNO intno) throw();
extern ER	ena_int(INTNO intno) throw();
#endif


/*
 *  ����ߥ��å����֤�����
 */
#define	SIL_PRE_LOC	IPM _intmask_ = current_intmask()
#define	SIL_LOC_INT()	_disint_()
#define	SIL_UNL_INT()	set_intmask(_intmask_)

/*
 *  �ץ����å��Υ���ǥ�����
 */
#define	SIL_ENDIAN	SIL_ENDIAN_BIG		/* �ӥå�����ǥ����� */

/*
 * ���������Ԥ�
 * �������ջ��ࡧ
 * ������ɸ��Ǥ�dlytim��UINT������16�ӥåȤ����ʤ��Τǡ�
 * ������sil_dly_nse_long()�Ǥϰ�����UW�����ѹ����Ƥ��롣
 * ������sil_dly_nse()��������sil_dly_nse_long()��ƤӽФ���
 * ������
 * ������sil_dly_nse()�Υץ��ȥ����������jsp/include/sil.h�ˤ��롣
 */
extern void sil_dly_nse_long(UW dlytim) throw();

/*
 *��I/O�ݡ��ȤΥǡ������ǥ��쥯����󡦥쥸����DDR�ؤΥ�������
 */

/*  �ݡ����ֹ�����  */
typedef enum  {
	IO_PORT1,
	IO_PORT2,
	IO_PORT3,
	IO_PORT5,	/*  �ݡ��ȣ���DDR�Ϥʤ�  */
	IO_PORT6,
	IO_PORT7,
	IO_PORT8,
	IO_PORT9,
	IO_PORTA,
	IO_PORTB,
	IO_PORTC,
	IO_PORTD,
	IO_PORTE,
	IO_PORTF,
	IO_PORTG
} IO_PORT_ID;

extern UB sil_reb_ddr(IO_PORT_ID port) throw();
extern void sil_wrb_ddr(IO_PORT_ID port, UB data) throw();
extern void sil_anb_ddr(IO_PORT_ID port, UB data) throw();
extern void sil_orb_ddr(IO_PORT_ID port, UB data) throw();


/*============================================================================*/
/*  �ǥХå��ѥ�����  */

#ifdef TEST_CPU_INSN
extern void test_cpu_insn(void);
#endif /* TEST_CPU_INSN */

#ifdef TEST_CPU_CONFIG
extern void test_cpu_config(void);
#endif /* TEST_CPU_CONFIG */

#ifdef TEST_H8S_SIL
extern void test_h8s_sil(void);
#endif /* TEST_H8S_SIL */

/*============================================================================*/



#endif /* _MACRO_ONLY */

#endif /* _CPU_DEFS_H_ */