/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
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
 *  @(#) $Id: sys_support.S,v 1.1 2004/07/21 02:49:37 monaka Exp $
 */

/*
 *	ターゲットシステム依存モジュール アセンブリ言語部（PC/AT用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */
		.text
	.globl hardware_init_hook
hardware_init_hook:	
	movl %cr0, %eax
	andl $~0x60000000, %eax
	movl %eax, %cr0 /* CD = 0, NW = 0 */
	ret; 

#define CALL_HANDLER(x) incl nest; sti; call x; cli; decl nest

/*
 *   割込みハンドラの出入口処理
 *
 *   割込み番号を%ecxに入れて呼び出される。
 */
	.text
interrupt:
	cld
	cmpl	$0, nest
	jne	interrupt_2		/* 内側のハンドラ */
	/* 外側のハンドラ */
	movl %esp, %eax
	movl	STACKTOP, %esp
	pushl %eax
interrupt_2:				
	pushl %ecx /* intno */
	movl int_table(,%ecx,4), %eax
	testl %eax, %eax
	jz 0f
	CALL_HANDLER(*%eax)
0:
	popl %eax /* intno */
	testb $8, %al
	jz 1f
	movb $0x20, %al
	outb %al, $0xa0 /* slave non-specific EOI */
1:
	movb $0x20, %al
	outb %al, $0x20 /* master non-specific EOI */
	cmpl	$0, nest
	jne	interrupt_3
	popl %eax
	movl %eax, %esp
	cmpl $0, reqflg
	je interrupt_3
ret_int:
	movl	$0, reqflg
	cmpl	$0, enadsp
	je	ret_int_r
	movl	runtsk, %ebx
	cmpl	%ebx, schedtsk
	je	ret_int_r
	movl %esp, TCB_esp(%ebx)
	movl	$ret_int_r, %eax
	movl	%eax, TCB_eip(%ebx)
	jmp	dispatch_loop
ret_int_r:
	call	calltex
interrupt_3:	
	popa
	iret

#define interrupt(intno) \
	.global interrupt##intno ; \
interrupt##intno: ; \
	pusha ; \
	movl $intno, %ecx ; \
	jmp interrupt

	interrupt(0)
	interrupt(1)
	interrupt(2)
	interrupt(3)
	interrupt(4)
	interrupt(5)
	interrupt(6)
	interrupt(7)
	interrupt(8)
	interrupt(9)
	interrupt(10)
	interrupt(11)
	interrupt(12)
	interrupt(13)
	interrupt(14)
	interrupt(15)
