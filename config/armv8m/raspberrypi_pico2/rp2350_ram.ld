MEMORY
{
    ROM (rx) : ORIGIN = 0x10000000, LENGTH = 4096K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 512K
    SCRATCH_X(rwx) : ORIGIN = 0x20080000, LENGTH = 4k
    SCRATCH_Y(rwx) : ORIGIN = 0x20081000, LENGTH = 4k
}

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);
ENTRY(_kernel_start)

SECTIONS
{
    .text :
    {  
        __text = . ; 
        KEEP(*(.vector))
        PROVIDE(__binary_info_header = .);
        KEEP (*(.binary_info_header))
        __binary_info_header_end = .;
        KEEP (*(.reset))
        *(.text*)
        *(.glue_7t)
        *(.glue_7)
    } > RAM
    _etext = .	;
    PROVIDE (etext = .)	;

    .rodata : { 
        *(.rodata*) 
    } > RAM

    . = ALIGN(4);
    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
        . = ALIGN(4);
    } > RAM
    .ARM :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
    } > RAM

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } > RAM
    __binary_info_end = .;
    . = ALIGN(4);

    .flash_end : {
        KEEP(*(.embedded_end_block*))
        PROVIDE(__flash_binary_end = .);
    } > RAM =0xaa

    . = ALIGN(4);
    .data   :  AT(__idata_start) 
    {
        __data_start = . ;
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        __preinit_array_start = .;
        *(.preinit_array)
        __preinit_array_end = .;

        __init_array_start = .;
        *(SORT(.init_array.*))
        *(.init_array)
        __init_array_end = .;

        __fini_array_start = .;
        *(SORT(.fini_array.*))
        *(.fini_array)
        __fini_array_end = .;
        . = ALIGN(16);
    } > RAM
    __idata_start = __data_start ;
    __idata_end = __idata_start + SIZEOF(.data);
    _edata  =  . ;
    PROVIDE (edata = .);

    .bss       :
    {
       __bss_start = .	;
      *(.bss*)
      *(COMMON)
      . = ALIGN(32 / 8);
    } > RAM
   . = ALIGN(32 / 8);
     _end = .		;
    __bss_end = .;
    PROVIDE (end = .)	;

    /* Start and end symbols must be word-aligned */
    .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } > SCRATCH_X AT > RAM
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } > SCRATCH_Y AT > RAM
    __scratch_y_source__ = LOADADDR(.scratch_y);

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
