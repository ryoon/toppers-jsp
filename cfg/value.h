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
 *  @(#) $Id: value.h,v 1.2 2001/11/12 14:59:27 takayuki Exp $
 */


#ifndef __VALUE_H
#define __VALUE_H

#include <vector>
#include <string>
#include <iostream>

class Value
{
public:
	enum tagType
	{
		UNKNOWN = 0,
		INTEGER,
		STRING,
		FLOAT,
		ARRAY
	};

	Value(void) {}
	virtual ~Value(void){};

	virtual operator enum tagType (void) const = 0;

	virtual Value * Duplicate(void) = 0;
	virtual void Serialize(std::ostream &) const = 0;
};

class Integer : public Value
{
protected:
	int value;

public:
	Integer(long src=0) { value = src; };
	Integer(const Integer & src) { value = src.value; };
	virtual ~Integer(void) {};

	virtual operator enum tagType (void) const { return INTEGER; };
	
	operator const int (void) const;
	operator int & (void);

	virtual Value * Duplicate(void);
	virtual void Serialize(std::ostream &) const;
};

class String : public Value, public std::string
{
public:
	String(void) {};
	String(const char *);
	String(std::string &);
	String(const String & src);
	virtual ~String(void) {};
	virtual operator enum tagType (void) const { return STRING; };
	virtual Value * Duplicate(void);
	virtual void Serialize(std::ostream &) const;
};

class Array : public Value, public std::vector<Value *>
{
public:
	Array(void) {};
	Array(const Array &);
	virtual ~Array(void);

	virtual operator enum tagType (void) const { return ARRAY; };
	Array & operator =(const Array &);

	void clear();
	
	virtual Value * Duplicate(void);
	virtual void Serialize(std::ostream &) const;
};


//--- Inline functions

inline Integer::operator const int (void) const
{	return value;	}

inline Integer::operator int & (void)
{	return value;	}

inline Value * Integer::Duplicate(void)
{	return new Integer(value);	}

inline void Integer::Serialize(std::ostream & out) const
{	out << value;	}

inline String::String(const char * str)
{	assign(str);	}

inline String::String(std::string & str)
{	assign(str.c_str());	}

inline String::String(const String & src)
{	assign(src);	}

inline Value * String::Duplicate(void)
{	return static_cast<Value *>(new String(*this));	}

inline void String::Serialize(std::ostream & out) const
{	out << static_cast<std::string>(*this);	}


inline Value * Array::Duplicate(void)
{	return new Array(*this);	}

//--- Related functions

inline std::ostream & operator <<(std::ostream & out, Value & val)
{
	val.Serialize(out);
	return out;
}

inline std::ostream & operator <<(std::ostream & out, Value * val)
{
	if(val != NULL)
		val->Serialize(out);
	else
		out << "<null>";
	return out;
}

#endif