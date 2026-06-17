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
 *		プロセッサ依存モジュール（Cortex-A用）
 */

#include "jsp_kernel.h"
#include <arm_cache.h>
#include <gic_config.h>
#include "hw_timer.h"
#include "check.h"
#include "task.h"

/*
 *  STPCBへのポインタテーブル
 */
STPCB *p_tspcb_table[TNUM_PRCID];

/*
 *  割込み・エクセプションハンドラテーブル
 */
static FP exch_handler_table[TNUM_PRCID][NUM_EXCNO];
static FP inth_handler_table[TNUM_PRCID][NUM_VECTORS];
FP* const p_exch_table[TNUM_PRCID] = {
	exch_handler_table[0],
#if TNUM_PRCID >= 2
	exch_handler_table[1]
#endif
};

FP* const p_inth_table[TNUM_PRCID] = {
	inth_handler_table[0],
#if TNUM_PRCID >= 2
	inth_handler_table[1]
#endif
};

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 *  未登録の割込みが発生した場合に呼び出される
 */
static void
default_int_handler(void *p_excinf)
{
	UW	intno = *(((UW *)p_excinf) + P_INTINF_OFFSET_INTNO);
	UW	pc = ((exc_frame_t *)(p_excinf))->pc;
	UW	cpsr = ((exc_frame_t *)(p_excinf))->cpsr;

	syslog(LOG_EMERG, "\nUnregistered Exception occurs.");
	syslog(LOG_EMERG, "Intno = 0x%08X, PC = 0x%08X, CPSR = 0x%08X, iipm p_excinf = 0x%08X",
		   intno, pc, cpsr, p_excinf);

	while(1);
}
#endif	/* OMIT_DEFAULT_INT_HANDLER */

#ifndef OMIT_DEFAULT_EXC_HANDLER
/*
 * 未定義の例外が入った場合の処理
 */

static void
dump_excframe(exc_frame_t *p_exc_frame)
{
	ID prc_id = ID_PRC(x_prc_index());

	syslog_3(LOG_EMERG, "Processor %d : nest_count = %d, ipm = %d.",
			 prc_id, p_exc_frame->nest_count, p_exc_frame->ipm);
	syslog_4(LOG_EMERG, "Processor %d : r0 = 0x%08x, r1 = 0x%08x, r2 = 0x%08x",
			 prc_id, p_exc_frame->r0, p_exc_frame->r1, p_exc_frame->r2);
	syslog_3(LOG_EMERG, "Processor %d : r3 = 0x%08x, r12 = 0x%08x",
			 prc_id, p_exc_frame->r3, p_exc_frame->r12);
	syslog_4(LOG_EMERG, "Processor %d : lr = 0x%08x, pc = 0x%08x, cpsr = 0x%08x",
			 prc_id, p_exc_frame->lr, p_exc_frame->pc, p_exc_frame->cpsr);
}

void
default_exc_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "\nProcessor %d : Unregistered Exception occurs.", prc_id);
	dump_excframe((exc_frame_t*)p_excinf);
	while(1);
}

void
default_undef_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "\nProcessor %d : Undef Exception occurs.", prc_id);
	dump_excframe((exc_frame_t*)p_excinf);
	while(1);
}

void
default_swi_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "\nProcessor %d : SWI Exception occurs.", prc_id);
	dump_excframe((exc_frame_t*)p_excinf);
	while(1);
}

void
default_prefetchabort_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "\nProcessor %d : Prefetch Abort Exception occurs.", prc_id);
	dump_excframe((exc_frame_t*)p_excinf);
	while(1);
}

void
default_dataabort_handler(void *p_excinf){
	ID prc_id = ID_PRC(x_prc_index());

	syslog_1(LOG_EMERG, "\nProcessor %d : Data Abort Exception occurs.", prc_id);
	dump_excframe((exc_frame_t*)p_excinf);
	while(1);
}

#endif	/* OMIT_DEFAULT_EXC_HANDLER */

static void
init_intmodel(STPCB *stpcb, UINT idx)
{
	assert(stpcb != NULL);
	assert(idx < TNUM_PRCID);

	p_tspcb_table[idx] = stpcb;
	stpcb->excpt_nest_count = 0;
	stpcb->p_exch_tbl = p_exch_table[idx];
	stpcb->p_inth_tbl = p_inth_table[idx];
	stpcb->stacktop  = (VP*)(STACKTOP - (PSTACKSIZE * idx));
}

/*
 *  Disable L4WD0
 */
static void
l4wd0_disable(void)
{
	/* reset watchdog  */ 
	sil_wrw_mem((VP)RSTMGR_PERMODRST,
				sil_rew_mem((VP)RSTMGR_PERMODRST) | RSTMGR_PERMODRST_L4WD0);
	/* clear reset watchdog  */
	sil_wrw_mem((VP)RSTMGR_PERMODRST,
				sil_rew_mem((VP)RSTMGR_PERMODRST) & ~RSTMGR_PERMODRST_L4WD0);
}

/*
 *  Enable Bridge
 */
