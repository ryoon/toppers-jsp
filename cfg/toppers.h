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
 *  @(#) $Id: toppers.h,v 1.2 2000/11/14 15:56:26 takayuki Exp $
 */

#ifndef __TOPPERS_H
#define __TOPPERS_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "serializer.h"
#include "staticapi.h"
#include "id.h"
#include "multistream.h"

#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <set>

using namespace std;

#define DECLARE_API(x) class API_##x : public StaticAPI { public: char * GetAPIName(void) { return #x; }; void Body(Array &); } x; friend class API_##x; 

#define HEADERFILE "kernel_id.h"
#define CFGFILE "kernel_cfg.c"

//-----------------------------------------------------------------
// Miscellanea : インクルードや定数定義など
//

class Miscellanea : public Serializer
{
protected:
	set<string> includefilename;
	map<string, Valient> define;
	set<string> initializer;
	bool secondpass;

public:
	DECLARE_API(INCLUDE);
	DECLARE_API(DEFINE);

		//include
	bool AddIncludeFile(const string & file);

		//define
	bool IsDefined(const string & id);
	bool AddDefinition(const string & id, const Valient & val);
	bool GetDefinision(const string & id, Valient & val);

	bool AddInitializer(const string &);

	void Check(void) { secondpass = false; };
	void Body(MultiStream & dest);
};


class Object : public Serializer
{
protected:
	ID Identifier;
	map<int, Array> InitialParameter;

	void OutputHeaderBlock(ostream *,char *, char *, char * =0l);
	void OutputFooterBlock(ostream *,char *, char * =0l);

	void CreateBufferDefinition(ostream *, int,char *,char *,int =-1);
public:
	Array * LoadParameters(Array *, int, int, int,int =0);
	int AssignObjectID(Valient &);
	int GetAssignedObjectID(Valient &);
	Array * GetInitialParameterArray(int);

	virtual void Check(bool &);
};

inline Array * Object::GetInitialParameterArray(int offset)
{
	if(!Identifier.IsAssigned(offset))
		return 0l;
	return &InitialParameter[offset];
}


//-----------------------------------------------------------------
// Task : タスク関連情報生成
//

class Task : public Object
{
protected:
	enum tagPart
	{	TYPE=0, EXINF, TASK, PRIORITY, STACKSIZE, STACK,TEXTYPE, TEXFUNC	};


public:
	DECLARE_API(CRE_TSK)
	DECLARE_API(DEF_TEX)

	void Body(MultiStream & src);
};

//-----------------------------------------------------------------
// Semaphore : セマフォ関連情報生成
//


class Semaphore : public Object
{
protected:
	enum tagPart
	{	ATR=0, ISEMCNT, MAXCNT	};

public:
	DECLARE_API(CRE_SEM)

	void Body(MultiStream & src);
};

class EventFlag : public Object
{
protected:
	enum tagPart { ATR=0, FLGPTN };
public:
	DECLARE_API(CRE_FLG)

	void Body(MultiStream &);
};

class DataQueue : public Object
{
protected:
	enum tagPart { ATR=0, CNT, DTQ };
public:
	DECLARE_API(CRE_DTQ)

	void Body(MultiStream &);
};

class Mailbox : public Object
{
protected:
	enum tagPart { ATR = 0, MPRI, HD };
public:
	DECLARE_API(CRE_MBX)

	void Body(MultiStream &);
};

class FixedsizeMemoryPool : public Object
{
protected:
	enum tagPart { ATR=0, CNT, SZ, MPF };
public:
	DECLARE_API(CRE_MPF)

	void Body(MultiStream &);
};

class CyclicHandler : public Object
{
protected:
	enum tagPart { ATR=0, EXINF, HDR, TIM, PHS };
public:
	DECLARE_API(CRE_CYC)

	void Body(MultiStream &);
};

class InterruptHandler : public Object
{
protected:
	enum tagPart { ATR=0, HDR, NO };
public:
	DECLARE_API(DEF_INH)

	void Check(bool & cont) {};
	void Body(MultiStream &);
};

class ExceptionHandler : public Object
{
protected:
	enum tagPart { ATR=0, HDR, NO };
public:
	DECLARE_API(DEF_EXC)

	virtual void Check(bool & cont) {};
	void Body(MultiStream &);
};

class InitializeHandler : public Object
{
protected:
	enum tagPart { ATR=0, EXINF, RTN};
public:
	DECLARE_API(ATT_INI)

	virtual void Check(bool & cont) {};
	void Body(MultiStream &);
};

#endif