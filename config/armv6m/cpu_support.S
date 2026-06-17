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
 *  プロセッサ依存モジュール アセンブリ言語部（ARMv6M用）
 */

#define _MACRO_ONLY
#include "jsp_kernel.h"
#include "offset.h"

/*
 *  CPU例外エントリ
 *
 *  割込みエントリと処理の内容は同等だが，ログの種類が異なるため，
 *  分けている．
 */
	.balign 4
	.text
	.global cpu_exc_entry
	.syntax unified
	.thumb
	.thumb_func
	.type cpu_exc_entry, %function
cpu_exc_entry:
	/*
	 *  例外/割込みが発生すると，発生時にアクティブなスタックにスクラ
	 *  ッチレジスタ等が保存される．
	 *  この内容に加えて，CPU例外ハンドラへの情報として，basepri の値と，
	 *  EXC_RETURNの情報を加えて保存する．basepriの値は，CPU例外からの
	 *  リターン時に割込み優先度マスクの値を元に戻すためにも用いられる．
	 *
	 *   -----------
	 *  |   iipm    |
	 *   -----------
	 *  | EXC_RETURN|
	 *   -----------
	 *  |    R0     |
	 *   -----------
	 *  |    R1     |
	 *   -----------
	 *  |    R2     |
	 *   -----------
	 *  |    R3     |
	 *   -----------
	 *  |    R12    |
	 *   -----------
	 *  |    LR     |
	 *   -----------
	 *  |    PC     |
	 *   -----------
	 *  |   xPSR    |
	 *   -----------
	 *
	 */

	/*
	 *  カーネル管理外の例外かチェック
	 *  カーネル内のクリティカルセクションの実行中，全割込みロック状態，
	 *  CPUロック状態，カーネル管理外の割込みハンドラ実行中のいずれかで
	 *  発生したCPU例外を，カーネル管理外のCPU例外と呼ぶ
	 *  全割込みロック状態はPRIMASKが'1'の状態で判断する．
	 */
	ldr r3, =iipm				/* 後で使用するため読み込む */
	ldrb r0, [r3]
	mrs r2, primask				/* CPUロック状態ならカーネル管理外例外処理へ */

	/*
	 * スタックを変更する必要があるかチェック
	 * EXC_RETURN（割込み時にLRに設定される値）をチェックして，例外発生時に
	 * アクティブなスタックを特定することで多重割込みか判定する．
	 */
	movs r1, #EXC_RETURN_PSP	/* 割込み元がMSPなら多重割込み */
	mov r2, lr
	tst r2, r1
	beq  cpu_exc_entry_1		/* 多重割込みならcpu_exc_entry_1へ */
	mrs  r3, psp				/* 一段目の割込みの場合はPSP上に */
	subs r3, #(0x04*2)			/* 割込み発生時のiipm(r0)，EXC_RETURN(r2)の順にPSP上に積む */
	str r2, [r3, 0x04]
	str r0, [r3, 0x00]
	msr psp, r3
	push {r0, r2}				/* MSPにも割込み発生時のiipm(r0)とEXC_RETURN(r2)を積む */
	mov   lr, r3				/* 未定義の割込みが発生した場合の情報とする */
	b cpu_exc_entry_2

cpu_exc_entry_1:			/* 多重割込みの場合 */
	push {r0,r2}				/* 割込み発生時のiipm(r0)，EXC_RETURN(r2)の順にPSP上に積む */
	mov lr, sp					/* CPU例外ハンドラへの引数となる */

	/*
	 *  共通処理
	 */
cpu_exc_entry_2:
	/*
	 *  CPU例外からリターンする際に割込み優先度マスクを戻すためのデータを保存
	 */
	ldr r3, =SYSTIC_CONTROL_STATUS
	ldr r2,[r3] 
	ldr r3, =NVIC_SETENA0
	ldr r1,[r3] 
	push {r1,r2}

	mrs r3, ipsr				/* ハンドラアドレスを取得 */
	ldr r1, =int_handler_table
	lsls r0, r3, #2
	ldr r2, [r1, r0]

	/*
	 *  CPU例外ハンドラの呼び出し
	 */
	mov r0, lr
	blx  r2
	b ret_exc

