/*
 *  rommon Simple Monitor
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2016-2022 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: $
 */

/*
 *	rommon Common Functions
 */

#include "comdefs.h"
#include "serial.h"

#define BS      0x08
#define LF      0x0a
#define CR      0x0d

char InputLine[MAX_SERIAL][DEFAULT_LINE_SIZE+1];

static char *InLinePointer[MAX_SERIAL];
static unsigned char InCharCount[MAX_SERIAL];


/*
 *	check Hexadecimal number in ASCII
 *  Input:	arg1: Input Line
 *  Output:	rval: Hexadecimal number, 0xff is not Hex
 */
static int IsHexASCII(char *s)
{
	char c = *s;

	if(c >= 'a')
		c -= 'a' - 'A';
	if(c >= '0' && c <= '9')
		return (c - '0');
	else if(c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	else
		return 0xff;
}

/*
 *	Hexadecimal ASCII to long word
 *  Input:	arg1: pointer to Input Line
 *          arg2: numbers of Input Line
 *  Output:	rval: long word
 */
unsigned long HexASCII2Long(char **ps, int size)
{
	unsigned long value = 0;
	int           c;

	while((c = IsHexASCII(*ps)) != 0xff && size > 0){
		value = (value << 4) + c;
		(*ps)++;
		size--;
	}
	return value;
}

/*
 *	Skip Blanks
 *  Input:  arg1: pointer to Input Line
 */
void SkipBlanks(char **ps)
{
	while(*(*ps) != 0){
		if(*(*ps) != ' ')
			break;
		(*ps)++;
	}
}

/*
 *	Send Character with count
 *  Input:  arg1: Character sent
 *          arg2: Count
 */
void SendCharCount(char c, int count)
{
	while(count > 0){
		apl_putchar(c);
		count--;
	}
}

/*
 *	Initialize Recive Line
 *  Input:  arg1: Serial Channel #
 */
void InitRecLine(int id)
{
	InCharCount[id]   = 0;
	InLinePointer[id] = &InputLine[id][0];
}

/*
 *	Recive Line
 *  Input:  arg1: send Serial Channel #
 *          arg2: recvie Serial Channel #
 *  output: rval: Input Buffer, 0 if no line
 */
char *ReciveLine(int sid, int rid)
{
	char *sp = &InputLine[rid][0];
	char *cp = InLinePointer[rid];
	char *rp = 0;
	char c;

	if(InCharCount[rid] == 0){
		do{
			c = ReciveChar(rid);
			if(c == BS){
				if(cp > sp){
					SendChar(sid, BS);
					SendChar(sid, ' ');
					SendChar(sid, BS);
					cp--;
				}
			}
			else if(c != CR && c != LF && c != 0){
				SendChar(sid, c);
				*cp++ = c;
				if(cp >= (sp+DEFAULT_LINE_SIZE))
					c = CR;
			}
			if(c == CR){
				SendChar(sid, CR);
				SendChar(sid, LF);
				InCharCount[rid] = (unsigned char)(cp-sp);
				*cp = '\0';
				rp = sp;
				break;
			}
		}while(c != 0);
		InLinePointer[rid] = cp;
	}
	else
		rp = sp;
	return rp;
}

