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
 *  @(#) $Id: hw_serial.h,v 1.3 2001/11/08 21:28:19 honda Exp $
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（Evaluator-7T用）
 *  SCI0を使用
 */

#include "evaluator7t.h"
#include "sys_defs.h"
#include "sys_config.h"


/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL_IN	 IRQ_NUMBER_UART0_RX
#define INHNO_SERIAL_OUT IRQ_NUMBER_UART0_TX


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
#define SCI0INTLVL_TX   5
#define SCI0INTLVL_RX   6


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
    UART0.UCON = 0;                /* Disable Interrupt */
    UART0.ULCON = ULCON_WL_8BIT;   /* 8Data, 1Stop, No Parity */
    UART0.UBRDIV = UBRDIV_19200;    /* 9600bps */


    /*
     * 割込み関連の設定
     */
    UART0.UCON = UCON_RxM_INT;    /* Enable Interrupt */
    /* Clear Rx Tx Pending bit */    
    *INTPND = (IRQ_BIT_UART0_RX | IRQ_BIT_UART0_TX);



    /* Set Interrupt priority (SW) */
    
    /* Set Interrupt priority (HW) */
    
//    *(INTPRI0 + SCI0INTLVL_TX / 4) =
//        IRQ_NUMBER_UART0_TX << (SCI0INTLVL_TX % 4);
//    *(INTPRI0 + SCI0INTLVL_RX / 4) =
//        IRQ_NUMBER_UART0_RX << (SCI0INTLVL_RX % 4);

    
    /* Enable Interrupt Rx */
    *INTMSK &= ~((1 << INT_GLOBAL) | IRQ_BIT_UART0_RX |IRQ_BIT_UART0_TX); 
    
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
    UART0.UCON = 0;             /* Disable Interrupt */    
    /* Clear Rx Tx Pending bit */    
    *INTPND = (IRQ_BIT_UART0_RX | IRQ_BIT_UART0_TX);
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
        *INTPND = IRQ_BIT_UART0_RX;
}


Inline void
hw_serial_handler_out()
{
    if (initflag[0] > 0)
        serial_handler_out(1);
    else
        *INTPND = IRQ_BIT_UART0_TX;
}


/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	return((UART0.USTAT & USTAT_RDR)!=0);
}


/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	return((UART0.USTAT & USTAT_TBE)!=0);
}


/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
    return(UART0.URXBUF);         
}


/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
    UART0.UTXBUF=c;
}


/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
    
    if (!(p->sendflag)) {
        UART0.UCON = (UART0.UCON & ~UCON_TxM_MASK) | UCON_TxM_INT;
/*        *INTPND = IRQ_BIT_UART0_TX;        */
        /* Enable Interrupt */        
        p->sendflag = 1;
    }
}


Inline void
hw_port_sendstop(HWPORT *p)
{
	if (p->sendflag) {
//        UART0.UCON = (UART0.UCON & ~UCON_TxM_MASK) | UCON_TxM_DIS;
        UART0.UCON = (UART0.UCON & ~UCON_TxM_MASK) | UCON_TxM_GDMA0;
        *INTPND = IRQ_BIT_UART0_TX;                
       /* Disable Interrupt */
        p->sendflag = 0;
	}
}

#endif /* _HW_SERIAL_H_ */





















