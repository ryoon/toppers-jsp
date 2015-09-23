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
 *  @(#) $Id: sys_config.h,v 1.1 2000/11/14 16:30:22 honda Exp $
 */


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  SH3のプロセッサタイプ
 */

#define SH7709A


/*
 *  起動メッセージのターゲット名
 */
#define	TARGET_NAME	"CARD-E09A + Evaluation Board"

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
 *  JSPカーネル動作時のメモリマップ stub無し
 *
 *      0c000000 - 0c02ffff     IPLのワークエリア(使用できない)
 *      0c030000 - 0c0fffff     コード領域(約1MB)
 *      0c100000 -              データ領域(約3MB)
 *               - 0c3fffff     タスク独立部用スタック
 *               - 0cf00000     メモリ終了
 *
 *  JSPカーネル動作時のメモリマップ stub有り
 *
 *      0c000000 - 0c02ffff     IPLのワークエリア(使用できない)
 *      0c030000 - 0c032fff     stubのワークエリア
 *      0c033000 - 0c122fff     コード領域(約1MB)
 *      0c123000 -              データ領域(約3MB)
 *               - 0c422fff     タスク独立部用スタック
 *               - 0cf00000     メモリ終了
 *
 */

/*
 *   スタック領域の定義
 */
#define STACKTOP    0x0c400000    /* タスク独立部用スタックの初期値 */   

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
 */

extern void sys_putc(char c);


/*
 *  SCIF,TMUの設定値
 *
 */

/*
 *   SCIFのボーレートの設定
 *   Pクロック33.2MHzなので
 *   33MHz / (8 + 1) /32 = 115200bps
 */

#define SCIF_BPS_VALUE 8  

/*
 *  タイマ値の内部表現と msec 単位との変換
 *  CARD-E09Aでは、Pクロックが132.8/4=33.2MHz
 *  分周比 /4,/16,/64/256 のいずれかを選択
 *  /4:  8.3Mhz   tick 8300   0.12 μsec
 *  /16: 2.075Mhz tick 2075   0.48 μsec 
 *  /64: 0.518Mhz tick 518    1.93 μsec
 *  /256:0.129Mhz tick 129    7.73 μsec
 *
 *  /16 を選択
 *  TMUのクロックの上限は2Mhzなので/16で動くか?
 */

#define TCR0_TPSC  0x0001


/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK		2075



#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */




