/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: cpu_support.S,v 1.9 2003/12/15 05:50:05 takayuki Exp $
 */

/*
 *	プロセッサ依存モジュール アセンブリ言語部（M32R用）
 */

#define	_MACRO_ONLY

#include <cpu_rename.h>

#include <m32rasm.inc>
#include <s_services.h>

#include "offset.h"


/**************************************************************************/

/***************************/
/* EITベクタエントリの定義 */
/***************************/

/* .vectorセクションはROM空間の先頭に配置される */

	.section ".vector","ax"
	.extern   _start

	.balign 0x10
Label _reset				//リセット例外
	bra		_start

	.balign 0x10
Label vector_entry_SBI
	stmdb	"r0,r1,r2,r14"
	ldi		r1, #(EXC_SBI*4-4)
	bra		_exception_handler

	.balign 0x10
Label vector_entry_RIE
	stmdb	"r0,r1,r2,r14"
	ldi		r1, #(EXC_RIE*4-4)
	bra		_exception_handler

	.balign 0x10
Label vector_entry_AE
	stmdb	"r0,r1,r2,r14"
	ldi		r1, #(EXC_AE*4-4)
	bra		_exception_handler

/*
 * トラップベクタエントリ
 *   要因番号を取りたいので、一旦リンクレジスタを退避してサブルーチンコール
 */

	.balign 0x10
Label vector_entry_trap
	bra		_trap_handler_0
	bra		_trap_handler_1
	bra		_trap_handler_2
	bra		_trap_handler_3
	bra		_trap_handler_4
	bra		_trap_handler_5
	bra		_trap_handler_6
	bra		_trap_handler_7
	bra		_trap_handler_8
	bra		_trap_handler_9
	bra		_trap_handler_10
	bra		_trap_handler_11
	bra		_trap_handler_12
	bra		_trap_handler_13
	bra		_trap_handler_14
	bra		_trap_handler_15


/**************************************************************************
Q: なぜ一度割込みスタックに積んだ情報をタスクスタックに積みなおすのか
A: 最悪割込みターンアラウンド時間を低く抑えるため

現在のM32R依存部の割込み出入り口処理の最悪ケース(クリティカルパス)は、タ
スクコンテキストで割込みがかかり、かつハンドラ内でより上位の優先度を持つ
タスクが起動された場合である。この場合、現在実行しているタスクの全コンテ
キスト内容が退避され、新しいタスクのコンテキスト内容を復帰する。このとき、
カーネル動作の一貫性を保障するために、システムスタックは割り込みがかかる
前の状態まで戻さなければならない。ここで、コンテキストの内容をシステムス
タックに積んでいた場合、最悪ケースの際には半数近いレジスタの積み直し(シ
ステムスタックからタスクスタックへの積み直し)が発生し、RAMアクセスの速度
に比例したオーバヘッドが生ずる。本OSはリアルタイムOSであるため、平均性能
の向上よりも、最悪実行時間の短縮に主眼を置き、このような実装手法を選択し
た。

[追記 20020810] なんか内蔵RAMからSDRAMのDMA転送が速いらしいので、これで
最悪ケースに対処するのも手かなと思う。


***************************************************************************/

/*
 * 割込みベクタエントリ と 割込みハンドラ起動/復帰処理部
 */

/*
 * レジスタの積み方
 *   R0,R1,R14,ICUISTS,R2-R7,ACC(HI/LO),BPC,[ICUIMASK|PSW],R8-R13
 */

	.balign 0x10
	.extern InterruptHandlerEntry
	.extern _unhandled_interrupt_handler
Label interrupt_handler

		/* 作業領域の作成 */
	stmdb	"r0,r1"

		/* タスクコンテキストからの割込みかな？ */
	mvfc	r0, psw
	and3	r1, r0, 0x8000
	beqz	r1, L1

		/* タスクからならタスクスタックへ切替 & R1に割込みスタック位置をコピー */
	or3		r0, r0, 0x80
	mv		r1, r15
	mvtc	r0, psw

		/* 割込みスタックに退避したものをタスクスタックにコピー */
	ld		r0, @(4,r1)
	st		r0, @-r15
	ld		r0, @r1
	st		r0, @-r15
	addi	r1, 8
	mvtc	r1, spi
	mvfc	r0, psw

