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
 *  @(#) $Id: manager.cpp,v 1.7 2001/05/07 07:23:44 takayuki Exp $
 */

#include "manager.h"
#include "staticapi.h"
#include "serializer.h"
#include "parser.h"
#include "multistream.h"
#include "exception.h"

#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

using namespace std;

Manager::Manager(Parser * src)
{
	Source = src;
	ClearOption();
}

Manager::~Manager(void)
{
	API.clear();
	SerializeUnit.clear();
}

bool Manager::Body(char * filename)
{
	DeclareAPI();
	DeclareSerializer();

	{
		map<string,StaticAPI *>::iterator func;
		istream * in;
		fstream infile;
		string api;
		Array param;
		
		if(filename != 0l)
		{
			infile.open(filename,ios::in);
			if(!infile.is_open())
				throw Exception(string(MSG_COULDNOTOPEN " [") + filename + "]");
			in = static_cast<istream *>(&infile);
		}else
			in = static_cast<istream *>(&cin);
		
		Source->SetStream(in);
		while(Source->GetNextStatement(api,param))
		{
			if((func = API.find(api)) == API.end())
			{
				char buffer[1024];
				sprintf(buffer,"%s(%d) : [%s] " MSG_APINOTEXIST, filename,Source->GetPreviousLinePosition(),api.c_str());
				throw Exception(buffer);
			}

			try
			{
				(*func).second->Body(param);
			}
			catch(Exception e)
			{
				char buffer[1024];
				sprintf(buffer,"%s(%d) : [%s] " MSG_FATALONAPI " (%s)",filename,Source->GetPreviousLinePosition(),api.c_str(),e.GetReason().c_str());
				throw Exception(buffer);
			}

			param.Clear();
		}

		if(infile.is_open())
			infile.close();
	}

	{
		bool cont;
		list<Serializer *> Checkers;
		list<Serializer *>::iterator scope;
		unsigned int LastCheckersCount;

		scope = SerializeUnit.begin();
		while(scope != SerializeUnit.end())
		{
			Checkers.push_back(*scope);
			scope ++;
		}

		do
		{
			LastCheckersCount = Checkers.size();
			scope = Checkers.begin();
			while(scope != Checkers.end())
			{
				try
				{
					cont = false;
					(*scope)->Check(cont);
					if(!cont)
						scope = Checkers.erase(scope);
					else
						scope ++;
				}
				catch(Exception e)
				{
					char buffer[1024];
					sprintf(buffer,MSG_VALIDATIONFAIL " (%s)",e.GetReason().c_str());
					throw Exception(buffer);
				}
			}
		}while(Checkers.begin() != Checkers.end() && LastCheckersCount != Checkers.size());

		if(Checkers.begin() != Checkers.end())
			throw Exception(MSG_VALIDATIONDEADLOCK);
	}

	{
		list<Serializer *>::iterator scope;
		MultiStream outfile;

		scope = SerializeUnit.begin();
		while(scope != SerializeUnit.end())
		{
			try
			{
				(*scope)->Body(outfile);
			}
			catch(Exception e)
			{
				char buffer[1024];
				sprintf(buffer,MSG_FATALONOUTPUT " (%s)",e.GetReason().c_str());
				throw Exception(buffer);
			}
			scope ++;
		}

		outfile.CloseAllStream();
	}

	return true;
}

void Manager::DeclareAPI(StaticAPI * api)
{
	if(api == 0l)
		throw Exception(MSG_INTERNAL " : " MSG_NULLAPIASSIGNMENT);
	
	*const_cast<Manager **>(&api->parent) = this;
	API[string(api->GetAPIName())] = api;
}

void Manager::DeclareSerializer(Serializer * sel)
{
	if(sel == 0l)
		throw Exception(MSG_INTERNAL " : " MSG_NULLSERASSIGNMENT);

	*const_cast<Manager **>(&sel->parent) = this;
	SerializeUnit.push_back(sel);
}

bool Manager::SetOption(enum Manager::tagOption _opt)
{
	bool result;
	unsigned long opt;

	opt = static_cast<int>(_opt);
	result = (OptionFlag & (1 << opt)) != 0;

	OptionFlag |= (1 << opt);
	return result;
}

bool Manager::ResetOption(enum Manager::tagOption _opt)
{
	bool result;
	unsigned long opt;

	opt = static_cast<int>(_opt);
	result = (OptionFlag & (1 << opt)) != 0;

	OptionFlag &= ~(1 << opt);
	return result;
}
