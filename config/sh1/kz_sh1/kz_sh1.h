/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: kz_sh1.h,v 1.7 2003/12/18 06:34:40 honda Exp $
 */

/*
 *  京都マイクロコンピュータ	KZ-SH1-01
 *  CQ出版 RISC評価キット	SH-1
 *  のハードウェア資源の定義
 */

#ifndef _KZ_SH1_H_
#define _KZ_SH1_H_

/*
 *  GDB STUB呼出しルーチン（未完成）
 */
#ifndef _MACRO_ONLY
#ifdef GDB_STUB

#define kz_sh1_exit	stub_exit
#define kz_sh1_putc	stub_putc

Inline void
stub_exit(void)
{
}

Inline void
stub_putc(char c)
{
}

/*
 *  CQ出版 RISC評価キットのモニタ呼出しルーチン（未完成）
 */
#elif defined(CQ_SH1_DEB) 	/* GDB_STUB */

#define kz_sh1_exit	monitor_exit
#define kz_sh1_putc(c)	monitor_putc(c)

Inline void
monitor_exit()
{
	while(1);
}

int monitor_putc(int c);



/*
 *  GDB stubやモニタを使わない場合（ROM化用）
 */
#else /* GDB_STUB、CQ_SH1_DEB*/

Inline void
kz_sh1_exit()
{
	while(1);
}

extern void	sh1sci_putc_pol(char c);	/*  sh1sci.c  */

Inline void 
kz_sh1_putc(char c)
{
	sh1sci_putc_pol(c);
}

#endif /* GDB_STUB、CQ_SH1_DEB */
#endif /* _MACRO_ONLY */
#endif /* _KZ_SH1_H_ */