static BOOL
enable_bridges(void)
{
	/* reset bridges  */ 
	sil_wrw_mem((VP)RSTMGR_BRGMODRST,
				sil_rew_mem((VP)RSTMGR_BRGMODRST) |
				(RSTMGR_BRGMODRST_H2F|RSTMGR_BRGMODRST_LWH2F|RSTMGR_BRGMODRST_F2H));

	/* ToDo error check */

	/* clear reset bridges  */ 
	sil_wrw_mem((VP)RSTMGR_BRGMODRST,
				sil_rew_mem((VP)RSTMGR_BRGMODRST) &
				~(RSTMGR_BRGMODRST_H2F|RSTMGR_BRGMODRST_LWH2F|RSTMGR_BRGMODRST_F2H));

	/* Map hps2fpga and lwhp2fpga */
	sil_wrw_mem((VP)L3REGS_REMAP,
				sil_rew_mem((VP)L3REGS_REMAP) | (L3REGS_REMAP_HPS2FPGA|L3REGS_REMAP_LWHP2FPGA));

	return TRUE;
}

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize(void)
{
	UINT	index = x_prc_index();
#ifndef OMIT_DEFAULT_INT_HANDLER
	UINT	i;
#endif

	/* Disbale high vector */
	set_high_vector(FALSE);

	if (index == 0) {
		/* Disable Watchdog(L4WD0) */
		l4wd0_disable();
		/* Enable FPGA bridge */
		enable_bridges();
	}

#ifdef USE_LOCAL_MY_TPCB
	/*
	 *  Thread ID レジスタへのPCBへのポインタの設定
	 */
	CPU15_PONLY_THREAD_ID_WRITE((UW)p_pcb_table[index]);
#endif /* USE_LOCAL_MY_TPCB */

	/*
	 * キャッシュを無効に
	 */
	cache_disable();

	/*
	 *  ブランチプリディクタを全無効化
	 */
	bpi_invalidate_all();

	/*
	 * MMUを有効に
	 */
	mmu_init();

	/*
	 * SMPモードへ
	 */
	smp_mode_enable();

	/*
	 * キャッシュを有効に
	 */
	cache_enable();

	/*
	 *  ブランチプリディクタを有効化
	 */
	btac_enable();

	/*
	 *  GICの各コアのSGIとPPIの設定
	 */
	gicd_sgi_ppi_init();

	/*
	 *  GICのCPUインタフェース初期化
	 */
	gicc_initialize();

	/*
	 *  割込み処理モデル関連の初期化
	 */
	init_intmodel(get_my_stpcb(), index);

	/*
	 *  例外ベクタテーブルの初期化
	 */
#ifndef OMIT_DEFAULT_EXC_HANDLER
	exch_handler_table[index][SVC_Number] = (FP)default_exc_handler;
	exch_handler_table[index][UND_Number] = (FP)default_undef_handler;
	exch_handler_table[index][SWI_Number] = (FP)default_swi_handler;
	exch_handler_table[index][PRFA_Number] = (FP)default_prefetchabort_handler;
	exch_handler_table[index][DATAA_Number] = (FP)default_dataabort_handler;
	exch_handler_table[index][IRQ_Number] = (FP)default_exc_handler;
	exch_handler_table[index][FIQ_Number] = (FP)default_exc_handler;
#endif	/* OMIT_DEFAULT_EXC_HANDLER */

	/*
	 *  割込みベクタテーブルの初期化
	 */
#ifndef OMIT_DEFAULT_INT_HANDLER
	for(i = 0 ; i < NUM_VECTORS ; i++){
		inth_handler_table[index][i] = (FP)default_int_handler;
	}
#endif	/* OMIT_DEFAULT_INT_HANDLER */

	CP15_SET_VBAR((UW) &vector_table);

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
	extern void    software_term_hook(void);
	void (*volatile fp)(void) = software_term_hook;
	UINT index = x_prc_index();

	/*
	 *  software_term_hookへのポインタを，一旦volatile指定のあるfpに代
	 *  入してから使うのは，0との比較が最適化で削除されないようにするた
	 *  めである．
	 */
	if (fp != 0) {
		(*fp)();
	}

	/*
	 *  DICのCPUインタフェースを停止
	 */
	gicc_terminate();

	/*
	 *  DICのDISインタフェースを停止
	 */
	if (index == 0) {
		gicd_terminate();
	}

	/*
	 *  システムタイマの割込み禁止
	 */
	x_config_int(INTNO_TIMER, FALSE, INTPRI_TIMER);
}

#ifndef TOPPERS_SYSTIM_GLOBAL

/*
 *  カーネルスタート時、システム設定関数
 *  SYSTIC_LOCALの場合、サブプロセッサタイマの初期化を行う
 */
void
system_start(ID prcid)
{
	if (prcid != 0) {
		hw_timer_initialize();
	}
}

#endif	/* TOPPERS_SYSTIM_GLOBAL */

/*
 *  スピンロックエラーが発生した場合のハンドラ
 */
void
spin_lock_error_handler(void *p_excinf)
{
	UW	intno = *(((UW *)p_excinf) + P_INTINF_OFFSET_INTNO);
	UW	pc = ((exc_frame_t *)(p_excinf))->pc;
	UW	cpsr = ((exc_frame_t *)(p_excinf))->cpsr;

	syslog(LOG_EMERG, "\nSpin Lock error Interrupt occurs.");
	syslog(LOG_EMERG, "Intno = 0x%08X, PC = 0x%08X, CPSR = 0x%08X, iipm p_excinf = 0x%08X",
		   intno, pc, cpsr, p_excinf);

	while(1);
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
	 *  割込みを禁止
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	disable_int(intno);

	/*
	 *  割込みをコンフィギュレーション
	 */
	gicd_config(intno, GICD_ICFGRn_LEVEL);

	/*
	 *  割込み優先度とターゲットプロセッサを設定
	 */
	gicd_set_priority(intno, intpri);

	/*
	 * ターゲットCPUの設定（グローバル割込みのみ）
	 */
	if (intno >= GIC_INTNO_SPI0) {
		gicd_set_target(intno, (1 << x_prc_index()));
	}

	/*
	 * 割込みを許可
	 */
	if (active) {
		enable_int(intno);
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
	CHECK_PAR(TMIN_INTPRI <= ipm && ipm <= TMAX_INTPRI);

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
