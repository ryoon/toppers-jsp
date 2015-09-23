OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300h)

ENTRY("_start")

MEMORY {
	/* for mode 5: 0x00000 -> 0xfffff */
        vectors		: o = 0x00000, l = 0x00100

	/* use 128k - 100 as the main ROM size.  */
	rom		: o = 0x00100, l = 0x1ff00

	/* use 384k as the main RAM size.  */
	ram		: o = 0x20000, l = 0x60000

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
	} >rom
	_etext = .	;
	PROVIDE (etext = .)	;

	.rodata : {
		*(.rodata)
	} >rom

	. = ALIGN(4);
	__idata_start = . ;
	.data   :  AT(__idata_start)
	{
		__data_start = . ;
		*(.data)
	} >ram
	__idata_end = __idata_start + SIZEOF(.data);
	_edata  =  . ;
	PROVIDE (edata = .);

	. = ALIGN(4);
	__bss_start = .	;
	.bss       :
	{
	  *(.bss)
	  *(COMMON)
	} >ram
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
