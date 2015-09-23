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
 *  @(#) $Id: cpu_config.c,v 1.5 2002/04/05 07:43:51 honda Exp $
 */

/*
 *	プロセッサ依存モジュール（Linux用）
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

/*
 *  タスクコンテキストでの割込みマスク
 */
#ifdef SUPPORT_CHG_IMS
IMS     task_sigmask;
#endif /* SUPPORT_CHG_IMS */

/*
 *  非タスクコンテキストでの割込みマスク
 */
IMS	int_sigmask;

void
dispatch()
{
    sigset_t waitmask;
    sigemptyset(&waitmask);
    sigaddset(&waitmask,SIGUSR1);
        if (!runtsk || (runtsk != schedtsk
                        && _setjmp(runtsk->tskctxb.env) == 0)) {
            while (!(runtsk = schedtsk)) {
                sigsuspend(&waitmask);
            }
            _longjmp(runtsk->tskctxb.env, 1);
        }else{
            calltex();
        }
}


/*
 *  現在のコンテキストを捨ててディスパッチ
 *
 *  exit_and_dispatch は，CPUロック状態で呼び出さなければならない．
 */

void
exit_and_dispatch() 
{                   
    runtsk = 0;
    dispatch();
}


/*
 *  プロセッサ依存の初期化
 */


extern void kernel_start();

void
cpu_initialize()
{

	/*
	 *  タスクコンテキストでの割込みマスクの初期化
	 */
#ifdef SUPPORT_CHG_IPM
    sigemptyset(&task_sigmask);
#endif /* SUPPORT_CHG_IPM */

    /*
     *  シグナルスタックを，プロセススタック上に取る．
     *  BSDのシグナルと異なりss.ss_flagsにSS_ONSTACKを
     *  書き込んでも反映されないため、タスク独立部を表す
     *  inSigStackを使用する。
     */


    /*
     *  ディスパッチ用のシグナルハンドラを設定．
     *  割り込みハンドラの出口でSIGUSR1を使用して呼び出す
     *  ことにする。
     */
	{
            struct sigaction dis_action;
            dis_action.sa_handler = dispatch;
            dis_action.sa_flags   =  0;
            sigfillset(&dis_action.sa_mask);
            sigaction(SIGUSR1,&dis_action,((void *)0));

	}
}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
}



#ifdef SUPPORT_CHG_IMS
/*
 *   IMASKの設定参照機能
 */
SYSCALL ER
chg_ims(IMS ims)
{

    if(sense_context()){
        if(!sigismember(&ims,SIGUSR1))
            return(E_PAR);
    }else{
        if(sigismember(&ims,SIGUSR1))
            return(E_PAR);
    }
    
    
    sigprocmask(SIG_SETMASK,&ims,0);    
	return(E_OK);
}

/*
 *  割込みマスクの参照
 */
SYSCALL ER
get_ims(IMS *p_ims)
{
    sigprocmask(SIG_SETMASK,p_ims,0);        
	return(E_OK);
}


#endif /* SUPPORT_CHG_IMS */


/*
 * タスク起動ルーチン
 *
 */

void
activate_r()
{
    /*
     *  シグナルマスクを設定して，タスクを起動する．
     */
#ifdef SUPPORT_CHG_IMS
    sigprocmask(SIG_SETMASK,&task_sigmask,((void *)0));
#else /* SUPPORT_CHG_IMS */
    sigset_t set;
    sigemptyset(&set);
    sigprocmask(SIG_SETMASK,&set,((void *)0));
#endif /* SUPPORT_CHG_IMS */
    (*runtsk->tinib->task)(runtsk->tinib->exinf);
    
    ext_tsk();
}



