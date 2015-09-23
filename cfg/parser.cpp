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
 *  @(#) $Id: parser.cpp,v 1.15 2002/04/11 08:07:53 takayuki Exp $
 */

// $Header: /home/CVS/configurator/parser.cpp,v 1.15 2002/04/11 08:07:53 takayuki Exp $

#include "parser.h"

using namespace std;

Token & Token::trim(void)
{
	string::iterator scope;

	scope = begin();
	while(*scope == ' ' || *scope == '\t' || *scope == '\r' || *scope == '\n')
		scope ++;
	erase(begin(), scope);

	if(!empty())
	{
		scope = end();
		do {
			scope --;
		} while(*scope == ' ' || *scope == '\t' || *scope == '\r' || *scope == '\n');
		scope ++;
		erase(scope, end());
	}
	return *this;
}

Token & Token::chopLiteral(void)
{
    if(type != STRINGLITERAL)
        return *this;

    string::iterator scope;

        //エラー処理のために非破壊処理をする
    string src(*this);
    string work;

    if(src[0] != '"' || src[src.length()-1] != '"')
        return *this;

    src = src.substr(1, src.length()-2);

    scope = src.begin();
    while(scope != src.end())
    {
        if(*scope == '\\')
        {
                //リテラルの末尾が\で終わることはないのでチェックしない
            scope++;
            switch(*scope)
            {
            case '"':   work += '"';   break;
            case 't':   work += '\t';  break;
            case 'r':   work += '\r';  break;
            case 'n':   work += '\n';  break;
            case 'b':   work += '\b';  break;
            case '\\':   work += '\\';  break;
            default:
                Exception("Illegal escape sequence [\\%c]","エスケープシーケンス[\\%c]は不正です").format(*scope);
            }
        }else
            work += *scope;

        scope ++;
    }

    type = STRING;
    assign(work);
    value = this->length();

    return *this;
}


const char * Parser::Punctuator =
	",;(){}";

const char * Parser::Operator =
	"+-*/&|%^~!?[]=:.#";

Token Parser::lastErrorToken;

inline int Parser::getChar(void)
{
    int work = current->stream->get();
    if(work == '\n')
        current->line ++;
    return work;
}

inline void Parser::putBack(int ch)
{
    if(ch == '\n')
        current->line --;
    current->stream->putback(ch);
}

Parser::~Parser(void)
{
	list<tagFile *>::iterator scope;

	if(current != 0)
		fileStack.push_front(current);

	scope = fileStack.begin();
	while(scope != fileStack.end())
	{
		if((*scope)->stream != 0 && (*scope)->stream != &cin)
			delete (*scope)->stream;
		delete (*scope);

		scope ++;
	}

	fileStack.clear();
	TokenStack.clear();
}

bool Parser::getIdentifier(Token & token,int ch)
{
	token.value = 0;

	do {
		token.value ++;
		token += static_cast<char>(ch);
        ch = getChar();
	} while( (ch >='a' && ch <= 'z') || (ch >='A' && ch <= 'Z') || (ch == '_') || (ch >= '0' && ch <= '9') );

	if(ch != -1)
		putBack(static_cast<char>(ch));

	token.type = Token::IDENTIFIER;
	return true;
}

bool Parser::getWhitespace(Token & token, int ch, bool allow_space)
{
	int prev;

	token.type = Token::SPACE;
	switch(ch)
	{
	case '/':
		ch = getChar();

		switch(ch)
		{
		case '*':
			token += "/*";
			prev = '\x0';
			while( ((ch = getChar()) != '/') || (prev!='*'))
			{
				token += static_cast<char>(ch);
				prev = ch;
			}
			token += static_cast<char>(ch);
			break;
		case '/':
			token += '/';
			do {
				token += static_cast<char>(ch);
			} while( (ch = getChar()) != '\n' );
			break;
		default:
			putBack(ch);
			return false;
		}
		break;

	case '#':
		{
			Token directive;
			map<string, ParseUnit *>::iterator scope;

			do {
				token += static_cast<char>(ch);
				ch = getChar();
			} while(ch == ' ' || ch == '\t');
			
			if(ch >= '0' && ch <= '9')
			{
				directive.assign("line");
				this->putBack(ch);
			}else
			{
				putBack(ch);
				getToken(directive);
				token += directive;
			}

			if(directive.compare("line") == 0)
			{
				Token token;

				getToken(token, Token::INTEGER);
				setCurrentLine(token.value -1);

				getToken(token, Token::STRINGLITERAL);
				token.chopLiteral();
				setStreamIdentifier(token);
			}else 
			{
				if(directive.compare("pragma") == 0)	
				{
					getToken(directive);
					token += " ";
					token += directive;

					if((scope = Directive::container.find(directive)) != Directive::container.end())
					{
						(*scope).second->body(directive, *Container, *this, string(""));

						if(!TokenStack.empty())
						{
							do {
								token = TokenStack.front();
								TokenStack.pop_front();
							} while(!TokenStack.empty() && !allow_space && token.type == Token::SPACE);

							return true;
						}
					}else
					{
						do {
							token += static_cast<char>(ch);
							ch = getChar();
						} while(ch != '\n');
						putBack(ch);
					}
				}else
				{
					if(directive.compare("include") == 0)
					{
						cerr << Message("Configurator found 'include' directive at ","#includeディレクティブを発見しました ") << getStreamLocation() << endl << Message("Kernel configuration file must be preprocessed by cpp"," カーネル構成ファイルはCプリプロセッサを通過させる必要があります") << endl;
						Exception("Illegal kernel configuration file","不正なカーネル構成ファイル");
					}
				}
			}
			break;
		}

	case ' ':
	case '\t':
	case '\n':
	case '\r':
		do {
			token += static_cast<char>(ch);
			ch = getChar();
		} while((ch == ' ') || (ch == '\n') || (ch == '\r') || (ch == '\t'));
		putBack(static_cast<char>(ch));
		break;
	}
	return true;
}

bool Parser::getInteger(Token & token, int ch)
{
	bool minus = false; 

	if(ch == '-')
	{
		minus = true;
		ch = getChar();
		if(ch < '0' || ch >'9')
		{
			putBack(static_cast<char>(ch));
			return false;
		}
		token += "-";
	}

	token.type = Token::INTEGER;
	token.value = 0;


	if(ch == '0')
	{
		token += static_cast<char>(ch);
		ch = getChar();
		if(ch == 'x' || ch == 'X')
		{
			token += static_cast<char>(ch);
			ch = getChar();
			while((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F'))
			{
				token += static_cast<char>(ch);
				if((ch -= '0') >= 10)
				{
					ch = ch + '0' - 'A' + 10;
					if(ch >= 16)
						ch = ch - ('a' - 'A');
				}
				token.value = token.value * 16 + ch;
				ch = getChar();
			}
		}else
		{
			while(ch >= '0' && ch <= '7')
			{
				token += static_cast<char>(ch);
				token.value = token.value * 8 + ch - '0';
				ch = getChar();
			}
		}
	}else
	{
		do {
			token += static_cast<char>(ch);
			token.value = token.value * 10 + ch - '0';
			ch = getChar();
		} while(ch >= '0' && ch <= '9');
	}

	if(minus)
		token.value = - token.value;

	if(ch != -1)
		putBack(static_cast<char>(ch));

	return true;
}

bool Parser::getOperator(Token & token, int ch)
{
	const char * work;

	for(work = Operator;*work != '\x0' && *work != ch;work++);

	if(*work == '\x0')
		return false;

	do {
		token += static_cast<char>(ch);
		ch = getChar();
		for(work = Operator;*work != '\x0' && *work != ch;work++);
	} while(*work != '\x0');

	putBack(ch);
	token.type = Token::OPERATOR;
	return true;
}

bool Parser::getStringLiteral(Token & token, int ch)
{
	int prev;
	int position;
	int prev_line;

	prev_line = current->line;
	position  = current->stream->tellg();

	token.value = 1;
	token.type = Token::STRINGLITERAL;
    token.assign("");
    token += static_cast<char>(ch);

    while(!current->stream->bad() && !current->stream->eof())
    {
    	prev = ch;
	    ch = getChar();
        token += static_cast<char>(ch);
        token.value ++;

        if(ch == '"' && prev != '\\')
            return true;
    }

		//いったん閉じて再オープンして、リテラル開始の " の次に移動
	
	current->line = prev_line;
	current->stream->seekg(position);
	current->stream->clear();

    Exception("Unterminated string literal appeared.","閉じられていない文字リテラルを検出しました");
    return false;
}

enum Token::tagTokenType Parser::getToken(Token & token, bool allow_space)
{
    static bool isHeadofLine = true;
	int ch;
	const char * work;

	do {
		token.erase();
		token.type = Token::ERROR;
		token.value = 0;

			//トークンスタックから切り出す
		if(!TokenStack.empty())
		{
			do {
				token = TokenStack.front();
				TokenStack.pop_front();
			} while(!TokenStack.empty() && !allow_space && token.type == Token::SPACE);

			if(!allow_space && token.type != Token::SPACE)
				return token.type;
		}

			//ストリームから切り出す
		if(current == NULL || current->stream == NULL || current->stream->bad())
		{
			token.assign("<End of stream>");
			return (token.type = Token::EOS);
		}
		if(current->stream->eof())
		{
			if(!fileStack.empty())
			{
				if(current->stream != &cin)
					delete current->stream;
				delete current;

				if(fileStack.empty())
				{
					token.assign("<End of stream>");
					return (token.type = Token::EOS);
				}
				
				current = *fileStack.begin();
				fileStack.pop_front();
			}else
			{
				token.assign("<End of stream>");
				return (token.type = Token::EOS);
			}
		}

		ch = getChar();

			//First(whitespaces) is [ \n\t\r/#]
		if( (ch == ' ') || (ch == '\t') || (ch == '\n') || (ch == '\r') || (ch == '/') || (isHeadofLine && ch == '#'))
		{
			if(ch == '\n')
				isHeadofLine = true;

			if(getWhitespace(token, ch, allow_space))
				if((token == Token::SPACE && allow_space) || !(token == Token::SPACE || token == Token::ERROR))
					return token.type;

			continue;
		}else
			break;
	}while(true);

    isHeadofLine = false;

    token.line = current->line;

		//First(identifier) is [a-zA-Z_]
	if(	(ch >='a' && ch <= 'z') || (ch >='A' && ch <= 'Z') || (ch == '_') )
		if(getIdentifier(token, ch))
			return Token::IDENTIFIER;

		//First(integer) is [\-0-9]
	if( (ch >='0' && ch <='9') || (ch == '-') )
		if(getInteger(token,ch))
			return Token::INTEGER;

		//First(string) is ["]
	if( ch == '"' )
		if(getStringLiteral(token,ch))
			return Token::STRINGLITERAL;

		//Operator
	if(getOperator(token,ch))
		return Token::OPERATOR;

		//Punctuator
	work = Punctuator;
	while(*work != '\x0')
		if( *(work++) == ch )
		{
			token += static_cast<char>(ch);
			return (token.type = Token::PUNCTUATOR);
		}

	token += static_cast<char>(ch);
	token.type = Token::UNKNOWN;
	return Token::UNKNOWN;
}

ostream & operator << (ostream & out, Token & src)
{
	switch(src.type)
	{
	case Token::IDENTIFIER:
		out << "<IDENTIFIER:["; break;
	case Token::INTEGER:
		out << "<INTEGER:["; break;
	case Token::STRINGLITERAL:
		out << "<STRINGLITERAL:["; break;
	case Token::STRING:
		out << "<STRING:["; break;
	case Token::OPERATOR:
		out << "<OPERATOR:["; break;
	case Token::PUNCTUATOR:
		out << "<PUNCTUATOR:["; break;
	case Token::RESERVEDWORD:
		out << "<RESERVEDWORD:["; break;
	case Token::SPECIAL:
		out << "<SPECIAL:["; break;
	case Token::SPACE:
		out << "<SPACE:["; break;
	case Token::UNKNOWN:
		out << "<UNKNOWN>"; return out;
	case Token::ERROR:
		out << "<ERROR>"; return out;
	default:
		out << "<???:[";
	}

	return out << static_cast<string &>(src) << "]("<<src.value<<")>";
}

void Parser::getToken(const char * term, const char * second, ...)
{
    Token token;
	va_list vl;

    if(term == NULL)
        Exception("Internal: GetToken received an empty string as reserved word.","内部エラー: GetTokenに空文字列が渡されました");

	va_start(vl, second);
	vl = (char *)vl - sizeof(const char *);

	do {
		getToken(token, false);
		if(token.compare(term) != 0)
		{
			lastErrorToken = token;
			Exception("Token [%s] should be replaced by [%s]","字句[%s]は[%s]であるべきです").format(token.c_str(),term);
		}
		term = va_arg(vl, const char *);
	}while(term != 0);
}

string Parser::getStreamLocation(void)
{
	list<tagFile *>::iterator scope;

	string location("[");
	char buffer[16];

	if(current == 0)
		Exception("Stream handling method invocation occured before attaching a stream.","不正なストリーム操作命令");

	::sprintf(buffer, ":%d", current->line);
	location += current->identifier;
	location += buffer;

	if(!fileStack.empty())
	{
		location += " (included at ";

		scope = fileStack.begin();
		while(scope != fileStack.end())
		{
			::sprintf(buffer, ":%d, ", (*scope)->line);
			location += (*scope)->identifier;
			location += buffer;

			scope ++;

		}

		location.erase(location.size()-2);
		location += ")";
	}

	location += "]";

	return location;
}

void Parser::pushStream(const std::string & filename, std::string strid)
{
	fstream * fin;

	if(current != 0)
		fileStack.push_front(current);
		
	fin = new fstream(filename.c_str(),ios::in);

	if(fin->is_open())
	{
		if(strid.size() == 0)
			strid = filename;

		current = new tagFile;
		current->stream     = fin;
		current->identifier = strid;
		current->line       = 1;
	}else
	{		
		Exception("File operatation failure : [%s]","ファイル操作に失敗しました [%s]").format(filename.c_str());
		delete fin;
	}
}

map<string, class ParseUnit *> StaticAPI::container;
map<string, class ParseUnit *> Directive::container;

ParseUnit::ParseUnit(void * _container, const char * name)
{
	map<string, ParseUnit *> * container;
    string work(name);
	string apiname;
	string::size_type i,j;

	i = 0;
	container = reinterpret_cast<map<string, ParseUnit *> *>(_container);

//	j = container->size();
	
	do {
		j = work.find_first_of(',', i);
		apiname = work.substr(i, j-i);

		if(container->find(apiname) != container->end())
	        Exception("Multiple registration of [%s]\n","[%s]が重複して登録されようとしています").format(apiname.c_str());
	    (*container)[apiname] = this;
		i = j + 1;
	}while(j != string::npos);
}

void ParseUnit::printList(void * _container)
{
    int i;
	map<string, ParseUnit *> * container;
    map<string, ParseUnit *>::iterator scope;

	container = reinterpret_cast<map<string, ParseUnit *> *>(_container);
    if(container->empty())
    {
        cout << "  " << Message("None of element registed", "登録されている要素はありません") << endl;
        return;
    }

    i = 0;
    scope = container->begin();
    while(scope != container->end())
    {
        cout << '[' << (*scope).first << "] ";

        if(i++ >= 6)
        {
            i = 0;
            cout << endl;
        }

        scope ++;
    }

    if(i != 0)
        cout << endl;
}

Token & ParseUnit::parseParameter(Parser & p)
{
	static Token result;
	Token token;
	int nest = 0;

	result.type = Token::ERROR;
	result.value = 0;
	result.assign("");

	do
	{
		p.getToken(token);
		if(token == Token::PUNCTUATOR)
		{
			if(token.compare("(") == 0)
				nest ++;
			else if(token.compare(")") == 0)
				nest --;
			else if(nest == 0)
				break;
			if(nest < 0)
				Exception("')' appeared before '('.","対応しない閉じ括弧があります");
		}

		if(result == Token::ERROR)
			result = token;
		else
		{
			result.type = Token::STRING;
			result += ' ';
			result += token;
		}

	}while(true);

	p.putBack(token);
	result.trim();
	return result;
}

int ParseUnit::parseParameters(Parser & p, Directory * container, int min, int max)
{
	Token work;
	int count = 0;

	if(max == 0)
		max = min;

	do
	{
		Token & token = parseParameter(p);
		if(token.type == Token::ERROR)
			break;

		if(token == Token::INTEGER)
			container->addChild(new Directory(token.value));
		else
			container->addChild(new Directory((string &)token));

		count ++;
		p.getToken(work);
	}while(work.compare(",")==0 && count < max);

	if(count < min)
		Exception("Too few parameters [%d/%d]","パラメータの数が少なすぎます [%d/%d]").format(count, min);

	p.putBack(work);
	return count;
}

int ParseUnit::parseParameters(Parser & p, Directory * container, const char * paramlist)
{
	Token work;
	int count;
	string list;
	string key;
	string::size_type head,tail;

	list.assign(paramlist);

	count = 0;
	head = 0;
	tail = list.find_first_of(',');
	key = list.substr(head,tail-head);

	do
	{
		if(head == string::npos)
			Exception("Too many parameters","パラメータの数が多すぎます");

		Token & token = parseParameter(p);
		if(token.type == Token::ERROR)
			break;

		if(token == Token::INTEGER)
			container->addChild(key,new Directory(token.value));
		else
			container->addChild(key,new Directory((string &)token));

		if(tail != string::npos)
		{
			head = tail + 1;
			tail = list.find_first_of(',',head);
			key = list.substr(head,tail-head);
			count ++;
			p.getToken(work);
		}else
			break;
	}while(work.compare(",")==0);

	if(tail != string::npos)
		Exception("Too few parameters","パラメータの数が少なすぎます");

	return count;
}

//------

Directory * StaticAPI::last = NULL;

Directory * StaticAPI::allocate(Directory & container, const Token & token, const char * id, bool regist)
{
	static unsigned int assignment_count = 0;
	Directory * node;

	if(!(token == Token::IDENTIFIER || token == Token::INTEGER))
		Exception("Given token(%s) is not suitable for an object identifier.","オブジェクトの識別名として利用できない字句(%s)が指定されました").format(token.c_str());

	if(regist && (token == Token::INTEGER && token.value <= 0))
		Exception("Cannot assign an ID number less or equal to 0.","0以下のID番号を設定することはできません");


	node = container.findChild(id);
	if(node != 0)
	{
		Directory::iterator scope;

		scope = node->begin();
		while(scope != node->end())
		{
			if((*scope).first.compare(token) == 0)
				Exception("Identifier %s is already used.","識別名%sはすでに利用されています").format(token.c_str());
			scope ++;
		}
	}else
		node = container.addChild(id);

	node = node->addChild(token);
	(*node)["#order"] = assignment_count++;

	if(token == Token::IDENTIFIER)
	{
		if(regist)
		{
			Directory * scope = container.openChild("/","identifier",token.c_str(),NULL);
			if(*scope == Directory::INTEGER)
				*node = scope->toInteger();
		}
	}else
		*node = token.value;

	last = node;
	return node;
}

//------

ParserComponent::ParserComponent(void)
{}

ParserComponent::~ParserComponent(void)
{}

void ParserComponent::parseOption(Directory & container)
{
    if(findOption(container, "h", "help"))
    {
        cout << Message(
            "Static API parser\n"
            "  -s, --source=filename : Specify the source file\n"
            "  -iapi ,--ignore-api   : Ignore unknown static api\n"
            "  --print-api           : Show registered static api list\n", 
            "静的APIパーサ\n"
            "  -s, --source=ファイル名  : 入力ファイル名を指定します\n"
            "  -iapi, --ignore-api      : 登録されていないAPIを無視します\n"
            "  --print-api              : 登録されているAPIの一覧を表示します\n");
        return;
    }

    if(findOption(container, 0, "print-api"))
    {
		cout << Message("List of Registerd Static API","静的API 一覧") << std::endl;
        StaticAPI::printList();
        return;
    }

	if(mergeOption(container, "s", DEFAULT_PARAMETER) != 0 || mergeOption(container, "s", "-source") != 0)
		activateComponent();
}

bool ParserComponent::parseStaticAPI(Parser & p, Directory & container, Token token, const string domain)
{
    map<string, ParseUnit *>::iterator api;

    if(token.type != Token::IDENTIFIER)
        return false;

	try {
	    api = StaticAPI::container.find(token);
	    if(api == StaticAPI::container.end())
	    {
	        if(findOption(container, "iapi", "ignore-api"))
	        {
	            cerr << Message("Unknown static api %s at %s was ignored. (skipped)\n","非登録のAPI %s%sは無視されます\n").format(token.c_str(), p.getStreamLocation().c_str());
	            do {
	                p.getToken(token);
	            }while(token.compare(";") != 0);
				p.putBack(token);
	            return true;
	        }
	        Exception("Static API [%s] is not registered in the configurator", "静的API[%s]は未登録です").format(token.c_str());
		}

		if(findOption(container,"D","debug"))
			cerr << Message("  StaticAPI [%s]\n","  静的API  [%s]\n").format((*api).first.c_str());

	    p.getToken("(");

	    (*api).second->body(token, container, p, domain);

	    p.getToken(")");
	    p.getToken(";");
	}
	catch(Exception * e)
	{
		string work;
		work = p.getStreamLocation() + string(" ") + (const string &)*e;

		dumpContainerSnapshot(container);
		StaticAPI::dropLastObject();
		failCount ++;

		token = p.getLastErrorToken();
		while (token != Token::ERROR && token != Token::EOS)
		{
			if( token == ";" )
				break;

				//読み出したトークンが静的APIと同じ名前なら きっとセミコロン忘れ
		    api = StaticAPI::container.find(token);
			if(api != StaticAPI::container.end())
			{
				cerr << Message("<The following error must be occured by lack of ';' at the end of previous line>","<次のエラーは直前行の';'忘れによる可能性が高いです>")
				     << endl;
				p.putBack(token);
				break;
			}

			p.getToken(token);
        }
		cerr << Message("Error : %s\n", "エラー : %s\n").format(work.c_str());
	}

    return true;
}

void ParserComponent::body(Directory & container)
{
    Token token;
    Parser p(container);
    Directory * scope;

	failCount = 0;
	scope = getOption(container, "s");
    if(scope->size() == 0)
    {
        scope = 0;
        p.pushStdStream(Message("Standard Input","標準入力"));
        VerboseMessage("Starting parse with standard input\n","標準入力からの字句解析を開始しました\n");
    }else
		scope = scope->getFirstChild();

    try{
        do {
            if(scope != 0)
            {
                VerboseMessage("Starting parse with file[%s]\n","ファイル[%s]の字句解析を開始しました\n").format((const char *)*scope);
				p.pushStream(*scope);
            }

			this->parse(p, container);

			if(p.getToken(token) != Token::EOS)
                Exception("Buffer has remaining tokens, parsing is not yet finished", "パースが中断されました");

			if(failCount != 0)
				Exception("Total %d failures found in this configuration.","%d個の障害があります").format(failCount);

            VerboseMessage("Parse finished\n","字句解析は正常に終了しました\n");

        } while(scope != 0 && (scope = scope->getNext()) != 0);
    }
    catch(Exception * e)
    {
		string work;

		work = p.getStreamLocation() + string(" ") + (const string &)*e;
		Exception("Error : %s\n", "エラー : %s\n").format(work.c_str());
    }
}

