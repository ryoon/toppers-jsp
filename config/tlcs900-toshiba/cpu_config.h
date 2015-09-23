/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2006-2010 by Witz Corporation, JAPAN
 * 
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 * 
 *  (3) When this software is redistributed in the forms unusable for the
 *      development of other software, such as the case when the software
 *      is embedded in a piece of equipment, either of the following two
 *      conditions must be satisfied:
 * 
 *    (a) The above copyright notice, this use conditions, and the
 *           disclaimer shown below must be shown without modification in
 *        the document provided with the redistributed software, such as
 *        the user manual.
 * 
 *    (b) How the software is to be redistributed must be reported to the
 *        TOPPERS Project according to the procedure described
 *        separately.
 * 
 *  (4) The above copyright holders and the TOPPERS Project are exempt
 *      from responsibility for any type of damage directly or indirectly
 *      caused from the use of this software and are indemnified by any
 *      users or end users of this software from any and all causes of
 *      action whatsoever.
 * 
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 * 
 *  @(#) $Id: cpu_config.h,v 1.1 2006/04/10 08:19:25 honda Exp $
 */

/*
 *	プロセッサ依存モジュール（TLCS-900L1用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
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
 *  chg_ipm/get_ipm をサポートするかどうかの定義
 */
#define	SUPPORT_CHG_IPM

/*
 *  TCB 中のフィールドのビット幅の定義
 *
 *  cpu_context.h に入れる方がエレガントだが，参照の依存性の関係で，
 *  cpu_context.h には入れられない．
 */
#define	TBIT_TCB_TSTAT		8	/* tstat フィールドのビット幅 */
#define	TBIT_TCB_PRIORITY	4	/* priority フィールドのビット幅 */

#ifndef _MACRO_ONLY
/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	VP	xsp;		/* スタックポインタ */
	FP	pc;		/* プログラムカウンタ */
} CTXB;

/* 
 *  割込み/CPU例外ネストカウンタ
 */
extern UINT	intcnt;

/*
 *  割込みマスク操作ライブラリ
 *
 *  割込みマスク（intmask）は，IPM（Interrupt Priority Mask）を12ビット
 *  左にシフトしたものである．
 */

/*
 *  現在の割込みマスクの読出し
 */
#pragma inline current_intmask
UH
current_intmask()
{
	return(current_sr() & 0x7000);
}

/*
 *  割込みマスクの設定
 */
#pragma inline set_intmask
void
set_intmask(UH intmask)
{
	set_sr((current_sr() & ~0x7000) | intmask);
}

/*
 *  システム状態参照
 */

#pragma inline sense_context
BOOL
sense_context()
{
	Asm("	ldc	hl, intnest");
	return __HL;
}

