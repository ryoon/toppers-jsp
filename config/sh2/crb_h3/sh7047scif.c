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
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id$
 */

/*
 *  SH7047内蔵シリアルコミュニケーションインターフェイスSCI用 簡易ドライバ
 */

#include <s_services.h>
#include "sh7047.h"
#include "sh7047scif.h"

/*
 *  シリアルI/Oポート初期化ブロック
 */
static const SIOPINIB siopinib_table[TNUM_PORT] = {
	{
		(IOREG_SCI_BASE+IOREG_SCI_WINDOW*1),
		(IOREG_INTC_BASE+IOREG_IPRI_OFFSET),
		9,
		ISR_SCR3_SHIFT
	},
	{
		(IOREG_SCI_BASE+IOREG_SCI_WINDOW*2),
		(IOREG_INTC_BASE+IOREG_IPRI_OFFSET),
		8,
		ISR_SCR4_SHIFT
	}
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 *      ID = 1 をSCI3に対応させている.
 */
SIOPCB siopcb_table[TNUM_PORT];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((UINT)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  文字を受信できるか？
 */
Inline BOOL
sh7047scif_getready (SIOPCB * siopcb)
{
	/* レシーブデータレジスタフル・フラグのチェック */
	return (sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SSR_OFFSET)) & SSR_RDRF);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
sh7047scif_putready (SIOPCB * siopcb)
{
	/* トランスミットFIFOデータレジスタエンプティ・フラグのチェック */
	return (sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SSR_OFFSET)) & SSR_TDRE);
}

/*
 *  受信した文字の取り出し
 */
Inline char
sh7047scif_getchar (SIOPCB * siopcb)
{
	VB data;

	data = sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_RDR_OFFSET));
	/* レシーブデータレジスタフル・フラグのクリア */
	sil_wrb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SSR_OFFSET),
		sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SSR_OFFSET)) & ~SSR_RDRF);
	return data;
}

/*
 *  送信する文字の書込み
 */
Inline void
sh7047scif_putchar (SIOPCB * siopcb, char c)
{
	sil_wrb_mem((VP)(siopcb->siopinib->cntrl+IOREG_TDR_OFFSET), c);
	/* トランスミットFIFOデータレジスタエンプティ・フラグのクリア */
	sil_wrb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SSR_OFFSET),
		sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SSR_OFFSET)) & ~SSR_TDRE);
}

/*
 *  SIOドライバの初期化ルーチン
 */
void
sh7047scif_initialize ()
{
	SIOPCB *siopcb;
	UINT i;

	/*
     *  シリアルI/Oポート管理ブロックの初期化
     */
	for (siopcb = siopcb_table, i = 0; i < TNUM_PORT; siopcb++, i++) {
		siopcb->siopinib = &siopinib_table[i];
		siopcb->openflag = FALSE;
	}
}

/*
 *  オープンしているポートはあるか？
 */
BOOL
sh7047scif_openflag (ID siopid)
{
	if(siopcb_table[INDEX_SIOP(siopid)].openflag)
		return TRUE;
	sil_dly_nse(sh2scif_DELAY*4);	/* Wait for Banner */
	return FALSE;
}

