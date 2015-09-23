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
 *  @(#) $Id: devicemanagerCP.h,v 1.2 2002/04/10 11:27:26 takayuki Exp $
 */

#ifndef _ATLDEVICEMANAGERCP_H_
#define _ATLDEVICEMANAGERCP_H_

template <class T>
class CProxy_IKernelEvents : public IConnectionPointImpl<T, &IID__IKernelEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnInterruptRequest(ULONG inhno)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IKernelEvents* p_IKernelEvents = reinterpret_cast<_IKernelEvents*>(sp.p);
			if (p_IKernelEvents != NULL)
				ret = p_IKernelEvents->OnInterruptRequest(inhno);
		}	return ret;
	
	}
};




template <class T>
class CProxy_IDeviceEvents : public IConnectionPointImpl<T, &IID__IDeviceEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnRead(LONG address, LONG sz, unsigned char * data)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IDeviceEvents* p_IDeviceEvents = reinterpret_cast<_IDeviceEvents*>(sp.p);
			if (p_IDeviceEvents != NULL)
				ret = p_IDeviceEvents->OnRead(address, sz, data);
		}	return ret;
	
	}
	HRESULT Fire_OnWrite(LONG address, LONG sz, unsigned char * data)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IDeviceEvents* p_IDeviceEvents = reinterpret_cast<_IDeviceEvents*>(sp.p);
			if (p_IDeviceEvents != NULL)
				ret = p_IDeviceEvents->OnWrite(address, sz,data);
		}	return ret;
	
	}
	HRESULT Fire_OnKernelStart()
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IDeviceEvents* p_IDeviceEvents = reinterpret_cast<_IDeviceEvents*>(sp.p);
			if (p_IDeviceEvents != NULL)
				ret = p_IDeviceEvents->OnKernelStart();
		}	return ret;
	
	}
	HRESULT Fire_OnKernelExit()
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IDeviceEvents* p_IDeviceEvents = reinterpret_cast<_IDeviceEvents*>(sp.p);
			if (p_IDeviceEvents != NULL)
				ret = p_IDeviceEvents->OnKernelExit();
		}	return ret;
	
	}
};




template <class T>
class CProxy_IInformationEvents : public IConnectionPointImpl<T, &IID__IInformationEvents, CComDynamicUnkArray>
{
	//Warning this class may be recreated by the wizard.
public:
	HRESULT Fire_OnDeviceChanged(SHORT reason, LONG devid, LONG extra = 0)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IInformationEvents* p_IInformationEvents = reinterpret_cast<_IInformationEvents*>(sp.p);
			if (p_IInformationEvents != NULL)
				ret = p_IInformationEvents->OnDeviceChanged(reason, devid, extra);
		}	return ret;
	
	}
	HRESULT Fire_OnKernelChanged(SHORT reason)
	{
		HRESULT ret;
		T* pT = static_cast<T*>(this);
		int nConnectionIndex;
		int nConnections = m_vec.GetSize();
		
		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
			_IInformationEvents* p_IInformationEvents = reinterpret_cast<_IInformationEvents*>(sp.p);
			if (p_IInformationEvents != NULL)
				ret = p_IInformationEvents->OnKernelChanged(reason);
		}	return ret;
	
	}
};
#endif