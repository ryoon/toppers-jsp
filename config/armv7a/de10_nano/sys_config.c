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
 * 
 *  @(#) $Id: sys_config.c 2246 2026-01-16 16:33:12Z roi $
 */

/*
 *		ターゲット依存モジュール（cyclone V用）
 */

#include "jsp_kernel.h"
#include <task.h>
#include <arm_cache.h>
#include <gic_config.h>
#include <pl310.h>
#include <hw_timer.h>		/* システム・タイマー関係 */
#include <hw_interpro.h>	/* プロセッサ間割込み関係 */
#include <hw_serial.h>		/* デバックシリアルコントローラ関係 */

/*
 *  L2キャッシュの初期化
 */
static void l2cache_init(void)
{
	UW	aux_ctrl = 0;

	aux_ctrl |= (1 << L2X0_AUX_CTRL_SHARE_OVERRIDE_SHIFT) |
	  (1 << L2X0_AUX_CTRL_DATA_PREFETCH_SHIFT) |
		(1 << L2X0_AUX_CTRL_INSTR_PREFETCH_SHIFT);

	pl310_init(aux_ctrl, ~0x0U);
}

/*
 *  ターゲット依存の初期化
 */
void
sys_initialize(void)
{
	volatile static UW pass = 0;
	UINT	idx = x_prc_index();

	/* Enable L2 cache by myself in case of without safeg or with safeg Non-Secure */
	if (idx == 0) {
		UINT	i, count;

		while(1) {
			for (i = 1, count = 0; i < TNUM_PRCID; i++) {
				if ((pass & (1<<i)) != 0) {
					count++;
				}
			}
			if (count == (TNUM_PRCID - 1)) {
				break;
			}
		}
		l2cache_init();
	}
	pass |= 1 << idx;

	/*
	 *  バナー出力用のシリアル初期化
	 */
	sio_init();
}

/*
 *  ターゲット依存の終了処理
 */
void
sys_exit(void)
{
	/*
	 *  開発環境依存の終了処理
	 */
	while(1);
}

/*
 * MMU initialization (called from the chip code)
 *
 * We create a simpe 1-to-1 (physical address = virtual address) memory
 * mapping and enable caching for the DDR memory.
 */
void
sys_mmu_init(void)
{
	MEMORY_ATTRIBUTE m_attribute;

	/*
	 * Basic initialization of the whole virtual memory space to
	 * non-cachable, non-bufferable, strongly-ordered.
	 */
	m_attribute.pa   = (VP_INT)0x00000000;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = 0x80000000; /* 2GB */
	m_attribute.ns   = 0;          /* 0=Secure */
	m_attribute.s    = 1;          /* 1=Shared */
	m_attribute.ap   = 3;          /* Full access */
	m_attribute.tex  = 0;          /* Strongly Ordered */
	m_attribute.c    = 0;
	m_attribute.b    = 0;

	mmu_map_memory (&m_attribute);

	m_attribute.pa   = (VP_INT)0x80000000; /* 2GB */
	m_attribute.va   = m_attribute.pa;

	mmu_map_memory (&m_attribute);

	/*
	 *  Enable caching for the DDR
	 */
	m_attribute.pa   = (VP_INT)DDR_ADDR;
	m_attribute.va   = m_attribute.pa;
	m_attribute.size = DDR_SIZE;
	m_attribute.ns   = 0;      /* NonSecure    */
	m_attribute.s    = 1;      /* 1=Shared */
	m_attribute.ap   = 3;      /* Full access */
	/* Outer and Inner Write-Back, Write-Allocate */
	m_attribute.tex  = 1;             
	m_attribute.c    = 1;             
	m_attribute.b    = 1;             

	mmu_map_memory(&m_attribute);

}

#if TNUM_PRCID >= 2

static void
copy_prc2_boot(void){
	int cnt;
	volatile UB	*dst_addr;

	dst_addr = (UB *)0x00000000; /* DDR */
	for (cnt = 0; cnt < (&prc2_boot_end - &prc2_boot); cnt++) {
		*(dst_addr + cnt) = *(volatile UB *)(&prc2_boot + cnt);
	}

	__asm__ volatile("dsb");
	__asm__ volatile("isb");
}

/*
 *  Master processor initialization before str_ker().
 *  (E.g.: initialize the interrupt controller or the SCU.)
 */
void
mprc_initialize(void)
{
	/*
	 *  GICディストリビュータの初期化
	 */
	gicd_initialize();

	/*
	 *  SCUの初期化
	 */
	scu_init();

	/* map DDR to 0x00000000 - 0xffff0000 (also for uboot)*/
	sil_wrw_mem((VP)MPUL2_ADRFILTER, 0x01);

	/* Core2 boot code to DDR */
	copy_prc2_boot();
	/* Set Core2 boot address */
	sil_wrw_mem((VP)SYSMGR_CPU1STARTADDR, (UW)start);
	/* Wake up processor 2 */
	sil_wrw_mem((VP)RSTMGR_MPUMODRST,
				sil_rew_mem((VP)RSTMGR_MPUMODRST) & ~RSTMGR_MPUMODRST_CPU1);

}

/*
 *  ターゲットプロセッサへの割込み要求（タスク・非タスクコンテキスト共用）
 */
void
x_ipi_raise(ID prcid, UH event)
{
	UB	intno;

	switch (event) {
	case IPI_EVENT_EXTKERNEL:
		intno = 2;
		break;
	case IPI_EVENT_TEXCEPT:
		intno = 0;
		break;
	default:
		intno = 1;
		break;
	}
	gicd_set_sgi((1<<(prcid-1)), intno);
}

#endif /* TNUM_PRCID >= 2 */

/*
 *  システムログの低レベル出力のための文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		sio_snd_chr_pol('\r');
	}
	sio_snd_chr_pol(c);
}

