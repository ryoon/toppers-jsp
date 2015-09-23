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
 *  @(#) $Id: devicecontrol.cpp,v 1.1 2001/02/23 12:33:45 takayuki Exp $
 */

#include "stdafx.h"
#include "device.h"
#include "devicecontrol.h"

/////////////////////////////////////////////////////////////////////////////
// CDeviceControl

const CLSID CLSID_Device = {0xFEE2A68B,0x7F98,0x40E0,{0x98,0x53,0x18,0x3E,0xE6,0x8B,0xC7,0xF8}};

__declspec(naked) void * __fastcall MemoryCopy(void * dest, void * src, unsigned int size)
{
	__asm	mov	eax, ecx
	__asm	mov edi, ecx
	__asm	mov esi, edx
	__asm	pop ebx
	__asm	pop ecx
	__asm	rep movsb
	__asm	jmp ebx
}

STDMETHODIMP CDeviceControl::Connect()
{
	if(!FAILED(manager.CoCreateInstance(CLSID_Device)) && !FAILED(manager.Advise((_IDeviceEvents *)this,__uuidof(_IDeviceEvents),&cookie)))
		return S_OK;

	if(! !manager)
		manager.Release();
	return E_FAIL;
}

STDMETHODIMP CDeviceControl::Close()
{
	if(!manager)
		return E_FAIL;

	AtlUnadvise(manager,__uuidof(_IDeviceEvents),cookie);
	manager.Release();
	cookie = 0;
	return S_OK;
}

STDMETHODIMP CDeviceControl::get_Valid(BOOL *pVal)
{
	*pVal = !manager ? FALSE : ~0;
	return S_OK;
}

STDMETHODIMP CDeviceControl::RaiseInterrupt(long inhno)
{
	if(!manager)
		return E_FAIL;
	
	return manager->RaiseInterrupt(inhno);
}

STDMETHODIMP CDeviceControl::Map(long address, long size)
{
	if(!manager)
		return E_FAIL;

	return manager->Map(address, size);
}

STDMETHODIMP CDeviceControl::Unmap(long address)
{
	if(!manager)
		return E_FAIL;

	return manager->Unmap(address);
}


STDMETHODIMP CDeviceControl::get_IsKernelStarted(BOOL *pVal)
{
	*pVal = KernelStarted ? ~0 : 0;
	return S_OK;
}

STDMETHODIMP CDeviceControl::get_Offset(long *pVal)
{
	*pVal = Offset;
	return S_OK;
}

STDMETHODIMP CDeviceControl::put_Offset(long newVal)
{
	if(newVal >= Limitation)
		return E_FAIL;

	Offset = newVal;

	return S_OK;
}

STDMETHODIMP CDeviceControl::get_AccessSize(short *pVal)
{
	*pVal = AccessSize;
	return S_OK;
}

STDMETHODIMP CDeviceControl::put_AccessSize(short newVal)
{
	switch(newVal)
	{
	case 1: case 2: case 4:
		break;
	default:
		return E_FAIL;
	}

	AccessSize = newVal;
	return S_OK;
}

STDMETHODIMP CDeviceControl::get_Value(long *pVal)
{
	if(DataPointer == 0)
		return E_FAIL;

	*pVal = 0;
	MemoryCopy(pVal, DataPointer + Offset, AccessSize);

	Offset += AccessSize;
	if(Offset >= Limitation)
		Offset = Limitation -1;
	
	return S_OK;
}

STDMETHODIMP CDeviceControl::put_Value(long newVal)
{
	if(DataPointer == 0)
		return E_FAIL;

	MemoryCopy(DataPointer + Offset, &newVal, AccessSize);

	Offset += AccessSize;
	if(Offset >= Limitation)
		Offset = Limitation -1;

	return S_OK;
}

STDMETHODIMP CDeviceControl::OnRead(long address,long sz,byte __RPC_FAR data[])
{
	if(!KernelStarted)
		this->OnKernelStart();

	DataPointer = (unsigned char *)data;
	Limitation = sz;
	Offset = 0;

	Fire_OnRead(address, sz);
	
	DataPointer = 0;
	return S_OK;
}

STDMETHODIMP CDeviceControl::OnWrite(long address,long sz,byte __RPC_FAR data[])
{
	if(!KernelStarted)
		this->OnKernelStart();

	DataPointer = (unsigned char *)data;
	Limitation = sz;
	Offset = 0;
	Fire_OnWrite(address,sz);
	DataPointer = 0;
	return S_OK;
}

STDMETHODIMP CDeviceControl::OnKernelStart()
{
	KernelStarted = true;
	Fire_OnKernelStart();
	return S_OK;
}

STDMETHODIMP CDeviceControl::OnKernelExit()
{
	KernelStarted = false;
	Fire_OnKernelExit();
	return S_OK;
}
