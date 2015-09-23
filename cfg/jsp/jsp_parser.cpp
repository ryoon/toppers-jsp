#include <stdarg.h>

#include "jsp_classes.h"
#include "mpstrstream.h"
#include <set>

using namespace std;

void CoreParser::parseOption(Directory & container)
{
	ParserComponent::parseOption(container);

	if(findOption(container,"h","help"))
	{
        cout << Message(
			"  -obj, --dump-object=filename : Dump the object tree into the file specified\n"
            "  -ao, --assign-order=order : Specify the order of automatic ID assignment\n"
			"    You can use three terms below as ordering rule.\n"
            "     alphabetic (in alphabetic order)\n"
			"     fcfs       (in arrival order [as default])\n"
			"     reverse    (reverse the order)\n",
			"  -obj, --dump-object=ファイル名 : 指定したファイルにオブジェクト情報を出力します\n"
            "  -ao, --assign-order=順序 : 自動ID割当の割当順序を指定します\n"
			"    割当順序は次の3つの組合せで指定します.\n"
            "     alphabetic (ABC順), fcfs (宣言順 [デフォルト]), reverse (逆順)\n");
        return;
	}
}

static int displayHandler(Directory & container, const char * category, const char * format)
{
	Directory * node;
	Directory * scope;

	node = container.findChild(OBJECTTREE,category,NULL);
	if(node == 0 || node->size() == 0)
		return 0;

	VerboseMessage("Handler assignment list [%s]\n","ハンドラ割付表 [%s]\n").format(category);

	scope = node->getFirstChild();
	while(scope != 0)
	{
		VerboseMessage::print(scope->format(format));
		scope = scope->getNext();
	}

	return node->size();
}

int CoreParser::assignID(Directory & container, const char * category)
{
	Directory * node;
	Directory * scope;
	Directory * work;
	set<int> idpool;
	map<int, Directory *> sorter;
	map<int, Directory *>::iterator p_sorter;
	enum tagAssignmentOrder { UNKNOWN, ALPHABETIC, FCFS, REVERSE=0x80, REVERSE_ALPHABETIC, REVERSE_FCFS } order;
	int i;

		//割当パラメータ解析
	i = FCFS;
	node = getOption(container,"ao","assign-order");
	if(node != 0)
	{
		scope = node->getFirstChild();
		while(scope != 0)
		{
			string param = scope->toString();
			if(param.compare("alphabetic") == 0 || param.compare("ALPHABETIC") == 0)
				i = (i & 0xf0) | ALPHABETIC;
			else if(param.compare("fcfs") == 0 || param.compare("FCFS") == 0)
				i = (i & 0xf0) | FCFS;
			else if(param.compare("reverse") == 0 || param.compare("REVERSE") == 0)
				i |= REVERSE;

			scope = scope->getNext();
		}
	}
	order = static_cast<enum tagAssignmentOrder>(i);

		//下準備
	node = container.findChild(OBJECTTREE,category,NULL);
	if(node == 0)
		return 0;

	for(i=1;i< (signed int) node->size() + 32; i++)
		idpool.insert(i);

		//割付順の決定と，割当済みIDの削除
	i = 0;
	scope = node->getFirstChild();
	while(scope != 0)
	{
		if( *scope == Directory::INTEGER )
			idpool.erase(*scope);
		else
		{
			switch(order)
			{
			case ALPHABETIC:
				sorter[i++] = scope;
				break;
			case REVERSE_ALPHABETIC:
				sorter[i--] = scope;
				break;
			case FCFS:
			default:
				sorter[scope->openChild("#order")->toInteger()] = scope;
				break;
			case REVERSE_FCFS:
				sorter[-scope->openChild("#order")->toInteger()] = scope;
				break;
			}
		}
		scope = scope->getNext();
	}

		//ID割当
	p_sorter = sorter.begin();
	while(p_sorter != sorter.end())
	{
		scope = (*p_sorter).second;
		if( !(*scope == Directory::INTEGER) )
		{
			work = container.openChild("/","identifier",scope->getKey().c_str(),NULL);

			if(*work == Directory::INTEGER)
			{
				i = work->toInteger();
				idpool.erase(i);
				VerboseMessage("Assigning the same ID (%d) since the name (%s[%s]) is duplicated\n","ID番号(%d)を異種同名のオブジェクト(%s[%s])に割り当てます．\n").format(i,scope->getKey().c_str(),category);
			}else
			{
				i = *(idpool.begin());
				*work = i;
				idpool.erase(idpool.begin());
			}
			*scope = i;
		}
		p_sorter ++;
	}

		//割当表作成
	if(node->size() != 0 && VerboseMessage::getVerbose())
	{
		VerboseMessage("Object ID assignment list [%s]\n","オブジェクトID割付表 [%s]\n").format(category);

		sorter.clear();
		scope = node->getFirstChild();
		while(scope != 0)
		{
			sorter[scope->toInteger()] = scope;
			scope = scope->getNext();
		}

		p_sorter = sorter.begin();
		while(p_sorter != sorter.end())
		{
			VerboseMessage("  %3d : %s\n","  %3d : %s\n").format((*p_sorter).first, (*p_sorter).second->getKey().c_str());
			p_sorter ++;
		}
	}

		//妥当性の判定
	if((signed)node->size()+1 != *(idpool.begin()))
		Exception("Discontinuous %s ID assignment occured","不連続なオブジェクトID(%s)").format(category);

	return node->size();
}

