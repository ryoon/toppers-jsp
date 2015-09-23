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
 *  @(#) $Id: staticapi.h,v 1.3 2000/11/24 09:14:47 takayuki Exp $
 */

//------------------------------------------------------------
// StaticAPI - class definition
//

#ifndef __STATICAPI_H
#define __STATICAPI_H

#include "array.h"
#include "exception.h"

class StaticAPI
{
private:
	char * CheckParameter_Primitive(Array *,char *);
protected:
	void CheckParameter(Array *, char *);
	void CheckParameterCount(Array &,unsigned int, unsigned int=0);
	void CheckParameterType(Array &,unsigned int,Valient::tagType);
	void CheckParameterType(Array &,unsigned int,Array::tagType);

public:
	StaticAPI(void) {};
	virtual ~StaticAPI(void) {};

	virtual char * GetAPIName(void) =0;
	virtual void Body(Array & Parameter) =0;
};

inline void StaticAPI::CheckParameterCount(Array & param,unsigned int start, unsigned int end)
{
	if((param.Size() < start) || (end != 0 && param.Size() > end))
		throw Exception("Too few or Too much parameters");
}

inline void StaticAPI::CheckParameterType(Array & param,unsigned int offset, Valient::tagType type)
{
	if(param.Size() > offset || param.GetValuePtr(offset) == 0l || !(param[offset] == type))
		throw Exception("Parameter type is not match");
}

inline void StaticAPI::CheckParameterType(Array & param,unsigned int offset, Array::tagType type)
{
	if((type != Array::EMPTY) && param.IsArray(offset) != (type == Array::ARRAY) ? true : false)
		throw Exception("Parameter type is not match");
}

inline void StaticAPI::CheckParameter(Array * param, char * format)
{
	if(CheckParameter_Primitive(param,format)!=0l)
		throw Exception("Internal error : Illegal parameter descriptor (Unterminated descriptor)");
}

#endif

//---
// Degined and Programmed by S.D.F. in Oct,2000
