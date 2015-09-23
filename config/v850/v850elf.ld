/*
 *  @(#) $Id: v850elf.ld,v 1.1 2000/11/24 03:41:29 takayuki Exp $
 */

OUTPUT_FORMAT("elf32-v850","elf32-v850","elf32-v850")
OUTPUT_ARCH(v850)

PROVIDE(__stack = 0);
PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
SECTIONS
{
	.vector : AT(0)
	{
		*(.vector)
	}
    .text :	
    {
        __text = . ;
        *(.text)

    }
    _etext = .	;
    PROVIDE (etext = .)	;
    .rodata : { *(.rodata) }
	_textend = .;

    . = 0x00ffb000;
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
