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
 *  @(#) $Id: vasyslog.c,v 1.4 2002/04/08 05:28:52 hiro Exp $
 */

/*
 *  可変数引数のシステムログライブラリ
 */

#include <jsp_services.h>
#include <stdarg.h>

ER
syslog(UINT prio, const char *format, ...)
{
	SYSLOG	syslog;
	va_list	ap;
	int	i;
	int	c;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	i = 1;
	va_start(ap, format);

	while ((c = *format++) != '\0' && i < TMAX_LOGINFO) {
		if (c != '%') {
			continue;
		}

		if ((c = *format++) == '0') {
			c = *format++;
		}
		while ('0' <= c && c <= '9') {
			c = *format++;
		}
		switch (c) {
		case 'd':
			syslog.loginfo[i++] = (VP_INT) va_arg(ap, int);
			break;
		case 'u':
		case 'x':
		case 'X':
			syslog.loginfo[i++] =
					(VP_INT) va_arg(ap, unsigned int);
			break;
		case 'c':
			syslog.loginfo[i++] = (VP_INT) va_arg(ap, int);
			break;
		case 's':
			syslog.loginfo[i++] = (VP_INT) va_arg(ap, char *);
			break;
		case '\0':
			format--;
			break;
		default:
			break;
		}
	}
	va_end(ap);
	return(syslog_write(prio, &syslog));
}
