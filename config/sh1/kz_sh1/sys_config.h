/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: sys_config.h,v 1.2 2001/10/22 02:16:43 imai Exp $
 */


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#ifndef OMIT_RENAME

#define board_id		_kernel_board_id
#define board_addr		_kernel_board_addr

#ifdef LABEL_ASM

#define _board_id		__kernel_board_id
#define _board_addr		__kernel_board_addr

#endif /* LABEL_ASM */
#endif /* OMIT_RENAME */

/*
 *  起動メッセージのターゲット名
 */
#define	TARGET_NAME	"KMC KZ-SH1-01"

/*
 *  vxget_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

/*
 *  JSPカーネル動作時のメモリマップ
 *      0x0000,0000 - 0x0004,0000  コード領域(256KB)
 *      0x0a00,0000 -              データ領域(256KB)
 *                  - 0x0a03,ffff  タスク独立部用スタック メモリ終了
 *
 */

/*
 *   スタック領域の定義
 *	タスク独立部用スタックの初期値
 */


#define STACKTOP    	 0x0a03fffc
#define str_STACKTOP	_TO_STRING(STACKTOP)


/*
 *  シリアル割り込みが入力/出力で異なるかどうかの定義
 *  	このマクロを定義すると送受信で割込みハンドラが異なる
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
 *  システムを終了する時に使う．
 */
extern void	sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．
 *
 */

extern void sys_putc(char c);



/*
 *  SCIの設定
 *  	京都マイクロコンピュータKZ-SH1-01のクロック周波数は20MHz
 *  	CQ出版 RISC評価キット SH-1のクロック周波数は19.6608MHz
 */

/*
 *  分周比とビットレートの設定
 *
 *  クロック	　19.6608MHz
 *  ビットレート　9600bps
 *
 *  　N = CLOCK*10^6 / (64*2^(2n-1)*BPS) - 1
 *  より
 *  　n=0(分周しないのでそのまま)
 *  　N=63で誤差0%
 *  よって、
 *  　シリアルモードレジスタSMRのクロックセレクトビットCKS=00
 *  　ビットレートレジスタBRR=63
 */
#ifdef CONFIG_19MHZ
#define SMR_CKS 	0x0	/*  分周比  		*/
#define SCI_BRR 	63	/*  ビットレート  	*/
#endif

/*
 *  クロック	　20MHzの場合
 *  ビットレート　9600bps
 *
 *  　n=0(分周しないのでそのまま)
 *  　N=64で誤差0.16%
 *  よって、
 *  　シリアルモードレジスタSMRのクロックセレクトビットCKS=00
 *  　ビットレートレジスタBRR=64
 */
#ifdef CONFIG_20MHZ
#define SMR_CKS 	0x0	/*  分周比  		*/
#define SCI_BRR 	64	/*  ビットレート  	*/
#endif


/*
 *  最初の1ビット分の待ち時間の設定
 *  クロック20MHzの場合の値を流用
 *  ループ1回に付き5命令（＝5クロック）
 *  　N = CLOCK*10^6 / (5*BPS)
 *  より
 *  　N=417回
 *  マージンを入れて
 *  　N=450回
 */
#define WAIT_TIME 	450


/*
 *  ITUの設定
 */

/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */

/*
 *   タイマへの入力クロックの分周比設定
 *   	f/8でカウント
 */
#define TCR_TPSC	0x3

/*
 *   タイマに供給されるクロック周波数[kHz]
 *   	19.6608MHz/8 =2.4576MHz =2457.6kHz
 */
#ifdef CONFIG_19MHZ
#define TIMER_CLOCK	2458
#endif

/*   	20MHz/8 =2.5MHz =2500kHz	*/
#ifdef CONFIG_20MHZ
#define TIMER_CLOCK	2500
#endif


#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
