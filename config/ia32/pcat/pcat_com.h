/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
 *  Copyright (C) 2008-     by Monami Software Limited Partnership, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

/*
 *	COMドライバ（PC/AT用）
 */

#ifndef _PCAT_COM_H_
#define _PCAT_COM_H_

#include <s_services.h>

#define SIO_ERDY_RCV (1u)
#define SIO_ERDY_SND (2u)

#ifndef _MACRO_ONLY

/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct {
  short base_address;
  short com_irq;
	UH divisor_latch;
} SIOPINIB;
typedef struct pcat_hardware_serial_port_descripter {
	const SIOPINIB *siopinib;
  INT	    initflag;	   /* 初期化済フラグへのポインタ   */
  VP_INT spcb;
} SIOPCB;

/*
 *  シリアルI/Oポートの初期化
 */
extern void pcat_com_initialize();

extern void pcat_com1_handler();
#if TNUM_PORT >= 2
extern void pcat_com2_handler();
#endif

extern SIOPCB *pcat_com_opn_por(ID portid, VP_INT spcb);

/*
 *  シリアルI/Oポートの終了
 */
extern void pcat_com_cls_por(SIOPCB *p);

extern void pcat_com_ierdy_rcv(VP_INT exinf);

extern void pcat_com_ierdy_snd(VP_INT exinf);

/*
 *  受信した文字の取り出し
 */
extern INT pcat_com_rcv_chr(SIOPCB *p);

/*
 *  送信する文字の書き込み
 */
extern BOOL pcat_com_snd_chr(SIOPCB *p, char c);

extern void pcat_com_ena_cbr(SIOPCB *siopcb, UINT cbrtn);

extern void pcat_com_dis_cbr(SIOPCB *siopcb, UINT cbrtn);

extern void pcat_com_init(ID siopid);

extern void pcat_com_putc(ID siopid, char c);

#endif /* _MACRO_ONLY */

#endif /* _PCAT_COM_H_ */
