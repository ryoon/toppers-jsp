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
 *  @(#) $Id: sys_config.c,v 1.2 2001/11/15 13:18:32 takayuki Exp $
 */

/*
 *	ターゲットシステム依存モジュール（RTE-V850E/MA1-CB用）
 */

#include "jsp_kernel.h"

#include "hw_serial.h"

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
  *(volatile char *)LED1 = 0xff;
  *(volatile char *)LED2 = 0xff;
  
  *(volatile short *)BCT0 = 0x88b8;
  *(volatile short *)DWC0 = 0x1111;
  *(volatile char  *)VSWC = 0x14;
  *(volatile short *)BCC  = 0xffc0;
  *(volatile short *)ASC  = 0x5555;
  *(volatile char  *)BCP  = 0;
  *(volatile short *)CSC0 = 0xfcf3;
  *(volatile short *)CSC1 = 0x2c11;
  *(volatile short *)SCR1 = 0x2096;
  *(volatile short *)RFS1 = 0x8017;

  hw_port_initialize(0);
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
    while( ( *(volatile char *)ASIF(SERIAL_PORT) & 0x2) != 0);
    
    *(volatile char *) TXB(SERIAL_PORT) = c;

    *(volatile unsigned char *)STIC(SERIAL_PORT) &= ~0x80;
}
