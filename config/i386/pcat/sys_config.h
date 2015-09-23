/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
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
 *  @(#) $Id: sys_config.h,v 1.2 2002/04/10 12:08:17 honda Exp $
 */

/*
 *	ターゲットシステム依存モジュール（PC/AT用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"i386(PC/AT)"

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

#define PIT_IRQ (0)

#define PCAT_COM1_IRQ (4)
#define PCAT_COM2_IRQ (3)
#define PCAT_COM1_BASE (0x03F8)
#define PCAT_COM2_BASE (0x02F8)

#define	IRQ0_VECTOR	0x20
#define	IRQ8_VECTOR	0x28
#define	IRQ_VECTOR(x)	(IRQ0_VECTOR + (x))

/*
 *  システムタスクに関する定義
 */
#define	CONSOLE_PORTID	1	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORTID	1	/* システムログを出力するシリアルポート番号 */

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

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
