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
 *		プロセッサ依存モジュール（Cortex-M3/M4/M7用）
 */

#include "jsp_kernel.h"
#include "hw_timer.h"
#include "check.h"
#include "task.h"

/*
 *  割込みハンドラテーブル
 */
FP int_handler_table[NUM_INTNO + NUM_EXCNO];


#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 *  未登録の割込みが発生した場合に呼び出される
 */
static void
default_int_handler(void *p_excinf)
{
	UW basepri = *(((UW *)p_excinf) + P_EXCINF_OFFSET_IIPM);
	UW pc      = *(((UW *)p_excinf) + P_EXCINF_OFFSET_PC);
	UW xpsr    = *(((UW *)p_excinf) + P_EXCINF_OFFSET_XPSR);
	UW excno   = get_ipsr() & IPSR_ISR_NUMBER;

	syslog(LOG_EMERG, "\nUnregistered Exception occurs.");
	syslog(LOG_EMERG, "Excno = 0x%08X, PC = 0x%08X, XPSR = 0x%08X, iipm = 0x%08X, p_excinf = 0x%08X",
		   excno, pc, xpsr, basepri, p_excinf);	

	while(1);
}
#endif /* OMIT_DEFAULT_INT_HANDLER */

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize(void)
{
	UINT	i;

	/*
	 *  ベクタテーブルを設定
	 */
	sil_wrw_mem((void*)NVIC_VECTTBL, (UW)vector_table);

	/*
	 *  各例外の優先度を設定
	 *  CPUロック状態でも発生するように，BASEPRIレジスタでマスクでき
	 *  ない'0'とする．
	 */
	set_exc_priority(EXCNO_HARD, 0);
	set_exc_priority(EXCNO_MPU, 0);
	set_exc_priority(EXCNO_BUS, 0);
	set_exc_priority(EXCNO_USAGE, 0);
	set_exc_priority(EXCNO_SVCALL, 0);
	set_exc_priority(EXCNO_DEBUG, 0);
	set_exc_priority(EXCNO_PENDSV, 0);

	/*
	 *  例外ベクタテーブルの初期化
	 */
#ifndef OMIT_DEFAULT_INT_HANDLER
	for(i = 0 ; i < (NUM_INTNO + NUM_EXCNO) ; i++){
		int_handler_table[i] = (FP)default_int_handler;
	}
#endif
	/*
	 *  システムタイマの割込み設定
	 */
	x_config_int(INTNO_TIMER, TRUE, INTPRI_TIMER);
}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate(void)
{
	/*
	 *  システムタイマの割込み禁止
	 */
	x_config_int(INTNO_TIMER, FALSE, INTPRI_TIMER);
}

/*
 *  例外の許可
 *
 *  Memory Management, Bus Fault, Usage Fault は禁止・許可が可能
 */
void
enable_exc(EXCNO excno)
{
	UW	tmp;

	switch (excno) {
	case EXCNO_MPU:
		tmp = sil_rew_mem((void *)NVIC_SYS_HND_CTRL);
		tmp |= NVIC_SYS_HND_CTRL_MEM;
		sil_wrw_mem((void *)NVIC_SYS_HND_CTRL, tmp);
		break;
	case EXCNO_BUS:
		tmp = sil_rew_mem((void *)NVIC_SYS_HND_CTRL);
		tmp |= NVIC_SYS_HND_CTRL_BUS;
		sil_wrw_mem((void *)NVIC_SYS_HND_CTRL, tmp);
		break;
	case EXCNO_USAGE:
		tmp = sil_rew_mem((void *)NVIC_SYS_HND_CTRL);
		tmp |= NVIC_SYS_HND_CTRL_USAGE;
		sil_wrw_mem((void *)NVIC_SYS_HND_CTRL, tmp);
		break;
	default:
		break;
	}
}

/*
 *  例外の禁止
 */
