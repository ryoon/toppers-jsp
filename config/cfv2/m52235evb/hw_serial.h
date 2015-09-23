/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Technology Inc
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
 *  @(#) $Id: hw_serial.h,v 1.9 2003/07/08 14:57:15 hiro Exp $
 */

/*
 *	シリアルI/Oデバイス（SIO/MCFUART)ドライバ（M52235EVB用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include <s_services.h>

#ifndef _MACRO_ONLY
#include "mcfuart.h"

/* 入出力ポートの設定はsys_config.c */
/* 割込みベクタ番号の設定はhw_serial.h */
/* 管理ブロックの設定はmcfuart.c */

#ifndef GDB_STUB

#define INHNO_SERIAL	 MCF_UART0
#if TNUM_PORT >= 2
#define INHNO_SERIAL2	 MCF_UART1
#endif /* TNUM_PORT >= 2 */

#else	/*  GDB_STUB  */

#define INHNO_SERIAL	 MCF_UART0

#endif	/*  GDB_STUB  */

/*
 *  SIOドライバの初期化ルーチン
 */
#define	sio_initialize	mcfuart_initialize

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
	openflag = mcfuart_openflag(siopid);

	/*
	 *  デバイス依存のオープン処理．
	 */
	siopcb = mcfuart_opn_por(siopid, exinf);

	/*
	 *  シリアルI/O割込みの割込みレベルを設定し，マスクを解除する．
	 */
	if (!openflag) {
            if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000200)) {
                sil_wrb_mem (MCF_INTC_ICR(MCF_INTC0, 13), siopcb->siopinib->int_level);
            } else if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000240)) {
                sil_wrb_mem (MCF_INTC_ICR(MCF_INTC0, 14), siopcb->siopinib->int_level);
            }
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
	mcfuart_cls_por(siopcb);

	/*
	 *  シリアルI/O割込みをマスクする．
	 */
        if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000200)) {
          sil_wrb_mem (MCF_INTC_ICR(MCF_INTC0, 13), 0);
        } else if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000240)) {
          sil_wrb_mem (MCF_INTC_ICR(MCF_INTC0, 14), 0);
        }
}

/*
 *  SIOの割込みハンドラ
 */
#define	sio_handler	mcfuart_isr
#if TNUM_PORT >= 2
#define	sio_handler2	mcfuart_isr2
#endif /* TNUM_PORT >= 2 */

/*
 *  シリアルI/Oポートへの文字送信
 */
#define	sio_snd_chr	mcfuart_snd_chr

/*
 *  シリアルI/Oポートからの文字受信
 */
#define	sio_rcv_chr	mcfuart_rcv_chr

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
#define	sio_ena_cbr	mcfuart_ena_cbr

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
#define	sio_dis_cbr	mcfuart_dis_cbr

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
#define	sio_ierdy_snd	mcfuart_ierdy_snd

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
#define	sio_ierdy_rcv	mcfuart_ierdy_rcv

#endif /* _MACRO_ONLY */
#endif /* _HW_SERIAL_H_ */
