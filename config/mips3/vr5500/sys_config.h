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

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  �����ͥ����������̾�Υ�͡���
 */
#include <sys_rename.h>

/*
 *  ����ߥ���ȥ����顦�ܡ��ɰ�¸�Υ��󥯥롼�ɥե�������ɤ߹���
 */
#include <pic_icu.h>	/* ����ߥ���ȥ�����ط��ξ����<CPU>���Ϥ��� */
#include <vr5500.h>	/* NOP_FOR_CP0_HAZARD�ξ����cpu_support.S���Ϥ��� */

/*
 *  vgxet_tim �򥵥ݡ��Ȥ��뤫�ɤ��������
 */
#define	SUPPORT_VXGET_TIM

/*
 *  ���ꥢ��ݡ��ȿ������
 */
#ifndef GDB_STUB
#define TNUM_PORT	2u	/* ���ݡ��Ȥ��륷�ꥢ��ݡ��Ȥο� */
#else	/*  GDB_STUB  */
#define TNUM_PORT	1u	/* ���ݡ��Ȥ��륷�ꥢ��ݡ��Ȥο� */
				/* GDB_STUB �����Ѥ���Ȥ��ϡ�SCC1�� GDB_STUB ��
				   ���Ѥ��뤿��ˡ�SCC0�Τߤ������ѤǤ��ʤ��� */
#endif	/*  GDB_STUB  */


/*
 *  �����ƥॿ�����˴ؤ������
 */
#define	LOGTASK_PORTID	1u	/* �����ƥ��������Ϥ��륷�ꥢ��ݡ����ֹ� */

/*
 *  ���������Ԥ��Τ�������
 */
#define	SIL_DLY_TIM1	2865
#define	SIL_DLY_TIM2	398

/*
 *  ����ߥޥ����ν���� (MIPS3�����Τ⥿�����åȥ����ƥ��¸�ʤΤǡ����������)
 */

/*  MIPS3�����δط� */
/*  RTE-VR5500-CB�Ǥ�2�ܤγ����������³����Ƥ��롣 */
#define INIT_CORE_IPM	( Cause_Int0 | Cause_Int0 )

/*  ����ߥ���ȥ�����ط�  */

#if TNUM_PORT >= 2
#define INIT_INT0M	( SERIAL1 | SERIAL0 | TIMER0 )
#else	/* TNUM_PORT >= 2 */
#define INIT_INT0M	( SERIAL0 | TIMER0 )
#endif	/* TNUM_PORT >= 2 */

#define INIT_INT1M	0u

#ifndef _MACRO_ONLY

/*
 *  �������åȥ����ƥ��¸�ν����
 */
extern void	sys_initialize(void);

/*
 *  �������åȥ����ƥ�ν�λ
 *
 *  �����ƥ��λ������˻Ȥ���ROM��˥��ƽФ��Ǽ¸����뤳�Ȥ����ꤷ
 *  �Ƥ��롥
 */
extern void	sys_exit(void);

/*
 *  �������åȥ����ƥ��ʸ������
 *
 *  �����ƥ�����٥��ʸ�����ϥ롼����ROM��˥��ƽФ��Ǽ¸����뤳
 *  �Ȥ����ꤷ�Ƥ��롥
 */

extern void	sys_putc(char c) throw();

#endif /* _MACRO_ONLY */

/*============================================================================*/
/* �ʲ��ϡ����̥ɥ�����Ȥ�̵���ȼ������ */

/*
 *  ���ơ������쥸�����ν����
 */
#define INIT_SR		(SR_BEV | INIT_CORE_IPM)
	/*
	 *  BEV = 1���㳰�٥�����kseg0�������Ȥ�����
	 *  IM = 0��CPU���å����� (����߶ػ�)
	 */

/*
 *  ����ե����쥸��������������ѥޥ���
 */
#define INIT_CONFIG_MASK	0x0

/*
 *  �����ƥॿ���������Ѥ���饤�֥��˴ؤ������
 */
#define NEWLIB			/* newlib ���Ѥ��� */

/*
 *  ���ꥢ�륳��ȥ�����Υܡ��졼�Ȥ����� (ʬ��������ǡ�����[bps]�ǻ���)
 */
#define DEVIDE_RATIO	9600u	/*  9600bps������  */

#endif /* _SYS_CONFIG_H_ */