/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2006-2016 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  $Id: gic_config.c 1219 2017-04-25 07:05:53Z ertl-honda $
 */

/*
 *		カーネルの割込みコントローラ依存部（GIC用）
 */

#include "jsp_kernel.h"
#include "gic_config.h"

/*
 *  CPUインタフェースの初期化
 */
void
gicc_initialize(void)
{
	/*
	 *  CPUインタフェースをディスエーブル
	 */
	sil_wrw_mem((VP)GICC_CTLR, GICC_CTLR_DISABLE);

	/*
	 *  割込み優先度マスクを最低優先度に設定
	 */
	gicc_set_priority((GIC_PRI_LEVEL - 1));

	/*
	 *  割込み優先度の全ビット有効に
	 */
	sil_wrw_mem((VP)GICC_BPR, 0U);

	/*
	 *  アクティブな割込みがあれば，EOIによりクリアする
	 */
	sil_wrw_mem((VP)GICC_EOIR, sil_rew_mem((VP)GICC_IAR));

	/*
	 *  CPUインタフェースをイネーブル
	 */
	sil_wrw_mem((VP)GICC_CTLR, GICC_CTLR_ENABLE);
}

/*
 *  CPUインタフェースの終了処理
 */
void
gicc_terminate(void)
{
	sil_wrw_mem((VP)GICC_CTLR, GICC_CTLR_DISABLE);
}

/*
 *  ディストリビュータの操作
 */

/*
 *  ディストリビュータの初期化
 */
void
gicd_initialize(void)
{
	int		i;

	/*
	 *  ディストリビュータをディスエーブル
	 */
	sil_wrw_mem((VP)GICD_CTLR, GICD_CTLR_DISABLE);

	/*
	 *  すべての割込みを禁止
	 */
	for (i = GIC_INTNO_SPI0 / 32; i < (GIC_TNUM_INTNO + 31) / 32; i++) {
		sil_wrw_mem((VP)GICD_ICENABLER(i), 0xffffffffU);
	}

	/*
	 *  すべての割込みペンディングをクリア
	 */
	for (i = GIC_INTNO_SPI0 / 32; i < (GIC_TNUM_INTNO + 31) / 32; i++) {
		sil_wrw_mem((VP)GICD_ICPENDR(i), 0xffffffffU);
	}

	/*
	 *  すべての割込みを最低優先度に設定
	 */
	for (i = GIC_INTNO_SPI0 / 4; i < (GIC_TNUM_INTNO + 3) / 4; i++){
		sil_wrw_mem((VP)GICD_IPRIORITYR(i), 0xffffffffU);
	}

	/*
	 *  すべての共有ペリフェラル割込みのターゲットをプロセッサ0に設定
	 */
	for (i = GIC_INTNO_SPI0 / 4; i < (GIC_TNUM_INTNO + 3) / 4; i++) {
		sil_wrw_mem((VP)GICD_ITARGETSR(i), 0x01010101U);
	}

	/*
	 *  すべてのペリフェラル割込みをレベルトリガに設定
	 */
	for (i = GIC_INTNO_PPI0 / 16; i < (GIC_TNUM_INTNO + 15) / 16; i++) {
		sil_wrw_mem((VP)GICD_ICFGR(i), 0x00000000U);
	}

	/*
	 *  ディストリビュータをイネーブル
	 */
	sil_wrw_mem((VP)GICD_CTLR, GICD_CTLR_ENABLE);
}

/*
 *  ディストリビュータの終了処理
 */
void
gicd_terminate(void)
{
	sil_wrw_mem((VP)GICD_CTLR, GICD_CTLR_DISABLE);
}

/*
 *  各コアで実行するDICインタフェースのSGIとPPIの初期化
 */
void
gicd_sgi_ppi_init(void)
{
	int i;

	/* 割込みを全て禁止 */
	for(i = 0; i < GIC_INTNO_SPI0/32; i++){
		sil_wrw_mem((VP)GICD_ICENABLER(i), 0xffffffffU);
	}

	/* ペンディングをクリア */
	for(i = 0; i < GIC_INTNO_SPI0/32; i++){
		sil_wrw_mem((VP)GICD_ICPENDR(i), 0xffffffffU);
	}

	/* 優先度最低に設定  */
	for(i = 0; i < GIC_INTNO_SPI0/4; i++){
		sil_wrw_mem((VP)GICD_IPRIORITYR(i), 0xffffffffU);
	}

	/* モード初期化(1-N Level) */
	for(i = 0; i < GIC_INTNO_SPI0/16; i++){
		sil_wrw_mem((VP)GICD_ICFGR(i), 0x00000000U);
	}
}