bool CoreParser::parse(Parser & p, Directory & container)
{
	Token token;

	try{
		do {
			p.getToken(token);
			this->parseStaticAPI(p, container, token);
		}while(token != Token::EOS);

		if(assignID(container, TASK) == 0)
			Exception("Kernel requires one or more task objects.","タスクオブジェクトがありません");

		assignID(container, SEMAPHORE);
		assignID(container, EVENTFLAG);
		assignID(container, DATAQUEUE);
		assignID(container, MAILBOX);
		assignID(container, FIXEDSIZEMEMORYPOOL);
		assignID(container, CYCLICHANDLER);

		displayHandler(container, INTERRUPTHANDLER, "  $@ : $(inthdr)\n");
		displayHandler(container, EXCEPTIONHANDLER, "  $@ : $(exchdr)\n");
		displayHandler(container, INITIALIZER, "  $@ : $(inirtn)($(exinf))\n");
		displayHandler(container, FINALIZER, "  $@ : $(finrtn)($(exinf))\n");

		return true;
	}
	catch(Exception * e)
	{	throw e;	}
}

DECLARE_DIRECTIVE(assigner,"assign_id")
{
	Token name;
	Token value;

	p.getToken(name, Token::IDENTIFIER);
	p.getToken(value, Token::INTEGER);

	if(value.value <= 0)
		Exception("Cannot assign an ID number less or equal to 0.","0以下のID番号を設定することはできません");

	*container.openChild("/","identifier",name.c_str(),NULL) = value.value;
}

DECLARE_DIRECTIVE(parameter,"parameter")
{
	Token token;
	string key;
	string::size_type i,j;

	do {
		p.getToken(token);
		if(!(token == Token::STRINGLITERAL))
			Exception("parameter pragma requires additional parameter put in '\"...\"'","parameterプラグマには\"...\"で括られた追加パラメータが必要です");

		token.chopLiteral();

		if(token[0] == '-')
		{
			i = token.find_first_of('=');
			j = token.find_first_not_of('-');
			if(i == j || j == string::npos)
                Exception(Exception::FATAL,"Wrong option [%s]","不正なオプション [%s]").format(token.c_str());

			if(i != string::npos)
			{
				key = DEFAULT_PARAMETER;
				*container.openChild(string("/Parameter/") + token.substr(1, i-1)) = token.substr(i+1);
			}
			else
			{
				key = token.substr(1);
				*container.openChild(string("/Parameter/") + key) = string("");
			}
		}else
			*container.openChild(string("/Parameter/") + key) = token;

		p.getToken(token);
	} while(token.compare(",") == 0);

	p.putBack(token);
}


//=====================================================================================================

