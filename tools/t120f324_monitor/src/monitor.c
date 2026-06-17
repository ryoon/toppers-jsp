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
 *	monitor.c -- rom Monitor Core
 */

#include "comdefs.h"
#include "mondefs.h"
#include "serial.h"

extern const void (*CommandVector[])(int id, char **s);

char *AddressPointer[MAX_SERIAL];
char *DumpPointer[MAX_SERIAL];
unsigned char CommandMode[MAX_SERIAL];
unsigned char Status[MAX_SERIAL];

static unsigned char DataSize[MAX_SERIAL];

/*
 *	Check Sum
 *	Input:	er3: Input Line + 1
 *	Output:	r0l: Check Sum, 0 if no error
 */
static unsigned char CheckSum(char *s)
{
	int len;
	unsigned int Sum = 0;

	len = HexASCII2Long(&s, 2);
	s -= 2;
	do{
		Sum += HexASCII2Long(&s, 2);
		len--;
	}while(len >= 0);
	return Sum;
}

/*
 *	Put Error Status
 *  Input:  arg1: Serial Channel #
 *          arg2: Error Status
 */

static void PutErrorStatus(int id, char status)
{
	apl_putchar(ERROR_PROMPT);
	apl_puthex(2, status);
	apl_putchar('\n');
}

/*
 *	Enter Data
 *  Input:  arg1: Serial Channel #
 *          arg2: input line
 */
static void EnterData(int id, char *s)
{
	*((unsigned long *)AddressPointer[id]) = (unsigned long)HexASCII2Long(&s, 128);
	AddressPointer[id] += 4;
}

/*
 *	Parse Command (Core)
 *  Input:  arg1: Serial Channel #
 */
void ParseCmdCore(int id)
{
	char *s;
	char c;
	unsigned short cmd;
	int  i;

	if((s = ReciveLine(id, id)) != 0){
		if(*s == 0){			/* just return key */
			switch(CommandMode[id]){
			case CMDMODE_ENTER:	/* increment address pointer */
				AddressPointer[id] += DataSize[id];
				break;
			case CMDMODE_DUMP:
				DumpLongCmd(id, &s);
				break;
			default:
				break;
			}
		}
		else if(CommandMode[id] == CMDMODE_ENTER){
			if(*s != COMMAND_END)
				EnterData(id, s);
			else
				CommandMode[id] = CMDMODE_ADDR;
		}
		else{
			c = *s++;
			cmd = (c << 8) | (unsigned char)*s;
			if(*s != 0)
				s++;
			for(i = 0 ; CommandString[i] ; i++){
				if(CommandString[i] == cmd){
					SkipBlanks(&s);
					CommandVector[i](id, &s);
					if(Status[id] != 0)
						PutErrorStatus(id, Status[id]);
					SendPrompt(id);
					return;
				}
			}
			PutErrorStatus(id, ERR_NO_COMMAND);
		}
		SendPrompt(id);
	}
}

/*
 *	Send Monitor Prompt
 *  Input:  arg1: Serial Channel #
 */
void MonSendPrompt(int id)
{
	if(CommandMode[id] != CMDMODE_ENTER){
		apl_puthex(1, id);
		apl_putchar(MONITOR_PROMPT);
	}
	else{
		apl_puthex(8, (unsigned int)AddressPointer[id]);
		apl_putchar(':');
		apl_puthex(8, *((unsigned int *)AddressPointer[id]));
		apl_putchar(':');
	}
}

/*
 *	Enter Data (Long Word) Command
 *		Format: el [<addr>]
 *  Input:  arg1: Serial Channel #
 *          arg2: input line
 */
void EnterLongCmd(int id, char **ps)
{
	char *s;
	unsigned long *a;

	s = *ps;
	a = (unsigned long *)HexASCII2Long(ps, 128);
	if(s == *ps)
		a = (unsigned long *)AddressPointer[id];
	AddressPointer[id] = (char *)a;
	CommandMode[id] = CMDMODE_ENTER;
	DataSize[id] = DATA_SIZE_LONG;
}


/*
 *	Get Address and Length
 *
 *		Input:	er3: input line
 *			er4: Serial Channel #
 *		Output: er0: Length
 *			er1: Address
 *			er3: input line
 */

static int GetAddrLen(int id, char **ps, void **a)
{
	char *s;
	int  len;

	SkipBlanks(ps);
	s = *ps;
	*a = (void *)HexASCII2Long(ps, 128);
	if(s == *ps){			/* no address specified, default length */
		*a = (void *)DumpPointer[id];
		return DEF_DUMP_LENGTH;
	}
	else{
		SkipBlanks(ps);
		s = *ps;
		len = HexASCII2Long(ps, 128);
		if(s == *ps)		/* no length specified, default length */
			len = DEF_DUMP_LENGTH;
		return len;
	}
}

