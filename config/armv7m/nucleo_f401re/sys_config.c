/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: sys_config.c 2246 2025-10-23 16:56:18Z roi $
 */

/*
 *		ターゲット依存モジュール（NUCLEO-F401RE用）
 */

#include "jsp_kernel.h"
#include <hw_timer.h>		/* システム・タイマー関係 */
#include <hw_serial.h>		/* デバックシリアルコントローラ関係 */


/*
 *  ベクターテーブル
 */
__attribute__ ((section(".vector"))) 
const FP vector_table[TMAX_INTNO + NUM_EXCNO + 1] = {
	(FP)STACKTOP,			/* 0 */
	(FP)(start),			/* 1 */
	(FP)(cpu_exc_entry),	/* 2 */
	(FP)(cpu_exc_entry),	/* 3 */
	(FP)(cpu_exc_entry),	/* 4 */
	(FP)(cpu_exc_entry),	/* 5 */
	(FP)(cpu_exc_entry),	/* 6 */
	(FP)(cpu_exc_entry),	/* 7 */
	(FP)(cpu_exc_entry),	/* 8 */
	(FP)(cpu_exc_entry),	/* 9 */
	(FP)(cpu_exc_entry),	/* 10 */
	(FP)(cpu_exc_entry),	/* 11 */
	(FP)(cpu_exc_entry),	/* 12 */
	(FP)(cpu_exc_entry),	/* 13 */
	(FP)(pendsvc_handler),	/* 14 */
 	(FP)(cpu_int_entry),	/* 15 */
 	(FP)(cpu_int_entry),	/* 0 */
 	(FP)(cpu_int_entry),	/* 1 */
 	(FP)(cpu_int_entry),	/* 2 */
 	(FP)(cpu_int_entry),	/* 3 */
 	(FP)(cpu_int_entry),	/* 4 */
 	(FP)(cpu_int_entry),	/* 5 */
 	(FP)(cpu_int_entry),	/* 6 */
 	(FP)(cpu_int_entry),	/* 7 */
 	(FP)(cpu_int_entry),	/* 8 */
 	(FP)(cpu_int_entry),	/* 9 */
 	(FP)(cpu_int_entry),	/* 10 */
 	(FP)(cpu_int_entry),	/* 11 */
 	(FP)(cpu_int_entry),	/* 12 */
 	(FP)(cpu_int_entry),	/* 13 */
 	(FP)(cpu_int_entry),	/* 14 */
 	(FP)(cpu_int_entry),	/* 15 */
 	(FP)(cpu_int_entry),	/* 16 */
 	(FP)(cpu_int_entry),	/* 17 */
 	(FP)(cpu_int_entry),	/* 18 */
 	(FP)(cpu_int_entry),	/* 19 */
 	(FP)(cpu_int_entry),	/* 20 */
 	(FP)(cpu_int_entry),	/* 21 */
 	(FP)(cpu_int_entry),	/* 22 */
 	(FP)(cpu_int_entry),	/* 23 */
 	(FP)(cpu_int_entry),	/* 24 */
 	(FP)(cpu_int_entry),	/* 25 */
 	(FP)(cpu_int_entry),	/* 26 */
 	(FP)(cpu_int_entry),	/* 27 */
 	(FP)(cpu_int_entry),	/* 28 */
 	(FP)(cpu_int_entry),	/* 29 */
 	(FP)(cpu_int_entry),	/* 30 */
 	(FP)(cpu_int_entry),	/* 31 */
 	(FP)(cpu_int_entry),	/* 32 */
 	(FP)(cpu_int_entry),	/* 33 */
 	(FP)(cpu_int_entry),	/* 34 */
 	(FP)(cpu_int_entry),	/* 35 */
 	(FP)(cpu_int_entry),	/* 36 */
 	(FP)(cpu_int_entry),	/* 37 */
 	(FP)(cpu_int_entry),	/* 38 */
 	(FP)(cpu_int_entry),	/* 39 */
 	(FP)(cpu_int_entry),	/* 40 */
 	(FP)(cpu_int_entry),	/* 41 */
 	(FP)(cpu_int_entry),	/* 42 */
 	(FP)(cpu_int_entry),	/* 43 */
 	(FP)(cpu_int_entry),	/* 44 */
 	(FP)(cpu_int_entry),	/* 45 */
 	(FP)(cpu_int_entry),	/* 46 */
 	(FP)(cpu_int_entry),	/* 47 */
 	(FP)(cpu_int_entry),	/* 48 */
 	(FP)(cpu_int_entry),	/* 49 */
 	(FP)(cpu_int_entry),	/* 50 */
 	(FP)(cpu_int_entry),	/* 51 */
 	(FP)(cpu_int_entry),	/* 52 */
 	(FP)(cpu_int_entry),	/* 53 */
 	(FP)(cpu_int_entry),	/* 54 */
 	(FP)(cpu_int_entry),	/* 55 */
 	(FP)(cpu_int_entry),	/* 56 */
 	(FP)(cpu_int_entry),	/* 57 */
 	(FP)(cpu_int_entry),	/* 58 */
 	(FP)(cpu_int_entry),	/* 59 */
 	(FP)(cpu_int_entry),	/* 60 */
 	(FP)(cpu_int_entry),	/* 61 */
 	(FP)(cpu_int_entry),	/* 62 */
 	(FP)(cpu_int_entry),	/* 63 */
 	(FP)(cpu_int_entry),	/* 64 */
 	(FP)(cpu_int_entry),	/* 65 */
 	(FP)(cpu_int_entry),	/* 66 */
 	(FP)(cpu_int_entry),	/* 67 */
 	(FP)(cpu_int_entry),	/* 68 */
 	(FP)(cpu_int_entry),	/* 69 */
 	(FP)(cpu_int_entry),	/* 70 */
 	(FP)(cpu_int_entry),	/* 71 */
 	(FP)(cpu_int_entry),	/* 72 */
 	(FP)(cpu_int_entry),	/* 73 */
 	(FP)(cpu_int_entry),	/* 74 */
 	(FP)(cpu_int_entry),	/* 75 */
 	(FP)(cpu_int_entry),	/* 76 */
 	(FP)(cpu_int_entry),	/* 77 */
 	(FP)(cpu_int_entry),	/* 78 */
 	(FP)(cpu_int_entry),	/* 79 */
 	(FP)(cpu_int_entry),	/* 80 */
 	(FP)(cpu_int_entry)		/* 81 */
};

/*
 *  ターゲット依存の初期化
 */
void
sys_initialize(void)
{
	/*
	 *  バナー出力用のシリアル初期化
	 */
	sio_init();
}

/*
 *  ターゲット依存の終了処理
 */
void
sys_exit(void)
{
	/*
	 *  開発環境依存の終了処理
	 */
	while(1);
}

/*
 *  システムログの低レベル出力のための文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		sio_snd_chr_pol('\r');
	}
	sio_snd_chr_pol(c);
}
