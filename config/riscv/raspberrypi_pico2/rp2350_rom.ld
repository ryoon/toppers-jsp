/*
 *  @(#) $Id: rp2350_rom.ld 127 2025-10-22 10:39:41Z roi $
 */

OUTPUT_ARCH( "riscv" )

MEMORY
{ 
	/* Run in FLASH */ 
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
    } >ROM AT>ROM 

    .text :
    {
        __text = . ;
        *(.text.unlikely .text.unlikely.*)
        *(.text)
        *(.text.*)
	    . = ALIGN(64);
        *(*vector)
        *(.gnu.linkonce.t.*)

    } >ROM AT>ROM 

    _etext = . ;
    PROVIDE (etext = .) ;

    .rodata :
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.srodata .srodata.*)
    } >ROM AT>ROM 

    /* Machine inspectable binary information */
    . = ALIGN(4);
    __binary_info_start = .;
    .binary_info :
    {
        KEEP(*(.binary_info.keep.*))
        *(.binary_info.*)
    } > ROM AT>ROM 
    __binary_info_end = .;

    . = ALIGN(4) ;
    __idata_start = . ;
    .data : 
    {
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
    } >RAM AT>ROM

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
    end = _end ;

    /* Start and end symbols must be word-aligned */
    .scratch_x : {
        __scratch_x_start__ = .;
        *(.scratch_x.*)
        . = ALIGN(4);
        __scratch_x_end__ = .;
    } > SCRATCH_X AT > ROM
    __scratch_x_source__ = LOADADDR(.scratch_x);

    .scratch_y : {
        __scratch_y_start__ = .;
        *(.scratch_y.*)
        . = ALIGN(4);
        __scratch_y_end__ = .;
    } > SCRATCH_Y AT > ROM
    __scratch_y_source__ = LOADADDR(.scratch_y);

    .flash_end : {
        KEEP(*(.embedded_end_block*))
        PROVIDE(__flash_binary_end = .);
    } > ROM =0xaa

    __StackTop = __scratch_y_start__ + SIZEOF(.scratch_y);

    .comment        0 : { *(.comment) }
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
}
