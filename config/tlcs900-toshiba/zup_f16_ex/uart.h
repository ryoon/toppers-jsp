/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2006-2010 by Witz Corporation, JAPAN
 * 
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 * 
 *  (3) When this software is redistributed in the forms unusable for the
 *      development of other software, such as the case when the software
 *      is embedded in a piece of equipment, either of the following two
 *      conditions must be satisfied:
 * 
 *    (a) The above copyright notice, this use conditions, and the
 *           disclaimer shown below must be shown without modification in
 *        the document provided with the redistributed software, such as
 *        the user manual.
 * 
 *    (b) How the software is to be redistributed must be reported to the
 *        TOPPERS Project according to the procedure described
 *        separately.
 * 
 *  (4) The above copyright holders and the TOPPERS Project are exempt
 *      from responsibility for any type of damage directly or indirectly
 *      caused from the use of this software and are indemnified by any
 *      users or end users of this software from any and all causes of
 *      action whatsoever.
 * 
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 * 
 *  @(#) $Id: uart.h,v 1.1 2006/04/10 08:19:25 honda Exp $
 */

/*
 *  非同期通信 (UART)シリアルチャンネルデバイス（SIO）ドライバ
 *  （Zup-F16拡張ボード用）
 */

#ifndef _UART_H_
#define _UART_H_

/*
 * シリアルコントロールレジスタの設定値 
 */
/* 19200bps = (6144000*4)/4/(BRCK)/(BRS+(16-BRK)/16)/16	*/
/* BRCK=4分周,BRS=5カウント,+(16-BRK)/16 は未使用 */
#define	BRCR_19200  	(TBIT_SIOBRCK2 | 5)
#define	BRADD_19200		0
/* 38400bps = (6144000*4)/4/(BRCK)/(BRS+(16-BRK)/16)/16	*/
/* BRCK=1分周,BRS=10カウント,+(16-BRK)/16 は未使用 */
#define	BRCR_38400		(TBIT_SIOBRCK0 | 10)
#define	BRADD_38400		0
#define	INT_LEVEL_UART	5		/* 割込みレベル */

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define	INHNO_SERIAL_IN1	INT_NO_RX1
#define	INHNO_SERIAL_OUT1	INT_NO_TX1
#define INHNO_SERIAL_IN2	INT_NO_RX0
#define INHNO_SERIAL_OUT2	INT_NO_TX0

#ifndef _MACRO_ONLY

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	UH  cntrl;		/* コントロールレジスタの番地 */
	UH  hint;		/* 割込みレジスタの番地 */

	UB  pcrfc_def;		/* デフォールトの設定値 (P9CR/P9FC) */
	UB  scmod0_def;		/* デフォールトの設定値 (SCMOD0)	*/
	UB  sccr_def;		/* デフォールトの設定値 (SCCR)		*/
	UB  brcr_def;		/* デフォールトの設定値 (BRCR)		*/
	UB  bradd_def;		/* デフォールトの設定値 (BRADD)		*/
	UB  int_clr;		/* 割込み要求フラグクリアコマンド	*/
} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block {
	const SIOPINIB	*siopinib;	/* シリアルI/Oポート初期化ブロック */
	VP_INT			exinf;		/* 拡張情報 */
	INT				rxb;		/* 受信バッファ	*/
	volatile UB		sts_flag;	/* 送信バッファ空フラグ	*/
} SIOPCB;

/*
 *  コールバックルーチンの識別番号
 */
#define UART_ERDY_SND		1u			/* 送信可能コールバック			*/
#define UART_ERDY_RCV		2u			/* 受信通知コールバック			*/

/*
 *  SIOドライバの初期化ルーチン
 */
extern void	uart_initialize(void);

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB * uart_opn_por(ID siopid, VP_INT exinf);

/*
 *  シリアルI/Oポートのクローズ
 */
extern void	uart_cls_por(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートへの文字送信
 */
extern BOOL	uart_snd_chr(SIOPCB *siopcb, char c);

/*
 *  シリアルI/Oポートからの文字受信
 */
extern INT	uart_rcv_chr(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void	uart_ena_cbr(SIOPCB *siopcb, UINT cbrtn);

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void	uart_dis_cbr(SIOPCB *siopcb, UINT cbrtn);

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void	uart_ierdy_snd(VP_INT exinf);

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void	uart_ierdy_rcv(VP_INT exinf);

#endif /* _MACRO_ONLY */
#endif /* _UART_H_ */

