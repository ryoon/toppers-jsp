/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id$
 */

/*
 *  ターゲット依存モジュール アセンブリ言語部（ARMv7a用）
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include "sys_asm.inc"
#include "cpu_insn.h"
#include "gic_config.h"

/*
 * 割込みハンドラ
 */
	.text
	.align 2
	.global irq_handler
irq_handler:
	/*
	 * IRQモードで実行される
	 */
	sub lr, lr, #4 /* 戻り先アドレス  */

	/*
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC!

	/*
	 *  割込み前のモード(スーパーバイザーモード)へ
	 *  移行し，コンテキストを保存する
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r3,ip,lr}
#if defined(SUPPORT_VFPv3_NEON)
	fmrx r0, fpscr
	push {r0}
	vpush {d0-d7} /* スクラッチレジスタの保存 */
#endif /* SUPPORT_VFPv3_NEON */

	/*
	 *  TPCBの取得
	 */
    my_tpcb r0, r3
    mov lr, sp				/* この時点のスタックを復帰のため取得 */

	/*
	 *  割り込み要因の判定．
	 */
	ldr r2, =(GICC_IAR)
	ldr r12, [r2]
	mov r1, r12, lsl #22	/* 下位10ビット以外をクリア */
	mov r1, r1, lsr #22

	/*
	 *  spurious割込み(INT_ID=1023(0x03ff))の場合，割込み処理を終了
	*/
	ldr r2, =(0x03ff)
	cmp r2, r1
	beq ret_int_1

#ifdef USE_IPI_DIS_HANDER_BYPASS
	cmp r1, #GIC_INTNO_DIS
	ldreq r2,=GICC_EOIR
	streq r12,[r2]			/* EOIレジスタへの書き込み */
	beq  ret_int_ipi_dispatch
