/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  $Id: sys_support.S 1.1 2025-12-10 21:01:25Z roi $
 */

/*
 *		プロセッサ依存モジュール アセンブリ言語部（Raspberry Pi Pico2 RISC-V用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"


#if defined(__riscv64)
#define LREG                ld
#define SREG                sd
#define LFREG               fld
#define SFREG               fsd
#define REGSIZE             8
#else
#define LREG                lw
#define SREG                sw
#define LFREG               flw
#define SFREG               fsw
#define REGSIZE             4
#endif

#if defined(USE_FPU) && !defined(__riscv_float_abi_soft)
#define SAVEREG1            27
#define SAVEREG2            42
#define SAVEREG3            26
#else
#define SAVEREG1            15
#define SAVEREG2            22
#define SAVEREG3            14
#endif

/*
 *  MACHINE割込み出口処理
 *
 *  第一レベルのMACHIE割込みを行う．スタックするレジスタはネスト割込みとほぼ
 *  同じ，ユーザースタック上に，t3-t6までのレジスタはユーザースタックに
 *  保存する．(ネスト割込みではシステムスタックに保存)そのため、
 *  4*RGSIZEバイト分、割込みのためにユーザースタックを消費する．
 *  例外/割込みが発生すると，システムスタック上に以下の情報とスクラッチ
 *  レジスタを保存する．
 *
 *   ------------
 *  | mstatus    |
 *   ------------
 *  | mpc        |
 *   ------------
 *  | mcause     |
 *   ------------
 *  | mie        |
 *   ------------
 *  | sp(x2)     |
 *   ------------
 *  | reserved   |
 *   ------------
 *  | a0(x10)    |
 *   ------------
 *  | a1(x11)    |
 *   ------------
 *  | ra(x1)     |
 *   ------------
 *  | a2(x12)    |
 *   ------------
 */
	.section .isr_vector, "ax"
	.option push
	.option norvc
	.option norelax
 	.global ivector
	.type ivector, %object
	.align 4

ivector:
 	j trap_entry
	.word 0
	.word 0
 	j trap_entry		/* soft irq */
	.word 0
	.word 0
	.word 0
	j m_timer_handler
	.word 0
	.word 0
	.word 0
	j irq_entry
	.word 0
	.word 0
	.word 0
	.word 0
	.option pop

/*
 *  MACHINE割込み出口処理
 *
 *  第一レベルのMACHIE割込みを行う．スタックするレジスタはネスト割込みとほぼ
 *  同じ，ユーザースタック上に，t3-t6までのレジスタはユーザースタックに
 *  保存する．(ネスト割込みではシステムスタックに保存)そのため、
 *  4*RGSIZEバイト分、割込みのためにユーザースタックを消費する．
 *  例外が発生すると，システムスタック上に以下の情報とスクラッチ
 *  レジスタを保存する．
 *
 *   ------------
 *  | mstatus    |
 *   ------------
 *  | mpc        |
 *   ------------
 *  | mcause     |
 *   ------------
 *  | mie        |
 *   ------------
 *  | sp(x2)     |
 *   ------------
 *  | meicontext |
 *   ------------
 *  | a0(x10)    |
 *   ------------
 *  | a1(x11)    |
 *   ------------
 *  | ra(x1)     |
 *   ------------
 *  | a2(x12)    |
 *   ------------
 */
	.section .text.bottom
	.align 4
	.global trap_entry
	.type trap_entry, %function
trap_entry:
	addi sp, sp, -SAVEREG2*REGSIZE	/* ユーザースタックにワークレジスタの保存領域を作成 */
	SREG t3, 18*REGSIZE(sp)			/* t3-t6をワークレジスタとして使用する */
	SREG t4, 19*REGSIZE(sp)
	SREG t5, 20*REGSIZE(sp)
	SREG t6, 21*REGSIZE(sp)

	la t3, inest_lvl				/* inest_lvlの判定 */
	lh t6, 0(t3)
	mv t4, sp						/* ユーザースタックをt4にコピー */
	bnez t6, trap_entry_1

	li sp, STACKTOP					/* スタック切り替え */
	addi sp, sp, -SAVEREG2*REGSIZE	/* システムスタック保存領域を作成 */

