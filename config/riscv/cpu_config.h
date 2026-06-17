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
 */

/*
 *		プロセッサ依存モジュール（RISC-V用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

/*
 *  カーネル内部識別名のリネーム
 */
#include "cpu_rename.h"

/*
 *  プロセッサの特殊命令のインライン関数定義
 */
#include <cpu_insn.h>

/*
 *  割込み設定定義
 */
#include "user_config.h"

/*
 * システム個別設定
 */
#include "sys_defs.h"

/*
 *  MCAUSE REGISTER定義
 */
#ifndef MCAUSE_CAUSE
#define MCAUSE_CAUSE            0x7FFFFFFF
#endif

/*
 *  MACHINEの最大割込み数を定義
 */
#define NUM_MACHNE_INTNO        32
#define NUM_MACHNE_EXCNO        16

/*
 *  設定可能な最高優先度
 */
#ifndef MAX_IPM
#define MAX_IPM  0xF	/* RISC-Vデフォルト  */
#endif

/*
 *  例外フレームのオフセット
 */
#define P_EXCINF_OFFSET_MSTATUS 0x00
#define P_EXCINF_OFFSET_PC      0x01
#define P_EXCINF_OFFSET_MCAUSE	0x02
#define P_EXCONF_OFFSET_MIE     0x03
#define P_EXCONF_OFFSET_SP      0x04


/*
 *  TCB 関連の定義
 *
 *  cpu_context.h に入れる方がエレガントだが，参照の依存性の関係で，
 *  cpu_context.h には入れられない．
 */

/*
 *  TCB 中のフィールドのビット幅の定義
 */
#define    TBIT_TCB_TSTAT       8    /* tstat フィールドのビット幅 */
#define    TBIT_TCB_PRIORITY    8    /* priority フィールドのビット幅 */

#ifndef _MACRO_ONLY
/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
    VP    sp;        /* スタックポインタ */
    FP    pc;        /* プログラムカウンタ */
} CTXB;


/*
 *  RISC-V依存プロセッサコントロールブロック
 */
typedef struct system_processor_control_block {
	/*
	 *  CPUロックフラグ実現のための変数
	 *
	 *  これらの変数は，CPUロック状態の時のみ書き換えてもよいとする．
	 *  インライン関数中で，アクセスの順序が変化しないよう，volatile を指定．
	 */
	volatile BOOL	lock_flag;  /* CPUロックフラグの値を保持する変数 */
	volatile UH		inest_lvl;  /* 割込みネストを保存する変数 */
	volatile unsigned long	kernel_mie;	/* デフォルトのMIE値を保存する変数 */
	volatile unsigned long	saved_trap;	/* MACHINE TRAPデータの保存変数 */

	/*
	 *  アイドル処理用のスタックの初期値
	 */
	VP* stacktop;
} STPCB;

/*
 *  STPCBの保存テーブル
 */
extern STPCB *p_tspcb_table[];

/*
 *  マルチプロセッサ処理モデルの実現
 *
 *  コア番号は標準実装．
 *  その他はシングルコアの場合、実装は不要．
 */

/*
 *  コア番号を取り出す(RISC-V標準)
 */
Inline UINT
x_prc_index(void)
{
	return read_csr(mhartid);
}

#define t_prc_index()	x_prc_index()
#define i_prc_index()	x_prc_index()

/*
 *  STPCBを取り出す
 */
Inline STPCB *
get_sys_stpcb(void)
{
	return p_tspcb_table[x_prc_index()];
}


/*
 *  TOPPERS標準割込み処理モデルの実現
 *
 *  割込み優先度マスクとしては，mie（MACHINE割込みイネーブルレジスタ）を
 *  用いる．全割込みを禁止する機能として，mstatusのMIEやPLICの割込みスレ
 *  ッドが，カーネル管理外の割込みをサポートするため，これらはCPUロック
 *  のために用いない．
 *  PLICの割込みスレッドでは、MITの割込みロックができないことも、
 *  この選択とした理由である．
 *
 *  CPUロック状態を管理すための変数(lock_flag)を用意する．
 *  システムスタックのネスト管理の変数(inest_lvl)を用意する．
 *  外部割込みの制御はPLICを用いるが、この制御はPRCでは管理せず
 *  ターゲットレベルで管理を行う．
 */

