/*
 *  @(#) $Id: lpc2300.ld,v 1.1 2006/06/11 12:18:56 roi Exp $
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)

MEMORY
{
         vector_rom(wxai) : ORIGIN = 0, LENGTH = 64
				rom(wxai) : ORIGIN = 64, LENGTH = 256K - 64
                ram(wxai) : ORIGIN = 1024M, LENGTH = 64K
}

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

SECTIONS
{

.vector :
    {
        * (*.vector *.vector.*)
        . = ALIGN(16);
    } > vector_rom

.text :
    {
        . = ALIGN(16);
        * (*.text *.text.* *.glue*)
        . = ALIGN(16);
        * (*.rodata *.rodata.*)
        . = ALIGN(16);
    /* added 15-apr-2008 by rwk */
        * (*.gnu.linkonce.* .gcc_except_table)
        . = ALIGN(16);
    } > rom

  . = ALIGN(4);
.data :
    {
        __data_start = . ;
        *(.data)
    } > ram
     AT> rom

__data_start = ADDR(.data);
__idata_start = LOADADDR(.data); 
__idata_end = __idata_start + SIZEOF(.data);

/* bss is zero initialised data */
.bss :
    {
        * (*.bss *.bss.*)
        . = ALIGN(16);
    } > ram
     AT> rom

__bss_start = ADDR(.bss);
__bss_end   = (ADDR(.bss) + SIZEOF(.bss));

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

	
		
	