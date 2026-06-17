/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2013 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
 *
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 *
 *  @(#) $Id: mpcore.c 1218 2017-04-25 07:05:23Z ertl-honda $
 */

#include "jsp_kernel.h"
#include "arm_cache.h"
#include "mpcore.h"

/*
 * SCU関連の関数
 */

/*
 *  CP15のAUXILIARYレジスタのSAビットとFWビットをセットしてSMPモードにする
 */
Inline void
enable_sa(void)
{
	UW	bits;

	CP15_AUXILIARY_READ(bits);
	bits |= CP15_AUXILIARY_SA_BIT;
	bits |= CP15_AUXILIARY_FW_BIT;
	CP15_AUXILIARY_WRITE(bits);
}

Inline void
scu_enable(void)
{
	UW scu_ctrl;

	/* SCUの tag RAMS を無効に */
	sil_wrw_mem((void*)MPCORE_SCU_IALL, MPCORE_SCU_IALL_WAYS);

	/* SCUを有効に */
	scu_ctrl  = sil_rew_mem((void *)MPCORE_SCU_CTRL);
	scu_ctrl |= MPCORE_SCU_CTRL_EN;
	sil_wrw_mem((void *)MPCORE_SCU_CTRL, scu_ctrl);
}

/*
 *  SCUの初期化処理（マスタプロセッサのみ実行）
 */
void
scu_init(void)
{
	/* SCU を有効に */
	scu_enable();

	/* 全てのコアのモードをNORMALとする */
	sil_wrw_mem((void *)MPCORE_SCU_CPUST, 0x00U);

}

/*
 *  SMPモードに設定する
 */
void
smp_mode_enable(void)
{
	UW	sr;

	/* 全割込み禁止 */
	sr = current_sr();
	set_sr(sr|CPSR_IRQ_BIT|CPSR_FIQ_BIT);

	/* キャシュを無効 */
	dcache_clean_and_invalidate();
	icache_invalidate();

	/* Data Synchronization Barrier */
	data_sync_barrier();

	/* TLBの初期化 */
	invalidate_unfied_tlb();

	/* CP15のSMP/nAMP bit をセットする */
	enable_sa();

	/* 割込み禁止状態を元に戻す */
	set_sr(sr);
}