trap_entry_1:
	lh   t6, 0(t3)					/* inest_lvlをインクリメント */
	addi t6, t6, 1
	sh   t6, 0(t3)

	SREG a0, 6*REGSIZE(sp)			/* スクラッチレジスタを保存 */
	SREG a1, 7*REGSIZE(sp)
	SREG a2, 8*REGSIZE(sp)
	SREG ra, 9*REGSIZE(sp)
	SREG a3, 10*REGSIZE(sp)
	SREG a4, 11*REGSIZE(sp)
	SREG a5, 12*REGSIZE(sp)
	SREG a6, 13*REGSIZE(sp)
	SREG a7, 14*REGSIZE(sp)
	SREG t0, 15*REGSIZE(sp)
	SREG t1, 16*REGSIZE(sp)
	SREG t2, 17*REGSIZE(sp)

	csrr a0, mcause					/* mcauseを2に保存 */
	SREG a0, 2*REGSIZE(sp)
	csrr t5, mepc					/* mpcを1に保存 */
	SREG t5, 1*REGSIZE(sp)
	csrr t6, mstatus				/* mstatusを0に保存 */
	SREG t6, 0*REGSIZE(sp)
	csrr t3, RVCSR_MEICONTEXT		/* meicontextを5に保存 */
	SREG t3, 5*REGSIZE(sp)
	csrr t3, mie					/* mieを3に保存 */
	SREG t3, 3*REGSIZE(sp)
	SREG t4, 4*REGSIZE(sp)			/* ユーザースタックを4に保存 */

	mv a1, sp						/* スタックをa1に設定 */
	add sp, sp, -16
	call handle_trap				/* MACHINEハンドラを呼び出す */
	add sp, sp, 16

