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
 *  @(#) $Id: hw_serial.h,v 1.1 2000/11/24 03:41:21 takayuki Exp $
 */

/*
 *	ターゲット依存シリアルI/Oモジュール（DVE68K/40用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include "sys_config.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct hardware_serial_port_descripter {
} HWPORT;


#define HWPORT1

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL	1

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	0	/* サポートするシリアルポートの数 */

/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
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
 *  シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler()
{
}

/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	return (FALSE);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	return FALSE;
}

/*
 *  受信した文字の取り出し
 */
Inline unsigned char
hw_port_getchar(HWPORT *p)
{
	return 0;
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, unsigned char c)
{
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

#endif /* _HW_SERIAL_H_ */
