/*
 *  TOPPERS BASE PLATFORM MIDDLEWARE
 * 
 *  Copyright (C) 2008-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2015-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  @(#) $Id: scanf.c,v 1.2 2017/05/27 18:31:06 roi Exp $
 */

/*
 *	scanf代用プログラム
 */

#include <stdarg.h>
#include <stdio.h>

typedef	void	*VP;		/* 型が定まらないものへのポインタ */
typedef	VP		VP_INT;		/* VP または INT */

#define	BOOL	int
#define TRUE	1
#define FALSE	0
#define FHALF   0
#define FINT    1
#define FLONG   2

static BOOL is_terminator(char c)
{
	if(c == '\0')
		return TRUE;
	else if(c == '\r' || c == '\n' || c == 0xc)
		return TRUE;
	else
		return FALSE;
}

/*
 *  文字列を数値に変換
 */
static int
convert(VP_INT p, int radix, int padzero, int width, int minus, int (*getsub)(char **p), char ** getbuf, int type)
{
	char	c;
	int		i;
	unsigned long val = 0;
	int count = 0;
	int csign = 0;

	if(width <= 0)
		width = 64;
	c = getsub(getbuf);				/* get first character */
	if(is_terminator((char)c))
		return count;
	if(c < '0' || (c > '9' && c < 'A')
			   || (c > 'Z' && c < 'a') || c > 'z'){
		c = getsub(getbuf);
		if(is_terminator((char)c))
			return count;
	}

	while(c == ' ' || c == '\t'){	/* skip white space */
		c = getsub(getbuf);
		if(is_terminator((char)c))
			return count;
	}

	if(padzero && c == '0'){		/* skip pad zero */
		do{
			c = getsub(getbuf);
			if(is_terminator((char)c))
				return count;
		}while(c == '0');
	}
	for( ; !is_terminator((char)c) ; c = getsub(getbuf), width--){
		if(c == '\t' || c == ' ' || c == ',')
			break;
		i = 0;
		if(c == '-' && minus){
			csign = 1;
			width++;
		}
		else if(c >= '0' && c <= '9')
			i = c - '0';
		else if(c >= 'A' && c <= 'Z')
			i = c - 'A' + 10;
		else if(c >= 'a' && c <= 'z')
			i = c - 'a' + 10;
		else
			continue;

		if(i >= radix)
			i = radix - 1;
		val = val * radix + i;
		count = 1;
		if(width <= 1)
			break;
	}
	if(csign)
		val = (unsigned long)((long)val * -1);

	switch(type){
	case FHALF:
		*((unsigned short *)p) = (unsigned short)val;
		break;
	case FLONG:
		*((unsigned long *)p) = (unsigned long)val;
		break;
	default:
		*((unsigned int *)p) = (unsigned int)val;
		break;
	}
	return count;
}


/*
 *  データ取り込みライブラリ関数
 */

int _getformat (int (*getsub)(char **p), char ** getbuf, const char * format, va_list ap)
{
	int c;
	int count = 0;
	int	width;
	int	padzero;
	VP_INT	val;
	char *str;

	while ((c = *format++) != '\0') {
		if (c != '%') {
			continue;
		}

		width = padzero = 0;
		c = *format++;
		if (c >= '1' && c <= '9') {
			padzero = 1;
		}
		while (c >= '0' && c <= '9') {
			width = width*10 + c - '0';
			c = *format++;
		}
		switch (c) {
		case 'l':						/* long */
			c = *format++;
			switch (c) {
			case 'd':					/* long 10進整数 */
			case 'i':					/* long 8,10,16進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 10, padzero, width, 1, getsub, getbuf, FLONG);
				break;
			case 'u':					/* unsigned long 10進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 10, padzero, width, 0, getsub, getbuf, FLONG);
				break;
			case 'o':					/* unsigned long 8進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 8, padzero, width, 0, getsub, getbuf, FLONG);
				break;
			case 'x':					/* unsigned long 16進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 16, padzero, width, 0, getsub, getbuf, FLONG);
				break;
			case 'X':					/* unsigned long 大文字16進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 16, padzero, width, 0, getsub, getbuf, FLONG);
				break;
			case '\0':
				format--;
				break;
			}
			break;
		case 'h':						/* short */
			c = *format++;
			switch (c) {
			case 'd':					/* short 8進整数 */
			case 'i':					/* short 8,10,16進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 10, padzero, width, 1, getsub, getbuf, FHALF);
				break;
			case 'u':					/* unsigned short 10進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 10, padzero, width, 0, getsub, getbuf, FHALF);
				break;
			case 'o':					/* unsigned short 8進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 8, padzero, width, 0, getsub, getbuf, FHALF);
				break;
			case 'x':					/* unsigned short 16進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 16, padzero, width, 0, getsub, getbuf, FHALF);
				break;
			case 'X':					/* unsigned short 大文字16進整数 */
				val = (VP_INT) va_arg(ap, VP_INT);
				count += convert(val, 16, padzero, width, 0, getsub, getbuf, FHALF);
				break;
			case '\0':
				format--;
				break;
			}
			break;
		case 'd':						/* int 10進整数 */
		case 'i':						/* int 8,10,16進整数 */
			val = (VP_INT) va_arg(ap, VP_INT);
			count += convert(val, 10, padzero, width, 1, getsub, getbuf, FINT);
			break;
		case 'u':						/* unsigned int 10進整数 */
			val = (VP_INT) va_arg(ap, VP_INT);
			count += convert(val, 10, padzero, width, 0, getsub, getbuf, FINT);
			break;
		case 'o':						/* unsigned int 8進整数 */
			val = (VP_INT) va_arg(ap, VP_INT);
			count += convert(val, 8, padzero, width, 0, getsub, getbuf, FINT);
			break;
		case 'x':						/* unsigned int 16進整数 */
			val = (VP_INT) va_arg(ap, VP_INT);
			count += convert(val, 16, padzero, width, 0, getsub, getbuf, FINT);
			break;
		case 'X':						/* unsigned int 大文字16進整数 */
			val = (VP_INT) va_arg(ap, VP_INT);
			count += convert(val, 16, padzero, width, 0, getsub, getbuf, FINT);
			break;

		case 'c':						/* 1バイト文字 */
			str = (char *) va_arg(ap, VP_INT);
			*str = getsub(getbuf);
			count++;
			break;
		case 's':						/* 文字列 */
			str = (char *) va_arg(ap, VP_INT);
			do{
				c = getsub(getbuf);
				if(c == ' ' || c == '\t')
					break;
				if(is_terminator((char)c))
					break;
				if(width > 0)
					*str++ = c;
				width--;
			}while(c != '\0');
			*str = '\0';
			count++;
			break;
		case '\0':
			format--;
			break;
		default:
			break;
		}
	}
	return count;
}

static int getzi (char ** p)
{
	return getchar();
}

int scanf (const char * format, ...)
{
    va_list args;
    va_start(args, format);
    return (_getformat(getzi, NULL, format, args));
}

