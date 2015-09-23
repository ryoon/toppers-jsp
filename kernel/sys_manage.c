/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: sys_manage.c,v 1.4 2002/03/26 08:19:38 hiro Exp $
 */

/*
 *	システム状態管理機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

/*
 *  タスクの優先順位の回転
 */
SYSCALL ER
rot_rdq(PRI tskpri)
{
	UINT	pri;

	CHECK_TSKCTX_UNL();
	CHECK_TPRI_SELF(tskpri);

	t_lock_cpu();
	pri = (tskpri == TPRI_SELF) ? runtsk->priority : INT_PRIORITY(tskpri);
	if (rotate_ready_queue(pri)) {
		dispatch();
	}
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  タスクの優先順位の回転（非タスクコンテキスト用）
 */
SYSCALL ER
irot_rdq(PRI tskpri)
{
	CHECK_INTCTX_UNL();
	CHECK_TPRI(tskpri);

	i_lock_cpu();
	if (rotate_ready_queue(INT_PRIORITY(tskpri))) {
		reqflg = TRUE;
	}
	i_unlock_cpu();
	return(E_OK);
}

/*
 *  実行状態のタスクIDの参照
 */
SYSCALL ER
get_tid(ID *p_tskid)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_tskid = TSKID(runtsk);
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  実行状態のタスクIDの参照（非タスクコンテキスト用）
 */
SYSCALL ER
iget_tid(ID *p_tskid)
{
	CHECK_INTCTX_UNL();

	i_lock_cpu();
	*p_tskid = (runtsk == NULL) ? TSK_NONE : TSKID(runtsk);
	i_unlock_cpu();
	return(E_OK);
}

/*
 *  CPUロック状態への移行
 */
SYSCALL ER
loc_cpu(void)
{
	CHECK_TSKCTX();

	if (!(t_sense_lock())) {
		t_lock_cpu();
	}
	return(E_OK);
}

/*
 *  CPUロック状態への移行（非タスクコンテキスト用）
 */
SYSCALL ER
iloc_cpu(void)
{
	CHECK_INTCTX();

	if (!(i_sense_lock())) {
		i_lock_cpu();
	}
	return(E_OK);
}

/*
 *  CPUロック状態の解除
 *
 *  CPUロック中は，ディスパッチが必要となるサービスコールを呼び出すこ
 *  とはできないため，CPUロック状態の解除時にディスパッチャを起動する
 *  必要はない．
 */
SYSCALL ER
unl_cpu(void)
{
	CHECK_TSKCTX();

	if (t_sense_lock()) {
		t_unlock_cpu();
	}
	return(E_OK);
}

/*
 *  CPUロック状態の解除（非タスクコンテキスト用）
 *
 *  CPUロック中は，ディスパッチが必要となるサービスコールを呼び出すこ
 *  とはできないため，CPUロック状態の解除時にディスパッチャの起動を要
 *  求する必要はない．
 */
SYSCALL ER
iunl_cpu(void)
{
	CHECK_INTCTX();

	if (i_sense_lock()) {
		i_unlock_cpu();
	}
	return(E_OK);
}

/*
 *  ディスパッチの禁止
 */
SYSCALL ER
dis_dsp(void)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	enadsp = FALSE;
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  ディスパッチの許可
 */
SYSCALL ER
ena_dsp(void)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	enadsp = TRUE;
	if (runtsk != schedtsk) {
		dispatch();
	}
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  コンテキストの参照
 */
SYSCALL BOOL
sns_ctx()
{
	return(sense_context() ? TRUE : FALSE);
}

/*
 *  CPUロック状態の参照
 */
SYSCALL BOOL
sns_loc()
{
	return(sense_lock() ? TRUE : FALSE);
}

/*
 *  ディスパッチ禁止状態の参照
 */
SYSCALL BOOL
sns_dsp()
{
	return(!(enadsp) ? TRUE : FALSE);
}

/*
 *  ディスパッチ保留状態の参照
 */
SYSCALL BOOL
sns_dpn()
{
	return((sense_context() || sense_lock() || !(enadsp)) ? TRUE : FALSE);
}
