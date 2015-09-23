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
 *  @(#) $Id: hw_serial.h,v 1.4 2002/04/14 15:04:35 reiji Exp $
 */


#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include "c54x.h"
#include "sys_config.h"
#include "cpu_config.h"

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL   INT1_VEC


/*
 *  シリアルポートのハードウェア依存情報の定義
 */
#ifndef _MACRO_ONLY
typedef struct hardware_serial_port_descripter{
  INT   *initflag;
  BOOL  sendflag;   /* 送信割り込みイネーブルフラグ */
}HWPORT;
#endif //_MACRO_ONLY

#define NUM_PORT  1       /* サポートするシリアルポートの数 */

static INT  initflag[1] = { 0 }; /* 初期化済フラグ */

#define HWPORT1 { &initflag[0], 0 }



/*
 *  シリアルI/Oポートの初期化
 */
#ifndef _MACRO_ONLY
Inline BOOL
hw_port_initialize(HWPORT *p)
{
  /*  FIFOを有効にし、受信送信FIFOをクリア、
   *  トリガレベル＝１(受信送信共通）       
   */
  UART_FCR=FCR_FIFO | FCR_RFRST | FCR_TFRST | FIFO_TRIG;
  UART_LCR=0x03;  /*  8データ、1ストップビット、no parity */
  UART_LCR|=LCR_DLAB;  /*  ボーレートレジスタにアクセス */
  UART_DLL=UART_BAUD_RATE;
  UART_DLM=UART_BAUD_RATE >> 8;
  UART_LCR&=~LCR_DLAB; /*  BAUD RATE設定終了  */
  UART_SCR=UART_LSR;   /*  エラーフラグをクリア  */
  UART_IER=IER_RIE;       /*  受信割り込み許可  */


  if (initflag[0] <= 0){
    IFR|=(0x1 << INT1);  /* 割り込みフラグのクリア */
    IMR|=(0x1 << INT1);  /* 割り込みマスクレジスタの設定 */
  };

  *(p->initflag) = 1;
  return (FALSE);
}


/*
 *  シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
  *(p->initflag) = -1; /* 初期化フラグ設定 */
  UART_IER=0x0;   /* 送信受信割り込み要求禁止 */
  p->sendflag = 0; 
  IMR&=~(0x1 << INT1); /* 割り込みマスク設定 */
}

/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void serial_handler_in(int portid);
extern void serial_handler_out(int portid);



/*
 *  シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler()
{
  if (initflag[0] > 0){
//      if ( UART_IIR & 0x02 ) {


      serial_handler_out(1);
      serial_handler_in(1);
//      }
//      else if ( !(UART_IIR & 0x0C) ) {

//      }

  }
  else
    UART_IER = 0;             /*  割り込みを禁止  */
}


/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
  return ((UART_LSR & LSR_DR) != 0);
}


/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
  return ((UART_LSR & LSR_THRE) != 0);
}


/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
  return ((byte)UART_RBR & 0x00FF);
}


/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
  UART_THR = c;
}



/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
  if (!(p->sendflag)){
    UART_IER |= IER_TIE;  /* 送信割り込み要求を許可 */
    p->sendflag = 1;
  }
}



Inline void
hw_port_sendstop(HWPORT *p)
{
  if (p->sendflag){
    UART_IER &= ~IER_TIE; /* 送信割り込み要求を禁止 */
    p->sendflag = 0;
  }
}
#endif //_MACRO_ONLY
#endif