/*
 *  システムタイマ割込みエントリ
 */
	.global cpu_systic_entry
	.syntax unified
	.thumb
	.thumb_func
	.type cpu_systic_entry, %function
cpu_systic_entry:
	/*
	 *  割込み発生時の割込み優先度マスクをスタックに保存するため取得
	 */
	ldr r3, =iipm				/* 後で使用するため読み込む */
	ldrb r0, [r3]

	/*
	 * 多重割込みかチェック
	 * EXC_RETURN（割込み時にLRに設定される値）をチェックして，例外発生時に
	 * アクティブなスタックを特定することで多重割込みか判定する．
	 */
	movs r1, #EXC_RETURN_PSP	/* 割込み元がMSPなら多重割込み */
	mov r2, lr
	tst r2, r1
	beq cpu_systic_entry_1		/* 多重割込みならcpu_int_entry_1へ */
	mrs r3, psp					/* 一段目の割込みの場合はPSP上に */
	subs r3, #(0x04*2)
	str r2, [r3, 0x04]
	str r0, [r3, 0x00]
	msr psp, r3
	push {r0, r2}               /* MSPにも割込み発生時のiipm(r0)とEXC_RETURN(r2)を積む */
	mov lr, r3					/* 未定義の割込みが発生した場合の情報とする */
	b cpu_systic_entry_2

cpu_systic_entry_1:			/* 多重割込みの場合 */
	push {r0, r2}				/* 割込み発生時のiipm(r0)とEXC_RETURN(r2)を積む */
	mov lr, sp					/* 未定義の割込みが発生した場合の情報とする */

	/*
	 *  共通処理
	 */
cpu_systic_entry_2:
	/*
	 *  割込みからリターンする際に割込み優先度マスクを戻すためのデータを保存
	 */
	ldr r3, =SYSTIC_CONTROL_STATUS
	ldr r2,[r3] 
	ldr r3, =NVIC_SETENA0
	ldr r1,[r3] 
	push {r1,r2}

	mrs r3, ipsr				/* 割込み要因番号の取得 */

	/*
	 *  iipmをシステムタイマの割込みレベルに設定する
	 */
	ldr r1, =NVIC_SYS_PRI3
	ldr r2, [r1]
	lsrs r2, r2, #(32-TBITW_IPRI)
	b cpu_int_entry_3

/*
 *  割込みエントリ
 */
	.global cpu_int_entry
	.syntax unified
	.thumb
	.thumb_func
	.type cpu_int_entry, %function
cpu_int_entry:
	/*
	 *  割込み発生時の割込み優先度マスクをスタックに保存するため取得
	 */
	ldr r3, =iipm				/* 後で使用するため読み込む */
	ldrb r0, [r3]

	/*
	 * 多重割込みかチェック
	 * EXC_RETURN（割込み時にLRに設定される値）をチェックして，例外発生時に
	 * アクティブなスタックを特定することで多重割込みか判定する．
	 */
	movs r1, #EXC_RETURN_PSP	/* 割込み元がMSPなら多重割込み */
	mov r2, lr
	tst r2, r1
	beq cpu_int_entry_1			/* 多重割込みならcpu_int_entry_1へ */
	mrs r3, psp					/* 一段目の割込みの場合はPSP上に */
	subs r3, #(0x04*2)			/* 割込み発生時のiipm(r0)とEXC_RETURN(r2)を積む */
	str r2, [r3, 0x04]
	str r0, [r3, 0x00]
	msr psp, r3
	push {r0, r2}               /* MSPにも割込み発生時のiipm(r0)とEXC_RETURN(r2)を積む */
	mov lr, r3					/* 未定義の割込みが発生した場合の情報とする */
	b cpu_int_entry_2

