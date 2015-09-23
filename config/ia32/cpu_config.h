/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
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
 *  @(#) $Id: cpu_config.h,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

/*
 *	プロセッサ依存モジュール（i386用）
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <cpu_rename.h>

/*
 *  プロセッサの特殊命令のインライン関数定義
 */
#ifndef _MACRO_ONLY
#include <cpu_insn.h>
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
	unsigned _int32_ eflag;

	Asm("pushf");
	Asm("popl %0" : "=g"(eflag));

	return ((eflag & 0x200) == 0);
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock

/*
 *  CPUロックとその解除
 *
 */

Inline void x_lock_cpu(void) {
	disint();
	Asm("":::"memory");
}

#define i_lock_cpu() x_lock_cpu()
#define t_lock_cpu() x_lock_cpu()

Inline void x_unlock_cpu(void) {
	Asm("":::"memory");
	enaint();
}

#define i_unlock_cpu() x_unlock_cpu()
#define t_unlock_cpu() x_unlock_cpu()

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
 *  CPU例外ハンドラの設定
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
 *  CPU例外ハンドラの設定
 *  ベクトル番号 excno のCPU例外ハンドラの起動番地を exchdr に設定
 */
extern void define_exc(EXCNO excno, FP exchdr);

/*
 *  CPU例外ハンドラの出入口処理
 */

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 */

#define EXCHDR_ENTRY(exchdr) extern void exchdr(VP p_excinf)

#define EXC_ENTRY(exchdr) exchdr

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
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

/*
 * i386セグメントデスクリプタの設定
 */
extern void set_segment_descriptor(int sel, unsigned addr, unsigned limit, unsigned type, unsigned dpl);
extern void set_gate_descriptor(int sel, unsigned selector, void *func, unsigned type, unsigned dpl);

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