/*
 *  コンテキストの参照
 *
 *  RISCVでは，割込みネストレベルでコンテキストを判定する．
 */
Inline BOOL
sense_context(void)
{
	return get_sys_stpcb()->inest_lvl != 0;
}

/*
 *  CPUロック状態の参照
 */
Inline BOOL
sense_lock(void)
{
	return get_sys_stpcb()->lock_flag;
}

#define t_sense_lock()	sense_lock()
#define i_sense_lock()	sense_lock()

/*
 *  MACHINE割込みの禁止
 *
 *  割込み制御で使用するため作成．
 */
Inline void
dis_intm(void)
{
	clear_csr(mstatus, MSTATUS_MIE);
}

/*
 *  MACHINE割込みの許可
 *
 *  割込み制御で使用するため作成．
 */
Inline void
ena_intm(void)
{
	set_csr(mstatus, MSTATUS_MIE);
}

/*
 *  CPUロック状態への移行
 *
 *  mie（MACHINE割込みイネーブルレジスタ）を，カーネル管理割込み設
 *  定をマスクすることでCPUロック状態を設定する．
 *  また，lock_flagをtrueにする．
 *
 *  この関数は，CPUロック状態（lock_flagがtrueの状態）で呼ばれることは
 *  ないものと想定している．
 */
Inline void
x_lock_cpu(void)
{
	disable_int_status(KERNEL_MIE);
	get_sys_stpcb()->lock_flag = TRUE;
}

#define t_lock_cpu()	x_lock_cpu()
#define i_lock_cpu()	x_lock_cpu()

/*
 *  CPUロック状態の解除
 *
 *  lock_flagをfalseにし，mie（MACHINE割込みイネーブルレジスタ）中の，
 *  イネーブル設定を行う．
 *
 *  この関数は，CPUロック状態（lock_flagがtrueの状態）でのみ呼ばれるも
 *  のと想定している．
 */
Inline void
x_unlock_cpu(void)
{
	get_sys_stpcb()->lock_flag = FALSE;
	enable_int_status(get_sys_stpcb()->kernel_mie);
}

#define t_unlock_cpu()	x_unlock_cpu()
#define i_unlock_cpu()	x_unlock_cpu()


/*
 *  例外ベクタテーブルの構造の定義
 */
typedef struct exc_vector_entry {
	FP		exc_handler;		/* 例外ハンドラの起動番地 */
} EXCVE;

/*
 *  最高優先順位タスクへのディスパッチ（cpu_support.S）
 *
 *  dispatchは，タスクコンテキストから呼び出されたサービスコール処理か
 *  ら呼び出すべきもので，タスクコンテキスト・CPUロック状態・ディスパッ
 *  チ許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さな
 *  ければならない．
 */
extern void	dispatch(void);

/*
 *  自タスクのマイグレーション（cpu_support.S）
 */
extern void dispatch_and_migrate(ID prcid);

/*
 *  現在のコンテキストを捨ててディスパッチ（cpu_support.S）
 *
 *  exit_and_dispatchは，ext_tskから呼び出すべきもので，タスクコンテキ
 *  スト・CPUロック状態・ディスパッチ許可状態・（モデル上の）割込み優先
 *  度マスク全解除状態で呼び出さなければならない．
 */
extern void	exit_and_dispatch(void);

/*
 *  MACHINE割込みトラップ関数 （cpu_support.S）
 *
 *  trap_entryは最初のMACHINE割込みを処理する関数．
 */
extern void trap_entry(void);

/*
 *  MACHINE割込みネストトラップ関数 （cpu_support.S）
 *
 *  trap_entryはネスト時のMACHINE割込みを処理する関数．
 */
