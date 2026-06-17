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
 *  $Id: cpu_support.S 1.1 2025-11-04 11:07:11Z roi $
 */

/*
 *		プロセッサ依存モジュール アセンブリ言語部（RISC-V用）
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

#ifdef DEFAULT_INTERRUPT

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
	.text
	.align 4
	.global trap_entry
	.type trap_entry, %function
trap_entry:
	addi sp, sp, -SAVEREG2*REGSIZE	/* ユーザースタックにワークレジスタの保存領域を作成 */
	SREG t3, 18*REGSIZE(sp)			/* t3-t6をワークレジスタとして使用する */
	SREG t4, 19*REGSIZE(sp)
	SREG t5, 20*REGSIZE(sp)
	SREG t6, 21*REGSIZE(sp)

	li t5, STACKTOP					/* スタック切り替え */
	mv t4, sp						/* ユーザースタックをt4にコピー */
	mv sp, t5						/* システムスタックを設定 */

	addi sp, sp, -SAVEREG2*REGSIZE	/* システムスタック保存領域を作成 */
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
#if SAVEREG2 == 42
	SFREG ft0, 22*REGSIZE(sp)
	SFREG ft1, 23*REGSIZE(sp)
	SFREG ft2, 24*REGSIZE(sp)
	SFREG ft3, 25*REGSIZE(sp)
	SFREG ft4, 26*REGSIZE(sp)
	SFREG ft5, 27*REGSIZE(sp)
	SFREG ft6, 28*REGSIZE(sp)
	SFREG ft7, 29*REGSIZE(sp)
	SFREG fa0, 30*REGSIZE(sp)
	SFREG fa1, 31*REGSIZE(sp)
	SFREG fa2, 32*REGSIZE(sp)
	SFREG fa3, 33*REGSIZE(sp)
	SFREG fa4, 34*REGSIZE(sp)
	SFREG fa5, 35*REGSIZE(sp)
	SFREG fa6, 36*REGSIZE(sp)
	SFREG fa7, 37*REGSIZE(sp)
	SFREG ft8, 38*REGSIZE(sp)
	SFREG ft9, 39*REGSIZE(sp)
	SFREG ft10, 40*REGSIZE(sp)
	SFREG ft11, 41*REGSIZE(sp)
#endif

	csrr a0, mcause					/* mcauseを2に保存 */
	SREG a0, 2*REGSIZE(sp)
	csrr t5, mepc					/* mpcを1に保存 */
	SREG t5, 1*REGSIZE(sp)
	csrr t6, mstatus				/* mstatusを0に保存 */
	SREG t6, 0*REGSIZE(sp)
	csrr t3, mie					/* mieを3に保存 */
	SREG t3, 3*REGSIZE(sp)
	SREG t4, 4*REGSIZE(sp)			/* ユーザースタックを4に保存 */

	mv a1, sp						/* スタックをa1に設定 */
	add sp, sp, -16
	call handle_trap				/* MACHINEハンドラを呼び出す */
	add sp, sp, 16

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
#if SAVEREG2 == 42
	LFREG ft0, 22*REGSIZE(sp)
	LFREG ft1, 23*REGSIZE(sp)
	LFREG ft2, 24*REGSIZE(sp)
	LFREG ft3, 25*REGSIZE(sp)
	LFREG ft4, 26*REGSIZE(sp)
	LFREG ft5, 27*REGSIZE(sp)
	LFREG ft6, 28*REGSIZE(sp)
	LFREG ft7, 29*REGSIZE(sp)
	LFREG fa0, 30*REGSIZE(sp)
	LFREG fa1, 31*REGSIZE(sp)
	LFREG fa2, 32*REGSIZE(sp)
	LFREG fa3, 33*REGSIZE(sp)
	LFREG fa4, 34*REGSIZE(sp)
	LFREG fa5, 35*REGSIZE(sp)
	LFREG fa6, 36*REGSIZE(sp)
	LFREG fa7, 37*REGSIZE(sp)
	LFREG ft8, 38*REGSIZE(sp)
	LFREG ft9, 39*REGSIZE(sp)
	LFREG ft10, 40*REGSIZE(sp)
	LFREG ft11, 41*REGSIZE(sp)
