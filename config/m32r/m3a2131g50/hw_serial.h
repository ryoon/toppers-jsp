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
 *  @(#) $Id: hw_serial.h,v 1.2 2002/04/05 09:01:29 takayuki Exp $
 */

/*
 *	ターゲット依存シリアルI/Oモジュール（M32102内蔵非同期シリアル用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include "sys_config.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */

typedef struct hardware_serial_port_descripter {
} HWPORT;
#define HWPORT1 {}

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL_IN		(48 + SERIAL_PORT)
#define INHNO_SERIAL_OUT	(49 + SERIAL_PORT)

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
  			/* 非同期シリアル通信路の初期化 */
	*(volatile unsigned char *)(SIOCR(SERIAL_PORT)+3) = 0;

			/* ポートを開ける */
	*(volatile unsigned char  *) PDATA(5) = 0;
	*(volatile unsigned char  *) PDIR(5)  = 0x80;	//TxDを出力に

			//UART, Non-parity, 1 stop-bit
	*(volatile unsigned char  *)(SIOMOD0(SERIAL_PORT)+3) = 0;	

			//8bit, internal clock
	*(volatile unsigned short *)(SIOMOD1(SERIAL_PORT)+2) = 0x0800;

			//M32R(32102) - f(BLK)=16MHzで115200bps
	*(volatile unsigned short *)(SIOBAUR(SERIAL_PORT)+2)  = SERIAL_CLKDIV;
	*(volatile unsigned char  *)(SIORBAUR(SERIAL_PORT)+3) = SERIAL_CLKCMP;

			//送受信割込み発生許可
	*(volatile unsigned short *)(SIOTRCR(SERIAL_PORT)+2) = 0x6;

			//ステータスクリア, 送受信動作開始
	*(volatile unsigned char  *)(SIOCR(SERIAL_PORT)+2) = 0x3;
	*(volatile unsigned char  *)(SIOCR(SERIAL_PORT)+3) = 0x3;

			//受信割込み受付許可
	*(volatile unsigned short *)(ICUCR(SIO,SERIAL_PORT)+2) = 0x1000;
	*(volatile unsigned short *)(ICUCR(SIO,SERIAL_PORT)+6) = 0x1000;

		//TxD,RxDピン有効
	*(volatile unsigned short *) PMOD(5) = 0x5500;

	return(FALSE);
}

/*
 *  シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
		//送受信動作停止
	*(volatile unsigned char *)(SIOCR(SERIAL_PORT)+3) = 0;
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
	return ((*(volatile char *)(SIOSTS(SERIAL_PORT)+3)) & 0x4) != 0 ? TRUE : FALSE;
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	return ((*(volatile char *)(SIOSTS(SERIAL_PORT)+3)) & 0x1) != 0 ? TRUE : FALSE;
}

/*
 *  受信した文字の取り出し
 */
Inline unsigned char
hw_port_getchar(HWPORT *p)
{	return *(volatile unsigned char *)(SIORXB(SERIAL_PORT)+3);	}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, unsigned char c)
{
	*(volatile unsigned char *)(SIOTXB(SERIAL_PORT)+3) = c;
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{}

Inline void
hw_port_sendstop(HWPORT *p)
{}

Inline void
hw_serial_handler_in(void)
{
    serial_handler_in(CONSOLE_PORTID);
}

Inline void
hw_serial_handler_out(void)
{
	*(volatile unsigned char  *)(SIOCR(SERIAL_PORT)+2) = 0x1;
	serial_handler_out(CONSOLE_PORTID);
}

#endif /* _HW_SERIAL_H_ */
