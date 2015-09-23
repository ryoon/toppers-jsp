/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2008-     by Monami Software Limited Partnership, JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
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
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: cpu_support.S,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

	.globl activate_r
activate_r:
	sti
	popl	%eax
	jmp	*%eax

/*
 *  Task dispatcher
 */
	.text
	.globl dispatch
	.globl exit_and_dispatch
dispatch:
	pusha
	movl	runtsk, %ebx
	movl	%esp, TCB_esp(%ebx)
	movl	$dispatch_r, %eax
	movl	%eax, TCB_eip(%ebx)
exit_and_dispatch:
	.globl dispatch_loop
dispatch_loop:	
	movl	schedtsk, %ebx
	movl	%ebx, runtsk
	cmpl	$0, %ebx
	jne	dispatch_1
	movl STACKTOP, %esp
	movl $1, nest
0:
	sti
	hlt
	cli
	cmpl $0, reqflg
	je 0b
	movl $0, reqflg
	movl $0, nest
	jmp	dispatch_loop
dispatch_1:
	movl	TCB_esp(%ebx), %esp
	movl	TCB_eip(%ebx), %eax
	jmp	*%eax

dispatch_r:
	call	calltex
	popa
	ret

/*
 *   CPU�㳰�ϥ�ɥ�ν���������
 */

/* %ecx���㳰�ֹ� */
/* %edx��CPU�㳰�˴ؤ������򵭲����Ƥ����ΰ����Ƭ���� */
	.extern exc_table
__exception_1:
	cld
	movl	%esp, %ebx
	movl	12(%ebx), %edx
exception:
	cmpl	$0, nest
	jne	exception_2
	movl %esp, %eax
	movl	STACKTOP, %esp
	pushl %eax
exception_2:
	pushl	%edx
	movl	exc_table(,%ecx,4), %eax
	testl %eax, %eax
	jz 1f
	incl nest
	testb $2, 13(%edx)
	jz 0f
	sti
0:
	call *%eax
	cli
	decl nest
1:
	addl	$4, %esp
	cmpl	$0, nest
	jne	exception_3
	popl %eax
	movl %eax, %esp
	cmpl $0, reqflg
	je exception_3
ret_exc:
	movl	$0, reqflg
	cmpl	$0, enadsp
	je	ret_exc_r
	movl	runtsk, %ebx
	cmpl	%ebx, schedtsk
	je	ret_exc_r
	movl %esp, TCB_esp(%ebx)
	movl	$ret_exc_r, %eax
	movl	%eax, TCB_eip(%ebx)
	jmp	dispatch_loop
ret_exc_r:
	call	calltex
exception_3:
	popa
	addl	$4, %esp
	iret

#define	exception(excno)	 \
	.global exception##excno ; \
exception##excno:		; \
	pushl	$0		; \
	pusha			; \
	movl	$excno, %ecx	; \
	jmp	__exception_1

#define	error_exception(excno)	 \
	.global exception##excno ; \
exception##excno:		; \
	pusha			; \
	movl	$excno, %ecx	; \
	jmp	__exception_1	; \


	exception(0)
	exception(1)
	exception(2)
	exception(3)
	exception(4)
	exception(5)
	exception(6)
	exception(7)
	error_exception(8)	
	exception(9)
	error_exception(10)
	error_exception(11)
	error_exception(12)
	error_exception(13)
	error_exception(14)
	exception(16)
	error_exception(17)
	exception(18)
	exception(19)