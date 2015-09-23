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
 *  @(#) $Id: multistream.cpp,v 1.4 2000/11/14 16:57:33 takayuki Exp $
 */

#include "multistream.h"
#include "exception.h"

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using namespace std;

MultiStream::MultiStream(void)
{
	Stream.clear();
}

MultiStream::~MultiStream(void)
{
	CloseAllStream();
}

bool MultiStream::Add(const string & name)
{
	if(name.empty())
		return false;

	if(Stream.find(name) != Stream.end())
		return false;

	fstream * work = new fstream(name.c_str(),ios::out|ios::trunc);

	if(!work->is_open())
	{
		delete work;
		return false;
	}

	Stream[name] = work;

	return true;
}

fstream * MultiStream::operator [](const string & name)
{
	map<string, fstream *>::iterator scope;

	scope = Stream.find(name);
	if(scope == Stream.end())
	{
		if(!Add(name))
			throw Exception(string("File [") + name + "] could not create.");
		return Stream[name];
	}

	return (*scope).second;
}

bool MultiStream::Close(const string & src)
{
	map<string, fstream *>::iterator scope;

	scope = Stream.find(src);
	if(scope == Stream.end())
		return false;

	if((*scope).second != 0l)
	{
		(*scope).second->close();
		delete (*scope).second;
	}

	Stream.erase(scope);
	return true;
}

bool MultiStream::CloseAllStream(void)
{
	map<string,fstream *>::iterator scope;

	scope = Stream.begin();
	while(scope != Stream.end())
	{
		if((*scope).second != 0l)
		{
			(*scope).second->close();
			delete (*scope).second;
		}

		scope ++;
	}

	Stream.clear();
	return true;
}

