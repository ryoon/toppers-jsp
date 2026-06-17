/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
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
 *		プロセッサ依存モジュール（Cortex-A用）
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

#include "gic_config.h"

#ifndef _MACRO_ONLY
#include <sil.h>
#endif

/*
 *  最大エクセプションと割込み数を定義
 */
#define NUM_INTNO	(TMAX_INTNO + 1)
#define NUM_EXCNO	7

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
	 *  割込み・例外のネスト回数
	 */
	UINT	excpt_nest_count;

	/*
	 *  CPU例外ハンドラテーブルへのポインタ
	 */
	const FP* p_exch_tbl;

	/*
	 *  割込みハンドラテーブルへのポインタ
	 */
	const FP* p_inth_tbl;

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
 *  エクセプションハンドラテーブル
 */
extern FP* const p_exch_table[];

/*
 *  割込みハンドラテーブル
 */
extern FP* const p_inth_table[];

/*
 *  ベクターテーブル（core_support.S）
 */
extern void *vector_table;

/*
 *  プロセッサINDEX（0オリジン）の取得
 */
Inline UINT
x_prc_index(void)
{
	UINT	index;

	CP15_CPUID_READ(index);
	return((index & CP15_CPUID_BIT));
}

#ifdef USE_LOCAL_MY_TPCB
/*
 * スレッドIDレジスタを用いたPCBアクセス
 */
Inline VP
get_my_local_tpcb(void)
{
	UW	my_local_tpcb;

	CPU15_PONLY_THREAD_ID_READ(my_local_tpcb);

	return (VP)my_local_tpcb;
}
#endif /* USE_LOCAL_MY_TPCB */

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
 *  ARMでは，タスクコンテキストと非タスクコンテキストの両方をスーパー
 *  バイザーモードで動作させる．そのため，CPSRの内容では判別できない．
 *  そのため，例外（割込み/CPU例外）のネスト回数をカウントする変数
 *  （excpt_nest_count）を用意し，例外の入り口でインクリメントすること
 *  で，コンテキストを判定する．
 */
Inline BOOL
sense_context(void)
{
	UW	tmp;
	UW	saved_sr;
	STPCB* my_p_stpcb;

	/*
	 *  マイグレーションされることを考慮して割込みを禁止してからチェッ
	 *  クする．
	 */
	saved_sr = current_sr();
	set_sr(saved_sr | CPSR_CPULOCK | CPSR_ALWAYS_SET);
	ARM_MEMORY_CHANGED;
	my_p_stpcb = get_sys_stpcb();
	tmp = my_p_stpcb->excpt_nest_count;
	set_sr(saved_sr);
	ARM_MEMORY_CHANGED;

	return(tmp > 0U);
}

/*
 *  CPUロック状態の参照
 */
Inline BOOL
sense_lock(void)
{
	return((current_sr() & CPSR_CPULOCK) == CPSR_CPULOCK);
}

#define t_sense_lock()    sense_lock()
#define i_sense_lock()    sense_lock()

/*
 *  CPUロック状態への移行
 */
Inline void
x_lock_cpu(void)
{
	set_sr(current_sr() | CPSR_CPULOCK | CPSR_ALWAYS_SET);
	ARM_MEMORY_CHANGED;
}

#define t_lock_cpu()   x_lock_cpu()
#define i_lock_cpu()   x_lock_cpu()

/*
 *  CPUロック状態の解除
 */
Inline void
x_unlock_cpu(void)
{
	ARM_MEMORY_CHANGED;
	set_sr((current_sr() & (~CPSR_CPULOCK)) | CPSR_ALWAYS_SET);
}

#define t_unlock_cpu() x_unlock_cpu()
#define i_unlock_cpu() x_unlock_cpu()

/*
 *  High exception vector を使うかの設定
 */
Inline void
set_high_vector(BOOL enable)
{
	UW	control;

	CP15_CONTROL_READ(control);

	if (enable) {
		control |= CP15_CONTROL_V_BIT;
	}
	else {
		control &= ~CP15_CONTROL_V_BIT;
	}

	CP15_CONTROL_WRITE(control);
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
	/*
	 *  割込み属性が設定されていない場合
	 */
	if (intno > TMAX_INTNO) {
		return FALSE;
	}

	gicd_disable_int(intno);
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
	if (intno > TMAX_INTNO) {
		return FALSE;
	}

	gicd_enable_int(intno);
	return TRUE;
}

/*
 *  割込み要求のクリア
 */
Inline void
clear_int(INTNO intno)
{
	gicd_clear_pending(intno);
}

/*
 *  割込み要求のチェック
 */
Inline BOOL
probe_int(INTNO intno)
{
	return(gicd_probe_pending(intno));
}


/*
 *  割込みハンドラの設定
 */
Inline void
define_inh(INHNO inhno, FP int_entry)
{
	FP *p_inth_handler = (FP *)p_inth_table[x_prc_index()];
	p_inth_handler[inhno] = int_entry;
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
	FP *p_exch_handler = (FP *)p_exch_table[x_prc_index()];
	p_exch_handler[excno] = exc_entry;
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
				((intpri) <= TMAX_INTPRI)

/*
 * （モデル上の）割込み優先度マスクの設定
 *
 */
Inline void
set_iipm(IPM ipm)
{
	assert(VALID_INTPRI_CHGIPM(ipm));
	gicc_set_priority(ipm);
}

/*
 * （モデル上の）割込み優先度マスクの参照
 *
 */
Inline PRI
get_iipm(void)
{
	return gicc_get_priority();
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
 *  CPU例外の発生した時のCPSRの取得
 */
Inline UW
exc_get_sr(void *p_excinf)
{
	return(((exc_frame_t *)(p_excinf))->cpsr);
}

/*
 *  CPU例外の発生した時のコンテキストの参照
 *
 *  CPU例外の発生した時のコンテキストが，タスクコンテキストの時にfalse，
 *  そうでない時にtrueを返す．
 */
Inline BOOL
exc_sense_context(void *p_excinf)
{
	return *((UW *)p_excinf + P_EXCINF_OFFSET_NEST_COUNT) != 0U;
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
					|| (exc_get_sr(p_excinf) & CPSR_CPULOCK) == CPSR_CPULOCK);
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
 *  未定義の例外が入った場合の処理
 */
extern void default_exc_handler(void *p_excinf);
extern void default_undef_handler(void *p_excinf);
extern void default_swi_handler(void *p_excinf);
extern void default_prefetchabort_handler(void *p_excinf);
extern void default_dataabort_handler(void *p_excinf);

/*
 *  スピンロックエラーが発生した場合のハンドラ
 */
extern void spin_lock_error_handler(void *p_excinf);

/*
 *  スタート関数
 */
extern void start(void);

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
