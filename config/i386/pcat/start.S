/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
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
 *  @(#) $Id: start.S,v 1.3 2002/04/10 12:08:17 honda Exp $
 */

/*
 *	カーネル用のスタートアップモジュール（Multiboot用）
 */

#define	_MACRO_ONLY
#include "sys_config.h"
#include "multiboot.h"
	
	.text
	.globl start
	.globl _start
start:
_start:
	jmp	multiboot_entry

	.align  4
	multiboot_header:
	/* magic */
	.long	MULTIBOOT_HEADER_MAGIC
	/* flags */
	.long	MULTIBOOT_HEADER_FLAGS
	/* checksum */
	.long	-(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
	/* header_addr */
	.long	multiboot_header
	/* load_addr */
	.long	_start
	/* load_end_addr */
	.long	_edata
	/* bss_end_addr */
	.long	_end
	/* entry_addr */
	.long	multiboot_entry

multiboot_entry:
	movl	STACKTOP, %esp
	pushl	$0
	popf

	/*
	 *  hardware_init_hook の呼出し（0 でない場合）
	 *
	 *  ターゲットハードウェアに依存して必要な初期化処理がある場合
	 *  は，hardware_init_hook という関数を用意する．
	 */
start_1:	
	movl hardware_init_hook, %eax
	cmpl $0, %eax
	je start_2
	call hardware_init_hook

	/*
	 *  software_init_hook を呼出し（0 でない場合）
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */
start_2:
	movl software_init_hook, %eax
	cmpl $0, %eax
	je start_3
	call software_init_hook

	/*
	 *  カーネルを起動する．
	 */
start_3:	
	jmp kernel_start


	
	/* 非タスクコンテキスト用のスタック */
	.globl stack
	.comm	stack, STACKSIZE
