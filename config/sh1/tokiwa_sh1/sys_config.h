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
 *  @(#) $Id: sys_config.h,v 1.1 2001/05/04 09:10:29 imai Exp $
 */


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_


/*
 *  起動メッセージのターゲット名
 */
#define	TARGET_NAME	"TOKIWA SH1/CPUB"

/*
 *  vxget_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

/*
 *  JSPカーネル動作時のメモリマップ
 *      0x0000,0000 - 0x0004,0000  コード領域(256KB)
 *      0x0f00,0000 -              データ領域(8KB)
 *                  - 0x0f00,2000  タスク独立部用スタック メモリ終了
 *
 */

/*
 *   スタック領域の定義
 *	タスク独立部用スタックの初期値
 */
#define STACKTOP    	 0x0f001ffc
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
 *  	常盤商行SH1/CPUBのクロック周波数は16MHz
 *  	（ICEの内部クロック周波数は20MHz）
 */

/*
 *  分周比とビットレートの設定
 *
 *  クロック	　16MHz
 *  ビットレート　9600bps
 *
 *  　N = CLOCK*10^6 / (64*2^(2n-1)*BPS) - 1
 *  より
 *  　n=0(分周しないのでそのまま)
 *  　N=51で誤差0.16%
 *  よって、
 *  　シリアルモードレジスタSMRのクロックセレクトビットCKS=00
 *  　ビットレートレジスタBRR=51
 */
#ifdef CONFIG_16MHZ
#define SMR_CKS 	0x0	/*  分周比  		*/
#define SCI_BRR 	51	/*  ビットレート  	*/
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
 *  クロック16MHzのとき
 *  ループ1回に付き5命令（＝5クロック）
 *  　N = CLOCK*10^6 / (5*BPS)
 *  より
 *  　N=333回
 *  マージンを入れて
 *  　N=350回
 */
#ifdef CONFIG_16MHZ
#define WAIT_TIME 	350
#endif


/*
 *  クロック20MHzのとき
 *  ループ1回に付き5命令（＝5クロック）
 *  　N = CLOCK*10^6 / (5*BPS)
 *  より
 *  　N=417回
 *  マージンを入れて
 *  　N=450回
 */
#ifdef CONFIG_20MHZ
#define WAIT_TIME 	450
#endif


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
 *   	16MHz/8 =2MHz =2000kHz
 */
#ifdef CONFIG_16MHZ
#define TIMER_CLOCK	2000
#endif

/*   	20MHz/8 =2.5MHz =2500kHz	*/
#ifdef CONFIG_20MHZ
#define TIMER_CLOCK	2500
#endif



#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
