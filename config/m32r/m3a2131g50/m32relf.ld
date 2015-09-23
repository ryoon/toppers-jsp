/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: m32relf.ld,v 1.2 2002/04/05 09:01:29 takayuki Exp $
 */

OUTPUT_FORMAT("elf32-m32r","elf32-m32r","elf32-m32r")
OUTPUT_ARCH(m32r)

PROVIDE(__stack = 0x00f10000);
PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(_unhandled_interrupt_handler = 0);

MEMORY
{
	ROM(raxi) : org = 0x00f00000, len = 64k
	RAM(waxi) : org = 0x01000000, len = 8M
}

SECTIONS
{
	.vector : 
	{	*(.vector)		} > ROM

	. = ALIGN(4);
    .text :	
    {
        __stext = . ;
        *(.text)
		__etext = . ;
    } > ROM

	. = ALIGN(4);
    .rodata : 
	{
		__srodata = .;
		 *(.rodata)
		__erodata = .;
	} > ROM

	. = ALIGN(4);
	__sidata = . ;

    .data   :  AT(__sidata) 
    {
		. = ALIGN(4);
		__sdata = .;
        *(.data)
		. = ALIGN(4);
		__edata = .;
    } > RAM

	__eidata = __edata - __sdata + __sidata;

    .bss :
    {
		. = ALIGN(4);
		__sbss = .;
		*(.bss)
		*(COMMON)
		. = ALIGN(4);
		__ebss = .;
    } > RAM


    .comment 0 : { *(.comment) }	
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }	
}