/*
 *	Dump Long Command
 *		Format: dl [<addr> [<len>]]
 *  Input:  arg1: Serial Channel #
 *          arg2: input line
 */
void DumpLongCmd(int id, char **ps)
{
	unsigned long *a;
	int           len, len2, blank;

	len = GetAddrLen(id, ps, (void **)&a);
	if(len > 0){
		a = (unsigned long *)((unsigned long)a & ~3);
		DumpPointer[id] = (char *)a;
		len  = (len + 3) & ~3;
		len2 = len;
		blank = (unsigned long)a & 15;
		do{
			apl_puthex(8, (unsigned long)a & ~15);
			SendCharCount(' ', (blank/4)*9+1);
			blank = 16 - blank;
			while(blank > 0 && len > 0){
				if(blank == 8 && len != len2)
					SendChar(id, '-');
				else
					SendChar(id, ' ');
				apl_puthex(8, *a);
				a++;
				blank -= 4;
				len   -= 4;
			}
			apl_putchar('\n');
		}while(len > 0);
	}
	CommandMode[id] = CMDMODE_DUMP;
	DataSize[id]    = DATA_SIZE_LONG;
}

/*
 *	Load Command
 *	Format: ld
 *	Input:  arg1: Serial Channel #, 0 or 1
 *          arg2: input line
 */
void LoadCommand(int id, char **v)
{
	unsigned long e = 0;
	char *ea = 0;
	char *sa = 0;
	char *a  = 0;
	char *s;
	char c;
	int  len, alen;
	unsigned char Sum, val;
	int  cnt=0;

	CommandMode[id] = CMDMODE_ADDR;
	do{
		do{
			InitRecLine(id);
			do{
				s = ReciveLine(DUMMY_MON_SERIAL, id);
			}while(s == 0);
		}while(*s++ != 'S');
		Sum = CheckSum(s+1);
		if(Sum != 0x00 && Sum != 0xff){
			cnt++;
			ea = a;
			if(Status[id] == 0)
				Status[id] = ERR_CHECK_SUM;	/* check sum error */
		}
		c = *s++;
		len = HexASCII2Long(&s, 2)-1;
		if(c == '1')
			alen = 4;
		else if(c == '2')
			alen = 6;
		else if(c == '3')
			alen = 8;
		else if(c == '7'){
			e = (unsigned long)HexASCII2Long(&s, 4);
			alen = 0;
		}
		else if(c == '8'){
			e = (unsigned long)HexASCII2Long(&s, 6);
			alen = 0;
		}
		else if(c == '9'){
			e = (unsigned long)HexASCII2Long(&s, 8);
			alen = 0;
		}
		else
			alen = 0;
		if(alen > 0){
			a = (char *)HexASCII2Long(&s, alen);
			if(sa == 0)
				sa = a;
			len -= alen/2;
			while(len > 0){
				val = HexASCII2Long(&s, 2);
				*a++ = val;
				len--;
			}
		}
	}while(c < '7' || c > '9');

	apl_printf("Start Address = ");
	apl_puthex(8, (unsigned int)sa);	/* start address */
	apl_printf("\nEnd   Address = ");
	apl_puthex(8, (unsigned int)a);		/* end address */
	apl_printf("\nEntry Address = ");
	apl_puthex(8, (unsigned int)e);
	if(Status[id] != 0){		/* error */
		apl_printf("\nError = ");
		apl_puthex(8, (unsigned int)ea);
	}
	apl_putchar('\n');
}

/*
 *	Go Command
 *	Format: go [<addr>]
 *	Input:  arg1: Serial Channel #, 0 or 1
 *          arg2: input line
 */
void GoCommand(int id, char **ps)
{
	char *s;
	void (*func)(void);

	s = *ps;
	func = (void (*)(void))HexASCII2Long(ps, 80);
#if defined(NOT_DEFVECTORTYPE)
	if(s == *ps)
		func = (void (*)(void))UserVector;
#elif !defined(RESET_VECTOR)
	if(s == *ps)
		func = (void (*)(void))UserVector[0];
#else
	if(s == *ps)
		func = (void (*)(void))UserVector[RESET_VECTOR];
#endif
	Disable();
	func();
	Disable();
	SetVector((void *)MonitorVector);
	Enable();
}

/*
 *	Help Command
 *	Command Format: ?
 *	Input:	arg1: Serial Channel #, 0 or 1
 *          arg2: input line
 */
void HelpCmd(int id, char **ps)
{
	char *s = (char *)HelpStrings;

	do{
		do{
			apl_putchar(*s++);
		}while(*s != 0);
		s++;
		apl_putchar('\n');
	}while(*s != 0);
}


