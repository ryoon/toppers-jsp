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
 *  @(#) $Id: hw_serial.h,v 1.1 2001/05/04 09:07:50 imai Exp $
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（常盤商行 SH1/CPUB用）
 *  SCI0を使用
 *  　　共通部のモジュールによって、XON/XOFFフロー制御が行わ
 *  　　れているので、信号線はTxDとRxDの2本でよい
 */

#include "sh1.h"
#include "sys_config.h"
#include "cpu_config.h"

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL_IN	 RXI0
#define INHNO_SERIAL_OUT TXI0
/*
 *  エラー処理はしていない
 * #define INHNO_SERIAL_ERROR  SCI_BRI_INTEVT
 */


/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct hardware_serial_port_descripter {
    INT	  *initflag;	   /* 初期化済フラグへのポインタ   */
    BOOL  sendflag;        /* 送信割り込みイネーブルフラグ */
} HWPORT;



/*
 * SCIの割り込みレベル
 * 　割込みハンドラは送受信別々に登録できるが、割込みレベルは
 * 　送信も受信も同じレベルにしか設定できない。
 */
#define SCIINTLVL	6	/*  送受信共用  */
#define LEVEL0		0	/*  割込み解除時のレベル  */
#define SCI_SHIFT	0	/*  割込みレベル登録時のビットシフト  */


/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	1	/* サポートするシリアルポートの数 */

static INT	initflag[NUM_PORT] = { 0 } ;	/* 初期化済フラグ */
#define HWPORT1	{ &initflag[0],  0 }

/*
 *　プログラム中のPORT1は、SCI0を指している
 */
#define PORT_ID1	1
#define PORT_INDEX1	0


/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
    int i;

    SCI.SCR0 &= ~(SCR_TE | SCR_RE);	/*  送受信停止  */
    					/*  ピンアサイン  */
    					/*  	PB8:RxD0 PB9:TxD0  */
    *PBCR1 = (*PBCR1 & PBCR1_TD0_RD0_MASK) |
    		PBCR1_TD0 | PBCR1_RD0;
    					/*  送受信フォーマット  */
    SCI.SMR0 = 	SMR_CA_ASYNC |	/*  調歩同期式  		*/
    		SMR_CHR8 |	/*  キャラクタレングス：8ビット */
    		SMR_PE_NON |	/*  パリティなし  		*/
    		SMR_STOP1 |	/*  ストップビットレングス：1 	*/
    		SMR_CKS;	/*  クロックセレクト（分周比）  */
    				/*  （sys_config.h）  		*/
    SCI.BRR0 = SCI_BRR;		/*  ボーレート設定  	*/
    				/*  （sys_config.h）  	*/

    				/*  割込み禁止とクロックソースの選択 */
    SCI.SCR0 = (IOREG)(~(SCR_TIE | SCR_RIE | SCR_TE | SCR_RE |
    		  SCR_MPIE | SCR_TEIE | SCR_CKE_MASK) | SCR_CKE);

    /*
     * ボーレートの設定後、1カウント分待たなければならない。
     */
    for(i=0; i<WAIT_TIME; i++);	/*  （sys_config.h）  	*/

    				/* エラーフラグをクリア		*/
    SCI.SSR0 &= ~(SSR_ORER | SSR_FER | SSR_PER);
    				/* 受信割り込み許可   	*/
    				/* 送受信許可  		*/
    SCI.SCR0 |= (SCR_RIE | SCR_TE | SCR_RE);
    		/*  送信割込みの許可は送信制御関数で行う  */

    /*
     * 割り込み関連の設定
     * エラーハンドラも登録すべき
     */
    if (initflag[PORT_INDEX1] <= 0) {
        			/* 割り込みレベル設定 */
        define_int_plevel(IPRD, SCIINTLVL, SCI_SHIFT);
    }

    *(p->initflag) = 1;			/* 初期化フラグ設定 */
    return(FALSE);
}

/*
 *  シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
    *(p->initflag) = -1;		/* 初期化フラグ設定     */
    SCI.SCR0 &= ~(SCR_TE | SCR_RE);	/*  送受信停止  	*/
    					/* 割り込みレベルクリア */
    define_int_plevel(IPRD, LEVEL0, SCI_SHIFT);
}

/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void	serial_handler_in(int portid);
extern void	serial_handler_out(int portid);


/*
 *  シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler_in()
{
    if (initflag[PORT_INDEX1] > 0)
        serial_handler_in(PORT_ID1);
    else
    	/*  初期化されていない場合は受信割り込みを禁止  */
        SCI.SCR0 &= ~SCR_RIE;
}

Inline void
hw_serial_handler_out()
{
    if (initflag[PORT_INDEX1] > 0)
        serial_handler_out(PORT_ID1);
    else
    	/*  初期化されていない場合は送信割り込みを禁止  */
        SCI.SCR0 &= ~SCR_TIE;
}


/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	/*  レシーブデータレジスタフル・フラグのチェック  */
	return(SCI.SSR0 & SSR_RDRF);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	/*  トランスミットデータレジスタエンプティ・フラグのチェック*/
	return(SCI.SSR0 & SSR_TDRE);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
    byte data;
    data = SCI.RDR0;
 	/*  レシーブデータレジスタフル・フラグのクリア  */
    SCI.SSR0 &= ~SSR_RDRF ;
    return data;
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
    SCI.TDR0 = c;
	/*  トランスミットデータレジスタエンプティ・フラグのクリア*/
    SCI.SSR0 &= ~SSR_TDRE;
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
    if (!(p->sendflag)) {
        SCI.SCR0 |= SCR_TIE;   /* 送信割り込み要求を許可 */
        p->sendflag = 1;
    }
}

Inline void
hw_port_sendstop(HWPORT *p)
{
    if (p->sendflag) {
        SCI.SCR0 &= ~SCR_TIE; /* 送信割り込み要求を禁止 */
        p->sendflag = 0;
    }
}

#endif /* _HW_SERIAL_H_ */
