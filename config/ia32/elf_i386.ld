OUTPUT_FORMAT("elf32-i386", "elf32-i386",
	      "elf32-i386")
OUTPUT_ARCH(i386)

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

STARTUP(start.o);

SECTIONS
{
  .text      :
  {
    *(.text)
  } =0x9090
  _etext = .;
  PROVIDE (etext = .);
  .rodata   : { *(.rodata) }
  . = ALIGN(0x1000) + (. & (0x1000 - 1));
  .data    :
  {
    *(.data)
  }
  _edata = .;
  PROVIDE (edata = .);
  __bss_start = .;
  .bss       :
  {
   *(.bss)
   *(COMMON)
   . = ALIGN(32 / 8);
  }
  . = ALIGN(32 / 8);
  _end = .;
  PROVIDE (end = .);
  /* Stabs debugging sections.  */
  .stab 0 : { *(.stab) }
  .stabstr 0 : { *(.stabstr) }
  .stab.excl 0 : { *(.stab.excl) }
  .stab.exclstr 0 : { *(.stab.exclstr) }
  .stab.index 0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment 0 : { *(.comment) }
}
