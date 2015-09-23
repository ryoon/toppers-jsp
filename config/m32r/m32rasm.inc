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
 *  @(#) $Id: m32rasm.inc,v 1.2 2002/04/05 09:01:40 takayuki Exp $
 */

#ifndef __M32RASM_INC
#define __M32RASM_INC

/*
 * 32ビット即値ロード
 */

	.macro	lea reg, value
		seth	\reg, ((\value) >> 16)
		or3		\reg, \reg, ((\value) & 0xffff)
	.endm


/*
 * 割込み許可
 *  引数はつぶしてもいいレジスタ
 */
	.macro	ei work=r0
		mvfc	\work, psw
		or3		\work, \work, 0x40
		mvtc	\work, psw
	.endm

/*
 * 割込み禁止
 *  引数はつぶしてもいいレジスタ
 */
	.macro	di work=r0
		mvfc	\work, psw
		and3	\work, \work, 0xcf
		mvtc	\work, psw
	.endm

/* 外部参照可能なシンボルの作成 */
	.macro Label name
		.globl \name
		\name:
	.endm

/* 関数エントリの作成 */
	.macro Function name
		.text
		.align 4
		.globl \name
		\name:
	.endm

/* 関数からの復帰 */
	.macro rts
		jmp  r14
	.endm

/* 関数呼出 */
	.macro call \name
		st	r14,@-r15
		bl	\name
	.endm

/* スタック積み */
	.macro stmdb regset
		.irp reg, \regset
			st	\reg, @-r15
		.endr
	.endm

/* スタック戻し */
	.macro ldmia regset
		.irp reg, \regset
			ld	\reg, @r15+
		.endr
	.endm

#endif
