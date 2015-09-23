/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2000-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
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
 *  @(#) $Id: sh7145sci1.c,v 1.1 2004/09/03 15:52:38 honda Exp $
 */

/*
 *   SH2内蔵シリアルコミュニケーションインタフェースSCI用 簡易ドライバ
 */

#include <s_services.h>
#ifdef GDB_STUB
#include "sh7145sci0.h"
#else	/*  GDB_STUB  */
#include "sh7145sci1.h"
#endif	/*  GDB_STUB  */

/*
 *  シリアルI/Oポート管理ブロックの定義
 *  　2chサポートに拡張する場合は初期値用のデータも含める
 */
// check ↑本当に含めるのか？
struct sio_port_control_block
{
	VP_INT exinf;				/* 拡張情報 */
	BOOL openflag;				/* オープン済みフラグ */
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 *  　　ID = 1 をSCI0に対応させている．
 */
SIOPCB siopcb_table[TNUM_SIOP];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((UINT)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  文字を受信できるか？
 */
Inline BOOL
sh2sci1_getready (SIOPCB * siopcb)
{
	/*  レシーブデータレジスタフル・フラグのチェック  */
	return (sil_reb_mem (SCI_SSR_1) & SSR_RDRF);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
sh2sci1_putready (SIOPCB * siopcb)
{
	/*  トランスミットFIFOデータレジスタエンプティ・フラグのチェック */
	return (sil_reb_mem (SCI_SSR_1) & SSR_TDRE);
}

/*
 *  受信した文字の取出し
 */
Inline char
sh2sci1_getchar (SIOPCB * siopcb)
{
	VB data;

	data = sil_reb_mem (SCI_RDR_1);
	/*  レシーブデータレジスタフル・フラグのクリア  */
	sil_wrb_mem (SCI_SSR_1, sil_reb_mem (SCI_SSR_1) & ~SSR_RDRF);
	return data;
}

/*
 *  送信する文字の書込み
 */
Inline void
sh2sci1_putchar (SIOPCB * siopcb, char c)
{
	sil_wrb_mem (SCI_TDR_1, c);
	/*  トランスミットFIFOデータレジスタエンプティ・フラグのクリア */
	sil_wrb_mem (SCI_SSR_1, sil_reb_mem (SCI_SSR_1) & ~SSR_TDRE);
}

/*
 *  SIOドライバの初期化ルーチン
 */
void
sh2sci1_initialize ()
{
	SIOPCB *siopcb;
	UINT i;

	/*
	 *  シルアルI/Oポート管理ブロックの初期化
	 */
	for (siopcb = siopcb_table, i = 0; i < TNUM_SIOP; siopcb++, i++) {
		siopcb->openflag = FALSE;
	}
}

/*
 *  オープンしているポートがあるか？
 */
BOOL
sh2sci1_openflag (void)
{
#if TNUM_SIOP < 2
	return (siopcb_table[0].openflag);
#else /* TNUM_SIOP < 2 */
	return (siopcb_table[0].openflag || siopcb_table[1].openflag);
#endif /* TNUM_SIOP < 2 */
}

/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sh2sci1_opn_por (ID siopid, VP_INT exinf)
{
	SIOPCB *siopcb;

	siopcb = get_siopcb (siopid);

#ifdef GDB_STUB
	sil_wrh_mem (MSTCR1, (VH)((VH)sil_reh_mem(MSTCR1)& ~0x0001));
#else	/*  GDB_STUB  */
	sil_wrh_mem (MSTCR1, (VH)((VH)sil_reh_mem(MSTCR1)& ~0x0002));
#endif	/*  GDB_STUB  */
//	sil_wrb_mem (SCI_SDCR_1,0x00);
	sil_wrb_mem (SCI_SDCR_1,0xf0);

	/*  送受信停止  */
	//sil_wrb_mem (SCI_SCR_1,sil_reb_mem (SCI_SCR_1) & ~(SCI_TE | SCI_RE));
	sil_wrb_mem (SCI_SCR_1,0x00);

	/*  送受信フォーマット  */
	sil_wrb_mem (SCI_SMR_1, 0x00);
	/*  調歩同期式  */
	/*  8ビット、パリティなし  */
	/*  ストップビットレングス：1   */
	/*  クロックセレクト */

	sil_wrb_mem (SCI_BRR_1, (VB) SCI_BRR);

	/*
	 * ボーレートの設定後、1カウント分待たなければならない。
	 */
	sil_dly_nse (sh2sci1_DELAY);	/* 値はｓｈ１と同じ */

	/*  SCIデータ入出力ポートの設定  */
	sil_wrh_mem (PACRL2, sil_reh_mem (PACRL2) | (PACRL2_TXD1 | PACRL2_RXD1));

	/* エラーフラグをクリア */
	sil_wrb_mem (SCI_SSR_1, sil_reb_mem (SCI_SSR_1) & ~(SSR_ORER | SSR_FER |SSR_PER));
	//送受信許可、受信割り込み許可
	sil_wrb_mem (SCI_SCR_1, (VB) (SCI_RIE | SCI_TE | SCI_RE));

	siopcb->exinf = exinf;
	siopcb->openflag = TRUE;
	return (siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sh2sci1_cls_por (SIOPCB * siopcb)
{
	/*  送受信停止、割込み禁止  */
	sil_wrb_mem (SCI_SCR_1,sil_reb_mem (SCI_SCR_1) & (VB) ~ (SCI_TIE | SCI_RIE | SCI_TE | SCI_RE));
#ifdef GDB_STUB
	sil_wrh_mem (MSTCR1, (VH)((VH)sil_reh_mem(MSTCR1) | 0x0001));
#else	/*  GDB_STUB  */
	sil_wrh_mem (MSTCR1, (VH)((VH)sil_reh_mem(MSTCR1) | 0x0002));
#endif	/*  GDB_STUB  */
	siopcb->openflag = FALSE;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sh2sci1_snd_chr (SIOPCB * siopcb, char c)
{
	if (sh2sci1_putready (siopcb)) {
		sh2sci1_putchar (siopcb, c);
		return (TRUE);
	}
	return (FALSE);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
INT
sh2sci1_rcv_chr (SIOPCB * siopcb)
{
	if (sh2sci1_getready (siopcb)) {
		return ((INT) (UB) sh2sci1_getchar (siopcb));
	}
	return (-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sh2sci1_ena_cbr (SIOPCB * siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:			/* 送信割り込み要求を許可 */
		sil_wrb_mem (SCI_SCR_1, sil_reb_mem (SCI_SCR_1) | SCI_TIE);
		break;
	case SIO_ERDY_RCV:			/* 受信割り込み要求を許可 */
		sil_wrb_mem (SCI_SCR_1, sil_reb_mem (SCI_SCR_1) | SCI_RIE);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sh2sci1_dis_cbr (SIOPCB * siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:			/* 送信割り込み要求を禁止 */
		sil_wrb_mem (SCI_SCR_1, sil_reb_mem (SCI_SCR_1) & ~SCI_TIE);
		break;
	case SIO_ERDY_RCV:			/* 受信割り込み要求を禁止 */
		sil_wrb_mem (SCI_SCR_1, sil_reb_mem (SCI_SCR_1) & ~SCI_RIE);
		break;
	}
}

/*
 *  シリアルI/Oポートに対する送信割込み処理
 */
Inline void
sh2sci1_isr_siop_out (SIOPCB * siopcb)
{
	VB scr0 = sil_reb_mem (SCI_SCR_1);

	if ((scr0 & SCI_TIE) != 0 && sh2sci1_putready (siopcb)) {
		/*
		 *  送信通知コールバックルーチンを呼び出す．
		 */
		sh2sci1_ierdy_snd (siopcb->exinf);
	}
}

/*
 *  シリアルI/Oポートに対する受信割込み処理
 */
Inline void
sh2sci1_isr_siop_in (SIOPCB * siopcb)
{
	VB scr0 = sil_reb_mem (SCI_SCR_1);

	if ((scr0 & SCI_RIE) != 0 && sh2sci1_getready (siopcb)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sh2sci1_ierdy_rcv (siopcb->exinf);
	}
}

/*
 *  SIO送信割込みサービスルーチン ｓｈ１と同じ
 *  
 *  　SH1内蔵のSCIでは割込み番号が送受信別、チャネル別に分かれているので、
 *  　SCI0の送信割込み以外でこのルーチンが呼ばれることはない
 *  
 */
void
sh2sci1_isr_out ()
{
	if (siopcb_table[0].openflag) {
		sh2sci1_isr_siop_out (&(siopcb_table[0]));
	}
}

/*
 *  SIO受信割込みサービスルーチン　ｓｈ１と同じ
 *  
 *  　SH1内蔵のSCIでは割込み番号が送受信別、チャネル別に分かれているので、
 *  　SCI0の受信割込み以外でこのルーチンが呼ばれることはない
 *  
 */
void
sh2sci1_isr_in ()
{
	if (siopcb_table[0].openflag) {
		sh2sci1_isr_siop_in (&(siopcb_table[0]));
	}
}

/*
 *  SIO受信エラー割込みサービスルーチン　ｓｈ１と同じ
 *  
 *  　SH1内蔵のSCIでは割込み番号がチャネル別に分かれているので、
 *  　SCI0の受信エラー割込み以外でこのルーチンが呼ばれることはない
 *  　
 *  　エラー処理自体はエラーフラグのクリアのみにとどめている。
 *  　　　・オーバーランエラー
 *  　　　・フレーミングエラー
 *  　　　・パリティエラー
 */
void
sh2sci1_isr_error (void)
{

	if (siopcb_table[0].openflag) {
	sil_wrb_mem (SCI_SSR_1, sil_reb_mem (SCI_SSR_1) & ~(SSR_ORER | SSR_FER |SSR_PER));
	}
}

/*
 * ポーリングによる文字の送信
 */
void
sh2sci1_putc_pol (char c)
{
	while (!sh2sci1_putready (&siopcb_table[0]));
	sh2sci1_putchar (&siopcb_table[0], c);
}
