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
 *  @(#) $Id: arm7tdmi.h,v 1.6 2002/04/05 07:43:50 honda Exp $
 */

#ifndef _ARM7TDMI_H_
#define _ARM7TDMI_H_


/*
 *    ARM7TDMIの定義
 */
#ifndef _MACRO_ONLY
typedef unsigned char   byte;
typedef volatile byte   IOREG;
typedef unsigned short  word;
typedef volatile word   HIOREG;
typedef volatile int    LIOREG;
#endif /* _MACRO_ONLY */

/*
 *  ARM Exception Vector
 */
#define SVC_Vector    0x00
#define UND_Vector    0x04
#define SWI_Vector    0x08
#define PRFA_Vector   0x0C
#define DATAA_Vector  0x10
#define IRQ_Vector    0x18
#define FIQ_Vector    0x1C


/*
 * ARM Exception Vector Number
 */
#define SVC_Number    0
#define UND_Number    1
#define SWI_Number    2
#define PRFA_Number   3
#define DATAA_Number  4
#define UNNOWN_Number 5
#define IRQ_Number    6
#define FIQ_Number    7

/*
 *  CPSR 割込み禁止ビット
 */
#define CPSR_INT_MASK 0xC0
#define CPSR_IRQ_BIT  0x80
#define CPSR_FIQ_BIT  0x40

/*
 *  CPSR のモードビット
 */
#define CPSR_MODE_MASK   0x1f
#define CPSR_USER        0x10
#define CPSR_FIQ         0x11
#define CPSR_IRQ         0x12
#define CPSR_SVC         0x13
#define CPSR_ABT         0x17
#define CPSR_SYS         0x1F


/*
 * Stub Call
 */
#ifndef _MACRO_ONLY
Inline void
stub_exit(void){
    Asm("swi 0xff");
}

Inline void
stub_putc(char c)
{
    Asm("mov r0,#0x0; mov r4,%0; swi 0x123456"
        :: "r"(c):"r0","r4");
}

Inline void
stub_write(const char *string)
{
    int pos = 0;
    
    while(string[pos] !=0)
        stub_putc(string[pos++]);
}

Inline void
stub_set_inh(INHNO inhno, FP inthdr)
{
    Asm("mov r0,#0x1; mov r4,%0; mov r5,%1; swi 0x123456"
        : /* no output */
        : "r"(inhno),"r"(inthdr)
        : "r0", "r4", "r5");
    
}

#endif /* _MACRO_ONLY */

#endif /* _ARM7TDMI_H_ */
