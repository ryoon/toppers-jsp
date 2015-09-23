/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: syslog.c,v 1.1 2000/11/14 14:44:25 hiro Exp $
 */

/*
 *  システムログサービス
 */

#include "../kernel/jsp_kernel.h"
#include <jsp_services.h>

/*
 *  システムログ用のリングバッファとそれにアクセスするためのポインタ
 */

#define	SYSLOG_BUFSZ	1024

static char	syslog_buffer[SYSLOG_BUFSZ];
static int	syslog_write_ptr;
static int	syslog_read_ptr;
static int	syslog_mode;
static int	syslog_mask;

/*
 *  システムログサービスの初期化
 */
void
syslog_initialize()
{
	syslog_write_ptr = syslog_read_ptr = 0;
	syslog_mode = 0;
	syslog_mask = LOG_UPTO(LOG_NOTICE);
}     

/*
 *  システムログへの1文字出力
 */
static void
syslog_putc(char c)
{
	switch (syslog_mode) {
	default:
		sys_putc(c);
		break;
	case 1:
		syslog_buffer[syslog_write_ptr] = c;
		if (++syslog_write_ptr >= SYSLOG_BUFSZ) {
			syslog_write_ptr = 0;
		}
		if (syslog_read_ptr == syslog_write_ptr) {
			if (++syslog_read_ptr >= SYSLOG_BUFSZ) {
				syslog_read_ptr = 0;
			}
		}
		break;
	}
}

/*
 *  システムログ出力用簡易版 vprintfライブラリ
 *
 *  LONG_TYPE が定義されていると，"l" の指定で LONG_TYPE に定義された
 *  型で数値を取り出す．LONGLONG_TYPE が定義されていると，"ll" の指定で 
 *  LONGLONG_TYPE に定義された型で数値を取り出す．
 */

/*
 *  扱うべき最も長い整数型を LONGEST（符号付き）と ULONGEST（符号無し）
 *  に型定義する．
 */
#ifdef LONGLONG_TYPE
typedef LONGLONG_TYPE		LONGEST;
typedef unsigned LONGLONG_TYPE	ULONGEST;
#else /* LONGLONG_TYPE */
#ifdef LONG_TYPE
typedef LONG_TYPE		LONGEST;
typedef unsigned LONG_TYPE	ULONGEST;
#else /* LONG_TYPE */
typedef int			LONGEST;
typedef unsigned int		ULONGEST;
#endif /* LONG_TYPE */
#endif /* LONGLONG_TYPE */

/*
 *  数値を文字列に変換
 */
static void
convert(ULONGEST val, int radix, const char *radchar,
		int width, int minus, int padzero)
{
	char	buf[24];
	int	i, j;

	i = 0;
	do {
		buf[i++] = radchar[val % radix];
		val /= radix;
	} while (val != 0);
	if (minus) {
		buf[i++] = '-';
	}

	for (j = i; j < width; j++) {
		syslog_putc((char)(padzero ? '0' : ' '));
	}
	while (i > 0) {
		syslog_putc(buf[--i]);
	}
}

static char const raddec[] = "0123456789";
static char const radhex[] = "0123456789abcdef";
static char const radHEX[] = "0123456789ABCDEF";

/*
 *  引数を取り出すためのマクロ
 */
#ifdef LONGLONG_TYPE
#ifdef LONG_TYPE
#define GET_ARG	((longflag > 1) ? va_arg(ap, LONGLONG_TYPE) \
			(longflag ? va_arg(ap, LONG_TYPE) : va_arg(ap, int)))
#else /* LONG_TYPE */
#define GET_ARG	((longflag > 1) ? va_arg(ap, LONGLONG_TYPE) : va_arg(ap, int))
#endif /* LONG_TYPE */
#else /* LONGLONG_TYPE */
#ifdef LONG_TYPE
#define GET_ARG	(longflag ? va_arg(ap, LONG_TYPE) : va_arg(ap, int))
#else /* LONG_TYPE */
#define GET_ARG	(va_arg(ap, int))
#endif /* LONG_TYPE */
#endif /* LONGLONG_TYPE */