trap_entry_2:
	la  t3, inest_lvl				/* inest_lvlをデクリメント */
	lh  t6, 0(t3)
	addi t6, t6, -1
	sh  t6, 0(t3)
	bnez t6, ret_int_3				/* inest_lvlがゼロでないならばret_int_3へ */
	la t5, reqflg					/* reqflgがfalseであればret_int_3へ */
	LREG t6, 0(t5)
	beqz t6, ret_int_3

	LREG a0, 6*REGSIZE(sp)			/* スクラッチレジスタを復帰 */
	LREG a1, 7*REGSIZE(sp)
	LREG a2, 8*REGSIZE(sp)
	LREG ra, 9*REGSIZE(sp)
	LREG a3, 10*REGSIZE(sp)
	LREG a4, 11*REGSIZE(sp)
	LREG a5, 12*REGSIZE(sp)
	LREG a6, 13*REGSIZE(sp)
	LREG a7, 14*REGSIZE(sp)
	LREG t0, 15*REGSIZE(sp)
	LREG t1, 16*REGSIZE(sp)
	LREG t2, 17*REGSIZE(sp)

	LREG t5, 1*REGSIZE(sp)			/* mepcの取り出し */
	LREG t4, 5*REGSIZE(sp)			/* meicontextの取り出し */
	LREG t3, 3*REGSIZE(sp)			/* mieの取り出し */
	LREG t6, 0*REGSIZE(sp)			/* mstatusの取り出し */
	LREG sp, 4*REGSIZE(sp)			/* ユーザースタックを復帰 */

	SREG t6, 0*REGSIZE(sp)			/* mstatusの保存 */
	SREG t3, 3*REGSIZE(sp)			/* mieの保存 */
	SREG t4, 5*REGSIZE(sp)			/* meicontextの保存 */
	SREG t5, 1*REGSIZE(sp)			/* mepcの保存 */
	SREG sp, 4*REGSIZE(sp)			/* ユーザースタックを保存 */
	SREG a0, 6*REGSIZE(sp)			/* スクラッチレジスタを保存 */
	SREG a1, 7*REGSIZE(sp)
	SREG a2, 8*REGSIZE(sp)
	SREG ra, 9*REGSIZE(sp)
	SREG a3, 10*REGSIZE(sp)
	SREG a4, 11*REGSIZE(sp)
	SREG a5, 12*REGSIZE(sp)
	SREG a6, 13*REGSIZE(sp)
	SREG a7, 14*REGSIZE(sp)
	SREG t0, 15*REGSIZE(sp)
	SREG t1, 16*REGSIZE(sp)
	SREG t2, 17*REGSIZE(sp)

	la a0, reqflg					/* reqflgをfalseに */
	SREG zero, 0(a0)
	la a1, lock_flag				/* loc_flagをtrueに */
	li a2, TRUE
	SREG a2, 0(a1)

	la a1, runtsk					/* runtskをa0に */
	LREG a0, 0(a1)
	la a2, enadsp					/* enadspがfalseならret_int_1へ */
	lw a3, 0(a2)
	beqz a3, ret_int_1
	la a2, schedtsk					/* runtskとschedtskが同じなら */
	LREG a3, 0(a2)
	beq a0, a3, ret_int_1			/* ret_int_1へ */

	LREG a1, 5*REGSIZE(sp)			/* meicontextの取り出し */
	LREG a3, 0*REGSIZE(sp)			/* mstatusの取り出し */
	csrw mstatus, a3				/* mstatusの復帰 */
	csrw RVCSR_MEICONTEXT, a1		/* meicontextの復帰 */
	la a2, lock_flag				/* CPUロック解除 */
	SREG zero, 0(a2)

	addi sp, sp, -SAVEREG3*REGSIZE	/* call先セーブのレジスタを保存 */
	SREG gp, 0*REGSIZE(sp)
	SREG tp, 1*REGSIZE(sp)
	SREG s0, 2*REGSIZE(sp)
	SREG s1, 3*REGSIZE(sp)
	SREG s2, 4*REGSIZE(sp)
	SREG s3, 5*REGSIZE(sp)
	SREG s4, 6*REGSIZE(sp)
	SREG s5, 7*REGSIZE(sp)
	SREG s6, 8*REGSIZE(sp)
	SREG s7, 9*REGSIZE(sp)
	SREG s8, 10*REGSIZE(sp)
	SREG s9, 11*REGSIZE(sp)
	SREG s10, 12*REGSIZE(sp)
	SREG s11, 13*REGSIZE(sp)

	la a2, dispatcher				/* dispatcherアドレスの取り出し */
	csrw mepc, a2					/* mpcに設定 */
	SREG sp, TCB_sp(a0)				/* タスクスタックを保存 */
	la a1, ret_int_r				/* 実行再開番地を保存 */
	SREG a1, TCB_pc(a0)
	mret							/* jump dispatcher */

    .global	_kernel_break_wait
	.type _kernel_break_wait, %function
_kernel_break_wait:
ret_int_r:
	LREG gp, 0*REGSIZE(sp)			/* call先セーブのレジスタを復帰 */
	LREG tp, 1*REGSIZE(sp)
	LREG s0, 2*REGSIZE(sp)
	LREG s1, 3*REGSIZE(sp)
	LREG s2, 4*REGSIZE(sp)
	LREG s3, 5*REGSIZE(sp)
	LREG s4, 6*REGSIZE(sp)
	LREG s5, 7*REGSIZE(sp)
	LREG s6, 8*REGSIZE(sp)
	LREG s7, 9*REGSIZE(sp)
	LREG s8, 10*REGSIZE(sp)
	LREG s9, 11*REGSIZE(sp)
	LREG s10, 12*REGSIZE(sp)
	LREG s11, 13*REGSIZE(sp)
	addi sp, sp, SAVEREG3*REGSIZE

