/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
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
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 *
 *  @(#) $Id: kz_arm9ex.h,v 1.4 2003/12/01 06:29:15 honda Exp $
 */

/*
 *   京都マイクロコンピュータ製KZ-ARM9EX用の定義
 *   sys_config.hからインクルードされる
 */

#ifndef _KZ_ARM9EX_H_
#define _KZ_ARM9EX_H_

/*
 *  起動メッセージのターゲットシステム名
 */
#define    TARGET_NAME    "KZ-ARM9EX, Excalibur-ARM(ARM922T)"


/*
 *  JSPカーネル動作時のメモリマップと関連する定義
 *
 *  システムメモリマップ
 *
 *  SRAM0   : 0x00000000 - 0x0000ffff   (64kbyte)
 *  SRAM1   : 0x00010000 - 0x0001ffff   (64kbyte)
 *  DPSRAM0 : 0x00100000 - 0x00107fff   (32kbyte)
 *  DPSRAM1 : 0x00108000 - 0x0010ffff   (32kbyte)
 *  SDRAM0  : 0x10000000 - 0x0bffffff   (64Mbye)
 *
 *  JSPでの割り当て
 *  0x00000000 - 0x0000ffff コード領域(64Kbyte)
 *  0x00010000 -            データ領域(64Kbyte)
 *             - 0x0001ffff タスク独立部スタック
 *  0x10000000 -            PLDコンフィギュレーションデータ(約1Mbyte)
 */

#define   STACKTOP 0x00020000   /* 非タスクコンテキスト用のスタックの初期値 */

/*
 *  メモリーマップの設定を行うか
 */
//#define EXCALIBUR_MEMMAP_INIT

/*
 *  Memmory Map Control Registe の設定値
 */
#define MMAP_SRAM0_VAL    0x00000781
#define MMAP_SRAM1_VAL    0x00010781
#define MMAP_DPSRAM0_VAL  0x00100701
#define MMAP_DPSRAM1_VAL  0x00108701
//#define MMAP_SDRAM0_VAL   
//#define MMAP_SDRAM1_VAL
#define MMAP_EVI0_VAL     0x40000A83
#define MMAP_EVI1_VAL     0x50000c03
//#define MMAP_EVI2_VAL
//#define MMAP_EVI3_VAL
//#define MMAP_PLD0_VAL
//#define MMAP_PLD1_VAL
//#define MMAP_PLD2_VAL
//#define MMAP_PLD3_VAL



/*
 * PLLの初期化を行うか
 */
#define EXCALIBUR_PLL_INIT

/*
 * PLL関連の設定値
 */
#define CLK_PLL1_NCNT_VAL   0x40000
#define CLK_PLL1_MCNT_VAL   0x20404
#define CLK_PLL1_KCNT_VAL   0x40000
#define CLK_PLL2_NCNT_VAL   0x40000
#define CLK_PLL2_MCNT_VAL   0x10807
#define CLK_PLL2_KCNT_VAL   0x10201

/* 
 * コンフィギュレーションクロック設定
 * AHB2_CLOCK /( X * 2 )
 * 16Mhz以下になるように設定する．
 */
#define CONFIG_CLOCK_DATA  4
//#define CONFIG_CLOCK_DATA  3


/*
 *   CPUに供給するクロックスピード(sys_config.SのPLLの初期化とあわせる．)
 */
#define AHB1_CLK             132000000

/*
 *    Stripeのベースアドレス
 */
#define EXCALIBUR_DEV_BASE   0x7fffc000

/*
 * UARTの割り込みレベル
 */
#define INTLV_UART        5

/*
 *  UARTのボーレート
 */
#define UART_BPS      115200

/*
 *  UARTのデータフォーマット（UART_MCへの設定値）
 */
#define UART_DATA_FOMAT 0x03

/*
 *  タイマーの割り込みレベル
 */
#define INTLV_TIM0        4


/*
 *  タイマ値の内部表現とミリ秒単位との変換
 */
#define TIMER_CLOCK          66000


/*
 *  キャッシュ関連の設定
 */

/* 
 * キャッシュの設定 CP5 No.1 に書き込む値 
 * ICache のみON
 */
#define CP5_NO1_VAL         0x1078


/*
 *  プロセッサのエンディアン
 */
#define SIL_ENDIAN  SIL_ENDIAN_LITTLE  /* リトルエンディアン */

#endif /* _KZ_ARM9EX_H_ */



 

