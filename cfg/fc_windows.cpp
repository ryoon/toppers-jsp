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
 *  @(#) $Id: fc_windows.cpp,v 1.2 2001/11/12 14:59:27 takayuki Exp $
 */



#include "filecontainer.h"

#include "exception.h"
#include "message.h"

#include <windows.h>
#include <imagehlp.h>

#include <string>

using namespace std;

class filecontainer_windows : public filecontainer
{
protected:
	HANDLE process;
	LOADED_IMAGE image;
	DWORD base;

public:
	filecontainer_windows(void);
	~filecontainer_windows(void);

	virtual bool attach_module(const char *);
	virtual bool change_endian(void * target, unsigned int size);
	virtual bool load_contents(void * dest, unsigned long address, unsigned int size);
	virtual unsigned long get_symbol_address(const char *);
	virtual const char * get_architecture(void) { return "Windows"; };

} Instance_of_filecontainer_windows;

filecontainer_windows::filecontainer_windows(void)
{
	process = 0;
	base = 0;
	container = this;
}

filecontainer_windows::~filecontainer_windows(void)
{}


bool filecontainer_windows::attach_module(const char * filename)
{
	process = ::GetCurrentProcess();
	if(::SymInitialize( process , NULL, FALSE) == FALSE)
	{
		Exception::Throw(Message("Internal Error : ImageHelper API initialization failure","内部エラー : 初期化に失敗しました (ImageHlp)"));
		return false;
	}
	
	base = ::SymLoadModule(process, NULL, const_cast<char *>(filename), NULL, 0, 0);
	
	image.SizeOfImage = sizeof(LOADED_IMAGE);
	if(::MapAndLoad(const_cast<char *>(filename), NULL, &image, FALSE, TRUE) == FALSE)
	{
		Exception::Throw(string(Message("Internel error : Module loading failure [","内部エラー:モジュールの読み込みに失敗しました [")) + filename + "]");
		return false;
	}
	return true;
}

bool filecontainer_windows::change_endian(void * target, unsigned int size)
{
		//Windowsネイティブ同士だから変換なんて要らない
	return true;
}


bool filecontainer_windows::load_contents(void * dest, unsigned long address, unsigned int size)
{
	PIMAGE_SECTION_HEADER header;
	unsigned int i;

	address -= base;
	for(i=0;i<image.NumberOfSections;i++)
	{
		header = image.Sections+i;
		if(address >= header->VirtualAddress && address < header->VirtualAddress + header->SizeOfRawData)
		{
			address -= header->VirtualAddress - header->PointerToRawData;
			::CopyMemory(dest,image.MappedAddress + address,size);

			return true;
		}
	}

	Exception::Throw(Message("Internel error: Memory read with unmapped address","内部エラー; マップされてないアドレスを使ってメモリリードが行われました"));

	return false;
}

unsigned long filecontainer_windows::get_symbol_address(const char * symbol)
{
	IMAGEHLP_SYMBOL sym;

	if(process == NULL || base == 0)
	{
		Exception::Throw(Message("Not initialized","初期化されてません"));
		return 0;
	}

	sym.SizeOfStruct = sizeof(sym);
	sym.MaxNameLength = 0;

	if(::SymGetSymFromName(process, const_cast<char *>(symbol), &sym) == FALSE)
	{
		Exception::Throw(string(Message("Unknown symbol [","不明なシンボル [")) + symbol + "]");
		return 0;
	}
	return sym.Address;
}
