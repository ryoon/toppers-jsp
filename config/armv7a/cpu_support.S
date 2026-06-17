/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
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
 *        コア依存モジュール アセンブリ言語部（ARMv7a用）
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"
#include "sys_asm.inc"

/*
 * 例外ベクタ
 */
	.section vector
	.global vector_table
	.syntax unified
vector_table:
	ldr pc, reset_vector       /* リセット             */
	ldr pc, undef_vector       /* 未定義命令           */
	ldr pc, swi_vector         /* ソフトウェア割込み   */
	ldr pc, prefech_vector     /* プリフェッチアボート */
	ldr pc, data_abort_vector  /* データアボート       */
	ldr pc, reset_vector
	ldr pc, irq_vector         /* IRQ                  */
	ldr pc, fiq_vector         /* FIQ                  */


/*
 *  例外ベクタの命令から参照される
 *  ジャンプ先アドレス
 */
	.global vector_ref_tbl, vector_ref_tbl_end
vector_ref_tbl:
reset_vector:
	.long   start
undef_vector:
	.long   undef_handler
swi_vector:
	.long   swi_handler
prefech_vector:
	.long   prefetch_handler
data_abort_vector:
	.long   data_abort_handler
irq_vector:
	.long   irq_handler
fiq_vector:
	.long   fiq_handler
vector_ref_tbl_end:

/*
 * CPU例外ハンドラ
 *
 * CPU例外ハンドラは，非タスクコンテキストで実行する．
 *
 */

/*
 *  未定義命令 例外ハンドラ
 */
	.text
	.align 2
	.global undef_handler
undef_handler:
	/*
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC!

	/*
	 *  例外発生前の動作時モード(スーパーバイザーモード)へ
	 *  割込みロック状態，CPUロック状態はCPU例外発生時の
	 *  状態を継承する．
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r3,ip,lr}
	mov r2, #UND_Number
	b  gic_exc_handler

/*
 *  SWI 例外ハンドラ
 */
	.text
	.align 2
	.global swi_handler
swi_handler:
	/*
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC!

	/*
	 *  例外発生前の動作時モード(スーパーバイザーモード)へ
	 *  割込みロック状態，CPUロック状態はCPU例外発生時の
	 *  状態を継承する．
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r3,ip,lr}
	mov r2, #SWI_Number
	b  gic_exc_handler

/*
 *  プリフェッチアボード 例外ハンドラ
 */
	.text
	.align 2
	.global prefetch_handler
prefetch_handler:
	/*
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC!

	/*
	 *  例外発生前の動作時モード(スーパーバイザーモード)へ
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r3,ip,lr}
	mov r2, #PRFA_Number
	b  gic_exc_handler

/*
 *  データアボード 例外ハンドラ
 */
	.text
	.align 2
	.global data_abort_handler
data_abort_handler:
	/*
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC!

	/*
	 *  例外発生前の動作時モード(スーパーバイザーモード)へ
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r3,ip,lr}
	mov r2, #DATAA_Number
	b gic_exc_handler


#ifndef TARGET_FIQ_HANDLER
/*
 *  FIQ 例外ハンドラ
 */
	.text
	.align 2
	.global fiq_handler
fiq_handler:
	/*
	 * lr(戻り先) spsr(cpsr_svc)をスーパーバイザーモードのスタックへ
	 * 保存．
	 */
	srsdb #CPSR_SVC!

	/*
	 *  例外発生前の動作時モード(スーパーバイザーモード)へ
	 */
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)
	stmfd sp!, {r0-r3,ip,lr}
	mov r2, #FIQ_Number
	b  gic_exc_handler
#endif /* TARGET_FIQ_HANDLER */

/*
 *  CPU例外ハンドラ出口処理
 *
 *  ret_excは，CPU例外ハンドラから戻った直後に実行するルーチンで，
 *  CPU例外ハンドラ終了後，ターゲット依存の処理を実行した後，
 *  カーネル管理の割込みを禁止，スタックをCPU例外前のスタックに，
 *  r0にCPU例外発生時に取得しているスピンロックの値を入れた状態
 *  で呼び出される．
 */
	.text
	.global ret_exc, ret_int, ret_int_1
/*
 *  割込みハンドラ/CPU例外ハンドラ出口処理
 *
 *  ret_intは，割込みハンドラから戻った直後に実行するルーチンで，
 *  割込みハンドラ終了後，ターゲット依存の処理を実行した後，
 *  カーネル管理の割込みを禁止，スタックを割込み前のスタックにした
 *  状態で呼び出される．
 */
