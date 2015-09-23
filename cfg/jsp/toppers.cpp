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
 *  @(#) $Id: toppers.cpp,v 1.15 2001/11/15 05:06:56 takayuki Exp $
 */

/*
 * このファイルは過渡的なものであり，次のバージョンでは削除される可能性がある
 *  (特にプロテクションドメインに関する部分)
 */

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#define MAX_IDENTIFIERLENGTH 128

#include "../message.h"
#include "toppers.h"
#include <string>
#include <stdio.h>

using namespace std;

ToppersManager InstanceOfToppersManager;

void make_uppercase(char * dest, const char * src)
{
	while(*src != '\x0')
		if(*src >= 'a' && *src <= 'z')
			*(dest++) = *(src++) - 'a' + 'A';
		else
			*(dest++) = *(src++);
	*dest = '\x0';
}

/*
 * BaseObject - ITRONオブジェクトに共通な部分
 */

bool BaseObject::LoadParameter(int id, Array * src, const char * parameter, const char ** position)
{
	unsigned int pos;
	Array * dest;
	int  store_position;
	bool allow_string;
	bool allow_integer;
	Value * value;

	if(position == NULL && (signed)Parameter.size() < id)
	{
		Parameter.resize(id);
	}
	dest = static_cast<Array *>(Parameter[id-1]);

	allow_string = false;
	allow_integer = false;
	store_position = -1;
	for(pos = 0;*parameter != '\x0';parameter++)
	{
		if(*parameter >= '0' && *parameter <='9')
		{
			if(store_position != -1)
				store_position = store_position * 10 + *parameter - '0';
			else
				store_position = *parameter - '0';
			continue;
		}

		switch(*parameter)
		{
		case 's': allow_string = true;  break;
		case 'i': allow_integer = true; break;
		case '{':
			value = (*src)[pos];
			if( *value != Value::ARRAY )
				throw Exception(Message("Illegal parameter value","不正なパラメータ値"));
			LoadParameter(id, static_cast<Array *>(value), parameter+1, &parameter);
			if(*(parameter+1) == ';')
				return true;
			break;
		case ',':
		case ';':
		case '}':
			if(pos >= (*src).size())
				throw Exception(Message("Illegal parameter value","不正なパラメータ値"));

			value = (*src)[pos];
			if(allow_string || allow_integer)
				if( !(allow_string && *value == Value::STRING) && !(allow_integer && *value == Value::INTEGER))
					throw Exception(Message("Illegal parameter value","不正なパラメータ値"));

			if(store_position != -1)
				(*dest)[store_position] = value->Duplicate();

			if(*parameter == '}')
			{
				if(position != NULL)
					*position = parameter;
				return true;
			}

			allow_integer = false;
			allow_string = false;
			store_position = -1;
			pos ++;
			break;
		}
	}

	return true;
}

void BaseObject::SetParameter(int id, int pos, Value * src)
{
	if(id <= (signed)Parameter.size())
		(*static_cast<Array *>(Parameter[id-1]))[pos] = src;
}

Value * BaseObject::GetParameter(int id, int pos)
{
	if(id <= (signed)Parameter.size())
		return (*static_cast<Array *>(Parameter[id-1]))[pos];
	return 0;
}

int BaseObject::AssignNewObjectID(Manager * mgr, int params)
{
	int id = Identifier.GetNewID();

	Identifier.Assign(id);

	Parameter.resize(id);
	Array * work = new Array;
	work->resize(params);
	Parameter[id-1] = work;

	ToppersDomain * domain;
	domain = static_cast<ToppersDomain *>(mgr->CurrentDomain);
	domain->Object[this].insert(id);

	return id;
}

int BaseObject::AssignObjectID(Manager * mgr, Value * val, int params)
{
	int id;
	Message AlreadyAssigned("Specified ID is already assigned","そのIDは使われている");
	Message NotSuitableID("Specified ID is not suitable ID","不正なID値");

	if(val == 0)
		throw Exception(NotSuitableID);
	
	switch(static_cast<Value::tagType>(*val))
	{
	case Value::STRING:
		if(ObjectID.find(*static_cast<String *>(val)) != ObjectID.end())
			throw Exception(AlreadyAssigned);
		
		id = Identifier.GetNewID();
		ObjectID[*static_cast<String *>(val)] = id;
		break;
	case Value::INTEGER:
		id = (const int)*(Integer *)(val);
		break;
	default:
		throw Exception(NotSuitableID);
	}

	if(Identifier.IsAssigned(id))
		throw Exception(AlreadyAssigned);

	Identifier.Assign(id);

	Parameter.resize(id);
	Array * work = new Array;
	work->resize(params);
	Parameter[id-1] = work;

	ToppersDomain * domain;
	domain = static_cast<ToppersDomain *>(mgr->CurrentDomain);
	domain->Object[this].insert(id);

	return id;
}

int BaseObject::GetObjectID(Value * val)
{
	int id;
	Message NoObject("No object found","指定されたオブジェクトは存在しない");
	Message IllegalID("Illegal Identifier","不正なオブジェクトIDが指定された");

	if(val == 0)
		throw Exception(IllegalID);
	
	switch(static_cast<Value::tagType>(*val))
	{
	case Value::STRING:
		{
			map<string,int>::iterator scope;
			scope = ObjectID.find(*static_cast<String*>(val));
			if(scope == ObjectID.end())
				throw Exception(NoObject);
			id = (*scope).second;
			break;
		}
	case Value::INTEGER:
		id = (const int)*(Integer *)(val);
		break;
	default:
		throw Exception(IllegalID);
	}

	if(!Identifier.IsAssigned(id))
		throw Exception(NoObject);

	return id;
}

void BaseObject::Serialization(Manager * mgr, MultiStream & dest, const char * short_name, const char * long_name, const char * symbol)
{
		//制御ブロックはドメイン関係なしなので、ドンドン吐くだけ

	int i;
	char uc_long_name[MAX_IDENTIFIERLENGTH];
	char uc_short_name[MAX_IDENTIFIERLENGTH];
	char uc_symbol[MAX_IDENTIFIERLENGTH];
	map<string,int>::iterator object;


    if(!Identifier.IsAvailable())
		throw Exception(string(Message("Object ID must not include any crack (","オブジェクトIDが連続していません (")) + string(long_name) + ")");
    
	if(symbol == NULL)
		symbol = short_name;

	make_uppercase(uc_short_name, short_name);
	make_uppercase(uc_symbol, symbol);
	make_uppercase(uc_long_name, long_name);

		//共通部分
	dest[CFGFILE];
	(*dest) << "\n/* " << long_name << " */\n";
	(*dest) << "#include \"" << long_name << ".h\"" << endl << endl;
	(*dest) << "#define TNUM_" << uc_short_name << "ID " << Identifier.GetCount() << endl << endl;
	(*dest) << "const ID tmax_" << short_name << "id = (TMIN_" << uc_short_name << "ID + TNUM_" << uc_short_name << "ID - 1);" << endl << endl;

	if(Identifier.GetCount() != 0)
	{		
			// オブジェクト初期化関数へ登録
		static_cast<ToppersManager *>(mgr)->Initializer.push_back(string(long_name));

			//制御テーブル作成
		(*dest) << "const " << uc_symbol << "INIB " << symbol << "inib_table[TNUM_" << uc_short_name << "ID] = {" << endl;

		for(i=0;i<Identifier.GetMaxID();i++)
		{
			(*dest) << "\t";
			if(i != 0)
				(*dest) << ',';
			(*dest) << Parameter[i] << endl;
		}

		(*dest) << "};" << endl << endl << uc_symbol << "CB " << symbol << "cb_table[TNUM_" << uc_short_name << "ID];" << endl << endl;

			// オブジェクト名 定義
		dest[HEADER];
		object = ObjectID.begin();
		while(object != ObjectID.end())
		{
			(*dest) << "#define " << (*object).first << " " << (*object).second << endl;
			object ++;
		}
		(*dest) << endl;

		if((*mgr)[CHECKERFILE])
		{
				// オブジェクト名 定義
			dest[CHECKER];
			object = ObjectID.begin();
			while(object != ObjectID.end())
			{
				(*dest) << "\tOBJECT(" << uc_long_name << "_" << (*object).second << "," <<(*object).first << ");\n";
				object ++;
			}
			(*dest) << endl;
		}

			// ORTIテンプレート部分
		if((*mgr)[ORTISUPPORT])
		{
			dest[ORTIFILE];
			object = ObjectID.begin();
			while(object != ObjectID.end())
			{
				(*dest) << "TEMPLATE T" << uc_long_name << "\t";
				(*dest) << (*object).first << "(\"" << (*object).second-1 << "\");" << endl;
				object ++;
			}
		}
	}else
	{
			//からっぽ
		(*dest) << "__ZERO(" << uc_symbol << "CB, " << symbol << "cb_table);" << endl;
		(*dest) << "__ZERO(const " << uc_symbol << "INIB, " << symbol << "inib_table);" << endl << endl;
	}
}

void BaseObject::CreateBuffer(Manager * mgr, MultiStream & dest, const char * prefix, int pos, int size, const char * type = "static char", const char * replace = 0)
{
		//バッファはドメイン単位で生成する

	typedef set<int> set_int;
	map<BaseObject *, set_int>::iterator container;
	set_int::iterator scope;
	ToppersDomain * domain;
	Array * param;
	Value * val;
	char work[MAX_IDENTIFIERLENGTH];
	char buffer[1024];
	int offset;

	domain = static_cast<ToppersDomain *>(static_cast<ToppersManager *>(mgr)->CurrentDomain);
	container = domain->Object.find(this);
	if(container == domain->Object.end())
		return;

	for(offset=0;*(prefix + offset) != '\x0'; offset ++)
		work[offset] = *(prefix + offset);

	scope = (*container).second.begin();
	while(scope != (*container).second.end())
	{
		if(*(scope) > (signed)Parameter.size())
			throw Exception(Message("Object not found","オブジェクトはありません"));

		sprintf(work+offset,"%d",*(scope));

		param = static_cast<Array *>(Parameter[*(scope)-1]);
		val = (*param)[pos];
		if(val != NULL)
		{
			if(*val != Value::STRING || static_cast<String *>(val)->compare("NULL") != 0)
				throw Exception(Message("The parameter treated as a pointer to buffer must be NULL","バッファへのポインタはNULLでなければいけません"));
				// (*dest) << "extern ";
			delete val;
		}
		(*param)[pos] = new String(work);

		val = (*param)[size];

		sprintf(buffer, type, work, ((String *)val)->c_str());
		(*dest) << buffer << endl;

		if(replace != 0)
		{
			sprintf(buffer, replace, ((String *)val)->c_str());
			((String *)val)->assign(buffer);
		}

		scope ++;
	}
}

/*
 * 静的APIの宣言
 */

void Task::API_CRE_TSK::Body(Manager * mgr, Array * api)
{
	int id;
	string work;
    char buffer[256];
    Value * val;

	id = Parent->AssignObjectID(mgr, (*api)[0], 8);
	Parent->LoadParameter(id, api,",{si0,si1,si2,si3,si4,si5};");

    val = Parent->GetParameter(id, STACKSIZE);
    if(*val == Value::INTEGER)
    {
		sprintf(buffer, "%d", (int)*(Integer *)val);
		Parent->SetParameter(id, STACKSIZE, new String(buffer));
	}

    val = Parent->GetParameter(id, EXINF);
    if(*val == Value::INTEGER)
    {
		sprintf(buffer, "(VP_INT)(%d)", (int)*(Integer *)val);
		Parent->SetParameter(id, EXINF, new String(buffer));
	}else
    {
		work = string("(VP_INT)(") + *(String *)val + ")";
		Parent->SetParameter(id, EXINF, new String(work));
	}

	work = string("INT_PRIORITY(") + *(String *)(Parent->GetParameter(id, PRIORITY)) + ")";
	Parent->SetParameter(id, PRIORITY, new String(work));

	Parent->SetParameter(id, TEXTYPE,  new String("TA_NULL"));
	Parent->SetParameter(id, TEXFUNC,  new String("NULL"));
}

void Task::API_DEF_TEX::Body(Manager * mgr, Array * api)
{
	int id;
	id = Parent->GetObjectID((*api)[0]);
	Parent->LoadParameter(id,api,",{si6,si7}");
}

void Semaphore::API_CRE_SEM::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignObjectID(mgr, (*api)[0], 3);
	Parent->LoadParameter(id, api, ",{si0,si1,si2}");
}

