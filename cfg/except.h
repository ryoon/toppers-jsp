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
 *  @(#) $Id: except.h,v 1.2 2002/04/05 08:48:31 takayuki Exp $
 */

// $Header: /home/CVS/configurator/except.h,v 1.2 2002/04/05 08:48:31 takayuki Exp $

#ifndef __EXCEPT
#define __EXCEPT

#ifdef _MSC_VER
	#pragma warning(disable:4786)
#endif

#include <string>
#include <stdarg.h>
#include <stdio.h>

#include <iostream>
#include <list>

#include "garbage.h"

#define NUM_SUPPORTLANGUAGE 2
#define MESSAGE_BUFFERSIZE  1024

/*
 * エラー時などにおけるメッセージは、呼び出すまでが面倒だとだんだん出力量が減っていくので、
 * できる限り簡単に埋め込めるようにしておく．そのせいでオーバーヘッドが増えてしまっても、
 * どうせホストで動くので無視．
 */

/*
 * Message : 複数語メッセージを保持するための基本クラス
 */

class Message
{
private:
	static unsigned long current_codepage_index;
	static unsigned long codepage[NUM_SUPPORTLANGUAGE];

protected:
	std::string message;

    Message(void) {};
    void initMessage(va_list, const char * = 0);

public:
	Message(const char *, ...);
	Message(Message &);

	operator const char * (void) const;
	operator const std::string & (void) const;

	static bool selectCodepage(unsigned long = 0x0000);

    template<class T>
    inline Message & format(const T first, ...)
    {
        va_list vl;
        char buffer[MESSAGE_BUFFERSIZE];

        va_start(vl, first);
        vl = (va_list)((char *)vl - sizeof(first));
        vsprintf(buffer, message.c_str(), vl);
        message.assign(buffer);

        return *this;
    }

    friend Message & format(Message &, ...);
    friend std::ostream & operator << (std::ostream &, const Message &);
};

inline Message::operator const char *(void) const
{	return message.c_str();	}

inline Message::operator const std::string & (void) const
{	return message;	}


//------

/*
 * VerboseMessage : 出力制御がついている一般メッセージ
 */

class VerboseMessage : public Message
{
private:
    static std::ostream * outputStream;
    static bool verboseControl;

public:
    VerboseMessage(const char *, ...);
    ~VerboseMessage(void);

    static inline void setStream(std::ostream * out)
    {   outputStream = out; };

    static inline void setVerbose(bool mode)
    {   verboseControl = mode;  };


	static inline bool getVerbose(void)
	{	return verboseControl && (outputStream != 0);	}
    
	template<class T>
    static inline void print(T msg)
    {
        if(getVerbose())
            (*outputStream) << msg;
    };
 
    template<class T>
    inline VerboseMessage & format(const T first, ...)
    {
        va_list vl;
        char buffer[MESSAGE_BUFFERSIZE];

        va_start(vl, first);
        vl = (va_list)((char *)vl - sizeof(first));
        vsprintf(buffer, message.c_str(), vl);
        message.assign(buffer);

        return *this;
    };
};

//------


//------

/*
 * Exception : 致命的な事象に対する言い訳メッセージ
 */

class Exception : public Message, public Garbage
{
protected:
    int  code;
    bool child;

    explicit Exception(const Exception *);
public:
    enum 
    {
        FATAL = 0x80000000 
    };

    Exception(int, const char * = 0, ...);
    Exception(const char *, ...);
    Exception(const Exception &);
    virtual ~Exception(void);

    bool isFatal(void) const;
    operator int (void) const;

    template<class T>
    inline Exception & format(const T first, ...)
    {
        va_list vl;
        char buffer[MESSAGE_BUFFERSIZE];

        va_start(vl, first);
        vl = (va_list)((char *)vl - sizeof(first));
        vsprintf(buffer, message.c_str(), vl);
        message.assign(buffer);

        return *this;
    }
};

#endif
