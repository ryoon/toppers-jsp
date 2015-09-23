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
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: sys_config.h,v 1.2 2001/11/15 13:18:32 takayuki Exp $
 */

/*
 *	ターゲットシステム依存モジュール（RTE-V850E/MA1-CB用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"RTE-V850E/MA1-CB"

#define NUM_INTERRUPT	0x38

	/* RTE-V850E/MA1-CB 50MHz で 1msec */
#define TIMER_PORT            0
#define TIMER_INT_PRIORITY    0
#define TIMER_PRESCALER       2      /* clk = f/16 */
#define TIMER_COMPAREVALUE    3125

/*
 *  vxget_tim をサポートするかどうかの定義
 */
//#define	SUPPORT_VXGET_TIM

/*
 *  JSPカーネル動作時のメモリマップと関連する定義
 */
#define	STACKTOP 0x0fffe800	/* 非タスクコンテキスト用のスタックの初期値 */

/*
 *  シリアルポート番号に関する定義
 */
#define	CONSOLE_PORTID	1	/* コンソール用に用いるシリアルポート番号 */
#define SEPARATE_SIO_INT

	/* RTE-V850E/MA1-CB 50MHz で 115.2kbps */
    /* 注意 : SERIAL_CLKDIV > 11, SERIAL_COMPAREVALUE < 8 で逝きます */
#define SERIAL_PORT			  (CONSOLE_PORTID-1)
#define SERIAL_INT_PRIORITY   0
#define SERIAL_CLKDIV         1    /* clk = f/2     */
#define SERIAL_COMPAREVALUE   108  /* bps = clk/108 */

#ifndef _MACRO_ONLY

/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．ROMモニタ呼出しで実現することを想定し
 *  ている．
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */
extern void	sys_putc(char c);

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