void EventFlag::API_CRE_FLG::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignObjectID(mgr, (*api)[0], 2);
	Parent->LoadParameter(id, api, ",{si0,si1}");
}

void DataQueue::API_CRE_DTQ::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignObjectID(mgr, (*api)[0], 3);
	Parent->LoadParameter(id, api, ",{si0,si1,si2}");

    Value * val = Parent->GetParameter(id, 1);
    if(*val == Value::INTEGER)
    {
		char buffer[32];
		sprintf(buffer, "%d", (int)*(Integer *)val);
		Parent->SetParameter(id, 1, new String(buffer));
	}
}

void Mailbox::API_CRE_MBX::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignObjectID(mgr, (*api)[0], 2);
	Parent->LoadParameter(id, api, ",{si0,si1,}");

	Array * parameter = (Array *)(*api)[1];
	if(*(*parameter)[2] != Value::STRING || ( ((String *)(*parameter)[2])->compare("NULL") != 0) )
		throw Exception(Message("Third parameter of CRE_MBX must be NULL","CRE_MBXの第3パラメータはNULLでなければいけません"));
}

void FixedsizeMemoryPool::API_CRE_MPF::Body(Manager * mgr, Array * api)
{
	string work;
	Value * val;
	char buffer[256];

	int id = Parent->AssignObjectID(mgr, (*api)[0], 4);
	Parent->LoadParameter(id, api, ",{si0,si1,si2,si3}");

	val = Parent->GetParameter(id, 2);
	if(*val == Value::INTEGER)
		sprintf(buffer, "TCOUNT_VP(%d)", (int)(*(Integer *)val));
	else
		sprintf(buffer, "TCOUNT_VP(%s)", ((String *)val)->c_str());

	work.assign(buffer);

	val = Parent->GetParameter(id, 1);
	if(*val == Value::INTEGER)
		sprintf(buffer, " * (%d)", (int)(*(Integer *)val));
	else
		sprintf(buffer, " * (%s)", ((String *)val)->c_str());

	work += buffer;

	Parent->SetParameter(id, 1, new String(work));

}

