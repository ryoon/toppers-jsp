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
 *  @(#) $Id: domain.cpp,v 1.3 2001/11/12 14:59:27 takayuki Exp $
 */



/*
 * ITRON-MMU プロテクションドメインサポートモジュール
 *  注)本モジュールは検討中の仕様(2001/07)を元にしており，
 *     過渡的なものであるため，次のバージョンではなくなる可能性がある
 *
 * ドメインのフォーマットは次の通り
 *   domain <name> [ : shared|system|user ] { 静的API };
 * ドメインのネストも許すが，最も外は共有になる．
 *
 * 追記) ドメインのネストは正式に仕様から外された
 */

#include "domain.h"
#include <algorithm>
#include "exception.h"
#include "manager.h"

using namespace std;

map<string, ProtectionDomain *> ProtectionDomain::Domain;

ProtectionDomain::ProtectionDomain(void)
{
	ParentDomain = NULL;
	Type = UNKNOWN;
	Name = "UNKNOWN";
};

ProtectionDomain::~ProtectionDomain(void)
{
	list<ProtectionDomain *> * container;
	list<ProtectionDomain *>::iterator scope;
	map<string, ProtectionDomain *>::iterator mapping;

		//親と手を切る
	if(ParentDomain != NULL)
	{
		container = &ParentDomain->ChildDomain;
		scope = find(container->begin(), container->end(), this);
		if(scope != container->end())
			container->erase(scope);
		ParentDomain = NULL;
	}

		//子を全滅させる
	scope = ChildDomain.begin();
	while(scope != ChildDomain.end())
	{
		(*scope)->ParentDomain = NULL;
		delete (*scope);
		scope ++;
	}
	ChildDomain.clear();

		//登録を抹消する
	mapping = Domain.find(Name);
	if(mapping != Domain.end())
		Domain.erase(mapping);
}

ProtectionDomain * ProtectionDomain::GetChildDomain(ProtectionDomain * prev)
{
	list<ProtectionDomain *>::iterator scope;

	if(prev != NULL)
	{
		scope = find(ChildDomain.begin(), ChildDomain.end(), prev);
	}else
		scope = ChildDomain.begin();

	if(scope != ChildDomain.end())
		scope ++;

	return scope == ChildDomain.end() ? NULL : (*scope);
}

void ProtectionDomain::Parse(Manager * mgr, Parser * p)
{
	Token token;

	mgr->CurrentDomain = this;
	p->GetToken(token,Token::IDENTIFIER,"domain");
	p->GetToken(token,Token::IDENTIFIER);
	Name = token;

	Type = USER;
	p->GetToken(token,Token::PUNCTUATOR);
	if(token.compare(":") == 0)
	{
		p->GetToken(token,Token::IDENTIFIER);
		if(token.compare("system") == 0)
			Type = SYSTEM;
		else if(token.compare("shared") == 0)
			Type = SHARED;
		else if(token.compare("user") == 0)
			Type = USER;
		else
			throw Exception();

		p->GetToken(token,Token::PUNCTUATOR);
	}

	if(token.compare("{") != 0)
		throw Exception();

	p->GetToken(token);
	while(token.compare("}") != 0)
	{
		if(token.Type != Token::IDENTIFIER)
			throw Exception();

		p->PutBack(token);
		if(token.compare("domain")==0)
		{
			ProtectionDomain * newdomain = CreateNewDomain(mgr);
			newdomain->ParentDomain = this;
			ChildDomain.push_back(newdomain);

			newdomain->Parse(mgr,p);
			mgr->CurrentDomain = this;
		}else
			mgr->API.Parse(mgr,p);
		p->GetToken(token);
	}
	p->GetToken(token,Token::PUNCTUATOR,";");
}
