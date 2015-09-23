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
 *  @(#) $Id: cpu_config.c,v 1.1 2000/11/14 16:31:38 takayuki Exp $
 */

/*
 *	プロセッサ依存モジュール（Windows用）
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "hal_msg.h"
#include "vwindows.h"

CRITICAL_SECTION CPULock;
volatile char CPUStatus;

/*
 *  タスクコンテキストでの割込みマスク
 */
#ifdef SUPPORT_CHG_IMS
unsigned int task_sigmask;
#endif /* SUPPORT_CHG_IMS */

/*
 *  非タスクコンテキストでの割込みマスク
 */

void
dispatch()
{	HALDispatchRequest(0);	}


/*
 *  現在のコンテキストを捨ててディスパッチ
 *
 *  exit_and_dispatch は，CPUロック状態で呼び出さなければならない．
 */



void
exit_and_dispatch() 
{
	HALDestroyRequest(runtsk);
}


/*
 *  プロセッサ依存の初期化
 */

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

void
cpu_initialize()
{
	CPUStatus = 0;
	InitializeCriticalSection(&CPULock);

	/*
	 * 最上位例外ハンドラの設定
	 */
	ini_exc();
	SetUnhandledExceptionFilter(HALExceptionHandler);

	/*
	 *  割込みエミュレータの初期化
	 */
	ini_int();
}

/*
 *  プロセッサ依存の終了処理
 */
extern HANDLE PrimaryDialogHandle;

void
cpu_terminate()
{
	fin_int();
	SetUnhandledExceptionFilter((void *)0l);
	fin_exc();
	DeleteCriticalSection(&CPULock);
}


/*
 * タスク起動ルーチン
 *
 */

DWORD WINAPI
activate_r( LPVOID param)
{
	TCB * tcb = (TCB *)param;
	__try{
		(*tcb->tinib->task)(tcb->tinib->exinf);
	}
	__except( HALExceptionHandler(GetExceptionInformation()) )
	{}
	HALDestroyRequest(tcb);
	return 0;
}