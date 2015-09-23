/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: cpu_config.h,v 1.9 2003/12/19 10:13:47 honda Exp $
 */

/*
 *	プロセッサ依存モジュール（SH1用）
 *　　　　　カーネル内部で使用する定義
 *　　　　　　データ型、マクロ、関数のプロトタイプ宣言
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
 *  SH1プロセッサの内部レジスタの定義
 */
#include <sh1.h>

/*
 *  設定可能な最高優先度
 */
#ifdef GDB_STUB
#define MAX_IPM  0xe	/* スタブありの場合は優先度14でCPUロック   */
#else
#define MAX_IPM  0xf	/* スタブなしの場合は最高優先度でCPUロック */
#endif /*  GDB_STUB  */

#define str_MAX_IPM  	TO_STRING(MAX_IPM)
#define str_intnest  	TO_STRING(_intnest)
#define str_reqflg  	TO_STRING(_reqflg)
#define str_ret_int  	TO_STRING(_ret_int)

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
 *  vxget_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

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
	return(current_sr() & 0x000000f0u);
}

/*
 *  割込みマスクの設定
 */
Inline void
set_intmask(UW intmask)
{
	set_sr((current_sr() & ~0x000000f0u) | intmask);
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
 *  割込みネストカウンタを読み出した直後に割込みが発生しても、
 *  戻ったときにはコンテキストも元に戻っている
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
 *  トしない場合には，task_intmask が常に 0 になっていると考えればよい．
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

/*
 *  割込みハンドラ／CPU例外ハンドラの設定
 */

/*
 *  例外ベクタテーブルの構造の定義
 */
typedef struct exc_vector_entry {
	FP	exchdr;			/* 例外ハンドラの起動番地 */
} EXCVE;

#define EXCVT_SIZE	256

#ifndef CQ_SH1_DEB

extern EXCVE BASE_VBR[EXCVT_SIZE];

#define EXCVT_KERNEL	BASE_VBR
#define EXCVT_ORIG	0

#endif /* CQ_SH1_DEB */

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
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
	excvt[inhno].exchdr = inthdr;
#else /* EXCVT_KERNEL */
	excvt = (EXCVE *) current_vbr();

#ifdef CQ_SH1_DEB	/*  シリアル割り込みは避ける  */
	if ((inhno != RXI0) && (inhno != TXI0))
#endif /* CQ_SH1_DEB */
		excvt[inhno].exchdr = inthdr;


#endif /* EXCVT_KERNEL */



#ifdef GDB_STUB	/*  スタブ呼び出し  */


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
	/*  SH1は割込みもCPU例外も同じ形式  */
	define_inh((INHNO)excno, exchdr);
}

/*
 *  割込みハンドラ／CPU例外ハンドラの出入口処理
 */

/*  C言語ルーチンの関数名から入口処理のラベルを生成  
 *     kernel_cfg.cで使用
 */
#define	INT_ENTRY(inthdr)	_kernel_##inthdr##_entry
#define	EXC_ENTRY(exchdr)	_kernel_##exchdr##_entry

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 *
 *  CPU例外ハンドラは，非タスクコンテキストで実行する．そのため，CPU例
 *  外ハンドラを呼び出す前に割込みモードに移行し，リターンしてきた後に
 *  元のモードに戻す．元のモードに戻すために，割込みモードに移行する前
 *  の SR を割込みスタック上に保存する．CPU例外がタスクコンテキストで
 *  発生し，reqflg が TRUE になった時に，ret_exc へ分岐する．
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 */

/*
 *  割込みハンドラ／CPU例外ハンドラの入口処理
 *
 *	SH1では、割込みとCPU例外の扱いがほとんど同じなので、
 *	入口処理のマクロも共通に定義している
 *
 *	割込みとCPU例外の相違点
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
"	.long  __kernel_intnest	    \n"				  	   \
"_reqflg_k_"#inthdr":	    \n"						   \
"	.long  __kernel_reqflg \n"					   \
"_mask_ipm_"#inthdr":	    \n"	/*  割込み禁止用マスク  		*/ \
"	.long  "str_MAX_IPM" << 4 \n"	/*  ipm以外のビットはゼロで良い	*/ \
"_c_routine_"#inthdr":	    \n"						   \
"	.long  _"#inthdr"   \n" /*  C言語ルーチンの先頭アドレス  	*/ \
"ret_int_"#inthdr":	    \n"	/*  出口処理ret_intのアドレス  		*/ \
"	.long  __kernel_ret_int \n"			  		   \
)
/*  _INTHDR_ENTRY()マクロ　ここまで  */




/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)					\
	extern void _kernel_##inthdr##_entry(void);		\
	_INTHDR_ENTRY(_kernel_##inthdr##_entry, inthdr, "", "")


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
 *  　
 *  	一般不当命令の場合は戻り番地を2バイト進める必要があるが
 *	対応していない
 *	（GDB stubがブレークポイントとして使用する。）
 */
#define	EXCHDR_ENTRY(exchdr)  						\
	extern void _kernel_##exchdr##_entry(VP sp);			\
	_INTHDR_ENTRY(_kernel_##exchdr##_entry, exchdr,			\
			SET_EXCHDR_ARG(r2), SET_EXCHDR_ARG(r15))


/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のコンテキストの参照
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
	UW sr = *(UW *)p_excinf; 
	return((sr& 0x00000f0u) == MAX_IPM << 4);
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

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
