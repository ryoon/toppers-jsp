#include "parser.h"
#include "jsp_classes.h"
#include <iostream>

using namespace std;

DECLARE_API(INCLUDE,"INCLUDE")
{
	Token token;
	Directory * node;
	Directory * scope;

	last = NULL;
	p.getToken(token, Token::STRINGLITERAL);
	token.chopLiteral();

	if( (token[0] != '\"' || token[token.size()-1] != '\"') &&
		(token[0] != '<' || token[token.size()-1] != '>') )
		Exception("An include filename must be enclosed with \"...\" or <...>","ファイル名は \"...\" か <...> の形でなければなりません");

	node = container.openChild(OBJECTTREE,INCLUDEFILE,NULL);
	last = node;

		//重複チェック
	scope = node->getFirstChild();
	while(scope != 0)
	{
		if(token.compare((*scope).toString()) == 0)
		{
			cerr << p.getStreamLocation() << ' ' << Message("Include file %s was already specified (ignored).","インクルードファイル%sが複数回指定されました (無視します)").format(token.c_str()) << endl;
			return;
		}
		scope = scope->getNext();
	}
	
	node->addChild(new Directory(token));
}

DECLARE_API(CRE_TSK,"CRE_TSK")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, TASK);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"tskatr,exinf,task,itskpri,stksz");
	p.getToken(",","NULL","}",NULL);

	(*node)["texatr"] = "TA_NULL";
	(*node)["texrtn"] = "NULL";
}

DECLARE_API(DEF_TEX,"DEF_TEX")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = find(container[OBJECTTREE], token, TASK);

	p.getToken(",","{",NULL);
	parseParameters(p, node, "texatr,texrtn");
	p.getToken("}");
}

DECLARE_API(CRE_SEM,"CRE_SEM")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, SEMAPHORE);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"sematr,isemcnt,maxsem");
	p.getToken("}");
}

DECLARE_API(CRE_FLG,"CRE_FLG")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, EVENTFLAG);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"flgatr,iflgptn");
	p.getToken("}");
}

DECLARE_API(CRE_DTQ,"CRE_DTQ")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, DATAQUEUE);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"dtqatr,dtqcnt");
	p.getToken(",","NULL","}",NULL);
}

DECLARE_API(CRE_MBX,"CRE_MBX")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, MAILBOX);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"mbxatr,maxmpri");
	p.getToken(",","NULL","}",NULL);
}

DECLARE_API(CRE_MPF,"CRE_MPF")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, FIXEDSIZEMEMORYPOOL);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"mpfatr,blkcnt,blksz");
	p.getToken(",","NULL","}",NULL);
}

DECLARE_API(CRE_CYC,"CRE_CYC")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, CYCLICHANDLER);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"cycatr,exinf,cychdr,cyctim,cycphs");
	p.getToken("}");
}

DECLARE_API(DEF_INH,"DEF_INH")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, INTERRUPTHANDLER, false);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"inhatr,inthdr");
	p.getToken("}");
}

DECLARE_API(DEF_EXC,"DEF_EXC")
{
	Token token;
	Directory * node;

	p.getToken(token);
	node = allocate(container[OBJECTTREE], token, EXCEPTIONHANDLER, false);
	(*node)["position"] = p.getStreamLocation();

	p.getToken(",","{",NULL);
	parseParameters(p,node,"excatr,exchdr");
	p.getToken("}");
}

DECLARE_API(ATT_INI,"ATT_INI")
{
	Token token;
	Directory * node;

	node = container.openChild(OBJECTTREE,INITIALIZER,NULL)->addChild();
	last = node;

	(*node)["position"] = p.getStreamLocation();

	p.getToken("{",NULL);
	parseParameters(p,node,"iniatr,exinf,inirtn");
	p.getToken("}");
}

DECLARE_API(ATT_FIN, "ATT_FIN")
{
	Token token;
	Directory * node;

	Component::barrier(Component::EXTENSION,Message("Use of the extended static API, ATT_FIN","拡張静的APIの使用 : ATT_FIN"));

	node = container.openChild(OBJECTTREE,FINALIZER,NULL)->addChild();
	last = node;

	(*node)["position"] = p.getStreamLocation();

	p.getToken("{",NULL);
	parseParameters(p,node,"finatr,exinf,finrtn");
	p.getToken("}");
}