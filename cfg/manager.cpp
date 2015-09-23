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
 *  @(#) $Id: manager.cpp,v 1.10 2001/11/12 14:59:27 takayuki Exp $
 */


#include "manager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "multistream.h"
#include "serializer.h"

using namespace std;

Manager * ToplevelManagerComponent = NULL;

Manager::Manager(void)
{
	Option.clear();
	CurrentDomain = NULL;

	if(::ToplevelManagerComponent == NULL)
		::ToplevelManagerComponent = this;
	else
		throw Exception();
}

Manager::~Manager(void)
{}

void Manager::ParseBody(ParserComponent * top, const char * filename)
{
	Parser parser;
	fstream file;
	istream * in;

	if(filename != NULL && *filename != '\x0')
	{
		file.open(filename,ios::in);
		if(!file.is_open())
			throw Exception("ファイルのオープンに失敗した");
		in = static_cast<istream *>(&file);
	}else
		in = &cin;

	parser.SetStream(in);
	top->Parse(this, &parser);

	if(filename != NULL)
		file.close();
}

void Manager::SerializeBody(void)
{
	unsigned int i;
	list<Serializer *> serializer;
	list<Serializer *>::iterator scope;
	MultiStream output;

		//Validation
	scope = SerializeUnit.begin();
	while(scope != SerializeUnit.end())
	{
		serializer.push_back(*scope);
		scope ++;
	}

	do{
		i = serializer.size();
		scope = serializer.begin();
		while(scope != serializer.end())
		{
			if((*scope)->Validation(this))
				scope = serializer.erase(scope);
			else
				scope ++;
		}
	}while(!serializer.empty() && (serializer.size() < i));

		//Serialization
	scope = SerializeUnit.begin();
	while(scope != SerializeUnit.end())
	{
		(*scope)->Serialization(this,output);
		scope ++;
	}
}
