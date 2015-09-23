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
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
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
 *   CPU例外ハンドラの出入口処理
 */

/* %ecxに例外番号 */
/* %edxにCPU例外に関する情報を記憶している領域の先頭番地 */
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