void CyclicHandler::API_CRE_CYC::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignObjectID(mgr, (*api)[0], 5);
	Parent->LoadParameter(id, api, ",{si0,si1,si2,si3,si4}");
}

void InterruptHandler::API_DEF_INH::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignNewObjectID(mgr, 3);
	Parent->LoadParameter(id, api, "si0,{si1,s2}");
}


void FixedsizeMemoryPool::Serialization(Manager * mgr, MultiStream & dest, const char * short_name, const char * long_name, const char * symbol)
{
	Array * param;
	string work;
	char buffer[MAX_IDENTIFIERLENGTH + 32];

	for(unsigned int i=1;i<=Parameter.size();i++)
	{
		param = static_cast<Array *>(Parameter[i-1]);

		if( *(*param)[2] == Value::INTEGER )
			sprintf(buffer, "TROUND_VP(%d)", (int)*(Integer *)(*param)[2]);
		else
			sprintf(buffer, "TROUND_VP(%s)", ((String *)(*param)[2])->c_str());

		SetParameter(i, 1, new String(buffer));

		work = *(String *)(*param)[3];
		SetParameter(i, 2, new String(work));
		work = string("(VP)(((VB *)") + work + string(") + sizeof(") + work + "))";
		SetParameter(i, 3, new String(work));
	}

	BaseObject::Serialization(mgr, dest, short_name, long_name, symbol);
}



