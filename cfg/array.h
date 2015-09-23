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
 *  @(#) $Id: array.h,v 1.2 2000/11/14 15:56:26 takayuki Exp $
 */

//----------------------------------------------------------------
// class array - class definition
//

#ifndef __ARRAY_H
#define __ARRAY_H

#include <vector>
#include "valient.h"

class Array
{
public:
	enum tagType
	{
		EMPTY, ARRAY, VALUE
	};

protected:
	struct tagValue
	{
		enum tagType Type;
		union
		{
			Array * ArrayedValue;
			Valient * SingleValue;
		};
	};

	std::vector<tagValue> Entity;

	bool Grow(unsigned int);
	bool Clear(tagValue *);

public:
	Array(void);
	Array(const Array &);
	virtual ~Array(void);

		//参照渡しはコピーを作るが、ポインタ渡しはその値を使う
	bool Set(unsigned int,const Valient &);
	bool Set(unsigned int,Valient *);
	bool Set(unsigned int,const Array &);
	bool Set(unsigned int,Array *);

	Valient * GetValuePtr(unsigned int);
	Array * GetArrayPtr(unsigned int);
	tagType GetType(unsigned int);

	Valient & operator [](unsigned int);

	bool IsArray(unsigned int);
	unsigned int Size(void);

	bool Clear(void);
};

//------------------------------------------------------------
// Inline function declaration

inline Valient & Array::operator [](unsigned int offset)
{
	return *(Entity[offset].SingleValue);
}

inline bool Array::IsArray(unsigned int offset)
{
	if( offset > Entity.size() )
		return false;

	return (Entity[offset].Type == ARRAY);
}

inline unsigned int Array::Size(void)
{	return Entity.size();	}

#endif

//--------
// Designed and Programmed by S.D.F.