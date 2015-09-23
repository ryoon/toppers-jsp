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
 *  @(#) $Id: manager.h,v 1.7 2001/11/12 14:59:27 takayuki Exp $
 */


#ifndef __MANAGER_H
#define __MANAGER_H

#include "staticapi.h"
#include "domain.h"
#include <set>

class Manager
{
protected:
	void ParseBody(ParserComponent *,const char * = NULL);
	void SerializeBody(void);

public:
	std::set<int> Option;
	std::list<class Serializer *> SerializeUnit;
	StaticAPIContainer API;
	ProtectionDomain * CurrentDomain;

	Manager(void);
	virtual ~Manager(void);

	void SetOption(int);
	const bool operator [](int) const;

	virtual const char * GetKernelName(void) = 0;
	virtual bool ParseOption(int, char *[]) =0;
	virtual void Body(void) =0;

	Manager & operator << (Serializer *);
	Manager & operator << (StaticAPI *);
};

inline void Manager::SetOption(int num)
{	Option.insert(num);	}

inline const bool Manager::operator [](int pos) const
{	return Option.find(pos) != Option.end();	}

inline Manager & Manager::operator << (Serializer * ser)
{
	if(ser != NULL)
		SerializeUnit.push_back(ser);
	return *this;
}

inline Manager & Manager::operator << (StaticAPI * api)
{
	if(api != NULL)
		API.Add(api);
	return *this;
}

extern class Manager * ToplevelManagerComponent;

#endif