#pragma inline sense_lock
BOOL
sense_lock()
{
	return(current_intmask() == 0x7000);
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock

/*
 *  CPUロックとその解除（タスクコンテキスト用）
 *
 *  task_intmask は，chg_ipm をサポートするための変数．chg_ipm をサポー
 *  トしない場合には，task_intmask が常に 0 になっていると考えればよい．
 */

#ifdef SUPPORT_CHG_IPM
extern UH	task_intmask;	/* タスクコンテキストでの割込みマスク */
#endif /* SUPPORT_CHG_IPM */

#pragma inline t_lock_cpu
void
t_lock_cpu()
{
	disint();
}

#pragma inline t_unlock_cpu
void
t_unlock_cpu()
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

extern UH	int_intmask;	/* 非タスクコンテキストでの割込みマスク */

#pragma inline i_lock_cpu
void
i_lock_cpu()
{
	UH	intmask;

	/*
	 *  一時変数 intmask を使っているのは，current_intmask() を呼
	 *  んだ直後に割込みが発生し，起動された割込みハンドラ内で
	 *  int_intmask が変更される可能性があるためである．
	 */
	intmask = current_intmask();
	disint();
	int_intmask = intmask;
}

#pragma inline i_unlock_cpu
void
i_unlock_cpu()
{
	set_intmask(int_intmask);
}

/*
 *  タスクディスパッチャ
 */

/*
 *  最高優先順位タスクへのディスパッチ（cpu_support.asm）
 *
 *  dispatch は，タスクコンテキストから呼び出されたサービスコール処理
 *  内で，CPUロック状態で呼び出さなければならない．
 */
extern void	dispatch(void);

/*
 *  現在のコンテキストを捨ててディスパッチ（cpu_support.asm）
 *
 *  exit_and_dispatch は，CPUロック状態で呼び出さなければならない．
 */
extern void	exit_and_dispatch(void);

/*
 *  割込みハンドラ／CPU例外ハンドラの設定
 */

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 *  割込みハンドラを登録するベクタテーブルはROM上に取り,
 *  そこに書き込む. そのためここでは何もしない．
 */

#pragma inline define_inh
void
define_inh(INHNO inhno, FP inthdr)
{
}

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号 excno のCPU例外ハンドラの起動番地を exchdr に設定する．
 *  割込みハンドラを登録するベクタテーブルはROM上に取り,
 *  そこに書き込む. そのためここでは何もしない．
 */
#pragma inline define_exc
void
define_exc(EXCNO excno, FP exchdr)
{
}


/*
 *  割込みハンドラ／CPU例外ハンドラの出入口処理
 */

/*
 *  割込みハンドラの入口処理の生成マクロ
 *
 *  割込み発生時のスタックの切り替えをソフトウェアで実現する．このため，
 *  割込みハンドラ入り口にて割込み禁止処理を行う．なお，割込み発生から
 *  割込み禁止を行うまでの間に多重割込みが発生すると，多重割込みである
 *  ことをソフトウェアで検知できないため，最初に発生した割込み処理を実
 *  行しないままタスクディスパッチする可能性がある．この多重割込みを検
 *  知するためにハードウェア INTNEST レジスタを利用する．
 *
 *  TLCS-900 Family C Compiler では XIZ レジスタ以外は全てスクラッチレ
 *  ジスタである．なお，入り口処理で使用するスクラッチレジスタのみをこ
 *  こで保持し，残りは割込み共通処理で保持する．
 *
 *  ここを修正する場合は，下のCPU例外ハンドラ入口処理生成マクロも同様に
 *  修正すること．
 */
#define	INTHDR_ENTRY(inthdr) \
extern void _kernel_interrupt(void); \
extern void inthdr##(void); \
void inthdr##_entry(void) \
{ \
Asm("	push	xwa	");	/* 使用するスクラッチレジスタを保存 */ \
Asm("	push	sr");	/* 割込み発生時のIFFを取得	*/ \
Asm("	pop		wa"); \
Asm("	ei		7");	/* 割込み禁止	*/ \
Asm("	push	xhl	");	/* 使用するスクラッチレジスタを保存 */ \
Asm("	ld		xhl, _" #inthdr );	/* ユーザハンドラポインタを取得	*/ \
Asm("	jp	__kernel_interrupt"	);	/* 割込み共通処理へ	*/ \
}
/*
 *  上記を修正する場合は，下のCPU例外ハンドラ入口処理生成マクロも同様に
 *  修正すること．
 */


#define	INT_ENTRY(inthdr)	inthdr##_entry

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 *
 *  CPU例外ハンドラは，非タスクコンテキストで実行する．
 *  CPU例外ハンドラは，割込みハンドラと同等処理とする．
 *  なお，例外ハンドラの引数型が異なるため，同等処理のマクロを別途
 *  生成する．
 *
 *  ここを修正する場合は，上の割込みハンドラ入口処理生成マクロも同様に
 *  修正すること．
 */

#define	EXCHDR_ENTRY(exchdr) \
extern void _kernel_interrupt(void); \
extern void exchdr##(VP p_excinf); \
void exchdr##_entry(void) \
{ \
Asm("	push	xwa	");	/* 使用するスクラッチレジスタを保存 */ \
Asm("	push	sr");	/* 割込み発生時のIFFを取得	*/ \
Asm("	pop		wa"); \
Asm("	ei		7");	/* 割込み禁止	*/ \
Asm("	push	xhl	");	/* 使用するスクラッチレジスタを保存 */ \
Asm("	ld		xhl, _" #exchdr );	/* ユーザハンドラポインタを取得	*/ \
Asm("	jp	__kernel_interrupt"	);	/* 割込み共通処理へ	*/ \
}
/*
 *  ここを修正する場合は，上の割込みハンドラ入口処理生成マクロも同様に
 *  修正すること．
 */

#define	EXC_ENTRY(exchdr)	exchdr##_entry


/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキストの参照
 */
#pragma inline exc_sense_context
BOOL
exc_sense_context(VP p_excinf)
{
	Asm("	ldc	hl, intnest");
	return ( __HL > 1 );
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
#pragma inline exc_sense_lock
BOOL
exc_sense_lock(VP p_excinf)
{
	return ((*((UH *) p_excinf) & 0x7000) == 0x7000);
}

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