ret_int_1:
	/*
	 *  enatexがtrueで，texptnが0でなく，ipmflgがtrueであれば，タスク
	 *  例外処理ルーチンを呼び出す．
	 */
	lbu a1, TCB_enatex(a0)			/* enatexがfalseならret_int_2へ */
	andi a1, a1, TCB_enatex_mask
	beqz a1, ret_int_2
	lw a1, TCB_texptn(a0)			/* texptnが0ならばret_int_2へ */
	beqz a1, ret_int_2
	jal call_texrtn

ret_int_2:
	/*
	 *  割込み処理からのリターンにより，CPUロック解除状態に移行するよ
	 *  う準備する．割込み優先度マスクは，MRET命令により元の値に戻るた
	 *  め，lock_flagをfalseにしておけばよい．
	 */
	la a0, lock_flag				/* CPUロック解除の準備 */
	SREG zero, 0(a0)

ret_int_3:
	LREG a3, 5*REGSIZE(sp)			/* meicontextの取り出し */
	csrw RVCSR_MEICONTEXT, a3		/* meicontextの復帰 */
	LREG a1, 3*REGSIZE(sp)			/* mieの取り出し */
	LREG a2, 1*REGSIZE(sp)			/* mpcの取り出し */
	LREG a3, 0*REGSIZE(sp)			/* mstatusの取り出し */
	csrw mepc, a2					/* mpcの復帰 */
	csrw mstatus, a3				/* mstatusの復帰 */
	csrw mie, a1					/* mieの復帰 */

	LREG a0, 6*REGSIZE(sp)			/* スクラッチレジスタの復帰 */
	LREG a1, 7*REGSIZE(sp)
	LREG a2, 8*REGSIZE(sp)
	LREG ra, 9*REGSIZE(sp)
	LREG a3, 10*REGSIZE(sp)
	LREG a4, 11*REGSIZE(sp)
	LREG a5, 12*REGSIZE(sp)
	LREG a6, 13*REGSIZE(sp)
	LREG a7, 14*REGSIZE(sp)
	LREG t0, 15*REGSIZE(sp)
	LREG t1, 16*REGSIZE(sp)
	LREG t2, 17*REGSIZE(sp)
	LREG sp, 4*REGSIZE(sp)			/* ユーザースタックを復帰 */

	LREG t3, 18*REGSIZE(sp)
	LREG t4, 19*REGSIZE(sp)
	LREG t5, 20*REGSIZE(sp)
	LREG t6, 21*REGSIZE(sp)
	addi sp, sp, SAVEREG2*REGSIZE
	mret

/*
 *  外部E割込み出口処理
 *
 *  第一レベルのMACHIE割込みを行う．スタックするレジスタはネスト割込みとほぼ
 *  同じ，ユーザースタック上に，t3-t6までのレジスタはユーザースタックに
 *  保存する．(ネスト割込みではシステムスタックに保存)そのため、
 *  4*RGSIZEバイト分、割込みのためにユーザースタックを消費する．
 *  割込みが発生すると，システムスタック上に以下の情報とスクラッチ
 *  レジスタを保存する．
 *
 *   ------------
 *  | mstatus    |
 *   ------------
 *  | mpc        |
 *   ------------
 *  | meinext    |
 *   ------------
 *  | mie        |
 *   ------------
 *  | sp(x2)     |
 *   ------------
 *  | meicontext |
 *   ------------
 *  | a0(x10)    |
 *   ------------
 *  | a1(x11)    |
 *   ------------
 *  | ra(x1)     |
 *   ------------
 *  | a2(x12)    |
 *   ------------
 */
	.align 2
	.global irq_entry
	.type irq_entry, %function