#endif
	LREG t5, 1*REGSIZE(sp)			/* mepcの取り出し */
	LREG t4, 2*REGSIZE(sp)			/* mcauseの取り出し */
	LREG t3, 3*REGSIZE(sp)			/* mieの取り出し */
	LREG t6, 0*REGSIZE(sp)			/* mstatusの取り出し */
	LREG sp, 4*REGSIZE(sp)			/* ユーザースタックを復帰 */

	SREG t6, 0*REGSIZE(sp)			/* mstatusの保存 */
	SREG t3, 3*REGSIZE(sp)			/* mieの保存 */
	SREG t4, 2*REGSIZE(sp)			/* mcauseの保存 */
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
#if SAVEREG2 == 42
	SFREG ft0, 22*REGSIZE(sp)
	SFREG ft1, 23*REGSIZE(sp)
	SFREG ft2, 24*REGSIZE(sp)
	SFREG ft3, 25*REGSIZE(sp)
	SFREG ft4, 26*REGSIZE(sp)
	SFREG ft5, 27*REGSIZE(sp)
	SFREG ft6, 28*REGSIZE(sp)
	SFREG ft7, 29*REGSIZE(sp)
	SFREG fa0, 30*REGSIZE(sp)
	SFREG fa1, 31*REGSIZE(sp)
	SFREG fa2, 32*REGSIZE(sp)
	SFREG fa3, 33*REGSIZE(sp)
	SFREG fa4, 34*REGSIZE(sp)
	SFREG fa5, 35*REGSIZE(sp)
	SFREG fa6, 36*REGSIZE(sp)
	SFREG fa7, 37*REGSIZE(sp)
	SFREG ft8, 38*REGSIZE(sp)
	SFREG ft9, 39*REGSIZE(sp)
	SFREG ft10, 40*REGSIZE(sp)
	SFREG ft11, 41*REGSIZE(sp)
#endif

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

	LREG a3, 0*REGSIZE(sp)			/* mstatusの取り出し */
	csrw mstatus, a3				/* mstatusの復帰 */
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
#if SAVEREG3 == 26
	SFREG fs0, 14*REGSIZE(sp)
	SFREG fs1, 15*REGSIZE(sp)
	SFREG fs2, 16*REGSIZE(sp)
	SFREG fs3, 17*REGSIZE(sp)
	SFREG fs4, 18*REGSIZE(sp)
	SFREG fs5, 19*REGSIZE(sp)
	SFREG fs6, 20*REGSIZE(sp)
	SFREG fs7, 21*REGSIZE(sp)
	SFREG fs8, 22*REGSIZE(sp)
	SFREG fs9, 23*REGSIZE(sp)
	SFREG fs10, 24*REGSIZE(sp)
	SFREG fs11, 25*REGSIZE(sp)
#endif
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
#if SAVEREG3 == 26
	LFREG fs0, 14*REGSIZE(sp)
	LFREG fs1, 15*REGSIZE(sp)
	LFREG fs2, 16*REGSIZE(sp)
	LFREG fs3, 17*REGSIZE(sp)
	LFREG fs4, 18*REGSIZE(sp)
	LFREG fs5, 19*REGSIZE(sp)
	LFREG fs6, 20*REGSIZE(sp)
	LFREG fs7, 21*REGSIZE(sp)
	LFREG fs8, 22*REGSIZE(sp)
	LFREG fs9, 23*REGSIZE(sp)
	LFREG fs10, 24*REGSIZE(sp)
	LFREG fs11, 25*REGSIZE(sp)
#endif
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
#if SAVEREG2 == 42
	LFREG ft0, 22*REGSIZE(sp)
	LFREG ft1, 23*REGSIZE(sp)
	LFREG ft2, 24*REGSIZE(sp)
	LFREG ft3, 25*REGSIZE(sp)
	LFREG ft4, 26*REGSIZE(sp)
	LFREG ft5, 27*REGSIZE(sp)
	LFREG ft6, 28*REGSIZE(sp)
	LFREG ft7, 29*REGSIZE(sp)
	LFREG fa0, 30*REGSIZE(sp)
	LFREG fa1, 31*REGSIZE(sp)
	LFREG fa2, 32*REGSIZE(sp)
	LFREG fa3, 33*REGSIZE(sp)
	LFREG fa4, 34*REGSIZE(sp)
	LFREG fa5, 35*REGSIZE(sp)
	LFREG fa6, 36*REGSIZE(sp)
	LFREG fa7, 37*REGSIZE(sp)
	LFREG ft8, 38*REGSIZE(sp)
	LFREG ft9, 39*REGSIZE(sp)
	LFREG ft10, 40*REGSIZE(sp)
	LFREG ft11, 41*REGSIZE(sp)
