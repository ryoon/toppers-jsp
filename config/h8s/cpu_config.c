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
 *  @(#) $Id: cpu_config.c,v 1.2 2002/04/13 13:14:45 imai Exp $
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

/*
 * プロセッサ依存モジュール(H8S用)
 */

/*
 *  タスクコンテキストでの割込みマスク
 */
#ifdef SUPPORT_CHG_IPM
IPM	task_intmask;   /* IPM -> UB：符号無し8ビット */
#endif /* SUPPORT_CHG_IPM */


/*
 * 非タスクコンテキストでの割込みマスク
 */

IPM	int_intmask;

/*
 *  割込みネストカウンタ
 */

UW	intnest;

/*
 *  プロセッサ依存の初期化
 */

void
cpu_initialize()
{
/*
 *  タスクコンテキストでの割込みマスクの初期化
 */
#ifdef SUPPORT_CHG_IPM
	task_intmask = 0x00;
#endif /* SUPPORT_CHG_IPM */

	/*
	 *  割込み／CPU例外ネストカウンタの初期化
	 */
       intnest = 1;

}

/*
 *  プロセッサ依存の終了処理
 */

void
cpu_terminate()
{
}

#ifdef SUPPORT_CHG_IPM

/*
 *  割込みマスクの変更
 *
 *  chg_ipm を使って IPM を MAX_IPM （NMI スタブリモートブレーク 以外
 *  のすべての割込みを禁止）以上に変更することはできない．NMI スタブリ
 *  モートブレーク以外のすべての割込みを禁止したい場合には、loc_cpu に
 *  よりCPUロック状態にすればよい．IPM が 0 以外の時にも，タスクディス
 *  パッチは保留されない．IPM は，タスクディスパッチによって，新しく実
 *  行状態になったタスクへ引き継がれる．そのため，タスクが実行中に，別
 *  のタスクによって IPM が変更される場合がある．JSPカーネルでは，IPM
 *  の変更はタスク例外処理ルーチンによっても起こるので，これによって扱
 *  いが難しくなる状況は少ないと思われる．IPM の値によってタスクディス
 *  パッチを禁止したい場合には，dis_dsp を併用すればよい．
 */

SYSCALL ER
chg_ipm(IPM ipm)
{
	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= ipm && ipm <= MAX_IPM - 1);

	t_lock_cpu();
	task_intmask = ipm;
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  割込みマスクの参照
 */
SYSCALL ER
get_ipm(IPM *p_ipm)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_ipm = task_intmask;
	t_unlock_cpu();
	return(E_OK);
}


#endif /* SUPPORT_CHG_IPM */



/*
 * 登録されていない割り込みが発生すると呼び出される
 */

void cpu_experr(EXCSTACK *sp)
{
    syslog(LOG_EMERG, "Unexpected interrupt.");
    syslog(LOG_EMERG, "PC  = %08x SP  = %08x", sp->pc, sp - 32);
    syslog(LOG_EMERG, "ER0 = %08x ER1 = %08x ER2 = %08x ER3 = %08x",
                       sp->er0, sp->er1, sp->er2, sp->er3);
    syslog(LOG_EMERG, "ER4 = %08x ER5 = %08x ER6 = %08x",
                       sp->er4, sp->er5, sp->er6);
    while(1);
}
