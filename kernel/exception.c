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
 *  @(#) $Id: exception.c,v 1.6 2002/03/26 08:19:38 hiro Exp $
 */

/*
 *	CPU例外管理機能
 */

#include "jsp_kernel.h"
#include "task.h"
#include "exception.h"

/*
 *  CPU例外ハンドラ番号の数（kernel_cfg.c）
 */
extern const UINT	tnum_excno;

/*
 *  CPU例外ハンドラ初期化ブロックのエリア（kernel_cfg.c）
 */
extern const EXCINIB	excinib_table[];

/* 
 *  CPU例外ハンドラ管理機能の初期化
 */
void
exception_initialize()
{
	UINT		i;
	const EXCINIB	*excinib;

	for (excinib = excinib_table, i = 0; i < tnum_excno; excinib++, i++) {
		define_exc(excinib->excno, excinib->exchdr);
	}
}

/*
 *  CPU例外の発生したコンテキストの参照
 */
SYSCALL BOOL
vxsns_ctx(VP p_excinf)
{
	return(exc_sense_context(p_excinf) ? TRUE : FALSE);
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
SYSCALL BOOL
vxsns_loc(VP p_excinf)
{
	return(exc_sense_lock(p_excinf) ? TRUE : FALSE);
}

/*
 *  CPU例外の発生した時のディスパッチ禁止状態の参照
 *
 *  sns_dsp と同一の処理内容となる．
 */
SYSCALL BOOL
vxsns_dsp(VP p_excinf)
{
	return(!(enadsp) ? TRUE : FALSE);
}

/*
 *  CPU例外の発生した時のディスパッチ保留状態の参照
 */
SYSCALL BOOL
vxsns_dpn(VP p_excinf)
{
	return((exc_sense_context(p_excinf) || exc_sense_lock(p_excinf)
					|| !(enadsp)) ? TRUE : FALSE);
}

/*
 *  CPU例外の発生した時のタスク例外処理禁止状態の参照
 *
 *  sns_tex と同一の処理内容となる．
 */
SYSCALL BOOL
vxsns_tex(VP p_excinf)
{
	return((runtsk != NULL && runtsk->enatex) ? FALSE : TRUE);
}