void InterruptHandler::Serialization(Manager * mgr, MultiStream & dest)
{
	unsigned int i;
	Array * param;

	dest[CFGFILE];
	
	(*dest) << "/*\n * Interrupt Handler Initialization Block\n */\n\n";
	(*dest) << "#include \"interrupt.h\"\n\n";
	(*dest) << "#define TNUM_INHNO " << Parameter.size() << endl;
	(*dest) << "const INHNO tnum_inhno = TNUM_INHNO;\n\n";

	if(Parameter.size() != 0)
	{
		static_cast<ToppersManager *>(mgr)->Initializer.push_back(string("interrupt"));

		for(i=0;i<Parameter.size();i++)
		{
			param = static_cast<Array *>(Parameter[i]);
			(*dest) << "CFG_INTHDR_ENTRY(" << (*param)[2] << ");\n";
		}

		(*dest) << "\nconst INHINIB inhinib_table[TNUM_INHNO] = {\n";

		for(i=0;i<Parameter.size();i++)
		{
			param = static_cast<Array *>(Parameter[i]);
			(*dest) << "\t";
			if(i != 0)
				(*dest) << ",";
			(*dest) << "{" << (*param)[0] << "," << (*param)[1] << ",INT_ENTRY(" << (*param)[2] << ")}\n";
		}

		(*dest) << "};\n";
	}else
		(*dest) << "__ZERO(const INHINIB, inhinib_table);\n";

	(*dest) << "\n";
}