/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sh7047scif_opn_por (ID siopid, VP_INT exinf)
{
	SIOPCB *siopcb;
	UW     base;
	unsigned char  dummy;

	siopcb = get_siopcb (siopid);
	base   = siopcb->siopinib->cntrl;
	/* 送受信停止 */
	sil_wrb_mem((VP)(base+IOREG_SCR_OFFSET),
		 sil_reb_mem((VP)(base+IOREG_SCR_OFFSET)) & ~(SCR_TE | SCR_RE));
	/* モードレジスタの設定 */
	sil_wrb_mem((VP)(base+IOREG_SMR_OFFSET), DEFAULT_SMR);
	/* ボーレートの設定 */
	sil_wrb_mem((VP)(base+IOREG_BRR_OFFSET), DEFAULT_BRR);

	sil_dly_nse(sh2scif_DELAY);
	/* 送受信、受信割込み許可 */
	sil_wrb_mem((VP)(base+IOREG_SCR_OFFSET), SCR_RIE|SCR_TE|SCR_RE);
	dummy = sil_reb_mem((VP)(base+IOREG_SSR_OFFSET));		/* Dummy read */
	/* 割込みコントローラの設定 */
	sil_wrb_mem((VP)(base+IOREG_SSR_OFFSET), SSR_TDRE);		/* Clear error flags */
	siopcb->exinf    = exinf;
	siopcb->openflag = TRUE;
	return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sh7047scif_cls_por (SIOPCB * siopcb)
{
	/* 送受信停止、割込み禁止 */
	sil_wrb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SCR_OFFSET),
			sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SCR_OFFSET)) & ~(SCR_TIE|SCR_RIE|SCR_TE|SCR_RE));

	siopcb->openflag = FALSE;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sh7047scif_snd_chr (SIOPCB * siopcb, char c)
{
	if (sh7047scif_putready (siopcb)) {
		sh7047scif_putchar (siopcb, c);
		return (TRUE);
	}
	return (FALSE);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
INT
sh7047scif_rcv_chr (SIOPCB * siopcb)
{
	if (sh7047scif_getready (siopcb)) {
		return ((INT) (UB) sh7047scif_getchar (siopcb));
	}
	return (-1);
}

/*
 *  シリアルI/Oポートからのコールバック許可
 */
void
sh7047scif_ena_cbr (SIOPCB * siopcb, UINT cbrtn)
{
	UW base = siopcb->siopinib->cntrl;

	switch (cbrtn) {
	case SIO_ERDY_SND:		/* 送信割込み要求を許可 */
		sil_wrb_mem((VP)(base+IOREG_SCR_OFFSET), sil_reb_mem((VP)(base+IOREG_SCR_OFFSET)) | SCR_TIE);
		break;
	case SIO_ERDY_RCV:		/* 受信割込み要求を許可 */
		sil_wrb_mem((VP)(base+IOREG_SCR_OFFSET), sil_reb_mem((VP)(base+IOREG_SCR_OFFSET)) | SCR_RIE);
		break;
	default:
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバック禁止
 */
void
sh7047scif_dis_cbr (SIOPCB * siopcb, UINT cbrtn)
{
	UW base = siopcb->siopinib->cntrl;

	switch (cbrtn) {
	case SIO_ERDY_SND:		/* 送信割込み要求を禁止 */
		sil_wrb_mem((VP)(base+IOREG_SCR_OFFSET), sil_reb_mem((VP)(base+IOREG_SCR_OFFSET)) & ~SCR_TIE);
		break;
	case SIO_ERDY_RCV:		/* 受信割込み要求を禁止 */
		sil_wrb_mem((VP)(base+IOREG_SCR_OFFSET), sil_reb_mem((VP)(base+IOREG_SCR_OFFSET)) & ~SCR_RIE);
		break;
	default:
		break;
	}
}

/*
 *  シリアルI/Oポートに対する送信割込み処理
 */
Inline void
sh7047scif_isr_siop_out (SIOPCB * siopcb)
{
	VB scr0 = sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SCR_OFFSET));

	if ((scr0 & SCR_TIE) != 0 && sh7047scif_putready (siopcb)) {
		/*
		 *  送信通知コールバックルーチンを呼び出す．
		 */
		sh7047scif_ierdy_snd (siopcb->exinf);
	}
}

/*
 *  シリアルI/Oポートに対する受信割込み処理
 */
Inline void
sh7047scif_isr_siop_in (SIOPCB * siopcb)
{
	VB scr0 = sil_reb_mem((VP)(siopcb->siopinib->cntrl+IOREG_SCR_OFFSET));

	if ((scr0 & SCR_RIE) != 0 && sh7047scif_getready (siopcb)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sh7047scif_ierdy_rcv (siopcb->exinf);
	}
}

/*
 *  SIO送信割込みサービスルーチン(SCI3用)
 */
void
sh7047scif3_isr_out ()
{
	if (siopcb_table[0].openflag) {
		sh7047scif_isr_siop_out (&(siopcb_table[0]));
	}
}

/*
 *  SIO送信割込みサービスルーチン(SCI4用)
 */
void
sh7047scif4_isr_out ()
{
	if (siopcb_table[1].openflag) {
		sh7047scif_isr_siop_out (&(siopcb_table[1]));
	}
}

/*
 *  SIO受信割込みサービスルーチン(SCI3用)
 */
void
sh7047scif3_isr_in ()
{
	if (siopcb_table[0].openflag) {
		sh7047scif_isr_siop_in (&(siopcb_table[0]));
	}
}

/*
 *  SIO受信割込みサービスルーチン(SCI4用)
 */
void
sh7047scif4_isr_in ()
{
	if (siopcb_table[1].openflag) {
		sh7047scif_isr_siop_in (&(siopcb_table[1]));
	}
}

/*
 *  SIO受信エラー割込みサービスルーチン(SCI3用)
 */
void
sh7047scif3_isr_error (void)
{
	UW base = siopcb_table[0].siopinib->cntrl;

	if (siopcb_table[0].openflag) {
		sil_wrb_mem((VP)(base+IOREG_SSR_OFFSET), sil_reb_mem((VP)(base+IOREG_SSR_OFFSET)) & ~(SSR_OPER|SSR_FER|SSR_PER));
	}
}

/*
 *  SIO受信エラー割込みサービスルーチン(SCI4用)
 */
void
sh7047scif4_isr_error (void)
{
	UW base = siopcb_table[1].siopinib->cntrl;

	if (siopcb_table[1].openflag) {
		sil_wrb_mem((VP)(base+IOREG_SSR_OFFSET), sil_reb_mem((VP)(base+IOREG_SSR_OFFSET)) & ~(SSR_OPER|SSR_FER|SSR_PER));
	}
}


