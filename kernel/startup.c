/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: startup.c,v 1.7 2003/12/13 05:09:54 hiro Exp $
 */

/*
 *	カーネルの初期化と終了処理
 */

#include "jsp_kernel.h"
#include "task.h"
#include "time_event.h"
#include "syslog.h"

/*
 *  カーネル動作状態フラグ
 */
BOOL	iniflg;


#if TNUM_PRCID > 1

/*
 *  バリア同期用変数
 */
static volatile UB prc_init[TNUM_PRCID];
static volatile UB sys_start;

/*
 *  バリア同期
 */
static void
barrier_sync(UB phase)
{
	volatile UINT i, j;
	volatile UINT flag;

	prc_init[get_prc_index()] = phase;

	if (is_master_proc()) {
		do{
			flag = 0;
			for(i = 0; i < TNUM_PRCID; i++){
				if(prc_init[i] == phase){
					flag++;
				}
			}
			for(j = 0; j < 100; j++);
		}while (flag < TNUM_PRCID);
		sys_start = phase;
	}
	else {
		while(sys_start != phase) {
			for(j = 0; j < 100; j++);
		}
	}
}

#else	/* TNUM_PRCID > 1 */

#define	barrier_sync(p)

#endif	/* TNUM_PRCID > 1 */

/*
 *  カーネル初期化終了時のシステム設定関数
 */
Weak void
system_start(ID prcid)
{
}

/*
 *  カーネルの初期化
 */
void
kernel_start()
{
	/*
	 *  ターゲット依存の初期化
	 */
	cpu_initialize();
	sys_initialize();
	tool_initialize();

	/*
	 *  ジャイアントロックの初期化
	 */
	x_initialize_glock();

	/*
	 *  システムログ機能の初期化
	 */
	syslog_initialize();

	/*
	 *  カーネル・ライブラリィをチェック
	 */
	if (tnum_prcid != TNUM_PRCID) {
		if (is_master_proc()) {
			syslog(LOG_EMERG, "Illegal number of prossores for the kernel.");
		}
		kernel_abort();
	}

	/*
	 *  初期化ルーチンの実行
	 *
	 *  マスタプロセッサによるの実行が終了してから，スレーブプロセッサは実
	 *  行する．
	 */
	barrier_sync(1);
	t_acquire_pure_glock();

	/*
	 *  タイムイベント管理モジュールの初期化
	 *
	 *  タイムイベント管理モジュールは他のモジュールより先に初期化
	 *  する必要がある．初期化終了までsyslogの使用は不可．
	 */
	tmevt_initialize();

	/*
	 *  他のモジュールの初期化
	 */
	object_initialize();

	/*
	 *  起動メッセージの表示
	 */
	print_banner();

	/*
	 *  初期化ルーチンの実行
	 */
	if (is_master_proc()) {
		call_inirtn();
	}

	/*
	 *  オブジェクトの初期化待ちのバリア同期
	 */
	x_release_pure_glock();
	barrier_sync(2);

	/*
	 *  最終システム設定関数の呼び出し
	 */
	system_start(get_prc_index());
	iniflg = TRUE;

	/*
	 *  カーネル動作の開始
	 */
	exit_and_dispatch();
}

/*
 *  カーネルの終了処理
 */
void
kernel_exit()
{
	/*
	 *  割込みを禁止
	 */
	if (!sense_lock()) {
		if (sense_context()) {
			i_lock_cpu();
		}
		else  {
			t_lock_cpu();
		}
	}
	if (iniflg) {
		iniflg = FALSE;
		multi_kernel_exit();
	}

	barrier_sync(3);
	t_acquire_pure_glock();

	if (is_master_proc()) {

		/*
		 *  終了処理ルーチンの実行
		 */
		call_terrtn();

		/*
		 *  atexit の処理とデストラクタの実行
		 */
		call_atexit();
	}

	/*
	 *  システムログ機能の終了処理
	 */
	syslog_terminate();

	/*
	 *  ターゲット依存の終了処理
	 */
	cpu_terminate();
	x_release_pure_glock();
	sys_exit();
}