void ConfigurationFileGenerator::parseOption(Directory & parameter)
{
	string work;
	mpstrstream * stream;

	if(findOption(parameter,"h","help"))
	{
        cout << endl << Message(
			"Kernel configuration file generator\n"
            "  -id, --kernel_id=filename  : Specify the name of ID assignment file\n"
            "  -cfg,--kernel_cfg=filename : Specify the name of kernel configuration file\n"
			"  -z, --nonzero              : Do not output ZERO macro and related definitions\n",
			"カーネル構成ファイルの生成\n"
            "  -id, --kernel_id=ファイル名  : ID割当ファイルの名前を指定します\n"
            "  -cfg,--kernel_cfg=ファイル名 : カーネル構成ファイルの名前を指定します\n"
			"  -z, --nonzero                : ZEROマクロの使用を制限します\n");
        return;
	}

	if(!findOption(parameter,"cfg","kernel_cfg",&work))
		work.assign("kernel_cfg.c");

	stream = new mpstrstream(work);
	stream->createpart("header","include",TASK,SEMAPHORE,EVENTFLAG,DATAQUEUE,MAILBOX,FIXEDSIZEMEMORYPOOL,CYCLICHANDLER,INTERRUPTHANDLER,EXCEPTIONHANDLER,INITIALIZER,OBJECT_INITIALIZER,"others",NULL);
	parameter["/file/kernel_cfg"] = stream;

	if(!findOption(parameter,"id","kernel_id",&work))
		work.assign("kernel_id.h");

	stream = new mpstrstream(work);
	stream->createpart("header","body","footer",NULL);
	(*stream)["header"] << "#ifndef __KERNEL_ID_H\n#define __KERNEL_ID_H\n\n";
	(*stream)["footer"] << "\n#endif /* __KERNEL_ID_H */\n";
	parameter["/file/kernel_id"] = stream;

	restrict_zero = findOption(parameter,"z","zero");

	activateComponent();
}

void ConfigurationFileGenerator::createObjectDefinition(mpstrstream * out, Directory & container, int flag, ...)
{
	Directory * scope;
	Directory * id;
	const char * buffer;
	string work;

	map<int, Directory *> sorter;
	map<int, Directory *>::iterator psorter;

	va_list vl;
	int i;

	va_start(vl, flag);
	id = container.openChild("/", "prefix", container.getKey().c_str(), NULL);

	work = string(va_arg(vl, const char *));
	(*id)["id"]  = work;
	for(i=0; i<(signed)work.size(); i++)
		work[i] = work[i] - 'a' + 'A';
	(*id)["ID"]  = work;

	if((flag & SHORT_ID) != 0)
	{
		work = string(va_arg(vl, const char *));
		(*id)["sid"]  = work;
		for(i=0; i<(signed)work.size(); i++)
			work[i] = work[i] - 'a' + 'A';
		(*id)["SID"]  = work;
	}else
	{
		(*id)["sid"] = (*id)["id"].toString();
		(*id)["SID"] = (*id)["ID"].toString();
	}

	work = container.getKey();
	out->moveto(work);

	if((flag & INIT) != 0 && container.size() != 0)
		container["/" OBJECT_INITIALIZER].addChild(new Directory(work));

	(*out) << "\t/* " << Message("Object initializer [%s]","オブジェクト初期化ブロック [%s]").format(work.c_str()) << " */\n\n";

	if((flag & HEADER) != 0)
		(*out) << "#include \"" << work << ".h\"\n\n";

	if((flag & TNUM) != 0)
	{
		(*out) << id->format("#define TNUM_$(ID)ID ") << container.size() << "\n\n";
		(*out) << id->format("const ID tmax_$(id)id = (TMIN_$(ID)ID + TNUM_$(ID)ID - 1);\n\n");
	}

	if((flag & TNUMNO) != 0)
	{
		(*out) << id->format("#define TNUM_$(ID)NO ") << container.size() << "\n\n";
		(*out) << id->format("const UINT tnum_$(id)no = TNUM_$(ID)NO;\n\n");
	}

		/* ID順にソート (この時点で連番が確定しているはずなので，チェックはしない) */
	if((flag & (BUFFER|DEFINITION)) != 0)
	{
		i = 0;
		scope = container.getFirstChild();
		while(scope != 0)
		{
			sorter[scope->toInteger(i++)] = scope;
			scope = scope->getNext();
		}
	}

	if((flag & BUFFER) != 0 && container.size() != 0)
	{
		buffer = va_arg(vl, const char *);
		psorter = sorter.begin();
		while(psorter != sorter.end())
		{
			(*out) << (*psorter).second->format(buffer) << '\n';
			psorter ++;
		}
		(*out) << '\n';
	}

	if((flag & DEFINITION) != 0)
	{
		buffer = va_arg(vl, const char *);
		if(container.size() != 0)
		{
			(*out) << id->format("const $(SID)INIB $(sid)inib_table[TNUM_$(ID)") << ( (flag & TNUMNO) != 0  ? "NO" : "ID") << "] = {\n";
			psorter = sorter.begin();
			while(psorter != sorter.end())
			{
				(*out) << "\t{" << (*psorter).second->format(buffer) << '}';
				psorter ++;
				if(psorter != sorter.end())
					(*out) << ',';
				(*out) << '\n';
			}
			(*out) << "};\n\n";
		}else
			if( !restrict_zero )
				(*out) << id->format("__ZERO(const $(SID)INIB,$(sid)inib_table);\n");
	}

	if((flag & CONTROLBLOCK) != 0)
	{
		if(container.size() != 0)
			(*out) << id->format("$(SID)CB $(sid)cb_table[TNUM_$(ID)ID];\n\n");
		else
			if( !restrict_zero )
				(*out) << id->format("__ZERO($(SID)CB, $(sid)cb_table);\n\n");
	}

}