irq_entry:
	addi sp, sp, -SAVEREG2*REGSIZE
	SREG t3, 18*REGSIZE(sp)			/* t3-t6をワークレジスタとして使用する */
	SREG t4, 19*REGSIZE(sp)
	SREG t5, 20*REGSIZE(sp)
	SREG t6, 21*REGSIZE(sp)

	la t3, inest_lvl				/* inest_lvlの判定 */
	lh t6, 0(t3)
	mv t4, sp						/* ユーザースタックをt4にコピー */
	bnez t6, irq_entry_1

	li sp, STACKTOP					/* システムスタックを設定 */
	addi sp, sp, -SAVEREG2*REGSIZE	/* システムスタック保存領域を作成 */

irq_entry_1:
	lh  t6, 0(t3)					/* inest_lvlをインクリメント */
	addi t6, t6, 1
	sh  t6, 0(t3)

	SREG a0, 6*REGSIZE(sp)			/* スクラッチレジスタを保存 */
	SREG a1, 7*REGSIZE(sp)
	SREG a2, 8*REGSIZE(sp)
	SREG ra, 9*REGSIZE(sp)
	SREG a3, 10*REGSIZE(sp)
	SREG a4, 11*REGSIZE(sp)
	SREG a5, 12*REGSIZE(sp)
	SREG a6, 13*REGSIZE(sp)
	SREG a7, 14*REGSIZE(sp)
	SREG t0, 15*REGSIZE(sp)
	SREG t1, 16*REGSIZE(sp)
	SREG t2, 17*REGSIZE(sp)

	csrr t5, mepc					/* mpcを1に保存 */
	SREG t5, 1*REGSIZE(sp)
	csrr t6, mstatus				/* mstatusを0に保存 */
	SREG t6, 0*REGSIZE(sp)
	csrrsi t3, RVCSR_MEICONTEXT, RVCSR_MEICONTEXT_CLEARTS
	SREG t3, 5*REGSIZE(sp)
	csrr t3, mie					/* mieを3に保存 */
	SREG t3, 3*REGSIZE(sp)
	SREG t4, 4*REGSIZE(sp)			/* ユーザースタックを4に保存 */
	csrs mstatus, MSTATUS_MIE		/* カーネル外割込み許可 */

irq_entry_2:
	csrrsi a0, RVCSR_MEINEXT, RVCSR_MEINEXT_UPDATE
	SREG a0, 2*REGSIZE(sp)
	bltz a0, irq_entry_4

    lui  a2, %hi(p_int_table)
#if defined(__riscv64)
    ld  a1, %lo(p_int_table)(a2)
#else
    lw  a1, %lo(p_int_table)(a2)
#endif


	add a1, a1, a0
	LREG a2, 0(a1)
	bnez a2, irq_entry_3
	la  a2, _kernel_default_hazard3_handler

irq_entry_3:
	mv a1, sp
	jalr ra, a2
	j irq_entry_2

irq_entry_4:
	li a1, KERNEL_MIE
	csrrc a1, mie, a1				/* 割込み禁止 */
	j trap_entry_2

m_timer_handler:
m_loop:
  j  m_loop

/*
 *  タスクディスパッチャ
 */
	.section .text.bottom
	.align 4
	.global dispatch, exit_and_dispatch
	.type dispatch, %function
	.type exit_and_dispatch, %function
dispatch:
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・ディスパッチ
	 *  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 *  れる．
	 */
	addi sp, sp, -SAVEREG1*REGSIZE	/* レジスタ保存領域を確保 */
	SREG ra, 0*REGSIZE(sp)			/* returnアドレスを保存 */
	SREG gp, 1*REGSIZE(sp)			/* call先セーブのレジスタを保存 */
	SREG tp, 2*REGSIZE(sp)
	SREG s0, 3*REGSIZE(sp)
	SREG s1, 4*REGSIZE(sp)
	SREG s2, 5*REGSIZE(sp)
	SREG s3, 6*REGSIZE(sp)
	SREG s4, 7*REGSIZE(sp)
	SREG s5, 8*REGSIZE(sp)
	SREG s6, 9*REGSIZE(sp)
	SREG s7, 10*REGSIZE(sp)
	SREG s8, 11*REGSIZE(sp)
	SREG s9, 12*REGSIZE(sp)
	SREG s10, 13*REGSIZE(sp)
	SREG s11, 14*REGSIZE(sp)

	la a1, runtsk					/* runtskをa0に */
	LREG a0, 0(a1)
	SREG sp, TCB_sp(a0)				/* タスクスタックを保存 */
	la a1, dispatch_r
	SREG a1, TCB_pc(a0)				/* 実行再開番地を保存 */