cpu_int_entry_1:			/* 多重割込みの場合 */
	push {r0, r2}				/* 割込み発生時の割込み優先度マスク(r0)とEXC_RETURN(r2)を積む */
	mov lr, sp					/* 未定義の割込みが発生した場合の情報とする */

	/*
	 *  共通処理
	 */
cpu_int_entry_2:
	/*
	 *  割込みからリターンする際に割込み優先度マスクを戻すためのデータを保存
	 */
	ldr r3, =SYSTIC_CONTROL_STATUS
	ldr r2,[r3] 
	ldr r3, =NVIC_SETENA0
	ldr r1,[r3] 
	push {r1,r2}

	mrs r3, ipsr				/* 割込み要因番号の取得 */

	/*
	 *  iipmを発生した割込みレベルに設定する
	 */
	movs r2, #3
	mov r0, r3					/* 割込み優先度レジスタのオフセットr3-16 = r0 */
	subs r0, #16
	mov r1, r0
	bics r1, r2					/* r1:4バイトアラインオフセット */
	ands r0, r2					/* r0:4バイト中のバイト位置 */
	ldr r2, =NVIC_PRI0			/* 割込み優先度レジスタを取り出す */
	ldr r2, [r1, r2]
	lsls r0, r0, #3				/* 割込み対応の優先度のシフト数を計算 */
	adds r0, #(8-TBITW_IPRI)
	lsrs r2, r0					/* r2:発生した割込みに対応した優先度 */

cpu_int_entry_3:
	ldr   r0, =iipm				/* iipmに優先度を保存 */
	strb  r2, [r0]

	ldr r1, =int_handler_table	/* ハンドラアドレスを取得 */
	lsls r0, r3, #2
	ldr r2, [r1, r0]

	/*
	 *  割込みハンドラの呼び出し
	 */
	mov r0, lr
	blx r2

/*
 *  割込み/例外出口
 *
 *  ret_exc/ret_intは，CPU例外/割込みハンドラから戻った直後に実行する
 *  ルーチンである．
 */
ret_exc:
ret_int:
	/*
	 *  割込みロック状態とする．この時点では，CPUロック状態にはならない
	 *  PRIMASKは，割込みからのリターン処理によって，'0'にクリアされる．
	 */
	cpsid i

	/*
	 *  割込み優先度マスクを元に戻す
	 */
	ldr r1, =0xffffffff
	ldr r2, =NVIC_CLRENA0		/* 一旦IRQ割り込みを全て禁止 */
	str r1, [r2]
	pop {r0,r1}					/* スタックから例外/割込み発生時のNVIC_SETENA0(r0),SYSTIC_CONTROL_STATUS(r1)の値を取得 */
	ldr r2, =NVIC_SETENA0		/* NVIC_SETENA0を元に戻す */
	str r0, [r2]
	ldr r3, =SYSTIC_CONTROL_STATUS	/* SYSTIC_CONTROL_STATUSを元に戻す */
	ldr r0,[r3]
	movs r2, #SYSTIC_TICINT
	orrs r1, r1, r2
	bics r0, r2
	orrs r0, r1
	str r0, [r3]
	pop {r0}					/* iipmを発生前に戻す */
	ldr r1, =iipm
	strb r0, [r1]

	/*
	 *  戻り先のコンテキストの判定
	 * 
	 *  割込みハンドラ実行にLRにセットされるEXC_RETURNをチェックして，戻り
	 *  先でMSPが使われていれば，割込み先が非タスクコンテキストと判定する．
	 */
	pop {r3}					/* lrをスタックから取得         */
	movs r1, #EXC_RETURN_PSP	/* 戻り先がMSPなら ret_int_1 へ */
	tst r3, r1
	beq ret_int_1

	/*
	 *  一段目の割込みの出口処理
	 */
	/*
	 *  PSP上から，EXC_RETURN(r0)と元のタスク実行情報の設定(iipm)(r1)
	 *  を破棄
	 */
	mrs r2, psp
	adds r2, r2, #(0x04*2)
	msr psp, r2

	/*
	 *  reqflgをチェックする
	 * 
	 *  カーネル管理内の割込みは禁止した状態で実行する必要があるため，
	 *  FAULTMASKを'1'にした状態で実行する．
	 *  reqflgをチェックする前に割込みを禁止するのは，reqflgをチェック
	 *  した直後に割込みハンドラが起動され，その中でディスパッチが要求
	 *  された場合に，すぐにディスパッチされないという問題が生じるため
	 *  である．
	 */
	ldr r0, =reqflg				/* reqflgがfalseならそのまま戻る */
	ldr r2, [r0]
	cmp r2, #1					/* trueならret_int_2へ           */
	beq ret_int_2

