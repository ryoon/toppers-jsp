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
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
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
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: sys_support.S,v 1.6 2003/06/04 01:52:23 hiro Exp $
 */

/*
 *	ターゲットシステム依存モジュール アセンブリ言語部（DVE68K/40用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"

#if 0
	.text
	.globl __udivsi3
__udivsi3:
	move.l	4(%sp),%d0
	.word 0x4C6F
	.word 0x0000
	.word 0x0008
	rts

	.globl __divsi3
__divsi3:
	move.l	4(%sp),%d0
	.word 0x4C6F
	.word 0x0800
	.word 0x0008
	rts

	.globl	__umodsi3
__umodsi3:
	move.l	(%sp)+, %a0	| pop the return address
	jbsr	__udivsi3
	mulu.l	4(%sp), %d0
	move.l	(%sp), %d1
	sub.l	%d0, %d1
	move.l	%d1, %d0	| move the modulus into %d0
	jmp	%a0@		| return

	.globl	__modsi3
__modsi3:
	move.l	(%sp)+, %a0	| pop the return address
	jbsr	__divsi3
	muls.l	4(%sp), %d0
	move.l	(%sp), %d1
	sub.l	%d0, %d1
	move.l	%d1, %d0	| move the modulus into %d0
	jmp	%a0@		| return

	.globl __mulsi3
__mulsi3:
	move.l	4(%sp),%d0
	muls.l	8(%sp),%d0
	rts

	.globl __mului3
__mului3:
	move.l	4(%sp),%d0
	mulu.l	8(%sp),%d0
	rts

#endif
