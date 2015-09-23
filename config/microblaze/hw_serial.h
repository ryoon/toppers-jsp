/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: hw_serial.h,v 1.1 2002/04/05 08:47:14 honda Exp $
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（MicroBlaze用）
 */

#include "microblaze.h"
#include "sys_config.h"
#include "cpu_config.h"


/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct hardware_serial_port_descripter {
    INT	  *initflag;	   /* 初期化済フラグへのポインタ   */
    BOOL  sendflag;        /* 送信割り込みイネーブルフラグ */
} HWPORT;



/*
 *  低レベルポート情報管理ブロックの初期値
 */
#define NUM_PORT	1	/* サポートするシリアルポートの数 */

static INT	initflag[1] = { 0 } ;	/* 初期化済フラグ */

#define HWPORT1	{ &initflag[0],  0 }


/*
 *  シリアルI/Oの割込みハンドラの割込み番号
 */
#define INHNO_SERIAL UARTINTLVL



/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
  /*
   *  送受信バッファクリア，割込み許可
   * 
   */
  *UARTLITE_CTREG = UARTLITE_RST_RX_FIFO | UARTLITE_RST_TX_FIFO | UARTLITE_ENABLE_INTR;

  /*
   * INTCの割込み許可
   */
  intc_enable_interrupt((1 << (UARTINTLVL - 1)));
  
  *(p->initflag) = 1;			/* 初期化フラグ設定 */
  return(FALSE);
}


/*
 * シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
  int dummy;
  
  *UARTLITE_CTREG =  ~UARTLITE_ENABLE_INTR; /* 割込み禁止         */
  
  /*
   * すでに割込みが入っている場合があるので割込み要求をクリア
   */
  dummy = 1 << (UARTINTLVL -1);
  intc_ack_interrupt(dummy);
  intc_disable_interrupt(dummy);  /* INTCの受け付け禁止 */
}


/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void	serial_handler_in(ID portid);
extern void	serial_handler_out(ID portid);


/*
 * シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler()
{
  volatile  char  dummy;
  
  if(initflag[0] > 0){
    serial_handler_in(1);
    serial_handler_out(1);
  }else{
    /*
     * 受信割込みの場合はデータを取り出すまで割込みはクリアされない
     * 送信は，バッファが空になると一回だけ割込みを発生するので
     * 特に割込みをクリアする処理を行わなくてもよい 
     */
    dummy = (char)(*UARTLITE_RXREG);
  }
}


/*
 * 文字を受信したか?
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
  return((*UARTLITE_SRREG & UARTLITE_RX_FIFO_VALID_DATA) != 0);
}


/*
 * 文字を送信できるか?
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
  return((*UARTLITE_SRREG & UARTLITE_TX_FIFO_FULL) == 0);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
  return(*UARTLITE_RXREG);
}


/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
  *UARTLITE_TXREG = c;
}


/*
 * 送信制御関数
 * UART Lite には送信割込みを個別に禁止許可する機能はない
 * そのため，送信終了時には必ず1回割込みが入る  
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
  p->sendflag = 1;
}

Inline void
hw_port_sendstop(HWPORT *p)
{
  p->sendflag = 0;
}

#endif /*  _HW_SERIAL_H_ */
