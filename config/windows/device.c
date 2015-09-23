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
 *  @(#) $Id: device.c,v 1.3 2002/04/10 11:20:09 takayuki Exp $
 */

#include "device.h"
#include "messages.h"
#include "hal_msg.h"
#include "cpu_defs.h"

	//定数値の宣言 (ローカルスコープ)
#define BLOCK 1
#define NONBLOCK 0

	//大域変数の宣言
struct tagDeviceGlobalVariables DeviceVars;

	//大域変数の宣言 (ローカルスコープ)
static unsigned char MessageBuffer[MAX_MSGSIZE];
static DWORD MessageSize;


Inline Lock(void)
{
	dis_int(0);
	//EnterCriticalSection(&DeviceVars.cs);	
}

Inline Unlock(void)
{
	ena_int(0);
	LeaveCriticalSection(&DeviceVars.cs);	
}
	
static BOOL Send(void * data, unsigned long size, int mode)
{
	DWORD count;

	if(WriteFile(DeviceVars.ManagerSlot, data, size, &count,NULL) == 0)
		return FALSE;

	if(mode == BLOCK)
		if(WaitForSingleObject(DeviceVars.Blocker, DEVICE_TIMEOUT) != WAIT_OBJECT_0)
			return FALSE;

	return TRUE;
}


static DWORD WINAPI ListenerThread(LPVOID param)
{
	while(1==1)
	{
		if(ReadFile(DeviceVars.ReplySlot,MessageBuffer,MAX_MSGSIZE,&MessageSize,NULL) == 0)
			break;
		
		switch(MessageBuffer[0])
		{
		case MSG_ACK:
		case MSG_NAK:
			SetEvent(DeviceVars.Blocker);
			WaitForSingleObject(DeviceVars.ThreadBlocker,INFINITE);
			break;

		case MSG_KERREP_INTERRUPT:
			{
				char msg = MSG_ACK;
				HALInterruptRequest(*(int *)&MessageBuffer[1]);
				Send(&msg,1,NONBLOCK);
				break;
			}

		default:
			if( (MessageBuffer[0] & 0x80) != 0)
			{
					//返答要求にNAKで応える
				char msg;
				msg = MSG_NAK;
				Send(&msg,1,NONBLOCK);
			}
		}
	}

	return 0;
}

void InitializeDeviceSupportModule(void)
{
	unsigned char work = 0xff;
	char buffer[128];
	int i;

	InitializeCriticalSection(&DeviceVars.cs);

		//メールスロット名の決定
	i = wsprintf(buffer+1,"\\\\.\\mailslot\\toppers\\kernel%08x\\deviceport",GetCurrentProcessId())+1;

	work &= (DeviceVars.ReplySlot = CreateMailslot(buffer+1,MAX_MSGSIZE,MAILSLOT_WAIT_FOREVER,NULL)) != INVALID_HANDLE_VALUE;
	work &= (DeviceVars.Blocker = CreateEvent(NULL,FALSE,FALSE,NULL)) != INVALID_HANDLE_VALUE;
	work &= (DeviceVars.ThreadBlocker = CreateEvent(NULL,FALSE,FALSE,NULL)) != INVALID_HANDLE_VALUE;
	work &= (DeviceVars.ManagerSlot = CreateFile("\\\\.\\mailslot\\toppers\\device\\devicemanager",GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)) != INVALID_HANDLE_VALUE;

	if(work)
		work &= (DeviceVars.ListenerThread = CreateThread(NULL,0,ListenerThread,NULL,NULL,&DeviceVars.ListenerThreadID)) != NULL;

	if(!work)
	{
		CloseHandle(DeviceVars.ManagerSlot);
		CloseHandle(DeviceVars.Blocker);
		CloseHandle(DeviceVars.ThreadBlocker);
		return;
	}

	*(unsigned char *)&buffer[0] = MSG_KERREQ_KERNELSTART;
	Send(&buffer,i,BLOCK);
	SetEvent(DeviceVars.ThreadBlocker);
}

void FinalizeDeviceSupportModule(void)
{
	unsigned char msg;

	Lock();
	if(DeviceVars.ListenerThread != NULL)
	{
		msg = MSG_KERREQ_KERNELEXIT;
		Send(&msg,1,BLOCK);

		TerminateThread(DeviceVars.ListenerThread,0);
		WaitForSingleObject(DeviceVars.ListenerThread,INFINITE);
		CloseHandle(DeviceVars.ListenerThread);
		DeviceVars.ListenerThread = NULL;

		CloseHandle(DeviceVars.Blocker);
		CloseHandle(DeviceVars.ThreadBlocker);
		CloseHandle(DeviceVars.ManagerSlot);
		CloseHandle(DeviceVars.ReplySlot);
	}
	Unlock();
	DeleteCriticalSection(&DeviceVars.cs);
}

int DeviceRead(unsigned long address, unsigned long size, void * storage)
{
	unsigned char buffer[16];
	int result;

	if(size > MAX_MSGSIZE-9)
		return -1;

	Lock();

	if(DeviceVars.ListenerThread == NULL)
	{
		Unlock();
		return -1;
	}

	buffer[0] = MSG_KERREQ_READ;
	*(unsigned long *)&buffer[1] = address;
	*(unsigned long *)&buffer[5] = size;
	
	Send(buffer,9,BLOCK);

	if( MessageBuffer[0] == 0 )
	{
		result = MessageSize - 1;

		if((unsigned)result > size)
			result = size;

		MoveMemory(storage, &MessageBuffer[1], result);
	}else
		result = -1;

	SetEvent(DeviceVars.ThreadBlocker);
	Unlock();

	return result;
}

int DeviceWrite(unsigned long address, unsigned long size, void * storage)
{
	unsigned char buffer[MAX_MSGSIZE];
	int result;

	if(size > MAX_MSGSIZE-9)
		return -1;

	Lock();

	if(DeviceVars.ListenerThread == NULL)
	{
		Unlock();
		return -1;
	}

	buffer[0] = MSG_KERREQ_WRITE;
	*(unsigned long *)&buffer[1] = address;
	*(unsigned long *)&buffer[5] = size;
	MoveMemory( &buffer[9], storage, size);
	
	Send(buffer, 9+size, BLOCK);

	result = MessageBuffer[0] == 0 ? size : -1;

	SetEvent(DeviceVars.ThreadBlocker);
	Unlock();

	return result;
}
