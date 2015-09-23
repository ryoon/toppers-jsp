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
 *  @(#) $Id: sys_support.S,v 1.2 2002/04/05 09:01:29 takayuki Exp $
 */

/*
 *	ターゲットシステム依存モジュール アセンブリ言語部（M3A-2131G50用）
 */

#define	_MACRO_ONLY
#include "cpu_defs.h"
#include "sys_defs.h"
#include "m32rasm.inc"

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */

	.macro	out		addr, val
		ld24	r0, \addr
		seth	r1, #((\val) >> 16)
		or3		r1, r1, #((\val) & 0xffff)
		st		r1, @r0
	.endm

	.macro	outh	addr, val
		ld24	r0, #(\addr)
		ldi		r1, #(\val)
		sth		r1, @r0
	.endm

	.macro	outb	addr, val
		ld24	r0, #(\addr)
		ldi		r1, #(\val)
		stb		r1, @r0
	.endm

Function hardware_init_hook

		//PLL動作開始
	outb	PLLCR+3,   0x1

		/* ブロック1, SDRAM用ピンを有効に */
	out		BSELCR(1), 0x0fffb50f
	outh	PMOD(0),   0x0100
	outh	PMOD(1),   0x1
	outh	PMOD(2),   0x1
	outh	PMOD(3),   0x5555
	outh	PMOD(4),   0x5555
	outh	PMOD(6),   0x0555

	outh	SDIR0+2,   0x182

		/* SDRAM初期化 & 初期化終了待ち */
	ld24	r0, SDIR1+3
	ldi		r1, 1
	stb		r1, @r0
	addi	r0, -2
1:	ldb		r1, @r0
	beqz	r1, 1b

	outh	SDMOD(0)+2, 0
	out		SDADR(0), 0x01000042	//0x01000000, 32bit, bank 4, 16MB
	outh	SDTR(0)+2,0x0001

		/* リフレッシュ有効 */
	outb	SDRF1+1, 1

		/* SDRAM有効 */
	outb	SDER(0)+3, 1


		//CPUクロック <= PLLクロック
	outb	CPUCLKCR+1, 0x2

	rts

Function vxget_tim
	rts
