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
 *  @(#) $Id: multistream.h,v 1.3 2000/11/14 16:57:33 takayuki Exp $
 */

#ifndef __MULTISTREAM_H
#define __MULTISTREAM_H

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <map>
#include <string>
#include <fstream>

class MultiStream
{
protected:
	std::map<std::string, std::fstream *> Stream;

public:
	MultiStream(void);
	~MultiStream(void);

	bool Add(const char *);
	bool Add(const std::string &);

	bool Close(const char *);
	bool Close(const std::string &);

	bool CloseAllStream(void);

	std::fstream * operator [](const char *);
	std::fstream * operator [](const std::string&);
};

inline bool MultiStream::Add(const char * src)
{	return Add(std::string(src));	}

inline bool MultiStream::Close(const char * src)
{	return Close(std::string(src));	}

inline std::fstream * MultiStream::operator [](const char * name)
{	return this->operator [] (std::string(name));	}

#endif