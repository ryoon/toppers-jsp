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
 *  @(#) $Id: filecontainer.h,v 1.3 2001/11/12 14:59:27 takayuki Exp $
 */

#ifndef __FILECONTAINER_H
#define __FILECONTAINER_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <map>
#include <string>

struct variable_information
{
	union {
		unsigned long address;
		long value;
	};
	unsigned int  size;
};

class filecontainer
{
protected:
	struct tagStructureInformation
	{
		unsigned int size;
		unsigned int offset;
	};

	std::map<std::string, std::string> objectname;
	std::map<std::string, struct tagStructureInformation> structure_information;

public:

	filecontainer(void) {};
	virtual ~filecontainer(void) {};

	virtual bool attach_module(const char *) =0;
	virtual bool change_endian(void * target, unsigned int size) =0;
	virtual bool load_contents(void * dest, unsigned long address, unsigned int size) =0;
	virtual unsigned long get_symbol_address(const char *) =0;
	virtual const char * get_architecture(void) =0;

	bool attach_infofile(const char *);
	bool load_variable(void * dest, unsigned int size, const char * name);
	bool load_variable(void * dest, unsigned int size, unsigned long address, const char * name);
	struct variable_information get_variable_information(const char *,unsigned long = 0);
	std::string get_objectname(int id, const char * type = "_GLOBAL");
};


extern filecontainer * container;

#endif