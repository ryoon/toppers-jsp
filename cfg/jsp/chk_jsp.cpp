/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: chk_jsp.cpp,v 1.2 2001/11/12 04:48:18 takayuki Exp $
 */

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "chk_jsp.h"
#include <stdio.h>
#include <map>
#include <string>

using namespace std;

unsigned int error_count = 0;

enum tagOptionList
{
	VERBOSE = 1,
	HELP    = 2
};

map<int, bool> option_container;
string modulename;
string symbolfilename;

void print_usage(void)
{
	printf(Message(" Usage : chk [options] module_file [checker_file]\n\n"," 使い方 : chk [オプション] モジュールファイル [チェッカーファイル]\n\n"));
	printf(Message(" Option list\n"," オプション\n"));
	printf("   -v --verbose    : %s\n",(const char *)Message("Verbose mode","詳細表示を行う"));
	printf("   -le --english   : %s\n",(const char *)Message("Shows all messages in English","メッセージを英語にする"));
	printf("   -lj --japansese : %s\n",(const char *)Message("Shows all messages in Japanese","メッセージを日本語にする"));
	printf("\n");
	printf(Message(" Module file\n"," モジュールファイル\n"));
	printf(Message("   \"Module file\" is the path of the target module file.\n\n","   対象となるモジュールへのパスを指定します\n\n"));
	printf(Message(" Checker file\n"," チェッカーファイル\n"));
	printf(Message("   \"Checker file\" is the path of checker file \n   that contains some informations of the corresponding target module.\n   If omitted, checker uses the path to target module\n   which replaced extensions by \".chk\" instead of the checker file path.","   モジュールに対応するチェッカーファイルのパスを指定します。\n   省略した場合はモジュール名の拡張子を.chkにしたファイルを\n   指定したものとします。\n"));

	printf("\n\n");
}


bool parse_option(int argc, char * argv [])
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
			case 'v': option_container[VERBOSE] = true; break;
			case 'h': option_container[HELP] = true;    break;
			case '?': option_container[HELP] = true;    break;

			case 'l':
				if(*(argv[i]+2) == 'j')
					Message::SelectCodePage(0x411);
				else
					Message::SelectCodePage();
				break;

			case '-':
				work = argv[i]+2;

				if(work.compare("english")==0)
				{
					Message::SelectCodePage();
					break;
				}

				if(work.compare("japanese")==0)
				{
					Message::SelectCodePage(0x411);
					break;
				}

				if(work.compare("verbose")==0)
				{
					option_container[VERBOSE] = true;
					break;
				}

				if(work.compare("help")==0)
				{
					option_container[HELP] = true;
					break;
				}

				throw Exception(string(UnknownOption) + argv[i] + "]");

			default:
				break;
			}
		}else
		{
			if(!modulename.empty())
			{
				if(!symbolfilename.empty())
					throw Exception(string(UnknownOption) + argv[i] + "]");
				else
					symbolfilename = argv[i];
			}else
				modulename = argv[i];
		}
	}

	return true;
}




#define LoadVal(x,y,z) x = 0; container->load_variable(&x, sizeof(x), y, z)
#define Load(x,y) container->load_variable(&(x), sizeof(x), y)

void print_header(Message & object, const char * type, int id, bool error = true)
{
	string work;
	string objectname;

	work = "    [";
	work += object;
	work += " (";

	objectname = container->get_objectname(id, type);
	if(objectname.empty())
	{
		char buffer[32];
		sprintf(buffer, "id = %d", id);
		work += buffer;
	}else
		work += objectname;
	work += ")]: ";

	printf(work.c_str());

	if(error)
		error_count ++;
}


