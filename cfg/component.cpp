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
 *  @(#) $Id: component.cpp,v 1.4 2002/04/11 07:26:37 takayuki Exp $
 */

// $Header: /home/CVS/configurator/component.cpp,v 1.4 2002/04/11 07:26:37 takayuki Exp $

#include "component.h"
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

list<Component *> Component::componentList;
list<Component *> Component::activatedComponentList;
int Component::barrierFlag = 0;

Component::~Component(void)
{
    list<Component *>::iterator scope;

	scope = componentList.begin();
	while(scope != componentList.end())
	{
		if(*scope == this)
		{
	        componentList.erase(scope);
			break;
		}
		scope ++;
	}
}

void Component::activateComponent(bool allow_multiinstance)
{
    if(allow_multiinstance || (find(activatedComponentList.begin(),activatedComponentList.end(), this) == activatedComponentList.end()))
        activatedComponentList.push_back(this);  
}

Directory * Component::getOption(Directory & container, const char * id, const char * longId)
{
    Directory * param;
    Directory * scope;
    string identifier;

    param = container.findChild("Parameter");

    if( ((id != 0) && ((scope = param->findChild(id)) != 0)) ||
        ((longId != 0) && ((scope = param->findChild(string("-") + longId)) != 0)) )
    {
		return scope;
    }

    return NULL;
}

Directory * Component::mergeOption(Directory & container, const char * to, const char * from)
{
	Directory * src;
	Directory * dest;
	Directory * node;

	src  = container.findChild("Parameter",to,NULL);
	dest = container.findChild(string("Parameter/") + from);

	if(dest == 0)
		return src;

	if(src == 0)
		src = container.openChild("Parameter",to,NULL);

	node = dest->getFirstChild();
	while(node != 0)
	{
		src->addChild(new Directory(node->toString()));
		node = node->getNext();
	}

	dest->erase();

	return src;
}

 
static void addOptionItem(Directory * parameter, const string & key, const string & item)
{
    Directory * node;
    char buffer[32];

    node = parameter->findChild(key);

    if(item.empty())
    {
        if(node == 0)
            parameter->addChild(key, new Directory(""));
        else
            *node = "";
    }else
    {
        if(node != 0)
        {
            sprintf(buffer,"%03d", node->size());
            node->addChild(buffer, item.empty() ? new Directory("") : new Directory(item));
            *node = item;
        }else
            parameter->addChild(key, new Directory(item))->addChild("000", new Directory(item));
    }
}

void Component::parseOption(int argc, char ** argv, Directory & container)
{
    list<Component *>::iterator scope;
    Directory * parameter;
	string work;
	string prevparam;
	string::size_type i,j;
 
    try{
        parameter = container.addChild("Parameter");

		for(i=1;*(argv+i)!=0;i++)
		{
			work += *(argv+i);
			work += ' ';
		}
		(*parameter)["#cmdline"] = work.substr(0,work.size()-1);
		
		
            //パラメータをパースする
		prevparam = DEFAULT_PARAMETER;
	    while(*(++argv) != 0)
	    {
		    work.assign(*argv);
		    if(work[0] == '-')
		    {
			    i = work.find_first_of('=');
			    j = work.find_first_not_of('-');
			    if(i == j || j == string::npos)
                    Exception(Exception::FATAL,"Wrong option [%s]","不正なオプション [%s]").format(work.c_str());

			    if(i != string::npos)
				{
					prevparam = DEFAULT_PARAMETER;
                    addOptionItem(parameter, work.substr(1, i-1), work.substr(i+1));
				}
			    else
				{
					prevparam = work.substr(1);
                    addOptionItem(parameter, prevparam, string(""));
				}
		    }else
                addOptionItem(parameter, prevparam, work);
	    }

		if(argc == 1)
			parameter->addChild("h");

        {       //グローバルなオプションを処理する

				//デバッグ処理
			if(findOption(container, "debug","debug"))
			{
				cout << "\n[debug-info] : Parameter" << endl;
				container.findChild("Parameter")->drawTree(&cout);
			}

				//言語系はエラーメッセージに影響するので一番最初に処理する
            if(findOption(container,"lj","japanese"))
                Message::selectCodepage(0x441);

            if(findOption(container,"le","english"))
                Message::selectCodepage(0);

            if(findOption(container, "debug","debug"))
                container.addChild("Debug");

            if(findOption(container,"h","help"))
                cout << '\n' << Message(
					"Global option\n"
					"  -lj, --japanese         : Use Japanese as display language\n"
					"  -le, --english          : Use English as display language(default)\n"
					"  -v, --verbose           : Display verbose messages\n"
					"  -ext, --extension       : Enable extensions\n"
					"  -no-ext, --no-extension : Restrict use of extensions (Override -ext)\n",
                    "全体のオプション\n"
					"  -lj, --japanese         : 日本語で表示します\n"
					"  -le, --english          : 英語で表示します\n"
                    "  -v,  --verbose          : 途中経過などを表示します\n"
					"  -ext, --extension       : 拡張機能を有効にします\n"
					"  -no-ext, --no-extension : 拡張機能を無効にします\n");

			if(findOption(container,"ext","extension"))
				enable(EXTENSION);

			if(findOption(container,"no-ext","no-extension"))
				disable(EXTENSION);

            if(findOption(container,"v","verbose"))
                VerboseMessage::setVerbose(true);
        }

            //コンポーネントによるパラメータ処理
        scope = componentList.begin();
        while(scope != componentList.end())
        {
//			VerboseMessage("Option parsing [%s]\n","オプション解析 [%s]\n").format((*scope)->getIdentifier());
            (*scope)->parseOption(container);
            scope++;
        }
    }
    catch(Exception * e)
    {
        Exception(Exception::FATAL, "Fatal error occured during command line parsing.\n  %s","コマンドラインオプションの解析中に致命的なエラーが見つかりました\n  %s").format((const char *)*e);
    }
}

void Component::dumpContainerSnapshot(Directory & container, bool forcedly)
{
	string dump;

	if(forcedly || findOption(container, "D", "dump", &dump) || findOption(container,"debug","debug"))
	{
		if(dump.empty())
			dump.assign("container.txt");

		fstream f(dump.c_str(), ios::out);
		if(f.is_open())
		{
			f << "Last configuration was finished successfully\n\n";
			container.drawTree(&f);
			f.close();
		}
	}
}

void Component::executeComponents(Directory & container)
{
    Component * cmp;

    while(!activatedComponentList.empty())
    {
        cmp = *activatedComponentList.begin();
        activatedComponentList.pop_front();
        try{
            cmp->body(container);
        }
		catch(Exception * e)
        {
			dumpContainerSnapshot(container);

            if(e->isFatal())
                throw e;
            cerr << *e << Message("(continue...)","(続行します...)") <<  endl;
        }
    }

	dumpContainerSnapshot(container);
}
