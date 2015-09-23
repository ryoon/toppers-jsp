/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
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
 *  @(#) $Id: cpu_config.c,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

/*
 *	プロセッサ依存モジュール（i386用）
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include <i386.h>

UW nest = 0; /* 割り込みネスト回数 */
FP exc_table[0x20]; /* CPU例外ハンドラのテーブル */

static SEGDESC gdt[3];    /* グローバルディスクリプタテーブル */
static GATEDESC idt[256]; /* 割り込みディスクリプタテーブル */

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize()
{
	DESCPTR desc_ptr;

	/* Flat model */
	set_segment_descriptor(0, 0x0, 0x00000, 0x00, 0);
	set_segment_descriptor(1, 0x0, 0xfffff, I386_TYPE_SEG_CODE | I386_TYPE_SEG_READABLE, 0);
	set_segment_descriptor(2, 0x0, 0xfffff, I386_TYPE_SEG_DATA | I386_TYPE_SEG_WRITEABLE, 0);

	desc_ptr.limit = sizeof(gdt) - 1;
	desc_ptr.base = (unsigned)gdt;

	Asm("movl	%0, %%eax": : "g"(&desc_ptr) : "eax");
	Asm("lgdt	(%eax)		\n"
	    "ljmp	$0x8, $csinit	\n" /* パイプラインに先読みした命令をクリアする */
	    "csinit:			\n"
	    "movw	$0x10, %ax	\n"
	    "movw	%ax, %ds	\n"
	    "movw	%ax, %es	\n"
	    "movw	%ax, %fs	\n"
	    "movw	%ax, %gs	\n"
	    "movw	%ax, %ss");

	set_gate_descriptor( 0, 0x8, exception0,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 1, 0x8, exception1,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 2, 0x8, exception2,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 3, 0x8, exception3,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 4, 0x8, exception4,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 5, 0x8, exception5,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 6, 0x8, exception6,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 7, 0x8, exception7,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 8, 0x8, exception8,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor( 9, 0x8, exception9,  I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(10, 0x8, exception10, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(11, 0x8, exception11, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(12, 0x8, exception12, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(13, 0x8, exception13, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(14, 0x8, exception14, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(16, 0x8, exception16, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(17, 0x8, exception17, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(18, 0x8, exception18, I386_TYPE_GATE_INTR, 0);
	set_gate_descriptor(19, 0x8, exception19, I386_TYPE_GATE_INTR, 0);

	desc_ptr.limit = sizeof(idt) - 1;
	desc_ptr.base = (unsigned)idt;

	Asm("movl	%0, %%eax	" : : "g"(&desc_ptr) : "eax");
	Asm("lidt	(%eax)		");

}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
}


/*
 * i386セグメントデスクリプタの設定
 */
void
set_segment_descriptor(int sel, unsigned addr, unsigned limit, unsigned type, unsigned dpl)
{
	SEGDESC *desc;
	desc = &gdt[sel];
	desc->base_L = (addr & 0xffffff);
	desc->base_H = ((addr >> 24) & 0xff);
	desc->limit_L = (limit & 0xffff);
	desc->limit_H = ((limit >> 16) & 0xf);
	desc->type = (type & 0xf);
	desc->s = 1;		/* code/data segment */
	desc->dpl = dpl;
	desc->p = 1;		/* segment is existing */
	desc->avl = 0;		/* unused */
	desc->x = 0;		/* reserved */
	desc->db = 1;		/* 32bits segment */
	desc->g = 1;		/* 4KB limits */
}

void
set_gate_descriptor(int sel, unsigned selector, void *func, unsigned type, unsigned dpl)
{
	GATEDESC *desc;
	desc = &idt[sel];
	desc->offset_L = ((unsigned)func & 0xffff);
	desc->offset_H = (((unsigned)func >> 16) & 0xffff);
	desc->selector = selector;
	desc->copy = 0;
	desc->type = type;
	desc->s = 0;
	desc->dpl = dpl;
	desc->p = 1;
}


void
define_exc(EXCNO excno, FP exchdr)
{
#if 0
	if (inhno >= 0x20 || inthdr == NULL)
	{
		return; /*??? Should I generate assertion? */
	}
#endif
	exc_table[excno] = exchdr;
}