/* 現在実行中のコンテキストを捨てて、タスクディスパッチャを呼び出す時 */
exit_and_dispatch:
	la a2, inest_lvl
	sh zero, 0(a2)					/* ネストカウンタをクリア */

dispatcher:
	/* ここには CPUロック（割り込み禁止）で来ること */
	la a1, schedtsk					/* schedtskをa0に取り出す */
	LREG a0, 0(a1)
	la a2, runtsk					/* schedtskをruntskにセット */
	SREG a0, 0(a2)					/* (タスクスイッチの実行) */
#ifdef	LOG_ANALYSIS
	addi sp, sp, -4*REGSIZE
	SREG a0, 2*REGSIZE(sp)			/* スクラッチレジスタを保存 */
	SREG a1, 3*REGSIZE(sp)
	jal iana_tsk					/* タスク実行情報の設定 */
	LREG a1, 3*REGSIZE(sp)			/* スクラッチレジスタの復帰 */
	LREG a0, 2*REGSIZE(sp)
	addi sp, sp, 4*REGSIZE
#endif	/* 	LOG_ANALYSIS */
	beqz a0, dispatcher_2			/* schedtsk が無ければ割込み待ちへ */

dispatcher_1:
	LREG sp, TCB_sp(a0)				/* タスクスタックを復帰 */
	LREG a1, TCB_pc(a0)				/* 実行再開番地へ分岐 */
	jr a1

	/*
	 *  実行すべきタスクが現れるまで待つ処理
	 *
	 *  ここでコンテキストを切り換えるのは，
	 *    ・ここで発生する割込み処理にどのスタックを使うかという問題の解決
	 *    ・割込みハンドラ内でのタスクディスパッチの防止
	 *  という二つの意味がある．
	 */ 
dispatcher_2:
	la a2, runtsk
	SREG zero, 0(a2)				/* runtsk = NULL */

	/*
	 *  CPUロック状態を解除する準備をする．
	 */
	csrr a5, mie					/* mieをa5に保存 */
	la a3, lock_flag				/* CPUロック解除状態に */
	SREG zero, 0(a3)

	li sp, STACKTOP					/* スタック切り替え */
	la a3, inest_lvl				/* inest_lvl = 1 */
	li a1, 1
	sh a1, 0(a3)

dispatcher_3:
	la a1, kernel_mie				/* 割込み待ち */
	LREG a2, 0(a1)
	csrs mie, a2
	nop
	nop
	csrw mie, a5					/* 元の状態に戻す */

	la a2, reqflg					/* reqflgがfalseならdispatcher_2へ */
	LREG a1, 0(a2)
	beqz a1, dispatcher_3
	SREG zero, 0(a2)				/* reqflgをfalseに */
	sh zero, 0(a3)					/* inest_lvl = 0 */

	/*
	 *  CPUロック状態に戻す．割込み待ちの間に実行した割込みハンドラによ
	 *  り，saved_iipmが書き換えられる可能性があるため，元の値に戻す必
	 *  要がある．dispatcherが実行される時は，mieがゼロとなっているため
	 *  ここではmieを元の状態に戻せばよい．
	 */
	la a3, lock_flag				/* CPUロック状態に */
	li a1, TRUE
	SREG a1, 0(a3)
	j dispatcher

