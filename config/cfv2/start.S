/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *  @(#) $Id: start.S,v 1.6 2003/06/04 01:51:39 hiro Exp $
 */

/*
 *	�����ͥ��ѤΥ������ȥ��åץ⥸�塼���COLDFIRE�ѡ�
 */

#define	_MACRO_ONLY
#include <t_config.h>
/*
 *�����㳰�٥����������COLDFIRE�ѡ�
 *�����������Ǥϥꥻ�åȻ���ɬ�פʺ���¤Υ٥��������Ѱդ��ʤ�
 *������cpu_initialize()���㳰�٥����ΰ�ư��VBR�ν񤭴����ˤ�Ԥ�
 */
#ifdef KERNEL_HAS_A_VECTOR_TABLE

/*
 *   �ʲ����㳰�װ��ǥǥե���ȤȤϰۤʤ��㳰�٥��������
 *   ������ϡ�sys_config.h�ǳ�������ޥ�����������롣
 */
	.section ".vector","ax"
	.align 2
	.long INIT_STACK		/*   0:�����å��ݥ��󥿤ν����  */
        .long TO_ASM(RESET_VECTOR)	/*   1:�ѥ����ꥻ�å�  */
	.long TO_ASM(ACC_VECTOR)	/*   2:�����������顼      */
  	.long TO_ASM(ADD_VECTOR)	/*   3:���ɥ쥹���顼      */
  	.long TO_ASM(II_VECTOR)		/*   4:��������̿��        */
	.long TO_ASM(RESERVED_VECTOR)	/*   5:d zero              */
	.long TO_ASM(RESERVED_VECTOR)   /*   6:ͽ��                */
        .long TO_ASM(II_VECTOR)         /*   7:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*   8:pv�����ƥ�ͽ��        */
        .long TO_ASM(RESERVED_VECTOR)   /*   9:trace               */
	.long TO_ASM(RESERVED_VECTOR)	/*  10:line a emulator     */
	.long TO_ASM(RESERVED_VECTOR)	/*  11:line f emulator     */
	.long TO_ASM(RESERVED_VECTOR)	/*  12:�����ƥ�ͽ��        */
        .long TO_ASM(RESERVED_VECTOR)   /*  13:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  14:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  15:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  16:�����ƥ�ͽ��        */
        .long TO_ASM(RESERVED_VECTOR)   /*  17:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  18:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  19:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  20:�����ƥ�ͽ��        */
        .long TO_ASM(RESERVED_VECTOR)   /*  21:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  22:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  23:�����ƥ�ͽ��        */
	.long TO_ASM(RESERVED_VECTOR)	/*  24:���ץꥢ��������  */
	.long TO_ASM(RESERVED_VECTOR)	/*  �Ĥ�ϥǥե���Ȥ��㳰����  */
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)

	/* CF-Flash Protection Field */
	.org 0x400
	.long 0xFFFFFFFF
	.long 0xFFFFFFFF
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0xFFFFFFFF
	
#endif  /* KERNEL_HAS_A_VECTOR_TABLE */
	.text
	.globl start
	.align 2
start:
	/*
	 *  �ץ����å��⡼�ɤȥ����å��ݥ��󥿤ν����
	 */
	move.l #0x2700, %d0
	move.w  %d0, %sr

	/* SRAM�١������ɥ쥹�ν���� */
	move.l	#0x20000000, %d0
	add.l   #0x21, %d0
	.long   0x4e7b0C05  /* movec   %d0, %rambar1 */
	
	/* ROM�١������ɥ쥹�ν���� */
	move.l	#0x00000000, %d0
	add.l   #0x21, %d0
	.long   0x4e7b0C04  /* movec   %d0, %flashbar */
	
	movea.l #INIT_STACK, %sp	/* �ե졼��ݥ��󥿤ν���� */
	movea.l #0, %fp	
	/*
	 *  hardware_init_hook �θƽФ���0 �Ǥʤ�����
	 *
	 *  �������åȥϡ��ɥ������˰�¸����ɬ�פʽ����������������
	 *  �ϡ�hardware_init_hook �Ȥ����ؿ����Ѱդ��롥
	 */
	lea hardware_init_hook, %a0
	tst.l %a0
	jbeq start_1
	jsr %a0@
start_1:
	move.l #__bss_start, %a0
	move.l #_end, %d0
	sub.l %a0, %d0
	jbeq start_3
start_2:
	clr.b (%a0)+
	subq.l	#1, %d0
	bne start_2
	clr.w %d0
	subq.l #1, %d0
	jbcc start_2
	
	/*
	 *  data���������ν������ROM���б���
	 *
	 *  __idata_start ���� __idata_end �ޤǤ�__data_start �ʹߤ�
	 *  ���ԡ����롥
	 */
start_3:
	move.l #__idata_end, %d0
	sub.l #__idata_start, %d0
	jbls start_5
	move.l #__idata_start, %a0
	move.l #__data_start, %a1
start_4:
	move.b (%a0)+, (%a1)+
	subq.l	#1, %d0
	bne start_4
	clr.w %d0
	subq.l #1, %d0
	jbcc start_4
	
	/*
	 *  software_init_hook ��ƽФ���0 �Ǥʤ�����
	 *
	 *  ���եȥ������Ķ����ä˥饤�֥��ˤ˰�¸����ɬ�פʽ������
	 *  ����������ϡ�software_init_hook �Ȥ����ؿ����Ѱդ����
	 *  �褤��
	 */
start_5:
	lea software_init_hook, %a0
	tst.l %a0
	jbeq start_6
	jsr %a0@

	/*
	 *  �����ͥ��ư���롥
	 */
start_6:
	jmp kernel_start