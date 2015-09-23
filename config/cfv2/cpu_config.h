/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *  @(#) $Id: cpu_config.h,v 1.14 2003/07/08 14:57:12 hiro Exp $
 */

/*
 *	プロセッサ依存モジュール（CFV2用）
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
 *  数値データ文字列化用マクロの定義
 */
#include <util.h>

/*
 *  ユーザー定義情報
 */
#include <user_config.h>

/*
 *  設定可能な最高優先度
 */
#ifdef GDB_STUB
#define MAX_IPM  0x6	/* スタブありの場合は優先度6でCPUロック   */
#else
#define MAX_IPM  0x7	/* スタブなしの場合は最高優先度でCPUロック */
#endif /*  GDB_STUB  */

#define str_MAX_IPM  		TO_STRING(MAX_IPM)

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
#define	TBIT_TCB_PRIORITY	8	/* priority フィールドのビット幅 */

#ifndef _MACRO_ONLY
/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	VP	msp;		/* スタックポインタ */
	FP	pc;		/* プログラムカウンタ */
} CTXB;

/*
 *  割込みマスク操作ライブラリ
 *
 *  割込みマスク（intmask）は，IPM（Interrupt Priority Mask）を8ビット
 *  左にシフトしたものである．
 */

/*
 *  現在の割込みマスクの読出し
 */
Inline UH
current_intmask()
{
	return(current_sr() & 0x0700);
}

/*
 *  割込みマスクの設定
 */
Inline void
set_intmask(UH intmask)
{
	set_sr((current_sr() & ~0x0700) | intmask);
}

/*
 *  割込み／CPU例外ネストカウンタ
 */
extern UW	intnest;

/*
 *  システム状態参照
 */

Inline BOOL
sense_context()
{
	return(intnest > 0);
}

