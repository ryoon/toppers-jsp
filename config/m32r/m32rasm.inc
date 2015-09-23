/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: m32rasm.inc,v 1.6 2003/12/24 07:35:03 takayuki Exp $
 */

#ifndef __M32RASM_INC
#define __M32RASM_INC

/*
 * 32ビット即値ロード (即値 & シンボル)
 */

	.macro	lea reg, value
		seth	\reg, #shigh(\value)
		or3		\reg, \reg, #low(\value)
	.endm

/*
 * テーブル引き 32ビット ロード - マクロ群
 *    1ルーチンで3つ以上32ビット即値を引くならテーブルに
 *    3つ以上のルーチンから引かれるシンボルもテーブルに
 */

		//テーブル作成 開始
	.macro	begin_table
		.section ".rodata","a"
		.align   4
	_table_base:
	.endm

		//テーブルエントリ定義
	.macro	entry	symbol
					.extern	\symbol
		p_\symbol:	.long	\symbol
	.endm

		//テーブル作成 終了
	.macro	end_table
	.endm

		//テーブル上のシンボル参照
	.macro	lds	dest, index, symbol
		ld	\dest, @(p_\symbol - _table_base, \index)
	.endm

		//テーブルインデックスの参照
	.macro	ldidx dest
/*		bl	1f
			.long	_table_base
		1:	ld	r14, @r14 */
		seth	\dest, #shigh(_table_base)
		or3		\dest, \dest, #low(_table_base)
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
