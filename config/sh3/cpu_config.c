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
 *  @(#) $Id: cpu_config.c,v 1.1 2000/11/14 16:29:53 honda Exp $
 */


#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

#include "sh3.h"

/*
 *  プロセッサ依存モジュール（SH3用）
 */

/*
 *  タスクコンテキストでの割込みマスク
 */
#ifdef SUPPORT_CHG_IPM
UW	task_intmask;
#endif /* SUPPORT_CHG_IPM */

/*
 *  非タスクコンテキストでの割込みマスク
 */
UW	int_intmask;


/*
 * 割り込みハンドラ/優先度の疑似テーブル
 */
FP  int_table[0x99];
VW  int_plevel_table[0x99];

/*
 * CPU例外ハンドラの疑似テーブル
 */
FP	exc_table[0x1E];

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize()
{
    INT i;

    /*
     *  タスクコンテキストでの割込みマスクの初期化
     */
#ifdef SUPPORT_CHG_IPM
    task_intmask = 0x0000;    
#endif /* SUPPORT_CHG_IPM */

#ifndef WITH_STUB
    /*
     * 割り込みコントローラの初期化 
     */
    *ICR0 = 0x0000;     
    *ICR1 = 0x0000;
    *ICR2 = 0x0000;     
    *PINTER = 0x0000;     
    *IPRA = 0x0000;     
    *IPRB = 0x0000;
    *IPRC = 0x0000;
    *IPRD = 0x0000;
    *IPRE = 0x0000;
    *IRR0 = 0x0000;   
    *IRR1 = 0x0000;
    *IRR2 = 0x0000;
    
    /*
     *  ベクタベースレジスターの初期化
     */
    set_vbr(BASE_VBR);
#endif

    /*
     *  割り込みハンドラ/優先度疑似テーブルの初期化
     */
    for(i = 0; i < 0x99; i++){
        int_table[0x99] = 0;
        int_plevel_table[0x99]  = 0;
    }
    
    /*
     *  CPU例外疑似ベクタテーブルの初期化
     */
    for(i = 0; i < 0x1E; i++){
        exc_table[i] = 0;
    }
    
}



/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
}

#ifdef SUPPORT_CHG_IPM

/*
 *  割込みマスクの変更
 *
 *  chg_ipm を使って IPM を MAX_IPM （NMI スタブリモートブレーク 以外
 *  のすべての割込みを禁止）以上に変更することはできない．NMI スタブリ
 *  モートブレーク以外のすべての割込みを禁止したい場合には、loc_cpu に
 *  よりCPUロック状態にすればよい．IPM が 0 以外の時にも，タスクディス
 *  パッチは保留されない．IPM は，タスクディスパッチによって，新しく実
 *  行状態になったタスクへ引き継がれる．そのため，タスクが実行中に，別
 *  のタスクによって IPM が変更される場合がある．JSPカーネルでは，IPM
 *  の変更はタスク例外処理ルーチンによっても起こるので，これによって扱
 *  いが難しくなる状況は少ないと思われる．IPM の値によってタスクディス
 *  パッチを禁止したい場合には，dis_dsp を併用すればよい．
 */

SYSCALL ER
chg_ipm(IPM ipm)
{
	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= ipm && ipm <= MAX_IPM - 1);

	t_lock_cpu();
	task_intmask = (ipm << 4);
	t_unlock_cpu();
	return(E_OK);
}

/*
 *  割込みマスクの参照
 */
SYSCALL ER
get_ipm(IPM *p_ipm)
{
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_ipm = (task_intmask >> 4);
	t_unlock_cpu();
	return(E_OK);
}


            
/*
 * Trapa以外の例外で登録されていない例外が発生すると呼び出される
 */
void cpu_expevt(VW expevt,VW spc,VW ssr,VW pr)
{
    syslog(LOG_EMERG, "Expevt error occurs.");

    syslog(LOG_EMERG, "Expevt = %08x SPC = %08x SR = %08x PR=%08X",expevt,spc,
             ssr,pr);
    while(1);
    exit();
}



#endif /* SUPPORT_CHG_IPM */








