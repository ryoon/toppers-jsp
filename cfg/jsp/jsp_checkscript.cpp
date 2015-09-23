#include "jsp_classes.h"
#include <string>
#include <iostream>

using namespace std;

void CheckScriptGenerator::parseOption(Directory & container)
{
	mpstrstream * stream;
	string work;

	if(findOption(container,"h","help"))
	{
		cout << '\n' << Message(
			"Configuration Checker Script Generation Option\n"
			"  -c, --check  : Generate a checker script\n",
			"カーネル構成チェックスクリプト生成用オプション\n"
			"  -c, --check  : チェックスクリプトを生成します\n");
		return;
	}

	if(findOption(container,"c","check",&work))
	{
		if(work.empty())
			work.assign("kernel_chk.c");

		stream = new mpstrstream(work);
		stream->createpart("header", "body","footer",NULL);

		container["/file/kernel_chk"] = stream;

		activateComponent(false);
	}

}

static void createScriptEntry(Directory & container, mpstrstream * out, const char * objname, const char * member, bool mode = true)
{
	string work;
	string inib;
	Directory * scope;
	int i, j;
	
	(*out) << container.format("\n\t\t/* $@ */\n\n");

	if(mode)
	{
		scope = container.getFirstChild();
		while(scope != 0)
		{
			work = scope->getKey();
			if(work[0] < '0' || work[1] > '9')
				(*out) << "\tOBJECT(" << container.getKey() << '_' << scope->toInteger() << ',' << work << ");\n";

			scope = scope->getNext();
		}

		(*out) << container.format("\tEVAR(ID,_kernel_tmax_$(/prefix/$@/id)id);\n");
	}else
	{
		i = 0;
		scope = container.getFirstChild();
		while(scope != 0)
		{
			work = scope->getKey();
			(*out) << "\tOBJECT(" << container.getKey() << '_' << (i++) << ',' << scope->getKey() << ");\n";
			scope = scope->getNext();
		}

		(*out) << container.format("\tEVAR(ID,_kernel_tnum_$(/prefix/$@/id)no);\n");
	}

	(*out) << container.format("\tEVAR($(/prefix/$@/SID)INIB,_kernel_$(/prefix/$@/sid)inib_table);\n");

	work.assign(member);
	i = 0;
	j = work.find_first_of(',');
	if(objname == NULL)
		inib = container.getKey();
	else
		inib = objname;
	
	do {
		(*out) << "\tMEMBER(" << inib << "_initialization_block," << work.substr(i, j-i) << ");\n";

		i = j + 1;
		j = work.find_first_of(',', i);
	} while(i != string::npos+1);
	(*out) << '\n';
}

inline void createScriptEntry(Directory & container, mpstrstream * out, const char * member)
{	createScriptEntry(container, out, NULL, member, true);	}


void CheckScriptGenerator::body(Directory & container)
{
	mpstrstream * out;

	out = reinterpret_cast<mpstrstream *>(container["/file/kernel_chk"].toPointer());

	out->moveto("header") <<
		"#include \"jsp_kernel.h\"\n"
		"#include \"logtask.h\"\n"
		"#include \"timer.h\"\n\n"
		"#define OBJECT(x,y) __asm(\"d\" #x \",\" #y \"@\");\n"
		"#define MEMBER(x,y) __asm(\"s\" #x \"::\" #y \",(%0),(%1)@\" ::\\\n"
		"     \"i\"(sizeof(((struct x *)0)->y)), \"i\"(&((struct x *)0)->y));\n"
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

	out->moveto("footer") << "}\n";

	out->moveto("body") <<
		"\tDEFS(TMAX_TPRI);\n\tDEFS(TMIN_TPRI);\n\n\tMEMBER(queue,next);\n\tMEMBER(queue,prev);\n\n";

	createScriptEntry(container[OBJECTTREE "/" TASK],      out, "tskatr,exinf,task,ipriority,stksz,stk,texatr,texrtn");
	createScriptEntry(container[OBJECTTREE "/" SEMAPHORE], out, "sematr,isemcnt,maxsem");
	createScriptEntry(container[OBJECTTREE "/" EVENTFLAG], out, "flgatr,iflgptn");
	createScriptEntry(container[OBJECTTREE "/" DATAQUEUE], out, "dtqatr,dtqcnt,dtq");
	createScriptEntry(container[OBJECTTREE "/" MAILBOX], out, "mbxatr,maxmpri");
	createScriptEntry(container[OBJECTTREE "/" FIXEDSIZEMEMORYPOOL], out, "fixed_memorypool", "mpfatr,blksz,mpf,limit");
	createScriptEntry(container[OBJECTTREE "/" CYCLICHANDLER], out, "cyclic_handler", "cycatr,exinf,cychdr,cyctim,cycphs");
	createScriptEntry(container[OBJECTTREE "/" INTERRUPTHANDLER], out, "interrupt_handler", "inhno,inhatr,inthdr", false);
	createScriptEntry(container[OBJECTTREE "/" EXCEPTIONHANDLER], out, "cpu_exception_handler", "excno,excatr,exchdr", false);

	VerboseMessage("Configuration check script generation was finished successfully.\n","カーネル構成チェックスクリプトファイルを出力しました");
}