L1:
	stmdb	"r14"

	ldidx	r14

		/* 割込み要因番号の取出し */
	ld24	r1, ICUISTS
	ld		r0, @r1
	st		r0, @-r15

		/* ハンドラ起動番地取出し */
	lds		r1, r14, InterruptHandlerEntry
	srli	r0, 20
	add		r1, r0
	addi	r1, -4
	ld		r1, @r1
	srli	r0, 2

		/* ハンドラの登録確認 */
	bnez	r1, L2

		/* ハンドラ未登録の割込み */
	lds		r1, r14, _unhandled_interrupt_handler
	beqz	r1, exit_interrupt_handler

L2:
		/* スクラッチ+BPCを退避してハンドラ起動 */
	stmdb	"r2,r3,r4,r5,r6,r7"
	mvfachi	r2
	st		r2, @-r15
	mvfaclo	r2
	st		r2, @-r15
	mvfc	r2, bpc
	st		r2, @-r15

		/* 割込みスタックモード, 割込み許可, PSW保存 */
	mvfc	r2, psw
	or3		r3, r2, 0x40
	and3	r3, r3, 0xff7f
	mvtc	r3, psw
	st		r2, @-r15

		/* ハンドラ起動 */
	lds		r14, r14, interrupt_handler_r
	jmp		r1


	/*
	 * 割込みハンドラ 復帰ポイント
	 */
	.text
	.align 4
interrupt_handler_r:

		/* 割込み禁止, スタックモード復帰 */
	ld		r0, @r15+
	mvtc	r0, psw

		/* 戻り先のチェック (BSMがユーザスタック -> タスクへ)*/
	and3	r0, r0, 0x8000
	beqz	r0, recover_int

		/* タスクコンテキストからの割込み - 復帰ルーチン */

	ldidx	r14

		/* reqflgのチェック */
	lds		r0, r14, _kernel_reqflg
	ld		r1, @r0
	beqz	r1, recover_task			/* ハンドラ内でのディスパッチ発生無し */

		/* reqflg = 0 */
	ldi		r1, #0
	st		r1, @r0

		/* タスクディスパッチの発生 */
	stmdb	"r8,r9,r10,r11,r12,r13"

	lds		r0, r14, _kernel_runtsk
	ld		r0, @r0

	lds		r1, r14, recover_task_r
	st		r1, @(TCB_pc, r0)
	ld24	r1, ICUIMASK
	ld		r2, @r1
	mvfc	r1, psw
	or		r1, r2
	st		r1, @(TCB_psw, r0)
	st		r15, @(TCB_sp, r0)
	bra		_kernel_exit_and_dispatch

recover_task_r:
	ldmia	"r13,r12,r11,r10,r9,r8"

		/* タスク例外起動 */
recover_task:
	bl		_kernel_calltex

		/* 共通復帰処理 */
recover_int:
	ld		r0, @r15+
	mvtc	r0, bpc
	ld		r0, @r15+
	mvtaclo	r0
	ld		r0, @r15+
	mvtachi	r0
	ldmia	"r7,r6,r5,r4,r3,r2"

exit_interrupt_handler:
		/* 割込みマスク復帰 */
	ld		r0, @r15+
	srli	r0, 16
	ld24	r1, ICUIMASK+1
	stb		r0, @r1

	ldmia	"r14,r1,r0"
	rte

/**************************************************************************/

	.section ".vector","ax"

/*
 * トラップのエントリ部 : トラップ番号の格納
 */
	.irp num,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
		.align	4
	_kernel__trap_handler_\num:
		stmdb	"r0,r1,r2,r14"
		ldi		r1, #((\num + EXC_TRAP00 -1) * 4)
		bra		_exception_handler
	.endr

/*
 * 例外/トラップハンドラ
 */