extern void trap_nest(void);

/*
 *  MACHINEタイマの割込みハンドリングプログラム
 */
extern void machine_timer_handler(void);

/*
 *  MACHINE割込みハンドラ領域のテーブル
 */
extern volatile EXCVE m_interrupt_handlers[TNUM_PRCID][NUM_MACHNE_INTNO];

/*
 *  スピンロックエラーが発生した場合のハンドラ
 */
extern void spin_lock_error_handler(unsigned long mcause, void *p_excinf);

/*
 *  MACHINEタイマの割込みハンドラ
 */
extern FP (*default_timer_handler)(void);

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号excnoのCPU例外ハンドラの出入口処理の番地をexc_entryに設
 *  定する．
 */
Inline void
define_exc(EXCNO excno, FP exc_entry)
{
	m_interrupt_handlers[x_prc_index()][excno].exc_handler = exc_entry;
}

/*
 *  MACHINE割込みハンドラの設定
 */
Inline void
machine_inh(INHNO inhno, FP int_entry)
{
	m_interrupt_handlers[x_prc_index()][inhno+16].exc_handler = int_entry;
}

/*
 *  MACINEタイマハンドラの設定
 */
Inline void
timer_inh(FP int_entry)
{
	default_timer_handler = (FP(*)(void))int_entry;
}

/*
 *  CPU例外ハンドラの出入口処理
 */

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 *
 */
#if defined(__riscv64)
#define __EXCHDR_ENTRY(exchdr, stacktop) \
extern void exchdr##_entry(void *sp);   \
asm(".text							\n" \
#exchdr "_entry:					\n" \
"	addi sp, sp, -32				\n"	 /* 保存領域を確保 */ \
"	sd ra, 24(sp)					\n"	 /* return addressをセーブ */ \
"	mv a0, a1						\n"  /* stack pointerをa0に */ \
"	jal " #exchdr "					\n"	 /* exchdrに分岐 */ \
"	ld ra, 24(sp)					\n"	 /* return addressをリストア */ \
"	addi sp, sp, 32					\n"	 /* 保存領域を開放 */ \
"	ret								\n");/* リターン */
#else
#define __EXCHDR_ENTRY(exchdr, stacktop) \
extern void exchdr##_entry(void *sp);   \
asm(".text							\n" \
#exchdr "_entry:					\n" \
"	addi sp, sp, -16				\n"	 /* 保存領域を確保 */ \
"	sw ra, 12(sp)					\n"	 /* return addressをセーブ */ \
"	mv a0, a1						\n"  /* stack pointerをa0に */ \
"	jal " #exchdr "					\n"	 /* exchdrに分岐 */ \
"	lw ra, 12(sp)					\n"	 /* return addressをリストア */ \
"	addi sp, sp, 16					\n"	 /* 保存領域を開放 */ \
"	ret								\n");/* リターン */
#endif

#define _EXCHDR_ENTRY(exchdr, stacktop)    __EXCHDR_ENTRY(exchdr, stacktop)
#define EXCHDR_ENTRY(exchdr)    _EXCHDR_ENTRY(exchdr, STACKTOP)

#define EXC_ENTRY(exchdr) exchdr##_entry

/*
 *  CPU例外の発生した時のコンテキストの参照
 *
 *  CPU例外の発生した時のコンテキストが，タスクコンテキストの時にfalse，
 *  そうでない時にtrueを返す．
 */
Inline BOOL
exc_sense_context(void *p_excinf)
{
	return get_sys_stpcb()->inest_lvl > 1;
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
Inline BOOL
exc_sense_lock(void *p_excinf)
{
	return(exc_sense_context(p_excinf)
					|| !sense_int_status(KERNEL_MIE));
}


/*
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

/*
 *  プロセッサ・スタート
 */
extern void start();

/*
 *  C言語レベル MACHINE割込みハンドラ
 */
extern UH handle_trap(unsigned long mcause, void *p_excinf);

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
