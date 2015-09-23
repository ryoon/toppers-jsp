/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: cpu_config.h,v 1.3 2001/11/02 11:13:39 imai Exp $
 */


/*
 *  プロセッサ依存モジュール（SH1用）
 */


#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#include "sh1.h"	/*  内部レジスタの型が必要  */

/*
 *  カーネルの内部識別名のリネーム
 */
#ifndef OMIT_RENAME

#define activate_r		_kernel_activate_r
#define ret_int			_kernel_ret_int
#define ret_exc			_kernel_ret_exc
#define task_intmask		_kernel_task_intmask
#define int_intmask		_kernel_int_intmask
#define intnest			_kernel_intnest

#ifdef LABEL_ASM

#define _activate_r		__kernel_activate_r
#define _ret_int		__kernel_ret_int
#define _ret_exc		__kernel_ret_exc
#define _task_intmask		__kernel_task_intmask
#define _int_intmask		__kernel_int_intmask
#define _intnest		__kernel_intnest


#endif /* LABEL_ASM */
#endif /* OMIT_RENAME */

/*
 *  設定可能な最高優先度
 */
#ifdef WITH_STUB
#define MAX_IPM  0xe	/* スタブありの場合は優先度14でCPUロック   */
#else
#define MAX_IPM  0xf	/* スタブなしの場合は最高優先度でCPUロック */
#endif /*  WITH_STUB  */

#define str_MAX_IPM  	_TO_STRING(MAX_IPM)
#define str_intnest  	_TO_STRING(_intnest)
#define str_reqflg  	_TO_STRING(_reqflg)
#define str_ret_int  	_TO_STRING(_ret_int)


/*
 *  プロセッサの特殊命令のインライン関数定義
 */
#ifndef _MACRO_ONLY
#include "cpu_insn.h"
#endif /* _MACRO_ONLY */

/*
 *  chg_ipm/get_ipm をサポートするかどうかの定義
 */
#define	SUPPORT_CHG_IPM

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

#ifndef _MACRO_ONLY
/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
        VP	sp;             /* スタックポインタ */
        FP	pc;             /* プログラムカウンタ */
} CTXB;

/*
 *  割込みマスク操作ライブラリ
 *
 *  割込みマスク（intmask）は，IPM（Interrupt Priority Mask）を4ビット
 *  左にシフトしたものである．
 */


/*
 *  現在の割込みマスクの読出し
 */
Inline UW
current_intmask()
{
	return(current_sr() & 0x000000f0);
}

/*
 *  割込みマスクの設定
 */
Inline void
set_intmask(UW intmask)
{
	set_sr((current_sr() & ~0x000000f0) | intmask);
}

/*
 *  割込み／CPU例外ネストカウンタ
 */
extern UW	intnest;

/*
 *  システム状態参照
 */

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
	return(current_intmask() == MAX_IPM << 4);
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
extern UW	task_intmask;	/* タスクコンテキストでの割込みマスク */
#endif /* SUPPORT_CHG_IPM */

Inline void
t_lock_cpu()
{
	disint();		/*  cpu_insn.h  */
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
	enaint();		/*  cpu_insn.h  */
#endif /* SUPPORT_CHG_IPM */
}


/*
 *  CPUロックとその解除（非タスクコンテキスト用）
 */

extern UW	int_intmask;	/* 非タスクコンテキストでの割込みマスク */

