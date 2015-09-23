/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: cpu_config.c,v 1.14 2007/07/27 11:28:44 honda Exp $
 */

#include "jsp_kernel.h"
#include "cpu_config.h"

UW exception_count;

UW exception_registers[40];

FP exc_table[TMAX_CORE_EXCNO] =
{
    (FP)_start,
    (FP)Breakpoint_exception,
    (FP)InstructionBusError_exception,
    (FP)Watchpoint_exception,
    (FP)DataBusError_exception,
    (FP)DivideByZero_exception,
    (FP)Interrupt_exception,
    (FP)SystemCall_exception
};

void dump_registers()
{
    syslog(LOG_EMERG, "r0  : 0x%08x, r1  : 0x%08x, r2  : 0x%08x, r3  : 0x%08x",
    	exception_registers[0], exception_registers[1], exception_registers[2], exception_registers[3]);
    syslog(LOG_EMERG, "r4  : 0x%08x, r5  : 0x%08x, r6  : 0x%08x, r7  : 0x%08x",
    	exception_registers[4], exception_registers[5], exception_registers[6], exception_registers[7]);
    syslog(LOG_EMERG, "r8  : 0x%08x, r9  : 0x%08x, r10 : 0x%08x, r11 : 0x%08x",
    	exception_registers[8], exception_registers[9], exception_registers[10], exception_registers[11]);
    syslog(LOG_EMERG, "r12 : 0x%08x, r13 : 0x%08x, r14 : 0x%08x, r15 : 0x%08x",
    	exception_registers[12], exception_registers[13], exception_registers[14], exception_registers[15]);    	
    syslog(LOG_EMERG, "r16 : 0x%08x, r17 : 0x%08x, r18 : 0x%08x, r19 : 0x%08x",
        exception_registers[16], exception_registers[17], exception_registers[18], exception_registers[19]);
    syslog(LOG_EMERG, "r20 : 0x%08x, r21 : 0x%08x, r22 : 0x%08x, r23 : 0x%08x",
        exception_registers[20], exception_registers[21], exception_registers[22], exception_registers[23]);    	
    syslog(LOG_EMERG, "r24 : 0x%08x, r25 : 0x%08x, r26 : 0x%08x, r27 : 0x%08x",
        exception_registers[24], exception_registers[25], exception_registers[26], exception_registers[27]);    	    	
    syslog(LOG_EMERG, "sp  : 0x%08x, ra  : 0x%08x, ea  : 0x%08x, ba  : 0x%08x",
        exception_registers[28], exception_registers[29], exception_registers[30], exception_registers[31]);
        
    syslog(LOG_EMERG, "ie  : 0x%08x, im  : 0x%08x, ip  : 0x%08x, icc : 0x%08x",
        exception_registers[32], exception_registers[33], exception_registers[34], exception_registers[35]);
    syslog(LOG_EMERG, "dcc : 0x%08x, cc  : 0x%08x, cfg : 0x%08x, eba : 0x%08x",
        exception_registers[36], exception_registers[37], exception_registers[38], exception_registers[39]);  	
}

void Breakpoint_exception()
{
    syslog(LOG_EMERG, "Breakpoint Exception occurs.");
    dump_registers();  
    while(1);
}

void InstructionBusError_exception()
{
    syslog(LOG_EMERG, "InstructionBusError Exception occurs.");
    dump_registers(); 
    while(1);
}

void Watchpoint_exception()
{
    syslog(LOG_EMERG, "Watchpoint Exception occurs.");
    dump_registers(); 
    while(1);
}

void DataBusError_exception()
{
    syslog(LOG_EMERG, "DataBusError Exception occurs.");
    dump_registers();       
    while(1);
}

void DivideByZero_exception()
{
    syslog(LOG_EMERG, "DivideByZero Exception occurs.");
    dump_registers();        
    while(1);
}

void Interrupt_exception()
{
    syslog(LOG_EMERG, "Interrupt Exception occurs.");
    dump_registers();         
    while(1);
}

void SystemCall_exception()
{
    syslog(LOG_EMERG, "SystemCall Exception occurs.");
    dump_registers();     
    while(1);
}

void cpu_initialize(void) 
{
	exception_count = 1;
}

void cpu_terminate(void)
{

}