bool check_taskblock(void)
{
	unsigned int id;
	unsigned int maxpri;
	unsigned int minpri;
	unsigned int work;
	unsigned int num;
	struct variable_information tinib_table;
	unsigned int old_error_count = error_count;
	Message object("Task","タスク");

	Load(num, "_kernel_tmax_tskid");
	tinib_table = container->get_variable_information("_kernel_tinib_table");
	maxpri = container->get_variable_information("TMAX_TPRI").value;
	minpri = container->get_variable_information("TMIN_TPRI").value;

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//属性チェック
		LoadVal(work, tinib_table.address, "task_initialization_block::tskatr");
		if((work & ~0x3) != 0)
		{
			print_header(object, "TASK", id);
			printf(Message("Illegal task attribute (It should be ((TA_HLNG||TA_ASM)|TA_ACT))\n","不正なタスク属性 ((TA_HLNG||TA_ASM)|TA_ACT)以外\n"));
		}
			//優先度チェック
		LoadVal(work, tinib_table.address, "task_initialization_block::ipriority");
		if(work < minpri)
		{
			print_header(object, "TASK", id);
			printf(Message("Initial priority is less than minimum priority (%d).\n","初期優先度が最低優先度(%d)を超えている\n"),minpri);
		}
		if(work > maxpri)
		{
			print_header(object, "TASK", id);
			printf(Message("Initial priority is greater than maximum priority (%d).\n","初期優先度が最大優先度(%d)を超えている\n"),maxpri);
		}
		tinib_table.address += tinib_table.size;
	}

	return old_error_count == error_count;
}

bool check_semaphoreblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;
	unsigned int work2;
	unsigned int old_error_count = error_count;

	struct variable_information seminib;
	Message object("Semaphore","セマフォ");

	Load(num, "_kernel_tmax_semid");
	seminib = container->get_variable_information("_kernel_seminib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, seminib.address, "semaphore_initialization_block::sematr");
		if((work & ~0x1) != 0)
		{
			print_header(object, "SEM", id);
			printf(Message("Illegal attribute (It should be (TA_TFIFO||TA_TPRI)).\n","(TA_TFIFO||TA_TPRI)以外の属性が指定されている\n"));
		}

			//maxcnt < isemcnt
		LoadVal(work, seminib.address, "semaphore_initialization_block::maxsem");
		LoadVal(work2,seminib.address, "semaphore_initialization_block::isemcnt");
		if(work < work2)
		{
			print_header(object, "SEM", id);
			printf(Message("Initial count[%d] is greater than the maximum count[%d] of this semaphore\n","初期値[%d]が最大値[%d]を超えている\n"),work2,work);
		}
	
		seminib.address += seminib.size;
	}

	return old_error_count == error_count;
}


