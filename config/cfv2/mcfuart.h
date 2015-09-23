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
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *  @(#) $Id: mcfuart.h,v 1.1 2005/07/06 00:45:07 honda Exp $
 */

/*
 *   COLDFIRE内蔵UART用 簡易ドライバ
 */

#ifndef _MCFUART_H_
#define _MCFUART_H_

#include <t_config.h>

/*
 *  シリアルI/Oポート初期化ブロック
 */
typedef struct sio_port_initialization_block
{
	UW reg_base;				/* レジスタのベースアドレス */
	UH brr;					/* ボーレートの設定値   */
	UB smr;					/* モードレジスタの設定値   */
	UB int_level;				/* 割り込みレベルの設定値   */
} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロック
 */
typedef struct sio_port_control_block
{
	const SIOPINIB *siopinib;	/* シリアルI/Oポート初期化ブロック */
	VP_INT exinf;				/* 拡張情報 */
	BOOL openflag;				/* オープン済みフラグ */
	BOOL getready;				/* 文字を受信した状態 */
	BOOL putready;				/* 文字を送信できる状態 */
	VB   imr;				/* 割込みマスクの状態 */
} SIOPCB;

/*
 *  コントロールレジスタのアドレスのオフセット 
 */
#define MCF_UART_UMR1	0x00
#define MCF_UART_UMR2	0x00
#define MCF_UART_USR	0x04
#define MCF_UART_UCSR	0x04
#define MCF_UART_UCR	0x08
#define MCF_UART_URB	0x0c
#define MCF_UART_UTB	0x0c
#define MCF_UART_UIPCR	0x10
#define MCF_UART_UACR	0x10
#define MCF_UART_UISR	0x14
#define MCF_UART_UIMR	0x14
#define MCF_UART_UBG1	0x18
#define MCF_UART_UBG2	0x1c
#define MCF_UART_UIP	0x34
#define MCF_UART_UOP1	0x38
#define MCF_UART_UOP0	0x3c

/*
 *  コントロールレジスタの設定値
 */
/*  FIFOコントロールレジスタSCFCRのビット  */
#define MCF_UART_UCR_RC_MASK  0x03
#define MCF_UART_UCR_RC_EN   0x01
#define MCF_UART_UCR_RC_DIS  0x02
#define MCF_UART_UCR_TC_MASK  0x0C
#define MCF_UART_UCR_TC_EN   0x04
#define MCF_UART_UCR_TC_DIS  0x08
#define MCF_UART_UCR_MISC_MASK 0x70
#define MCF_UART_UCR_MISC_RRC  0x10
#define MCF_UART_UCR_MISC_RR   0x20
#define MCF_UART_UCR_MISC_RT   0x30
#define MCF_UART_UCR_MISC_RE   0x40
#define MCF_UART_UCR_MISC_RBK  0x50
#define MCF_UART_UCR_MISC_SBK  0x60
/*  シリアルコントロールレジスタUIMRのビット  */
#define MCF_UART_UIMR_TXRDY 0x01
#define MCF_UART_UIMR_RXRDY 0x02
#define MCF_UART_UIMR_DB    0x04
/*  シリアルコントロールレジスタUISRのビット  */
#define MCF_UART_UISR_TXRDY 0x01
#define MCF_UART_UISR_RXRDY 0x02
#define MCF_UART_UISR_DB    0x04
/*  シリアルステータスレジスタSC1SSRのビット  */
#define MCF_UART_USR_RXRDY (0x01)	/* 受信レディ */
#define MCF_UART_USR_FULL  (0x02)	/* FIFOフル   */
#define MCF_UART_USR_TXRDY (0x04)	/* 送信レディ */
#define MCF_UART_USR_TEMP  (0x08)	/* トランスミッタエンプティ */
#define MCF_UART_USR_OE    (0x10)	/* オーバーランエラー */
#define MCF_UART_USR_RE    (0x20)	/* パリティエラー */
#define MCF_UART_USR_FE    (0x40)	/* フレーミングエラー */
#define MCF_UART_USR_RB    (0x80)	/* ブレーク受信 */

/*
 *  UBGの設定
 */
#define SMR_CKS 	0x0			/*  分周比          */
#define BRR9600 	195 			/*  9600 ビットレート    */
#define BRR19200	97			/*  19200 ビットレート    */
#define BRR38400	48			/*  38400 ビットレート    */
#define BRR57600 	32			/*  57600 ビットレート 	*/
#define BRR115200 	16			/*  115200 ビットレート */

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_ERDY_SND	1u		/* 送信可能コールバック */
#define SIO_ERDY_RCV	2u		/* 受信通知コールバック */

/*
 *  SIOドライバの初期化ルーチン
 */
extern void mcfuart_initialize (void);

/*
 *  オープンしているポートがあるか？
 */
extern BOOL mcfuart_openflag (ID siopid);

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB *mcfuart_opn_por (ID siopid, VP_INT exinf);

/*
 *  シリアルI/Oポートのクローズ
 */
extern void mcfuart_cls_por (SIOPCB * siopcb);

/*
 *  シリアルI/Oポートへの文字送信
 */
extern BOOL mcfuart_snd_chr (SIOPCB * siopcb, char c);

/*
 *  シリアルI/Oポートからの文字受信
 */
extern INT mcfuart_rcv_chr (SIOPCB * siopcb);

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void mcfuart_ena_cbr (SIOPCB * siopcb, UINT cbrtn);

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void mcfuart_dis_cbr (SIOPCB * siopcb, UINT cbrtn);

/*
 *  SIOの割込みサービスルーチン
 */
extern void mcfuart_isr (void);

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void mcfuart_ierdy_snd (VP_INT exinf);

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void mcfuart_ierdy_rcv (VP_INT exinf);

#endif /* _MCFUART_H_ */
