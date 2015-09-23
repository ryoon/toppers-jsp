/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
 *  Copyright (C) 2008-     by Monami Software Limited Partnership, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: sys_config.c,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

/*
 *	ターゲットシステム依存モジュール（PC/AT用）
 */

#include "jsp_kernel.h"
#include <sil.h>
#include <i386.h>
#include <irc.h>
#include <irc_inline.h>

#ifndef SYS_PUTC_PORTID
#define SYS_PUTC_PORTID 1
#endif

#if SYS_PUTC_PORTID >= 1
#include <pcat_com.h>
#else
#define VIDEO_ADDRESS 0xb8000
#define VIDEO_ATTRIBUTE 7
#define VIDEO_X_SIZE 80
#define VIDEO_Y_SIZE 25
UB video_x;
UB video_y;
Inline void video_write(UB x, UB y, char c) {
	sil_wrb_mem((VP)(VIDEO_ADDRESS + (VIDEO_X_SIZE * y + x) * 2), c);
	sil_wrb_mem((VP)(VIDEO_ADDRESS + (VIDEO_X_SIZE * y + x) * 2 + 1), VIDEO_ATTRIBUTE);
}
void video_init(void) {
	UB y;
	for (y = 0; y < VIDEO_Y_SIZE; ++y) {
		UB x;
		for (x = 0; x < VIDEO_X_SIZE; ++x) {
			video_write(x, y, ' ');
		}
	}
}
void video_putc(char c) {
	switch (c) {
	case '\n':
		video_x = VIDEO_X_SIZE;
		break;
	case '\r':
		video_x = 0;
		break;
	default:
		video_write(video_x, video_y, c);
		++video_x;
		break;
	}
	if (video_x >= VIDEO_X_SIZE) {
		UB x;
		video_x = 0;
		++video_y;
		if (video_y >= VIDEO_Y_SIZE) {
			video_y = 0;
		}
		for (x = 0; x < VIDEO_X_SIZE; ++x) {
			video_write(x, video_y, ' ');
		}
	}
}
#endif

FP int_table[0x10]; /* 割込みハンドラのテーブル */

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	set_gate_descriptor(0x20, 0x8, interrupt0, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x21, 0x8, interrupt1, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x22, 0x8, interrupt2, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x23, 0x8, interrupt3, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x24, 0x8, interrupt4, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x25, 0x8, interrupt5, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x26, 0x8, interrupt6, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x27, 0x8, interrupt7, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x28, 0x8, interrupt8, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x29, 0x8, interrupt9, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x2a, 0x8, interrupt10, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x2b, 0x8, interrupt11, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x2c, 0x8, interrupt12, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x2d, 0x8, interrupt13, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x2e, 0x8, interrupt14, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(0x2f, 0x8, interrupt15, I386_TYPE_GATE_INTR, 0);

	irc_initialize();
#if SYS_PUTC_PORTID >= 1
	pcat_com_init(SYS_PUTC_PORTID);
#else
	video_init();
#endif
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
	while(1);
}

ER irc_dis_irq(UB irq)
{
	BOOL cpu_locked;

	if (irq > 15) {
		return E_PAR;
	}

	cpu_locked = sense_lock();

	if(!cpu_locked)
	{
		x_lock_cpu();
	}

	if(irq < 8)
	{
		sil_wrb_iop((VP)0x21, sil_reb_iop((VP)0x21) | (1 << irq));
	}
	else
	{
		sil_wrb_iop((VP)0xA1, sil_reb_iop((VP)0xA1) | (1 << (irq - 8)));
	}

	if(!cpu_locked)
	{
		x_unlock_cpu();
	}

	return E_OK;
}

ER irc_ena_irq(UB irq)
{
	BOOL cpu_locked;

	if (irq > 15) {
		return E_PAR;
	}

	cpu_locked = sense_lock();

	if(!cpu_locked)
	{
		x_lock_cpu();
	}

	if(irq < 8)
	{
		sil_wrb_iop((VP)0x21, sil_reb_iop((VP)0x21) & ~(1 << irq));
	}
	else
	{
		sil_wrb_iop((VP)0xA1, sil_reb_iop((VP)0xA1) & ~(1 << (irq - 8)));
	}

	if(!cpu_locked)
	{
		x_unlock_cpu();
	}

	return E_OK;
}

#if SYS_PUTC_PORTID >= 1
#define SYS_PUT_CHAR(c) pcat_com_putc(SYS_PUTC_PORTID, (c))
#else
#define SYS_PUT_CHAR(c) video_putc(c)
#endif

void
sys_putc(char c)
{
    if (c == '\n') {
        SYS_PUT_CHAR('\r');
    }
    SYS_PUT_CHAR(c);
}


void
define_inh(INHNO inhno, FP inthdr)
{
#if 0
	if (inhno >= 256 || inthdr == NULL)
	{
		return; /*??? Should I generate assertion? */
	}
#endif
	int_table[TO_INTNO(inhno)] = inthdr;
}

ER vdef_inh(INHNO inhno, const T_DINH *pk_dinh) {
	BOOL locked;
	if (!(0x20 <= inhno && inhno <= 0x2f)) {
		return E_PAR;
	}
	if (pk_dinh->inhatr != TA_HLNG) {
		return E_RSATR;
	}
	locked = sense_lock();
	if (!locked) {
		x_lock_cpu();
	}
	define_inh(inhno, pk_dinh->inthdr);
	if (!locked) {
		x_unlock_cpu();
	}
	return E_OK;
}
