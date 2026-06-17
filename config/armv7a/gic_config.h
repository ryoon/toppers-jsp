/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2006-2017 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2016-2024 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)?(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  $Id: gic_config.h 1260 2026-03-09 16:05:41Z roi $
 */

/*
 *		カーネルの割込みコントローラ依存部（GIC用）
 *
 *  このヘッダファイルは，target_config.h（または，そこからインク
 *  ルードされるファイル）のみからインクルードされる．他のファイルから
 *  直接インクルードしてはならない．
 */

#ifndef TOPPERS_GIC_KERNEL_H
#define TOPPERS_GIC_KERNEL_H

#include <sil.h>
#include "cpu_insn.h"

/*
 *  Number of interrupts supported by the GICv1.0 in this board. Note
 *  that these values could be obtained dynamically from the corresponding
 *  GIC register.
 */
#define GIC_TNUM_INTNO		(TMAX_INTNO+1)

/*
 *  割込み優先度の操作
 *
 *  割込み優先度の内部表現は，uint_t型で表し，0が最高優先度で，値が大き
 *  いほど優先度が下がるものとする．GICのレジスタ構成と整合させるために，
 *  優先度の段数が256段階の時にあわせて表す．
 */
#ifndef GIC_PRI_LEVEL
#define GIC_PRI_LEVEL		(TMAX_INTPRI - TMIN_INTPRI + 1)
#endif /* GIC_PRI_LEVEL */


#if GIC_PRI_LEVEL == 16
#define GIC_PRI_SHIFT		4
#define GIC_PRI_MASK		(0x0f)
#elif GIC_PRI_LEVEL == 32
#define GIC_PRI_SHIFT		3
#define GIC_PRI_MASK		(0x1f)
#elif GIC_PRI_LEVEL == 64
#define GIC_PRI_SHIFT		2
#define GIC_PRI_MASK		(0x3f)
#elif GIC_PRI_LEVEL == 128
#define GIC_PRI_SHIFT		1
#define GIC_PRI_MASK		(0x7f)
#elif GIC_PRI_LEVEL == 256
#define GIC_PRI_SHIFT		0
#define GIC_PRI_MASK		(0xff)
#else
#error Invalid number of priority levels for GIC.
#endif /* GIC_PRI_LEVEL == 16 */


/*
 *  GICレジスタのアドレスを定義するためのマクロ
 *
 *  GICレジスタのアドレスを，アセンブリ言語からも参照できるようにするた
 *  めのマクロ．
 */

/*
 *  CPUインタフェース関連の定義
 */
#define GICC_CTLR		(GICC_BASE + 0x00)
#define GICC_PMR		(GICC_BASE + 0x04)
#define GICC_BPR		(GICC_BASE + 0x08)
#define GICC_IAR		(GICC_BASE + 0x0C)
#define GICC_EOIR		(GICC_BASE + 0x10)
#define GICC_RPR		(GICC_BASE + 0x14)
#define GICC_HPIR		(GICC_BASE + 0x18)

/*
 *  CPUインタフェース制御レジスタ（GICC_CTLR）の設定値（GICv1でセキュリ
 *  ティ拡張がない場合）
 */
#define GICC_CTLR_DISABLE		(0x00000000)
#define GICC_CTLR_ENABLE		(0x00000001)


/*
 *  ディストリビュータ関連の定義
 */
#define GICD_CTLR			(GICD_BASE + 0x000)
#define GICD_TYPER			(GICD_BASE + 0x004)
#define GICD_IIDR			(GICD_BASE + 0x008)
#define GICD_IGROUPR(n)		(GICD_BASE + 0x080 + (n) * 4)
#define GICD_ISENABLER(n)	(GICD_BASE + 0x100 + (n) * 4)
#define GICD_ICENABLER(n)	(GICD_BASE + 0x180 + (n) * 4)
#define GICD_ISPENDR(n)		(GICD_BASE + 0x200 + (n) * 4)
#define GICD_ICPENDR(n)		(GICD_BASE + 0x280 + (n) * 4)
#define GICD_ISACTIVER(n)	(GICD_BASE + 0x300 + (n) * 4)
#define GICD_ICACTIVER(n)	(GICD_BASE + 0x380 + (n) * 4)
#define GICD_IPRIORITYR(n)	(GICD_BASE + 0x400 + (n) * 4)
#define GICD_ITARGETSR(n)	(GICD_BASE + 0x800 + (n) * 4)
#define GICD_ICFGR(n)		(GICD_BASE + 0xc00 + (n) * 4)
#define GICD_NSCAR(n)		(GICD_BASE + 0xe00 + (n) * 4)
#define GICD_SGIR			(GICD_BASE + 0xf00)
#define GICD_CPENDSGIR(n)	(GICD_BASE + 0xf10 + (n) * 4)
#define GICD_SPENDSGIR(n)	(GICD_BASE + 0xf20 + (n) * 4)

/*
 *  ディストリビュータ制御レジスタ（GICD_CTLR）の設定値
 */
#define GICD_CTLR_DISABLE	(0x00000000)
#define GICD_CTLR_ENABLE	(0x00000001)

/*
 *  割込みコンフィギュレーションレジスタ（GICD_ICFGRn）の設定値
 *
 */
#define GICD_ICFGRn_LEVEL	(0x00000000)
#define GICD_ICFGRn_EDGE	(0x00000002)

/*
 *  ソフトウェア生成割込みのレジスタ（GICD_SGIR）の設定値
 */
#define GICD_SGIR_CPUTARGET_OFFSET	16u

#ifndef _MACRO_ONLY

/*
 *  CPUインタフェースの操作
 */

/*
 *  割込み優先度マスクを設定（priは内部表現）
 */ 
Inline void
gicc_set_priority(UINT pri)
{
	sil_wrw_mem((VP)GICC_PMR, (pri << GIC_PRI_SHIFT));
}

/*
 *  割込み優先度マスクを取得（内部表現で返す）
 */ 
Inline UINT
gicc_get_priority(void)
{
	return(sil_rew_mem((VP)GICC_PMR) >> GIC_PRI_SHIFT);
}

/*
 *  CPUインタフェースの初期化
 */
extern void gicc_initialize(void);

/*
 *  CPUインタフェースの終了
 */
extern void gicc_terminate(void);

/*
 *  ディストリビュータの操作
 */

/*
 *  割込み禁止（割込みイネーブルのクリア）
 */
Inline void
gicd_disable_int(INTNO intno)
{
	sil_wrw_mem((VP)GICD_ICENABLER(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込み許可（割込みイネーブルのセット）
 */
Inline void
gicd_enable_int(INTNO intno)
{
	sil_wrw_mem((VP)GICD_ISENABLER(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込みペンディングのクリア
 */
Inline void
gicd_clear_pending(INTNO intno)
{
	sil_wrw_mem((VP)GICD_ICPENDR(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込みペンディングのセット
 */
Inline void
gicd_set_pending(INTNO intno)
{
	sil_wrw_mem((VP)GICD_ISPENDR(intno / 32), (1U << (intno % 32)));
}

/*
 *  割込みペンディングのチェック
 */
Inline BOOL
gicd_probe_pending(INTNO intno)
{
	return((sil_rew_mem((VP)GICD_ISPENDR(intno / 32)) & (1U << (intno % 32)))
																	!= 0U);
}

/*
 *  割込みのコンフィグレーション
 */
Inline void
gicd_config(INTNO intno, UINT config)
{
	UINT	shift = (intno % 16) * 2;
	UW	reg;

	reg = sil_rew_mem((VP)GICD_ICFGR(intno / 16));
	reg &= ~(0x03U << shift);
	reg |= (config << shift);
	sil_wrw_mem((VP)GICD_ICFGR(intno / 16), reg);
}

/*
 *  割込み要求ラインに対する割込み優先度の設定（priは内部表現）
 */
Inline void
gicd_set_priority(INTNO intno, UINT pri)
{
	UINT	shift = (intno % 4) * 8;
	UW	reg;

	reg = sil_rew_mem((VP)GICD_IPRIORITYR(intno / 4));
	reg &= ~(0xffU << shift);
	reg |= ((pri << GIC_PRI_SHIFT) << shift);
	sil_wrw_mem((VP)GICD_IPRIORITYR(intno / 4), reg);
}

/*
 *  割込みターゲットプロセッサの設定
 *
 *  prcsは，ターゲットとするプロセッサを表すビットのビット毎論理和で指
 *  定する．
 *		プロセッサ0 : 0x01
 *		プロセッサ1 : 0x02
 *		プロセッサ2 : 0x04
 *		プロセッサ3 : 0x08
 */
Inline void
gicd_set_target(INTNO intno, UINT prcs)
{
	UINT	shift = (intno % 4) * 8;
	UW	reg;

	reg = sil_rew_mem((VP)GICD_ITARGETSR(intno / 4));
	reg &= ~(0xffU << shift);
	reg |= (prcs << shift);
	sil_wrw_mem((VP)GICD_ITARGETSR(intno / 4), reg);
}

/*
 *  ソフトウェア割込みを発行
 */
Inline void
gicd_set_sgi(int cpu, int id)
{
	CP15_DATA_SYNC_BARRIER();
	sil_wrw_mem((VP)GICD_SGIR, (cpu << GICD_SGIR_CPUTARGET_OFFSET)|id);
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  ディストリビュータの初期化
 */
extern void gicd_initialize(void);

/*
 *  各コアで実行するDICインタフェースのSGIとPPIの初期化
 */
extern void gicd_sgi_ppi_init(void);

/*
 *  ディストリビュータの終了
 */
extern void gicd_terminate(void);

#endif /* _MACRO_ONLY */
#endif /* TOPPERS_GIC_KERNEL_H */