void ConfigurationFileGenerator::body(Directory & container)
{
	Directory * scope;
	mpstrstream * out;
	string work;

		/* kernel_id.hの出力 */
	out = reinterpret_cast<mpstrstream *>(container("/file/kernel_id")->toPointer());
	out->moveto("body");
	scope = container("/identifier")->getFirstChild();
	while(scope != 0)
	{
		(*out) << "#define " << scope->getKey() << " " << scope->toInteger() << endl;
		scope = scope->getNext();
	}

		/* kernel_cfg.cの出力 */
	out = reinterpret_cast<mpstrstream *>(container("/file/kernel_cfg")->toPointer());
	out->moveto("header");
	(*out) << "/* Configured by [" << container["/Parameter/#cmdline"].toString() << "] */\n\n";
	(*out)	<< "#include \"jsp_kernel.h\"\n#include \"kernel_id.h\"\n\n#if TKERNEL_PRVER >= 0x1011\n  #define CFG_INTHDR_ENTRY(inthdr) INTHDR_ENTRY(inthdr)\n  #define CFG_EXCHDR_ENTRY(exchdr) EXCHDR_ENTRY(exchdr)\n#else\n  #define CFG_INTHDR_ENTRY(inthdr) INTHDR_ENTRY(ENTRY(inthdr), inthdr)\n  #define CFG_EXCHDR_ENTRY(exchdr) EXCHDR_ENTRY(ENTRY(exchdr), exchdr)\n  #define INT_ENTRY(inthdr) ENTRY(inthdr)\n  #define EXC_ENTRY(exchdr) ENTRY(exchdr)\n#endif\n\n";
	if( !restrict_zero )
	{
		if(!findOption(container, "ZERO","ZERO",&work))
			work.assign("x y[0]");
		(*out) << "#ifdef _MSC_VER\n  #define __ZERO(x,y) _declspec(naked) void y(void){}\n#else\n  #define __ZERO(x,y) " << work << "\n#endif\n\n";
	}

		/* INCLUDE */
	out->moveto(INCLUDEFILE);
	scope = container(OBJECTTREE "/" INCLUDEFILE)->getFirstChild();
	(*out) << "\t/* " << Message("User specified include files","ユーザ定義のインクルードファイル") << "*/\n";
	while(scope != 0)
	{
		(*out) << "#include " << scope->toString() << endl;
		scope = scope->getNext();
	}
	(*out) << '\n';

		/* TASK */
	createObjectDefinition(out, container[OBJECTTREE "/" TASK], HEADER|TNUM|SHORT_ID|BUFFER|DEFINITION|CONTROLBLOCK|INIT,"tsk", "t", "static VP __stack_$@[TCOUNT_VP($(stksz))];","$(tskatr), (VP_INT)($(exinf)), $(task), INT_PRIORITY($(itskpri)), TROUND_VP($(stksz)), __stack_$@, $(texatr), $(texrtn)");

		/* SEMAPHORE */
	createObjectDefinition(out, container[OBJECTTREE "/" SEMAPHORE], HEADER|TNUM|DEFINITION|CONTROLBLOCK|INIT,"sem", "$(sematr), $(isemcnt), $(maxsem)");

		/* EVENTFLAG */
	createObjectDefinition(out, container[OBJECTTREE "/" EVENTFLAG], HEADER|TNUM|DEFINITION|CONTROLBLOCK|INIT,"flg", "$(flgatr), $(iflgptn)");

		/* DATAQUEUE */
	createObjectDefinition(out, container[OBJECTTREE "/" DATAQUEUE], HEADER|TNUM|BUFFER|DEFINITION|CONTROLBLOCK|INIT,"dtq", "#if ($(dtqcnt)) > 0\n  static VP_INT __dataqueue_$@[$(dtqcnt)];\n#else\n  #define __dataqueue_$@ NULL\n#endif","$(dtqatr), $(dtqcnt), __dataqueue_$@");

		/* MAILBOX */
	createObjectDefinition(out, container[OBJECTTREE "/" MAILBOX], HEADER|TNUM|DEFINITION|CONTROLBLOCK|INIT,"mbx","$(mbxatr), $(mbxpri)");

		/* FIXEDSIZEMEMORYPOOL */
	createObjectDefinition(out, container[OBJECTTREE "/" FIXEDSIZEMEMORYPOOL], HEADER|TNUM|BUFFER|DEFINITION|CONTROLBLOCK|INIT,"mpf","static VP __fixedsize_memorypool_$@[TCOUNT_VP($(blksz)) * (($(blkcnt)))];","$(mpfatr), TROUND_VP($(blksz)), __fixedsize_memorypool_$@, (VP)(((VB *)__fixedsize_memorypool_$@ + sizeof(__fixedsize_memorypool_$@))");

		/* CYCLICHANDLER */
	createObjectDefinition(out, container[OBJECTTREE "/" CYCLICHANDLER], HEADER|TNUM|DEFINITION|CONTROLBLOCK|INIT,"cyc","$(cycatr),$(exinf),$(cychdr),$(cyctim),$(cycphs)");

		/* INTERRUPTHANDLER */
	createObjectDefinition(out, container[OBJECTTREE "/" INTERRUPTHANDLER], HEADER|BUFFER|TNUMNO|DEFINITION|INIT,"inh","CFG_INTHDR_ENTRY($(inthdr));","$@,$(inhatr),INT_ENTRY($(inthdr))");

		/* EXCEPTIONHANDLER */
	createObjectDefinition(out, container[OBJECTTREE "/" EXCEPTIONHANDLER], HEADER|BUFFER|TNUMNO|DEFINITION|INIT,"exc","CFG_EXCHDR_ENTRY($(exchdr));","$@,$(excatr),EXC_ENTRY($(exchdr))");


		/* オブジェクト初期化ルーチン */
	out->moveto(OBJECT_INITIALIZER);
	(*out) << "\t/* " << Message("Object initialization routine","オブジェクト初期化ルーチン") << " */\n\n";
	(*out) << "void\nobject_initialize(void)\n{\n";
	scope = container["/" OBJECT_INITIALIZER].getFirstChild();
	while(scope != 0)
	{
		(*out) << '\t' << scope->toString() << "_initialize();\n";
		scope = scope->getNext();
	}
	(*out) << "}\n\n";

		/* 初期化ハンドラ */
	out->moveto(INITIALIZER);
	(*out) << "\t/* " << Message("Initialization handler","初期化ハンドラ起動ルーチン") << " */\n\n";
	(*out) << "void\ncall_inirtn(void)\n{\n";
	scope = container[OBJECTTREE "/" INITIALIZER].getFirstChild();
	while(scope != 0)
	{
		(*out) << scope->format("\t$(inirtn)( (VP_INT)($(exinf)) );\n");
		scope = scope->getNext();
	}
	(*out) << "}\n\n";

	(*out) << "void\ncall_finrtn(void)\n{\n";
	scope = container[OBJECTTREE "/"  FINALIZER].getLastChild();
	while(scope != 0)
	{
		(*out) << scope->format("\t$(finrtn)( (VP_INT)($(exinf)) );\n");
		scope = scope->getPrev();
	}
	(*out) << "}\n\n";

	out->moveto("others");
	(*out) << "#include \"time_event.h\"\n\nTMEVTN	tmevt_heap[TNUM_TSKID + TNUM_CYCID];\n\n";

	if(findOption(container,"obj","dump-object",&work))
	{
		if(work.empty())
			work.assign("kernel_obj.dat");

		fstream f(work.c_str(),ios::out|ios::binary);
		if(f.is_open())
		{
			container[OBJECTTREE].Store(&f);
			f.close();
		}else
			Exception(" Failed to open the file %s for storing object definitions"," オブジェクト情報を格納するためのファイル(%s)が開けません").format(work.c_str());
	}
}

