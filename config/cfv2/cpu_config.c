/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *  @(#) $Id: cpu_config.c,v 1.10 2005/11/12 14:57:17 hiro Exp $
 */

/*
 *	プロセッサ依存モジュール（COLDFIRE用）
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

/*
 *  タスクコンテキストでの割込みマスク
 */
#ifdef SUPPORT_CHG_IPM
UH	task_intmask;
#endif /* SUPPORT_CHG_IPM */

/*
 *  非タスクコンテキストでの割込みマスク
 */
UH	int_intmask;

/*
 *  割込み／CPU例外ネストカウンタ
 */
UW      intnest;

extern void board_leds_display (unsigned char number);
/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize()
{
	/*
	 *  タスクコンテキストでの割込みマスクの初期化
	 */
#ifdef SUPPORT_CHG_IPM
	task_intmask = 0x0000;
#endif /* SUPPORT_CHG_IPM */

	/*
	 *  割込み／CPU例外ネストカウンタの初期化
	 */
	intnest = 1u;
        
	/*
	 *  例外ベクタテーブルの初期化
	 */
#ifdef KERNEL_HAS_A_VECTOR_TABLE
        if ((UW)&vector_table != current_vbr()) {
	     set_vbr(vector_table);
        }
#endif /* KERNEL_HAS_A_VECTOR_TABLE */
}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
#ifdef KERNEL_HAS_A_VECTOR_TABLE
  if ((UW)&vector_table != EXCVT_ORIG) {  
	set_vbr(EXCVT_ORIG);
  }
#endif /* EXCVT_KERNEL */
}

#ifdef SUPPORT_CHG_IPM

/*
 *  割込みマスクの変更
 *
 *  chg_ipm を使って IPM を 7（NMI 以外のすべての割込みを禁止）に変更
 *  することはできない．NMI 以外のすべての割込みを禁止したい場合には，
 *  loc_cpu によりCPUロック状態にすればよい．
 *  IPM が 0 以外の時にも，タスクディスパッチは保留されない．IPM は，
 *  タスクディスパッチによって，新しく実行状態になったタスクへ引き継が
 *  れる．そのため，タスクが実行中に，別のタスクによって IPM が変更さ
 *  れる場合がある．JSPカーネルでは，IPM の変更はタスク例外処理ルーチ
 *  ンによっても起こるので，別のタスクによって IPM が変更されることに
 *  よって，扱いが難しくなる状況は少ないと思われる．IPM の値によってタ
 *  スクディスパッチを禁止したい場合には，dis_dsp を併用すればよい．
 */
SYSCALL ER
chg_ipm(IPM ipm)
{
	ER	ercd;

	LOG_CHG_IPM_ENTER(ipm);
	CHECK_TSKCTX_UNL();
	CHECK_PAR(0 <= ipm && ipm <= 6);

	t_lock_cpu();
	task_intmask = (ipm << 8);
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	LOG_CHG_IPM_LEAVE(ercd);
	return(ercd);
}

/*
 *  割込みマスクの参照
 */
SYSCALL ER
get_ipm(IPM *p_ipm)
{
	ER	ercd;

	LOG_GET_IPM_ENTER(p_ipm);
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	*p_ipm = (task_intmask >> 8);
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	LOG_GET_IPM_LEAVE(ercd, *p_ipm);
	return(ercd);
}

#endif /* SUPPORT_CHG_IPM */

/*
 * 登録されていない例外が発生すると呼び出される
 */
void cpu_experr(EXCSTACK *sp)
{
    syslog(LOG_EMERG, "Exception error occurs.");
    
    syslog(LOG_EMERG, "PC = 0x%08x SR = 0x%08x",
    			sp->pc, sp->sr);
    
    syslog(LOG_EMERG, "d0 = 0x%08x d1 = 0x%08x d2 = 0x%08x",
    			sp->d0, sp->d1, sp->d2);
    syslog(LOG_EMERG, "d3 = 0x%08x d4 = 0x%08x d5 = 0x%08x",
    			sp->d3, sp->d4, sp->d5);
    syslog(LOG_EMERG, "d6 = 0x%08x d7 = 0x%08x a0 = 0x%08x",
    			sp->d6, sp->d7, sp->a0);
    syslog(LOG_EMERG, "a2 = 0x%08x a3 = 0x%08x a3 = 0x%08x",
    			sp->a1, sp->a2, sp->a3);
    syslog(LOG_EMERG, "a4 = 0x%08x a5 = 0x%08x a6 = 0x%08x",
    			sp->a4, sp->a5, sp->a6);
    			/*  例外発生直前のスタックポインタの値  */
    syslog(LOG_EMERG, "a7 = 0x%08x", (sp->a7)+2*4);

    while(1);
}

