/*
 *  @(#) $Id: m68kelf.ld,v 1.1 2000/11/14 14:44:14 hiro Exp $
 */

OUTPUT_FORMAT("elf32-m68k","elf32-m68k","elf32-m68k")
OUTPUT_ARCH(m68k)

PROVIDE(__stack = 0);
PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
SECTIONS
{
    .text :	
    {
        __text = . ;
        *(.text)

    }
    _etext = .	;
    PROVIDE (etext = .)	;
    .rodata : { *(.rodata) }
    . = ALIGN(4);
    __idata_start = . ;
    .data   :  AT(__idata_start) 
    {
        __data_start = . ;
        *(.data)
    }
    __idata_end = __idata_start + SIZEOF(.data);
    _edata  =  . ;
    PROVIDE (edata = .);
    . = ALIGN(4);
    __bss_start = .	;		
    .bss       :
    {

      *(.bss)
      *(COMMON)
    }
    _end = .		;
    PROVIDE (end = .)	;
    .comment 0 : { *(.comment) }	
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }	
}
