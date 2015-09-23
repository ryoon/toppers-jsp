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
 *  @(#) $Id: mpstrstream.cpp,v 1.2 2002/04/05 08:48:31 takayuki Exp $
 */


// $Header: /home/CVS/configurator/mpstrstream.cpp,v 1.2 2002/04/05 08:48:31 takayuki Exp $

#include "mpstrstream.h"
#include <stdarg.h>

using namespace std;

typedef std::list<struct mpstrstream::tagStreamPartInfo *> List;

list<struct mpstrstream::tagStreamPartInfo *>::iterator mpstrstream::_find(const char * partname) const
{
	List::const_iterator scope;

	scope = container.begin();
	while(scope != container.end())
	{
		if((*scope)->name.compare(partname) == 0)
			break;
		scope++;
	}
	return *reinterpret_cast<List::iterator *>(&scope);
}

mpstrstream::~mpstrstream(void)
{
	if(!filename.empty())
		serialize();

	while(!container.empty())
	{
		delete *(container.begin());
		container.pop_front();
	}
}

strstream & mpstrstream::moveto(const char * partname)
{
	List::iterator scope;

	scope = container.begin();
	while(scope != container.end())
	{
		if((*scope)->name.compare(partname) == 0)
			break;
		scope ++;
	}

	current = scope;
	return (*current)->stream;
}

strstream & mpstrstream::moveto(unsigned int pos)
{
	List::iterator scope;

	scope = container.begin();
	while(pos -- > 0)
		scope ++;

	current = scope;
	return (*current)->stream;
}

strstream & mpstrstream::moveto(enum tagPosition pos)
{
	switch(pos)
	{
	case BEGIN:
		current = container.begin();
		break;
	case END:
		current = container.end();
		break;
	}
	return (*current)->stream;
}

strstream & mpstrstream::createpart(const char * partname, const char * second, ... )
{
	struct tagStreamPartInfo * info;
	va_list vl;

	va_start(vl, second);
	vl = (char *)vl - sizeof(const char *);

	while(partname != 0)
	{
		info = new struct tagStreamPartInfo;
		info->name.assign(partname);
		info->flags = DEFAULT;
		container.insert(current, info);

		partname = va_arg(vl, const char *);
	}

	return (*current)->stream;
}

strstream & mpstrstream::insertpart(const char * partname, const char * successor)
{
	struct tagStreamPartInfo * info;
	List::iterator scope;

	scope = container.end();
	if(successor != 0)
		if((scope = _find(successor)) == container.end())
			Exception("Stream does not have the part (%s).","ストリームは区画(%s)を持ちません").format(successor);

	info = new struct tagStreamPartInfo;
	info->name.assign(partname);
	info->flags = DEFAULT;
	current = container.insert(scope, info);
	
	return (*current)->stream;
}

void mpstrstream::erasepart(const char * partname)
{
	List::iterator scope;

	if((scope = _find(partname)) == container.end())
		return;

	if(scope == container.begin())
	{
		current = container.begin();
		current ++;
	}else
	{
		current = scope;
		current --;
	}

	delete (*scope);
	container.erase(scope);
}

void mpstrstream::movepart(const char * partname, const char * successor)
{
	List::iterator src;
	List::iterator dest;

	if((src = _find(partname)) == container.end())
		Exception("Stream dones not have the part (%s).","ストリームは区画(%s)を持ちません").format(partname);
	if(successor != 0)
	{
		if((dest = _find(successor)) == container.end())
			Exception("Stream dones not have the part (%s).","ストリームは区画(%s)を持ちません").format(successor);
	}else
		dest = container.end();

	current = container.insert(dest, (*src));
	container.erase(src);
}

void mpstrstream::serialize(void)
{
	fstream file(filename.c_str(), ios::out);

	if(!file.is_open())
		Exception("File creation failure of %s","ファイル(%s)が作れません").format(filename.c_str());

	serialize(file);

	file.close();
}

ostream & mpstrstream::serialize(ostream & out)
{
	List::iterator scope;

	scope = container.begin();
	while(scope != container.end())
	{
		if(((*scope)->flags & NO_SERIALIZE) == 0)
			out.write((*scope)->stream.str(), (*scope)->stream.pcount());
		scope ++;
	}

	return out;
}

int mpstrstream::setf(int flag)
{
	int old_flag;
	if(current == container.end())
		return -1;

	old_flag = (*current)->flags;
	(*current)->flags = flag;

	return old_flag;
}

