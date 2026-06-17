/*
 *  @(#) $Id: riscv64elf.ld 127 2025-11-04 15:58:19Z roi $
 */

OUTPUT_ARCH(riscv)

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

SECTIONS
{
    __stack_size = DEFINED(__stack_size) ? __stack_size : 8K;

    .init :
    {
        KEEP (*(.init))
    }
    .text :
    {
        __text = . ;
        *(.text .stub .text.* .gnu.linkonce.t.*)

    }
    _etext = . ;
    PROVIDE (etext = .) ;
    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.srodata .srodata.*)
    }

    . = ALIGN(8);
    __idata_start = . ;
    .data :
    {
        __data_start = . ;
        /* Writable data segment (.data segment) */
        *(.data .data.*)
        *(.sdata .sdata.*)
        *(.sdata2 .sdata2.*)
        . = ALIGN(8);
        PROVIDE( __global_pointer$ = ABSOLUTE(.) + 0x800);
        *(.eh_frame)
        *(.gcc_except_table*)
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
    }
    __idata_end = __idata_start + SIZEOF(.data) ;
    _edata  = . ;
    PROVIDE (edata = .) ;
    . = ALIGN(8) ;
    __bss_start = . ;
    .bss :
    {
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
    }
    _end = . ;
    __bss_end = . ;
    PROVIDE (end = .) ;
    .comment        0 : { *(.comment) }
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
}

