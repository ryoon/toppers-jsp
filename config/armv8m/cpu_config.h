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
 *		プロセッサ依存モジュール（Cortex-M33用）
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
 * システム個別設定
 */
#include "sys_defs.h"

#ifndef _MACRO_ONLY
#include <sil.h>
#endif
/*
 *  最大エクセプションと割込み数を定義
 */
#define NUM_INTNO	(TMAX_INTNO + 1)
#define NUM_EXCNO	16

/**
 *  割込み優先度のビット幅
 */
#ifndef TBITW_IPRI
#define TBITW_IPRI	3
#endif

#define MAX_IPM		((1 << TBITW_IPRI) - 1)

/*
 *  割込み優先度マスクをNVICの優先度に変換
 */
#define INT_NVIC_PRI(ipm)	((ipm)  << (8 - TBITW_IPRI))

/*
 *  TCB 関連の定義
 *
 *  cpu_context.h に入れる方がエレガントだが，参照の依存性の関係で，
 *  cpu_context.h には入れられない．
 */

/*
 *  TCB 中のフィールドのビット幅の定義
 */
#define	TBIT_TCB_TSTAT		8    /* tstat フィールドのビット幅 */
#define	TBIT_TCB_PRIORITY	8    /* priority フィールドのビット幅 */

#ifndef _MACRO_ONLY
/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
    VP    sp;        /* スタックポインタ */
    FP    pc;        /* プログラムカウンタ */
} CTXB;

/*
 *  ARMV8M依存プロセッサコントロールブロック
 */
typedef struct system_processor_control_block {
	/*
	 *  CPUロックフラグ実現のための変数
	 *
	 *  これらの変数は，CPUロック状態の時のみ書き換えてもよいとする．
	 *  インライン関数中で，アクセスの順序が変化しないよう，volatile を指定．
	 */
	volatile UW	interrupt_map; /* 割込み有効化マップ */

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
 *  割込みハンドラテーブル
 */
extern FP* const p_int_table[];

/*
 *  例外の許可
 */
extern void enable_exc(EXCNO excno);

/*
 *  例外の禁止
 */
extern void disable_exc(EXCNO excno);


/*
 *  TOPPERS標準割込み処理モデルの実現
 *
 *  割込み許可禁止としては，PRIMASKを用いる．非タスクコンテキスト
 *  では、FAULTMASKのfビットを変併用する．
 *
 *  割込み優先度マスクとしてBASEPRIを用いる．
 *  ターゲット依存部の簡素化のためカーネル管理外の割込みを
 *  サポートしない．
 */

/*
 *  STPCBを取り出す
 */
Inline STPCB *
get_sys_stpcb(void)
{
	return p_tspcb_table[x_prc_index()];
}

/*
 *  コンテキストの参照
 *
 *  割込みネストレベルをスタックの種類で判定する．
 */
Inline BOOL
sense_context(void)
{
#ifndef OMIT_PSP
	/*
	 *  PSPが有効ならタスクコンテキスト，MSPが有効なら非タスクコンテキスト
	 *  とする． 
	 */
	if ((get_control() & CONTROL_PSP) == CONTROL_PSP){
		return FALSE;
	}
	else {
		return TRUE;
	}
#else /* OMIT_PSP */
	return get_sys_stpcb()->inest_lvl != 0;
#endif /* OMIT_PSP */
}

/*
 *  CPUロック状態の参照
 */
Inline BOOL
sense_lock(void)
{
	return(read_primask() == 0x1u);
}

#define t_sense_lock()	sense_lock()
#define i_sense_lock()	sense_lock()


/*
 *  CPUロック状態への移行
 *
 *  primaskのiビットを1に設定
 */
Inline void
x_lock_cpu(void)
{
	set_primask();
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	ARM_MEMORY_CHANGED;
}

#define t_lock_cpu()	x_lock_cpu()
#define i_lock_cpu()	x_lock_cpu()

/*
 *  CPUロック状態の解除
 *
 *  primaskのiビットを0に設定
 */
Inline void
x_unlock_cpu(void)
{
	/* クリティカルセクションの前後でメモリが書き換わる可能性がある */
	ARM_MEMORY_CHANGED;
	clear_primask();
}

#define t_unlock_cpu()	x_unlock_cpu()
#define i_unlock_cpu()	x_unlock_cpu()

/*
 *  エクセプションの優先度を取り出す
 */
Inline UB
get_exc_priority(UB excno)
{
	UW reg = (NVIC_SYS_PRI1 - 4) + (excno & ~3);
	UW tmp = sil_rew_mem((void *)reg);
	return (UB)(tmp >> (8 * (excno & 3)) + (8 - TBITW_IPRI));
}

/*
 *  割込みの優先度を取り出す
 */
Inline UB
get_int_priority(UB intno)
{
	UW reg = NVIC_PRI0 + (intno & ~3);
	UW tmp = sil_rew_mem((void *)reg);
	return (UB)(tmp >> (8 * (intno & 3)) + (8 - TBITW_IPRI));
}

/*
 *  エクセプションの優先度を設定する
 */
Inline void
set_exc_priority(UB excno, UB priority)
{
	UW reg = (NVIC_SYS_PRI1 - 4) + (excno & ~3);
	UW tmp = sil_rew_mem((void *)reg);
	tmp &= ~(0xFF << (8 * (excno & 0x03)));
	tmp |= (priority << (8 - TBITW_IPRI)) << (8 * (excno & 0x03));
	sil_wrw_mem((void *)reg, tmp);
}

/*
 *  割込みの優先度を設定する
 */
Inline void
set_int_priority(UB intno, UB priority)
{
	UW reg = NVIC_PRI0 + (intno & ~3);
	UW tmp = sil_rew_mem((void *)reg);
	tmp &= ~(0xFF << (8 * (intno & 0x03)));
	tmp |= (priority << (8 - TBITW_IPRI)) << (8 * (intno & 0x03));
	sil_wrw_mem((void *)reg, tmp);
}

/*
 *  割込み要求禁止フラグのセット
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline BOOL
disable_int(INTNO intno)
{
	UW tmp;

	/*
	 *  割込み属性が設定されていない場合
	 */
	if (intno > TMAX_INTNO || intno < -1) {
		return FALSE;
	}

	if (intno == IRQ_VECTOR_SYSTICK) {
		tmp = sil_rew_mem((void *)SYSTIC_CONTROL_STATUS);
		tmp &= ~SYSTIC_TICINT;
		sil_wrw_mem((void *)SYSTIC_CONTROL_STATUS, tmp);
	}
	else {
		sil_wrw_mem((void *)((UW *)NVIC_CLRENA0 + (intno >> 5)),
					(1 << (intno & 0x1f)));
	}

	return TRUE;
}