/*
 * レジスタの積み方
 *   R0-R7,R14,ACC(HI/LO),BPC,[ICUIMASK|PSW],R8-R13
 */

	.extern ExceptionHandlerEntry
_exception_handler:

	ldidx	r14

		/* ハンドラ読み出し */
	lds		r0, r14, ExceptionHandlerEntry
	add		r0, r1
	ld		r0, @r0
	bnez	r0, L3

		/* ハンドラ未登録の割込み */
	lds		r1, r14, _unhandled_interrupt_handler
	beqz	r1, exit_exception_handler		//ハンドラ未登録なら即終了

		/* ハンドラ起動 & スタック切替 */
L3:	stmdb	"r3,r4,r5,r6,r7"
	mvfachi	r1
	st		r1, @-r15
	mvfaclo	r1
	st		r1, @-r15
	mvfc	r1, bpc
	st		r1, @-r15
	mvfc	r2, psw
	and3	r2, r2, 0xff7f
	mvtc	r2, psw
	lds		r14, r14, exception_handler_r
	jmp		r0

	.text
	.align 4
exception_handler_r:

	ldidx	r14

		/* 戻り先はどこ？ (BSM=1->タスク)*/
	mvfc	r0, psw
	and3	r1, r0, 0x8000
	beqz	r1, L5

		/* タスクスタックへ切り替える */
	or3		r1, r0, 0x80
	mvtc	r1, psw

		/* CPU例外中にディスパッチがあるか? */
	lds		r0, r14, _kernel_reqflg
	ld		r1, @r0
	beqz	r1, L4

		/* ディスパッチの発生 */
	stmdb	"r8,r9,r10,r11,r12,r13"
	ld24	r0, ICUIMASK
	ld		r0, @r0
	mvfc	r1, psw
	srli	r1, 8			//IE,SM,C <= BIE,BSM,BC
	or		r0, r1
	lds		r2, r14, _kernel_runtsk
	ld		r2, @r2
	st		r0, @(TCB_psw, r2)
	lds		r3, r14, recover_exception_exc_task_r
	st		r0, @(TCB_pc, r2)
	st		r15, @(TCB_sp, r2)

		/* ディスパッチルーチンへ飛ばす */
	lds		r0, r14, _kernel_exit_and_dispatch
	mvtc	r0, bpc
	xor		r0, r0
	mvtc	r0, psw
	rte

	.align 4
recover_exception_exc_task_r:
	ldmia	"r13,r12,r11,r10,r9,r8"

		/* タスク例外の起動 */
L4:	bl		_kernel_calltex

		/* スクラッチ復帰 */
L5:	ld		r0, @r15+
	mvtc	r0, bpc
	ld		r0, @r15+
	mvtachi	r0
	ld		r0, @r15+
	mvtaclo	r0
	ldmia	"r7,r6,r5,r4,r3"

exit_exception_handler:
	ldmia	"r14,r2,r1,r0"
	rte



/**************************************************************************/

/*
 *  タスクディスパッチャ
 *		extern void dispatch(void);
 *		extern void exit_and_dispatch(void);
 */

Function _kernel_dispatch
		/*
		 * レジスタ退避
		 */
	stmdb	"r8,r9,r10,r11,r12,r13,r14"

	ldidx	r14

		/*
		 * TCBにスタックと再起動番地を保存
		 */
	lds		r0, r14, _kernel_runtsk
	lds		r1, r14, dispatch_r
	ld		r0, @r0
	st		r1, @(TCB_pc, r0)	// runtsk->tskctxb->pc = dispatch_r;
	st		r15,@(TCB_sp, r0)	// runtsk->tskctxb->sp = r15;

		/*
		 * PSW[12:15]に割込みマスクビットを突っ込んでTCBに保存
		 */
	ld24	r2, ICUIMASK
	mvfc	r1, psw
	ld		r3, @r2
	or		r1, r3
	st		r1, @(TCB_psw, r0)	//runtsk->tskctxb->psw = PSW|*(ICUIMASK);

	/*
 	 * 注意 : この場所は.alignを使わずに32ビット境界であることを
	 *        保障すること。そうでないとexit_and_dispatchへの分岐
	 *        で直前の16ビット命令が実行されてしまう。
	 */