void
disable_exc(EXCNO excno)
{
	UW	tmp;

	switch (excno) {
	case EXCNO_MPU:
		tmp = sil_rew_mem((void *)NVIC_SYS_HND_CTRL);
		tmp &= ~NVIC_SYS_HND_CTRL_MEM;
		sil_wrw_mem((void *)NVIC_SYS_HND_CTRL, tmp);
		break;
	case EXCNO_BUS:
		tmp = sil_rew_mem((void *)NVIC_SYS_HND_CTRL);
		tmp &= ~NVIC_SYS_HND_CTRL_BUS;
		sil_wrw_mem((void *)NVIC_SYS_HND_CTRL, tmp);
		break;
	case EXCNO_USAGE:
		tmp = sil_rew_mem((void *)NVIC_SYS_HND_CTRL);
		tmp &= ~NVIC_SYS_HND_CTRL_USAGE;
		sil_wrw_mem((void *)NVIC_SYS_HND_CTRL, tmp);
		break;
	default:
		break;
	}
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  JSPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．cfg_intサービスコールを設ける場合には，エラーを返すよう
 *  にすべきであろう．
 */
void
x_config_int(INTNO intno, BOOL active, PRI intpri)
{
	assert(intno <= TMAX_INTNO);
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込みのマスク
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	(void) disable_int(intno);

	/*
	 *  割込み優先度の設定
	 */
	if (intno < 0) {
		set_exc_priority(intno+NUM_EXCNO, intpri);
	}
	else {
		set_int_priority(intno, intpri);
	}

	/*
	 *  割込みのマスク解除（必要な場合）
 	 */
	if (active) {
		(void) enable_int(intno);
	}
}


/*
 *  割込み発生許可処理
 */
ER
ena_int(INTNO intno)
{
	ER	ercd = E_OK;

	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= intno && intno <= TMAX_INTNO);

	if (!enable_int(intno)) {	/* 割り込みを許可する */
		ercd = E_SYS;
	}
    exit:
	return(ercd);
}

/*
 *  割込み発生禁止処理
 */
ER
dis_int(INTNO intno)
{
	ER	ercd = E_OK;

	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= intno && intno <= TMAX_INTNO);

	if (!disable_int(intno)) {	/* 割り込みを禁止する */
		ercd = E_SYS;
	}
    exit:
	return(ercd);
}

/*
 *  割込み発生要因クリア
 */
ER
clr_int(INTNO intno)
{
	ER	ercd = E_OK;

	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= intno && intno <= TMAX_INTNO);

	clear_int(intno);			/* 割り込み要因をクリアする */

    exit:
	return(ercd);
}

#ifdef SUPPORT_CHG_IPM

/*
 *  割込みマスクの変更
 *
 *  chg_ipm を使って IPM を0xf（NMI 以外のすべての割込みを禁止）に変更
 *  することはできない．NMI 以外のすべての割込みを禁止したい場合には，
 *  loc_cpu によりCPUロック状態にすればよい．
 *  IPM が 0 以外の時にも，タスクディスパッチは保留されない．IPM は，
 *  タスクディスパッチによって，新しく実行状態になったタスクへ引き継が
 *  れる．そのため，タスクが実行中に，別のタスクによって IPM が変更さ
 *  れる場合がある．JSPカーネルでは，IPM の変更はタスク例外処理ルーチ
 *  ンによっても起こるが，これによって扱いが難しくなる状況は少ないと
 *  思われる．IPM の値によってタスクディスパッチを禁止したい場合には，
 *  dis_dsp を併用すればよい．
 */
SYSCALL ER
chg_ipm(IPM ipm)
{
	ER	ercd;

	LOG_CHG_IPM_ENTER(ipm);
	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= ipm && ipm <= MAX_IPM);

	t_lock_cpu();
	set_iipm(ipm);
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	LOG_CHG_IPM_LEAVE(ercd);
	return(ercd);
}

/*
 *  割込みマスクの参照
 */
SYSCALL ER
get_ipm(IPM *p_ipm)
{
	ER	ercd;

	LOG_GET_IPM_ENTER(p_ipm);
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_ipm = get_iipm();
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	LOG_GET_IPM_LEAVE(ercd, *p_ipm);
	return(ercd);
}

#endif /* SUPPORT_CHG_IPM */

/*
 *  割込み設定
 */
#ifndef OMIT_VXCFG_INT

SYSCALL ER
vxcfg_int(INTNO intno, BOOL intreq, PRI intpri)
{
	ER	ercd;
	BOOL	locked;

	LOG_VXCFG_INT_ENTER(intno, intreq, intpri);
	CHECK_INTNO(intno);

	locked = sense_lock();
	if (!locked) {
		x_lock_cpu();
	}
	x_config_int(intno, intreq, intpri);
	if (!locked) {
		x_unlock_cpu();
	}
	ercd = E_OK;

	exit:
	LOG_VXCFG_INT_LEAVE(ercd);
	return(ercd);
}

#endif /* OMIT_VXCFG_INT */
