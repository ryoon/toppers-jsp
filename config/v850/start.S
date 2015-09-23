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
 *  @(#) $Id: start.S,v 1.5 2002/04/10 11:17:16 takayuki Exp $
 */

#define	_MACRO_ONLY
#include "sys_config.h"
#include "v850asm.inc"
#include "offset.h"


/*
 *	V850用スタートアップファイル
 */

Function start
	di

	/*
	 * スタックポインタの初期化
	 */
	mov		r0, r2
	Lea		STACKTOP, r3

	/*
	 *  hardware_init_hook の呼出し（0 でない場合）
	 *
	 *  ターゲットハードウェアに依存して必要な初期化処理がある場合
	 *  は，hardware_init_hook という関数を用意する．
	 */

	Lea		hardware_init_hook, r1
	cmp		r1, r0
	bz		1f
	jarl	hardware_init_hook, r31
1:


	/*
	 *  bssセクションのクリア
	 *   アラインされているので4ずつ行く
	 */

	Lea		__bss_start, ep
	Lea		_end, r6
	Lea		1f, r31
1:
	cmp		ep, r6
	be		1f
	sst.w	r0, 0[ep]
	add		4, ep
	jmp		r31
1:


	/*
	 *  dataセクションの初期化（ROM化対応）
	 *
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする．
	 */
	Lea		__idata_start, r6
	Lea		__idata_end,   r7
	Lea		__data_start,  r8
	Lea		1f, r31
1:
	cmp		r6, r7
	be		1f
	ld.w	0[r6], r9
	st.w	r9, 0[r8]
	add		4, r6
	add		4, r8
	jmp		r31
1:

	/*
	 *  software_init_hook を呼出し（0 でない場合）
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */

	Lea		software_init_hook, r1
	cmp		r1, r1
	bz		1f
	jarl	software_init_hook, r31
1:

	/*
	 *  カーネルを起動する．
	 */
	jarl _kernel_start, r31
	halt
