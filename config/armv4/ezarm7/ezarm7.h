/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005- by Monami software, LP.
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
 *  @(#) $Id$
 */

/*
 *   Optimize製EZARM7用の定義
 *   sys_config.hからインクルードされる
 */

#ifndef _EZARM7_H_
#define _EZARM7_H_

#include <lpc2214.h>

/*
 *  起動メッセージのターゲットシステム名
 */
#define    TARGET_NAME    "Optimize EZARM7"


/*
 *  JSPカーネル動作時のメモリマップと関連する定義
 *
 *  システムメモリマップ
 *
 *  On-Chip Non-volatile Memory : 0x00000000 - 0x0001FFFF   (128KB)
 *  On-chip SRAM0               : 0x40000000 - 0x40003fff   ( 16KB)
 *
 *  JSPでの割り当て
 *  0x00000000 - 0x0001ffff コード領域(128Kbyte)
 *  0x40000000 -            データ領域( 64Kbyte)
 *             - 0x4003ffff タスク独立部スタック
 */

#define   STACKTOP 0x40003ffc   /* 非タスクコンテキスト用のスタックの初期値 */

/*
 *  UARTのボーレート
 */
#define UART_BPS      115200

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK          1000


/*
 *  プロセッサのエンディアン
 */
#define SIL_ENDIAN  SIL_ENDIAN_LITTLE  /* リトルエンディアン */

#endif /* _EZARM7_H_ */



 

