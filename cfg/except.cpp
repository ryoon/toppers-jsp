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
 *  @(#) $Id: except.cpp,v 1.2 2002/04/05 08:48:31 takayuki Exp $
 */

// $Header: /home/CVS/configurator/except.cpp,v 1.2 2002/04/05 08:48:31 takayuki Exp $

#include "except.h"
#undef Throw

using namespace std;

unsigned long Message::current_codepage_index = 0;

unsigned long Message::codepage[NUM_SUPPORTLANGUAGE] =
{
	0x0000, /* Neutral language ( = English ) */
	0x0441  /* Japanese */
};

Message::Message(const char * msg, ...)
{
	va_list vl;
	va_start(vl, msg);
    initMessage(vl, msg);
}

void Message::initMessage(va_list vl, const char * msg)
{
    int count = current_codepage_index;

    while(count-- > 0)
        msg = va_arg(vl, const char *);

    message = msg;
}

Message::Message(Message & src)
{	message = src.message;	}


bool Message::selectCodepage(unsigned long _codepage)
{
	unsigned long i;

	for(i=0;i<NUM_SUPPORTLANGUAGE;i++)
		if(_codepage == codepage[i])
		{
			current_codepage_index = i;
			return true;
		}

	return false;
}
/*
Message & format(Message & str, ...)
{
	va_list vl;
	char buffer[1024];

	va_start(vl, str);
	vsprintf(buffer, (const char *)str, vl);

    str.message.assign(buffer);

	return str;
}
*/

ostream & operator << (ostream & out, const Message & msg)
{
    out << (const char *)msg;
    return out;
}

//------

Exception::Exception(int _code, const char * first, ...) : code(_code)
{
    va_list vl;

    if(first != 0)
    {
        va_start(vl, first);
        initMessage(vl, first);
    }else
        message = "Unknown Exception";
}

Exception::Exception(const char * first, ...) : code(FATAL), child(false)
{
    va_list vl;

    va_start(vl, first);
    initMessage(vl, first);
}

Exception::Exception(const Exception & exc)
{
    child = false;
    code = exc.code;
    message = exc.message;
}

Exception::Exception(const Exception * exc)
{
    child = true;
    code = exc->code;
    message = exc->message;
}

Exception::~Exception(void)
{
    if(!child)
    {
        this->Garbage::~Garbage();
        throw new Exception(this);
    }
}

bool Exception::isFatal(void) const
{   return code < 0;    }


Exception::operator int (void) const
{   return code;    }

//------

bool VerboseMessage::verboseControl = false;
std::ostream * VerboseMessage::outputStream = 0;

VerboseMessage::VerboseMessage(const char * msg, ...)
{
	va_list vl;
	va_start(vl, msg);
    initMessage(vl, msg);
}

VerboseMessage::~VerboseMessage(void)
{
    if(verboseControl && (outputStream != 0))
        *outputStream << message;
}