/*
 *  システムログへの vprintf
 */
static void
syslog_vprintf(const char *format, va_list ap)
{
	int	c;
	LONGEST	val;
	int	width;
	int	padzero;
	int	longflag;
	char	*str;

	while ((c = *format++) != '\0') {
		if (c != '%') {
			syslog_putc((char) c);
			continue;
		}

		width = padzero = longflag = 0;

		if ((c = *format++) == '0') {
			padzero = 1;
			c = *format++;
		}
		while ('0' <= c && c <= '9') {
			width = width*10 + c - '0';
			c = *format++;
		}
		while (c == 'l') {
			longflag++;
			c = *format++;
		}
		switch (c) {
		case 'd':
			val = GET_ARG;
			if (val >= 0) {
				convert(val, 10, raddec, width, 0, padzero);
			}
			else {
				convert(-val, 10, raddec, width, 1, padzero);
			}
			break;
		case 'u':
			val = GET_ARG;
			convert(val, 10, raddec, width, 0, padzero);
			break;
		case 'x':
			val = GET_ARG;
			convert(val, 16, radhex, width, 0, padzero);
			break;
		case 'X':
			val = GET_ARG;
			convert(val, 16, radHEX, width, 0, padzero);
			break;
		case 'c':
			syslog_putc((char) va_arg(ap, int));
			break;
		case 's':
			str = va_arg(ap, char *);
			while ((c = *str++) != '\0') {
				syslog_putc((char) c);
			}
			break;
		case '%':
			syslog_putc('%');
			break;
		case 0:
			format--;
			break;
		default:
			break;
		}
	}
}

/* 
 *  システムログ出力関数
 *
 *  カーネル内からも呼び出されるため，CPUロック状態の操作には，カーネ
 *  ル用のルーチンを用いている．
 */
void
syslog(int priority, const char *format, ...)
{
	va_list	ap;

	if (syslog_mask & LOG_MASK(priority)) {
		/*
		 *  LOG_EMERG で呼び出された場合，出力先を低レベルの出
		 *  力ルーチンに切り替える．
		 */
		if (priority == LOG_EMERG) {
			syslog_mode = 0;
		}

		/*
		 *  CPUロック状態や実行コンテキストによらず動作できる
		 *  ようにする．
		 */
		va_start(ap, format);
		if (sense_lock()) {
			syslog_vprintf(format, ap);
			syslog_putc('\n');
		}
		else if (sense_context()) {
			i_lock_cpu();
			syslog_vprintf(format, ap);
			syslog_putc('\n');
			i_unlock_cpu();
		}
		else {
			t_lock_cpu();
			syslog_vprintf(format, ap);
			syslog_putc('\n');
			t_unlock_cpu();
		}
		va_end(ap);
	}
}

/* 
 *  ログマスクの設定
 */
int
setlogmask(int maskpri)
{
	int	old_maskpri;

	old_maskpri = syslog_mask;
	syslog_mask = maskpri;
	return(old_maskpri);
}

/*
 *  リングバッファからの読み出し
 */
int
syslog_read()
{
	int	c;

	if (sns_ctx() || sns_loc()) {
		return(-1);
	}

	loc_cpu();
	if (syslog_read_ptr == syslog_write_ptr) {
		c = -1;
	}
	else {
		c = (unsigned char) syslog_buffer[syslog_read_ptr];
		if (++syslog_read_ptr >= SYSLOG_BUFSZ) {
			syslog_read_ptr = 0;
		}
	}
	unl_cpu();
	return(c);
}

/* 
 *  ログモードの変更
 */
int
setlogmode(int mode)
{
	int	old_mode;

	old_mode = syslog_mode;
	syslog_mode = mode;
	return(old_mode);
}
