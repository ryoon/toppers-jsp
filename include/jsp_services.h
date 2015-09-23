/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: jsp_services.h,v 1.2 2001/09/11 03:52:27 hiro Exp $
 */

/*
 *	アプリケーション用 標準インクルードファイル
 *
 *  JSPカーネル上で動作するプログラムが標準的にインクルードすべきファ
 *  イル．標準的なシステムサービスを使うためのインクルードファイルを含
 *  む．
 */

#ifndef _JSP_SERVICES_H_
#define _JSP_SERVICES_H_

/*
 *  JSPカーネルを呼び出すために必要なインクルードファイル
 */
#include <jsp_stddef.h>
#include <kernel.h>

/*
 *  カーネルの終了処理
 */
extern void	kernel_exit(void);

/*
 *  シリアルインタフェースモジュール
 */
#include <serial.h>

/*
 *  システムログサービス
 */
#include <syslog.h>

/*
 *  syscallマクロ，_syscall マクロの定義
 */

extern void	jsp_perror(const char *file, int line,
				const char *expr, ER ercd);
extern void	jsp_panic(const char *file, int line,
				const char *expr, ER ercd);

Inline ER
_jsp_perror(const char *file, int line, const char *expr, ER ercd)
{
	if (MERCD(ercd) < 0) {
		jsp_perror(file, line, expr, ercd);
	}
	return(ercd);
}

Inline ER
_jsp_panic(const char *file, int line, const char *expr, ER ercd)
{
	if (MERCD(ercd) < 0) {
		jsp_panic(file, line, expr, ercd);
	}
	return(ercd);
}

#define	syscall(s)	_jsp_perror(__FILE__, __LINE__, #s, (s));
#define	_syscall(s)	_jsp_panic(__FILE__, __LINE__, #s, (s));

#endif /* _JSP_SERVICES_H_ */
