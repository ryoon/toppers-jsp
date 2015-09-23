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
 *  @(#) $Id: array.cpp,v 1.4 2000/11/24 09:14:47 takayuki Exp $
 */

#include "array.h"

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using namespace std;


Array::Array(void)
{
	Entity.clear();
}

Array::Array(const Array & src)
{
	vector<tagValue>::const_iterator scope;
	tagValue work;

	Entity.clear();

	scope = src.Entity.begin();
	while(scope != src.Entity.end())
	{
		work.Type = (*scope).Type;
		switch((*scope).Type)
		{
		case ARRAY:
			if((work.ArrayedValue = new Array(*(*scope).ArrayedValue)) == 0l)
				work.Type = EMPTY;
			break;
		case VALUE:
			if((work.SingleValue = new Valient(*(*scope).SingleValue)) == 0l)
				work.Type = EMPTY;
			break;
		}

		Entity.push_back(work);

		scope ++;
	}
}

Array::~Array(void)
{
	Clear();
}

bool Array::Clear(void)
{
	vector<tagValue>::iterator scope;
	
	scope = Entity.begin();
	while(scope != Entity.end())
	{
		switch((*scope).Type)
		{
		case ARRAY:
			delete (*scope).ArrayedValue;
			break;
		case VALUE:
			delete (*scope).SingleValue;
			break;
		}
		scope ++;
	}
	Entity.clear();
	return true;
}

bool Array::Grow(unsigned int required)
{
	if(required < Entity.size())
		return true;

	tagValue emptyvalue;

	emptyvalue.Type = EMPTY;
	for(required -= Entity.size(); required > 0; required --)
		Entity.push_back(emptyvalue);

	return true;
}

bool Array::Clear(tagValue * scope)
{
	if(scope == 0l)
		return false;

	switch(scope->Type)
	{
	case ARRAY:
		delete scope->ArrayedValue;
		break;
	case VALUE:
		delete (Valient *)scope->SingleValue;
		break;
	}
	scope->Type = EMPTY;
	return true;
}

bool Array::Set(unsigned int offset, const Valient & src)
{
	if( offset >= Entity.size() )
	{
		Grow(offset+1);
	}

	tagValue * scope = &Entity[offset];

	Clear(scope);
	scope->Type = VALUE;
	if((scope->SingleValue = new Valient(src)) == 0l)
	{
		scope->Type = EMPTY;
		return false;
	}

	return true;
}

bool Array::Set(unsigned int offset, Valient * src)
{
	if( offset >= Entity.size() )
	{
		Grow(offset+1);
	}

	tagValue * scope = &Entity[offset];

	Clear(scope);
	scope->Type = VALUE;
	scope->SingleValue = src;

	return true;
}

bool Array::Set(unsigned int offset, const Array & src)
{
	if( offset >= Entity.size() )
	{
		Grow(offset+1);
	}

	tagValue * scope = &Entity[offset];

	Clear(scope);
	scope->Type = ARRAY;
	if((scope->ArrayedValue = new Array(src)) == 0l)
	{
		scope->Type = EMPTY;
		return false;
	}

	return true;
}

bool Array::Set(unsigned int offset, Array * src)
{
	if( offset >= Entity.size() )
	{
		Grow(offset+1);
	}

	tagValue * scope = &Entity[offset];

	Clear(scope);
	scope->Type = ARRAY;
	scope->ArrayedValue = src;

	return true;
}

Valient * Array::GetValuePtr(unsigned int offset)
{
	if( offset > Entity.size() )
		return 0l;

	tagValue * scope = &Entity[offset];

	if( scope->Type != VALUE)
		return 0l;

	return scope->SingleValue;
}

Array * Array::GetArrayPtr(unsigned int offset)
{
	if( offset > Entity.size() )
		return 0l;

	tagValue * scope = &Entity[offset];

	if( scope->Type != ARRAY)
		return 0l;

	return scope->ArrayedValue;
}
