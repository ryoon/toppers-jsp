/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: start.S,v 1.2 2002/04/05 09:01:40 takayuki Exp $
 */

#define _MACRO_ONLY

#include "sys_defs.h"
#include "m32rasm.inc"

/**************************/
/* スタートアップルーチン */
/**************************/

	.text
	.align 4
	.globl _start
_start:

	.extern		kernel_start
	.extern		hardware_init_hook

	bl		1f
		.long	__stack
		.long	hardware_init_hook
		.long	__sbss
		.long	__ebss
		.long	__sidata
		.long	__eidata
		.long	__sdata
		.long	software_init_hook
1:	mv		r8, r14

	/* 割込みスタックポインタの初期化 */
	ld		r15, @r8+	//__stack

	/* ハードウェア初期化ルーチンの起動 */
	ld		r0, @r8+	//hardware_init_hook
	beqz	r0, 1f
	jl		r0

1:
	/* .bssセクションのクリア */
	xor		r0, r0
	ld		r1, @r8+ 	//__sbss
	ld		r2, @r8+ 	//__ebss
	bl		1f
1:
	beq		r1, r2, 2f;
	st		r0, @-r2
	jmp		r14
2:

	/* .dataセクションの初期化 */
	ld		r1, @r8+ 	//__sidata
	ld		r2, @r8+ 	//__eidata
	ld		r3, @r8+ 	//__sdata

	addi	r3, -4
	beq		r1, r2, 2f
1:	ld		r0, @r1+
	st		r0, @+r3
	bne		r1, r2, 1b
2:

	/* 初期化ルーチンの起動 */
	ld		r0, @r8+	//software_init_hook
	beqz	r0, 1f
	jl		r0
1:
		/* マスク解除 & 割込み許可 */
//	ldi		r0, 0x7
//	ld24	r1, (ICUIMASK+1)
//	stb		r0, @r1
//	ldi		r0, 0x40
//	mvtc	r0, psw

	/* カーネル起動 */
	bl		kernel_start

1:	bra		1b
