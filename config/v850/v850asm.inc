/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: v850asm.inc,v 1.1 2000/11/24 03:41:29 takayuki Exp $
 */

/*
 *  V850アセンブラ用いろいろ定義ファイル
 */


	/*
	 * ラベルロード Lea (Load Effective Address)
	 */
	 
	.macro	Lea name, destination
	movhi	hi(\name), r0, \destination
	movea	lo(\name), \destination, \destination
	.endm


	/*
	 * 関数宣言 Function
	 *
	 *   関数名として使うラベル割付と 諸設定を行う
	 */
	 
	.macro	Function name
	.text
	.align	4
	.globl	\name
	.type	\name,@function
\name:
	.endm

	/*
	 * ラベル宣言 Label
	 *
	 *   関数名として使うラベル割付を行う
	 */
	
	.macro	Label name
	.globl	\name
	.type	\name,@function
\name:
	.endm

	/*
	 * スタック操作 - プッシュ
	 */
	
	.macro	Push source
	add		-4, r3
	st.w	\source, 0[r3]
	.endm
	
	/*
	 * スタック操作 - ポップ
	 */
	
	.macro	Pop dest
	ld.w	0[r3],\dest
	add		4, r3
	.endm

	/*
	 * スタック操作 - スクラッチ退避
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
	 * スタック操作 - スクラッチ復帰
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
