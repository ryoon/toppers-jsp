/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: tinet_sys_config.h,v 1.5 2004/09/03 15:39:08 honda Exp $
 */

#ifndef _TINET_SYS_CONFIG_H_
#define _TINET_SYS_CONFIG_H_

/*
 *  データリンク層 (ネットワークインタフェース) に関する定義
 */

/*
 *  NIC (NE2000 互換) に関する定義
 */

/* 注意: 16 ビットモードは未実装 */
#ifdef IF_ED_CFG_16BIT

#define NUM_IF_ED_TXBUF		2	/* 送信バッファ数			*/

#else	/* of #ifdef IF_ED_CFG_16BIT */

#define NUM_IF_ED_TXBUF		1	/* 送信バッファ数			*/

#endif	/* of #ifdef IF_ED_CFG_16BIT */

#define TMO_IF_ED_GET_NET_BUF	1	/* [ms]、受信用 net_buf 獲得タイムアウト	*/
					/* [s]、 送信タイムアウト			*/
#define TMO_IF_ED_XMIT		(2*IF_TIMER_HZ)

/*#define IF_ED_CFG_ACCEPT_ALL		 マルチキャスト、エラーフレームも受信するときはコメントを外す。*/

/*
 *  NIC (RTL8019AS) に関する定義
 */

#define ED_BASE_ADDRESS		0x00200000		/* NIC のレジスタベースアドレス */

#define INHNO_IF_ED		IRQ_EXT5		/* IRQ5 */
#define ED_IER_IP_BIT		H8IER_IRQ5E_BIT
#define ED_IPR			H8IPRA
#define ED_IPR_IP_BIT		H8IPR_IRQ5_BIT

#endif /* _TINET_SYS_CONFIG_H_ */
