/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
#   Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
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
 *  @(#) $Id: m52235evb.h,v 1.2 2003/06/04 01:52:23 hiro Exp $
 */

/*
 *  フリースケール・セミコンダクタ M52235EVB
 *  ボード固有の定義
 */

#ifndef _M52235EVB_H_
#define _M52235EVB_H_
#include "m52235.h"
/*
 *  割込みレベル設定のための定義
 */
#define	TIRQ_NMI	0x7u		/* ノンマスカブル割込み */
#define	TIRQ_LEVEL6	0x6u		/* 割込みレベル6 */
#define	TIRQ_LEVEL5	0x5u		/* 割込みレベル5 */
#define	TIRQ_LEVEL4	0x4u		/* 割込みレベル4 */
#define	TIRQ_LEVEL3	0x3u		/* 割込みレベル3 */
#define	TIRQ_LEVEL2	0x2u		/* 割込みレベル2 */
#define	TIRQ_LEVEL1	0x1u		/* 割込みレベル1 */

#ifndef _MACRO_ONLY
/*
 *  GDB STUB呼出しルーチン
 */

extern void mcfuart_putc_pol(ID portid, char c);	/*  mcfuart.c  */

#ifdef GDB_STUB

#define cfv2_exit	gdb_stub_exit
#define cfv2_putc	gdb_stub_putc

Inline void
gdb_stub_exit (void)
{
	/* テストしていない */
	Asm("trap #3"::);
}

Inline int
gdb_stub_putc (ID portid, int c)
{
	mcfuart_putc_pol (portid, c);
	return(c);
}

#else /* GDB_STUB */

Inline void
cfv2_exit ()
{
	while (1);
}

Inline void
cfv2_putc (ID portid, char c)
{
	mcfuart_putc_pol (portid, c);
}

#endif /* GDB_STUB */
#endif /* _MACRO_ONLY */
#endif /* _M52235EVB_H_ */
