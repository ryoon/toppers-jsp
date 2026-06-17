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
 *  @(#) $Id: mpcore.h 1218 2017-04-25 07:05:23Z ertl-honda $
 */

/*
 *  MPCORE のハードウェア資源の定義
 */

#ifndef MPCORE_H
#define MPCORE_H

#include <sil.h>

/*
 * 割込み番号
 */
#define IRQNO_MPCORE_GTM     27
#define IRQNO_MPCORE_TM      29
#define IRQNO_MPCORE_WDT     30
#define IRQNO_GTIM           29
 
/*
 *  内蔵タイマ関連の定義
 */
#define MPCORE_TM_LR     (MPCORE_PMR_BASE + 0x0600)  /* ロードレジスタ           */
#define MPCORE_TM_COUNT  (MPCORE_PMR_BASE + 0x0604)  /* カウンターレジスタ       */
#define MPCORE_TM_CNT    (MPCORE_PMR_BASE + 0x0608)  /* コントロールレジスタ     */
#define MPCORE_TM_ISR    (MPCORE_PMR_BASE + 0x060C)  /* 割込みステータスレジスタ */

#define MPCORE_TM_CNT_EN        0x1
#define MPCORE_TM_CNT_AR        0x2
#define MPCORE_TM_CNT_IEN       0x4
#define MPCORE_TM_CNT_PS_OFFSET   8

#define MPCORE_TM_ISR_SCBIT    0x01

/*
 * Global Timer関連の定義（r1以上でサポート）
 */
#define MPCORE_GTM_COUNT_L (MPCORE_PMR_BASE + 0x0200)  /* カウンターレジスタL       */
#define MPCORE_GTM_COUNT_U (MPCORE_PMR_BASE + 0x0204)  /* カウンターレジスタH       */
#define MPCORE_GTM_CNT     (MPCORE_PMR_BASE + 0x0208)  /* コントロールレジスタ      */
#define MPCORE_GTM_ISR     (MPCORE_PMR_BASE + 0x020C)  /* 割込みステータスレジスタ  */
#define MPCORE_GTM_COMPV_L (MPCORE_PMR_BASE + 0x0210)  /* コンペアバリューレジスタL */
#define MPCORE_GTM_COMPV_U (MPCORE_PMR_BASE + 0x0214)  /* コンペアバリューレジスタH */
#define MPCORE_GTM_AIR     (MPCORE_PMR_BASE + 0x0218)  /* オートインクリメントレジスター */

#define MPCORE_GTM_ISR_EF        0x01

#define MPCORE_GTM_CNT_EN        0x01
#define MPCORE_GTM_CNT_CE        0x02
#define MPCORE_GTM_CNT_IEN       0x04
#define MPCORE_GTM_CNT_AI        0x08
#define MPCORE_GTM_CNT_PS_OFFSET   8

/*
 *  Snoop Control Unit
 */
#define MPCORE_SCU_CTRL    (MPCORE_PMR_BASE + 0x0000)  /* コントロールレジスタ */
#define MPCORE_SCU_CONFIG  (MPCORE_PMR_BASE + 0x0004)  /* コンフィギュレーションレジスタ */
#define MPCORE_SCU_CPUST   (MPCORE_PMR_BASE + 0x0008)  /* CPUステータス */
#define MPCORE_SCU_IALL    (MPCORE_PMR_BASE + 0x000C)  /* インバリッドオール */
#define MPCORE_SCU_SNSAC   (MPCORE_PMR_BASE + 0x0054)  /* ノンセキュアアクセスコントロールレジスタ*/


#define MPCORE_SCU_CTRL_EN (1 << 0)   /* SCUイネーブル */

#define MPCORE_SCU_PM_CTRL_ENA  (1 << 0)    /* パフォーマンスモニタ有効化 */
#define MPCORE_SCU_PM_CTRL_RST  (1 << 1)    /* パフォーマンスモニタリセット */

#define MPCORE_SCU_PM_EVT_CYC_COUNT0_4 (31 << 0) /* サイクルカウント指定 */

#define MPCORE_SCU_IALL_WAYS   (0x0000ffffU)

#ifndef _MACRO_ONLY

/*
 *  SCUの初期化
 */
extern void scu_init(void);

/*
 *  SMPモードの有効化
 */
extern void smp_mode_enable(void);

#endif /* _MACRO_ONLY */
#endif /* MPCORE_H */
