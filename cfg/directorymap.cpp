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
 *  @(#) $Id: directorymap.cpp,v 1.3 2002/04/05 08:48:31 takayuki Exp $
 */

// $Header: /home/CVS/configurator/directorymap.cpp,v 1.3 2002/04/05 08:48:31 takayuki Exp $

#include "directorymap.h"
#include "except.h"
#include <stdarg.h>
#include <typeinfo>

#ifdef _MSC_VER
  #pragma warning(disable:4786)
#endif

using namespace std;

int Directory::defaultflag = Directory::NOTHING;

Directory::Directory(const Directory & src)
{
    parent = 0;
    flag = defaultflag;
    defaultflag &= ~DESTRUCT;

    type = src.type;
    switch(type)
    {
    case LITERAL:
        content.literal = new string(*src.content.literal);
        break;
    default:
        content = src.content;
        break;
    }
}

Directory::~Directory(void)
{
	if(parent != 0)
	{
		parent->erase(myself);
		parent = 0;
	}
}

void Directory::_clear(void)
{
	switch(this->getType())
	{
	case LITERAL:
		delete content.literal;
		break;
	case OBJECT:
		delete content.instance;
		break;
	default:
		break;
	}

	type = UNKNOWN;
	content.pointer = 0;
}

Directory * Directory::_find(bool automatic_creation, const string & path)
{
    string::size_type top, tail, length;
    string work;
    Directory::iterator scope;
    Directory * node = this;

    length = path.length();
    top = 0;
    if(path[0] == '/')
    {
        while(node->getParent() != 0)
            node = node->getParent();
        top = 1;
    }

    do {
        tail = path.find_first_of('/', top);
        if(tail == string::npos)
            work = path.substr(top);
        else
            work = path.substr(top, tail-top);

		if(work.compare(".") == 0 || work.compare("..") == 0)
		{
			if(work.size() > 1 && node->getParent() != 0)
				node = node->getParent();
		}else
		{
			scope = node->begin();
			while(scope != node->end())
			{
				if(work.compare((*scope).first) == 0)
					break;
				scope ++;
			}

			if(scope == node->end())
			{
				if(!automatic_creation)
					return 0;
				node = node->addChild(work, new Directory);
			}else
				node = (*scope).second;
		}
        top = tail + 1;
    } while( tail != string::npos && top < length );

    return node;
}

Directory * Directory::_find(bool automatic_creation, const char * key, va_list vl)
{
    Directory::iterator scope;
    Directory * node = this;

    if(*key == '/' && *(key+1) == '\x0')
    {
        while(node->getParent() != 0)
            node = node->getParent();
		if(vl == 0)
			return node;
        key = va_arg(vl, const char *);
    }

    do {
        scope = node->begin();
        while(scope != node->end())
        {
            if((*scope).first.compare(key) == 0)
                break;
            scope ++;
        }

        if(scope == node->end())
        {
            if(!automatic_creation)
                return 0;
            node = node->addChild(key, new Directory);
        }else
            node = (*scope).second;

		if(vl != 0)
	        key = va_arg(vl, const char *);
		else
			break;
    } while( key != 0 );

    return node;
}

Directory & Directory::operator =(void * _pointer)
{
	if(this->getType() != UNKNOWN && this->getType() != POINTER)
		_clear();

	type = POINTER;
	content.pointer = _pointer;
	return *this;
}

Directory & Directory::operator =(long _value)
{
	if(this->getType() != UNKNOWN && this->getType() != INTEGER)
		_clear();

	type = INTEGER;
	content.value = _value;
	return *this;
}

Directory & Directory::operator =(const string & _literal)
{
	if(this->getType() != UNKNOWN && this->getType() != LITERAL)
		_clear();

	type = LITERAL;
	content.literal = new string(_literal);

	return *this;
}

Directory & Directory::operator =(const char * _constliteral)
{
	if(this->getType() != UNKNOWN && this->getType() != CONSTLITERAL)
		_clear();

	type = CONSTLITERAL;
	content.const_literal = _constliteral;
	return *this;
}

Directory & Directory::operator =(Garbage * _instance)
{
	if(this->getType() != UNKNOWN)
		_clear();

	type = OBJECT;
	content.instance = _instance;
	return *this;
}

void * Directory::operator new(size_t sz)
{
	defaultflag |= DESTRUCT;
	return malloc(sz);	
}

Directory::operator const long(void) const
{
	if( type == UNKNOWN )
		Exception("Bad cast exception","不正キャスト例外");
	return content.value;
}

void * Directory::operator * (void) const
{
	if( type == UNKNOWN )
		Exception("Bad cast exception","不正キャスト例外");
	return content.pointer;
}

void Directory::_erase(void)
{
	iterator scope;

	_clear();
	parent = 0;

	scope = begin();
	while(scope != end())
	{
		(*scope).second->_erase();
		scope ++;
	}

	map<string,Directory*>::clear();

	if((flag & DESTRUCT) != 0)
		delete this;
}

Directory::iterator Directory::erase(iterator it)
{
    iterator result;
	Directory * scope = (*it).second;
	scope->_erase();
    if((result = it) == begin())
      result ++;
    else
      result --;
    map<string, Directory*>::erase(it);
	return result;
}

void Directory::disconnect(void)
{
	if(parent != 0)
	{
		parent->map<string,Directory*>::erase(myself);
		parent = 0;
	}
}

Directory * Directory::getNext(void) const
{
	if(parent == 0)
		return 0;

	iterator scope;
	scope = myself;
	scope ++;
	if(scope == parent->end())
		return 0;

	return (*scope).second;
}

Directory * Directory::getPrev(void) const
{
	if(parent == 0 && myself == parent->begin())
		return 0;

	reverse_iterator scope;
	
	scope = parent->rbegin();
	while(scope != parent->rend() && (*scope).second != (*myself).second)
		scope ++;

	scope ++;
	return scope != parent->rend() ? (*scope).second : 0;
}

bool Directory::changeKey(const string & _key)
{
	Directory * _parent;

	if( _key.size() == 0)
		return false;

	_parent = parent;

	disconnect();
	_parent->addChild(_key, this);
	return true;
}


void Directory::drawTree(ostream * out, int level, string * link)
{
	iterator scope;
	iterator scope2;

	if(level == 0)
		link = new string;
	else
		*out << (*link).substr(0, (level-1)*3) << " +-";

	*out << '[' << getKey() << ']';
    switch(type)
    {
    case POINTER:
        out->setf(ios::hex);
        *out << " : PTR [" << content.pointer << "]";
        break;
    case INTEGER:
        out->setf(ios::dec);
        *out << " : INT [" << content.value << "]";
        break;
    case LITERAL:
        *out << " : STR [" << *content.literal << "]";
        break;
    case CONSTLITERAL:
        *out << " : CSTR[" << content.const_literal << "]";
        break;
    case OBJECT:
        {
            *out << " : OBJ";
            break;
        }
    default:
        *out << "UNKNOWN";
    }
    *out << endl;
	(*link) += " | ";
	scope = begin();
	while(scope != end())
	{
		scope2 = scope;
		scope ++;

		if(scope == end())
			(*link)[level*3+1] = ' ';
		(*scope2).second->drawTree(out, level+1, link);
	}


	link->erase(level*3);
	if(level == 0)
		delete link;
}

Directory * Directory::findChild(const char * key)
{
    string work(key);
    if(work.find_first_of('/') != string::npos)
        return findChild(work);
    return _find(false, key, 0);
}

Directory * Directory::findChild(const char * key, const char * second, ... )
{
    va_list vl;
    va_start(vl, second);
	vl = (char *)vl - sizeof(const char *);
    return _find(false, key, vl);
}

Directory * Directory::openChild(const char * key)
{
    string work(key);
    if(work.find_first_of('/') != string::npos)
        return openChild(work);
    return _find(true, key, 0);
}


Directory * Directory::openChild(const char * key, const char * second, ... )
{
    va_list vl;
    va_start(vl, second);
	vl = (char *)vl - sizeof(const char *);
    return _find(true, key, vl);
}

    //指定したキーを持つ子孫を探す。サーチ順は中順
Directory * Directory::findDescandant(const string & key, unsigned int level) const
{
    Directory::const_iterator scope;
    const Directory * node = this;

    if(empty())
        return 0;

        //子で探す
    scope = begin();
    while( scope != end() )
    {
        if((*scope).first.compare(key) == 0)
            return const_cast<Directory *>((*scope).second);
        scope ++;
    }

    if(level > 0)
    {
        scope = begin();
        while( scope != end() )
        {
            if((node = (*scope).second->findDescandant(key, level-1)) != 0)
                return const_cast<Directory *>(node);
            scope ++;
        }
    }
    return 0;
}

void Directory::copyTo(Directory * dest, int nest)
{
    Directory::iterator scope;
    Directory * node;

    node = dest;
    while(node != 0)
    {
        if(node == this)
            Exception("CopyTo: dest must not be a descendant node.","CopyTo: 子孫ノードへのコピーはできません");
        node = node->getParent();
    }

    scope = begin();
    while(scope != end())
    {
        node = dest->findChild((*scope).first);
        if(node != 0)
            node->erase();

        node = dest->addChild((*scope).first, new Directory(*(*scope).second));
        if(nest > 0)
            (*scope).second->copyTo(node, nest-1);

        scope++;
    }
}

 /*
  * ツリーのスナップショットを残す。とりあえずベタ吐き。ポインタとかもそのまま。
  * 豊富なリソース(CPU,メモリ,ディスク)はプログラマをだめにする。
  */
void Directory::Store(ostream * out)
{
    int i;
    Directory * node;

    out->write((const char *)&type, sizeof(type));
    switch(type)
    {
    case INTEGER:
        out->write((const char *)&content.value, sizeof(content.value));
        break;
    case LITERAL:
        i = content.literal->size();
        out->write((const char *)&i, sizeof(int));
        out->write(content.literal->c_str(), i);
        break;
    case CONSTLITERAL:
        i = strlen(content.const_literal);
        out->write((const char *)&i, sizeof(int));
        out->write(content.const_literal, i);
        break;
    default:
        out->write((const char *)&content.pointer, sizeof(content.pointer));
    }
    i = size();
    out->write((const char *)&i, sizeof(int));
    for(node = getFirstChild(); node != 0; node = node->getNext())
    {
        const string & work = node->getKey();
        i = work.size();
        out->write((const char *)&i, sizeof(int));
        out->write(work.c_str(), i);
        node->Store(out);
    }
}

void Directory::Load(istream * in)
{
    int i;
    int count;
    char buffer[1024];

    in->read((char *)&type, sizeof(type));
    switch(type)
    {
    case INTEGER:
        in->read((char *)&content.value, sizeof(content.value));
        break;
    case CONSTLITERAL:
    case LITERAL:
        in->read((char *)&i, sizeof(int));
        in->read(buffer, i);
        buffer[i] = '\x0';
        *this = string(buffer);
        break;
    default:
        in->read((char *)&content.pointer, sizeof(content.pointer));
    }
    in->read((char *)&count, sizeof(int));
    while(count-- > 0)
    {
        in->read((char *)&i, sizeof(int));
        in->read(buffer, i);
        buffer[i] = '\x0';

        addChild(buffer)->Load(in);
    }
}

string Directory::toString(const string & _default) const
{
	if(this == 0)
		return _default;

	switch(type)
	{
	case POINTER:
	case OBJECT:
		{
			char buffer[256];
			sprintf(buffer,"%08lx", (long)content.pointer);
			return string(buffer);
		}
	case INTEGER:
		{
			char buffer[256];
			sprintf(buffer,"%ld", content.value);
			return string(buffer);
		}
	case CONSTLITERAL:
		return string(content.const_literal);
	case LITERAL:
		return string(*content.literal);
        default:
            return _default;
	}

	return _default;
}

void * Directory::toPointer(const void * _default) const
{
	if(this == 0)
		return (void *)_default;

	switch(type)
	{
	case INTEGER:
		return (void *)&content.value;
	case CONSTLITERAL:
		return (void *)content.const_literal;
	case LITERAL:
		return (void *)content.literal->c_str();
	case POINTER:
		return (void *)content.pointer;
	case OBJECT:
		return (void *)content.instance;
	default:
	    return (void *)_default;
	}

	return (void *)_default;
}

long Directory::toInteger(const long _default) const
{
	bool minus = false;
	long work;
	const char * str;

	if(this == 0)
		return _default;

	switch(type)
	{
	case INTEGER:
		return content.value;

	case POINTER:
		return (long)content.pointer;

	case LITERAL:
	case CONSTLITERAL:
		if(type == CONSTLITERAL)
			str = content.const_literal;
		else
			str = content.literal->c_str();

		if(*str == '-')
		{
			minus = true;
			str ++;
		}

		if(*str == '0')
		{
			str ++;
			if(*str == 'x' || *str == 'X')
			{
				if(sscanf(str+1, "%ux", (int *)&work) == 0)
					return _default;
			}else
				if(*str != '\x0')
				{
					if(sscanf(str, "%o", (int *)&work) == 0)
						return _default;
				}else
					return 0;
		}else
			if(sscanf(str, "%d", (int *)&work) == 0)
				return _default;

		if(minus)
			work = -work;
		return work;
	default:
		return _default;
	}

	return _default;
}

string Directory::format(const string & fmt)
{
	Directory * node;
	string work;
	string key;
	string::size_type top,tail;
	int i;


	tail = 0;
	while((top = fmt.find_first_of('$', tail)) != string::npos)
	{
		if(top != tail)
			work += fmt.substr(tail, top - tail);

		if(fmt[top+1] == '$')
		{
			tail = top+2;
			work += '$';
			continue;
		}

		if(fmt[top+1] == '@')
		{
			work += (*myself).first;
			tail = top + 2;
		}
			

		if(fmt[top+1] == '(')
		{
			i = fmt.find_first_of(')', top);
			key = fmt.substr(top+2, i - top - 2);
			if(key.find_first_of('$') != string::npos)
			{
				int j;

				if(key.find_first_of('(') != string::npos)
				{
					key += ')';
					key = format(key);
					j = fmt.find_first_of(')',i+1);
					key += fmt.substr(i+1, j - i - 1);
					i = j;
				}else
					key = format(key);
			}
			node = _find(false,key);
			if(node != 0)
				work += node->toString("(null)");
			else
				work += "(null)";

			tail = i+1;
			continue;
		}
	}

	work += fmt.substr(tail);

	return work;
}