void ExceptionHandler::API_DEF_EXC::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignNewObjectID(mgr, 3);
	Parent->LoadParameter(id, api, "si0,{si1,s2}");
}

void ExceptionHandler::Serialization(Manager * mgr, MultiStream & dest)
{
	unsigned int i;
	Array * param;

	dest[CFGFILE];

	(*dest) << "/*\n * Exception Handler Initialization Block\n */\n\n";
	(*dest) << "#include \"exception.h\"\n\n";
	(*dest) << "#define TNUM_EXCNO " << Parameter.size() << endl;
	(*dest) << "const EXCNO tnum_excno = TNUM_EXCNO;\n\n";

	if(!Parameter.empty())
	{
		static_cast<ToppersManager *>(mgr)->Initializer.push_back(string("exception"));

		for(i=0;i<Parameter.size();i++)
		{
			param = static_cast<Array *>(Parameter[i]);
			(*dest) << "CFG_EXCHDR_ENTRY(" << (*param)[2] << ");\n";
		}

		(*dest) << "\nconst EXCINIB excinib_table[TNUM_EXCNO] = {\n";

		for(i=0;i<Parameter.size();i++)
		{
			param = static_cast<Array *>(Parameter[i]);
			(*dest) << "\t";
			if(i != 0)
				(*dest) << ",";
			(*dest) << "{" << (*param)[0] << "," << (*param)[1] << ",EXC_ENTRY(" << (*param)[2] << ")}\n";
		}
		(*dest) << "};\n";
	}else
		(*dest) << "__ZERO(const EXCINIB, excinib_table);\n";

	(*dest) << "\n";
}

void InitializeHandler::API_ATT_INI::Body(Manager * mgr, Array * api)
{
	int id = Parent->AssignNewObjectID(mgr, 3);
	Parent->LoadParameter(id, api, "{si,si0,si1}");
}

void InitializeHandler::Serialization(Manager * _mgr, MultiStream & dest)
{
		//現時点ではドメインはサポートしないので吐きまくる
	unsigned int i;
	Array * param;

	dest[CFGFILE];

	(*dest) << "\n/*\n * Initialization Handler Block\n */\n\n";
	(*dest) << "void\ncall_inirtn()\n{\n";

	for(i=0;i<Parameter.size();i++)
	{
		param = static_cast<Array *>(Parameter[i]);

		(*dest) << "\t" << (*param)[1] << "((VP_INT)(" << (*param)[0] << "));\n";
	}

	(*dest) << "}\n";
}

/*
 *  TOPPERS/JSP Configuration Manager
 */

ToppersManager::ToppersManager(void)
{
	filename.erase();
}

bool ToppersManager::ParseOption(int argc, char * argv [])
{
	int i;
	string work;
	Message UnknownOption("Unknown Option [","不明なオプション [");

	for(i=1;i<argc;i++)
	{
		if(*argv[i] == '-')
		{
			switch(*(argv[i]+1))
			{
			case 'o': SetOption(ORTISUPPORT); break;
			case 'd': SetOption(DOMAINSUPPORT); break;
			case 'D': SetOption(FORCEDOMAINSUPPORT); break;
			case 'v': SetOption(VERBOSE); break;
			case 'c': SetOption(CHECKERFILE); break;

			case 'l':
				if(*(argv[i]+2) == 'j')
					Message::SelectCodePage(0x411);
				else
					Message::SelectCodePage();
				break;

			case '-':
				work = argv[i]+2;

				if(work.compare("odl")==0)
				{
					SetOption(ORTISUPPORT);
					break;
				}

				if(work.compare("checkerfile")==0)
				{
					SetOption(CHECKERFILE);
					break;
				}

				if(work.compare("domain")==0)
				{
					SetOption(DOMAINSUPPORT);
					break;
				}

				throw Exception(string(UnknownOption) + argv[i] + "]");

			default:
				break;
			}
		}else
		{
			if(filename.empty())
				filename = argv[i];
			else
				throw Exception(string(UnknownOption) + argv[i] + "]");
		}
	}

	return true;
}

