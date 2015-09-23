/*
 *  @(#) $Id: excalibur.ld,v 1.4 2003/07/01 15:59:15 honda Exp $
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)
	
MEMORY {
	/*  0x00000000 -> 0x0001FFFF */
	rom1		: o = 0x00000000, l = 0x20000

	rom2		: o = 0x00020000, l = 0x20000

	/* use 16k for .stack, .data and .bss */
	in_ram		: o = 0x40000000, l = 0x4000

	ex_ram		: o = 0x80000000, l = 0x50000000

}

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE (software_term_hook = 0);
/*PROVIDE(sbi_data_start = 0);*/

SECTIONS
{
    . = 0x0;
    .vector : {
        *(.vector)
    } >in_ram AT>rom1
    .text :	
    {
        __text = . ; 	
	    start.o(.text)
        *(.text)
        *(.glue_7t)
        *(.glue_7)
    } >rom1

    _etext = .	;
    PROVIDE (etext = .)	;

    .rodata : { 
	*(.rodata) 
        *(.rodata*)

    } >rom1

	. = ALIGN(4);
    __idata_start = . ;
    .data  : {
        __data_start = .;
        *(.data)
	*(.sbi)
    }  >in_ram AT>rom1
    __idata_end = __idata_start + SIZEOF(.data);

    _edata  =  . ;
    PROVIDE (edata = .);

    .bss ALIGN(4) :
    {
    __bss_start = .;
      *(.bss)
   . = ALIGN(32 / 8);
    } >in_ram

   . = ALIGN(32 / 8);
     _end = .		;
    __bss_end = .;
    PROVIDE (end = .)	;
    .comment 0 : { *(.comment) }	

  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to 
     the beginning of the section so we begin them at 0.  */

  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }

  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }

  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }

  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }

  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
}	

	
		
	