Inline void
i_lock_cpu()
{
	UW	intmask;

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
 *  割込みハンドラ／CPU例外ハンドラの設定
 */


/*
 *  例外ベクタテーブルの構造の定義
 */
typedef struct exc_vector_entry {
	FP	exchdr;			/* 例外ハンドラの起動番地 */
} EXCVE;

#endif /* #ifndef _MACRO_ONLY_ */

/*
 *  ベクタベースの定義
 */


#define EXCVT_SIZE	256


#ifndef _MACRO_ONLY

extern EXCVE BASE_VBR[EXCVT_SIZE];

#define EXCVT_KERNEL	BASE_VBR
#define EXCVT_ORIG	0

/*
 *
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 *  stubを使う場合はstub呼び出しによりstubに登録する
 *
 */

extern FP general_exception();
extern FP interrupt();

Inline void
define_inh(INHNO inhno, FP inthdr)
{
	EXCVE	*excvt;

#ifdef EXCVT_KERNEL
	/*
	 *  EXCVT_KERNEL が定義されている時は，初期化処理の中で VBR を
	 *  EXCVT_KERNEL に設定するので，EXCVT_KERNEL を使う．
	 */
	excvt = (EXCVE *) EXCVT_KERNEL;
#else /* EXCVT_KERNEL */
	excvt = (EXCVE *) current_vbr();
#endif /* EXCVT_KERNEL */
	excvt[inhno].exchdr = inthdr;

#ifdef WITH_STUB	/*  注意：スタブ呼び出し  */
        Asm("mov #0x8,r0;  mov %0,r4; mov %1,r5; trapa #0x3f"
	    : /* no output */
	    : "r"(inhno),"r"(interrupt)
	    : "r0", "r4", "r5");
#endif
}

/*
 *   CPU例外ハンドラの設定
 */

Inline void
define_exc(EXCNO excno, FP exchdr)
{
	/*  SH1は割込みもCPU例外も同じ形式  */
	define_inh(excno, exchdr);
}


/*
 *  割り込みレベルの設定
 *	（共通部からは呼び出されない）
 */

Inline void
define_int_plevel(HIOREG *reg, UW level, UINT shift)
{
    *reg = (HIOREG)((*reg & ~(0xf << shift)) | (level << shift));
}



/*
 *  割込みハンドラ／CPU例外ハンドラの出入口処理
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
 *  割込みハンドラ／CPU例外ハンドラの入口処理
 *
 *	SH1では、割込みとCPU例外の扱いがほとんど同じなので、
 *	入口処理のマクロも共通に定義している
 *	相違点
 *	　　・CPU例外ハンドラに引数を与える
 *	　　・CPU例外では、例外発生時のIPMと同じ値で割込みを許可する
 *		（CPU例外により、IPMを変化させない）
 *
 *	entry：入口処理のラベル
 *	inthdr：C言語ルーチンの先頭アドレス
 *	set_exchdr_arg1：
 *	set_exchdr_arg2：
 *		CPU例外ハンドラの引数設定用の命令
 *
 */
#define	_INTHDR_ENTRY(entry, inthdr, 					   \
			set_exchdr_arg1, set_exchdr_arg2) 		   \
