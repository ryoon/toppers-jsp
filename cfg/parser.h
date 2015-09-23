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
 *  @(#) $Id: parser.h,v 1.9 2002/04/11 07:26:37 takayuki Exp $
 */

// $Header: /home/CVS/configurator/parser.h,v 1.9 2002/04/11 07:26:37 takayuki Exp $

#ifndef __PARSER_H
#define __PARSER_H

#include "except.h"

#include "directorymap.h"
#include "component.h"

#include <stdarg.h>
#include <iostream>
#include <list>
#include <string>
#include <fstream>

#ifdef assert
#undef assert
#endif

class Token : public std::string
{
        //メンバの隠蔽はしない
public:
	enum tagTokenType
	{
		IDENTIFIER		= 0x01,
		INTEGER			= 0x02,
		STRINGLITERAL	= 0x03,
        STRING          = 0x04,
		OPERATOR		= 0x05,
		PUNCTUATOR		= 0x06,
		RESERVEDWORD	= 0x07,
		SPECIAL			= 0x80,
		SPACE			= 0x81,
		UNKNOWN			= 0xfd,
		ERROR			= 0xfe,
        EOS             = 0xff      //End of Stream
	};

	enum tagTokenType type;
	long value;
    unsigned int line;

	Token(void)			        { type = UNKNOWN;  value = 0;	};
	Token(const Token & src)	{ (*this) = src; };
	Token(enum tagTokenType, const char *);

	operator const enum tagTokenType (void) const { return type; };

	Token & operator =(const Token & src);
	bool operator == (const Token & src) const;

	Token & trim(void);
    Token & chopLiteral(void);
	void    assert(const char *) const;
};

class Parser
{
protected:
	struct tagFile
	{
		std::string    identifier;
		std::istream * stream;
		unsigned int   line;
	};

	static const char * Punctuator;
	static const char * Operator;
	static Token        lastErrorToken;
	Directory *         Container;

	tagFile *             current;
	std::list<Token>      TokenStack;
	std::list<tagFile *>  fileStack;

	bool getIdentifier(Token &, int);
	bool getWhitespace(Token &, int, bool);
	bool getInteger(Token &, int);
	bool getStringLiteral(Token &, int);
	bool getOperator(Token &, int);

    int  getChar(void);
    void putBack(int);

public:
	Parser(Directory & cnt) { Container = &cnt; current = 0; };
	Parser(Directory * cnt) { Container =  cnt; current = 0; };
	~Parser(void);

	void pushStream(const std::string & filename, std::string = "");
	void pushStdStream(std::string = "Standard Input");

	void setStreamIdentifier(const std::string & id) { current->identifier = id; };
	void setCurrentLine(unsigned int pos) { current->line = pos; };
    unsigned int getCurrentLine(void);
	const char * getStreamIdentifier(void);
	std::string getStreamLocation(void);

    enum Token::tagTokenType getToken(Token &,bool = false);
	void getToken(Token &,enum Token::tagTokenType, const char * = NULL);
	void getToken(const char *, const char * = 0, ...);

    void putBack(Token &);

	static const Token & getLastErrorToken(void) { return lastErrorToken; };
};


//---

class ParseUnit
{
protected:
	static Token & parseParameter(Parser &);
	static int     parseParameters(Parser &, Directory *, int, int=0);
	static int     parseParameters(Parser &, Directory *, const char *);

public:
	ParseUnit(void *, const char *);

    const char * getIdentifier(void) const;
    virtual void body(const std::string &, Directory &, Parser &, const std::string &) =0;

    static void printList(void * container);
};

#define __DECLARE_PARSEUNIT(x,y,z) class x##_##y : public x { public: x##_##y(void) : x(z) {}; protected: virtual void body(const std::string &, Directory &, Parser &, const std::string &); } instance_of_##x##_##y; void x##_##y::body(const std::string & identifier, Directory & container, Parser & p, const std::string & domain)

//---

class StaticAPI : public ParseUnit
{
protected:
	static Directory * last;
	Directory * allocate(Directory &, const Token &, const char *, bool = true);
	Directory * find    (Directory &, const Token &, const char *);
public:
    static std::map<std::string, class ParseUnit *> container;
	StaticAPI(const char * name) : ParseUnit(&container, name) {};
    static void printList(void) { ParseUnit::printList(&container); }
	static void dropLastObject(void);
};

#define DECLARE_API(x,y) __DECLARE_PARSEUNIT(StaticAPI,x,y)

//---

class Directive : public ParseUnit
{
public:
    static std::map<std::string, class ParseUnit *> container;
	Directive(const char * name) : ParseUnit(&container, name) {};
    static void printList(void) { ParseUnit::printList(&container); }
};

#define DECLARE_DIRECTIVE(x,y) __DECLARE_PARSEUNIT(Directive,x,y)

//---

class ParserComponent : public Component
{
protected:
    int failCount;

    virtual void parseOption(Directory &);    
    virtual void body(Directory &);

    bool parseStaticAPI(Parser & p, Directory & container, Token token, const std::string = "");

	virtual bool parse(Parser & p, Directory & container) = 0;

public:
    ParserComponent(void);
    virtual ~ParserComponent(void);
};

//---

inline void Parser::pushStdStream(std::string strid)
{
	if(current != 0)
		fileStack.push_front(current);

	current = new tagFile;
	current->stream     = &std::cin;
	current->identifier = strid;
	current->line       = 1;
}

inline Token::Token(enum tagTokenType type, const char * term)
{
	type = type;
	value = 0;
	assign(term);
}

inline Token & Token::operator =(const Token & src)
{	
	type = src.type; 
	value = src.value; 
    line = src.line;
	assign(src); 
	return *this;	
}

inline bool Token::operator ==(const Token & src) const
{
	if(type != src.type)
		return false;
	if(type == Token::INTEGER && value != src.value)
		return false;
	else
		if(compare(src) != 0)
			return false;

	return true;
}

inline void Token::assert(const char * str) const
{
	if(compare(str) != 0)
		Exception("Illegal token (%s) appears during parse process.","字句解析の途中で不正なトークン(%s)が出現しました").format(str);
}

inline void Parser::getToken(Token & token, enum Token::tagTokenType type, const char * term)
{
	getToken(token, type == Token::SPACE);

    if(type == Token::STRING && token == Token::STRINGLITERAL)
        token.chopLiteral();

	if((type != token) || (term != NULL && token.compare(term) != 0))
	{
		lastErrorToken = token;
		Exception("Parse error on reading [%s]","字句解析のエラー [%s]").format(token.c_str());
	}
}

inline void Parser::putBack(Token & token)
{	TokenStack.push_front(token);	}

inline Directory * StaticAPI::find(Directory & container, const Token & token, const char * id)
{
	Directory * node;

	node = container.findChild(id,token.c_str(),NULL);
	if(node == 0)
		Exception("The object %s(%s) does not exist.","オブジェクト%s(%s)は未定義です").format(token.c_str(),id);

	return node;
}

inline void StaticAPI::dropLastObject(void)
{
	if(last != NULL)
	{
		last->erase();
		last = NULL;
	}
}

#endif