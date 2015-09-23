/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: start.S,v 1.1 2001/05/04 09:07:50 imai Exp $
 */
#define _MACRO_ONLY
#include "sys_config.h"
#include "cpu_config.h"


/*  リセットベクタの定義  */
#ifdef WITH_STUB
 注意：スタブ完成後に定義
#else
#define _reset_start  _start
#endif /*  WITH_STUB  */
#define _init_stack  STACKTOP


	.section ".vector","ax"
	.align 4

	.long   _reset_start	/*  パワーオンリセット  */
	.long   _init_stack
	.long   _reset_start	/*  マニュアルリセット  */
	.long   _init_stack
				/*  残りはデフォルトの例外処理  */
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception
	.long   _no_reg_exception



/*
 *   SH1用スタートアップモジュール
 *   カーネルはアプリケーションとリンクして使用する
 *   だけなのでJSPカーネルのみ使用する。
 */
	.global	_start
	.section .text
	.align 2
_start:
	/*
	 *  SRを初期化する。
	 *  IPM=0xe
	 *  stub実行時のみIPM=0xfとなる。
	 */
	mov.l _init_sr,r0
	ldc   r0,sr
	/*
	 *  割込み／CPU例外ネストカウンタを初期化する。
	 */
	mov.l _intnest_k,r1
	mov   #0x01,r0
	mov.l r0,@r1
	/*
	 * タスク独立のスタックをSTACKTOPに初期化する
	 * STACKTOPは、sys_config.hで定義
	 */
	mov.l  _stack_k, r15

	/*
	 *  _hardware_init_hook を呼び出す。(0でない場合)
	 *
	 *  ハードウェア依存に必要な初期化処理がある場合は，
	 *  hardware_init_hook という関数を用意すればよい．
	 *  具体的にはROM化の際、RAMを使用可能にするための
	 *  バスコントローラの初期化等を行う。
	 *  sys_support.S 内で hardware_init_hook を定義してい
	 *  る．
	 */

start_0:
	mov.l   _hardware_init_k, r0
	tst    r0,r0
	bt     start_1
	jsr     @r0
        nop


	/*
         * bssセクションをクリア
	 */
start_1:
	mov.l	_bss_start_k,r0
	mov.l	_end_k,r1
	cmp/eq  r0,r1
	bt      start_3
	mov	#0,r2
start_2:
	/*
	 *  r0のインクリメントが冗長に見えるが
	 *  mov.l r4,@r0+命令はない
	 *	ディスティネーションのアドレスはディクリメントのみ
	 */
        mov.l	r2,@r0
	add	#4,r0
 	cmp/hi	r0,r1
	bt	start_2

	/*
	 *  dataセクションを初期化する（ROM化対応）．
	 *
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする．
	 */
start_3:
#ifndef USE_ICE		/*  実機の場合  */
	mov.l   __idata_start_k,r1
	mov.l   __idata_end_k,r2
	cmp/eq  r1,r2
	bt      start_5
	mov.l   __data_start_k,r0
start_4:
	mov.l   @r1+,r4
	mov.l   r4,@r0
	cmp/hi  r1,r2
	add     #4,r0
	bt      start_4
#endif 	/* USE_ICE */

	/*
	 *  software_init_hook を呼び出す（0 でない場合）．
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */
start_5:
	mov.l  _software_init_hook_k, r0
	tst    r0,r0
	bt     start_6
	jsr    @r0
	nop


start_6:
	/*
         *  カーネルを起動する
	 */
	! call the mainline
	mov.l	_kernel_start_k,r0
	jsr	@r0
	nop


	.align 4
_intnest_k:
	.long	_intnest
_init_sr:
	.long	MAX_IPM << 4
_stack_k:
	.long   STACKTOP
_bss_start_k:
	.long   __bss_start
_end_k:
	.long  	_end
#ifndef USE_ICE		/*  実機の場合  */
__idata_start_k:
	.long 	__idata_start
__idata_end_k:
	.long   __idata_end
__data_start_k:
	.long  	__data_start
#endif 	/* USE_ICE */
_kernel_start_k:
	.long	_kernel_start
_hardware_init_k:
	.long   _hardware_init_hook
_software_init_hook_k:
	.long   _software_init_hook
