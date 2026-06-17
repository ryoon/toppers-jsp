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
 *  @(#) $Id: hw_interpro.h 889 2025-12-11 22:30:20Z roi $
 */

/*
 * プロセッサ間割込みに関する定義（raspberrypi_pico用）
 */

#ifndef _HW_INTERPRO_H_
#define _HW_INTERPRO_H_

#include <s_services.h>		/* デバイスドライバ用標準インクルードファイル */

/*
 *  ディスパッチ要求プロセッサ間割込みの割込み番号
 */
#define INHNO_IPI_PRC1	IRQ_VECTOR_SIO_PROC0
#define INHNO_IPI_PRC2	IRQ_VECTOR_SIO_PROC1
#define INTNO_IPI_PRC1	INHNO_IPI_PRC1
#define INTNO_IPI_PRC2	INHNO_IPI_PRC2
#define INTPRI_IPI_PRC	2		/* 割込み優先度 */


#ifndef _MACRO_ONLY

#if TNUM_PRCID > 1		/* マルチコアのみ有効 */

/*
 *  プロセッサ間割込み初期化関数
 */
extern void interpro_init(void);

/*
 *  プロセッサ間割込み終了関数
 */
extern void interpro_term(void);

#else	/* TNUM_PRCID > 1 */

#define	interpro_init()
#define interpro_term()

#endif	/* TNUM_PRCID > 1 */

/*
 *  プロセッサ間割込み割込みハンドラ
 */
extern void interpro_int_handler(void);

#endif /* _MACRO_ONLY */

#endif /* _HW_INTERPRO_H_ */
