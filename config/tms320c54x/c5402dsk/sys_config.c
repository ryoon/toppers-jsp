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
 *  @(#) $Id: sys_config.c,v 1.2 2002/04/12 12:36:31 reiji Exp $
 */


#include "jsp_kernel.h"
#include "c54x.h"


INT board_id;
INT board_addr;

void
sys_initialize(void)
{
/* sys_putcにprintfではなくシリアルへのポーリングでの
 * 送信を行う場合の初期化処理
 */

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

}

void
sys_exit(void)
{
}


void
sys_putc(char c)
{
  if(c == '\n'){
    uart_putc('\r');
  }
  uart_putc(c);
}





