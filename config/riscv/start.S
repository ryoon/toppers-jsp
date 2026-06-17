/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005,2006 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2018-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  @(#) $Id: start.S 264 2020-10-08 18:04:21Z roi $
 */

/*
 *  カーネル対応のスタートアップモジュール（RISC-V用）
 */

#define _MACRO_ONLY
#include <sys_defs.h>
#include <t_config.h>

#if defined(__riscv64)
#define LREG                ld
#else
#define LREG                lw
#endif

	.text
	.global start
	.type start, %function
start:
	.cfi_startproc
	.cfi_undefined ra

#if defined(TOPPERS_GD32VF103)
	csrc mstatus, MSTATUS_MIE
	/* Jump to logical address first to ensure correct operation of RAM region  */
	la a0,	start
	li  a1, 1
	slli a1, a1, 29
	bleu a1, a0, start_0
	srli a1, a1, 2
	bleu a1, a0, start_0
	la  a0,	start_0
	add a0, a0, a1
	jr  a0

start_0:
    csrci CSR_MCOUNTINHIBIT, 0x5
#endif	/* TOPPERS_GD32VF103 */

    .option push
    .option norelax
    la gp, __global_pointer$
    .option pop

#if defined(TOPPERS_K210)
    csrw mideleg, 0
    csrw medeleg, 0
#endif	/* TOPPERS_K210 */
    csrw mie, 0
    csrw mip, 0
    csrr a0, mhartid
    li sp, (STACKTOP-PSTACKSIZE)	/* core#1 stack */
	bnez a0, start_7
    li sp, STACKTOP					/* core#0 stack */

	/*
	 *  bssセクションのクリア
	 */
start_1:
	la a0, __bss_start
	la a1, _end
	bgeu a0, a1, start_3
start_2:
	sw zero, (a0)
	addi a0, a0, 4
	bltu a0, a1, start_2

	/*
	 *  dataセクションの初期化（ROM化対応）
	 *
	 *  __idata_startから__idata_endまでを，__data_start以降にコピーす
	 *  る．
	 */
start_3:
	la a0, __idata_start
	la a1, __data_start
	la a2, __idata_end
	bgeu a0, a2, start_5

start_4:
	lw t0, (a0)
	sw t0, (a1)
	addi a0, a0, 4
	addi a1, a1, 4
	bltu a0, a2, start_4

	/*
	 * hardware_init_hookの呼び出し(リンクされない場合は未処理関数)
	 *
	 * ターゲットハードウェアに依存して必要な初期化処理がある場合
	 * は，hardware_init_hook という関数を用意する
	 */
start_5:
	la t0, hardware_init_hook
	beqz t0, start_6
	jalr t0

	/*
	 *  software_init_hook を呼出し（リンクされない場合は未処理関数）
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */
start_6:
	la t0, software_init_hook
	beqz t0, start_7
	jalr t0

start_7:

#ifndef __riscv_float_abi_soft
	/* Enable FPU */
	li t0, MSTATUS_FS
	csrs mstatus, t0
	csrr t1, mstatus
	and t1, t1, t0
	beqz t1, start_8
	fssr x0

start_8:
#endif

	/*
	 *  カーネルを起動する．
	 */
	j   kernel_start
	.cfi_endproc

