/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
 *  Copyright (C) 2010 by Cronus Computer Works, JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id
 */

#include "jsp_kernel.h"
#include <sil.h>
#include <s_services.h>
#include "sh726xscif.h"

/*
 *  ターゲットシステム依存の初期化
 */
/*
 *  シリアルI/Oポートの初期化 banner出力のためカーネルの初期化と無関係に行う
 */
void
sys_initialize ()
{
	SIOPCB *siopcb;

	/* 使用する周辺機器はここで有効にしておく。 */
	/*  SCIFデータ入出力ポートの設定  */

#ifndef GDB_STUB

	/* SCIF3 */
	sil_wrb_mem (STBCR4, (VB) ((VB) sil_reb_mem (STBCR4) & ~0x10));
	sil_wrh_mem (PECRL4, sil_reh_mem (PECRL4) | (0x0030 | 0x0003));

#else /* GDB_STUB */

	/* SCIF3 */
	sil_wrb_mem (STBCR4, (VB) ((VB) sil_reb_mem (STBCR4) & ~0x10));
	sil_wrh_mem (PECRL4, sil_reh_mem (PECRL4) | (0x0003 | 0x0030));

#endif /* GDB_STUB */

	/* CMT0 */
	sil_wrb_mem (STBCR4, (VB) ((VB) sil_reb_mem (STBCR4) & ~0x04));
	/*
	 *  デバイス依存のオープン処理．
	 */
	/*バナー出力するため */
	sh2scif_initialize();
	siopcb = sh2scif_opn_por (LOGTASK_PORTID, 0);
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit ()
{
	sh2_exit ();
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc (char c)
{
	if (c == '\n') {
		sh2_putc (LOGTASK_PORTID, '\r');
	}
	sh2_putc (LOGTASK_PORTID, c);
}

/* ハードウェアの設定 */
/* ここではバスの設定のみ行う。周辺機器の設定はsys_initializeで行う。 */
#ifndef GDB_STUB
void
hardware_init_hook (void)
{
	/* クロック発振器の初期化 */
	sil_wrh_mem (FRQCR, 0x0104);	/* クロック設定 Iφ=200MHz, Bφ=66.666MHz, Pφ=33.333MHz */

	/* バスコントローラの設定 */
	sil_wrw_mem (CMNCR, 0x00001010);
	sil_wrw_mem (CS0BCR, 0x24920400);
	sil_wrw_mem (CS0WCR, 0x00001382);
	sil_wrw_mem (CS3BCR, 0x24924600);
	sil_wrw_mem (CS3WCR, 0x00004912);
	sil_wrw_mem (SDCR, 0x00000909);
	sil_wrw_mem (RTCSR, 0xa55a0010);
	sil_wrw_mem (RTCOR, 0xa55a0040);

	/* I/Oポートの設定 */
	sil_wrh_mem (PBIORL, 0x0c00);	/* PC11,PC10 出力方向 (LED接続ポート) */
	sil_wrh_mem (PBCRL4, 0x5a00);
	sil_wrh_mem (PBCRL3, 0x0000);	/* PC11,PC10モード ポート選択 (LED接続ポート) */
	sil_wrh_mem (PBCRL2, 0x0000);
	sil_wrh_mem (PBCRL1, 0x0000);
	sil_wrh_mem (PCIORL, 0x0000);
	sil_wrh_mem (PCCRL4, 0x0011);
	sil_wrh_mem (PCCRL3, 0x1111);
	sil_wrh_mem (PCCRL2, 0x1111);
	sil_wrh_mem (PCCRL1, 0x1111);
	sil_wrh_mem (PDIORL, 0x0000);
	sil_wrh_mem (PDCRL4, 0x1111);
	sil_wrh_mem (PDCRL3, 0x1111);
	sil_wrh_mem (PDCRL2, 0x1111);
	sil_wrh_mem (PDCRL1, 0x1111);
	sil_wrh_mem (PEIORL, 0x0000);
	sil_wrh_mem (PECRL4, 0x0000);
	sil_wrh_mem (PECRL3, 0x1010);
	sil_wrh_mem (PECRL2, 0x1111);
	sil_wrh_mem (PECRL1, 0x1111);
	sil_wrh_mem (PFIORH, 0x7fff);
	sil_wrh_mem (PFIORL, 0xffff);
	sil_wrh_mem (PFCRH4, 0x0000);
	sil_wrh_mem (PFCRH3, 0x0000);
	sil_wrh_mem (PFCRH2, 0x0000);
	sil_wrh_mem (PFCRH1, 0x0000);
	sil_wrh_mem (PFCRL4, 0x0000);
	sil_wrh_mem (PFCRL3, 0x0000);
	sil_wrh_mem (PFCRL2, 0x0000);
	sil_wrh_mem (PFCRL1, 0x0000);

	/* CMT へクロック供給 */
	sil_wrb_mem (STBCR4, (VB) ((VB) sil_reb_mem (STBCR4) & ~0x04));

	/* 100usec 待ち */
	sil_wrh_mem (CMSTR, 0x0000);	/* STR0=0 カウンタ0 停止 */
	sil_wrh_mem (CMCSR_0, 0x0000);	/* CKS=00 クロック選択 PΦ/8 */
	sil_wrh_mem (CMCNT_0, 0);		/* カウンタ0 クリア，↓コンスタント値設定 */
	sil_wrh_mem (CMCOR_0, ((((PCLOCK / 8)/1000000) * 100) - 1));
	sil_wrh_mem (CMSTR, 0x0001);	/* STR0=1 カウンタ0 開始 */
	while ((sil_reh_mem (CMCSR_0) & 0x0080) == 0);
									/* CMF==1 になるまで待つ */

	sil_wrh_mem (SDRAM_MODE, 0x000);	/* SDRAMモードレジスタを設定 */

	/* キャッシュの設定 */
	sil_wrw_mem (CCR1, 0x00000808);	/* キャッシュをフラッシュ */
	sil_wrw_mem (CCR1, CCR_MODE);	/* キャッシュモードを設定 */
}
#else /*  GDB_STUB  */
void
hardware_init_hook (void)
{
}
#endif /*  GDB_STUB  */
