/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
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
 *  @(#) $Id: start.S,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

/*
 *	カーネル用のスタートアップモジュール（Multiboot用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include <multiboot.h>
	
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
	cmpl 0, %eax
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
	cmpl 0, %eax
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
