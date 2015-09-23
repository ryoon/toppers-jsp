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
 *  @(#) $Id: valient.cpp,v 1.3 2000/11/14 16:57:33 takayuki Exp $
 */

#include "valient.h"

Valient::Valient(const Valient & src)
{
	switch(src.Type)
	{
	case STRING:
		Type = EMPTY;
		this->Set(src.String.Entity);
		break;

	default:
		Type = src.Type;
		Double = src.Double;
	}
}

Valient::~Valient(void)
{	Clear();	}

bool Valient::Set(int src)
{
	Clear();
	Type = INTEGER;
	Integer = src;
	return true;
}

bool Valient::Set(unsigned int src)
{
	Clear();
	Type = UNSIGNEDINTEGER;
	UnsignedInteger = src;
	return true;
}

bool Valient::Set(double src)
{
	Clear();
	Type = DOUBLE;
	Double = src;
	return true;
}

bool Valient::Set(char * src)
{
	unsigned int i,j;

	Clear();
	Type = STRING;
	
	for(i=0;*(src+i)!='\x0';i++);

	String.Size = i;
	String.Entity = new char [i+1];
	for(j=0;j<=i;j++)
		*(String.Entity+j)=*(src+j);

	return true;
}

bool Valient::Set(void * src)
{
	Clear();
	Type = POINTER;
	Pointer = src;
	return true;
}

bool Valient::Clear(void)
{
	switch(Type)
	{
	case STRING:
		if(String.Entity != 0l)
			delete [] String.Entity;
		String.Entity = 0l;
		break;
	}
	Type = EMPTY;
	return true;
}

Valient & Valient::operator =(const Valient & src)
{
	Clear();
	
	Type = src.Type;
	if(src.Type == STRING)
	{
		unsigned int i;
		String.Entity = new char[src.String.Size+1];
		for(i=0;i<=src.String.Size;i++)
			*(String.Entity+i) = *(src.String.Entity+i);
		String.Size = src.String.Size;
	}else
		Double = src.Double;

	return *this;
}

