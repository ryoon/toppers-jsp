#include "jsp_defs.h"
#include "component.h"
#include "filecontainer.h"
#include <fstream>

class ConfigurationChecker : public Component
{
protected:
	unsigned int error_count;
	std::string banner;

	void set_banner(const std::string &);
	void set_banner(Directory &, const Message &, const char *, int);
	void notify(const Message &, bool = true);

	bool check_taskblock(Directory &, filecontainer *);
	bool check_semaphoreblock(Directory &, filecontainer *);
	bool check_eventflagblock(Directory &, filecontainer *);
	bool check_dataqueueblock(Directory &, filecontainer *);
	bool check_mailboxblock(Directory &, filecontainer *);
	bool check_fixed_memorypoolblock(Directory &, filecontainer *);
	bool check_cyclic_handlerblock(Directory &, filecontainer *);
	bool check_interrupt_handlerblock(Directory &, filecontainer *);
	bool check_exception_handlerblock(Directory &, filecontainer *);

	virtual void parseOption(Directory &);
	virtual void body(Directory &);
};

//------------------------------------------------------
using namespace std;

const char * BootBanner = "--- TOPPERS/JSP Configuration Checker (ver 2.0) ---";


ConfigurationChecker  instance_of_ConfigurationChecker;

//------------------------------------------------------


#define LoadVal(x,y,z) x = 0; container->load_variable(&x, sizeof(x), y, z)
#define Load(x,y) container->load_variable(&(x), sizeof(x), y)

inline void ConfigurationChecker::set_banner(const string & src)
{
	if(VerboseMessage::getVerbose())
		cout << src << endl;
	else
		banner = src;
}

void ConfigurationChecker::set_banner(Directory & container, const Message & object, const char * type, int id)
{
	string objectname;
	Directory * node;
	char buffer[32];


	banner = string("    ") + (const char *)object + " : ";

	objectname = filecontainer::get_instance()->get_objectname(id, type);
	sprintf(buffer, "id = %d", id);

	if(objectname.empty())
		banner += buffer;
	else
		banner += objectname + " (" + buffer + ") ";

	node = container.findChild(OBJECTTREE, type, objectname.c_str(), "position", NULL);
	if(node != 0)
		banner += string(" at ") + node->toString();

	if(VerboseMessage::getVerbose())
	{
		cout << banner << endl;
		banner.erase();
	}
}

inline void ConfigurationChecker::notify(const Message & msg, bool error)
{
	if(!banner.empty())
	{
		cout << banner << endl;
		banner.erase();
	}

	cout << "      ";

	if(error)
	{
		cout << Message("[ Error ] ","[エラー] ");
		error_count ++;
	}else
		cout << Message("[Warning] ","[ 警告 ] ");
	cout << msg << endl;
}


bool ConfigurationChecker::check_taskblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int maxpri;
	unsigned int minpri;
	unsigned int work;
	unsigned int num;
	struct variable_information tinib_table;
	unsigned int old_error_count = error_count;

	Message object("Task","タスク");

	if(!Load(num, "_kernel_tmax_tskid"))
		Exception("Internal error: Unknown symbol (Probably, Symbol table was stripped)","内部エラー: 不正なシンボル名 (実行形式がシンボルテーブルを含んでない可能性があります)");
	if(num <= 1)
	{
		error_count ++;
		cout << Message("  [Task] : No tasks created\n","  [タスク] : タスクオブジェクトがありません\n");
		return true;
	}

	tinib_table = container->get_variable_information("_kernel_tinib_table");
	maxpri = container->get_variable_information("TMAX_TPRI").value;
	minpri = container->get_variable_information("TMIN_TPRI").value;
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, TASK, id);

			//属性チェック
		LoadVal(work, tinib_table.address, "task_initialization_block::tskatr");
		if((work & ~0x3) != 0)
			notify(Message("Illegal task attribute (It should be ((TA_HLNG||TA_ASM)|TA_ACT))","不正なタスク属性 ((TA_HLNG||TA_ASM)|TA_ACT)以外"));

			//優先度チェック
		LoadVal(work, tinib_table.address, "task_initialization_block::ipriority");
		if(work < minpri)
			notify(Message("Initial priority is less than minimum priority (%d).","初期優先度が最低優先度(%d)を超えている").format(minpri));
		if(work > maxpri)
			notify(Message("Initial priority is greater than maximum priority (%d).","初期優先度が最大優先度(%d)を超えている").format(maxpri));

		tinib_table.address += tinib_table.size;
	}

	return old_error_count == error_count;
}

