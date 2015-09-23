/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: cpu_config.c,v 1.5 2002/04/11 11:30:20 imai Exp $
 */


#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

#include "sh1.h"

/*
 *  プロセッサ依存モジュール（SH1用）
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
 *  割込み／CPU例外ネストカウンタ
 */
UW	intnest;


/*
 *  例外ベクタテーブル
 */
#ifdef EXCVT_KERNEL
EXCVE BASE_VBR[EXCVT_SIZE];
#endif	/*  EXCVT_KERNEL  */


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
	intnest = 1;

#ifndef WITH_STUB

	/*
	 * 割り込みコントローラの初期化
	 */
	*IPRA = 0x0000;
	*IPRB = 0x0000;
	*IPRC = 0x0000;
	*IPRD = 0x0000;
	*IPRE = 0x0000;

#endif	/*  WITH_STUB  */

#ifndef CQ_SH1_DEB
#ifdef EXCVT_KERNEL
	/*
	 *  ベクタテーブルの初期化
	 */
	memcpy(EXCVT_KERNEL, EXCVT_ORIG, EXCVT_SIZE*sizeof(VP));
	
	/*
	 *  ベクタベースレジスタの初期化
	 */
	set_vbr(EXCVT_KERNEL);
#endif /* EXCVT_KERNEL */
#endif /* CQ_SH1_DEB */

}



/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
#ifdef EXCVT_KERNEL
	set_vbr(EXCVT_ORIG);
#endif /* EXCVT_KERNEL */
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


#endif /* SUPPORT_CHG_IPM */


/*
 * 登録されていない例外が発生すると呼び出される
 */
void cpu_experr(EXCSTACK *sp)
{
    syslog(LOG_EMERG, "Exception error occurs.");

    syslog(LOG_EMERG, "PC = %08x SR = %08x PR = %08x",
    			sp->pc, sp->sr, sp->pr);

    syslog(LOG_EMERG, "r0 = %08x r1 = %08x r2 = %08x",
    			sp->r0, sp->r1, sp->r2);
    syslog(LOG_EMERG, "r3 = %08x r4 = %08x r5 = %08x",
    			sp->r3, sp->r4, sp->r5);
    syslog(LOG_EMERG, "r6 = %08x r7 = %08x r8 = %08x",
    			sp->r6, sp->r7, sp->r8);
    syslog(LOG_EMERG, "r9 = %08x r10 = %08x r11 = %08x",
    			sp->r9, sp->r10, sp->r11);
    syslog(LOG_EMERG, "r12 = %08x r13 = %08x r14 = %08x",
    			sp->r12, sp->r13, sp->r14);
    			/*  例外発生直前のスタックポインタの値  */
    syslog(LOG_EMERG, "r15 = %08x", (sp->r15)+19*4);

    while(1);
}


/*
 *  メモリブロック操作ライブラリ
 *	(ItIsからの流用)
 *
 *  関数の仕様は，ANSI C ライブラリの仕様と同じ．標準ライブラリのものを
 *  使った方が効率が良い可能性がある．
 *
 */
VP
_dummy_memcpy(VP dest, VP src, UINT len)
{
	VB	*d = dest;
	VB	*s = src;

	while (len-- > 0) {
		*d++ = *s++;
	}
	return(dest);
}
