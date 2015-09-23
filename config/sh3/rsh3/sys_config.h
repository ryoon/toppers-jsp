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
 *  @(#) $Id: sys_config.h,v 1.6 2001/11/09 08:35:55 honda Exp $
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
#define	TARGET_NAME	"RSH3"

/*
 *  vgxet_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

/*
 *  キャッシュモードの設定
 *  P0,U0,P1ライトスルーモード
 */
#define CCR_MODE      0x00000003  

/*
 *  JSPカーネル動作時のメモリマップ 
 *      0c000000 - 0c000fff     stubのワークエリア
 *      0c001000 - 0c100000     コード領域(1MB)
 *      0c100000 -              データ領域(3MB)
 *               - 0c3fffff     タスク独立部用スタック
 *               - 0c400000     メモリ終了
 *
 */

/*
 *   スタック領域の定義
 */
#define STACKTOP    0x0c400000    /* タスク独立部用スタックの初期値 */

 
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
 */

extern void sys_putc(char c);


/*
 *  SCIF,SCI,TMUの設定値
 *
 */

/*
 *   SCI1のボーレートの設定
 *   Pクロックの違いにより分ける
 */

#ifdef CONFIG_16MHZ

/* 
 * 16MHz / (8 + 1) /32 = 57600bps
 */
#define SCI1_BPS_VALUE 8

#else  /* CONFIG_40MHZ */

/*
 * 20MHz / (4 + 1) /32 = 57600bps
 */
#define SCI1_BPS_VALUE 10

#endif



/*
 *  タイマ値の内部表現と msec 単位との変換
 *  CARD-E09Aでは、Pクロックが132.8/4=33.2MHz
 *  分周比 /4,/16,/64/256 のいずれかを選択 
 *  CKIO = 16Mhz
 *  /4:  4Mhz      tick 4000   0.25 μsec  
 *  /16: 1Mhz      tick 1000   1    μsec
 *  /64: 0.25Mhz   tick 250    4    μsec
 *  /256:0.0625Mhz tick 62     16   μsec
 *
 *  /16 を選択
 *
 *  CKIO = 20MHz
 *  /4:  5Mhz      tick 5000   0.20 μsec 
 *  /16: 1.25Mhz   tick 1250   0.8  μsec
 *  /64: 0.3125Mhz tick 312    3.2  μsec
 *  /256:0.078Mhz  tick 78     12.8 μsec
 *
 *  /16 を選択
 */

#define TCR0_TPSC  0x0001


/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */

#ifdef CONFIG_16MHZ
#define TIMER_CLOCK		1000
#else  /* CONFIG_40MHZ */
#define TIMER_CLOCK		1250
#endif


/*
 *  シリアルコントローラのボーレート設定時の待ち時間の設定
 *  バスステートコントローラのリフレッシュカウンタを使う
 */
#define WAIT_RFCR_FOR_SCI    200

#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */




