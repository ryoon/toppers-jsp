/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
 *  Copyright (C) 2008-     by Monami Software Limited Partnership, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: sys_config.h,v 1.1 2004/07/21 02:49:37 monaka Exp $
 */

/*
 *	ターゲットシステム依存モジュール（PC/AT用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"IA-32(PC/AT)"

/*
 *  vxget_tim をサポートするかどうかの定義
 */
#if 0
#define	SUPPORT_VXGET_TIM
#endif

/*
 * 非タスクコンテキスト用のスタックの初期値
 */
#define	STACKTOP	$(stack + STACKSIZE)
#define	STACKSIZE	8192

/*
 *  シリアルポート数の定義
 */
#define TNUM_PORT	2	/* サポートするシリアルポートの数 */
#define	TNUM_SIOP	2	/* サポートするシリアルI/Oポートの数 */

/*
 *  シリアルポート番号に関する定義
 */
#ifndef LOGTASK_PORTID
#define	LOGTASK_PORTID	1	/* システムログに用いるシリアルポート番号 */
#endif


#define PIT_INHNO 0x20

#define PCAT_COM1_INHNO 0x24
#define PCAT_COM2_INHNO 0x23
#define PCAT_COM1_BASE (0x03F8)
#define PCAT_COM2_BASE (0x02F8)

#define	IRQ0_VECTOR	0x20
#define	IRQ8_VECTOR	0x28
#define	IRQ_VECTOR(x)	(IRQ0_VECTOR + (x))

#define TO_INTNO(a) ((a) - 0x20)

#ifndef _MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 */
extern void	sys_putc(char c);

/*
 *  割込みハンドラの設定
 */

/*
 *  割込みハンドラの出入口処理（sys_support.S）
 */
extern void interrupt0(void);
extern void interrupt1(void);
extern void interrupt2(void);
extern void interrupt3(void);
extern void interrupt4(void);
extern void interrupt5(void);
extern void interrupt6(void);
extern void interrupt7(void);
extern void interrupt8(void);
extern void interrupt9(void);
extern void interrupt10(void);
extern void interrupt11(void);
extern void interrupt12(void);
extern void interrupt13(void);
extern void interrupt14(void);
extern void interrupt15(void);

/*
 *  割込みハンドラのテーブル（sys_config.c）
 *
 *  割込みハンドラは一旦このテーブルに登録され、別に用意された出入口処理
 *  を呼び出す。
 */
extern FP int_table[0x10];

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
extern void define_inh(INHNO inhno, FP inthdr);

/*
 *  割込みハンドラの出入口処理
 */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 */

#define INTHDR_ENTRY(inthdr) extern void inthdr(void)

#define INT_ENTRY(inthdr) inthdr

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
