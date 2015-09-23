/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: sys_config.h,v 1.1 2002/04/05 08:47:14 honda Exp $
 */

/*
 *	ターゲットシステム依存モジュール（Insight VirtexII用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#ifndef OMIT_RENAME

#ifdef LABEL_ASM

#endif /* LABEL_ASM */
#endif /* OMIT_RENAME */

/*
 *  起動メッセージのターゲット名
 */
#define	TARGET_NAME	"INSIGHT VIRTEXII(Microblaze)"

/*
 *  コンフィギュレーション依存の定義
 */
/*
 *  JSPカーネル動作時のメモリマップ
 *  全て内臓のブロックRAM 
 *      0x0400 - 0x7fff   コード領域(約32KB)
 *      0x8000 -          データ領域(32KB)
 *             - 0xffff   タスク独立部用スタック
 *             - 0x10000  メモリ終了
 *
 */

/*
 *   スタック領域の定義
 */                 
#define STACKTOP    0x10000    /* タスク独立部用スタックの初期値 */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK		24000

/*
 *  INTC のベースアドレス
 */
#define INTC_BASE 0xFFFF1000

/*
 *  TIMER のベースアドレス
 */
#define TIMER_BASE 0xFFFF0000   

/*
 * カーネルで使用する UART のベースアドレス
 */
#define UART_BASE 0xFFFF8100


/*
 *  TIM 割込みの割込み番号
 */
#define TIMINTLVL        1


/*
 * UART 割込みの割込み番号
 */
#define UARTINTLVL 2



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
extern void sys_putc(char c);
                        
/*
 *  LCDドライバ
 */
extern void lcd_init(void);
extern void lcd_write_string(char *string);
extern BOOL lcd_set_cur_posi(int row, int col);

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */




