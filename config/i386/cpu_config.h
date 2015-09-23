/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
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
 *  @(#) $Id: cpu_config.h,v 1.2 2002/04/06 12:23:01 honda Exp $
 */

/*
 *	プロセッサ依存モジュール（i386用）
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#ifndef _MACRO_ONLY
#include "i386.h"
#endif /* _MACRO_ONLY */

/*
 *  カーネルの内部識別名のリネーム
 */
#ifndef OMIT_RENAME

#define activate_r      _kernel_activate_r
#define ret_int         _kernel_ret_int
#define ret_exc         _kernel_ret_exc
#define task_intmask    _kernel_task_intmask
#define int_intmask     _kernel_int_intmask

#ifdef LABEL_ASM

#define _activate_r     __kernel_activate_r
#define _ret_int        __kernel_ret_int
#define _ret_exc        __kernel_ret_exc
#define _task_intmask   __kernel_task_intmask
#define _int_intmask    __kernel_int_intmask

#endif /* LABEL_ASM */
#endif /* OMIT_RENAME */

/*
 *  プロセッサの特殊命令のインライン関数定義
 */
#ifndef _MACRO_ONLY
#include "cpu_insn.h"
#endif /* _MACRO_ONLY */

/*
 *  TCB 関連の定義
 *
 *  cpu_context.h に入れる方がエレガントだが，参照の依存性の関係で，
 *  cpu_context.h には入れられない．
 */

/*
 *  TCB 中のフィールドのビット幅の定義
 */
#define	TBIT_TCB_PRIORITY	8	/* priority フィールドのビット幅 */
#define	TBIT_TCB_TSTAT		8	/* tstat フィールドのビット幅 */
#define	TBIT_TCB_TWAIT		8	/* twait フィールドのビット幅 */

#ifndef _MACRO_ONLY
/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	VP	esp;		/* スタックポインタ */
	FP	eip;		/* プログラムカウンタ */
} CTXB;

/*
 *  割り込みのネスト回数
 *
 *  1以上の時は非タスクコンテキストになる。割り込みハンドラや例外ハンドラの
 *  出入口でインクリメント・デクリメントされる。
 */
extern UW nest;

/*
 *  システム状態参照
 */

Inline BOOL
sense_context()
{
	return (nest != 0);
}

Inline BOOL
sense_lock()
{
	unsigned __int32 eflag;

	Asm("pushf");
	Asm("popl %0" : "=g"(eflag));

	return ((eflag & 0x200) == 0);
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock

/*
 *  CPUロックとその解除
 * （タスクコンテキスト/非タスクコンテキスト共通）
 *
 */

Inline void
i386_lock_cpu()
{
	Asm("cli");
}

Inline void
i386_unlock_cpu()
{
	Asm("sti");
}

/*
 *  CPUロックとその解除（タスクコンテキスト用）
 *
 */

Inline void
i_lock_cpu()
{
	i386_lock_cpu();
}

Inline void
t_lock_cpu()
{
	i386_lock_cpu();
}

/*
 *  CPUロックとその解除（非タスクコンテキスト用）
 */
Inline void
i_unlock_cpu()
{
	i386_unlock_cpu();
}

Inline void
t_unlock_cpu()
{
	i386_unlock_cpu();
}

/*
 *  タスクディスパッチャ
 */

/*
 *  最高優先順位タスクへのディスパッチ（cpu_support.S）
 *
 *  dispatch は，タスクコンテキストから呼び出されたサービスコール処理
 *  内で，CPUロック状態で呼び出さなければならない．
 */
extern void	dispatch(void);

/*
 *  現在のコンテキストを捨ててディスパッチ（cpu_support.S）
 *
 *  exit_and_dispatch は，CPUロック状態で呼び出さなければならない．
 */
extern void	exit_and_dispatch(void);

/*
 *  割込みハンドラ／CPU例外ハンドラの設定
 */

/*
 *  例外ハンドラの出入口処理（cpu_support.S）
 */
extern void exception0(void);
extern void exception1(void);
extern void exception2(void);
extern void exception3(void);
extern void exception4(void);
extern void exception5(void);
extern void exception6(void);
extern void exception7(void);
extern void exception8(void);
extern void exception9(void);
extern void exception10(void);
extern void exception11(void);
extern void exception12(void);
extern void exception13(void);
extern void exception14(void);
extern void exception16(void);
extern void exception17(void);
extern void exception18(void);
extern void exception19(void);

/*
 *  CPU例外ハンドラのテーブル（cpu_config.c）
 *
 *  CPU例外ハンドラは一旦このテーブルに登録され、別に用意された出入口処理
 *  を呼び出す。
 */
extern FP exc_table[0x20];

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
extern void define_inh(INHNO inhno, FP inthdr);

/*
 *  CPU例外ハンドラの設定
 *  ベクトル番号 excno のCPU例外ハンドラの起動番地を exchdr に設定
 */
extern void define_exc(EXCNO excno, FP exchdr);

/*
 *  割込みハンドラ／CPU例外ハンドラの出入口処理
 */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 */

#define	INTHDR_ENTRY(inthdr)		\
extern void inthdr##_entry(void);	\
asm(".text				\n"\
#inthdr "_entry:			\n"\
"	pusha				\n"\
"	movl	$"#inthdr", %eax	\n"\
"	jmp	interrupt		\n")

#define INT_ENTRY(inthdr)		inthdr##_entry

/*
 *  CPU例外ハンドラの出入口処理？の生成マクロ
 */

#define	EXCHDR_ENTRY(exchdr)		\
extern void exchdr##_entry(VP p_excinf);	\
asm(".text				\n"\
#exchdr "_entry:			\n"\
"	jmp	" #exchdr "		\n")

#define EXC_ENTRY(exchdr)	exchdr##_entry

/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキストの参照
 */
Inline BOOL
exc_sense_context(VP p_excinf)
{
	return (nest > 1);
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
Inline BOOL
exc_sense_lock(VP p_excinf)
{
	return ((((UW *)p_excinf)[3] & 0x200) == 0);
}

/*
 *  ラベルの別名を定義するためのマクロ
 */
#define	LABEL_ALIAS(new_label, defined_label) \
	asm(".globl " #new_label "\n" #new_label " = " #defined_label);

/*
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
