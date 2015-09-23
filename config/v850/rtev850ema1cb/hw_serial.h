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
 *  @(#) $Id: hw_serial.h,v 1.2 2001/11/15 13:18:32 takayuki Exp $
 */

/*
 *	ターゲット依存シリアルI/Oモジュール（V850E/MA1 内蔵非同期シリアル用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include "sys_config.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */

#define FLG_RECEIVED 1

typedef struct hardware_serial_port_descripter {
} HWPORT;
#define HWPORT1 {}

static unsigned int _serial_flag = 0;

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL_IN		(0x2e + (SERIAL_PORT * 4))
#define INHNO_SERIAL_OUT	(0x2f + (SERIAL_PORT * 4))

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	1	/* サポートするシリアルポートの数 */

/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
	if( (*(volatile unsigned char *) ASIM(SERIAL_PORT) & 0x80) == 0 )
	{
	    	/* 非同期シリアル通信路の初期化 */

			/* ポートを開ける */
		*(volatile unsigned char *) PFC4 = 0x1b;
		*(volatile unsigned char *) PMC4 = 7 << (3*SERIAL_PORT);

			/* ボーレート発生器の初期化 */
		*(volatile unsigned char *) CKSR(SERIAL_PORT)  = SERIAL_CLKDIV;
		*(volatile unsigned char *) BRGC(SERIAL_PORT)  = SERIAL_COMPAREVALUE;
		*(volatile unsigned char *) ASIM(SERIAL_PORT) |= 0x80;	//enable ASIO

		__asm("nop; nop");

		*(volatile unsigned char *) ASIM(SERIAL_PORT) |= 0x04;	//8bit length

		__asm("nop; nop");

		*(volatile unsigned char *) ASIM(SERIAL_PORT) |= 0x60;	//Tx, Rx on

			/* 割込み制御 */
		*(volatile unsigned char *)SRIC(SERIAL_PORT) = 0 | SERIAL_INT_PRIORITY;
		*(volatile unsigned char *)STIC(SERIAL_PORT) = 0 | SERIAL_INT_PRIORITY;
	}
	return(FALSE);
}

/*
 *  シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
}

/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void	serial_handler_in(int portid);
extern void	serial_handler_out(int portid);

/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	return (_serial_flag & FLG_RECEIVED) != 0 ? TRUE : FALSE;
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	return (*(volatile char *)ASIF(SERIAL_PORT) & 0x2) != 0 ? FALSE : TRUE;
}

/*
 *  受信した文字の取り出し
 */
Inline unsigned char
hw_port_getchar(HWPORT *p)
{
	_serial_flag &= FLG_RECEIVED;
	*(volatile unsigned char *) SRIC(SERIAL_PORT) &= 0x7f;
	return *(volatile unsigned char *)RXB(SERIAL_PORT);
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, unsigned char c)
{
	*(volatile unsigned char *) TXB(SERIAL_PORT) = c;
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
}

Inline void
hw_port_sendstop(HWPORT *p)
{
}

Inline void
hw_serial_handler_in(void)
{
	_serial_flag |= FLG_RECEIVED;
    serial_handler_in(CONSOLE_PORTID);
}

Inline void
hw_serial_handler_out(void)
{
	serial_handler_out(CONSOLE_PORTID);
}

#endif /* _HW_SERIAL_H_ */
