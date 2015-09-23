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
 *  @(#) $Id: exception.h,v 1.7 2001/11/12 14:59:27 takayuki Exp $
 */


#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <string>
#include <iostream>

class Exception
{
protected:
	std::string Reason;
	static bool ThrowControl;

public:
	Exception(void) 
		{ Reason = ""; };
	Exception(const char * src)
		{ Reason = src; };
	Exception(const std::string & src)
		{ Reason = src; };
	Exception(const Exception & src)
		{ Reason = src.Reason; };
	
	std::string & GetReason(void) 
		{ return Reason; };
	operator const char * (void)
		{ return this->GetReason().c_str(); };
	operator const std::string & (void)
		{ return this->GetReason(); }

	static bool GetThrowControl(void);
	static bool SetThrowControl(bool);
	static void Throw(const char *);
	static void Throw(const std::string &);
	static void Throw(const Exception &);
};

inline std::ostream & operator << (std::ostream & dest, Exception & e)
{	return (dest << static_cast<const char *>(e));	}

inline bool Exception::GetThrowControl(void)
{	return ThrowControl;	}

inline bool Exception::SetThrowControl(bool mode)
{
	bool old = ThrowControl;
	ThrowControl = mode;
	return old;
}

inline void Exception::Throw(const char * src)
{
	if(ThrowControl)
		throw Exception(src);
}

inline void Exception::Throw(const Exception & src)
{
	if(ThrowControl)
		throw src;
}

inline void Exception::Throw(const std::string & src)
{
	if(ThrowControl)
		throw Exception(src);
}

#endif
