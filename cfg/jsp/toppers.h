/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: toppers.h,v 1.5 2001/11/13 11:58:17 takayuki Exp $
 */

/*
 * このファイルは過渡的なものであり，次のバージョンでは削除される可能性がある
 *  (特にプロテクションドメインに関する部分)
 */

#ifndef __TOPPERS_H
#define __TOPPERS_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <string>
#include <vector>
#include <map>
#include <set>

#include "../manager.h"
#include "../domain.h"
#include "../staticapi.h"
#include "../serializer.h"
#include "../garbage.h"
#include "../id.h"
#include "../value.h"

	//吐き出すファイルの名前
#define ORTIFILE "toppers_info.odl"
#define CFGFILE "kernel_cfg.c"
#define DOMAINCFGPREFIX "_cfg.c"
#define SHAREDOMFILE "shared_cfg.c"
#define HEADER "kernel_id.h"
#define LDSCRIPT "ld.script"
#define CHECKER "kernel_chk.c"

	//内部マクロ
#define DECLARE_API(x) class API_##x : public StaticAPI { public: class BaseObject * Parent; const char * GetAPIName(void) { return #x; }; void Body(Manager *, Array *); } x; friend class x;
#define DECLARE_PAPI(x) class API_##x : public StaticAPI { const char * GetAPIName(void) { return #x; }; void Body(Manager *, Array *); } x; friend class x;
#define INIT_API(x) x##.Parent = this;
#define DECLARE_OBJECT(x,y) class x : public BaseObject { public: DECLARE_API(y); x(void) { INIT_API(y); }; };

enum tagOption
{
	ORTISUPPORT,
	DOMAINSUPPORT,
	VERBOSE,
	FORCEDOMAINSUPPORT,
	CHECKERFILE
};

class BaseObject
{
public:
	std::map<std::string, int> ObjectID;
	ID Identifier;
	Array Parameter;

	void InitializeParameter(int, int);
	bool LoadParameter(int, Array *, const char *,const char ** = NULL);
	void SetParameter(int, int, Value *);
	Value * GetParameter(int, int);

	int AssignNewObjectID(Manager *, int);
	int AssignObjectID(Manager *,Value *,int);
	int GetObjectID(Value *);

	void Serialization(Manager *, MultiStream &, const char *, const char *, const char * = NULL);
	void CreateBuffer(Manager *, MultiStream &, const char *, int, int, const char *, const char *);
};


	//タスクオブジェクト
class Task : public BaseObject
{
public:
	enum tagPart
	{	TYPE=0, EXINF, TASK, PRIORITY, STACKSIZE, STACK, TEXTYPE, TEXFUNC};

	DECLARE_API(CRE_TSK);
	DECLARE_API(DEF_TEX);
	Task(void) 
	{ 
		INIT_API(CRE_TSK); 
		INIT_API(DEF_TEX);
	};
};

	//ひとつしかAPIがない有象無象ども
DECLARE_OBJECT(Semaphore, CRE_SEM);
DECLARE_OBJECT(EventFlag, CRE_FLG);
DECLARE_OBJECT(DataQueue, CRE_DTQ);
DECLARE_OBJECT(Mailbox,   CRE_MBX);
DECLARE_OBJECT(CyclicHandler, CRE_CYC);

class FixedsizeMemoryPool : public BaseObject
{
public:
	FixedsizeMemoryPool(void) { INIT_API(CRE_MPF); };

	DECLARE_API(CRE_MPF);
	void Serialization(Manager *, MultiStream &, const char *, const char *, const char * = NULL);
};

	//割込みハンドラ
class InterruptHandler : public BaseObject
{
public:
	InterruptHandler(void) { INIT_API(DEF_INH); };

	DECLARE_API(DEF_INH);

	void Serialization(Manager *, MultiStream &);
};

	//例外ハンドラ
class ExceptionHandler : public BaseObject
{
public:
	ExceptionHandler(void) { INIT_API(DEF_EXC); };

	DECLARE_API(DEF_EXC);

	void Serialization(Manager *, MultiStream &);
};

	//初期化ハンドラオブジェクト
class InitializeHandler : public BaseObject
{
protected:
	std::list<std::string> HandlerList;

public:
	InitializeHandler(void) { INIT_API(ATT_INI); };
	
	DECLARE_API(ATT_INI);

	void Serialization(Manager *, MultiStream &);
};
	

	//今のJSPでは保護ドメインをサポートしていないので、
	//システム内で単一 (オプションで外せるけど効果なし)
class ToppersDomain : public ProtectionDomain, public Serializer, public Garbage
{
protected:
	ProtectionDomain * CreateNewDomain(Manager *);
public:
	typedef std::set<int> set_int;
	std::map<BaseObject *, set_int> Object;

	void Parse(class Manager *, Parser *);
	bool Validation(Manager *);
	void Serialization(Manager *, MultiStream &);
};

/*
 * TOPPERS/JSPカーネルコンフィギュレータ 本体
 */
class ToppersManager : public Manager, public Serializer
{
protected:
	std::string filename;

public:
	std::list<std::string> IncludeFile;
	std::list<std::string> Initializer;
	
	Task task;
	Semaphore semaphore;
	EventFlag eventflag;
	DataQueue dataqueue;
	Mailbox mailbox;
	FixedsizeMemoryPool fmempool;
	CyclicHandler cyclic;
	InterruptHandler inthdr;
	ExceptionHandler exchdr;
	InitializeHandler inihdr;

	ToppersManager(void);
	~ToppersManager(void) {};

	const char * GetKernelName(void) { return "TOPPERS/JSP kernel ver.1.1 (patchlevel=1)"; };
	bool ParseOption(int, char *[]);
	void Body(void);
	void Serialization(Manager *, MultiStream &);

	DECLARE_PAPI(INCLUDE);
};

#endif
