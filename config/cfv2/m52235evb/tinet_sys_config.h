/*
 *  TINET (TCP/IP Protocol Stack)
 * 
 *  Copyright (C) 2001-2006 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
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
 *      用できない形で再配布する場合には，次の条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: $
 */

#ifndef _TINET_SYS_CONFIG_H_
#define _TINET_SYS_CONFIG_H_

/*
 *  データリンク層 (ネットワークインタフェース) に関する定義
 */

/*
 *  NIC (NE2000 互換) に関する定義
 */

#define NUM_IF_FEC_TXBUF		2	/* 送信バッファ数			*/
#define NUM_IF_FEC_RXBUF		1	/* 送信バッファ数			*/


#define TMO_IF_FEC_GET_NET_BUF	1	/* [ms]、受信用 net_buf 獲得タイムアウト	*/
					/* [s]、 送信タイムアウト			*/
#define TMO_IF_FEC_XMIT		(2*IF_TIMER_HZ)

/*#define IF_ED_CFG_ACCEPT_ALL		 マルチキャスト、エラーフレームも受信するときはコメントを外す。*/

/*
 *  イーサネット出力時に、NIC で net_buf を開放する場合に指定する。
 *
 *  注意: 以下の指定は、指定例であり、if_ed では、
 *        開放しないので、以下のコメントを外してはならない。
 */

/*#define ETHER_NIC_CFG_RELEASE_NET_BUF*/

/*
 *  FEC に関する定義
 */
#define FEC_BASE_ADDRESS	(IPSBAR + 0x001000)	/* FECのレジスタベースアドレス */
#define PHY_BASE_ADDRESS	(IPSBAR + 0x1E0000)	/* EPHYのレジスタベースアドレス */
#define FEC_PHY0                (0x00)

#define INHNO_FEC_XINF     87
#define INHNO_FEC_XINB     88
#define INHNO_FEC_UN       89
#define INHNO_FEC_RL       90
#define INHNO_FEC_RINF     91
#define INHNO_FEC_RINB     92
#define INHNO_FEC_MII      93
#define INHNO_FEC_LC       94
#define INHNO_FEC_HBERR    95
#define INHNO_FEC_GRA      96
#define INHNO_FEC_EBERR    97
#define INHNO_FEC_BABT     98
#define INHNO_FEC_BABR     99
#define INHNO_EPHY         100

#define TBIT_FEC_XINF     23
#define TBIT_FEC_XINB     24
#define TBIT_FEC_UN       25
#define TBIT_FEC_RL       26
#define TBIT_FEC_RINF     27
#define TBIT_FEC_RINB     28
#define TBIT_FEC_MII      29
#define TBIT_FEC_LC       30
#define TBIT_FEC_HBERR    31
#define TBIT_FEC_GRA      32
#define TBIT_FEC_EBERR    33
#define TBIT_FEC_BABT     34
#define TBIT_FEC_BABR     35
#define TBIT_EPHY         36

/*
 *  JSP-1.4.2 以降では、割込みハンドラの登録方法が変更され、
 *  割込みプライオリティ・レベルを設定する必要がある。
 */
#define FEC_INT_LEVEL			5
#define FEC_INT_PRI			7

/*  
 * 割込みハンドラ実行中の割込みマスクの値
 * 　　他の割込みをマスクするための設定  
 * 　　自分と同じレベルの割込み要求をブロックするため、
 * 　　上記の割込み要求レベルより１つ高いレベルに設定する。
 */
#define if_fec_handler_intmask		(FEC_INT_LEVEL+1)

#ifndef _MACRO_ONLY

#if TKERNEL_PRVER < 0x1042u	/* JSP-1.4.2 以前 */

/*
 *  JSP-1.4.2 以前では、IPM をエラーが発生しないように定義する。
 */

typedef UINT	IPM;

#else	/* of #if TKERNEL_PRVER < 0x1042u */

#define fec_ena_inter(ipm)	chg_ipm(ipm)

#endif	/* of #if TKERNEL_PRVER < 0x1042u */

/*
 *  関数
 */

#if TKERNEL_PRVER < 0x1042u	/* JSP-1.4.2 以前 */

extern void fec_ena_inter (IPM ipm);

#endif	/* of #if TKERNEL_PRVER < 0x1042u */

extern IPM fec_dis_inter (void);
extern void fec_bus_init (void);
extern void fec_inter_init (void);

#endif	/* of #ifndef _MACRO_ONLY */

#endif /* _TINET_SYS_CONFIG_H_ */
