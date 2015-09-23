/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: component.h,v 1.2 2002/04/05 08:48:31 takayuki Exp $
 */

// $Header: /home/CVS/configurator/component.h,v 1.2 2002/04/05 08:48:31 takayuki Exp $

#ifndef __COMPONENT
#define __COMPONENT

#ifdef _MSC_VER
	#pragma warning(disable:4786)
#endif

#include <list>
#include <map>
#include <string>

#include "except.h"
#include "directorymap.h"

#define DEFAULT_PARAMETER " "

//------

class Component
{
public:
	enum tagBarrier   { EXTENSION, IGNORE_UNKNOWN_API };

protected:
	static std::list<Component *> componentList;
	static std::list<Component *> activatedComponentList;
	static int barrierFlag;

    static Component * findComponent(const char *);
    static bool findOption(Directory &, const char *, const char * = 0, std::string * = 0);
	static Directory * getOption(Directory &, const char *, const char * = 0);
	static Directory * mergeOption(Directory &, const char *, const char *);

	static void enable(enum tagBarrier);
	static void disable(enum tagBarrier);

	static void dumpContainerSnapshot(Directory &, bool = false);
    
    virtual void parseOption(Directory &) {};    
    virtual void body(Directory &) {};

public:
	Component(void);
    virtual ~Component(void);

    void activateComponent(bool = true);

	static bool isEnabled(enum tagBarrier);
	static void barrier(enum tagBarrier, const Message & reason);

    static void parseOption(int, char **, Directory &);
    static void executeComponents(Directory &);
};

#include <iostream>

inline Component::Component(void)
{	componentList.push_back(this);	}

inline bool Component::findOption(Directory & container, const char * id, const char * longId, std::string * value)
{
	Directory * node = getOption(container, id, longId);
	if(node != 0)
	{
		if(value != 0)
			*value = node->toString();
		return true;
	}
	return false;
}

inline void Component::enable(enum tagBarrier bf)
{	barrierFlag |= 1 << (int)bf;	}

inline void Component::disable(enum tagBarrier bf)
{	barrierFlag &= ~(1 << (int)bf);	}

inline bool Component::isEnabled(enum tagBarrier bf)
{	return (barrierFlag & (1 << (int)bf)) != 0;	}

inline void Component::barrier(enum tagBarrier bf, const Message & reason)
{
	if((barrierFlag & (1 << (int)bf)) == 0)
		Exception("The operation detailed below is not permitted.\n %s\n","次の操作は許可されていません\n %s\n").format(((std::string &)reason).c_str());

	if(!((std::string &)reason).empty())
		VerboseMessage("Configurator allows the operation below\n %s\n","次の操作を許可しました\n %s\n").format(((std::string &)reason).c_str());
}

#endif
