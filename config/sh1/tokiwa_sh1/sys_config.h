/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: sys_config.h,v 1.8 2003/12/18 06:34:40 honda Exp $
 */

/*
 *	ターゲットシステム依存モジュール（常盤商行SH1/CPUB用）
 *　　　　　カーネル内部で使用する定義
 *　　　　　　データ型、マクロ、関数のプロトタイプ宣言
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include <tokiwa_sh1.h>

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"TOKIWA SH1/CPUB"

/*
 *  数値データ文字列化用マクロ
 */
#define _TO_STRING(arg)	#arg
#define TO_STRING(arg)	_TO_STRING(arg)

/*
 *  JSPカーネル動作時のメモリマップ
 *      0x0000,0000 - 0x0004,0000  コード領域(256KB)
 *      0x0f00,0000 -              データ領域(8KB)
 *                  - 0x0f00,2000  タスク独立部用スタック メモリ終了
 */

/*
 *   スタック領域の定義
 *	非タスクコンテキスト用スタックの初期値
 */
#define STACKTOP    	0x0f001ffc
#define str_STACKTOP	TO_STRING(STACKTOP)

/*
 *  シリアルポート数の定義
 *
 *  TNUM_PORTはシリアルドライバ（serial.c）、つまり GDICレベルでサポー
 *  トしているシリアルポートの数であり、機種依存部で定義するよう共通部
 *  とのインターフェースで規定されている。
 *  一方、TNUM_SIOPはPDICレベルでサポートしているシリアルポートの数で
 *  あり、機種依存部の中でのみ用いている。
 *　
 */
#define TNUM_PORT 1	/* GDICレベルでサポートするシリアルポートの数 */
#define	TNUM_SIOP 1	/* PDICレベルでサポートするシリアルポートの数 */

#define	LOGTASK_PORTID	1  /* システムログに用いるシリアルポート番号 */

/*
 *  微少時間待ちのための定義（未調整）
 */
#define	SIL_DLY_TIM1	1143
#define	SIL_DLY_TIM2	 832

#ifndef _MACRO_ONLY
/*
 *  プロセッサ識別のための変数（マルチプロセッサ対応）
 *　　　SH1では未実装
 */
#if 0
extern UINT	board_id;	/* ボードID */
extern VP	board_addr;	/* ローカルメモリの先頭アドレス */
#endif

/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 *　　ROMモニタ／GDB STUB呼出しは未実装
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．
 *　　ROMモニタ／GDB STUB呼出しは未実装
 */
extern void	sys_putc(char c) throw();

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
