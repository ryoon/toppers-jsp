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
 *  @(#) $Id: simpleparser.cpp,v 1.5 2000/11/14 16:57:33 takayuki Exp $
 */

#include "simpleparser.h"

using namespace std;

SimpleParser::SimpleParser(void)
{
	in = 0l;
	CurrentLinePos = 1;
	PreviousLinePos = 1;
}

SimpleParser::~SimpleParser(void)
{}


bool SimpleParser::SetStream(istream * source)
{
	if(in != 0l)
		delete in;

	in = source;
	return true;
}

bool SimpleParser::GetArrayedParameter(Array & param, bool parenthesis = false)
{
	char buffer[1024];
	int pos;
	int ch;
	int offset;

	if(in == 0l)
		return false;

	offset = 0;

	do
	{
		if(in->eof())
			return false;

		do{
			ch = in->get();
			if(ch == '\n')
				CurrentLinePos++;
		}while(ch == ' ' || ch == '\t' || ch == '\n' || ch=='\r');

		if(parenthesis)
		{
			if(ch == '(')
				ch = '{';
			if(ch == ')')
				ch = '}';
		}

		switch(ch)
		{
		case ',':
			break;
		case '{':
			{
				Array * newarray = new Array;
				param.Set(offset,newarray);
				offset++;
				if(!GetArrayedParameter(*newarray))
					return false;
				break;
			}
		case '}':
			return true;

		case '"':
			pos = 0;
			do{
				if((ch = in->get()) == '"')
					break;
				if(ch == '\\')
					ch = in->get();
				buffer[pos++] = ch;
			}while(true);
			buffer[pos] = '\0';
			param.Set(offset,new Valient(buffer));
			offset ++;
			break;

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			{
				int work = ch - '0';

				ch = in->get();
				while( ch>='0' && ch<='9' )
				{
					work = work * 10 + ch - '0';
					ch = in->get();
				}
				in->putback(ch);

				param.Set(offset++,new Valient(work));
				break;
			}

		default:
			buffer[0] = ch;
			pos = 1;
			do{
				buffer[pos++] = in->get();
			}while(buffer[pos-1] != ',' && !(parenthesis && buffer[pos-1] == ')') && buffer[pos-1] != '}');
			in->putback(buffer[pos-1]);
			buffer[pos-1] = '\0';
			param.Set(offset,new Valient(TrimString(buffer)));
			offset ++;
			break;
		}

	}while(true);

	return false;
}

bool SimpleParser::GetNextStatement(string & api,Array & param)
{
	char buffer[32];
	int pos;
	int ch;

	if(in == 0l)
		return false;

	pos = 0;

	if((ch = in->get()) == '#')
	{
		do
		{
			ch = in->get();
		}while(ch != '\xa' && !in->eof());
	}

	while( ch != '(' && !in->eof() )
	{
		if(ch == '\xa')
		{
			CurrentLinePos++;
			if((ch = in->get()) == '#')
			{
				do
				{
					ch = in->get();
				}while(ch != '\xa' && !in->eof());
			}else
				if( ch == '\xa')
					in->putback(ch);
		}

		if(ch != '\n' && ch != '\r' && ch != ' ' && ch != '\t')
			buffer[pos++] = ch;
		ch = in->get();
	}

	if(in->eof())
		return false;

	PreviousLinePos = CurrentLinePos;

	buffer[pos] = '\0';
	api = TrimString(buffer);

	if(!GetArrayedParameter(param,true))
		return false;

	do
	{
		if(in->eof())
			return false;

		ch = in->get();
		if(ch == '\n')
			CurrentLinePos++;
	}while(ch != ';');

	return true;
}