Label _kernel_exit_and_dispatch

	ldidx	r14

		/* スタックモード切替 */
	xor		r0, r0
	mvtc	r0, psw

		/* runtsk = NULL */
	lds		r1, r14, _kernel_runtsk
	ldi		r0, #0
	st		r0, @r1
	

		/* 次に起動すべきタスクを探す */
	lds		r1, r14, _kernel_schedtsk
L6:	ld		r0, @r1
	bnez	r0, L7

		/* 起動すべきタスクがないので、割込み許可して待機 */
	ld24	r2, (ICUIMASK+1)
	ldi		r3, #7
	stb		r3, @r2		//IMASK = 全許可
	ldi		r0, 0x40
	mvtc	r0, psw		//ei

/*
	ldi		r0, #1
	ld24	r2, CLKMOD+3
	stb		r0, @r2
	ldb		r0, @r2
	.rept	8
		nop
	.endr
*/

	xor		r0, r0
	mvtc	r0, psw		//di
	bra		L6

L7:	/* R0 = schedtsk, R1 = &schedtsk */

		/* runtsk = schedtsk; */
	lds		r2, r14, _kernel_runtsk
	st		r0, @r2

		/* PSWと割込みマスクを復帰 (スタックモードも切替) */
	ld		r1, @(TCB_psw, r0)
	and3	r2, r1, 0xffff
	mvtc	r2, psw
	srli	r1, 16
	ld24	r2, ICUIMASK+1
	stb		r1, @r2

		/* スタックポインタとプログラムカウンタを復帰 */
	ld		r15, @(TCB_sp, r0)
	ld		 r0, @(TCB_pc, r0)
	jmp		 r0


	/*
 	 * 注意 : この場所は.alignを使わずに32ビット境界であることを
	 *        保障すること。
	 */

	/* ディスパッチ復帰部 */
dispatch_r:
		/* タスク例外の呼出 */
	bl	_kernel_calltex

	ldmia	"r14,r13,r12,r11,r10,r9,r8"

	rts

/**************************************************************************/

	/*
	 * タスクの起動開始(タスクスタートアップルーチン)
	 *		extern void activate_r(void);
	 */

	.extern _kernel_runtsk
	.extern ext_tsk
Function _kernel_activate_r

	ldidx	r14

		/* タスク起動準備 */
	lds		r2, r14, _kernel_runtsk
	ld		r2, @r2
	ld		r2, @(TCB_tinib, r2)	// r2 = runtsk->tinib
	ld		r0, @(TINIB_exinf, r2)
	ld		r1, @(TINIB_task,  r2)

		/* タスクから戻ろうとしたらext_tskへ */
	lds		r14, r14, ext_tsk

	jmp		r1

/**************************************************************************/

	/*
	 * 各種 雑多なモジュール
	 *		extern ER get_ipm(IPM * ipm);
	 *		extern ER chg_ipm(IPM   ipm);
	 */

Function get_ipm
	ld24	r1, ICUIMASK+1
	ldub	r2, @r1
	st		r2, @r0
	xor		r0, r0
	rts

Function chg_ipm
	ld24	r1, ICUIMASK+1
	and3	r0, r0, 0xf
	st		r0, @r1
	xor		r0, r0
	rts


 /*
  * 32ビットシンボル値参照用テーブル
  */

begin_table
	entry	InterruptHandlerEntry
	entry	_unhandled_interrupt_handler
	entry	interrupt_handler_r
	entry	_kernel_reqflg
	entry	_kernel_runtsk
	entry	_kernel_schedtsk
	entry	recover_task_r
	entry	ExceptionHandlerEntry
	entry	exception_handler_r
	entry	recover_exception_exc_task_r
	entry	_kernel_exit_and_dispatch
	entry	dispatch_r
	entry	ext_tsk
end_table
