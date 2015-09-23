/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: cpu_config.h,v 1.7 2002/04/10 11:20:09 takayuki Exp $
 */


/*
 *	プロセッサ依存モジュール（Windows用）
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

/*
 *  Windows-HAL独自のインクルードファイル
 */


/*
 *  chg_ims/ref_ims をサポートするかどうかの定義
 */
#define	SUPPORT_CHG_IMS

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

#include "cpu_defs.h"
#include "hal_msg.h"
#include "vwindows.h"

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	HANDLE	ThreadHandle;
	DWORD	ThreadID;
} CTXB;


extern CRITICAL_SECTION CPULock;

#define CPU_STAT_LOCK	0x01
#define CPU_STAT_EXC	0x02
extern volatile char CPUStatus;

/*
 *   CPU例外ハンドラ
 */

/*#define EXCHNO_ZDIV  EXCEPTION_INT_DIVIDE_BY_ZERO */

/*
 *  システム状態参照
 */

Inline BOOL
sense_context()
{
	return (CurrentInterruptLevel != 0) ? TRUE : FALSE;
}

Inline BOOL
sense_lock()
{
	return (CPUStatus & (CPU_STAT_LOCK|CPU_STAT_EXC)) != 0 ? TRUE : FALSE;
}

#define t_sense_lock	sense_lock
#define i_sense_lock	sense_lock

/*
 *  CPUロックとその解除（タスクコンテキスト用）
 *
 *  task_intmask は，chg_ipm をサポートするための変数．chg_ipm をサポー
 *  トしない場合には，task_intmask が常に 0 になっていると考えればよい．
 */

#include "debugout.h"
#include "device.h"

Inline void
i_lock_cpu()
{
	dis_int(0);
}

Inline void
i_unlock_cpu()
{
	ena_int(0);
}

#define t_lock_cpu	i_lock_cpu
#define t_unlock_cpu	i_unlock_cpu

/*
 *  タスクディスパッチャ
 */

/*
 *  最高優先順位タスクへのディスパッチ
 *
 *  dispatch は，タスクコンテキストから呼び出されたサービスコール処理
 *  内で，CPUロック状態で呼び出さなければならない．
 */
extern void	dispatch();


/*
 *  現在のコンテキストを捨ててディスパッチ
 *
 *  exit_and_dispatch は，CPUロック状態で呼び出さなければならない．
 */

extern void    exit_and_dispatch(void);

/*
 *  割込みハンドラ／CPU例外ハンドラの設定
 */


/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */

Inline void
define_inh(INHNO inhno, FP inthdr)
{
	def_int((unsigned int)inhno,(void *)inthdr);
}

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号 excno のCPU例外ハンドラの起動番地を exchdr に設定する．
 */
Inline void
define_exc(EXCNO excno, FP exchdr)
{
	def_exc( (DWORD)excno, (void *)exchdr);
}

/*
 *  割込みハンドラ／CPU例外ハンドラの出入口処理
 */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 *  全ての割り込みハンドラはディスパッチャを起動するSIGUSR1をマスクして
 *  動作する。
 *  reqflg をチェックする前に割込みを禁止しないと，reqflg をチェック後
 *  に起動された割込みハンドラ内でディスパッチが要求された場合に，ディ
 *  スパッチされない．
 */

	/* Windows-HALにおいてこの処理は割込みエミュレータに一任している */

#define INT_ENTRY(hdr) hdr
#define INTHDR_ENTRY(inthdr) extern void inthdr(void)

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

#define EXC_ENTRY(hdr) hdr
#define EXCHDR_ENTRY(inthdr) extern void inthdr(void)

/*
 *  CPU例外の発生した時のシステム状態の参照
 */

/*
 *  CPU例外の発生した時のディスパッチ
 */
Inline BOOL
exc_sense_context(VP p_excinf)
{
	return (CPUStatus & CPU_STAT_EXC) != 0 ? TRUE : FALSE;
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
#define exc_sense_lock sense_lock


/*
 *  プロセッサ依存の初期化
 */
extern void	cpu_initialize(void);

/*
 *  プロセッサ依存の終了時処理
 */
extern void	cpu_terminate(void);

/* Windows Dedicated Routines */
extern LONG WINAPI HALExceptionHandler( EXCEPTION_POINTERS * exc );

#endif /* _MACRO_ONLY */
#endif /* _CPU_CONFIG_H_ */
