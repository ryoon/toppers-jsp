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
 *  @(#) $Id: task.cpp,v 1.3 2000/11/14 16:57:33 takayuki Exp $
 */

#pragma warning(disable:4786)

#include "Serializer.h"
#include "staticapi.h"
#include "id.h"

#include <map>
#include <list>
#include <string>
#include <algorithm>

using namespace std;

#define DECLARE_API(x) \
class API_##x : public StaticAPI { public: char * GetAPIName(void) { return #x; }; bool Body(Array &); } x; friend class API_##x; 

class Includer : public Serializer
{
protected:
	list<string> filename;

public:
	DECLARE_API(INCLUDE)

	bool Body(ostream & dest)
	{
		list<string>::iterator scope = filename.begin();

		while(scope != filename.end())
		{
			dest << "#include \"" << *scope << "\"" << endl;
			scope ++;
		}
		return true;
	}
} includer;

bool Includer::API_INCLUDE::Body(Array & Parameter)
{
	includer.filename.push_front(Parameter[0].GetString());
	return true;
}


class Definer : public Serializer
{
protected:
	struct tagValue
	{
		string Identifier;
		Valient Instance;
	};

	list<tagValue> define;

public:
	bool IsDefined(string & id)
	{
		list<tagValue>::iterator scope;

		scope = define.begin();
		while(scope != define.end())
		{
			if( (*scope).Identifier == id)
				return true;
			scope ++;
		}
		
		return false;
	}
	
	void Add(string & id, Valient & val)
	{
		tagValue work;
		work.Identifier = id;
		work.Instance = val;
		define.push_back(work);
	};

	bool Body(ostream & dest)
	{
		list<tagValue>::iterator scope;

		scope = define.begin();
		while(scope != define.end())
		{
			dest << "#define " << (*scope).Identifier << " " << (*scope).Instance << endl;
			scope ++;
		}
		return true;
	};
} definer;

class Task : public Serializer
{
protected:
	ID TaskID;

	enum tagPart
	{
		TYPE=0, EXINF, TASK, PRIORITY, STACKSIZE, STACK
	};

	map<int, Array> InitialParameter;

public:
	DECLARE_API(CRE_TSK)

	bool Body(ostream & src)
	{
		return true;
 	}
} task;


bool Task::API_CRE_TSK::Body(Array & parameter)
{
	int taskid;
	Valient work;

	Array * info;
	Array * subarray;

	work = parameter[0];
	if(work == Valient::STRING)
	{
		if(definer.IsDefined(string(work.GetString())))
		{
			//Already Defined
			return false;
		}
		taskid = task.TaskID.GetNewID();
		definer.Add(string(work.GetString()),Valient(taskid));
	}else
	{
		taskid = work.GetInteger();
		if(task.TaskID.IsAssigned(taskid))
			return false;
	}

	task.TaskID.Assign(taskid);
	info = &task.InitialParameter[taskid];
	subarray = parameter.GetArrayPtr(1);
	info->Set(TYPE, (*subarray)[0]);
	info->Set(EXINF, (*subarray)[1]);
	info->Set(TASK, (*subarray)[2]);
	info->Set(PRIORITY, (*subarray)[3]);
	info->Set(STACKSIZE, (*subarray)[4]);
	info->Set(STACK, (*subarray)[5]);

	return true;
}


#include "manager.h"

void Manager::DeclareAPI(void)
{
	DeclareAPI(&task.CRE_TSK);
	DeclareAPI(&includer.INCLUDE);
}

void Manager::DeclareSerializer(void)
{
	DeclareSerializer(&includer);
	DeclareSerializer(&definer);
	DeclareSerializer(&task);
}
