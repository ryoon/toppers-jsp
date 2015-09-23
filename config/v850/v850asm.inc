/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  �嵭����Ԥϡ�Free Software Foundation �ˤ�äƸ�ɽ����Ƥ��� 
 *  GNU General Public License �� Version 2 �˵��Ҥ���Ƥ����狼����
 *  ���ξ��Τ����줫�����������˸¤ꡤ�ܥ��եȥ��������ܥ��եȥ���
 *  ������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ�������ѡ������ۡʰʲ���
 *  ���ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ�����������Ѳ�ǽ�ʥХ��ʥꥳ���ɡʥ���������֥륪��
 *      �������ȥե������饤�֥��ʤɡˤη������Ѥ�����ˤϡ�����
 *      ��ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ����
 *      �������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ�������������Բ�ǽ�ʥХ��ʥꥳ���ɤη��ޤ��ϵ������
 *      �߹�����������Ѥ�����ˤϡ����Τ����줫�ξ������������ȡ�
 *    (a) ���Ѥ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭������
 *        ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) ���Ѥη��֤��̤�������ˡ�ˤ�äơ��嵭����Ԥ���𤹤�
 *        ���ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥϡ�
 *  �ܥ��եȥ������˴ؤ��ơ�����Ŭ�Ѳ�ǽ����ޤ�ơ������ʤ��ݾڤ�Ԥ�
 *  �ʤ����ޤ����ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū����������
 *  ���ʤ�»���˴ؤ��Ƥ⡤������Ǥ�����ʤ���
 * 
 *  @(#) $Id: v850asm.inc,v 1.3 2001/11/15 13:18:32 takayuki Exp $
 */

/*
 *  V850������֥��Ѥ�����������ե�����
 */


	/*
	 * ��٥������ Lea (Load Effective Address)
	 */
	 
	.macro	Lea name, destination
	movhi	hi(\name), r0, \destination
	movea	lo(\name), \destination, \destination
	.endm


	/*
	 * �ؿ���� Function
	 *
	 *   �ؿ�̾�Ȥ��ƻȤ���٥���դ� �������Ԥ�
	 */
	 
	.macro	Function name
	.text
	.align	4
	.globl	\name
	.type	\name,@function
\name:
	.endm

	/*
	 * ��٥���� Label
	 *
	 *   �ؿ�̾�Ȥ��ƻȤ���٥���դ�Ԥ�
	 */
	
	.macro	Label name
	.globl	\name
	.type	\name,@function
\name:
	.endm

	/*
	 * �����å���� - �ץå���
	 */
	
	.macro	Push source
	add		-4, r3
	st.w	\source, 0[r3]
	.endm
	
	/*
	 * �����å���� - �ݥå�
	 */
	
	.macro	Pop dest
	ld.w	0[r3],\dest
	add		4, r3
	.endm

	/*
	 * �����å���� - ������å�����
	 */
	.macro	PushScratchRegisters
	st.w	r12,  -4[r3]
	st.w	r13,  -8[r3]
	st.w	r14, -12[r3]
	st.w	r15, -16[r3]
	st.w	r16, -20[r3]
	st.w	r17, -24[r3]
	st.w	r18, -28[r3]
	st.w	r19, -32[r3]
	add		-32, r3
	.endm
	
	/*
	 * �����å���� - ������å�����
	 */
	.macro	PopScratchRegisters
	add		32, r3
	ld.w	 -4[r3], r12
	ld.w	 -8[r3], r13
	ld.w	-12[r3], r14
	ld.w	-16[r3], r15
	ld.w	-20[r3], r16
	ld.w	-24[r3], r17
	ld.w	-28[r3], r18
	ld.w	-32[r3], r19
	.endm

	.macro	Led1 dest, work
	Lea		0x07802000, \work1
	st.b	\dest, 0[\work]
	.endm

	.macro	Led2 dest, work
	Lea		0x07802000, \work1
	st.b	\dest, 0[\work]
	.endm
