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
 *  @(#) $Id: staticapi.cpp,v 1.4 2001/02/23 16:52:17 takayuki Exp $
 */

#include "staticapi.h"

char * StaticAPI::CheckParameter_Primitive(Array * param, char * format)
{
	int position;
	Valient * work;
	unsigned int flag;

	position = 0;
	flag = 0;
	while(true)
	{
		switch(*format)
		{
		case '{':
			if(!param->IsArray(position))
				throw Exception(MSG_ILLEGALTYPE);

			if((format = CheckParameter_Primitive(param->GetArrayPtr(position),format+1)) == '\x0')
				throw Exception(MSG_INTERNAL " : " MSG_SYNTAXERROR);

			if(*format == '\x0')
				return 0l;
			position ++;
			break;
		
		case '}':
		case ',':
		case '\x0':
			work = param->GetValuePtr(position);
			if(work == 0l)
				throw Exception(MSG_TOOFEWPARAM);
			
			if((flag & (1 << (int)work->GetType())) == 0)
				throw Exception(MSG_ILLEGALTYPE);
			
			if( *format == '}' )
				return format+1;
			if( *format == '\x0' )
				return 0l;

			flag = 0;
			position ++;
			break;

		case '-':
			flag |= ~0;
			break;

		case 'S':
		case 's':
			flag |= 1 << Valient::STRING;
			break;
		case 'I':
		case 'i':
			flag |= 1 << Valient::INTEGER;
			break;
		case 'D':
		case 'd':
			flag |= 1 << Valient::DOUBLE;
			break;
		case 'U':
		case 'u':
			flag |= 1 << Valient::UNSIGNEDINTEGER;
			break;
		case 'P':
		case 'p':
			flag |= 1 << Valient::POINTER;
			break;
		default:
			throw Exception(MSG_INTERNAL " : " MSG_SYNTAXERROR);
		}

		format ++;
	}
}
