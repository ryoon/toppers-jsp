/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: cpu_config.h,v 1.2 2002/04/13 13:14:45 imai Exp $
 */


/*
 *  プロセッサ依存モジュール(H8S用)
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#ifndef OMIT_RENAME

#define activate_r		_kernel_activate_r
#define ret_int			_kernel_ret_int
#define ret_exc			_kernel_ret_exc
#define task_intmask		_kernel_task_intmask
#define int_intmask		_kernel_int_intmask

#ifdef LABEL_ASM

#define _activate_r		__kernel_activate_r
#define _ret_int		__kernel_ret_int
#define _ret_exc		__kernel_ret_exc
#define _task_intmask		__kernel_task_intmask
#define _int_intmask		__kernel_int_intmask

#endif /* LABEL_ASM */
#endif /* OMIT_RENAME */




/*  設定可能な最高優先度  */
#define  MAX_IPM	0x7
#define  str_MAX_IPM	_TO_STRING(MAX_IPM)
#define  MAX_IPM_CCR	0x80


/*
 *  プロセッサの特殊命令のインライン関数定義
 */

#ifndef _MACRO_ONLY
#include "cpu_insn.h"
#endif /* _MACRO_ONLY */

#ifndef _MACRO_ONLY

/*  インライン関数展開の指定  */
#pragma inline(current_intmask, set_intmask, sense_context) 
#pragma inline(sense_lock, t_lock_cpu, t_unlock_cpu)
#pragma inline(i_lock_cpu, i_unlock_cpu, define_inh, define_exc)
#pragma inline(exc_sense_context, exc_sense_lock)


/*
 *  chg_ipm/get_ipm をサポートするかどうかの定義
 */
#define		SUPPORT_CHG_IPM

/*
 *  タスクコンテキストブロックの定義
 */

typedef struct task_context_block {
	VP	sp;		/* スタックポインタ */
	FP	pc;		/* プログラムカウンタ */
} CTXB;


/*
 *  割込みマスク操作ライブラリ
 */

/*
 *  現在の割込みマスクの読出し
 */

Inline UB
current_intmask(void)
{
	return((UB)(current_exr() & 0x07));
}

/*
 *  割込みマスクの設定
 */

Inline void
set_intmask(IPM intmask)
{
	set_exr((UB)((current_exr() & ~0x07) | intmask));
}

/*
 *  割込みネストカウンタ
 */

extern UW	intnest;

/*
 *  コンテキスト参照
 *
 *  割込みネストカウンタを読み出した直後に割込みが発生しても、戻ったと
 *  きにはコンテキストも元に戻っている
 */

Inline BOOL
sense_context(void)
{
	return(intnest > 0);
}


Inline BOOL
sense_lock(void)
{
	return(current_intmask() == 0x07);
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock
/*
 *  CPUロックとその解除（タスクコンテキスト用）
 *
 *  task_intmask は，chg_ipm をサポートするための変数．chg_ipm をサポー
 *  トしない場合には，t_unlock_cpu 中の task_intmask は 0 に置き換えて
 *  よい．
 */

#ifdef SUPPORT_CHG_IPM
extern IPM	task_intmask;	/* タスクコンテキストでの割込みマスク */
#endif /* SUPPORT_CHG_IPM */

Inline void
t_lock_cpu(void)
{
	disint();
}

Inline void
t_unlock_cpu(void)
{
#ifdef SUPPORT_CHG_IPM
	/*
	 *  t_unlock_cpu が呼び出されるのは CPUロック状態のみであるた
	 *  め，処理の途中で task_intmask が書き換わることはない．
	 */
	set_intmask(task_intmask);
#else /* SUPPORT_CHG_IPM */
	enaint();
#endif /* SUPPORT_CHG_IPM */
}

/*
 *  CPUロックとその解除（非タスクコンテキスト用）
 */

extern 	IPM	int_intmask;	/* 非タスクコンテキストでの割込みマスク */

Inline void
i_lock_cpu(void)
{
	IPM	intmask;

	/*
	 *  一時変数 intmask を使っているのは，current_intmask()を呼ん
	 *  だ直後に割込みが発生し，起動された割込みハンドラ内で
	 *  int_intmask が変更される可能性があるためである．
	 */
	intmask = current_intmask();
	disint();
	int_intmask = intmask;
}

Inline void
i_unlock_cpu(void)
{
	set_intmask(int_intmask);
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
 *
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 *  　(例外処理ベクタテーブルが固定のために、この関数は何も処理をしない。)
 */

Inline void
define_inh(INHNO inhno, FP inthdr)
{
}

/*
 *   CPU例外ハンドラの設定
 *    (例外処理ベクタテーブルが固定のために、この関数は何も処理をしない。)
 */

Inline void
define_exc(EXCNO excno, FP exchdr)
{
}

/*
 *  割込みハンドラの出入口処理
 *
 */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *		（主に入口処理）
 *
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 *
 *
 */

/*  C言語ルーチンの関数名から入口処理のラベルを生成  */

#define	INT_ENTRY(inthdr)	inthdr##_entry
#define	EXC_ENTRY(exchdr)	exchdr##_entry

/*
 *  割込みハンドラの入口処理
 *
 *	H8Sでは、割込みとCPU例外の扱いが同じなので、
 *	入口処理のマクロも共通に定義している
 *
 *	entry：入口処理のラベル
 *	inthdr：C言語ルーチンの先頭アドレス
 *
 */

/*  割込みの出入り口処理は、in_header.txtからperlによって
 *  int_hdler.srcを生成している。  */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */


#define INTHDR_ENTRY(inthdr)				\
	extern void inthdr##_entry(void)		

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 */

#define	EXCHDR_ENTRY(exchdr)	INTHDR_ENTRY(exchdr)

/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキスト判定
 */

Inline BOOL
exc_sense_context(VP p_excinf)
{
	/* １と比較するのは、現在実行中のCPU例外の分 		*/
	/*  割込みネストカウンタがインクリメントされているため 	*/
	return(intnest > 1);
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 *  （H8S版では、呼ばれることは無いが、一応設定する） 
 */

Inline BOOL
exc_sense_lock(VP p_excinf)
{
	return(TRUE);
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
 *  未定義割込み発生時のエラー出力時のスタック構造の定義
 */

typedef struct exc_stack {
	VW	er0;
	VW	er1;
	VW	er2;
	VW	er3;
	VW	er4;
	VW	er5;
	VW	er6;
	VW	pc;	/*  プログラム・カウンタ  	*/
} EXCSTACK;

/*
 * 未定義割込み発生時のエラー出力 (cpu_config.c, cpu_support.S)
 */

extern void     cpu_experr(EXCSTACK *);

#endif /* _MACRO_ONLY_ */

#endif /* _CPU_CONFIG_H_ */
