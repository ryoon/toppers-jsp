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
 *		プロセッサ依存モジュール（RISC-V用）
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

#ifdef OMIT_SYSTEM_TPCB
#error "Not support STPCB type."
#endif

/*
 *  MACHINE割込みハンドラ領域のテーブル
 */
volatile EXCVE m_interrupt_handlers[TNUM_PRCID][NUM_MACHNE_INTNO];

/*
 *  TPCBへのポインタテーブル
 */
STPCB *p_tspcb_table[TNUM_PRCID];

/*
 *  MACHINEタイマ用割込みハンドラ
 */
FP (*default_timer_handler)(void);

#ifndef OMIT_DEFAULT_INT_HANDLER
/*
 *  未登録の割込みが発生した場合に呼び出される
 */
static void
default_int_handler(unsigned long mcause, void *p_excinf)
{
	unsigned long mstatus = *(((unsigned long *)p_excinf) + P_EXCINF_OFFSET_MSTATUS);
	unsigned long pc      = *(((unsigned long *)p_excinf) + P_EXCINF_OFFSET_PC);
	unsigned long excno   = mcause & MCAUSE_CAUSE;

	if((long)mcause < 0)
		excno += NUM_MACHNE_EXCNO;
	syslog(LOG_EMERG, "\nUnregistered Interrupt occurs.");
	syslog(LOG_EMERG, "Excno = 0x%02X, PC = 0x%X, mstatus = 0x%X, p_excinf = 0x%X",
		   excno, pc, mstatus, p_excinf);

	while(1);
}
#endif /* OMIT_DEFAULT_INT_HANDLER */

static void
init_intmodel(STPCB *stpcb, UINT idx)
{
	assert(stpcb != NULL);
	assert(idx < TNUM_PRCID);

	p_tspcb_table[idx] = stpcb;
	stpcb->lock_flag = TRUE;
	stpcb->inest_lvl = 0;
	stpcb->kernel_mie = KERNEL_MIE;
	stpcb->saved_trap = read_csr(mtvec);
	stpcb->stacktop  = (VP*)(STACKTOP - (PSTACKSIZE * idx));
}

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize(void)
{
	STPCB *my_stpcb = get_my_stpcb();
	UW	idx = x_prc_index();
	int  i;

	/*
	 *  CPUロックフラグ実現のための変数の初期化
	 */
	init_intmodel(my_stpcb, idx);
	disable_int_status(KERNEL_MIE);
	set_csr(mie, MIP_MSIP);

	/*
	 *  例外ベクタテーブルの初期化
	 */
#ifndef OMIT_DEFAULT_INT_HANDLER
	for(i = 0 ; i < NUM_MACHNE_INTNO ; i++){
		m_interrupt_handlers[x_prc_index()][i].exc_handler = (FP)default_int_handler;
	}
#endif
	default_timer_handler = NULL;
#ifdef DEFAULT_INTERRUPT
	/*
	 *  タイマ割込み要求をクリアする．
	 */
	machine_inh(IRQ_M_TIMER, (FP)machine_timer_handler);
#endif

	my_stpcb->saved_trap = read_csr(mtvec);
	write_csr(mtvec, &trap_entry);
	ena_intm();


#ifndef OMIT_CSR_MISA
	/*
	 *  FPU拡張の初期化
	 */
	if(read_csr(misa) & (1 << ('F' - 'A'))){
		set_csr(mstatus, MSTATUS_FS);	/* FPU設定、但し割込みはサポートしない */
		write_csr(fcsr, 0);				/* ラウンディングモード設定 */
	}
#elif defined(USE_FPU)
	set_csr(mstatus, MSTATUS_FS);	/* FPU設定、但し割込みはサポートしない */
	write_csr(fcsr, 0);				/* ラウンディングモード設定 */
#endif

	(void)(i);
}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate(void)
{
	STPCB *my_stpcb = get_my_stpcb();
	write_csr(mtvec, my_stpcb->saved_trap);
}

/*
 *  C言語レベル MACHINE割込みハンドラ
 */
UH
handle_trap(unsigned long mcause, void *p_excinf)
{
	UW      intno = mcause & MCAUSE_CAUSE;
	UW      idx   = x_prc_index();
	TPCB	*tpcb = get_pure_tpcb(idx);
	STPCB   *my_stpcb = get_my_stpcb();
	void    (*func)(unsigned long, void *);

#ifdef DEFAULT_INTERRUPT
	if(my_stpcb->inest_lvl++ == 0)
		write_csr(mtvec, &trap_nest);
#endif
	if((long)mcause < 0){
		disable_int_status(KERNEL_MIE);
		intno += NUM_MACHNE_EXCNO;
	}
	ena_intm();
	if(intno < NUM_MACHNE_INTNO){
		func = (void(*)(unsigned long, void *))m_interrupt_handlers[idx][intno].exc_handler;
		if(func != NULL)
			func(mcause, p_excinf);
		if(tpcb->locspnid != 0)
			spin_lock_error_handler(32, p_excinf);
	}
	else{
		syslog(LOG_EMERG, "Irrigal machine Exception mcause = %08X", (int)intno);
	}
	disable_int_status(KERNEL_MIE);
#ifdef DEFAULT_INTERRUPT
	if(--my_stpcb->inest_lvl == 0)
		write_csr(mtvec, &trap_entry);
#endif
	return my_stpcb->inest_lvl;
}

#ifdef DEFAULT_INTERRUPT

/*
 *  MACHINEタイマの割込みハンドリングプログラム
 */
void
machine_timer_handler(void)
{
	STPCB   *my_stpcb = get_my_stpcb();
	UB  threshold = (UB)current_ithreshold();
	my_stpcb->kernel_mie &= ~MIP_MTIP;
	set_ithreshold((UW)INTPRI_TIMER);
	set_csr(mie, my_stpcb->kernel_mie);
	if(default_timer_handler != NULL)
		default_timer_handler();
	clear_csr(mie, KERNEL_MIE);
	set_ithreshold((UW)threshold);
	my_stpcb->kernel_mie |= MIP_MTIP;
}

#endif

/*
 *  スピンロックエラーが発生した場合のハンドラ
 */
void
spin_lock_error_handler(unsigned long mcause, void *p_excinf)
{
	unsigned long mstatus = *(((unsigned long *)p_excinf) + P_EXCINF_OFFSET_MSTATUS);
	unsigned long pc      = *(((unsigned long *)p_excinf) + P_EXCINF_OFFSET_PC);
	unsigned long excno   = mcause & MCAUSE_CAUSE;

	if((long)mcause < 0)
		excno += NUM_MACHNE_EXCNO;
	syslog(LOG_EMERG, "\nSpin Lock error Interrupt occurs.");
	syslog(LOG_EMERG, "Excno = 0x%02X, PC = 0x%X, mstatus = 0x%X, p_excinf = 0x%X",
		   excno, pc, mstatus, p_excinf);

	while(1);
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
	CHECK_PAR(0 <= ipm && ipm <= (MAX_IPM - 1));

	t_lock_cpu();
	set_ithreshold(ipm);
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
	*p_ipm = current_ithreshold();
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
