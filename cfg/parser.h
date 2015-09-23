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
 *  @(#) $Id: parser.h,v 1.5 2001/11/12 14:59:27 takayuki Exp $
 */


#ifndef __PARSER_H
#define __PARSER_H

#include <iostream>
#include <list>
#include <string>

class Token : public std::string
{
public:
	enum tagTokenType
	{
		IDENTIFIER		= 0x01,
		INTEGER			= 0x02,
		STRINGLITERAL	= 0x03,
		OPERATOR		= 0x04,
		PUNCTUATOR		= 0x05,
		RESERVEDWORD	= 0x06,
		SPECIAL			= 0x80,
		SPACE			= 0x81,
		UNKNOWN			= 0xfe,
		ERROR			= 0xff
	};

	enum tagTokenType Type;
	long Value;

	Token(void)			{ Type = UNKNOWN;  Value = 0;	};
	Token(enum tagTokenType, const char *);
	Token(const Token & src)	{ (*this) = src; };

	operator const enum tagTokenType (void) const { return Type; };

	Token & operator =(const Token & src);
	bool operator == (const Token & src) const;

	Token & Trim(void);
};

class Parser
{
protected:
	std::istream * in;
	static const char * Punctuator;
	static const char * Operator;
	std::list<Token> TokenStack;

	bool GetIdentifier(Token &, int);
	bool GetWhitespace(Token &, int);
	bool GetInteger(Token &, int);
	bool GetStringLiteral(Token &, int);
	bool GetOperator(Token &, int);

public:
	Parser(void);
	~Parser(void) {};

	std::istream * SetStream(std::istream *);
	enum Token::tagTokenType GetToken(Token &,bool = false);
	void GetToken(Token &,enum Token::tagTokenType, const char * = NULL);
	void PutBack(Token &);
};

class ParserComponent
{
public:
	virtual void Parse(class Manager *, Parser *) =0;
};

inline std::istream * Parser::SetStream(std::istream * _in)
{
	std::istream * old = in;
	in = _in;
	return old;
}

inline Token::Token(enum tagTokenType type, const char * term)
{
	Type = type;
	Value = 0;
	assign(term);
}

inline Token & Token::operator =(const Token & src)
{	
	Type = src.Type; 
	Value = src.Value; 
	assign(src); 
	return *this;	
}

inline bool Token::operator ==(const Token & src) const
{
	if(Type != src.Type)
		return false;
	if(Type == Token::INTEGER && Value != src.Value)
		return false;
	else
		if(compare(src) != 0)
			return false;

	return true;
}

inline void Parser::PutBack(Token & token)
{	TokenStack.push_front(token);	}

#endif