/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 *
 *  @(#) $Id: cpu_support.S,v 1.3 2002/04/11 04:37:48 honda Exp $
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
dispatch_loop:	
	movl	schedtsk, %ebx
	movl	%ebx, runtsk
	cmpl	$0, %ebx
	jne	dispatch_1
	movl	$0, enadsp
	sti
	hlt
	cli
	movl	$1, enadsp
	jmp	dispatch_loop
dispatch_1:
	movl	TCB_esp(%ebx), %esp
	movl	TCB_eip(%ebx), %eax
	jmp	*%eax

dispatch_r:
	call	calltex
	popa
	ret

#define CALL_HANDLER(x) incl nest; sti; call x; cli; decl nest

/*
 *   割込みハンドラの出入口処理
 *
 *   出入口処理のマクロからハンドラの番地を%eaxに入れて呼び出される。
 */
	.text
	.globl interrupt
interrupt:
	cmpl	$0, nest
	jne	interrupt_2		/* 内側のハンドラ */
	/* 外側のハンドラ */
	movl	runtsk, %ebx		/* 割り込み用スタックに切り替える。 */
	movl	%esp, TCB_esp(%ebx)
	movl	STACKTOP, %esp
interrupt_2:				
	CALL_HANDLER(*%eax)
	cmpl	$0, nest
	jne	interrupt_3
	cmpl	$1, reqflg
	je	ret_int
	movl	runtsk, %ebx
	movl	TCB_esp(%ebx), %esp
	jmp	interrupt_3
ret_int:
	movl	$0, reqflg
	cmpl	$0, enadsp
	je	ret_int_r
	movl	runtsk, %ebx
	cmpl	%ebx, schedtsk
	je	ret_int_r
	movl	$ret_int_r, %eax
	movl	%eax, TCB_eip(%ebx)
	jmp	dispatch_loop
ret_int_r:
	call	calltex
interrupt_3:	
	popa
	iret

/*
 *   CPU例外ハンドラの出入口処理
 */

/* %ecxに例外番号 */
/* %edxにCPU例外に関する情報を記憶している領域の先頭番地 */
	.extern exc_table
__exception_1:
	movl	%esp, %ebx
	movl	12(%ebx), %edx
exception:
	cmpl	$0, nest
	jne	exception_2
	movl	runtsk, %ebx
	movl	%esp, TCB_esp(%ebx)
	movl	STACKTOP, %esp
exception_2:
	pushl	%edx
	movl	exc_table(,%ecx,4), %eax
	CALL_HANDLER(*%eax)
	addl	$4, %esp
	cmpl	$0, nest
	jne	exception_3
	cmpl	$1, reqflg
	je	ret_exc
	movl	runtsk, %ebx
	movl	TCB_esp(%ebx), %esp
	jmp	exception_3
ret_exc:
	movl	$0, reqflg
	cmpl	$0, enadsp
	je	ret_exc_r
	movl	runtsk, %ebx
	cmpl	%ebx, schedtsk
	je	ret_exc_r
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
	error_exception(9)
	error_exception(10)
	error_exception(11)
	error_exception(12)
	error_exception(13)
	error_exception(14)
	exception(16)
	error_exception(17)
	exception(18)
	exception(19)