#endif
	LREG sp, 4*REGSIZE(sp)			/* ユーザースタックを復帰 */

	LREG t3, 18*REGSIZE(sp)
	LREG t4, 19*REGSIZE(sp)
	LREG t5, 20*REGSIZE(sp)
	LREG t6, 21*REGSIZE(sp)
	addi sp, sp, SAVEREG2*REGSIZE
	mret

/*
 *  MACHINEネスト割込み出入口処理
 *
 *  trap_nestは，ネストしたMACHINE割込みの出入口処理を行う．
 *  mtvecの設定は呼び出したhandle_trapで行う．
 *  handle_trapからのリターンはネスト状態に限定されるため
 *  この出入り口では、スタックの変更も行わない．
 */
	.text
	.align 4
	.global trap_nest
	.type trap_nest, %function
trap_nest:
	addi sp, sp, -SAVEREG2*REGSIZE	/* システムスタック保存領域を作成 */
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
	SREG t3, 18*REGSIZE(sp)
	SREG t4, 19*REGSIZE(sp)
	SREG t5, 20*REGSIZE(sp)
	SREG t6, 21*REGSIZE(sp)
#if SAVEREG2 == 42
	SFREG ft0, 22*REGSIZE(sp)
	SFREG ft1, 23*REGSIZE(sp)
	SFREG ft2, 24*REGSIZE(sp)
	SFREG ft3, 25*REGSIZE(sp)
	SFREG ft4, 26*REGSIZE(sp)
	SFREG ft5, 27*REGSIZE(sp)
	SFREG ft6, 28*REGSIZE(sp)
	SFREG ft7, 29*REGSIZE(sp)
	SFREG fa0, 30*REGSIZE(sp)
	SFREG fa1, 31*REGSIZE(sp)
	SFREG fa2, 32*REGSIZE(sp)
	SFREG fa3, 33*REGSIZE(sp)
	SFREG fa4, 34*REGSIZE(sp)
	SFREG fa5, 35*REGSIZE(sp)
	SFREG fa6, 36*REGSIZE(sp)
	SFREG fa7, 37*REGSIZE(sp)
	SFREG ft8, 38*REGSIZE(sp)
	SFREG ft9, 39*REGSIZE(sp)
	SFREG ft10, 40*REGSIZE(sp)
	SFREG ft11, 41*REGSIZE(sp)
#endif

	csrr a3, mstatus			    /* mstatusを0に保存 */
	SREG a3, 0*REGSIZE(sp)
	csrr a1, mie					/* mieを3に保存 */
	SREG a1, 3*REGSIZE(sp)
	SREG sp, 4*REGSIZE(sp)			/* システムスタックを4に保存 */
	csrr a0, mcause					/* mcauseをa0に取り出し */
	SREG a0, 2*REGSIZE(sp)			/* mcauseを2に保存 */
	csrr a3, mepc					/* mpcを1に保存 */
	SREG a3, 1*REGSIZE(sp)

	mv a1, sp						/* スタックをa1に設定 */
	add sp, sp, -16
	call handle_trap				/* MACHINEハンドラを呼び出す */
	add sp, sp, 16
	j ret_int_3

/*
 *  タスクディスパッチャ
 */
	.text
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
#if SAVEREG1 == 27
	SFREG fs0, 15*REGSIZE(sp)
	SFREG fs1, 16*REGSIZE(sp)
	SFREG fs2, 17*REGSIZE(sp)
	SFREG fs3, 18*REGSIZE(sp)
	SFREG fs4, 19*REGSIZE(sp)
	SFREG fs5, 20*REGSIZE(sp)
	SFREG fs6, 21*REGSIZE(sp)
	SFREG fs7, 22*REGSIZE(sp)
	SFREG fs8, 23*REGSIZE(sp)
	SFREG fs9, 24*REGSIZE(sp)
	SFREG fs10, 25*REGSIZE(sp)
	SFREG fs11, 26*REGSIZE(sp)