asm(".text								\n"\
"	.align 2							\n"\
"	.global _"#entry"						\n"\
"_"#entry ":								\n"\
			/*  必要最小限のレジスタをスタックに待避  */	   \
"	mov.l	r0,@-r15						\n"\
				/*  割込み禁止とSR復元の準備  		*/ \
		/*  割込みハンドラの場合				*/ \
		/*  	割込み受付時にハードウェアがIPMを設定する	*/ \
		/*  	割込み許可にはこのIPMの値を用いる		*/ \
		/*  CPU例外ハンドラの場合				*/ \
		/*  	CPU例外受付時にIPMの値は変化しない		*/ \
		/*  	割込み許可にはCPU例外発生時のIPMの値を用いる	*/ \
"	mov.l	r1,@-r15						\n"\
				/*  srをr1にコピー			*/ \
"	stc	sr,r1							\n"\
				/*  割込み禁止  			*/ \
"	mov.l	_mask_ipm_"#inthdr",r0					\n"\
"	ldc	r0,sr							\n"\
				/*  r2をスタックに待避  		*/ \
"	mov.l	r2,@-r15						\n"\
			/*  割込み／CPU例外ネストカウンタのチェック 	*/ \
"	mov.l	_intnest_k_"#inthdr", r0				\n"\
"	mov.l	@r0,r2							\n"\
				/* 割込み発生時のコンテキストを判定  	*/ \
"	tst     r2,r2   						\n"\
				/*  割込みネストカウンタをインクリメント*/ \
"	add	#0x1,r2							\n"\
				/*  add命令ではsrのTビットは変化しない	*/ \
"	mov.l	r2,@r0							\n"\
				/* 多重割込みならジャンプ   		*/ \
"	bf	_interrupt_from_int_"#inthdr"				\n"\
	/*  遅延スロットなし  */					   \
				/* スタック入れ替え元の			*/ \
				/* タスクスタックポインタを保存        	*/ \
"	mov     r15,r2							\n"\
				/* 割込みスタックに切り替え		*/ \
"	mov.l   _stacktop_k_"#inthdr",r15				\n"\
				/*  割込み許可  */			   \
"	ldc  	r1,sr							\n"\
				/*  残りのスクラッチレジスタを保存  	*/ \
				/*  	r2:タスクスタックポインタ  	*/ \
"	mov.l   r3,@-r2							\n"\
"	mov.l   r4,@-r2							\n"\
"	mov.l   r5,@-r2							\n"\
"	mov.l   r6,@-r2							\n"\
"	mov.l   r7,@-r2							\n"\
"	sts.l	pr,@-r2							\n"\
	set_exchdr_arg1		/*  CPU例外の場合の引数設定  		*/ \
				/*  C言語ルーチン呼び出し		*/ \
"	mov.l	_c_routine_"#inthdr",r1					\n"\
"	jsr	@r1							\n"\
"	mov.l	r2,@-r15 \n"	/*  遅延スロット　  			*/ \
				/*  タスクスタックポインタを		*/ \
				/*  割込みスタックに積む  		*/ \
									   \
				/*  割込み禁止		*/		   \
"	mov.l	_mask_ipm_"#inthdr",r0					\n"\
"	ldc	r0,sr							\n"\
				/* 割込み／CPU例外ネストカウンタをクリア*/ \
"	mov.l	_intnest_k_"#inthdr",r0				  	\n"\
"	mov	#0x0,r1							\n"\
"	mov.l	r1,@r0							\n"\
				/* スタック切替え  			*/ \
"	mov.l	@r15,r15						\n"\
				/* reqflgのチェック                    	*/ \
"	mov.l	_reqflg_k_"#inthdr",r4					\n"\
"       mov.l	@r4,r1							\n"\
			/* reqflgがFALSEならret_to_task_intに飛ぶ 	*/ \
"	tst	r1,r1							\n"\
"	bt   	_ret_to_task_int_"#inthdr"				\n"\
	/*  遅延スロットなし  */					   \
"	mov	#0x0,r0							\n"\
"	mov.l	ret_int_"#inthdr",r1					\n"\
				/*  ret_intへジャンプ  			*/ \
"	jmp	@r1							\n"\
				/*  遅延スロット reqflgをクリア        	*/ \
"	mov.l	r0,@r4							\n"\
									   \
									   \
/*   多重割込みの処理 							*/ \
/* 	割込み発生時のコンテキストを判別後、 				*/ \
/*   		割込み禁止 						*/ \
/*   		r1：割込み受付時のsrのコピー 				*/ \
/* 	の状態でここに飛んでくる 					*/ \
/* 	（割込みネストカウンタのインクリメントは済んでいる） 		*/ \
									   \
"_interrupt_from_int_"#inthdr":						\n"\
				/* 割込み許可  				*/ \
"	ldc	r1,sr							\n"\
			/*  残りのスクラッチレジスタをスタックに積む　	*/ \
"	mov.l	r3,@-r15						\n"\
"	mov.l	r4,@-r15						\n"\
"	mov.l	r5,@-r15						\n"\
"	mov.l	r6,@-r15						\n"\
"	mov.l	r7,@-r15						\n"\
"	sts.l	pr,@-r15						\n"\
	set_exchdr_arg2			/*  CPU例外の場合の引数設定  	*/ \
					/*  C言語ルーチン呼び出し 	*/ \
"	mov.l	_c_routine_"#inthdr",r1					\n"\
"	jsr	@r1							\n"\
"	nop			\n"	/*  遅延スロット　 		*/ \
									   \
					/*  割込み禁止			*/ \
"	mov.l	_mask_ipm_"#inthdr",r0					\n"\
"	ldc	r0,sr							\n"\
		/* 割込み／CPU例外ネストカウンタをディクリメント	*/ \
"	mov.l	_intnest_k_"#inthdr",r0					\n"\
"	mov.l	@r0,r2							\n"\
"	add	#-1,r2							\n"\
"	mov.l	r2,@r0							\n"\
									   \
									   \
/*   reqflgがFALSEの場合の処理 						*/ \
/*    									*/ \
/*   	・ディスパッチもタスク例外処理も必要ない 			*/ \
/* 	・chg_ipm()の処理は必要ない 					*/ \
/* 	　（非タスクコンテキストでは、chg_ipm()は使用不可） 		*/ \
									   \
"_ret_to_task_int_"#inthdr": \n"					   \
"	lds.l	@r15+,pr    \n"	/*  レジスタ復元  			*/ \
"	mov.l	@r15+,r7    \n"						   \
"	mov.l	@r15+,r6    \n"						   \
"	mov.l	@r15+,r5    \n"						   \
"	mov.l	@r15+,r4    \n"						   \
"	mov.l	@r15+,r3    \n"						   \
"	mov.l	@r15+,r2    \n"						   \
"	mov.l	@r15+,r1    \n"						   \
"	mov.l	@r15+,r0    \n"						   \
"	rte		    \n"	/*  割込み元に戻る  			*/ \
"	nop		    \n"						   \
									   \
"	.align  4	    \n"						   \
"_stacktop_k_"#inthdr":     \n"	/* タスク独立部のスタックの初期値  	*/ \
"	.long  "str_STACKTOP" \n"					   \
"_intnest_k_"#inthdr":	    \n"	/*  割込み／CPU例外ネストカウンタ  	*/ \
"	.long  "str_intnest"	    \n"				  	   \
"_reqflg_k_"#inthdr":	    \n"						   \
"	.long  "str_reqflg" \n"						   \
"_mask_ipm_"#inthdr":	    \n"	/*  割込み禁止用マスク  		*/ \
"	.long  "str_MAX_IPM" << 4 \n"	/*  ipm以外のビットはゼロで良い	*/ \
"_c_routine_"#inthdr":	    \n"						   \
"	.long  _"#inthdr"   \n" /*  C言語ルーチンの先頭アドレス  	*/ \
"ret_int_"#inthdr":	    \n"	/*  出口処理ret_intのアドレス  		*/ \
"	.long  "str_ret_int"\n"				  		   \
)
/*  _INTHDR_ENTRY()マクロ　ここまで  */



