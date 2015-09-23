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
 *  @(#) $Id: cpu_defs.c,v 1.2 2000/12/25 14:07:58 takayuki Exp $
 */

#include "hal_msg.h"
#include "cpu_defs.h"
#include "cpu_config.h"


/*
 * m68kっぽい割込みエミュレータ (Windows HAL)
 */

struct tagInterruptLevel InterruptLevel[INT_NUMINTERRUPTS];
CRITICAL_SECTION InterruptCriticalSection;
unsigned int CurrentInterruptLevel;

#define CHECK_IMS(x) ( (x) != 0 && (x) <= INT_NUMINTERRUPTS )

struct tagExceptionLevel ExceptionLevel[EXC_MAXITEMS];

static LPTOP_LEVEL_EXCEPTION_FILTER AnotherExceptionFilter;

static unsigned int
isns_int( unsigned int ipl )
{
	unsigned int result = INT_NUMINTERRUPTS+1;
	while( result > ipl)
	{
		result --;
		if( (InterruptLevel[result].Status & (INT_STAT_PENDING|INT_STAT_RUNNING)) != 0)
			return result+1;
	}
	while( result > 0)
	{
		result --;
		if( (InterruptLevel[result].Status & INT_STAT_RUNNING) != 0)
			return result+1;
	}
	return 0;
}

DWORD WINAPI
InterruptHandlerWrapper(LPVOID param)
{
	unsigned int i;
	unsigned int PrevLevel;
	struct tagInterruptLevel * intlv = (struct tagInterruptLevel *)param;

	while(1==1)
	{
			/* 割込み前処理 */
		EnterCriticalSection(&InterruptCriticalSection);
		PrevLevel = CurrentInterruptLevel;
		CurrentInterruptLevel = (unsigned int)(intlv - InterruptLevel)+1;
		intlv->Status &= ~INT_STAT_PENDING;
		intlv->Status |= INT_STAT_RUNNING;
		LeaveCriticalSection(&InterruptCriticalSection);

			/* 割込みハンドラの起動 */
		( (void (*)(void) )(intlv->Routine))();

			/* 割込み後処理 */
		EnterCriticalSection(&InterruptCriticalSection);
		intlv->Status &= ~INT_STAT_RUNNING;
		i = isns_int(PrevLevel);
		CurrentInterruptLevel = PrevLevel;
		LeaveCriticalSection(&InterruptCriticalSection);
		
			/* ディスパッチルーチンの起動 */
		if(i != 0)
			HALInterruptRequestAndWait(0);
		else
			HALDispatchRequest(NULL);
	}
	ExitThread(0);
	return 0;
}

BOOL
def_int(unsigned int ims, void * rtn)
{
	if(!CHECK_IMS(ims))
		return FALSE;

	ims--;
	
	EnterCriticalSection(&InterruptCriticalSection);
	if(InterruptLevel[ims].ThreadHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(InterruptLevel[ims].ThreadHandle,0);
		CloseHandle(InterruptLevel[ims].ThreadHandle);
	}
	InterruptLevel[ims].Routine = rtn;
	InterruptLevel[ims].ThreadHandle = CreateThread(NULL,0,InterruptHandlerWrapper,(LPVOID)&InterruptLevel[ims],CREATE_SUSPENDED,&InterruptLevel[ims].ThreadID);
	LeaveCriticalSection(&InterruptCriticalSection);
	return TRUE;
}

BOOL
ini_int(void)
{
	int i;

	InitializeCriticalSection(&InterruptCriticalSection);
	EnterCriticalSection(&InterruptCriticalSection);
	for(i=0;i<INT_NUMINTERRUPTS;i++)
	{
		InterruptLevel[i].Routine = (void *)0l;
		InterruptLevel[i].ThreadHandle = INVALID_HANDLE_VALUE;
		InterruptLevel[i].ThreadID = 0;
		InterruptLevel[i].Status = 0;
	}
	CurrentInterruptLevel = 0;
	LeaveCriticalSection(&InterruptCriticalSection);
	return TRUE;
}

void
fin_int(void)
{
	int i;
	EnterCriticalSection(&InterruptCriticalSection);
	for(i=0;i<INT_NUMINTERRUPTS;i++)
	{
		if(InterruptLevel[i].ThreadHandle != INVALID_HANDLE_VALUE)
		{
			TerminateThread(InterruptLevel[i].ThreadHandle,0);
			CloseHandle(InterruptLevel[i].ThreadHandle);
		}
		InterruptLevel[i].Routine = (void *)0l;
		InterruptLevel[i].ThreadHandle = INVALID_HANDLE_VALUE;
		InterruptLevel[i].ThreadID = 0;
		InterruptLevel[i].Status = 0;
	}
	CurrentInterruptLevel = 0;
	LeaveCriticalSection(&InterruptCriticalSection);
	//DeleteCriticalSection(&InterruptCriticalSection);
}

BOOL
ras_int(unsigned int ims)
{
	BOOL result;
	if(!CHECK_IMS(ims))
		return FALSE;

	ims --;

	EnterCriticalSection(&InterruptCriticalSection);
	if(InterruptLevel[ims].ThreadHandle == INVALID_HANDLE_VALUE)
	{
		result = FALSE;
	}else
		InterruptLevel[ims].Status = INT_STAT_PENDING;
	LeaveCriticalSection(&InterruptCriticalSection);
	return result;
}

unsigned int
sns_int( void )
{
	int result;
	EnterCriticalSection(&InterruptCriticalSection);
	result = isns_int(CurrentInterruptLevel);
	LeaveCriticalSection(&InterruptCriticalSection);
	return result;
}

HANDLE
sch_int( void )
{
	HANDLE result;
	unsigned int level;

	EnterCriticalSection(&InterruptCriticalSection);
	level = isns_int(CurrentInterruptLevel);
	if(level != 0)
		result = InterruptLevel[level-1].ThreadHandle;
	else
		result = INVALID_HANDLE_VALUE;
	LeaveCriticalSection(&InterruptCriticalSection);

	return result;
}

ER
ena_int(unsigned int ims)
{
	return chg_ims(0);
}

ER
dis_int(unsigned int ims)
{
	return chg_ims(INT_NUMINTERRUPTS+1);
}

ER
chg_ims(unsigned int ims)
{
	int i;
	if(!CHECK_IMS(ims))
		return -17 /*E_PAR*/;
	EnterCriticalSection(&InterruptCriticalSection);
	CurrentInterruptLevel = ims;
	i = isns_int(ims);
	LeaveCriticalSection(&InterruptCriticalSection);
	if(i != 0)
		HALInterruptRequest(0);
	return 0 /*E_OK*/;
}

ER
get_ims(unsigned int *p_ims)
{
	if(p_ims == (void *)0l)
		return -17 /*E_PAR*/;

	EnterCriticalSection(&InterruptCriticalSection);
	*p_ims = CurrentInterruptLevel;
	LeaveCriticalSection(&InterruptCriticalSection);
	return 0 /*E_OK*/;
}


/*
 * 最上位レベルWindows構造化例外ハンドラ

 */



LONG WINAPI
HALExceptionHandler( EXCEPTION_POINTERS * exc )
{
	int i;

	if((CPUStatus & CPU_STAT_EXC) == 0)
	{
		CPUStatus |= CPU_STAT_EXC;
		for(i=0;i<EXC_MAXITEMS;i++)
		{
			if(ExceptionLevel[i].ExceptionCode == exc->ExceptionRecord->ExceptionCode)
			{
				i = EXCEPTION_CONTINUE_SEARCH;
				( * ((void (*)(void *,int *))ExceptionLevel[i].Routine)) (exc,&i);
				return i;
			}
		}
		CPUStatus &= ~CPU_STAT_EXC;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL
ini_exc(void)
{
	int i;

	for(i=0;i<EXC_MAXITEMS;i++)
	{
		ExceptionLevel[i].ExceptionCode = 0;
		ExceptionLevel[i].Routine = 0l;
	}

	AnotherExceptionFilter = SetUnhandledExceptionFilter(HALExceptionHandler);
	
	return TRUE;
}

void
fin_exc(void)
{}

BOOL
def_exc(DWORD exc, void * routine)
{
	int j;
	int i;

	if(routine == 0l)
	{
		for(i=0;i<EXC_MAXITEMS;i++)
			if(ExceptionLevel[i].ExceptionCode == exc)
			{
				ExceptionLevel[i].ExceptionCode = 0;
				ExceptionLevel[i].Routine = 0;
				return TRUE;
			}
		return FALSE;
	}

	j = EXC_MAXITEMS;
	for(i=0;i<EXC_MAXITEMS;i++)
	{
		if(ExceptionLevel[i].ExceptionCode != 0)
		{
			if(ExceptionLevel[i].ExceptionCode == exc)
				return FALSE;
		}else
			if(j > i)
				j = i;
	}

		//This sequence will never change
	ExceptionLevel[j].Routine = routine;
	ExceptionLevel[j].ExceptionCode = exc;

	return TRUE;
}



