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
 *  @(#) $Id: vector_table.c,v 1.3 2005/07/06 00:45:07 honda Exp $
 */

/*
 *	例外ベクタの定義（COLDFIRE用）
 */

#include "jsp_kernel.h"


#ifdef KERNEL_HAS_A_VECTOR_TABLE

static VP _current_vbr = EXCVT_ORIG;

extern void start(void);		/*  start.S  */
extern void no_reg_exception(void);	/*  cpu_support.S  */

VP
current_vbr()
{
        return _current_vbr;
}

void
set_vbr(VP vbr)
{
        _current_vbr = vbr;
	Asm("movec %0, %%vbr" : : "r"(vbr));
}

FP vector_table [192] = {
	(FP)INIT_STACK,		/*   0:スタックポインタの初期値  */
        RESET_VECTOR,		/*   1:パワーオンリセット  */
	ACC_VECTOR,		/*   2:アクセスエラー      */
  	ADD_VECTOR,		/*   3:アドレスエラー      */
  	II_VECTOR,		/*   4:一般不当命令        */
	RESERVED_VECTOR,	/*   5:d zero              */
	RESERVED_VECTOR,        /*   6:予約                */
        II_VECTOR,              /*   7:システム予約        */
	RESERVED_VECTOR,	/*   8:pvシステム予約        */
        RESERVED_VECTOR,        /*   9:trace               */
	RESERVED_VECTOR,	/*  10:line a emulator     */
	RESERVED_VECTOR,	/*  11:line f emulator     */
	RESERVED_VECTOR,	/*  12:システム予約        */
        RESERVED_VECTOR,        /*  13:システム予約        */
	RESERVED_VECTOR,	/*  14:システム予約        */
	RESERVED_VECTOR,	/*  15:システム予約        */
	RESERVED_VECTOR,	/*  16:システム予約        */
        RESERVED_VECTOR,        /*  17:システム予約        */
	RESERVED_VECTOR,	/*  18:システム予約        */
	RESERVED_VECTOR,	/*  19:システム予約        */
	RESERVED_VECTOR,	/*  20:システム予約        */
        RESERVED_VECTOR,        /*  21:システム予約        */
	RESERVED_VECTOR,	/*  22:システム予約        */
	RESERVED_VECTOR,	/*  23:システム予約        */
	RESERVED_VECTOR,	/*  24:スプリアス割り込み  */
				/*  残りはデフォルトの例外処理  */
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
	no_reg_exception,
};

#endif /* KERNEL_HAS_A_VECTOR_TABLE */

