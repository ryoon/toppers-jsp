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
 *  @(#) $Id: serial_mailbox.c,v 1.3 2001/11/12 14:55:23 takayuki Exp $
 */


/*
 * Windowsのメールスロットを使ったシリアルインターフェースモジュール
 *  (注)普通のシリアル(COMデバイス)は使えません
 */

#include <jsp_services.h>
#include "kernel_id.h"

#define TRANSLATE_PORTID(x) \
	x = (x != 0) ? (x) : CONSOLE_PORTID 

#define CHECK_PORTID(x) \
	( (x) > 0 && (x) <= NUM_PORT )

#define CLOSE_HANDLE(x) \
	CloseHandle(x), x=INVALID_HANDLE_VALUE;

#define INHNO_SERIAL	3
#define NUM_PORT 1
#define SIO_POLINTERVAL 300

	/* プロトタイピング */
void	serial_initialize(VP_INT portid);
ER		serial_open(ID portid);
int		serial_close(ID portid, int flush);
ER_UINT serial_read(ID portid, char *buf, unsigned int len);
ER_UINT serial_write(ID portid, char *buf, unsigned int len);
ER		serial_ioctl(ID portid, UINT ioctl);
void	serial_handler(void);

typedef struct serial_port_control_block {
	char *	InDevName;
	HANDLE	InDevHandle;
	ID		InSemaphore;

	char *	OutDevName;
	HANDLE	OutDevHandle;

	HANDLE	MailWatcherHandle;
	DWORD	MailWatcherID;
} SPCB;

SPCB spcb[NUM_PORT] =
{
	{
		"\\\\.\\mailslot\\toppers\\serial_in", INVALID_HANDLE_VALUE, SEM_SERIAL1_IN,
		"\\\\.\\mailslot\\toppers\\serial_out", INVALID_HANDLE_VALUE,
		INVALID_HANDLE_VALUE, 0
	}
};

DWORD WINAPI
MailWatcher(LPVOID param)
{
	SPCB * cb = (SPCB *)param;
	DWORD count;
	char Buffer;

	while(1==1)
	{
		ReadFile(cb->InDevHandle,&Buffer,0,&count,NULL);
		/*{
			count = GetLastError();
			syscall(sig_sem(cb->InSemaphore));
			CloseHandle(cb->MailWatcherHandle);
			cb->MailWatcherHandle = INVALID_HANDLE_VALUE;
			ExitThread(0);
		}*/
		syscall(sig_sem(cb->InSemaphore));
		SuspendThread(GetCurrentThread());
	}
	return 0;
}

Inline void
CloseAllHandles(SPCB * cb)
{
	if(cb->MailWatcherHandle != INVALID_HANDLE_VALUE)
	{
		TerminateThread(cb->MailWatcherHandle,0);
		CLOSE_HANDLE(cb->MailWatcherHandle);

		if(cb->OutDevHandle != INVALID_HANDLE_VALUE)
			CLOSE_HANDLE(cb->OutDevHandle);
	
		if(cb->InDevHandle != INVALID_HANDLE_VALUE)
			CLOSE_HANDLE(cb->InDevHandle);
	}
}


/*
 *  シリアルインタフェースドライバの起動
 */
void
serial_initialize(VP_INT portid)
{
	syscall(serial_open((int)portid));
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


	if((cb->OutDevHandle = CreateFile(cb->OutDevName,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)) == INVALID_HANDLE_VALUE)
	{
		CloseAllHandles(cb);
		return E_OBJ;
	}

	if((cb->InDevHandle = CreateMailslot(cb->InDevName,0,MAILSLOT_WAIT_FOREVER,NULL)) == INVALID_HANDLE_VALUE)
	{
		CloseAllHandles(cb);
		return E_OBJ;
	}

	if((cb->MailWatcherHandle = CreateThread(NULL,0,MailWatcher,(void *)cb,CREATE_SUSPENDED,&cb->MailWatcherID)) == NULL)
	{
		cb->MailWatcherHandle = INVALID_HANDLE_VALUE;
		CloseAllHandles(cb);
		return E_OBJ;
	}

	return E_OK;
}

int
serial_close(ID portid, int flush)
{
	SPCB * cb;

	TRANSLATE_PORTID(portid);
	if(!CHECK_PORTID(portid))
		return E_PAR;

	cb = &spcb[portid-1];
	if(cb->MailWatcherHandle == INVALID_HANDLE_VALUE)
		return E_OBJ;

	if(flush != 0)
		FlushFileBuffers(cb->OutDevHandle);

	CloseAllHandles(cb);

	return E_OK;
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
	if(cb->MailWatcherHandle == INVALID_HANDLE_VALUE)
		return E_OBJ;

	while(len>0)
	{
		GetMailslotInfo(cb->InDevHandle,NULL,&cnt,NULL,NULL);
		if(cnt == MAILSLOT_NO_MESSAGE)
		{
			ResumeThread(cb->MailWatcherHandle);
			syscall(wai_sem(cb->InSemaphore));
		}

		if(ReadFile(cb->InDevHandle,buf,len,&cnt,NULL)==0)
		{
			cnt = GetLastError();
			return E_OBJ;
		}

		len -= cnt;
		buf += cnt;
	}

	return E_OK;
}

ER_UINT
serial_write(ID portid,char * buf, unsigned int len)
{
	SPCB * cb;
	DWORD cnt;

	if(len == 0)
		return 0;
	TRANSLATE_PORTID(portid);
	if(!CHECK_PORTID(portid) || buf == 0l)
		return E_PAR;

	cb = &spcb[portid-1];
	if(cb->MailWatcherHandle == INVALID_HANDLE_VALUE)
		return E_OBJ;

	if(WriteFile(cb->OutDevHandle,buf,len,&cnt,NULL) == 0)
		return E_OBJ;
	
	return (UINT)cnt;
}

int
serial_ioctl(ID portid,UINT ioctl)
{
	return E_OK;
}

void
serial_handler(void)
{}
