/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
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
 *  @(#) $Id: sys_config.c,v 1.2 2002/04/10 12:08:17 honda Exp $
 */

/*
 *	ターゲットシステム依存モジュール（PC/AT用）
 */

#include "jsp_kernel.h"
#include "cpu_insn.h"

#include "irc.h"
#include "irc_inline.h"

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	irc_initialize();
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
	while(1);
}

ER irc_eoi_irq(UB irq)
{
	UW mask;
	BOOL cpu_locked;

	cpu_locked = sense_lock();

	if(!cpu_locked)
	{
		i386_lock_cpu();
	}

	mask = 1 << (irq & 7);
	if (irq < 8)
	{
		outb(0x21, inb(0x21) | mask);/* block the irq */
		outb(0x20, 0x20);/* acknowledge the irq */

		outb(0x20, 0x0B);
		if (inb(0x20) & mask)
			syslog_0(LOG_EMERG, "irq still active");
	}
	else 
	{
		unsigned foo;

		outb(0xA1, inb(0xA1) | mask);/* block */
		outb(0xA0, 0x20);/* acknowledge */

		outb(0xA0, 0x0B);
		if ((foo = inb(0xA0)) == 0)
			outb(0x20, 0x20);
		if (foo & mask)
		syslog_0(LOG_EMERG, "irqslave still active");
	}

	if(!cpu_locked)
	{
		i386_unlock_cpu();
	}

	return E_OK;
}

ER irc_dis_irq(UB irq)
{
	BOOL cpu_locked;

	cpu_locked = sense_lock();

	if(!cpu_locked)
	{
		i386_lock_cpu();
	}

	if(irq < 8)
	{
		outb(0x21, inb(0x21) | (1 << irq));
	}
	else
	{
		outb(0xA1, inb(0xA1) | (1 << (irq - 8)));
	}

	if(!cpu_locked)
	{
		i386_unlock_cpu();
	}

	return E_OK;
}

ER irc_ena_irq(UB irq)
{
	BOOL cpu_locked;

	cpu_locked = sense_lock();

	if(!cpu_locked)
	{
		i386_lock_cpu();
	}

	if(irq < 8)
	{
		outb(0x21, inb(0x21) & ~(1 << irq));
	}
	else
	{
		outb(0xA1, inb(0xA1) & ~(1 << (irq - 8)));
	}

	if(!cpu_locked)
	{
		i386_unlock_cpu();
	}

	return E_OK;
}


#ifdef WITH_STUB
#error "WITH_STUB option isn't supported in this release."
#define SYS_PUT_CHAR(c) stub_putc(c)
#else
#include "serial.h"	/*  serial_write( )  */
#define SYS_PUT_CHAR(c) {		\
	char cc=c;			\
	serial_write(1, &cc, 1);	\
}
#endif /* WITH_STUB */

void
sys_putc(char c)
{
    if (c == '\n') {
        SYS_PUT_CHAR('\r');
    }
    SYS_PUT_CHAR(c);
}
