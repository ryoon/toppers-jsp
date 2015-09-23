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
 *  @(#) $Id: cpu_config.h,v 1.14 2007/07/27 11:28:44 honda Exp $
 */

#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#include "cpu_defs.h"

#ifndef _MACRO_ONLY
#include <cpu_insn.h>
#endif

#ifndef _MACRO_ONLY

extern UW exception_count;
extern UW exception_registers[40];

typedef struct task_context_block {
        VP	sp;             
        FP	pc;             
} CTXB;


Inline BOOL
sense_context()
{
    return(exception_count > 0);
}

Inline BOOL
sense_lock()
{
    return((current_sr() & 0x1) == 0);
}

#define t_sense_lock     sense_lock
#define i_sense_lock     sense_lock

Inline void
lock_cpu()
{
    disint();
}

Inline void
unlock_cpu()
{
    enaint();
}

#define EXC_ENTRY(exchdr)     exchdr
#define EXCHDR_ENTRY(exchdr)  extern void exchdr(VP sp)

Inline BOOL
exc_sense_context(VP p_excinf)
{
    return(exception_count > 1);
}

Inline BOOL
exc_sense_lock(VP p_excinf)
{
	return((current_sr() & 0x2) == 0);
}


#define t_lock_cpu      lock_cpu
#define i_lock_cpu      lock_cpu
#define t_unlock_cpu    unlock_cpu
#define i_unlock_cpu    unlock_cpu

extern void dispatch(void);
extern void exit_and_dispatch(void);

extern FP int_table[TMAX_ALL_INTNO];
extern FP exc_table[TMAX_CORE_EXCNO];

Inline void define_inh(INTNO intno, FP inthdr)
{
    int_table[intno] = inthdr;
}

Inline void define_exc(EXCNO excno, FP exchdr)
{
    exc_table[excno] = exchdr;
}

extern void _start();
extern void Breakpoint_exception();
extern void InstructionBusError_exception();
extern void Watchpoint_exception();
extern void DataBusError_exception();
extern void DivideByZero_exception();
extern void Interrupt_exception();
extern void SystemCall_exception();

extern void cpu_initialize(void);
extern void cpu_terminate(void);

#endif /* _MACRO_ONLY */

#endif /* _CPU_CONFIG_H_ */

