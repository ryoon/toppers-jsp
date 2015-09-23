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
 *  @(#) $Id: filecontainer.cpp,v 1.2 2001/11/12 14:59:27 takayuki Exp $
 */


#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <stdio.h>

#include "exception.h"
#include "message.h"
#include "filecontainer.h"

using namespace std;

filecontainer * container;

static char * parse_uint(char * src, unsigned int * dest, unsigned int default_value = 0)
{
	while(*src >= '0' && *src <='9')
		default_value = default_value * 10 + *(src++) - '0';

	*dest = default_value;
	return src;
}

bool filecontainer::attach_infofile(const char * filename)
{
	char buffer[256];
	FILE * f;
	char * work;
	struct tagStructureInformation info;

	f = fopen(filename,"rt");
	if(f == NULL)
	{
		Exception::Throw(string(Message("Structure information file was not found. ","構造体情報ファイルが見つかりません ")) + "[" + filename + "]");
		return false;
	}
	while(fgets(buffer, 255, f) != NULL)
	{
		work = buffer + strlen(buffer) -1;
		if(*work == '\n')
			*work = '\x0';

		switch(buffer[0])
		{
		case 's':
			work = buffer+1;
			while(*work != ',' && *work != '\x0')
				work++;
			if(*work == '\x0')
				continue;

			*work = '\x0';
			work = parse_uint(work+1,&info.size);
			work = parse_uint(work+1,&info.offset);

			structure_information[string(buffer+1)] = info;
			break;

		case 'd':
			work = buffer+1;
			while(*work != ',' && *work != '\x0')
				work++;
			if(*work == '\x0')
				continue;

			*work = '\x0';

			objectname[string(buffer+1)] = string(work+1);
			break;

		default:
			Exception::Throw(string(Message("Unknown information type identifier found - [","未知の型識別子 - ")) + buffer[0] + "]");
		}
	}

	fclose(f);
	return true;
}

bool filecontainer::load_variable(void * dest, unsigned int size, const char * name)
{
	map<string, struct tagStructureInformation>::iterator scope;
	unsigned long address;

	scope = structure_information.find(string(name));
	if(scope == structure_information.end())
		return false;

	if(size > (*scope).second.size)
		return false;

	if((*scope).second.size < size)
		size = (*scope).second.size;

	address = get_symbol_address(name);
	if(address == 0 || !load_contents(dest, address + (*scope).second.offset, size))
		return false;

	if(!change_endian(dest,size))
		return false;

	return true;
}

bool filecontainer::load_variable(void * dest, unsigned int size, unsigned long address, const char * name)
{
	map<string, struct tagStructureInformation>::iterator scope;

	scope = structure_information.find(string(name));
	if(scope == structure_information.end())
		return false;

	if(size < (*scope).second.size)
		return false;

	if((*scope).second.size < size)
		size = (*scope).second.size;

	if(!load_contents(dest, address + (*scope).second.offset, size))
		return false;

	if(!change_endian(dest,size))
		return false;

	return true;
}

struct variable_information filecontainer::get_variable_information(const char * name, unsigned long base)
{
	struct variable_information result;
	map<string, struct tagStructureInformation>::iterator scope;
	const char * work;
	bool is_member = false;

	result.address = 0;
	result.size = 0;

	for(work = name; *work != '\x0'; work++)
		if(*work == ':')
		{
			is_member = true;
			break;
		}

	scope = structure_information.find(string(name));
	if(scope != structure_information.end())
	{
		if(!is_member)
		{
			bool old = Exception::SetThrowControl(false);
			result.address = this->get_symbol_address(name);
			Exception::SetThrowControl(old);
			if(result.address == 0)
				result.value = (long)((*scope).second.size);
			else
				result.size = (*scope).second.size;
		}else
		{
			result.address = base + (*scope).second.offset;
			result.size = (*scope).second.size;
		}
	}

	return result;
}

string filecontainer::get_objectname(int id, const char * type)
{
	char buffer[32];
	string work;
	map<string, string>::iterator scope;

	work = type;

	sprintf(buffer, "_%d", id);
	work += buffer;

	scope = objectname.find(work);
	if(scope != objectname.end())
		work = (*scope).second;
	else
		work.erase();

	return work;
}

