/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: sys_config.h,v 1.10 2003/12/06 09:47:04 hiro Exp $
 */

/*
 *	ターゲットシステム依存モジュール（M52235EVB用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  ユーザー定義情報
 */
#include <user_config.h>

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "m52235evb.h"

/*
 *  数値データ文字列化用マクロの定義
 */
#include <util.h>

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"M52235EVB"

/*
 *　非タスクコンテキスト用スタックの初期値
 */
#define str_STACKTOP	TO_STRING(STACKTOP)


/*
 *　初期の例外処理ベクタ
 */
#define EXCVT_ORIG   (0x00000000)

/*
 *  微少時間待ちのための定義
 */
#define	SIL_DLY_TIM1   120
#define	SIL_DLY_TIM2	80

#ifndef _MACRO_ONLY
/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．ROMモニタ／GDB STUB呼出しで実現する．
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ／GDB STUB呼出しで実
 *  現する．
 */
extern void	sys_putc(char c);

#endif /* _MACRO_ONLY */

/*
 *  例外ベクタテーブル関連のマクロ定義
 *  　　マクロの説明
 *  　　　KERNEL_HAS_A_VECTOR_TABLE
 *  　　　　カーネルが例外ベクタテーブルを持つ
 *  　　　SIO_RESERVED
 *  　　　　シリアルデバイスがデバッガによって使用されている
 */
#ifdef GDB_STUB
/*
 *  GDB-stubの場合
 *  　　カーネルは例外ベクタテーブルを持たず、割込みの際には
 *　　　stubが管理している例外ベクタテーブルを参照する
 *  　　シリアルポートはstubが占有している
 */
//#define SIO_RESERVED

#else /* GDB_STUB */
/*
 *  ROM化の場合
 *  　　リセット時：ROM上にある例外ベクタテーブルを参照する
 *  　　初期化後：RAM上にある例外ベクタテーブルを参照する
 */
#define KERNEL_HAS_A_VECTOR_TABLE

#endif /* GDB_STUB */

#endif /* _SYS_CONFIG_H_ */
