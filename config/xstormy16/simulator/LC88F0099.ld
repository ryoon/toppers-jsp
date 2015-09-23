OUTPUT_FORMAT("elf32-xstormy16", "elf32-xstormy16", "elf32-xstormy16")
OUTPUT_ARCH(xstormy16)
ENTRY(_start)
GROUP( -lc -lgcc -lm -liberty)

INPUT( crti.o )
INPUT( crtbegin.o )
INPUT( crtend.o )
INPUT( crtn.o )

                                /*************************************/
                                /** memory region definition        **/
                                /*************************************/
__malloc_start = 0x7800;
MEMORY
{
    RAM (w)    : ORIGIN = 0x00000, LENGTH = 0x7800
    CHIP (r)   : ORIGIN = 0x07f00, LENGTH = 0x100
    ROM (!w)   : ORIGIN = 0x08000, LENGTH = 0x78000
    BIOS (!w)  : ORIGIN = 0x7FF00+0x100-0x1000, LENGTH = 0x1000
    BIVEC (!w) : ORIGIN = 0x7FF00-0x100, LENGTH = 0x100
    OPTION (r) : ORIGIN = 0x7FF00, LENGTH = 0x100
}
                                /*************************************/
                                /** program header definition       **/
                                /*************************************/
PHDRS {
	RAM  PT_LOAD FLAGS(6) ;
	CHIP PT_LOAD FLAGS(4) ;
	ROM  PT_LOAD FLAGS(5) ;
	DROM PT_LOAD FLAGS(6) ;
	CROM PT_LOAD FLAGS(4) ;
	PROM PT_LOAD FLAGS(5) ;
}

