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
 *  @(#) $Id: stdafx.cpp,v 1.3 2002/04/10 11:27:26 takayuki Exp $
 */

/*
 * デバイスマネージャ 本体(CExeModule)
 */

#pragma warning(disable:4786)

#include "stdafx.h"

#ifdef _ATL_STATIC_REGISTRY
#include <statreg.h>
#include <statreg.cpp>
#endif

#include <atlimpl.cpp>
#include <algorithm>

#include <initguid.h>
#include "devicemanager.h"
#include "device.h"
#include "kernel.h"
#include "information.h"

using namespace std;

DWORD Count;
char Buffer[1024];
#define DebugOut(x) ::WriteFile(::GetStdHandle(STD_OUTPUT_HANDLE),Buffer, (x), &Count, NULL)


CExeModule::CExeModule(void) : Kernel(0)
{
	::InitializeCriticalSection(&cs);
}


DeviceID CExeModule::AddDevice(CDevice *device)
{
	list<CDevice *>::iterator scope;
	list<CInformation *>::iterator view;

	::EnterCriticalSection(&cs);

	scope = find(DeviceList.begin(), DeviceList.end(), device);
	if(scope != DeviceList.end())
	{
		::LeaveCriticalSection(&cs);
		return 0;
	}

	DebugOut(::wsprintfA(Buffer,"AddDevice\n"));
	view = ViewerList.begin();
	while(view != ViewerList.end())
	{
		(*view)->Fire_OnDeviceChanged(1,reinterpret_cast<long>(device));
		view ++;
	}

	DeviceList.push_back(device);

	::LeaveCriticalSection(&cs);
	return reinterpret_cast<DeviceID>(device);
}

bool CExeModule::DeleteDevice(CDevice *device)
{
	list<CDevice *>::iterator dev;
	map<Range,CDevice *>::iterator mem[2];

	::EnterCriticalSection(&cs);

	DebugOut(::wsprintfA(Buffer,"DelDevice\n"));

	dev = find(DeviceList.begin(), DeviceList.end(), device);
	if(dev == DeviceList.end())
	{
		::LeaveCriticalSection(&cs);
		return false;
	}

	mem[0] = DeviceMap.begin();
	while(mem[0] != DeviceMap.end())
	{
		if((*mem[0]).second == device)
		{
			mem[1] = mem[0];
			mem[0] ++;

			DeviceMap.erase(mem[1]);
		}else
			mem[0] ++;
	}

	DeviceList.erase(dev);

	list<CInformation *>::iterator view;
	view = ViewerList.begin();
	while(view != ViewerList.end())
	{
		(*view)->Fire_OnDeviceChanged(2,reinterpret_cast<long>(device));
		view ++;
	}

	::LeaveCriticalSection(&cs);
	return true;
}

bool CExeModule::Map(CDevice *device, Range area)
{
	map<Range,CDevice *>::iterator scope;

	::EnterCriticalSection(&cs);
	

	scope = DeviceMap.begin();
	while(scope != DeviceMap.end())
	{
		if( area.IsIncludedIn((*scope).first) )
		{
			::LeaveCriticalSection(&cs);
			return false;
		}
		scope ++;
	}

	DeviceMap[area] = device;

	DebugOut(::wsprintfA(Buffer,"Map\n"));
	list<CInformation *>::iterator view;
	view = ViewerList.begin();
	while(view != ViewerList.end())
	{
		(*view)->Fire_OnDeviceChanged(3,reinterpret_cast<long>(device),area.GetBase());
		view ++;
	}

	::LeaveCriticalSection(&cs);
	return true;
}


bool CExeModule::Unmap(CDevice *device, unsigned long address)
{
	map<Range,CDevice *>::iterator scope;
	Range work(address);

	::EnterCriticalSection(&cs);

	scope = DeviceMap.begin();
	while(scope != DeviceMap.end())
	{
		if( (*scope).second == device && work.IsIncludedIn((*scope).first) )
		{
			DeviceMap.erase(scope);

			DebugOut(::wsprintfA(Buffer,"Unmap\n"));
			list<CInformation *>::iterator view;
			view = ViewerList.begin();
			while(view != ViewerList.end())
			{
				(*view)->Fire_OnDeviceChanged(4,reinterpret_cast<long>(device),address);
				view ++;
			}

			::LeaveCriticalSection(&cs);

			return true;
		}
		scope ++;
	}
	::LeaveCriticalSection(&cs);

	return false;
}

void CExeModule::AddViewer(CInformation *view)
{
	list<CInformation *>::iterator scope;

	::EnterCriticalSection(&cs);
	scope = find(ViewerList.begin(), ViewerList.end(), view);
	if(scope == ViewerList.end())
		ViewerList.push_back(view);
	::LeaveCriticalSection(&cs);
}

void CExeModule::RemoveViewer(CInformation *view)
{
	list<CInformation *>::iterator scope;

	::EnterCriticalSection(&cs);
	scope = find(ViewerList.begin(), ViewerList.end(), view);
	if(scope != ViewerList.end())
		ViewerList.erase(scope);
	::LeaveCriticalSection(&cs);
}

bool CExeModule::AttachKernel(CKernel *kernel)
{
	bool result;
	list<CDevice *>::iterator scope;
	list<CInformation *>::iterator view;

	DebugOut(::wsprintfA(Buffer,"AttachKernel\n"));

	::EnterCriticalSection(&cs);
	if( (result = Kernel == 0) )
	{
		Kernel = kernel;

		scope = DeviceList.begin();
		while(scope != DeviceList.end())
		{
			(*scope)->Fire_OnKernelStart();
			scope ++;
		}

		view = ViewerList.begin();
		while(view != ViewerList.end())
		{
			(*view)->Fire_OnKernelChanged(1);
			view ++;
		}

	}
	::LeaveCriticalSection(&cs);

	return result;
}

bool CExeModule::DetachKernel(CKernel *kernel)
{
	bool result;
	list<CDevice *>::iterator scope;
	list<CInformation *>::iterator view;

	DebugOut(::wsprintfA(Buffer,"DetachKernel\n"));

	::EnterCriticalSection(&cs);
	if( (result = Kernel == kernel) )
	{
		scope = DeviceList.begin();
		while(scope != DeviceList.end())
		{
			(*scope)->Fire_OnKernelExit();
			scope ++;
		}

		view = ViewerList.begin();
		while(view != ViewerList.end())
		{
			(*view)->Fire_OnKernelChanged(2);
			view ++;
		}

		Kernel = 0l;
	}
	::LeaveCriticalSection(&cs);

	return result;
}

bool CExeModule::Read(CKernel * kernel, unsigned long address, unsigned long sz, unsigned char data[])
{
	bool result;
	map<Range,CDevice *>::iterator scope;

	DebugOut(::wsprintfA(Buffer,"Read\n"));

	::EnterCriticalSection(&cs);

	if(Kernel != kernel)
	{
		::LeaveCriticalSection(&cs);

		DebugOut(::wsprintfA(Buffer,"Illegal Kernel\n"));
		
		return false;
	}

	scope = DeviceMap.find(Range(address,sz));
	
	result = (scope != DeviceMap.end());
	if(result)
		(*scope).second->Fire_OnRead(address, sz, data);

	::LeaveCriticalSection(&cs);

	return result;

}

bool CExeModule::Write(CKernel *kernel, unsigned long address, unsigned long sz, unsigned char *data)
{
	bool result;
	map<Range,CDevice *>::iterator scope;

	DebugOut(::wsprintfA(Buffer,"Write\n"));

	::EnterCriticalSection(&cs);

	if(Kernel != kernel)
	{
		::LeaveCriticalSection(&cs);
		DebugOut(::wsprintfA(Buffer,"Illegal Kernel\n"));
		return false;
	}

	scope = DeviceMap.find(Range(address,sz));
	
	result = (scope != DeviceMap.end());
	if(result)
	{
		DebugOut(::wsprintfA(Buffer,"FireWrite\n"));
		(*scope).second->Fire_OnWrite(address, sz,data);
	}
	::LeaveCriticalSection(&cs);

	return result;
}

bool CExeModule::RaiseInterrupt(unsigned long inhno)
{
	bool result = false;

	::EnterCriticalSection(&cs);

	if(Kernel != 0 && Kernel->Fire_OnInterruptRequest(inhno) >= 0)
		result = true;

	::LeaveCriticalSection(&cs);

	return result;
}
