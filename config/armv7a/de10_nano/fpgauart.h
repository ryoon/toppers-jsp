/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: fpgauart.h 1019 2013-07-11 13:29:21Z ertl-honda $
 */

/*
 *   FPGA UART 用 簡易SIOドライバ
 */
#ifndef _FPGAUART_H_
#define _FPGAUART_H_

#include <fpga.h>

/*
 *  UART
 */
#define FUART_BASE	TADR_FPGAUART_BASE


#ifndef _MACRO_ONLY

/*
 * 文字を受信したか?
 */ 
Inline BOOL
fpgauart_getready(const SIOPINIB *siopinib)
{
	UW	status = sil_rew_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_STATUS));
	return (status & AVALON_UART_STATUS_RRDY) != 0;
}

/*
 * 文字を送信できるか?
 */
Inline BOOL
fpgauart_putready(const SIOPINIB *siopinib)
{
	UW	status = sil_rew_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_STATUS));
	return (status & AVALON_UART_STATUS_TRDY) != 0;
}

/*
 *  受信した文字の取り出し
 */
Inline UB
fpgauart_getchar(const SIOPINIB *siopinib)
{
	return (UB)sil_rew_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_RXDATA));
}

/*
 *  送信する文字の書き込み
 */
Inline void
fpgauart_putchar(const SIOPINIB *siopinib, UB c)
{
	sil_wrw_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_TXDATA), (UW)c);
}

/*
 *  送信割込み許可
 */
Inline void
fpgauart_enable_send(const SIOPINIB *siopinib)
{
	*siopinib->control |= AVALON_UART_CONTROL_TRDY;
	sil_wrw_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_CONTROL), *siopinib->control);
}

/*
 *  送信割込み禁止
 */
Inline void
fpgauart_disable_send(const SIOPINIB *siopinib)
{
	*siopinib->control &= ~AVALON_UART_CONTROL_TRDY;
	sil_wrw_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_CONTROL), *siopinib->control);
}

/*
 *  受信割込み許可
 */
Inline void
fpgauart_enable_rcv(const SIOPINIB *siopinib)
{
	*siopinib->control |= AVALON_UART_CONTROL_RRDY;
	sil_wrw_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_CONTROL), *siopinib->control);
}

/*
 *  受信割込み禁止
 */
Inline void
fpgauart_disable_rcv(const SIOPINIB *siopinib)
{
	*siopinib->control &= ~AVALON_UART_CONTROL_RRDY;
	sil_wrw_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_CONTROL), *siopinib->control);
}

/*
 * SIOPINIBの初期化
 */
Inline void
fpgauart_init_siopinib(const SIOPINIB *siopinib)
{
	*siopinib->control = AVALON_UART_CONTROL_RTS | AVALON_UART_CONTROL_RRDY | AVALON_UART_CONTROL_DCTS | AVALON_UART_CONTROL_TRDY;
	sil_wrw_mem((VP)(siopinib->reg_base + TOFF_FPGAUART_CONTROL), *siopinib->control);
}

#endif	/* _MACRO_ONLY */
#endif	/* _FPGAUART_H_ */
