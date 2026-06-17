/*
 *  @(#) $Id: rp2350_rom.ld 127 2025-10-22 10:39:05Z roi $
 */

OUTPUT_ARCH( "riscv" )

MEMORY
{ 
	/* Run in RAM */ 
	ROM (rxai!w) : ORIGIN = 0x10000000, LENGTH = 4096K
	RAM (wxa!ri) : ORIGIN = 0x20000000, LENGTH = 512K 
    SCRATCH_X(rwx) : ORIGIN = 0x20080000, LENGTH = 4k
    SCRATCH_Y(rwx) : ORIGIN = 0x20081000, LENGTH = 4k
}

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

SECTIONS
{
    .init :
    {
        PROVIDE(__binary_info_header = .);
        KEEP (*(.binary_info_header))
        __binary_info_header_end = .;
        KEEP (*(.init))
    } >RAM AT>RAM 

    .text :
    {
        __text = . ;
        *(.text.unlikely .text.unlikely.*)
        *(.text)
        *(.text.*)
	    . = ALIGN(64);
        *(*vector)
        *(.gnu.linkonce.t.*)

    } >RAM AT>RAM 

    _etext = . ;
    PROVIDE (etext = .) ;

    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.srodata .srodata.*)
        . = ALIGN(4);
    } >RAM AT>RAM 

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } >RAM
    __binary_info_end = .;

    . = ALIGN(4) ;
    .flash_end : {
        KEEP(*(.embedded_end_block*))
        PROVIDE(__flash_binary_end = .);
    } > RAM =0xaa

    . = ALIGN(4) ;
    .data : 
    {
	    __idata_start = . ;
        __data_start = . ;
        *(.rdata) 
        /* Writable data segment (.data segment) */
        *(.time_critical*)
        *(.data .data.*)
        *(.sdata .sdata.*)
        *(.sdata2 .sdata2.*)
        . = ALIGN(8);
        PROVIDE( __global_pointer$ = . + 0x800 );
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
    } >RAM AT>RAM

    __idata_end = __idata_start + SIZEOF(.data) ;
    _edata  = . ;
    PROVIDE (edata = .) ;
    . = ALIGN(4) ;
    __bss_start = . ;

    .bss :
    {
        *(.bss .bss.*)
        *(.sbss .sbss.*)
        *(COMMON)
    } >RAM AT>RAM 
    _end = . ;
    __bss_end = . ;
    PROVIDE (end = .) ;

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

    __StackTop  = __scratch_y_start__ + SIZEOF(.scratch_y);

    .comment        0 : { *(.comment) }
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
}
