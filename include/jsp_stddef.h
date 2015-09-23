/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: jsp_stddef.h,v 1.6 2002/04/14 11:56:18 hiro Exp $
 */

/*
 *	JSPカーネル・アプリケーション 共通インクルードファイル
 */

#ifndef _JSP_STDDEF_H_
#define _JSP_STDDEF_H_

/*
 *  コンパイラの拡張機能のためのマクロ定義
 */

#ifdef __GNUC__

#define	inline		__inline__
#define	Inline		static __inline__
#define	asm		__asm__
#define	Asm		__asm__ volatile

#elif defined(_MSC_VER)

#define inline		__forceinline
#define	Inline		static __forceinline

#elif defined(_TMS320C5XX)

#define	inline
#define	Inline		static __inline

#else /* その他のコンパイラ */

#define inline		
#define	Inline		static

#endif /* その他のコンパイラ */

/*
 *  assertマクロの定義
 */
#ifndef NDEBUG
#define assert(exp)							\
{									\
	if (!(exp)) {							\
		syslog_3(LOG_EMERG, "%s:%u: Assertion `%s' failed.",	\
					__FILE__, __LINE__, #exp);	\
		kernel_exit();						\
	}								\
}
#else /* NDEBUG */
#define	assert(exp)
#endif /* NDEBUG */

#endif /* _JSP_STDDEF_H_ */
