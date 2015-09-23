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
 *  @(#) $Id: jsp_kernel.h,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	JSPカーネル 標準インクルードファイル
 */

#ifndef _JSP_KERNEL_H_
#define _JSP_KERNEL_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include "jsp_rename.h"

/*
 *  アプリケーションと共通のインクルードファイル
 */
#include <jsp_stddef.h>
#include <kernel.h>

/*
 *  システムログサービスのための定義
 */
#ifndef _MACRO_ONLY
#include <syslog.h>
#endif /* _MACRO_ONLY */

/*
 *  カーネル内で用いるマクロの定義
 */
#define SYSCALL

/*
 *  システムやプロセッサに依存する定義
 */
#include "sys_config.h"
#include "cpu_config.h"

/*
 *  オブジェクトIDの最小値の定義
 */
#define	TMIN_TSKID	1	/* タスクIDの最小値 */
#define	TMIN_SEMID	1	/* セマフォIDの最小値 */
#define	TMIN_FLGID	1	/* フラグIDの最小値 */
#define	TMIN_DTQID	1	/* データキューIDの最小値 */
#define	TMIN_MBXID	1	/* メールボックスIDの最小値 */
#define	TMIN_MPFID	1	/* 固定長メモリプールIDの最小値 */
#define	TMIN_CYCID	1	/* 周期ハンドラIDの最小値 */

/*
 *  優先度の段階数の定義
 */
#define	TNUM_TPRI	(TMAX_TPRI - TMIN_TPRI + 1)
#define	TNUM_MPRI	(TMAX_MPRI - TMIN_MPRI + 1)

/*
 *  ヘッダファイルを持たないモジュールの関数の宣言
 */
#ifndef _MACRO_ONLY

/*
 *  各モジュールの初期化（kernel_cfg.c）
 */
extern void	object_initialize(void);

/*
 *  初期化ルーチンの実行（kernel_cfg.c）
 */
extern void	call_inirtn(void);

/*
 *  起動メッセージの表示（banner.c）
 */
extern void	print_banner(void);

/*
 *  カーネルの終了処理（startup.c）
 */
extern void	kernel_exit(void);

#endif /* _MACRO_ONLY */
#endif /* _JSP_KERNEL_H_ */
