OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300h)

ENTRY("_start")

MEMORY {
	/* for mode 5: 0x000000 -> 0xffffff */
	vectors		: o = 0xffc000, l = 0x000100	/* for debug */

	/* use 1M for .text and .rodata */
	ex_ram		: o = 0x400000, l = 0x100000

	/* use 1M for .data and .bss  */
	ex_ram5		: o = 0x500000, l = 0x100000

	/* use 16k for .stack  */
	in_ram		: o = 0xffbf20, l = 0x004000

	}

PROVIDE(_hardware_init_hook = 0);
PROVIDE(_software_init_hook = 0);
PROVIDE(_software_term_hook = 0);
SECTIONS {

	.vectors : {
		*(.vectors)
		} >vectors

	.text :
	{
		__text = . ;
		*(.text)
	} >ex_ram
	_etext = .	;
	PROVIDE (etext = .)	;

	. = ALIGN(4);
	.rodata : {
		. = ALIGN(4);
		*(.rodata)
	} >ex_ram

	. = ALIGN(4);
	__idata_start = . ;
	.data   :  AT(__idata_start)
	{
		__data_start = . ;
		*(.data)
	} >ex_ram5
	__idata_end = __idata_start + SIZEOF(.data);
	_edata  =  . ;
	PROVIDE (edata = .);

	. = ALIGN(4);
	__bss_start = .	;
	.bss       :
	{
		*(.bss)
		*(COMMON)
	} >ex_ram5
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

PROVIDE(_memcpy = _local_memcpy);