/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)				\
	extern void inthdr##_entry(void);		\
	_INTHDR_ENTRY(inthdr##_entry, inthdr, "", "")


/*
 *  CPU例外の場合の引数設定用マクロ
 *
 *　　　　タスクスタックポインタをコピー
 *　　　　　（スタック先頭から40バイト下にsrが積まれている）
 */
#define	SET_EXCHDR_ARG(sp)						\
"	mov	"#sp",r4						\n"\
"	add	#40,r4							\n"


/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 *  　注意
 *  	一般不当命令の場合は戻り番地を2バイト進める必要があるが
 *	対応していない
 */
#define	EXCHDR_ENTRY(exchdr)  					\
	extern void exchdr##_entry(VP sp);			\
	_INTHDR_ENTRY(exchdr##_entry, exchdr,			\
			SET_EXCHDR_ARG(r2), SET_EXCHDR_ARG(r15))

/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキスト判別
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
	return(((*(UW *)p_excinf) & 0x00000f0) == MAX_IPM << 4);
}

/*
 *  ラベルの別名を定義するためのマクロ
 */
#define	_LABEL_ALIAS(new_label, defined_label) \
	asm(".globl _" #new_label "\n_" #new_label " = _" #defined_label);
#define LABEL_ALIAS(x, y) _LABEL_ALIAS(x, y)

/*
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

/*
 * プロセッサ依存シリアル出力
 */
extern void     cpu_putc(char c);


/*
 *  下記のログ出力時のスタック構造の定義
 */
typedef struct exc_stack {
	VW	r0;
	VW	r1;
	VW	r2;
	VW	r3;
	VW	r4;
	VW	r5;
	VW	r6;
	VW	r7;
	VW	r8;
	VW	r9;
	VW	r10;
	VW	r11;
	VW	r12;
	VW	r13;
	VW	r14;
	VW	r15;
	VW	pr;	/*  プロシージャ・レジスタ  	*/
	VW	pc;	/*  プログラム・カウンタ  	*/
	VW	sr;	/*  ステータス・レジスタ  	*/
} EXCSTACK;


/*
 * 例外発生時のログ出力 (cpu_config.c, cpu_support.S)
 */
extern void     cpu_experr(EXCSTACK *);


#endif /* _MACRO_ONLY_ */
#endif /* _CPU_CONFIG_H_ */