#endif
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
	la a1, trap_nest
	csrw mtvec, a1					/* trap_nestを設定 */

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
	la a1, trap_entry
	csrw mtvec, a1					/* trap_entryに戻す */
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

#endif	/* DEFAULT_INTERRUPT */

/*
 *  ディスパッチャ出口処理
 *  ・ディスパッチャ処理において、a0 = runtsk となっていることに注意
 *  ・スタックポインタも設定済み
 */
	.global dispatch_r
	.type dispatch_r, %function
dispatch_r:
	LREG ra, 0*REGSIZE(sp)			/* returnアドレスを復帰 */
	LREG gp, 1*REGSIZE(sp)			/* call先セーブのレジスタを復帰 */
	LREG tp, 2*REGSIZE(sp)
	LREG s0, 3*REGSIZE(sp)
	LREG s1, 4*REGSIZE(sp)
	LREG s2, 5*REGSIZE(sp)
	LREG s3, 6*REGSIZE(sp)
	LREG s4, 7*REGSIZE(sp)
	LREG s5, 8*REGSIZE(sp)
	LREG s6, 9*REGSIZE(sp)
	LREG s7, 10*REGSIZE(sp)
	LREG s8, 11*REGSIZE(sp)
	LREG s9, 12*REGSIZE(sp)
	LREG s10, 13*REGSIZE(sp)
	LREG s11, 14*REGSIZE(sp)
#if SAVEREG1 == 27
	LFREG fs0, 15*REGSIZE(sp)
	LFREG fs1, 16*REGSIZE(sp)
	LFREG fs2, 17*REGSIZE(sp)
	LFREG fs3, 18*REGSIZE(sp)
	LFREG fs4, 19*REGSIZE(sp)
	LFREG fs5, 20*REGSIZE(sp)
	LFREG fs6, 21*REGSIZE(sp)
	LFREG fs7, 22*REGSIZE(sp)
	LFREG fs8, 23*REGSIZE(sp)
	LFREG fs9, 24*REGSIZE(sp)
	LFREG fs10, 25*REGSIZE(sp)
	LFREG fs11, 26*REGSIZE(sp)
#endif
	addi sp, sp, SAVEREG1*REGSIZE	/* レジスタ保存領域を解放 */
	lbu a1, TCB_enatex(a0)			/* enatexがfalseならret_int_2へ */
	andi a1, a1, TCB_enatex_mask
	beqz a1, dispatch_r_1
	LREG a1, TCB_texptn(a0)			/* texptnが0ならリターン */
	bnez a1, call_texrtn			/* タスク例外処理ルーチンの呼出し */
dispatch_r_1:
	ret

/*
 *  タスク起動処理
 *  ・ディスパッチャ処理において、a0 = runtsk となっていることに注意
 *  ・スタックポインタも設定済み
 */
	.text
	.global activate_r
	.type activate_r, %function
activate_r:
	la a1, lock_flag				/* CPUロック解除状態に */
	SREG zero, 0(a1)
	la ra, ext_tsk					/* ext_tskを戻り番地に設定 */
	li a2, KERNEL_MIE
	csrrs a2, mie, a2				/* 割込み許可 */
	LREG a1, TCB_tinib(a0)			/* runtsk->tinibをa1に */
	LREG a0, TINIB_exinf(a1)		/* exinfを引数レジスタa0に  */
	LREG a2, TINIB_task(a1)			/* タスクの起動番地をa2に */
	la ra, ext_tsk					/* タスクからの戻り番地を設定 */
	jr a2

/*
 *  微少時間待ち（本来はSILのターゲット依存部）
 */
	.global sil_dly_nse
	.type sil_dly_nse, %function
	.align 4
sil_dly_nse:
	addi  a0, a0, (-SIL_DLY_TIM1)	/* a0からSIL_DLY_TIM1を引く */
	blez  a0, _sil_dly_nse_2		/* 結果が0以下ならリターン */
_sil_dly_nse_1:
	addi  a0, a0, (-SIL_DLY_TIM2)	/* a0からSIL_DLY_TIM2を引く */
	bgtz  a0, _sil_dly_nse_1		/* 結果が0より大きければループ */
_sil_dly_nse_2:
	ret