SECTIONS
{
								/*************************************/
								/**	.bss_below100 section   		**/
								/*************************************/
	.bss_below100 0x0002 (NOLOAD) : {
		__bss_start = .;
        *(.bss_below100)
        *(.bss_below100.*)
        . = ALIGN(2);
	} > RAM :RAM
								/*************************************/
								/**	.data_below100 section  		**/
								/*************************************/
	.data_below100 : AT ( __rdata ) {
		__data_start = . ;
        *(.data_0)
        *(.data_below100)
        *(.data_below100.*)
        . = ALIGN(2);
	} > RAM :DROM =0
                                /*************************************/
                                /** .data section                   **/
                                /*************************************/
    .data : AT ( __rdata + SIZEOF(.data_below100) ) {
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d.*)
        SORT(CONSTRUCTORS)
        . = ALIGN(2);
		__data_end = . ;
    } > RAM :DROM =0
                                /*************************************/
                                /** .bss section                    **/
                                /*************************************/
    .bss (NOLOAD) : {
        *(.dynbss)
        *(.bss)
        *(.bss.*)
        *(.gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(2);
		__bss_end = .;
    } > RAM :RAM
    _end = .;
                                /*************************************/
                                /** define top of stack             **/
                                /*************************************/
    __stack = .;
                                /*************************************/
                                /** sections for target chip info   **/
                                /*************************************/
    .chip_info : {
        KEEP(*(.chip_info))
    } > CHIP :CHIP =0
                                /*************************************/
                                /** reset/interrupt vectors at 8000 **/
                                /*************************************/
    .int_vec : {
        KEEP(*(.int_vec))
    } > ROM :ROM =0
                                /*************************************/
                                /** read only section               **/
                                /*************************************/
    .rodata : {
        *(.rodata)
        *(.rodata.*)
        *(.gnu.linkonce.r.*)
    } > ROM :ROM =0
                                /*************************************/
                                /** construcrtors/destructors       **/
                                /*************************************/
    .ctors : {
        KEEP (*crtbegin.o(.ctors))
        KEEP (*(EXCLUDE_FILE (*crtend.o ) .ctors))
        KEEP (*(SORT(.ctors.*)))
        KEEP (*(.ctors))
    } > ROM :ROM =0
   .dtors : {
        KEEP (*crtbegin.o(.dtors))
        KEEP (*(EXCLUDE_FILE (*crtend.o ) .dtors))
        KEEP (*(SORT(.dtors.*)))
        KEEP (*(.dtors))
    } > ROM :ROM =0
                                /*************************************/
                                /** stab for the pointer to function**/
                                /*************************************/
    .plt : {
        *(.plt)
    } > ROM :ROM =0
                                /*************************************/
                                /** other information               **/
                                /*************************************/
    .jcr : {
        KEEP (*(.jcr))
    } > ROM :ROM =0
    .eh_frame : {
        KEEP (*(.eh_frame))
    } > ROM :ROM =0
    .gcc_except_table : {
        KEEP (*(.gcc_except_table))
    } > ROM :ROM =0
                                /*************************************/
                                /** .text section                   **/
                                /*************************************/
    .text : {
        *(.text)
        *(.text.*)
        *(.stub)
        *(.gnu.warning)
        *(.gnu.linkonce.t.*)
    } > ROM :ROM =0
                                /*************************************/
                                /** .init/.fini section             **/
                                /*************************************/
    .init : {
        KEEP (*crti.o(.init))
        KEEP (*(EXCLUDE_FILE (*crtn.o ) .init))
        KEEP (*(SORT(.init.*)))
        KEEP (*(.init))
    } > ROM :ROM =0
    .fini : {
        KEEP (*crti.o(.fini))
        KEEP (*(EXCLUDE_FILE (*crtn.o ) .fini))
        KEEP (*(SORT(.fini.*)))
        KEEP (*(.fini))
    } > ROM :ROM =0
                                /*************************************/
                                /** external data                   **/
                                /*************************************/
    .ex_data : {
        *(.ex_data)
    } > ROM :ROM =0
                                /*************************************/
                                /** initialize value for .data      **/
                                /*************************************/
    .data_init (NOLOAD) : {
        __rdata = . ;
		. += SIZEOF(.data_below100) ;
        . += SIZEOF(.data) ;
    } > ROM
                                /*************************************/
                                /** CG-ROM                          **/
                                /*************************************/
    .cg_rom ALIGN(256) : {
        KEEP(*(.cg_rom))
    } > ROM :CROM =0
								/*************************************/
								/**	unused ROM area 				**/
								/*************************************/
	__etext = .;
	__erom = 0x7FF00+0x100-0x1000;
                                /*************************************/
                                /** .data section for BIOS          **/
                                /*************************************/
    .bios_data 0 : {
        *(.bios.data)
        *(.bios.data.*)
    }
                                /*************************************/
                                /** .text section for BIOS          **/
                                /*************************************/
    .bios_text 0x7FF00+0x100-0x1000 : {
        *(.bios.text)
        *(.bios.text.*)
    } > BIOS :PROM =0
                                /*************************************/
                                /** .text section for Vector for BIOS*/
                                /*************************************/
    .bios_vect 0x7FF00-0x100 : {
        KEEP(*(.bios.vect))
    } > BIVEC :PROM =0
                                /*************************************/
                                /** sections for user option data   **/
                                /*************************************/
    .user_option 0x7FF00 : {
        KEEP(*(.user_option))
    } > OPTION :PROM =0
                                /*************************************/
                                /** Stab debugging sections         **/
                                /*************************************/
    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }
                                /*************************************/
                                /** DWARF debug sections            **/
                                /*************************************/
    /* Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
    /* DWARF 1 */
    .debug 0 : { *(.debug) }
    .line 0 : { *(.line) }
    /* GNU DWARF 1 extensions */
    .debug_srcinfo 0 : { *(.debug_srcinfo) }
    .debug_sfnames 0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2 */
    .debug_aranges 0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2 */
    .debug_info 0 : { *(.debug_info) *(.gnu.linkonce.wi.*) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_line 0 : { *(.debug_line) }
    .debug_frame 0 : { *(.debug_frame) }
    .debug_str 0 : { *(.debug_str) }
    .debug_loc 0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    .debug_ranges   0 : { *(.debug_ranges) }
    /* SGI/MIPS DWARF 2 extensions */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
}