void ToppersManager::Body(void)
{
	*this << &INCLUDE;
	*this << &task.CRE_TSK;
	*this << &task.DEF_TEX;
	*this << &semaphore.CRE_SEM;
	*this << &eventflag.CRE_FLG;
	*this << &dataqueue.CRE_DTQ;
	*this << &mailbox.CRE_MBX;
	*this << &fmempool.CRE_MPF;
	*this << &cyclic.CRE_CYC;
	*this << &inthdr.DEF_INH;
	*this << &exchdr.DEF_EXC;
	*this << &inihdr.ATT_INI;

	
	ToppersDomain * dom = new ToppersDomain;
	*this << this;
	*this << dom;

	ParseBody(dom, filename.c_str());
	SerializeBody();
}

void ToppersManager::Serialization(Manager * mgr, MultiStream & dest)
{
	static int step = 0;

	switch(step++)
	{
	case 0:	//しょっぱな
		{
			dest[CFGFILE];
			list<string>::iterator file;

			(*dest) <<
				"#include \"jsp_kernel.h\"\n"
				"#include \"kernel_id.h\"\n\n";

			
			file = IncludeFile.begin();
			while(file != IncludeFile.end())
			{
				(*dest) << "#include " << (*file) << endl;
				file ++;
			}

			(*dest) << endl <<
				"#ifdef _MSC_VER\n  #define __ZERO(x,y) _declspec(naked) void y(void){}\n"
				"#else\n"
				"  #define __ZERO(x,y) x y[0]\n#endif\n\n";

			(*dest) << 
				"#if TKERNEL_PRVER >= 0x1011\n"
				"  #define CFG_INTHDR_ENTRY(inthdr) INTHDR_ENTRY(inthdr)\n"
				"  #define CFG_EXCHDR_ENTRY(exchdr) EXCHDR_ENTRY(exchdr)\n"
				"#else\n"
				"  #define CFG_INTHDR_ENTRY(inthdr) INTHDR_ENTRY(ENTRY(inthdr),inthdr)\n"
				"  #define CFG_EXCHDR_ENTRY(exchdr) EXCHDR_ENTRY(ENTRY(exchdr),exchdr)\n"
				"  #define INT_ENTRY(inthdr) ENTRY(inthdr)\n"
				"  #define EXC_ENTRY(exchdr) ENTRY(exchdr)\n"
				"#endif\n\n";

			if((*this)[CHECKERFILE])
				(*dest[CHECKER]) <<
					"#include \"jsp_kernel.h\"\n"
					"#include \"logtask.h\"\n"
					"#include \"timer.h\"\n\n"
					"#define OBJECT(x,y) __asm(\"d\" #x \",\" #y \"@\");\n"
					"#define MEMBER(x,y) __asm(\"s\" #x \"::\" #y \",(%0),(%1)@\" :: \"i\"(sizeof(((struct x *)0)->y)), \"i\"(&((struct x *)0)->y));\n"
					"#define VAR(x) __asm(\"s\" #x \",(%0),(0)@\" :: \"i\"(sizeof(x)));\n"
					"#define EVAR(x,y) __asm(\"s\" #y \",(%0),(0)@\" :: \"i\"(sizeof(x)));\n"
					"#define SVAR(x) __asm(\"s\" #x \",(%0),(0)@\" :: \"i\"(sizeof(x[0])));\n"
					"#define DEFS(x) __asm(\"s\" #x \",(%0),(0)@\" :: \"i\"((unsigned long)x));\n\n"
					"#include \"queue.h\"\n\n"
					"#include \"task.h\"\n"
					"#include \"semaphore.h\"\n"
					"#include \"eventflag.h\"\n"
					"#include \"dataqueue.h\"\n"
					"#include \"mailbox.h\"\n"
					"#include \"mempfix.h\"\n"
					"#include \"cyclic.h\"\n"
					"#include \"../kernel/exception.h\"\n"
					"#include \"interrupt.h\"\n"
					"#include \"wait.h\"\n\n"
					"void checker_function(void)\n{\n";

				//最後にもっかい呼び出す
			*this << this;
			break;
		}

	case 1:	//トリ
		dest[CFGFILE];
		(*dest) << endl << "/*\n * System Domain\n */" << endl;
		task.Serialization(mgr, dest, "tsk","task","t");
		semaphore.Serialization(mgr, dest, "sem","semaphore");
		eventflag.Serialization(mgr, dest, "flg","eventflag");
		dataqueue.Serialization(mgr, dest, "dtq","dataqueue");
		mailbox.Serialization(mgr, dest, "mbx","mailbox");
		fmempool.Serialization(mgr, dest, "mpf","mempfix");
		cyclic.Serialization(mgr, dest, "cyc","cyclic");
		inthdr.Serialization(mgr, dest);
		exchdr.Serialization(mgr, dest);
		inihdr.Serialization(mgr, dest);

		if((*this)[CHECKERFILE])
			(*dest[CHECKER]) << 		
				"\tDEFS(TMAX_TPRI);\n"
				"\tDEFS(TMIN_TPRI);\n\n"
				"\tEVAR(ID,_kernel_tmax_tskid);\n"
				"\tEVAR(TINIB,_kernel_tinib_table);\n"
				"\tMEMBER(task_initialization_block,tskatr);\n"
				"\tMEMBER(task_initialization_block,exinf);\n"
				"\tMEMBER(task_initialization_block,task);\n"
				"\tMEMBER(task_initialization_block,ipriority);\n"
				"\tMEMBER(task_initialization_block,stksz);\n"
				"\tMEMBER(task_initialization_block,stk);\n"
				"\tMEMBER(task_initialization_block,texatr);\n"
				"\tMEMBER(task_initialization_block,texrtn);\n\n"
				"\tEVAR(ID,_kernel_tmax_cycid);\n"
				"\tEVAR(CYCINIB, _kernel_cycinib_table);\n"
				"\tMEMBER(cyclic_handler_initialization_block,cycatr);\n"
				"\tMEMBER(cyclic_handler_initialization_block,exinf);\n"
				"\tMEMBER(cyclic_handler_initialization_block,cychdr);\n"
				"\tMEMBER(cyclic_handler_initialization_block,cyctim);\n"
				"\tMEMBER(cyclic_handler_initialization_block,cycphs);\n\n"
				"\tEVAR(ID,_kernel_tmax_dtqid);\n"
				"\tEVAR(SEMINIB,_kernel_dtqinib_table);\n"
				"\tMEMBER(dataqueue_initialization_block,dtqatr);\n"
				"\tMEMBER(dataqueue_initialization_block,dtqcnt);\n"
				"\tMEMBER(dataqueue_initialization_block,dtq);\n\n"
				"\tEVAR(ID,_kernel_tmax_flgid);\n"
				"\tEVAR(FLGINIB,_kernel_flginib_table);\n"
				"\tMEMBER(eventflag_initialization_block,flgatr);\n"
				"\tMEMBER(eventflag_initialization_block,iflgptn);\n\n"
				"\tEVAR(ID, _kernel_tnum_inhno);\n"
				"\tEVAR(INHINIB,_kernel_inhinib_table);\n"
				"\tMEMBER(cpu_exception_handler_initialization_block,excno);\n"
				"\tMEMBER(cpu_exception_handler_initialization_block,excatr);\n"
				"\tMEMBER(cpu_exception_handler_initialization_block,exchdr);\n\n"
				"\tEVAR(ID, _kernel_tnum_excno);\n"
				"\tEVAR(EXCINIB,_kernel_excinib_table);\n"
				"\tMEMBER(interrupt_handler_initialization_block,inhno);\n"
				"\tMEMBER(interrupt_handler_initialization_block,inhatr);\n"
				"\tMEMBER(interrupt_handler_initialization_block,inthdr);\n\n"
				"\tEVAR(ID,_kernel_tmax_mbxid);\n"
				"\tEVAR(MBXINIB,_kernel_mbxinib_table);\n"
				"\tMEMBER(mailbox_initialization_block,mbxatr);\n"
				"\tMEMBER(mailbox_initialization_block,maxmpri);\n\n"
				"\tEVAR(ID,_kernel_tmax_mpfid);\n"
				"\tEVAR(MPFINIB,_kernel_mpfinib_table);\n"
				"\tMEMBER(fixed_memorypool_initialization_block,mpfatr);\n"
				"\tMEMBER(fixed_memorypool_initialization_block,blksz);\n"
				"\tMEMBER(fixed_memorypool_initialization_block,mpf);\n"
				"\tMEMBER(fixed_memorypool_initialization_block,limit);\n\n"
				"\tMEMBER(queue,next);\n"
				"\tMEMBER(queue,prev);\n\n"
				"\tEVAR(ID,_kernel_tmax_semid);\n"
				"\tEVAR(SEMINIB,_kernel_seminib_table);\n"
				"\tMEMBER(semaphore_initialization_block,sematr);\n"
				"\tMEMBER(semaphore_initialization_block,isemcnt);\n"
				"\tMEMBER(semaphore_initialization_block,maxsem);\n\n"
				"}\n";

		list<string>::iterator scope;
		
		(*dest[CFGFILE]) <<
			"\n#include \"time_event.h\"\n"
			"TMEVTN	tmevt_heap[TNUM_TSKID + TNUM_CYCID];\n"
			"\n/*\n * Kernel Object Initialization Handler\n */\n\n"
			"void\nobject_initialize()\n{\n";

		scope = Initializer.begin();
		while(scope != Initializer.end())
		{
			(*dest) << "\t" << (*scope) << "_initialize();\n";
			scope ++;
		}
		(*dest) << "}\n";



		break;
	}
}

