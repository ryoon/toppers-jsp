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
 *  @(#) $Id: valient.h,v 1.3 2000/11/14 16:57:33 takayuki Exp $
 */

//----------------------------------------------------------------
// class Valient - class definition
//

#ifndef __VALIENT_H
#define __VALIENT_H

class Valient
{
public:
	enum tagType
	{
		EMPTY, INTEGER, UNSIGNEDINTEGER, DOUBLE, STRING, POINTER
	};

protected:
	enum tagType Type;
	union
	{
		int Integer;
		unsigned int UnsignedInteger;
		double Double;
		struct
		{
			unsigned int Size;
			char * Entity;
		} String;
		void * Pointer;
	};

public:
	Valient(void);
	Valient(int);
	explicit Valient(unsigned int);
	Valient(double);
	explicit Valient(char *);
	Valient(void *);
	Valient(const Valient &);

	virtual ~Valient(void);

	bool Set(int);
	bool Set(unsigned int);
	bool Set(double);
	bool Set(char *);
	bool Set(void *);

	int GetInteger(int =0);
	char * GetString(char * =0l);
	double GetDouble(double =0.0);
	void * GetPointer(void * =0l);

	bool Clear(void);

	enum tagType GetType(void);

	bool operator ==(enum tagType);
	char * operator =(char *);
	Valient & operator =(const Valient &);

	operator const enum tagType(void)
	{	return Type;	};
};

inline Valient::Valient(void)
{	Type=EMPTY;	}

inline Valient::Valient(int src)
{	Type=EMPTY;	this->Set(src);	}

inline Valient::Valient(unsigned int src)
{	Type=EMPTY;	this->Set(src);	}

inline Valient::Valient(double src)
{	Type=EMPTY;	this->Set(src);	}

inline Valient::Valient(char * src)
{	Type=EMPTY;	this->Set(src);	}

inline Valient::Valient(void * src)
{	Type=EMPTY;	this->Set(src);	}

inline bool Valient::operator ==(enum tagType src)
{	return Type == src;	}

inline char * Valient::operator =(char * src)
{
	Set(src);
	return src;
}

inline int Valient::GetInteger(int def)
{
	if(Type != INTEGER)
		return def;
	return Integer;
}

inline char * Valient::GetString(char * def)
{
	if(Type != STRING)
		return def;
	return String.Entity;
}

inline double Valient::GetDouble(double def)
{
	if(Type != DOUBLE)
		return def;
	return Double;
}

inline void * Valient::GetPointer(void * def)
{
	if(Type != POINTER)
		return def;
	return Pointer;
}

inline enum Valient::tagType Valient::GetType(void)
{	return Type;	}

#endif

//--------
// Designed and Programmed by S.D.F.