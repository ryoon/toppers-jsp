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
 *  @(#) $Id: serial_pipe.c,v 1.2 2001/11/12 14:55:23 takayuki Exp $
 */

/*
 * Windowsのパイプを使ったシリアルインターフェースモジュール
 *  (注)普通のシリアルは使えません
 */

#include "Debug.h"
#include <jsp_services.h>
#include "kernel_id.h"

#define TRANSLATE_PORTID(x) \
	x = (x != 0) ? (x) : CONSOLE_PORTID 

#define CHECK_PORTID(x) \
	( (x) > 0 && (x) <= NUM_PORT )

#define INHNO_SERIAL	3
#define NUM_PORT 1
#define SIO_POLINTERVAL 300

	/* プロトタイピング */
void	serial_initialize(ID portid);
ER		serial_open(ID portid);
int		serial_close(ID portid, int flush);
ER_UINT serial_read(ID portid, char *buf, unsigned int len);
ER_UINT serial_write(ID portid, char *buf, unsigned int len);
int		serial_ioctl(ID portid, int req, int arg);
void	serial_handler(void);


typedef struct serial_port_control_block {
	char *	DeviceName;
	HANDLE	DeviceHandle;
	ID		InSem;
	HANDLE	PipeWatcherHandle;
	DWORD	PipeWatcherID;
} SPCB;

SPCB spcb[NUM_PORT] =
{
	{ "\\\\.\\pipe\\toppersserial", INVALID_HANDLE_VALUE, SEM_SERIAL1_IN, INVALID_HANDLE_VALUE, 0 }
};

DWORD WINAPI
PipeWatcher(LPVOID param)
{
	SPCB * cb = (SPCB *)param;
	DWORD count;
	char Buffer;

	while(1==1)
	{
		if(ReadFile(cb->DeviceHandle,&Buffer,0,&count,NULL)==0)
		{
			syscall(sig_sem(cb->InSem));
			ExitThread(0);
		}
		syscall(sig_sem(cb->InSem));
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

/*
 *  シリアルインタフェースドライバの起動
 */
void
serial_initialize(ID portid)
{
	syscall(serial_open(portid));
	syslog(LOG_NOTICE, "Serial driver service starts on port %d.", portid);
}

ER
serial_open(ID portid)
{
	SPCB * cb;

	TRANSLATE_PORTID(portid);
	if(!CHECK_PORTID(portid))
		return E_PAR;

	cb = &spcb[portid-1];
	cb->DeviceHandle = CreateFile(cb->DeviceName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	cb->PipeWatcherHandle = CreateThread(NULL,0,PipeWatcher,(LPVOID)cb,0,&cb->PipeWatcherID);

	return (cb->DeviceHandle != INVALID_HANDLE_VALUE) ? E_OK : E_OBJ;
}

int
serial_close(ID portid, int flush)
{
	SPCB * cb;

	TRANSLATE_PORTID(portid);
	if(!CHECK_PORTID(portid))
		return E_PAR;

	cb = &spcb[portid-1];
	if(cb->DeviceHandle == INVALID_HANDLE_VALUE)
		return E_OBJ;

	if(flush != 0)
		FlushFileBuffers(cb->DeviceHandle);
	CloseHandle(cb->DeviceHandle);
	TerminateThread(cb->PipeWatcherHandle,0);
	CloseHandle(cb->PipeWatcherHandle);
	cb->DeviceHandle = INVALID_HANDLE_VALUE;

	return (cb->DeviceHandle != INVALID_HANDLE_VALUE) ? E_OK : E_OBJ;
}

ER_UINT
serial_read(ID portid, char * buf, unsigned int len)
{
	SPCB * cb;
	DWORD cnt;

	if(len == 0)
		return 0;
	TRANSLATE_PORTID(portid);
	if(!CHECK_PORTID(portid) || buf == 0l)
		return E_PAR;

	cb = &spcb[portid-1];
	if(cb->DeviceHandle == INVALID_HANDLE_VALUE)
		return E_OBJ;

	DebugOut(wsprintf(DebugBuffer,"SERIAL_READ"));

	do{
		PeekNamedPipe(cb->DeviceHandle,NULL,NULL,NULL,&cnt,NULL);
		if(cnt < len)
		{
			ResumeThread(cb->PipeWatcherHandle);
			syscall(wai_sem(cb->InSem));
		}
	}while(cnt < len);


	if(ReadFile(cb->DeviceHandle,buf,len,&cnt,NULL) == 0)
		return E_OBJ;
	
	return (UINT)cnt;
}

ER_UINT
serial_write(ID portid,char * buf, unsigned int len)
{
	SPCB * cb;
	DWORD cnt;
	OVERLAPPED ovr;

	if(len == 0)
		return 0;
	TRANSLATE_PORTID(portid);
	if(!CHECK_PORTID(portid) || buf == 0l)
		return E_PAR;

	cb = &spcb[portid-1];
	if(cb->DeviceHandle == INVALID_HANDLE_VALUE)
		return E_OBJ;

	DebugOut(wsprintf(DebugBuffer,"SERIAL_WRITE"));

	ovr.hEvent = INVALID_HANDLE_VALUE;
	if(WriteFile(cb->DeviceHandle,buf,len,&cnt,&ovr) == 0)
		return E_OBJ;
	
	return (UINT)cnt;
}

int
serial_ioctl(ID portid,int req,int arg)
{
	return E_OK;
}

void
serial_handler(void)
{}
