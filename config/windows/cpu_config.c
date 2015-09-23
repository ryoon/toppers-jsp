/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: cpu_config.c,v 1.5 2002/04/10 11:20:09 takayuki Exp $
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

void
cpu_initialize()
{
	CPUStatus = 0;
	InitializeCriticalSection(&CPULock);

	/*
	 * 最上位例外ハンドラの設定
	 */
	ini_exc();

	/*
	 *  割込みエミュレータの初期化
	 */
	ini_int();
}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{	HALQuitRequest();	}


/*
 * タスク起動ルーチン
 *
 */

DWORD WINAPI
activate_r( LPVOID param)
{
	TCB * tcb = (TCB *)param;
	__try{
		CoInitialize(NULL);
		(*tcb->tinib->task)(tcb->tinib->exinf);
		CoUninitialize();
	}
	__except( HALExceptionHandler(GetExceptionInformation()) )
	{}
	HALDestroyRequest(tcb);
	return 0;
}