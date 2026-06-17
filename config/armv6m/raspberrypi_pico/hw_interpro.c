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
 *  @(#) $Id: hw_interpro.c 2246 2025-12-15 15:44:39Z roi $
 */

/*
 *  プロセッサ間割込みのシステム依存部（raspberrypi_pico用）
 */

#include <t_services.h>
#include <sys_config.h>
#include <interpro.h>
#include <hw_interpro.h>
#include <t_syslog.h>

#if TNUM_PRCID > 1		/* マルチコアのみ有効 */


/*
 *  IPIの初期化を行う
 */
void
interpro_init(void)
{
    sil_wrw_mem((VP)(TADR_SIO_FIFO_ST),
                SIO_FIFO_ST_ROE | SIO_FIFO_ST_WOF);
	x_ipi_clear_int(INTNO_IPI_PRC1);
	x_ipi_clear_int(INTNO_IPI_PRC2);
}

/*
 *  プロセッサ間割込み終了関数
 */
void
interpro_term(void)
{
	if(x_prc_index() == 0){
		x_config_int(INHNO_IPI_PRC1, FALSE, INTPRI_IPI_PRC);
	}
	else{
		x_config_int(INHNO_IPI_PRC2, FALSE, INTPRI_IPI_PRC);
	}
}

#endif	/* TNUM_PRCID > 1 */

/*
 *  プロセッサ間割込み割込みハンドラ
 */
void
interpro_int_handler(void)
{
#if TNUM_PRCID > 1		/* マルチコアのみ有効 */
	UW	event = x_ipi_event();
	ID	prcid = event >> 24;

	kernel_ipi_handler(prcid, (event & 0xffffu));
#endif	/* TNUM_PRCID > 1 */
}