bool ConfigurationChecker::check_semaphoreblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;
	unsigned int work2;
	unsigned int old_error_count = error_count;

	struct variable_information seminib;
	Message object("Semaphore","セマフォ");

	Load(num, "_kernel_tmax_semid");
	if(num <= 1)
		return true;

	seminib = container->get_variable_information("_kernel_seminib_table");
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, SEMAPHORE, id);

			//attribute validation check
		LoadVal(work, seminib.address, "semaphore_initialization_block::sematr");
		if((work & ~0x1) != 0)
			notify(Message("Illegal attribute (It should be (TA_TFIFO||TA_TPRI)).","(TA_TFIFO||TA_TPRI)以外の属性が指定されている"));

			//maxcnt < isemcnt
		LoadVal(work, seminib.address, "semaphore_initialization_block::maxsem");
		LoadVal(work2,seminib.address, "semaphore_initialization_block::isemcnt");
		if(work < work2)
			notify(Message("Initial count[%d] is greater than the maximum count[%d] of this semaphore","初期値[%d]が最大値[%d]を超えている").format(work2,work));
	
		seminib.address += seminib.size;
	}

	return old_error_count == error_count;
}


bool ConfigurationChecker::check_eventflagblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Event flag","イベントフラグ");

	Load(num, "_kernel_tmax_flgid");
	if(num <= 1)
		return true;

	inib = container->get_variable_information("_kernel_flginib_table");
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, EVENTFLAG, id);
			
			//attribute validation check
		LoadVal(work, inib.address, "eventflag_initialization_block::flgatr");
		if((work & ~0x7) != 0)
			notify(Message("Illegal attribute value [0x%x]","おかしな属性値 [0x%x]").format(work & ~0x7));
		if((work & 0x2) != 0)
			notify(Message("Attribute TA_WMUL is not supported in current version.","TA_WMULはサポート外").format(work & ~0x7));
	
		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


bool ConfigurationChecker::check_dataqueueblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Data queue","データキュー");

	Load(num, "_kernel_tmax_dtqid");
	if(num <= 1)
		return true;

	inib = container->get_variable_information("_kernel_dtqinib_table");
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, DATAQUEUE, id);

			//attribute validation check
		LoadVal(work, inib.address, "dataqueue_initialization_block::dtqatr");
		if((work & ~0x1) != 0)
			notify(Message("Illegal attribute value [0x%x]","おかしな属性値 [0x%x]").format(work & ~0x1));
	
		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


bool ConfigurationChecker::check_mailboxblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Mailbox","メールボックス");

	Load(num, "_kernel_tmax_mbxid");
	if(num <= 1)
		return true;
	
	inib = container->get_variable_information("_kernel_mbxinib_table");
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, MAILBOX, id);

			//attribute validation check
		LoadVal(work, inib.address, "mailbox_initialization_block::mbxatr");
		if((work & ~0x3) != 0)
			notify(Message("Illegal attribute value [0x%x]","おかしな属性値 [0x%x]").format(work & ~0x3));
	
			//mailbox message priority check
		LoadVal(work, inib.address, "mailbox_initialization_block::maxmpri");
		if( container->get_variable_information("mailbox_initialization_block::maxmpri").size == 2)
			work <<= 16;
		if((signed)work < 0)
			notify(Message("Priority must not be a negative number.","優先度が負値").format(work & ~0x1));

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool ConfigurationChecker::check_fixed_memorypoolblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Fixed size memory pool","固定長メモリプール");

	Load(num, "_kernel_tmax_mpfid");
	if(num <= 1)
		return true;

	inib = container->get_variable_information("_kernel_mpfinib_table");
	VerboseMessage("%s object : %d items","%sオブジェクト : %d 個").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, FIXEDSIZEMEMORYPOOL, id);

			//attribute validation check
		LoadVal(work, inib.address, "fixed_memorypool_initialization_block::mpfatr");
		if((work & ~0x1) != 0)
			notify(Message("Illegal attribute value [0x%x]","おかしな属性値 [0x%x]").format(work & ~0x1));

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


