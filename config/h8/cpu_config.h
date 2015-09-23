/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: cpu_config.h,v 1.11 2004/09/03 15:39:07 honda Exp $
 */

/*
 *  プロセッサ依存モジュール（H8用）
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#include <h8.h>

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

#ifndef _MACRO_ONLY

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
current_intmask()
{
	return(current_ccr() & H8INT_MASK_ALL);
}

/*
 *  割込みマスクの設定
 */

Inline void
set_intmask(UB intmask)
{
	set_ccr((current_ccr() & ~H8INT_MASK_ALL) | intmask);
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
sense_context()
{
	return(intnest > 0);
}

Inline BOOL
sense_lock()
{
	return(current_ccr() & H8INT_MASK_ALL);
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock

/*
 *  CPUロックとその解除（タスクコンテキスト用）
 */

Inline void
t_lock_cpu()
{
	disint();
}

Inline void
t_unlock_cpu()
{
	enaint();
}

/*
 *  CPUロックとその解除（非タスクコンテキスト用）
 */

extern UB	int_intmask;	/* 非タスクコンテキストでの割込みマスク */

Inline void
i_lock_cpu()
{
	UB	intmask;

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
i_unlock_cpu()
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
 *
 */

Inline void
define_inh(INHNO inhno, FP inthdr)
{
	}

/*
 *   CPU例外ハンドラの設定
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
 *	H8では、割込みとCPU例外の扱いが同じなので、
 *	入口処理のマクロも共通に定義している
 *
 *	entry：入口処理のラベル
 *	inthdr：C言語ルーチンの先頭アドレス
 *
 */

#define	_INTHDR_ENTRY(entry, inthdr) 					   \
asm(".text								\n"\
"	.align 2							\n"\
"	.global _"#entry"						\n"\
"_"#entry ":								\n"\
			/*  必要最小限のレジスタをスタックに待避  */	   \
"	push.l	er0							\n"\
"	push.l	er1							\n"\
			/*  割込みネストカウンタのチェック 		*/ \
"	mov.l	@__kernel_intnest, er0					\n"\
"	mov.l	er0, er1   						\n"\
				/*  割込みネストカウンタを +1 */	   \
"	inc.l	#1, er0							\n"\
"	mov.l	er0, @__kernel_intnest					\n"\
				/* 割込み発生時のコンテキストを判定  	*/ \
"	or.l	er1, er1   						\n"\
				/* 多重割込みならジャンプ   		*/ \
"	bne	_interrupt_from_int_"#inthdr"				\n"\
				/*  個別ハードウェア割り込み禁止 */	   \
"	jsr	@_"#inthdr"_disable_int					\n"\
				/* スタック入れ替え元の			*/ \
				/* タスクスタックポインタを保存        	*/ \
"	mov.l	sp, er0							\n"\
				/* 割込みスタックに切り替え		*/ \
"	mov.l	#"str_STACKTOP", sp					\n"\
				/* 割り込み許可				*/ \
"	andc	#"str_H8INT_ENA_ALL", ccr				\n"\
				/*  残りのレジスタを保存 */		   \
"	mov.l	er2, @-er0						\n"\
"	mov.l	er3, @-er0						\n"\
"	mov.l	er4, @-er0						\n"\
"	mov.l	er5, @-er0						\n"\
"	mov.l	er6, @-er0						\n"\
"	push.l	er0							\n"\
"	jsr	@_"#inthdr"						\n"\
				/* 割り込み禁止				*/ \
"	orc	#"str_H8INT_DIS_ALL", ccr				\n"\
				/*  個別ハードウェア割り込み許可 */	   \
"	jsr	@_"#inthdr"_enable_int					\n"\
				/* 割込みネストカウンタを-1 */		   \
"	mov.l	@__kernel_intnest, er0				  	\n"\
"	dec.l	#1, er0							\n"\
"	mov.l	er0, @__kernel_intnest					\n"\
				/* スタック切替え  			*/ \
"	mov.l	@sp, sp							\n"\
				/* reqflgのチェック                    	*/ \
"       mov.l	@__kernel_reqflg, er0							\n"\
			/* reqflgがFALSEならret_to_task_intに飛ぶ 	*/ \
"	or.l	er0, er0						\n"\
"	beq	_ret_to_task_int_"#inthdr"				\n"\
				/*  reqflgをクリア  		      	*/ \
"	sub.l	er0, er0						\n"\
"       mov.l	er0, @__kernel_reqflg					\n"\
				/*  ret_intへジャンプ  			*/ \
"	jmp	@_kernel_ret_int					\n"\
									   \
									   \
/*   多重割込みの処理 							*/ \
/* 	割込み発生時のコンテキストを判別後、割り込み禁止		*/ \
/* 	の状態でここに飛んでくる 					*/ \
/* 	（割込みネストカウンタのインクリメントは済んでいる） 		*/ \
									   \
"_interrupt_from_int_"#inthdr":						\n"\
				/*  個別ハードウェア割り込み禁止 */	   \
"	jsr	@_"#inthdr"_disable_int					\n"\
				/* 割り込み許可				*/ \
"	andc	#"str_H8INT_ENA_ALL", ccr				\n"\
				/*  残りのレジスタを保存	  	*/ \
"	push.l	er2							\n"\
"	push.l	er3							\n"\
"	push.l	er4							\n"\
"	push.l	er5							\n"\
"	push.l	er6							\n"\
"	jsr	@_"#inthdr"						\n"\
				/* 割り込み禁止				*/ \
"	orc	#"str_H8INT_DIS_ALL", ccr				\n"\
				/*  個別ハードウェア割り込み許可 */	   \
"	jsr	@_"#inthdr"_enable_int					\n"\
				/* 割込みネストカウンタを -1 */		   \
"	mov.l	@__kernel_intnest, er0				  	\n"\
"	dec.l	#1, er0							\n"\
"	mov.l	er0, @__kernel_intnest					\n"\
									   \
									   \
/*   reqflgがFALSEの場合の処理 						*/ \
									   \
"_ret_to_task_int_"#inthdr": \n"					   \
"	pop.l	er6	\n"	/*  レジスタ復元  			*/ \
"	pop.l	er5	\n"						   \
"	pop.l	er4	\n"						   \
"	pop.l	er3	\n"						   \
"	pop.l	er2	\n"						   \
"	pop.l	er1	\n"						   \
"	pop.l	er0	\n"						   \
"	rte		\n"	/*  割込み元に戻る  			*/ \
)

/*  _INTHDR_ENTRY()マクロ　ここまで  */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */

#define INTHDR_ENTRY(inthdr)				\
	extern void inthdr##_entry(void);		\
	_INTHDR_ENTRY(inthdr##_entry, inthdr)

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
 */

Inline BOOL
exc_sense_lock(VP p_excinf)
{
	return(*((UW *)p_excinf) & (((UW)H8INT_MASK_ALL) << 24));
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

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */

extern void cpu_putc(char c);

#endif /* _MACRO_ONLY_ */

#endif /* _CPU_CONFIG_H_ */
