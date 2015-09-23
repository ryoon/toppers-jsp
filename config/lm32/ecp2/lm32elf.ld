/* 
 *  @(#) $Id: lm32elf.ld,v 1.14 2007/07/27 11:28:44 honda Exp $
 */



OUTPUT_FORMAT("elf32-lm32")
ENTRY(reset)

/*
 * This section defines memory attributes (name, origin, length) for the platform
 */
MEMORY
{
    sram  :  ORIGIN = 0x02000000,  LENGTH = 1048576
}

SECTIONS
{

  /* code */
  .boot : { *(.boot) } > sram
  .text : 
  {
    . = ALIGN(4);
     _ftext = .;
     *(.text .stub .text.* .gnu.linkonce.t.*)
     *(.gnu.warning)
     KEEP (*(.init))
     KEEP (*(.fini))

     /* Exception handlers */
     *(.eh_frame_hdr)
     KEEP (*(.eh_frame))
     *(.gcc_except_table)

     /* Constructors and destructors */
     KEEP (*crtbegin*.o(.ctors))
     KEEP (*(EXCLUDE_FILE (*crtend*.o ) .ctors))
     KEEP (*(SORT(.ctors.*)))
     KEEP (*(.ctors))
     KEEP (*crtbegin*.o(.dtors))
     KEEP (*(EXCLUDE_FILE (*crtend*.o ) .dtors))
     KEEP (*(SORT(.dtors.*)))
     KEEP (*(.dtors))
     KEEP (*(.jcr))
    _etext = .;
  } > sram =0 

  /* read-only data */
  .rodata : 
  { 
     . = ALIGN(4);
     _frodata = .;
     _frodata_rom = LOADADDR(.rodata);
     *(.rodata .rodata.* .gnu.linkonce.r.*) 
     *(.rodata1)
     _erodata = .;
  } > sram

  /* read/write data */
  .data : 
  {
     . = ALIGN(4);
     _fdata = .;
     *(.data .data.* .gnu.linkonce.d.*)
     *(.data1)
     SORT(CONSTRUCTORS)
     _gp = ALIGN(16) + 0x7ff0;
     *(.sdata .sdata.* .gnu.linkonce.s.*)
     _edata = .;
  } > sram

  /* bss */
  .bss : 
  { 
     . = ALIGN(4);
     _fbss = .;
     *(.dynsbss)
     *(.sbss .sbss.* .gnu.linkonce.sb.*)
     *(.scommon)
     *(.dynbss)
     *(.bss .bss.* .gnu.linkonce.b.*)
     *(COMMON)
     . = ALIGN(4);
     _ebss = .;
     _end = .;
     PROVIDE (end = .);
  } > sram

  /* first location in stack is highest address in ram */
  PROVIDE(_fstack = ORIGIN(sram) + LENGTH(sram) - 4);

  /* stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }

  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
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
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
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

