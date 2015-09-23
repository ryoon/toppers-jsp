/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ��ξ������������˸¤ꡤ�ܥ��եȥ��������ܥ�
 *  �եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ�����
 *  �ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ�������Х��ʥꥳ���ɤη��ޤ��ϵ�����Ȥ߹����������
 *      �Ѥ�����ˤϡ����Τ����줫�ξ������������ȡ�
 *    (a) ���Ѥ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭������
 *        ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) ���Ѥη��֤��̤�������ˡ�ˤ�äơ��嵭����Ԥ���𤹤�
 *        ���ȡ�
 *  (3) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥϡ�
 *  �ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����ޤ�ơ������ʤ��ݾڤ�Ԥ�
 *  �ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū����������
 *  ���ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: cyclic.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	�����ϥ�ɥ鵡ǽ
 */

#include "jsp_kernel.h"
#include "check.h"
#include "cyclic.h"

/*
 *  �����ϥ�ɥ�ID�κ����͡�kernel_cfg.c��
 */
extern const ID	tmax_cycid;

/*
 *  �����ϥ�ɥ������֥��å��Υ��ꥢ��kernel_cfg.c��
 */
extern const CYCINIB	cycinib_table[];

/*
 *  �����ϥ�ɥ�����֥��å��Υ��ꥢ��kernel_cfg.c��
 */
extern CYCCB	cyccb_table[];

/*
 *  �����ϥ�ɥ�ID��������ϥ�ɥ�����֥��å�����Ф�����Υޥ���
 */
#define INDEX_CYC(cycid)	((cycid) - TMIN_CYCID)
#define get_cyccb(cycid)	(&(cyccb_table[INDEX_CYC(cycid)]))

/*
 *  �����ϥ�ɥ鵯ư�Τ���Υ����।�٥�ȥ֥��å�����Ͽ
 */
static void
tmevtb_enqueue_cyc(CYCCB *cyccb, EVTTIM evttim)
{
	static void	call_cychdr(CYCCB *cyccb);	/* �������� */

	tmevtb_enqueue_evttim(&(cyccb->tmevtb), evttim,
				(CBACK) call_cychdr, (VP) cyccb);
	cyccb->evttim = evttim;
}

/*
 *  �����ϥ�ɥ鵡ǽ�ν����
 */
void
cyclic_initialize()
{
	INT	i;
	CYCCB	*cyccb;

	for (cyccb = cyccb_table, i = 0; i < tmax_cycid; cyccb++, i++) {
		cyccb->cycinib = &(cycinib_table[i]);
		if ((cyccb->cycinib->cycatr & TA_STA) != 0) {
			cyccb->cycsta = TRUE;
			tmevtb_enqueue_cyc(cyccb, cyccb->cycinib->cycphs);
		}
		else {
			cyccb->cycsta = FALSE;
		}
	}
}

/*
 *  �����ϥ�ɥ��ư���
 */
SYSCALL ER
sta_cyc(ID cycid)
{
	CYCCB	*cyccb;

	CHECK_TSKCTX_UNL();
	CHECK_CYCID(cycid);
	cyccb = get_cyccb(cycid);

	t_lock_cpu();
	if (cyccb->cycsta) {
		tmevtb_dequeue(&(cyccb->tmevtb));
	}
	else {
		cyccb->cycsta = TRUE;
	}
	tmevtb_enqueue_cyc(cyccb, base_time + cyccb->cycinib->cyctim);
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  �����ϥ�ɥ��ư�����
 */
SYSCALL ER
stp_cyc(ID cycid)
{
	CYCCB	*cyccb;

	CHECK_TSKCTX_UNL();
	CHECK_CYCID(cycid);
	cyccb = get_cyccb(cycid);

	t_lock_cpu();
	if (cyccb->cycsta) {
		cyccb->cycsta = FALSE;
		tmevtb_dequeue(&(cyccb->tmevtb));
	}
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  �����ϥ�ɥ鵯ư�롼����
 */
static void
call_cychdr(CYCCB *cyccb)
{
	/*
	 *  ����ε�ư�Τ���Υ����।�٥�ȥ֥��å�����Ͽ���롥
	 */
	tmevtb_enqueue_cyc(cyccb, cyccb->evttim + cyccb->cycinib->cyctim);

	/*
	 *  �����ϥ�ɥ��CPU���å�������֤ǸƤӽФ���
	 */
	i_unlock_cpu();
	(*cyccb->cycinib->cychdr)(cyccb->cycinib->exinf);
	i_lock_cpu();
}