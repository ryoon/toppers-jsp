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
 *  @(#) $Id: staticapi.cpp,v 1.6 2001/11/12 14:59:27 takayuki Exp $
 */



#include "staticapi.h"
#include "exception.h"
#include "message.h"

using namespace std;

StaticAPIContainer::StaticAPIContainer(void)
{}

StaticAPIContainer::~StaticAPIContainer(void)
{}

bool StaticAPIContainer::Add(StaticAPI * api)
{
	string work;

	if(api == NULL || api->GetAPIName() == NULL)
		return false;

	work = api->GetAPIName();
	if(API.find(work) != API.end())
		return false;

	API[work] = api;
	return true;
}

void StaticAPIContainer::ParseParameterBlock(Manager * mgr, Parser * p, Array * parameter, Token delimiter)
{
	Token token;
	Token param;
	int nest;
	bool skip;

	skip = true;
	nest = 0;
	for(p->GetToken(token,true); token != delimiter; p->GetToken(token,true))
	{
		if(token == Token::PUNCTUATOR)
		{
			if(token.compare(",") == 0)
			{
				if(nest > 0)
				{
					param += token;
					continue;
				}

				if(!skip)
				{
					if(param == Token::INTEGER)
						parameter->push_back(new Integer(param.Value));
					else
						parameter->push_back(new String(param.Trim()));
				}
				param.erase();
				continue;
			}

			skip = false;

			if(token.compare("(") == 0)
			{
				param.Type = Token::SPECIAL;
				nest ++, param += token;
				continue;
			}

			if(token.compare(")") == 0)
			{
				nest --, param += token;
				continue;
			}

			if(token.compare("{") == 0)
			{
				Array * child = new Array;
				parameter->push_back(child);
				ParseParameterBlock(mgr,p,child,Token(Token::PUNCTUATOR,"}"));
				skip = true;
				continue;
			}
		}

		if(skip == true && token != Token::SPACE)
			skip = false;

		if(param.empty())
		{
			if(token != Token::SPACE)
				param = token;
		}else
		{
			param.Type = Token::SPECIAL;
			if(token == Token::SPACE)
				param += ' ';
			else
				param += token;
		}
	}

	if(!skip)
	{
		if(param == Token::INTEGER)
			parameter->push_back(new Integer(param.Value));
		else
			parameter->push_back(new String(param.Trim()));
	}
}

void StaticAPIContainer::Parse(Manager * mgr, Parser * p)
{
	map<string,StaticAPI *>::iterator scope;
	Token token;
	string api;
	Array parameter;

	p->GetToken(token,Token::IDENTIFIER);
	api = token;

	if((scope = API.find(api)) == API.end())
		throw Exception(string(Message("Unknown Static API Name [","不正なAPI名称 [")) + api + "]");

	p->GetToken(token,Token::PUNCTUATOR,"(");
	ParseParameterBlock(mgr, p, &parameter, Token(Token::PUNCTUATOR,")"));

	p->GetToken(token, Token::PUNCTUATOR,";");

	if((*scope).second->Enabled == true)
		(*scope).second->Body(mgr, &parameter);
}
	
