/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *                2003      by Advanced Data Controls, Corp
 *                2009 by Suikan
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
 */


/*
 *  ターゲットシステム依存モジュール（CQ-FRK-NXP-ARM用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 * システム依存部の著作権表示。print_banner()から参照される。
 */
#define COPYRIGHT_SYS "Copyright (C) 2009-2010 by TOPPERS/JSP for LPC2388 project\n"\
"            http://sourceforge.jp/projects/toppersjsp4lpc/\n"

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 * LPC2388共通部のインクルードファイルを読み込む
 */
#include <chip_config.h>


/*
 *  シリアルポート番号に関する定義
 *  LPC2388版では、U0, U2, U3を使う。U1は使わない。
 */
#define TNUM_PORT   1   /* サポートするシリアルポートの数 */
#define TNUM_SIOP   1   /* サポートするシリアルI/Oポートの数. 最大3 */

/*
 *  シリアルポート番号に関する定義
 */
#define LOGTASK_PORTID  1   /* システムログに用いるシリアルポート番号 */


/*
 * LP2388のクロック設定
 */
#define LP2388_CCLK (72000000)	/* 72MHz */
#define LP2388_SYSTEM_TIMER3_CLOCK (LP2388_CCLK/4)
#define LP2388_SYSTEM_UART0_CLOCK (LP2388_CCLK/4)

#define UART0_BAUD_RATE 57600

#endif /* _SYS_CONFIG_H_ */