bool ConfigurationChecker::check_cyclic_handlerblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Cyclic handler","周期ハンドラ");

	Load(num, "_kernel_tmax_cycid");
	if(num <= 1)
		return true;

	inib = container->get_variable_information("_kernel_cycinib_table");
	VerboseMessage("%s object : %d items","%sオブジェクト : %d 個").format((const char *)object, num);
	for(id = 1; id <= num; id++)
	{
		set_banner(parameter, object, CYCLICHANDLER, id);

			//attribute validation check
		LoadVal(work, inib.address, "cyclic_handler_initialization_block::cycatr");
		if((work & ~0x7) != 0)
			notify(Message("Illegal attribute value [0x%x]","おかしな属性値 [0x%x]").format(work & ~0x1));
		if( (work & 0x1) != 0 || (work & 0x4) != 0)
			notify(Message("TA_ASM and TA_PHS are not supported in this kernel.","TA_ASMとTA_PHSはサポート外"));

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool ConfigurationChecker::check_interrupt_handlerblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Interrupt handler","割込みハンドラ");

	Load(num, "_kernel_tnum_inhno");
	if(num == 0)
		return true;

	inib = container->get_variable_information("_kernel_inhinib_table");
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 0; id < num; id++)
	{
		set_banner(parameter, object, INTERRUPTHANDLER, id);

			//attribute validation check
		LoadVal(work, inib.address, "interrupt_handler_initialization_block::inhatr");
		if(work != 0)
			notify(Message("The attribute can take only TA_HLNG","TA_HNLG以外の属性は設定できません"));

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool ConfigurationChecker::check_exception_handlerblock(Directory & parameter, filecontainer * container)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Exception handler","例外ハンドラ");

	Load(num, "_kernel_tnum_excno");
	if(num == 0)
		return true;

	inib = container->get_variable_information("_kernel_excinib_table");
	VerboseMessage("%s object : %d items\n","%sオブジェクト : %d 個\n").format((const char *)object, num);
	for(id = 0; id < num; id++)
	{
		set_banner(parameter, object, EXCEPTIONHANDLER, id);

			//attribute validation check
		LoadVal(work, inib.address, "exception_handler_initialization_block::excatr");
		if(work != 0)
			notify(Message("The attribute can take only TA_HLNG","TA_HNLG以外の属性は設定できません"));

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


//------------------------------------------------------
#undef LoadVal
#undef Load

void ConfigurationChecker::parseOption(Directory & parameter)
{
	string loadmodule;
	string work;

	if(findOption(parameter,"h","help"))
	{
		cout << endl << Message(
			"Configuration checker - option\n"
			"  -m, --module=filename : Specify the load module (essential option)\n"
			"  -cs, --script=filename  : Specify the checker script file\n",
			"コンフィギュレーションチェッカ - オプション\n"
			"  -m, --module=ファイル名 : ロードモジュール名を指定します (必須項目)\n"
			"  -cs, --script=ファイル名  : チェッカスクリプトを指定します\n");
		return;
	}

	if(findOption(parameter,"m","module",&loadmodule) || findOption(parameter,DEFAULT_PARAMETER,NULL,&loadmodule))
	{
		if(findOption(parameter,"s","source"))
			Exception("Configuration checker can not execute while Configurator executes","コンフィギュレータとチェッカは同時に起動できません");

		parameter["/file/loadmodule"] = loadmodule;
		activateComponent(false);
	}

	if(!findOption(parameter,"cs","script",&work))
		work = loadmodule.substr(0,loadmodule.find_last_of('.')) + ".chk";
	parameter["/file/checkerscript"] = work;

	if(findOption(parameter,"obj","load-object",&work))
	{
		if(work.empty())
			work.assign("kernel_obj.dat");

		fstream f(work.c_str(), ios::in|ios::binary);
		if(f.is_open())
		{
			parameter["/object"].Load(&f);
			f.close();
		}else
			Exception(" Failed to open the file %s for storing object definitions"," オブジェクト情報を格納するためのファイル(%s)が開けません").format(work.c_str());
	}
}

void ConfigurationChecker::body(Directory & parameter)
{
	filecontainer * container;
	bool result = true;

	container = filecontainer::get_instance();
	container->attach_module(parameter["/file/loadmodule"].toString().c_str());
	container->attach_infofile(parameter["/file/checkerscript"].toString().c_str());

	cout << Message("  Target architecture : ","  ターゲットアーキテクチャ : ") << container->get_architecture() << endl;

	error_count = 0;
	result &= check_taskblock(parameter,container);
	result &= check_semaphoreblock(parameter,container);
	result &= check_eventflagblock(parameter,container);
	result &= check_dataqueueblock(parameter,container);
	result &= check_mailboxblock(parameter,container);
	result &= check_fixed_memorypoolblock(parameter,container);
	result &= check_cyclic_handlerblock(parameter,container);
	result &= check_interrupt_handlerblock(parameter,container);
	result &= check_exception_handlerblock(parameter,container);

	if(!result)
		Exception("Total %d errors found in current configuration.\n","全部で%d個のエラーが検出されました\n").format(error_count);

	cout << Message("No error found in current configuration\n","構成に異常はありませんでした\n");
}
