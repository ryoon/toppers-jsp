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
 *  @(#) $Id: sys_config.h,v 1.4 2002/04/05 07:43:51 honda Exp $
 */

/*
 *	ターゲットシステム依存モジュール（Evaluator-7T用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  起動メッセージのターゲットシステム名
 */
#define	TARGET_NAME	"Evaluator-7T"

/*
 *  vxget_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

/*
 *  JSPカーネル動作時のメモリマップと関連する定義
 *
 *  使用可能メモリ領域
 *  0x00008000 〜 0x00074000
 *  JSPでの割り当て
 *  0x00008000 - 0x00039fff コード領域(200Kbyte 0x32000)
 *  0x0003A000 - 0x0006Bfff データ領域(200Kbyte 0x32000)
 *  0x0006C000
 *             - 0x00073fff タスク独立部スタック(32Kbyte 0x8000)
 */
#define	STACKTOP 0x00074000	/* 非タスクコンテキスト用のスタックの初期値 */
//#define	STACKTOP 0x00008000	/* 非タスクコンテキスト用のスタックの初期値 */


/*
 *  シリアル割り込みが入力/出力で異なるかどうかの定義
 */
#define SEPARATE_SIO_INT


/*
 *  シリアルポート番号に関する定義
 */
#define	CONSOLE_PORTID	1	/* コンソール用に用いるシリアルポート番号 */


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


/*
 *  割込み用のテーブル
 */
extern FP   int_table[20];


/*
 *  IRQハンドラ(sys_support.S)
 */
extern void IRQ_Handler(void);

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)  extern void inthdr(void);

#define INT_ENTRY(inthdr) inthdr

/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
extern void define_inh(INHNO inhno, FP inthdr);


/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK 50000
//#define TIMER_CLOCK 500000


#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