ret_int_1:
	/*
	 *  ここには割込みロック状態（PREMASKがセット）された状態で来る．
	 *  Threadモードからのリターンにより自動的に割込みロック解除状態になる．
	 *  割込み優先度マスクは割込み前に状態に戻す．
	 */ 
	cpsie i						/* 割込みのリターンでクリアされないためクリア */
	bx r3						/* リターン */

ret_int_2:
	/*
	 *  ここでは，戻り先がタスクであり，PSP上にスクラッチレジスタとiipmが
	 *  保存された状態になっている．また，プロセッサは，Handlerモード
	 *  割込みロック状態となっている．
	 *  また，r0には，reqflgのアドレス値が保持されている．
	 */
	movs r1, #0					/* reqflgをfalseに */
	str r1, [r0]

	/*
	 *  Threadモードへ移行する．
	 *
	 *  dispatcherやcall_texrnを呼び出す場合は，Threadモードである必
	 *  要があるため，PSPスタック上にダミーの例外フレームを置いて，
	 *  擬似的に割込みハンドラからリターンする．
	 *  リターンと同時にFAULTMASKが自動的にクリアされ，カーネル管理外の
	 *  割込みが許可される．
	 */
	ldr r0, =ret_int_3			/* PC   */
	ldr r1, =EPSR_T				/* xPSR(Tビットが'1'である必要がある) */
	mrs r2, psp
	subs r2, #(0x04*2)
	str r1, [r2, 0x04]
	str r0, [r2, 0x00]
	subs r2, #(EXC_FRAME_SIZE - (4*2))	/* r0-r3,r12,lrの内容は設定する必要がない */
	msr psp,r2
	bx r3						/* Threadモードへ移行 */

ret_int_3:
	/*
	 *  上記の処理により，Threadモードで実行される．
	 *  enadspがfalseである場合と，runtskとschedtskが同じ場合には，
	 *  ディスパッチを行わない．このチェックが必要なのは，タスク例外処
	 *  理ルーチンの呼出しが必要な場合に，ディスパッチが必要なくても，
	 *  reqflgをtrueにするためである．
	 */
	ldr r0, =runtsk				/* ディスパッチを行わない場合でも，r1にruntsk の値(TCB) */
	ldr r1, [r0]				/* が入っている必要があるので，先に読み込む */
	ldr r0, =enadsp
	ldr r2, [r0]
	cmp r2, #0x00				/* enadspがfalseならret_int_r_1へ */
	beq ret_int_r_1
	ldr r0, =schedtsk
	ldr r2, [r0]
	cmp r1, r2					/* runtskとschedtskが同じなら */
	beq ret_int_r_1				/*              ret_int_r_1へ */
	mov r0, r8
	mov r1, r9
	mov r2, r10
	mov r3, r11
	push {r0-r7}
	ldr r2, =runtsk
	ldr r1, [r2]
	mov r0, sp
	str r0, [r1,#TCB_sp]		/* タスクスタックを保存 */
	ldr r2, =ret_int_r			/* 実行再開番地を保存   */
	str r2, [r1,#TCB_pc]
	b dispatcher				/* ディスパッチャへ */

/*
 * 割込みによりプリエンプトされたタスクへのリターン処理
 *
 * Threadモードで，ディスパッチャや割込みの出口処理から呼び出される．
 * 割込みによりプリエンプトされたタスクへリターンするには，いったん
 * Handlerモードに移行し，PCに0xfffffffdを代入してリターンする必要
 * がある．そのため，SVCにより，SVCハンドラを呼び出し，Handlerモー
 * ドへ移行する．
 */
	.global ret_int_r, _kernel_break_wait
	.syntax unified
	.thumb
	.thumb_func
	.type ret_int_r, %function
_kernel_break_wait:
ret_int_r:
	pop {r0-r7}
	mov r8,  r0
	mov r9,  r1
	mov r10, r2
	mov r11, r3

ret_int_r_1:
	/*
	 *  enatexがtrueで，texptnが0でなければ，タスク例外処理ルーチンを
	 *  呼び出す．
	 *  dispatcherから呼び出されるため，TCBのアドレスはr1に入っている
	 */
	ldr r2, =runtsk
	ldr r1, [r2]
	ldrb r0, [r1,#TCB_enatex]
	movs r2, #TCB_enatex_mask
	tst r0, r2
	beq ret_int_r_2				/* enatex が false なら ret_int_r_2へ */
	ldr r0, [r1,#TCB_texptn]	/* texptn が 0 ならリターン     */
	cmp r0, #0x00
	beq ret_int_r_2
	bl call_texrtn				/* タスク例外ルーチンの呼び出し */

ret_int_r_2:
	ldr r0, =NVIC_ICSR
	ldr r1, =NVIC_PENDSVSET
	str r1, [r0]
	cpsie i                     /* PRIMASKをクリアしてPendSVCを受け付ける */
1:                              /* ここには来ない */
	b 1b

/*
 *  PendSVCハンドラ
 */
	.global pendsvc_handler
	.syntax unified
	.thumb
	.thumb_func
	.type pendsvc_handler, %function
pendsvc_handler:
	/*
	 *  割込み処理からのリターンにより，CPUロック解除状態に移行するよ
	 *  う準備する．
	 */
	mrs r0, psp
	adds r0, #EXC_FRAME_SIZE	/* スタックを捨てる   */
	msr psp, r0
	movs r0, #0
	bx lr						/* リターン     */

/*
 *  タスクディスパッチャ
 */
	.section .text.bottom
	.align 4
	.global dispatch, exit_and_dispatch
	.syntax unified
	.thumb
	.thumb_func
	.type dispatch, %function
	.type exit_and_dispatch, %function
dispatch:
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・ディスパッチ
	 *  許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さ
	 *  れる．
	 */
	mov r0, r8				/* レジスタの保存 */
	mov r1, r9
	mov r2, r10
	mov r3, r11
	push {r0-r7,lr}

	ldr r0, =runtsk			/* runtskを読み込む */
	ldr r1, [r0]
	mov r2, sp
	str r2, [r1,#TCB_sp]	/* タスクスタックを保存 */
	ldr r3, =dispatch_r		/* 実行再開番地を保存 */
	str r3, [r1,#TCB_pc]

/* 現在実行中のコンテキストを捨てて、タスクディスパッチャを呼び出す時 */
exit_and_dispatch:
	ldr r0,=STACKTOP		/* MSPを初期化   */
	msr msp, r0				/* 使用しているため初期化する  */
	msr psp, r0				/* pspも初期化 */
	movs r0, #CONTROL_PSP	/* 安全のためPSPを初期化  */
	msr   control, r0
	isb						/* control の操作後に必要 */

/*
 *  ディスパッチャ本体
 */
dispatcher:
	/* ここには CPUロック（割り込み禁止）で来ること */
	ldr r1, =schedtsk		/* schedtskをruntskに */
	ldr r0, [r1]
	ldr r2, =runtsk			/* schedtskをruntskにセット */
	str r0, [r2]			/* (タスクスイッチの実行) */
#ifdef	LOG_ANALYSIS
	push {r0-r3}
	bl iana_tsk				/* タスク実行情報の設定 */
	pop {r0-r3}
#endif	/* 	LOG_ANALYSIS */
	cmp r0, #0x00			/* schedtsk が無ければ割込み待ちへ */
	beq dispatcher_1
	ldr r2, [r0,#TCB_sp]	/* タスクスタックを復帰 */
	mov sp, r2
	ldr r2, [r0,#TCB_pc]	/* 実行再開番地を復帰   */
	mov pc, r2

dispatcher_1:
	/*
	 * CPUロック状態の解除と，非タスクコンテキスト実行状態への
	 * 準備をする
	 */
	ldr r2, =runtsk
	movs r0, #0				/* r0 <- '0' */
	str r0, [r2]			/* runtsk = NULL */
	movs  r0, #CONTROL_MSP  /* MSPを有効に  */
	msr   control, r0       
	isb                     /* control の操作後に必要 */

dispatcher_2:
	wfi
	cpsie i					/* PRIMASK をクリア（割込みを受け付ける） */
	nop
	nop
	cpsid i					/* PRIMASK をセット */

	ldr r3, =reqflg
	ldr r0, [r3]			/* reqflgがfalseならdispatcher_2へ */
	cmp r0, #0
	beq dispatcher_2
	movs r0, #0				/* r0 <- '0' */
	str r0, [r3]			/* reqflgをfalseに */
	/*
	 *  CPUロック状態に戻す．iipmの復帰処理はおこなわない．
	 */
	movs  r0, #CONTROL_PSP  /* PSPを有効に  */ 
	msr   control, r0      
	isb                     /* control の操作後に必要 */
	b dispatcher

/*
 *  ディスパッチャ出口処理
 *  ・ディスパッチャ処理において、r0 = runtskのアドレス
 *    となっていることに注意
 *  ・スタックポインタも設定済み
 */
	.global dispatch_r
	.syntax unified
	.thumb
	.thumb_func
	.type dispatch_r, %function
dispatch_r:
	mov r11, r0					/* runtskをセーブ */
	pop {r0-r7}                 /* レジスタの復帰 */
	mov r8,  r0
	mov r9,  r1
	mov r10, r2
	mov r0, r11
	mov r11, r3
	/*
	 * タスク例外処理ルーチンの起動
	 */
	ldrb r1, [r0,#TCB_enatex]
	movs r2, #TCB_enatex_mask
	tst r1, r2
	beq dispatch_r_1			/* enatex が false ならリターン */
	ldr r1, [r0,#TCB_texptn]	/* texptn が 0 ならリターン     */
	tst r1, r1
	beq dispatch_r_1
	ldr r0, =call_texrtn		/* タスク例外ルーチンの呼び出し */
	bx r0
dispatch_r_1:					/* タスクへのcall_textnから戻る */
	pop  {pc}

/*
 *  タスク起動処理
 *  ・ディスパッチャ処理において、r0 = runtsk となっていることに注意
 *  ・スタックポインタも設定済み
 */
	.global activate_r
	.syntax unified
	.thumb
	.thumb_func
	.type activate_r, %function
activate_r:
	ldr r2, =ext_tsk			/* ext_tskを戻り番地に設定 */
	mov lr, r2
	cpsie i						/* 割込み許可 */
	ldr r2, [r0, #TCB_tinib]	/* runtsk->tinibをr2に  */
	ldr r0, [r2, #TINIB_exinf]	/* exinfを引数レジスタr0に  */
	ldr r1, [r2, #TINIB_task]	/* タスク起動番地にジャンプ */
	bx r1


/*
 *  微少時間待ち
 */
	.balign 4
	.text
	.global sil_dly_nse
	.syntax unified
	.thumb
	.thumb_func
	.type sil_dly_nse, %function
sil_dly_nse:
	ldr   r1, =SIL_DLY_TIM1
	subs  r0, r0, r1
	cmp   r0, #0
	bgt   sil_dly_nse1
	bx    lr
sil_dly_nse1:
	ldr   r1, =SIL_DLY_TIM2
sil_dly_nse2:
	subs  r0, r0, r1
	cmp   r0, #0
	bgt   sil_dly_nse2
	bx    lr

