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

#include "jsp_kernel.h"
#include "task.h"

#undef offsetof
#define offsetof(structure, field) \
                        ((INT) &(((structure *) 0)->field))

#define OFFSET_DEF(TYPE, FIELD)						\
	Asm("! BEGIN\n" #TYPE "_" #FIELD " = %0\n\t! END"		\
	  : /* no output */						\
	  : "g"(offsetof(TYPE, FIELD)))

#define OFFSET_DEF2(TYPE, FIELD, FIELDNAME)				\
	Asm("! BEGIN\n" #TYPE "_" #FIELDNAME " = %0\n\t! END"		\
	  : /* no output */						\
	  : "g"(offsetof(TYPE, FIELD)))

void
makeoffset()
{
        OFFSET_DEF2(TCB, tinib, tinib);
        OFFSET_DEF2(TINIB, task, task);
        OFFSET_DEF2(TINIB, exinf, exinf);
	OFFSET_DEF(TCB, texptn);

	OFFSET_DEF2(TCB, tskctxb.sp, sp);
        OFFSET_DEF2(TCB, tskctxb.pc, pc);

	OFFSET_DEF(INT_TABLE, intmask);

	/*
	 *  ��¤��ICU_IPM��Υ��ե��å� 
	 *    ����ߥ���ȥ������¸�ʤΤǡ������ƥ��¸�������
	 */
#ifdef OFFSET_DEF_ICU_IPM
	OFFSET_DEF_ICU_IPM;
#endif /* OFFSET_DEF_ICU_IPM */

}

asm("! BIT_REF");
UW	ref_4 = 0x12345678;
UH	ref_2 = 0x1234;
UB	ref_1 = 0x12;
asm("! END");

asm("! BIT_BB");
TCB	TCB_enatex = {
		{ NULL, NULL }, NULL, 0, 0,
		FALSE, FALSE, TRUE,
		0, NULL, { NULL, NULL }
	};
asm("! END");