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
 *  @(#) $Id: hw_serial.h,v 1.3 2002/04/10 11:20:09 takayuki Exp $
 */


/*
 *  TOPPERS/JSP for Windows - シリアルエミュレータ
 */

#ifndef __HW_SERIAL_H
#define __HW_SERIAL_H

#include "itron.h"
#include "sys_config.h"
#include "vwindows.h"

typedef struct hardware_port_descriptor
{
	int    Flag;
	char * DeviceName;
	HANDLE Handle;
} HWPORT;

	/* デバイスを開く側の設定 */
#define SIO_SERVER		0x00000001
#define SIO_CLIENT		0x00000000

	/* デバイスの種別 */
#define SIO_PIPE	    0x00000010
/*#define SIO_COMOBJECT	0x000000f0 */

#define INHNO_SERIAL	3
#define NUM_PORT 1
#define HWPORT1	{ SIO_PIPE|SIO_CLIENT, "\\\\.\\pipe\\toppersserial", INVALID_HANDLE_VALUE }

extern BOOL hw_port_initialize(HWPORT *p);
extern void hw_port_terminate(HWPORT *p);
extern BOOL hw_port_getready(HWPORT *p);
extern BOOL hw_port_putready(HWPORT *p);
extern byte hw_port_getchar(HWPORT *p);
extern void hw_port_putchar(HWPORT *p, byte c);
extern void hw_port_sendstart(HWPORT *p);
extern void hw_port_sendstop(HWPORT *p);

#endif