Inline BOOL
sense_lock()
{
	return(current_intmask() == (MAX_IPM << 8));
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

Inline void
t_lock_cpu()
{
	disint();
}

Inline void
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

Inline void
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

#endif  /* _MACRO_ONLY */

/*
 *  割込みハンドラ／CPU例外ハンドラの設定
 */

/*
 *  例外ベクタテーブルの定義
 *  　　マクロの説明
 *  　　　KERNEL_HAS_A_VECTOR_TABLE
 *  　　　　カーネルが例外ベクタテーブルを持つ
 *  　　　SIO_RESERVED
 *  　　　　シリアルデバイスがデバッガによって使用されている
 */
#ifndef _MACRO_ONLY

#ifdef KERNEL_HAS_A_VECTOR_TABLE
extern FP vector_table[];	/*  例外ベクタテーブル  */
#endif /* KERNEL_HAS_A_VECTOR_TABLE */

typedef struct exc_vector_entry {
	FP	exchdr;			/* 例外ハンドラの起動番地 */
} EXCVE;

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
Inline void
define_inh(INHNO inhno, FP inthdr)
{
#ifdef GDB_STUB
        Asm("moveq.l #0x8,%%d0;  move.l %0,%%d2; move.l %1,%%d3; trap #0xf"
	    : /* no output */
	    : "r"(inhno), "r"(inthdr)
	    : "%%d0", "%%d2", "%%d3");
#else	/*  GDB_STUB  */

#ifdef KERNEL_HAS_A_VECTOR_TABLE

/* SIO_RESERVEDは使用しない。sys_config.hでコメントにしてある */
/*#ifdef SIO_RESERVED	*//*  シリアル割り込みは避ける  */
/*	if ((inhno != RXI0) && (inhno != TXI0))*/
/*#endif *//* SIO_RESERVED */ 
	vector_table[inhno] = inthdr;

#endif /* KERNEL_HAS_A_VECTOR_TABLE */

#endif	/*  GDB_STUB  */
}

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号 excno のCPU例外ハンドラの起動番地を exchdr に設定する．
 */
Inline void
define_exc(EXCNO excno, FP exchdr)
{
	define_inh(excno, exchdr);
}

/*  C言語ルーチンの関数名から入口処理のラベルを生成  
 *     kernel_cfg.cで使用
 */
#define	INT_ENTRY(inthdr)	_kernel_##inthdr##_entry
#define	EXC_ENTRY(exchdr)	_kernel_##exchdr##_entry


/*
 *  割込みハンドラ／CPU例外ハンドラの入口処理の共通部分
 *
 *	ColdFireでは、割込みとCPU例外の扱いがほとんど同じなので、
 *	入口処理のマクロも共通に定義している
 *
 *　　引数
 *	label：入口処理のラベル
 *	inthdr：C言語ルーチンの先頭アドレス
 *	common_routine：cpu_support.S内の分岐先アドレス
 *			interrupt_entry：割込みの場合
 *			cpu_exception_entry：CPU例外の場合
 *
 *　　レジスタ割当
 *　　　　d1：割込み受付直後のSRのコピー
 *　　　　a0：C言語ルーチンの先頭アドレス
 *
 *	割込みとCPU例外の相違点
 *	　　・CPU例外ハンドラに引数を与える
 *	　　・CPU例外では、例外発生時のIPMと同じ値で割込みを許可する
 *		（CPU例外により、IPMを変化させない）
 *
 *	　　・割込みハンドラの場合				
 *	　　  	割込み受付時にハードウェアがIPMを設定する	
 *	　　  	割込み許可にはこのIPMの値を用いる		
 *	　　・CPU例外ハンドラの場合				
 *	　　  	CPU例外受付時にIPMの値は変化しない		
 *	　　  	割込み許可にはCPU例外発生時のIPMの値を用いる	
 */
#define	HANDLER_ENTRY_PROC(label, handler, common_routine)		   \
asm(".text								\n"\
"	.align 2							\n"\
"	.global "#label"						\n"\
#label":								\n"\
"	lea.l   -16(%sp), %sp		\n"                                \
"	movem.l %d0-%d1/%a0-%a1, (%sp)	\n" /* スクラッチレジスタを保存 */ \
				/*  割込み禁止とSR復元の準備  		*/ \
"	move.w	%sr,%d1							\n"\
	/*  割込み禁止  						*/ \
	/*     割込みを禁止する前に別の割込みが入った場合の注意点は 	*/ \
	/*     cpu_suppourt.Sのret_intを参照 				*/ \
"	move.l	#("str_MAX_IPM" << 8),%d0	                        \n"\
"	ori.l	#0x00002000,%d0	                                        \n"\
"	move.w	%d0,%sr	                                                \n"\
"	move.l	#"#handler", %a0				                \n"\
"	jbra	_kernel_"#common_routine"        \n" /* C言語ルーチンの先頭アドレス*/\
)

/*
 *  割込みハンドラの入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)	INTHDR_ENTRY2(INT_ENTRY(inthdr), inthdr)

#define INTHDR_ENTRY2(entry, inthdr)					\
	extern void entry(void);					\
	HANDLER_ENTRY_PROC(entry, inthdr, interrupt_entry)


/*
 *  CPU例外ハンドラの入口処理の生成マクロ
 */
#define	EXCHDR_ENTRY(exchdr)	EXCHDR_ENTRY2(EXC_ENTRY(exchdr), exchdr)

#define EXCHDR_ENTRY2(entry, exchdr)					\
	extern void entry(void);					\
	HANDLER_ENTRY_PROC(entry, exchdr, cpu_exception_entry)
	/*
	 *  void entry()は単なるエントリのラベルなので、
	 *  引数は付けない
	 */

/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキストの参照
 */
Inline BOOL
exc_sense_context(VP p_excinf)
{
	/* 
	 *  １と比較するのは、現在実行中のCPU例外の分
	 *  割込みネストカウンタがインクリメントされているため 	
	 *  
	 *  CPU例外の入口処理中に別のCPU例外は発生しないと仮定
	 *  している
	 */
	return(intnest > 0);
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
Inline BOOL
exc_sense_lock(VP p_excinf)
{
	return((*((UH *) p_excinf) & 0x0700) == 0x0700);
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
 *  下記のログ出力時のスタック構造の定義
 */
typedef struct exc_stack {
	VW	d0;
	VW	d1;
	VW	d2;
	VW	d3;
	VW	d4;
	VW	d5;
	VW	d6;
	VW	d7;
	VW	a0;
	VW	a1;
	VW	a2;
	VW	a3;
	VW	a4;
	VW	a5;
	VW	a6;
	VW	a7;
	VW	sr;	/*  ステータス・レジスタ  	*/
	VW	pc;	/*  プログラム・カウンタ  	*/
} EXCSTACK;


/*
 * 例外発生時のログ出力 (cpu_config.c, cpu_support.S)
 */
extern void     cpu_experr(EXCSTACK *);

#endif /* _MACRO_ONLY */


/*
 *  例外ベクタに設定するデフォルトの値
 *  	以下の例外要因でデフォルトとは異なる例外ベクタを定義
 *	する場合は、sys_config.hで該当するマクロを定義する。
 */
#define RESET_VECTOR 	start		/*  リセットベクタ  */
#define INIT_STACK  	STACKTOP	/*  スタックポインタの初期値  */

#define RESERVED_VECTOR	RESET_VECTOR	/*  システム予約のリセットベクタ  */
					/*  実際には使用されない  */

#ifndef ACC_VECTOR	/*  アクセスエラー  */
#define ACC_VECTOR	RESET_VECTOR
#endif /* ACC_VECTOR */

#ifndef ADD_VECTOR	/*  アクセスエラー  */
#define ADD_VECTOR	RESET_VECTOR
#endif /* ACC_VECTOR */

#ifndef II_VECTOR	/*  不当命令  */
#define II_VECTOR	RESET_VECTOR
#endif /* II_VECTOR */

#ifndef PV_VECTOR	/*  スロット不当命令  */
#define PV_VECTOR	RESET_VECTOR
#endif /* SII_VECTOR */

#ifndef CAE_VECTOR	/*  CPUアドレスエラー  */
#define CAE_VECTOR	RESET_VECTOR
#endif /* CAE_VECTOR */

#ifndef DAE_VECTOR	/*  DMAアドレスエラー  */
#define DAE_VECTOR	RESET_VECTOR
#endif /* DAE_VECTOR */

#ifndef NMI_VECTOR	/*  NMI  */
#define NMI_VECTOR	RESET_VECTOR
#endif /* NMI_VECTOR */

#endif /* _CPU_CONFIG_H_ */