ret_exc:
ret_int:
	/*
	 *  例外・割込みのネストカウント（excpt_nest_count)のデクリメント
	 */
	my_tpcb r0, r1
	ldr r1, [r0,#TPCB_excpt_nest_count]	/* r1 <-excpt_nest_count */
	sub r2, r1, #1
	str r2, [r0,#TPCB_excpt_nest_count]
	cmp r2, #0				/* 戻り先が非タスクコンテキストなら */
	bne ret_int_1			/* すぐにリターン                   */

	/*
	 *  reqflgをチェックする前に割込みを禁止するのは，reqflgをチェック
	 *  した直後に割込みハンドラが起動され，その中でディスパッチが要求
	 *  された場合に，すぐにディスパッチされないという問題が生じるため
	 *  である．
	 */
	ldr r1, [r0, #TPCB_reqflg]	/* reqflgがtrueであればret_int_2へ */
	cmp r1, #0
	bne ret_int_2

ret_int_1:
	/*
	 *  割込み処理からのリターンにより，CPUロック解除状態に移行しなければ
	 *  ならない．
	 *  ARMでは，CPSRのIRQビットでCPUロック解除状態とするため，単にリターン
	 *  すればよい．
	 */
#if defined(SUPPORT_VFPv3_NEON)
	vpop {d0-d7}			/* スクラッチレジスタの復帰 */
	pop {R0}				/* FPSCRレジスタの復帰      */
	vmsr    fpscr, R0
#endif /* SUPPORT_VFPv3_NEON */
	ldmfd sp!,{r0-r3,ip,lr}
	rfeia sp!

ret_int_2:
	/*
	 *  ここでは，戻り先がタスクであり，スタックは，タスクスタックの上
	 *  にスクラッチレジスタのみが保存された状態になっている．また，
	 *  プロセッサは，スーパーバイザーモード・カーネル管理の割込みを禁止
	 *  した状態となっている．
	 */
	mov r1, #0				/* reqflgをFALSEに */
	str r1, [r0, #TPCB_reqflg]

    /*
     *  CPUロック状態に移行し，割込み優先度マスクを割込み処理前の値に
     *  設定する．
     *
     *  この時点でCPUロック状態とするのは，dispatcherへ分岐する時と，
     *  call_texrtnを呼び出す時に，CPUロック状態になっている必要がある
     *  ためである．
     *
     *  CPUロック状態に関しては，ARMでは，CPSRのIRQビットでCPUロック
     *  状態を表現するため，ここでは，特に何も行わない．
     *
     *  割込み優先度マスクに関しては，ターゲット毎に操作が異なるため，
     *  割込みハンドラ終了直後にターゲット依存部で割込み処理前の値に
     *  設定する．
     */

ret_int_3:
	/*
	 *  dspflgがfalseである場合と，p_runtskとp_schedtskが同じ場合には，
	 *  ディスパッチを行わない．このチェックが必要なのは，タスク例外処
	 *  理ルーチンの呼出しが必要な場合に，ディスパッチが必要なくても，
	 *  reqflgをTRUEにするためである．
	 */
	ldr r1, [r0, #TPCB_runtsk]	/* runtsk をr1に読み込む */
	ldr r2, [r0, #TPCB_enadsp]	/* enadspがfalseでならばret_int_r_1へ */
	cmp r2, #0
	beq ret_int_r_1

	/*
	 *  ディスパッチの必要性をチェック
	 */
	ldr r2, [r0, #TPCB_schedtsk]	/* schedtsk をr2に読み込む */
	cmp r1, r2					/* runtskとschedtskが同じなら */
	beq ret_int_r_1				/*              ret_int_r_1へ */
	/* 
	 * スタックを8バイト境界に調整 
	 */
	and r2, sp, #4
	sub sp, sp, r2
	stmfd sp!, {r2, r3}			/* r3はスタック境界の調整用のダミー */

	stmfd sp!, {r4-r11}			/* 残りのレジスタを保存 -(8*4) */
#if defined(SUPPORT_VFPv3_NEON)
	/* 残りの浮動小数点/NEONレジスタを保存  */
	vpush {d8-d15}
	vpush {d16-d31}
#endif /* SUPPORT_VFPv3_NEON */
	str sp, [r1,#TCB_sp]		/* タスクスタックを保存 */
	adr r2, ret_int_r			/* 実行再開番地を保存   */
	str r2, [r1,#TCB_pc]
	b dispatcher

	.global ret_int_r, _kernel_break_wait
/*
 * 割込みによりプリエンプトされたタスクへのリターン処理
 */
_kernel_break_wait:
ret_int_r:
#if defined(SUPPORT_VFPv3_NEON)
	vpop {d16-d31}
	vpop {d8-d15}
#endif /* SUPPORT_VFPv3_NEON */
	ldmfd sp!, {r4-r11}			/* レジスタの復帰 +(8*4) */
	/* 
	 *  8バイト境界に調整したスタックを元に戻す
	 */
	ldmfd sp!, {r2, r3}			/* r3はスタック境界の調整用のダミー */
	add   sp, sp, r2

ret_int_r_1:
	/*
	 *  enatexがtrueで，texptnが0でなく，ipmflgがtrueであれば，タスク
	 *  例外処理ルーチンを呼び出す．
	 *  dispatcherから呼び出されるため，TCBのアドレスはr1に入っている
	 */
	ldrb r0, [r1,#TCB_enatex]
	tst r0, #TCB_enatex_mask
	beq ret_int_r_2				/* enatex が false なら ret_int_r2 へ */
	ldr r0, [r1,#TCB_texptn]	/* texptnをロード               */
	tst r0, r0					/* texptn が0ならリターン       */
	beq ret_int_r_2

	/*
	 * スタックを8バイト境界に調整してタスク例外ルーチンを呼び出す
	 */
	and r2, sp, #4
	sub sp, sp, r2
	stmfd sp!, {r2, r3}			/* r3はスタック境界の調整用のダミー */
	bl call_texrtn				/* タスク例外ルーチンの呼び出し */
	ldmfd sp!, {r2, r3}			/* r3はスタック境界の調整用のダミー */
	add sp, sp, r2

ret_int_r_2:
	/*
	 *  割込み処理からのリターンにより，CPUロック解除状態に移行するよ
	 *  うにする．ARMはCPSRのIRQビットによってCPUロック状態を表してい
	 *  るため，そのままリターンすればよい．
	 */

#if defined(SUPPORT_VFPv3_NEON)
	vpop {d0-d7}				/* スクラッチレジスタの復帰 */
	pop {R0}					/* FPSCRレジスタの復帰 */
	vmsr fpscr, R0
#endif /* SUPPORT_VFPv3_NEON */
	ldmfd sp!,{r0-r3,ip,lr}
	rfeia sp!

/*
 *  ディスパッチャ本体
 */
	.text
	.align 2
	.global dispatch, exit_and_dispatch
dispatch:
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・ディスパッチ
	 *  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 *  れる．
	 */
	stmfd sp!, {r4-r12,lr}		/* スクラッチレジスタ以外のレジスタの保存,r12はスタック境界調整のためのダミー -(10*4) */
#if defined(SUPPORT_VFPv3_NEON)
	vpush {d8-d15}
	vpush {d16-d31}
#endif /* SUPPORT_VFPv3_NEON */
	my_tpcb r0, r1				/* runtskを読み込む */
	ldr r1, [r0,#TPCB_runtsk]
	str sp, [r1,#TCB_sp]		/* タスクスタックを保存 */
	adr r2, dispatch_r
	str r2, [r1,#TCB_pc]		/* 実行再開番地を保存 */

/*
 *  現在のコンテキストを捨ててディスパッチ
 */
exit_and_dispatch:
    /* ディスパッチャ本体（dispatcher）へ */
    msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET) /* CPUロック状態へ */

/*
 *  ディスパッチャ本体
 */
dispatcher:
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・ディスパッチ
	 *  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 *  れる．実行再開番地へもこの状態のまま分岐する．
	 *
	 *  すなわち，スーパーバイザーモード，IRQ禁止となっている．
	 */
	my_tpcb r1, r0				/* runtskを読み込む */
	ldr r0, [r1, #TPCB_schedtsk]	/* schedtskをruntskに */
	str r0, [r1, #TPCB_runtsk]
#ifdef	LOG_ANALYSIS
    stmfd sp!, {r0,r1}
	bl iana_tsk					/* タスク実行情報の設定 */
	ldmfd sp!, {r0,r1}
#endif	/* 	LOG_ANALYSIS */
	cmp r0, #0					/* runtskがNULLならdispatcher_1へ */
	beq dispatcher_1
	ldr sp, [r0,#TCB_sp]		/* タスクスタックを復帰 */
	ldr pc, [r0,#TCB_pc]		/* 実行再開番地を復帰 */

dispatcher_1:
	/*
	 * CPUロック状態の解除と，非タスクコンテキスト実行状態への
	 * 準備をする
	 */
	my_tpcb r4, r1
	ldr sp, [r4,#TPCB_stacktop]	/* 非タスクコンテキストのスタックへ */
	mov r1, #1
	str r1, [r4, #TPCB_excpt_nest_count]	/* excpt_nest_count = 1 */

dispatcher_2:
	/*
	 *  割込みを許可し，非タスクコンテキスト実行状態とし割込みを待つ．
	 *
	 *  ここで非タスクコンテキスト実行状態に切り換えるのは，ここで発生
	 *  する割込み処理にどのスタックを使うかという問題の解決と，割込み
	 *  ハンドラ内でのタスクディスパッチの防止という2つの意味がある．
	 *
	 *  プロセッサを割込み待ちに移行させる処理と，割込み許可とは，不可
	 *  分に行なう必要がある．
	 *  これを不可分に行なわない場合，割込みを許可した直後に割込
	 *  みが入り，その中でタスクが実行可能状態になると，実行すべきタス
	 *  クがあるにもかかわらずプロセッサが割込み待ちになってしまう．
	 *
	 *  割込み待ちの間は，p_runtskをNULL（＝0）に設定しなければならな
	 *  い．このように設定しないと，割込みハンドラからiget_tidを呼び出
	 *  した際の動作が仕様に合致しなくなる．
	 *
	 *  ターゲットによっては，省電力モード等に移行するため，標準の方法と
	 *  異なる手順が必要な場合がある．
	 *  そのようなターゲットでは，ターゲット依存において，TOPPERS_CUSTOM_IDLE
	 *  を定義し，アセンブラマクロとして，toppers_asm_custom_idle を用意
	 *  すればよい．
	 *
	 *  なお，toppers_asm_custom_idle の記述にあたっては，次のレジスタは
	 *  toppers_asm_custom_idleの前後で使用するため，
	 *  toppers_asm_custom_idle 内で使用する場合は，前後で保存復帰すること．
	 *  これらのレジスタは Calee saved レジスタであるため，
	 *  toppers_asm_custom_idle として関数呼び出しをした場合は，呼び出した
	 *  関数で自動的に保存復帰されるため，アセンブラレベルでの保存復帰は必
	 *  要ない．
	 *
	 *  レジスタ : 内容
	 *   r4      : TPCBのアドレス
	 *   sp      : 非タスクコンテキスト用のスタックの先頭アドレス．
	 */

#ifdef TOPPERS_CUSTOM_IDLE
	toppers_asm_custom_idle
#else
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_ALWAYS_SET)	/* 割り込み許可(システムモード) */
	nop
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_CPULOCK | CPSR_ALWAYS_SET)	/* CPUロック(システムモード) */
#endif /* TOPPERS_CUSTOM_IDLE */
	ldr r0, [r4, #TPCB_reqflg]	/* reqflgがfalseならdispatcher_2へ */
	cmp r0, #0
	beq dispatcher_2
	mov r1, #0
	str r1, [r4, #TPCB_reqflg]	/* reqflgをFALSEに */
	str r1, [r4, #TPCB_excpt_nest_count]	/* excpt_nest_count = 0 */
	b dispatcher

/*
 *  ディスパッチャ出口処理
 *  ・ディスパッチャ処理において、r0 = runtsk となっていることに注意
 *  ・スタックポインタも設定済み
 */
	.global dispatch_r
dispatch_r:
#if defined(SUPPORT_VFPv3_NEON)
	vpop {d16-d31}
	vpop {d8-d15}
#endif /* SUPPORT_VFPv3_NEON */
	ldmfd sp!,{r4-r12,lr}     /* スクラッチレジスタ以外のレジスタの復帰,r12はスタック境界調整のためのダミー -(10*4) */
	/*
	 *  タスク例外処理ルーチンの起動
	 *  dispatcherから呼び出されるため，TCBのアドレスはr1に入っている
	 */
	ldrb r1,[r0,#TCB_enatex]
	tst r1,#TCB_enatex_mask
	beq dispatch_r_1			/* enatex が false ならリターン */
	ldr r1,[r0,#TCB_texptn]		/* texptn が0ならリターン       */
	tst r1,r1
	beq dispatch_r_1
	ldr r0, =call_texrtn		/* タスク例外ルーチンの呼び出し */
	bx r0
dispatch_r_1:
	bx lr

/*
 *  タスク起動処理
 *  ・ディスパッチャ処理において、r0 = runtsk となっていることに注意
 *  ・スタックポインタも設定済み
 */
	.text
	.global activate_r
activate_r:
	msr cpsr_cxsf, #(CPSR_SVC | CPSR_ALWAYS_SET) /* CPUロック解除状態に */
	ldr lr, =ext_tsk			/* 戻り番地設定 */
	ldr r2, [r0, #TCB_tinib]	/* p_runtsk->p_tinibをr2に  */
	ldr r0, [r2, #TINIB_exinf]	/* exinfを引数レジスタr0に  */
	ldr r1, [r2, #TINIB_task]	/* タスク起動番地にジャンプ */
	bx  r1

#if TNUM_PRCID > 1

/*
 *  自タスクのマイグレーション
 *  引数(r0)にマイグレーション先のプロセッサが入っているため破壊しないこと
 */
	.text
	.align 2
	.global dispatch_and_migrate
dispatch_and_migrate:
	stmfd sp!, {r4 - r12,lr}	/* スクラッチレジスタ以外のレジスタの保存,r12はスタック境界調整のためのダミー +(10*4) */
#if defined(SUPPORT_VFPv3_NEON)
	vpush {d8-d15}
	vpush {d16-d31}
#endif /* SUPPORT_VFPv3_NEON */
	my_tpcb r2, r1				/* runtskを読み込む */
	ldr r1, [r2,#TPCB_runtsk]
	str sp, [r1,#TCB_sp]			/* タスクスタックを保存 */
	adr r3, dispatch_r
	str r3, [r1,#TCB_pc]			/* 実行再開番地を保存 */
	ldr sp, [r2,#TPCB_stacktop]
    b      migrate_self				/* 自タスクのマイグレーションルーチンへ */

#endif	/* TNUM_PRCID > 1 */


/*
 *  ディスパッチ要求用のプロセッサ間割込みのバイパス処理
 *
 *  共通部で用意されている ipi_handler を呼び出さずにディスパッチャを
 *  呼び出す．最も外側のルーチンでない場合は，reqflgをセットする．
 *  自PCBのアドレスがr0に入っていることを想定している．
 */
#ifdef USE_IPI_DIS_HANDER_BYPASS
    .global ret_int_ipi_dispatch
ret_int_ipi_dispatch:
    ldr   r2, [r0,#TPCB_excpt_nest_count]	/* 戻り先がタスクコンテキストなら */
    cmp   r2, #0						/* ret_int_3へ */
    beq   ret_int_3
    mov   r1, #1						/* 戻り先が非タスクコンテキストなら */
    str   r1, [r0, #TPCB_reqflg]		/* reqflgをtrueにしてリターン       */
    b     ret_int_1
#endif /* USE_IPI_DIS_HANDER_BYPASS */

/*
 *  微少時間待ち
 */
#ifndef TOPPERS_CUSTOM_SILDLYNSE
	.global sil_dly_nse
sil_dly_nse:
	sub r0, r0, #SIL_DLY_TIM1
	cmp r0, #0
	bgt sil_dly_nse1
	bxle lr
sil_dly_nse1:
	sub r0, r0, #SIL_DLY_TIM2
	cmp r0, #0
	bgt sil_dly_nse1
	bx lr
#endif /* TOPPERS_CUSTOM_SILDLYNSE */

#ifdef __thumb__

	.text
	.align 2
	.global current_sr
current_sr:
	mrs r0, cpsr
	bx lr

	.global set_sr
set_sr:
	msr cpsr_cxsf, r0
	bx lr

#endif /* __thumb__ */

	.text
	.align 2
	.global clean_invalidate_dcache_all
clean_invalidate_dcache_all:
	push {r4, r5, r6, r7, r8, lr}
	dmb
	mov r6, #0
	mov r7, #0
level_loop1:
	mrc 15, 1, r3, cr0, cr0, 1
	lsr r3, r3, r6
	and r3, r3, #7
	sub r3, r3, #2
	cmp r3, #2
	bhi level_loop2
	lsl r5, r7, #1
	mcr 15, 2, r5, cr0, cr0, 0
	isb
	mrc 15, 1, r3, cr0, cr0, 0
	ubfx r4, r3, #3, #10
	ubfx ip, r3, #13, #15
	add r4, r4, #1
	and r3, r3, #7
	add r0, r3, #4
	mov lr, r4
	add ip, ip, #1
	mvn r3, #0
log2_nuw_way_loop:
	lsrs lr, lr, #1
	add r3, r3, #1
	bne log2_nuw_way_loop

	rsb r8, r3, #32
way_loop:
	orr r1, r5, lr, lsl r8
	mov r3, #0
set_loop:
	orr r2, r1, r3, lsl r0
	mcr 15, 0, r2, cr7, cr14, 2
	add r3, r3, #1
	cmp ip, r3
	bne set_loop
	add lr, lr, #1
	cmp lr, r4
	bne way_loop
	dsb
level_loop2:
	add r6, r6, #3
	add r7, r7, #1
	cmp r6, #21
	bne level_loop1
	dsb st
	isb
	pop {r4, r5, r6, r7, r8, pc}
