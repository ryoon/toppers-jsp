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
 *  @(#) $Id: hw_serial.h,v 1.1 2000/11/14 16:30:22 honda Exp $
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（RSH3用）
 *  SCI1を使用
 */

#include "sh3.h"
#include "sys_config.h"
#include "cpu_config.h"


/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL_IN	 SCI1_RXI_INTEVT       
#define INHNO_SERIAL_OUT SCI1_TXI_INTEVT	
#define INHNO_SERIAL_ERROR  SCI1_BRI_INTEVT

/*
 *  シリアルポートのハードウェア依存情報の定義
 *  Not yet! m68kのようにコントロールレジスタを構造体にする。
 */
typedef struct hardware_serial_port_descripter {
    INT	  *initflag;	   /* 初期化済フラグへのポインタ   */
    BOOL  sendflag;        /* 送信割り込みイネーブルフラグ */
} HWPORT;



/*
 * SCI1の割り込みレベル
 * 受信も送信も同じに設定する。
 */


#define SCI1INTLVL   6


/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	1	/* サポートするシリアルポートの数 */

static INT	initflag[1] = { 0 } ;	/* 初期化済フラグ */

#define HWPORT1	{ &initflag[0],  0 }


/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
    SCI1.SCSCR1 = 0;          /* SCI1を停止,内部Pクロックを使う */
    SCI1.SCFCR1 = SCFCR1_TFRST | SCFCR1_RFRST; /* 受信送信FIFOをクリア */
    SCI1.SCSMR1 = 0;          /* 8データ,1ストップビット,no parity */
    SCI1.SCBRR1 = SCI1_BPS_VALUE;        

    /*
     * ボーレートの設定後、1カウント分待たなければならない。
     * バスステートコントローラのリフレッシュカウンタを使い
     * 1カウント分待つ
     */

    *RFCR = RFCR_CLEAR;
    while( *RFCR < WAIT_RFCR_FOR_SCI);
    
        
    SCI1.SCFCR1 = 0x30;           /* R-FIFO=1,T-FIFO=1 トリガの設定 */

    SCI1.SCSSR1 = 0x0060;         /* エラーフラグをクリア           */
    SCI1.SCSCR1 = 0x70;           /* Start Tx Rx 受信割り込み許可   */

    
    /*
     * 割り込み関連の設定
     * エラーハンドラも登録すべき
     */
    if (initflag[0] <= 0) {
        *IPRE=(*IPRE&0xf0ff)|(SCI1INTLVL<<8); /* 割り込みレベル設定 */
        define_int_plevel(SCI1_RXI_INTEVT,SCI1INTLVL);
        define_int_plevel(SCI1_TXI_INTEVT,SCI1INTLVL);
        *IRR1=0;                              /* 割り込み要求クリア */  
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
	*(p->initflag) = -1;		/* 初期化フラグ設定           */
    SCI1.SCSCR1 = 0x00;         /* シリアルコントローラの停止 */
    *IPRE=(*IPRE&0xf0ff);       /* 割り込みマスククリア       */
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
    if (initflag[0] > 0)
        serial_handler_in(1);        
    else
        SCI1.SCSCR1 &= ~(SCSCR1_RIE);
}

Inline void
hw_serial_handler_out()
{
    if (initflag[0] > 0)
        serial_handler_out(1);
    else
        SCI1.SCSCR1 &= ~(SCSCR1_TIE);
}


/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	return((SCI1.SCSSR1 & SCSSR1_RDF)!=0);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
    return((SCI1.SCSSR1 & SCSSR1_TDFE) !=0);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
    byte data;
    data = SCI1.SCFRDR1;         
    SCI1.SCSSR1 &= ~SCSSR1_RDF ; 
    return data;
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
    SCI1.SCFTDR1=c;
    SCI1.SCSSR1 &= ~(SCSSR1_TDFE |SCSSR1_TEND);  
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
    if (!(p->sendflag)) {
        SCI1.SCSCR1 |= SCSCR1_TIE;   /* 送信割り込み要求を許可 */
        p->sendflag = 1;
    }
}

Inline void
hw_port_sendstop(HWPORT *p)
{
	if (p->sendflag) {
        SCI1.SCSCR1 &= ~SCSCR1_TIE; /* 送信割り込み要求を禁止 */
        p->sendflag = 0;
	}
}

#endif /* _HW_SERIAL_H_ */





