/*
 *  割込み要求禁止フラグの解除
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline BOOL
enable_int(INTNO intno)
{
	UW tmp;

	/*
	 *  割込み属性が設定されていない場合
	 */
	if (intno > TMAX_INTNO || intno < -1) {
		return FALSE;
	}

	if (intno == IRQ_VECTOR_SYSTICK) {
		tmp = sil_rew_mem((void *)SYSTIC_CONTROL_STATUS);
		tmp |= SYSTIC_TICINT;;
		sil_wrw_mem((void *)SYSTIC_CONTROL_STATUS, tmp);
	}
	else {
		sil_wrw_mem((void *)((UW *)NVIC_SETENA0 + (intno >> 5)),
					(1 << (intno & 0x1f)));
	}

	return TRUE;
}

/*
 *  割込み要求のクリア
 */
Inline void
clear_int(INTNO intno)
{
	sil_wrw_mem((void *)((UW *)NVIC_ICER0 + (intno >> 5)), (1 << (intno & 0x1f)));
}

/*
 *  割込み要求のチェック
 */
Inline BOOL
probe_int(INTNO intno)
{
	UW penddata = sil_rew_mem((void *)((UW *)NVIC_ISER0 + (intno >> 5)));
	return  (penddata & (1 << (intno & 0x1f))) != 0;
}

/*
 *  割込みハンドラの設定
 */
Inline void
define_inh(INHNO inhno, FP int_entry)
{
	FP *p_int_handler = (FP *)p_int_table[x_prc_index()];
	p_int_handler[inhno + NUM_EXCNO] = int_entry;
}

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号excnoのCPU例外ハンドラの出入口処理の番地をexc_entryに設
 *  定する．
 */
