/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: sys_config.h,v 1.2 2001/02/23 21:14:08 honda Exp $
 */


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_


/*
 *  起動メッセージのターゲット名
 */
#define	TARGET_NAME	"DVESH7700"

/*
 *  vxget_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

/*
 *  キャッシュモードの設定
 *  P0,U0,P1ライトスルーモード
 */
#define CCR_MODE      0x00000003  


/*
 *  JSPカーネル動作時のメモリマップ 
 *      0c000000 - 0c000fff     stubワークエリア
 *      0c001000 - 0c080000     コード領域(約0.5MB)
 *      0c080000 -              データ領域(約0.5MB)
 *               - 0c0fffff     タスク独立部用スタック メモリ終了
 *
 */

/*
 *   スタック領域の定義
 */
#define STACKTOP    0x0c100000    /* タスク独立部用スタックの初期値 */   

/*
 *  シリアル割り込みが入力/出力で異なるかどうかの定義
 */

#define SEPARATE_SIO_INT


/*
 *  システムタスクに関する定義
 */
#define	CONSOLE_PORTID	1	/* コンソール用に用いるシリアルポート番号 */
#define	LOGTASK_PORTID	1	/* システムログを出力するシリアルポート番号 */

/*
 *  システムタスクが使用するライブラリに関する定義
 */
#define NEWLIB			/* newlib を用いる */

#ifndef _MACRO_ONLY

/*
 *  プロセッサ識別のための変数（マルチプロセッサ対応）
 */
extern INT	board_id;	/* ボードID */
extern INT	board_addr;	/* ローカルメモリの先頭アドレス */



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
 *
 */

extern void sys_putc(char c);



/*
 *  SCIF,SCI,TMUの設定値
 *  Not yet!
 */

#ifdef CONFIG_30MHZ

/*
 *  16MHz / (3 + 1) /32 = 115200bps
 */
#define SCI_BPS_VALUE 3

#else  /* CONFIG_60MHZ */

/*
 * 30MHz / (7 + 1) /32 = 115200bps
 */
#define SCI_BPS_VALUE 7

#endif


/*
 *  DVESH7700は 
 *  モード7 I:B:P 1:1:1/2 で動作
 *  タイマ値の内部表現と msec 単位との変換
 *  P = 15Mhz I = 30MHz
 *  /4:  3.75Mhz       tick 3750   0.266 μsec 
 *  /16: 0.9375Mhz     tick 938    1.066 μsec  X
 *  /64: 0.234375Mhz   tick 234    4.266 μsec
 *  /256:0.0585Mhz     tick 58     17.09 μsec
 *
 *  P = 30MHz I = 30MHz
 *  /4:  7.5Mhz     tick 7500   0.133 μsec 
 *  /16: 1.875Mhz   tick 1875   0.533 μsec X
 *  /64: 0.469Mhz   tick 469    2.133 μsec
 *  /256:0.117Mhz   tick 117    8.545 μsec
 *
 *
 * YDC DU
 *  P = 7.5MHz I = 15MHz
 *  /4:   1.875Mhz      tick 1875   0.532 μsec X
 *  /16:  0.468Mhz      tick 468    2.132 μsec
 *  /64:  0.117Mhz      tick 117    8.532 μsec
 *  /256: 0.029Mhz      tick 29     34.18 μsec
 *
 * YDC DU
 *  P = 3.75MHz I = 15MHz
 *  /4:   0.937Mhz       tick 937   1.067 μsec X
 *  /16:  0.234Mhz       tick 234   4.273 μsec
 *  /64:  0.0585Mhz      tick 59    17.09 μsec
 *  /256: 0.0146Mhz      tick 15    68.49 μsec
 *
 *  TMUのクロックの上限は2Mhzなので/16で動くか?
 */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */

#ifdef CONFIG_15MHZ

/*
 *   P/4でカウント
 */
#define TCR0_TPSC   0x0000
#define TIMER_CLOCK 937

#elif CONFIG_30MHZ

/*
 *   P/16でカウント
 */
#define TCR0_TPSC   0x0001
#define TIMER_CLOCK 1875

#elif CONFIG_60MHZ

/*
 *   P/16でカウント
 */
#define TCR0_TPSC   0x0001
#define TIMER_CLOCK 1875

#endif


#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */




