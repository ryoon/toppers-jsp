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

#define _MACRO_ONLY

#include <t_config.h>	/* CCR_I, MAX_IPM */

/*
 *  H8S�ѥ������ȥ��åץ⥸�塼��
 *  �����ͥ�ϥ��ץꥱ�������ȥ�󥯤��ƻ��Ѥ���
 *  �����ʤΤ�JSP�����ͥ�Τ߻��Ѥ��롣
 */
	.h8300s

	.text
	.align 2

	.global	start
	.global bss_clear	/* hardware_init_hook �������ꥢ�ɥ쥹 */

start:

	/*
	 *  ��������Ω�Υ����å���STACKTOP�˽��������
	 *  ��STACKTOP�ϡ�sys_config.h�������
	 *  ���ϡ��ɥ������ޥ˥奢�� ��4.2.4 �ꥻ�å�ľ��γ���ߡ�(P90)���
	 *    �ץ���������Ƭ̿��� SP �ν����̿��Ǥʤ���Фʤ�ʤ���
	 */
	mov.l	#STACKTOP, sp

	/*
	 *  CCR �ν����
	 *  �����������⡼��0 �ˤ��������߶ػ� (I = 1 ; CCR)
	 *    ( _hardware_init_hook ��ǳ��������⡼�ɤ��ѹ�����ޤǤϡ�
	 *     ���������⡼�ɤ�0�Ǥ��롣)
	 */
	ldc.b	#CCR_I, ccr

	/*
	 *  EXR �ν����
	 *  �����������⡼��2 �ˤ��������߶ػ� (I2, I1, I0 = 1 ; CCR)
	 */
	ldc.b	#MAX_IPM, exr

	/*
	 *  hardware_init_hook ��ƤӽФ���(0�Ǥʤ����)
	 *  ���������åȥϡ��ɥ������˰�¸��������������������ϡ�
	 *    hardware_init_hook �Ȥ����ؿ����Ѱդ��롥
	 *      ���ROM���κݡ�RAM����Ѳ�ǽ�ˤ��뤿��ΥХ�����ȥ�����ν����
	 *    ����ϡ�sys_support.S ��� hardware_init_hook ��������Ƥ��롥
	 */
hardware_init:
	mov.l	@_hardware_init_hook_k, er0
	or.l	er0, er0
	beq	bss_clear		/* er0 = 0 �ʤ� bss_clear �� */
	jmp	@er0

	/*
	 * bss���������򥯥ꥢ
	 */
bss_clear:
	mov.l	#__bss_start, er0
	mov.l	#_end, er1
	sub.l	er2, er2
loop_bss_clear:
	mov.l	er2, @er0
	add.l	#4, er0
	cmp.l	er1, er0
	blo	loop_bss_clear		/* er1 > er0 �ޤǥ����� */

	/*
	 *  data�������������������ROM���б���
	 *
	 *  __idata_start ���� __idata_end �ޤǤ�__data_start �ʹߤ˥��ԡ�����
	 */
data_sec_copy:
	mov.l	#__idata_start, er0
	mov.l	#__idata_end, er1
	mov.l	#__data_start, er2
loop_data_sec_copy:
	mov.l	@er0+, er4		/* *er2++ = *er0 ++; */
	mov.l	er4, @er2
	add.l	#4, er2
	cmp.l	er1, er0
	blo	loop_data_sec_copy	/* er1 > er0 �ޤǥ����� */

	/*
	 *  software_init_hook ��ƤӽФ���0 �Ǥʤ�����
	 *
	 *  ���եȥ������Ķ����ä˥饤�֥��ˤ˰�¸����ɬ�פʽ��������������
	 *  ���ϡ�software_init_hook �Ȥ����ؿ����Ѱդ��롥
	 */
software_init:
	mov.l	@_software_init_hook_k, er0
	or.l	er0, er0
	beq	start_kernel
	jsr	@er0

	/*
	 *  �����ͥ��ư����
	 */
start_kernel:
	jmp	@_kernel_start

	.align 4
_hardware_init_hook_k:
	.long   _hardware_init_hook
_software_init_hook_k:
	.long   _software_init_hook