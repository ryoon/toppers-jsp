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
 *  @(#) $Id: stdafx.h,v 1.1 2001/02/23 09:34:31 takayuki Exp $
 */

#if !defined(AFX_STDAFX_H__4015F3E9_0639_43DE_A5EE_C32ECAFDFC74__INCLUDED_)
#define AFX_STDAFX_H__4015F3E9_0639_43DE_A5EE_C32ECAFDFC74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#pragma warning(push)
#pragma warning(disable:4786)

#include <list>
#include <map>
#include "range.h"
#include "messages.h"

typedef unsigned long DeviceID;

#include <atlbase.h>
// CComModule クラスから派生したクラスを使用して、オーバーライドする場合
// _Module の名前は変更しないでください。
class CExeModule : public CComModule
{
public:
	bool RaiseInterrupt(unsigned long inhno);
	bool Write(class CKernel * kernel, unsigned long address, unsigned long sz, unsigned char data[]);
	bool Read(class CKernel * kernel, unsigned long address, unsigned long sz, unsigned char data[]);
	bool DetachKernel(class CKernel * kernel);
	bool AttachKernel(class CKernel * kernel);
	void RemoveViewer(class CInformation * view);
	void AddViewer(class CInformation * view);
	bool Unmap(class CDevice * device, unsigned long address);
	bool Map(class CDevice * device, Range map);
	bool DeleteDevice(class CDevice * device);
	DeviceID AddDevice(class CDevice * device);

		//勝手に作られた部分
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;


		//自分で作った部分
	CRITICAL_SECTION cs;
	class CKernel * Kernel;
	std::map<Range, class CDevice *> DeviceMap;
	std::list<class CDevice *> DeviceList;
	std::list<class CInformation *> ViewerList;
	

	CExeModule(void);
	
};
extern CExeModule _Module;
#include <atlcom.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#pragma warning(pop)

#endif // !defined(AFX_STDAFX_H__4015F3E9_0639_43DE_A5EE_C32ECAFDFC74__INCLUDED)