void ToppersManager::API_INCLUDE::Body(Manager * mgr, Array * param)
{
	Message msg("INCLUDE takes only one string parameter","INCLUDEのパラメータはファイル名ひとつだけです");
	if(param->size() != 1 || (enum Value::tagType)*(*param)[0] != Value::STRING)
		throw Exception(msg);

	static_cast<ToppersManager *>(mgr)->IncludeFile.push_back(*static_cast<String *>((*param)[0]));
}



/*
 *  TOPPERS/JSP Protection Domain 
 */

ProtectionDomain * ToppersDomain::CreateNewDomain(Manager * mgr)
{
	ToppersDomain * work = new ToppersDomain;
	*mgr << static_cast<Serializer *>(work);
	return static_cast<ProtectionDomain *>(work);
}

void ToppersDomain::Parse(Manager * mgr, Parser * p)
{
	ProtectionDomain * old_dom = mgr->CurrentDomain;

	mgr->CurrentDomain = this;

	if((*mgr)[DOMAINSUPPORT])
	{
		ProtectionDomain::Parse(mgr,p);
	}else
	{
		Token token;

		Name = "system_domain";
		Type = ProtectionDomain::SYSTEM;

		while(p->GetToken(token) == Token::IDENTIFIER)
		{
			p->PutBack(token);
			mgr->API.Parse(mgr,p);
		}
	}
	mgr->CurrentDomain = old_dom;

}

