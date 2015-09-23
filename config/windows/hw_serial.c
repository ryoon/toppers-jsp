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
 *  @(#) $Id: hw_serial.c,v 1.1 2000/11/14 16:31:38 takayuki Exp $
 */
#include "hw_serial.h"


extern HANDLE PrimaryThreadHandle;

/*
 * シリアル(相当)デバイスの初期化
 */

BOOL
hw_port_initialize(HWPORT *p)
{
	if(p->Handle != INVALID_HANDLE_VALUE)
		CloseHandle(p->Handle);

	if( (p->Flag & SIO_SERVER) != 0)
	{
		p->Handle = CreateNamedPipe(p->DeviceName,PIPE_ACCESS_DUPLEX|FILE_FLAG_OVERLAPPED,PIPE_TYPE_BYTE|PIPE_NOWAIT,PIPE_UNLIMITED_INSTANCES,2048,2048,0,NULL);
	}else
	{
		p->Handle = CreateFile(p->DeviceName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	}
	if(p->Handle != INVALID_HANDLE_VALUE)
	{
		SetTimer(PrimaryThreadHandle,(0xffff0000|INHNO_SERIAL),100,NULL);
		return FALSE;
	}
	return TRUE;
}

/*
 * シリアル(相当)デバイスのシャットダウン
 */

void
hw_port_terminate(HWPORT *p)
{
	if(p->Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(p->Handle);
		p->Handle = INVALID_HANDLE_VALUE;
	}
}

/*
 *  文字を受信したか？
 */
BOOL
hw_port_getready(HWPORT *p)
{
	DWORD charinbuf;
	if((p->Flag & SIO_PIPE) != 0)
	{
		PeekNamedPipe(p->Handle,NULL,0,NULL,&charinbuf,NULL);
		return (charinbuf != 0) ? TRUE : FALSE;
	}
	return FALSE;
}

/*
 *  文字を送信できるか？
 */
BOOL
hw_port_putready(HWPORT *p)
{
		/*書いてみないとわかんないので常に書けると判断*/
	return TRUE;
}

/*
 *  受信した文字の取り出し
 */
byte
hw_port_getchar(HWPORT *p)
{
	DWORD chars;
	char Buffer;
	ReadFile(p->Handle,&Buffer,1,&chars,NULL);
    return Buffer;
}

/*
 *  送信する文字の書き込み
 */
void
hw_port_putchar(HWPORT *p, byte c)
{
	DWORD chars;
	WriteFile(p->Handle,&c,1,&chars,NULL);
}

/*
 *  送信制御関数
 */
void
hw_port_sendstart(HWPORT *p)
{}

void
hw_port_sendstop(HWPORT *p)
{}

