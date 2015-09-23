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
 *　　シリアルI/Oデバイス（SIO/SCIF）ドライバ（CRB_H3用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include <s_services.h>
#ifndef _MACRO_ONLY
#include "sh7047scif.h"
#endif /* _MACRO_ONLY */

/*
 *  SIO/SCIFの割込みハンドラのベクタ番号
 */
/* 入出力ポートの設定はsys_config.c */
/* 割込みベクタ番号の設定はhw_serial.h */
/* 管理ブロックの設定はsh7615scif.c */

#define INHNO_SERIAL_IN	   RXI_3
#define INHNO_SERIAL_OUT   TXI_3
#define INHNO_SERIAL_ERROR ERI_3
#if TNUM_PORT >= 2
#define INHNO_SERIAL2_IN   RXI_4
#define INHNO_SERIAL2_OUT  TXI_4
#define INHNO_SERIAL2_ERROR ERI_4
#endif /* TNUM_PORT >= 2 */

/*
 * SCIの割り込みレベル
 * 　割込みハンドラは送受信別々に登録できるが、割込みレベルは
 * 　送信も受信も同じレベルにしか設定できない。
 */
#define LEVEL0		0			/*  割込み解除時のレベル  */

#ifndef _MACRO_ONLY

/*
 *  SIOドライバの初期化ルーチン
 */
#define	sio_initialize	sh7047scif_initialize

/*
 *  シリアルI/Oポートのオープン
 */
Inline SIOPCB *
sio_opn_por(ID siopid, VP_INT exinf)
{
	SIOPCB	*siopcb;
	BOOL	openflag;

	/*
	 *  オープンしたポートがあるかを openflag に読んでおく．
	 */
	openflag = sh7047scif_openflag (siopid);

	/*
	 *  デバイス依存のオープン処理．
	 */
	siopcb = sh7047scif_opn_por(siopid, exinf);

	/*
	 *  割込みコントローラ依存
	 *  　シリアルI/O割込みの割込みレベルを設定する
	 */
	if (!openflag) {
		define_int_plevel ((VH*)siopcb->siopinib->intctl, siopcb->siopinib->level, siopcb->siopinib->shift);
	}
	return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
Inline void
sio_cls_por(SIOPCB *siopcb)
{
	/*
	 *  デバイス依存のクローズ処理．
	 */
	sh7047scif_cls_por(siopcb);

	/*
	 *  シリアルI/O割込みレベルを最低レベルにする
	 */
	define_int_plevel ((VH*)siopcb->siopinib->intctl, LEVEL0, siopcb->siopinib->shift);

}

/*
 *  SIO/SCIFの割込みハンドラ
 */
#define	sio_handler_in	sh7047scif3_isr_in	/*  受信割込みハンドラ  */
#define	sio_handler_out	sh7047scif3_isr_out	/*  送信割込みハンドラ  */
#define	sio_handler_error sh7047scif3_isr_error	/*  受信エラー割込みハンドラ  */
#if TNUM_PORT >= 2
#define	sio_handler2_in		sh7047scif4_isr_in	/*  受信割込みハンドラ  */
#define	sio_handler2_out	sh7047scif4_isr_out	/*  送信割込みハンドラ  */
#define	sio_handler2_error	sh7047scif4_isr_error	/*  受信エラー割込みハンドラ  */
#endif /* TNUM_PORT >= 2 */

/*
 *  シリアルI/Oポートへの文字送信
 */
#define	sio_snd_chr	sh7047scif_snd_chr

/*
 *  シリアルI/Oポートからの文字受信
 */
#define	sio_rcv_chr	sh7047scif_rcv_chr

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
#define	sio_ena_cbr	sh7047scif_ena_cbr

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
#define	sio_dis_cbr	sh7047scif_dis_cbr

/*
 *  シリアルI/Oポートからの送信可能コールバック
 *  　　jsp/systask/serial.cにある実体をリネームしている
 *  　　（一種の前方参照）
 */
#define	sio_ierdy_snd	sh7047scif_ierdy_snd

/*
 *  シリアルI/Oポートからの受信通知コールバック
 *  　　jsp/systask/serial.cにある実体をリネームしている
 *  　　（一種の前方参照）
 */
#define	sio_ierdy_rcv	sh7047scif_ierdy_rcv

#endif /* _MACRO_ONLY */
#endif /* _HW_SERIAL_H_ */