Inline void
define_exc(EXCNO excno, FP exc_entry)
{
	FP *p_int_handler = (FP *)p_int_table[x_prc_index()];
	/*
	 *  一部の例外は許可を行う必要がある
	 */
	enable_exc(excno);

	p_int_handler[excno] = exc_entry;
}


/*
 *  chg_ipmで有効な割込み優先度の範囲の判定
 *
 *  TMIN_INTPRIの値によらず，chg_ipmでは，((1 << TBITW_IPRI)-1) ～TIPM_ENAALL（＝0）
 *  の範囲に設定できることとする（ターゲット定義の拡張）．
 *  割込み優先度のビット幅(TBITW_IPRI)が 4 の場合は，15 ～ 0 が指定可能である．
 *   
 */
#define VALID_INTPRI_CHGIPM(intpri) \
				((intpri) < (1 << TBITW_IPRI))

/*
 * （モデル上の）割込み優先度マスクの設定
 *
 */
Inline void
set_iipm(IPM ipm)
{
	UB	iipm = INT_NVIC_PRI(ipm);

	assert(VALID_INTPRI_CHGIPM(ipm));
	set_basepri(iipm);
}

/*
 * （モデル上の）割込み優先度マスクの参照
 *
 */
Inline PRI
get_iipm(void)
{
	return get_basepri() >> (8 - TBITW_IPRI);
}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)	extern void inthdr(void)
#define INT_ENTRY(inthdr)	inthdr

/*
 *  CPU例外ハンドラの出入口処理
 */

/*
 *  CPU例外ハンドラの出入口処理の生成マクロ
 */

#define	EXCHDR_ENTRY(exchdr)	extern void exchdr(void *p_exinf)
#define	EXC_ENTRY(exchdr)	exchdr

/*
 *  CPU例外の発生した時のコンテキストの参照
 *
 *  CPU例外の発生した時のコンテキストが，タスクコンテキストの時にfalse，
 *  そうでない時にtrueを返す．
 */
Inline BOOL
exc_sense_context(void *p_excinf)
{
	UW exc_return;

	exc_return = *((UW *)p_excinf + P_EXCINF_OFFSET_EXC_RETURN);
	if ((exc_return & EXC_RETURN_PSP) == EXC_RETURN_PSP){
		return FALSE;
	}
	else {
		return TRUE;
	}
}

/*
 *  CPU例外の発生した時のIPM（ハードウェアの割込み優先度マスク，内部表
 *  現）の参照
 */
Inline UW
exc_get_iipm(void *p_excinf)
{
	return(*((UW *)p_excinf + P_EXCINF_OFFSET_IIPM));
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
Inline BOOL
exc_sense_lock(void *p_excinf)
{
	return(exc_sense_context(p_excinf)
					|| sense_lock());
}

/*
 *  最高優先順位タスクへのディスパッチ（prc_support.S）
 *
 *  dispatchは，タスクコンテキストから呼び出されたサービスコール処理か
 *  ら呼び出すべきもので，タスクコンテキスト・CPUロック状態・ディスパッ
 *  チ許可状態・（モデル上の）割込み優先度マスク全解除状態で呼び出さな
 *  ければならない．
 */
extern void	dispatch(void);

/*
 *  現在のコンテキストを捨ててディスパッチ（prc_support.S）
 *
 *  exit_and_dispatchは，ext_tskから呼び出すべきもので，タスクコンテキ
 *  スト・CPUロック状態・ディスパッチ許可状態・（モデル上の）割込み優先
 *  度マスク全解除状態で呼び出さなければならない．
 */
extern void	exit_and_dispatch(void);

/*
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

/*
 *  割込み要求ラインの属性の設定
 *
 *  Cortex-M0/M0+では，優先度0～3までの設定．
 */
extern void x_config_int(INTNO intno, BOOL active, PRI intpri);

/*
 *  エクセプション・ベクター関数
 */
extern void cpu_exc_entry(void);

/*
 *  割込み・ベクター関数
 */
extern void cpu_int_entry(void);

/*
 *  スピンロックエラーが発生した場合のハンドラ
 */
extern void spin_lock_error_handler(void *p_excinf);

/*
 *  PendSVCハンドラ
 */
extern void pendsvc_handler(void);

/*
 *  スタート関数
 */
extern void start(void);

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