#endif /* USE_IPI_DIS_HANDER_BYPASS */

	/*
	 *  多重割込みか判定
	 */
	ldr r3, [r0, #TPCB_excpt_nest_count]	/* 例外・割込みネスト回数を取得 */
	add r2, r3, #1			/* 例外・割込みネスト回数を更新 */
	str r2, [r0, #TPCB_excpt_nest_count]
	cmp r3, #0

	/*
	 *  多重割込みの場合はスタックを8バイト境界に調整
	 */
	andne r2, sp, #4
	subne sp, sp, r2

	/*
	 * 一段目の例外（割込み）ならスタックを変更する
	 */
	ldreq sp, [r0, #TPCB_stacktop]
	stmfd sp!, {lr}			/* 復帰用のスタックポインタの保存 -4 */

	/*
	 *  割込みハンドラのアドレスを取得
	 */
	ldr r3, [r0, #TPCB_p_inth_tbl]	/* 割込みハンドラテーブルの読み込み */
	ldr r0, [r3, r1, lsl #2]		/* r0<-割込みハンドラ */

	/*
	 *  割込み要因の割込み優先度を求め(モデル上の)割込み優先度マスクをセット
	 *  する．またその際，ハンドラ実行前の(モデル上の)割込み優先度マスクを
	 *  保存する．
	 */
	ldr lr, =(GICC_RPR)
	ldr r2, [lr]
	ldr r3, =(GICC_PMR)		/* 割込み発生前の割込み優先度マスクを取得 */
	ldr lr, [r3]
	str r2, [r3]			/* 新しい割込み優先度マスクをセットする */
    stmfd sp!, {lr}			/* 割込み発生前の割込み優先度マスクをスタックに保存 -8 */

    stmfd sp!,{r12}			/* GICC_IAR から取得した内容を保存  -12 */
    stmfd sp!,{r1}			/* inhno を保存  -16 */

    /*
     * 割り込み許可
     */
#ifndef ENABLE_NMFI
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_ALWAYS_SET)
#endif  /* ENABLE_NMFI */

    /*
     * 割込みハンドラの呼び出し
     */
	mov r1, r0
	mov r0, sp
	mov lr, pc
	blx r1

	my_tpcb r0, r2			/* スピンロック設定の検証 */
	ldr r2, [r0, #TPCB_locspnid]
	tst r2, r2				/* スピンロックが設定されていなければスキップ */
	beq irq_ret_int
	bl spin_lock_error_handler

irq_ret_int:
	ldmfd sp!,{r0}			/* inhno を復帰  -12 */
	ldmfd sp!,{r12}			/* GICC_IAR から取得した内容を取得  -8 */
	ldr  r1,=GICC_EOIR
	str  r12,[r1]			/* EOIレジスタへの書き込み */

	/*
	 * カーネル管理の割込みを禁止する
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)

	/*
	 * 割込み優先度マスクを元に戻す
	 */
	ldmfd sp!,{r1}			/* 元の割込み優先度マスクを取得 +12 */
	ldr r0, =(GICC_PMR)		/* DICにセット */
	str r1, [r0]

	/*
	 * スタックポインタの復帰
	 */
	ldmfd sp!,{r2}			/* 元のスタックポインタを取得 */
	mov sp, r2

	/*
	 * 後の処理はARM依存部で実行
	 */
	b ret_int


/*
 * ターゲット依存の例外入口処理
 * ARMv6/7はARMv4と呼び出し時条件が異なるため注意すること．
 * スーパーバイザーモードで呼び出される．
 */
	.text
	.global gic_exc_handler
gic_exc_handler:
#if defined(SUPPORT_VFPv3_NEON)
	fmrx r0, fpscr
	push {r0}
	vpush {d0-d7}		/* スクラッチレジスタの保存 */
#endif /* SUPPORT_VFPv3_NEON */

	mov lr, sp			/* この時点のスタックを復帰のため取得 */

	ldr r0, =(GICC_PMR)	/* 例外発生前の割込み優先度マスクを取得 */
	ldr r3, [r0]
	asr r3, r3, #GIC_PRI_SHIFT	/* 外部表現に変更 */
	stmfd sp!, {r3}	/* 例外発生前の割込み優先度マスクをスタックに保存 */

	/*
	 *  コンテキスト判定のため，excpt_nest_count をスタックに保存．
	 *  スタックに保存せず，現在のexcpt_nest_countを-1すると取得できるが，
	 *  スタックに積んでおいた方がデバッグ等が行いやすいので，スタックに
	 *  保存する．
	 */
	my_tpcb r0, r3
	ldr r3, [r0, #TPCB_excpt_nest_count]
	stmfd sp!, {r3}
	mov r3, sp			/* 例外フレーム番地を保存 */

    /*
     * カーネル管理外のCPU例外か判定する
     *
     * カーネル管理外のCPU例外は，カーネル実行中，全割込みロック状態，
     * CPUロック状態，カーネル管理外の割込みハンドラ実行中に発生した
     * CPU例外である．ARMの場合は，SPSRのI/Fフラグのいずれかが'1'なら，
     * 該当する．
     */
    ldr r1, [sp, #P_EXCINF_OFFSET_CPSR*4]     /* 例外フレームからspsr(例外発生時のcpsr)を取得 */
    tst r1, #(CPSR_FIQ_BIT | CPSR_IRQ_BIT)
    bne gic_kernel_unc_exc_handler /* カーネル管理外のCPU例外の処理へ */

    /*
     *  例外発生時のコンテキストを判定
     */
	ldr r1, [r0, #TPCB_excpt_nest_count]
	add r1, r1, #1
	str r1, [r0, #TPCB_excpt_nest_count]
	cmp r1, #1

	/*
	 *  多重割込みの場合はスタックを8バイト境界に調整
	 */
	andne r1, sp, #4
	subne sp, sp, r1

	/*
	 * タスクコンテキストでCPU例外が発生した場合
	 * スタックを非タスクコンテキストに切り替える．
	 */
	ldreq sp, [r0, #TPCB_stacktop]
	stmfd sp!, {r0, lr}	/* 復帰用のスタックポインタの保存(lr), r0はダミー -8 */

	/*
	 * 割込み発生前の割込み優先度マスクをスタックに保存
	 * 割込みハンドラと出口ルーチンを共有するために保存
	 */
	ldr r12, =(GICC_PMR) /* 例外発生前の割込み優先度マスクを取得 */
	ldr r1, [r12]
	stmfd sp!, {r1}  /* 例外発生前の割込み優先度マスクをスタックに保存 -12 */

	/*
	 *  CPU例外ハンドラの起動番地を取得
	 */
	ldr r12, [r0, #TPCB_p_exch_tbl]	/* 割込みハンドラテーブルの読み出し */
	ldr r1, [r12, r2, lsl #2]		/* r1<-例外ハンドラ                 */
	stmfd sp!,{r2}      /* excno を保存 -16 */

	/*
	 * 割り込み許可
	 * CPU管理外の例外ハンドラは別ルーチンで呼び出すため，単純にSVCモード
	 * にするだけでよい．
	 */
#ifndef ENABLE_NMFI
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_ALWAYS_SET)
#endif /* ENABLE_NMFI */

	/*
	 * CPU例外ハンドラの呼び出し
	 *
	 * 例外フレームの先頭を引数として渡す
	 */
	mov r0,  r3
	mov lr,  pc
    bx r1
	ldmfd sp!,{r0}		/* excno を復帰  -12 */

	/*
	 * カーネル管理の割込みを禁止する
	 */
	msr   cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)

	/*
	 * 割込み優先度マスクを元に戻す
	 */
	ldmfd sp!,{r1}			/* 元の割込み優先度マスクを取得 -8 */
	ldr r0, =(GICC_PMR)		/* DICにセット */
	str r1, [r0]

	/*
	 * スタックポインタの復帰
	 */
	ldmfd sp!,{r0, r2}		/* 元のスタックポインタを取得(r2)、r0はダミー */
	mov sp, r2

	/*
	 * 後の処理はARM依存部で実行
	 */
	b ret_exc


/*
 * カーネル管理外のCPU例外の出入口処理
 */
gic_kernel_unc_exc_handler:
	/*
	 * 割込み発生時のコンテキストを判定
	 */
	ldr r1, [r0, #TPCB_excpt_nest_count]
	add r1, r1, #1
	str r1, [r0, #TPCB_excpt_nest_count]
	cmp r1, #1

	/*
	 *  多重割込みの場合はスタックを8バイト境界に調整
	 */
	andne r1, sp, #4
	subne sp, sp, r1

	/*
	 * タスクコンテキストでCPU例外が発生した場合
	 * スタックを非タスクコンテキストに切り替える．
	 */
	ldreq sp, [r0, #TPCB_stacktop]
	stmfd sp!, {r1,lr}		/* 復帰用のスタックポインタ(lr)の保存,r1はスタック境界調整のためのダミー -8 */

	/*
	 *  CPU例外ハンドラの起動番地を取得
	 */
	ldr r1, [r0, #TPCB_p_exch_tbl]	/* 割込みハンドラテーブルの読み出し */
	ldr r1, [r1, r2, lsl #2]		/* r1<-例外ハンドラ                 */

	/*
	 * システム状態（コンテキストは除く）を，CPU例外発生時の状態へ
	 */
	ldr r0, [r3, #P_EXCINF_OFFSET_CPSR*4]     /* CPU例外発生前のCPSRの取得 */
	and r0, r0, #(CPSR_IRQ_BIT | CPSR_FIQ_BIT)
	orr r0, r0, #(CPSR_SVC | CPSR_ALWAYS_SET)
	msr cpsr_cxsf, r0

	/*
	 * CPU例外ハンドラの呼び出し
	 * 例外フレームの先頭を引数として渡す
	 */
	mov r0, r3
	mov lr, pc
	bx  r1

	/*
	 *  例外・割込みのネストカウント（excpt_nest_count)のデクリメント
	 */
	my_tpcb r0, r3
	ldr r1, [r0, #TPCB_excpt_nest_count]
	sub r1, r1, #1
	str r1, [r0, #TPCB_excpt_nest_count]

	/*
	 * スタックポインタの復帰
	 */
	ldmfd sp!, {r1,r2}	/* 元のスタックポインタ(r2)を取得，r1はスタック境界調整のためのダミー +0 */
	mov sp, r2

	/*
	 * CPU例外からの復帰
	 */
#if defined(SUPPORT_VFPv3_NEON)
 	vpop  {d0-d7}		/* スクラッチレジスタの復帰 */
	pop   {r0}			/* FPSCRレジスタの復帰  */
	vmsr  fpscr, r0
#endif /* SUPPORT_VFPv3_NEON */
	ldmfd sp!,{r0-r3,ip,lr}
	rfeia sp!