bool check_eventflagblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Event flag","イベントフラグ");

	Load(num, "_kernel_tmax_flgid");
	inib = container->get_variable_information("_kernel_flginib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "eventflag_initialization_block::flgatr");
		if((work & ~0x7) != 0)
		{
						print_header(object, "FLG", id);
			printf(Message("Illegal attribute value [0x%x]\n","おかしな属性値 [0x%x]\n"),(work & ~0x7));
		}
		if((work & 0x2) != 0)
		{
						print_header(object, "FLG", id);
			printf(Message("Attribute TA_WMUL is not supported in current version.\n","TA_WMULはサポート外\n"),(work & ~0x7));
		}
	
		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


bool check_dataqueueblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Data queue","データキュー");

	Load(num, "_kernel_tmax_dtqid");
	inib = container->get_variable_information("_kernel_dtqinib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "dataqueue_initialization_block::dtqatr");
		if((work & ~0x1) != 0)
		{
			print_header(object, "DTQ", id);
			printf(Message("Illegal attribute value [0x%x]\n","おかしな属性値 [0x%x]\n"),(work & ~0x1));
		}
	
		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


bool check_mailboxblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Mailbox","メールボックス");

	Load(num, "_kernel_tmax_mbxid");
	inib = container->get_variable_information("_kernel_mbxinib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "mailbox_initialization_block::mbxatr");
		if((work & ~0x3) != 0)
		{
			print_header(object, "MBX", id);
			printf(Message("Illegal attribute value [0x%x]\n","おかしな属性値 [0x%x]\n"),(work & ~0x3));
		}
	
			//mailbox message priority check
		LoadVal(work, inib.address, "mailbox_initialization_block::maxmpri");
		if( container->get_variable_information("mailbox_initialization_block::maxmpri").size == 2)
			work <<= 16;
		if((signed)work < 0)
		{
			print_header(object, "MBX", id);
			printf(Message("Priority must not be a negative number.\n","優先度が負値\n"),(work & ~0x1));
		}

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool check_fixed_memorypoolblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Fixed size memory pool","固定長メモリプール");

	Load(num, "_kernel_tmax_mpfid");
	inib = container->get_variable_information("_kernel_mpfinib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "fixed_memorypool_initialization_block::mpfatr");
		if((work & ~0x1) != 0)
		{
			print_header(object, "MPF", id);
			printf(Message("Illegal attribute value [0x%x]\n","おかしな属性値 [0x%x]\n"),(work & ~0x1));
		}

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}


bool check_cyclic_handlerblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Cyclic handler","周期ハンドラ");

	Load(num, "_kernel_tmax_cycid");
	inib = container->get_variable_information("_kernel_cycinib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "cyclic_handler_initialization_block::cycatr");
		if((work & ~0x7) != 0)
		{
						print_header(object, "CYC", id);
			printf(Message("Illegal attribute value [0x%x]\n","おかしな属性値 [0x%x]\n"),(work & ~0x1));
		}
		if( (work & 0x1) != 0 || (work & 0x4) != 0)
		{
						print_header(object, "CYC", id);
			printf(Message("TA_ASM and TA_PHS are not supported in this kernel.\n","TA_ASMとTA_PHSはサポート外"));
		}

		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool check_interrupt_handlerblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Interrupt handler","割込みハンドラ");

	Load(num, "_kernel_tnum_inhno");
	inib = container->get_variable_information("_kernel_inhinib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "interrupt_handler_initialization_block::inhatr");
		if(work != 0)
		{
						print_header(object, "INT", id);
			printf(Message("The attribute can take only TA_HLNG\n","TA_HNLG以外の属性は設定できません\n"));
		}
		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool check_exception_handlerblock(void)
{
	unsigned int id;
	unsigned int num;
	unsigned int work;

	unsigned int old_error_count = error_count;
	struct variable_information inib;
	Message object("Exception handler","例外ハンドラ");

	Load(num, "_kernel_tnum_excno");
	inib = container->get_variable_information("_kernel_excinib_table");

	if(option_container[VERBOSE] && num != 0)
		printf(string(string("  ") + (const char *)object + (const char *)Message(" object","オブジェクト") + " : %d items\n").c_str(), num);

	for(id = 1; id <= num; id++)
	{
			//attribute validation check
		LoadVal(work, inib.address, "exception_handler_initialization_block::excatr");
		if(work != 0)
		{
						print_header(object, "EXC", id);
			printf(Message("The attribute can take only TA_HLNG\n","TA_HNLG以外の属性は設定できません\n"));
		}
		inib.address += inib.size;
	}

	return old_error_count == error_count;
}

bool check_body(void)
{
	bool result = true;
	
	if(option_container[HELP])
	{
		print_usage();
		return true;
	}

	if(modulename.empty())
	{
		printf(Message(" Failure : Requires path for the target module\n\n","  エラー : 対象モジュール名を指定してください\n\n"));
		print_usage();
		return false;
	}

	if(symbolfilename.empty())
		symbolfilename = modulename.substr(0, modulename.find_last_of('.')) + ".chk";

	container->attach_module(modulename.c_str());
	container->attach_infofile(symbolfilename.c_str());
	printf("  Target architecture : %s\n",container->get_architecture());

	result &= check_taskblock();
	result &= check_semaphoreblock();
	result &= check_eventflagblock();
	result &= check_dataqueueblock();
	result &= check_mailboxblock();
	result &= check_fixed_memorypoolblock();
	result &= check_cyclic_handlerblock();
	result &= check_interrupt_handlerblock();
	result &= check_exception_handlerblock();

	if(result)
		printf(Message("No error found in current configuration\n","構成に異常はありませんでした\n"));
	else
		printf(Message("Total %d errors found in current configuration.\n","全部で%d個のエラーが検出されました\n"),error_count);

	return result;
}