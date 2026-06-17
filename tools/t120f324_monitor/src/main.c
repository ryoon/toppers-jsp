////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2013-2022 Efinix Inc. All rights reserved.              
//
// This   document  contains  proprietary information  which   is        
// protected by  copyright. All rights  are reserved.  This notice       
// refers to original work by Efinix, Inc. which may be derivitive       
// of other work distributed under license of the authors.  In the       
// case of derivative work, nothing in this notice overrides the         
// original author's license agreement.  Where applicable, the           
// original license agreement is included in it's original               
// unmodified form immediately below this header.                        
//                                                                       
// WARRANTY DISCLAIMER.                                                  
//     THE  DESIGN, CODE, OR INFORMATION ARE PROVIDED “AS IS” AND        
//     EFINIX MAKES NO WARRANTIES, EXPRESS OR IMPLIED WITH               
//     RESPECT THERETO, AND EXPRESSLY DISCLAIMS ANY IMPLIED WARRANTIES,  
//     INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF          
//     MERCHANTABILITY, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR    
//     PURPOSE.  SOME STATES DO NOT ALLOW EXCLUSIONS OF AN IMPLIED       
//     WARRANTY, SO THIS DISCLAIMER MAY NOT APPLY TO LICENSEE.           
//                                                                       
// LIMITATION OF LIABILITY.                                              
//     NOTWITHSTANDING ANYTHING TO THE CONTRARY, EXCEPT FOR BODILY       
//     INJURY, EFINIX SHALL NOT BE LIABLE WITH RESPECT TO ANY SUBJECT    
//     MATTER OF THIS AGREEMENT UNDER TORT, CONTRACT, STRICT LIABILITY   
//     OR ANY OTHER LEGAL OR EQUITABLE THEORY (I) FOR ANY INDIRECT,      
//     SPECIAL, INCIDENTAL, EXEMPLARY OR CONSEQUENTIAL DAMAGES OF ANY    
//     CHARACTER INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF      
//     GOODWILL, DATA OR PROFIT, WORK STOPPAGE, OR COMPUTER FAILURE OR   
//     MALFUNCTION, OR IN ANY EVENT (II) FOR ANY AMOUNT IN EXCESS, IN    
//     THE AGGREGATE, OF THE FEE PAID BY LICENSEE TO EFINIX HEREUNDER    
//     (OR, IF THE FEE HAS BEEN WAIVED, $100), EVEN IF EFINIX SHALL HAVE 
//     BEEN INFORMED OF THE POSSIBILITY OF SUCH DAMAGES.  SOME STATES DO 
//     NOT ALLOW THE EXCLUSION OR LIMITATION OF INCIDENTAL OR            
//     CONSEQUENTIAL DAMAGES, SO THIS LIMITATION AND EXCLUSION MAY NOT   
//     APPLY TO LICENSEE.                                                
//
////////////////////////////////////////////////////////////////////////////////
#include "comdefs.h"
#include "mondefs.h"
#include "serial.h"

/*
 *	Command String and Vector
 */
const long CommandVector[] = {
	(long)EnterLongCmd,			/* enter long data command */
	(long)DumpLongCmd,			/* dump long command */
	(long)LoadCommand,			/* load command */
	(long)GoCommand,			/* go command */
	(long)HelpCmd
};

const unsigned short CommandString[] = {
	MONCMD_ENTER_LONG,
	MONCMD_DUMP_LONG,
	MONCMD_LOAD,
	MONCMD_GO,
	CMDSTR_HELP,
	0			/* terminator */
};

const char HelpStrings[] = 
		TARGET_CPU_STR
		" Monitor v1.04 Mini\n"
		"Copyright (C) 1999-2004 CSE Tomakomai NCT\n"
		"Copyright (C) 2016-2023 TOPPERS PROJECT.\0"
		"----------------------------------------------------------------------------\0"
		"ld             load program          | go[<ad>]       go program\0"
		"dl[<ad>[<ln>]] dump long word        | el[<ad>]       enter long word\0"
		"?              help\0"
		"\0";

extern void BeforeInit(void);

/*
 *	Send Prompt	
 *	Input:	arg1: Serial Channel #, 0 or 1
 */
void SendPrompt(int id)
{
	MonSendPrompt(id);
	Status[id] = 0;
	InitRecLine(id);
}


void main() {
	int c;

    BeforeInit();
	InitSerial(0);

	apl_printf(HelpStrings);
	apl_putchar('\n');
	SendPrompt(DEF_MON_SERIAL);
	for(;;){
		ParseCmdCore(DEF_MON_SERIAL);
	}
}


