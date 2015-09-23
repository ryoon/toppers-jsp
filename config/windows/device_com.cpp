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
 *  @(#) $Id: device_com.cpp,v 1.5 2001/11/12 14:55:23 takayuki Exp $
 */


#include "sys_defs.h"

#ifdef DEVICE_ENABLE

#include <windows.h>
#define __HAL_MSG_MSGONLY
#include "hal_msg.h"

#include <objbase.h>
#include <unknwn.h>
#include <ocidl.h>

#define DEV_READ  (WM_APP + 0x10)
#define DEV_WRITE (WM_APP + 0x11)
#define DEV_QUIT WM_QUIT

/*
 * デバイスサポート (COMばーじょん)
 *    定義ブロック
 */

//typedef struct _IID
//{
//    unsigned long x;
//    unsigned short s1;
//    unsigned short s2;
//    unsigned char  c[8];
//} IID;

	//諸々のIDの宣言
const IID IID__IKernelEvents = {0x1353969D,0xE84F,0x463F,{0xB2,0x11,0x33,0x7E,0x9B,0xCF,0xB9,0x9E}};
const IID IID_IKernel = {0xD3E42099,0x3FDD,0x4A78,{0xBD,0xBD,0x4E,0x57,0xD3,0x62,0xF5,0xED}};
const CLSID CLSID_Kernel = {0x51789667,0x9F20,0x40AF,{0xAF,0x7F,0x98,0x56,0x32,0x5D,0xFB,0x0B}};


class IKernel : public IUnknown
{
public:
	STDMETHOD(Read)(unsigned long address,unsigned long sz,char __RPC_FAR data[]) PURE;
	STDMETHOD(IsValid)(BOOL __RPC_FAR * valid) PURE;
	STDMETHOD(Write)(unsigned long address,unsigned long sz,char __RPC_FAR data[]) PURE;
};

class IKernelEvents : public IUnknown
{
public:
	STDMETHOD(QueryInterface)(REFIID iid, void ** unk);
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(OnInterruptRequest)(unsigned long inhno);

	IKernelEvents(void);

	long RefCount;
};


	//プロトタイプ宣言 (device_com.hはC言語仕様なので)
extern "C" void _cdecl InitializeDeviceSupportModule(void);
extern "C" void _cdecl FinalizeDeviceSupportModule(void);
extern "C" int _cdecl DeviceRead(unsigned long address, unsigned long size, void * storage);
extern "C" int _cdecl DeviceWrite(unsigned long address, unsigned long size, void * storage);

extern "C" int ena_int(unsigned int);
extern "C" int dis_int(unsigned int);


/*
 * デバイスサポート (COM version)
 *    実装ブロック
 */

static HANDLE WorkerThread;
static DWORD WorkerThreadID;
static HANDLE Blocker;

struct tagParameter
{
	unsigned long address;
	unsigned long size;
	void * storage;
};

bool Advise(IUnknown * container, REFIID iid, IUnknown * sink, DWORD * cookie, IConnectionPoint ** pcp = NULL)
{
	IConnectionPointContainer * cpc;
	IConnectionPoint * cp;

	if(pcp == NULL)
		pcp = &cp;

	container->QueryInterface(IID_IConnectionPointContainer, (void **)&cpc);
	if(cpc == 0)
		return false;

	cpc->FindConnectionPoint(iid, pcp);
	cpc->Release();
	if(*pcp == 0)
		return false;

	if(FAILED((*pcp)->Advise(sink, cookie)))
		return false;

	if(pcp == &cp)
		cp->Release();
	return true;
}

DWORD WINAPI WorkerThreadProcess(LPVOID _param)
{
	IKernel * manager;
	IKernelEvents * sink;
	IConnectionPoint * cp;
	DWORD cookie;
	BOOL Success;
	MSG msg;
	tagParameter * param;

		/*
		 * Initialize
		 */
	::CoInitialize(NULL);

	manager = 0;
	if(FAILED(::CoCreateInstance(CLSID_Kernel, NULL, CLSCTX_ALL, IID_IKernel, (void **)&manager)))
		goto _WorkerThreadProc_Cleanup;

	cp = 0;
	sink = new IKernelEvents;
	if(!Advise(manager, IID__IKernelEvents, sink, &cookie,&cp))
	{
		delete sink;
		goto _WorkerThreadProc_Cleanup;
	}

	manager->IsValid(&Success);
	if(Success != 0)
	{
		::SetEvent(Blocker);

		while( ::GetMessage(&msg, NULL, 0,0) != 0 )
		{
			switch(msg.message)
			{
			case DEV_READ:
				param = (tagParameter *)msg.lParam;
				manager->Read(param->address, param->size, (char *)param->storage);
				break;
			case DEV_WRITE:
				param = (tagParameter *)msg.lParam;
				manager->Write(param->address, param->size, (char *)param->storage);
				break;
			default:
				::DispatchMessage(msg);
			}
			::SetEvent(Blocker);
		}
	}
		/*
		 * Finalize
		 */

	if(cp != 0)
	{
		cp->Unadvise(cookie);
		cp->Release();
		cp = 0;
		sink = 0;
	}


_WorkerThreadProc_Cleanup:

	if(manager != 0)
		manager->Release();
	manager = 0l;
	WorkerThreadID = 0;
	WorkerThread = 0;
	::SetEvent(Blocker);
	::CloseHandle(Blocker);
	Blocker = INVALID_HANDLE_VALUE;
	::CoUninitialize();
	return 0;
}


extern "C"
void _cdecl InitializeDeviceSupportModule(void)
{
	Blocker = ::CreateEvent(NULL,FALSE,FALSE,NULL);
	WorkerThread = ::CreateThread(NULL,0,WorkerThreadProcess,NULL,NULL,&WorkerThreadID);
	::WaitForSingleObject(Blocker,INFINITE);
}

extern "C"
void _cdecl FinalizeDeviceSupportModule(void)
{
	::PostThreadMessage(WorkerThreadID,DEV_QUIT,0,0);
	::WaitForSingleObject(WorkerThread,INFINITE);
}

extern "C"
int _cdecl DeviceRead(unsigned long address, unsigned long size, void * storage)
{
	tagParameter param;

	if(WorkerThreadID == 0)
		return -1;
	
	dis_int(0);
	param.address = address;
	param.size = size;
	param.storage = storage;
	::PostThreadMessage(WorkerThreadID, DEV_READ, 0, (LPARAM)&param);
	::WaitForSingleObject(Blocker,INFINITE);
	ena_int(0);

	return size;
}

extern "C"
int _cdecl DeviceWrite(unsigned long address, unsigned long size, void * storage)
{
	tagParameter param;

	if(WorkerThreadID == 0)
		return -1;

	dis_int(0);
	param.address = address;
	param.size = size;
	param.storage = storage;
	::PostThreadMessage(WorkerThreadID, DEV_WRITE, 0, (LPARAM)&param);
	::WaitForSingleObject(Blocker,INFINITE);
	ena_int(0);

	return size;
}


/*
 *   IKernelEvents実装
 */

IKernelEvents::IKernelEvents(void) : RefCount(0)
{}

HRESULT IKernelEvents::QueryInterface(REFIID iid, void ** unk)
{
	if( iid == IID__IKernelEvents || iid == IID_IUnknown )
	{
		*unk = this;
		::InterlockedIncrement(&RefCount);
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG IKernelEvents::AddRef()
{	return ::InterlockedIncrement(&RefCount);	}

ULONG IKernelEvents::Release()
{
	if( ::InterlockedDecrement(&RefCount) == 0)
		delete this;
	return RefCount;
}

extern "C" HANDLE PrimaryDialogHandle;
HRESULT IKernelEvents::OnInterruptRequest(unsigned long inhno)
{
	::PostMessage((HWND)PrimaryDialogHandle,HALMSG_MESSAGE,HALMSG_INTERRUPT,(LPARAM)inhno);
	return S_OK;
}

#endif