bool ToppersDomain::Validation(Manager * mgr)
{
	if(!(*mgr)[FORCEDOMAINSUPPORT])
	{
		if(Type != ProtectionDomain::SYSTEM)
			throw Exception(Message("All TOPPERS/JSP objects MUST BE belong to the system domain.","現在のバージョンではシステム以外のドメインは作れません"));

		if(GetParentDomain() != NULL)
			throw Exception(Message("Only one system domain can exist in this kernel.","カーネルは単一のシステムドメインしかサポートしていません"));
	}else
	{
		if(Type == ProtectionDomain::SHARED && GetParentDomain() != NULL)
			throw Exception(Message("Shared domain must be declared in outermost section.","共有ドメインは最も外側で宣言されなければなりません"));

		if(Type != ProtectionDomain::SHARED && GetParentDomain() == NULL)
			throw Exception(Message("The outermost domain must be declared as shared.","最外ドメインは共有ドメインでなければなりません"));
	}
	return true;
}

void ToppersDomain::Serialization(Manager * mgr, MultiStream & dest)
{
	ToppersManager * manager = static_cast<ToppersManager *>(mgr);

	manager->CurrentDomain = this;

	switch(Type)
	{
	case ProtectionDomain::USER:
		dest[Name + DOMAINCFGPREFIX];
		break;
	case ProtectionDomain::SYSTEM:
		dest[CFGFILE];
		break;
	case ProtectionDomain::SHARED:
		dest[SHAREDOMFILE];
		break;
	default:
		break;
	}

	(*dest) << endl << "/*\n * Protection domain [" << Name << "]\n */" << endl;
	manager->task.CreateBuffer(mgr, dest, "_stack", 5, 4, "static VP %s[TCOUNT_VP(%s)];", "TROUND_VP(%s)");
	manager->dataqueue.CreateBuffer(mgr, dest, "_dtqbuf", 2, 1, "static VP_INT %s[%s];");
	manager->fmempool.CreateBuffer(mgr, dest, "_fmempool", 3, 1, "static VP %s[%s];");
}
