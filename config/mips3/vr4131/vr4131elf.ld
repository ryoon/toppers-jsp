OUTPUT_FORMAT("elf32-littlemips","elf32-littlemips","elf32-littlemips")
OUTPUT_ARCH(mips)

PROVIDE(_stack_top = 0);
PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);

SECTIONS
{
	_rom_base = 0xbfc00700;		/* ROM 焼き用として固定値を取ること。 */
	_stack_top = 0x8c03fffc;	/* = 0x8c040000 - 0x4 (SRAM) ; kseg1 */
/*	_stack_top = 0xac03fffc;*/	/* = 0xac040000 - 0x4 (SRAM) ; kseg0 */

	.exce_vec_map 0xbfc00000 :
	{
		*(.exce_vec_map)
	}

	.reset 0xbfc00400 :		/* キャッシュ初期化などを含む */
	{
		*(.reset)
	}

	.text 0x9fc00700 :		/* kseg1で利用する場合 */
/*	.text 0xbfc00700 :*/		/* kseg0で利用する場合 */
	AT( _rom_base )
	{
		_stext = . ;
		*(.text)
		_etext  = . ;
	}

	_rom_base += SIZEOF(.text);

	.rodata . :
	AT( _rom_base )
	{
		*(.rodata)
	}

	__idata_start = _rom_base + SIZEOF(.rodata);

	.data 0xa0200000 :	/* DATAセクションのスタートアドレス
				   SDRAM の最初から取っていないのは、最初の2MB分は
				   ROM のミラー用に確保するため */
	AT ( __idata_start )
	{
		__data_start  = . ;
		*(.data)
	}

	_gp = . + 0x8000;	/* 理由は良く分からないけれども、
				   こう書くのが一般的らしい。 */

	.sdata . :
	AT ( __idata_start + SIZEOF(.data) )
	{
		*(.lit8) 
		*(.lit4) 
		*(.sdata) 
	}
	__idata_end = __idata_start + SIZEOF(.data) + SIZEOF(.sdata);

	__bss_start = .	;
	.sbss : 
	{
		_fbss = . ;
		*(.sbss)
		*(.scommon)
	}

	.bss : 
	{
		*(.bss)
		*(COMMON)
		__bss_end = .;
		_end = . ;
	}

	PROVIDE (end = .);

	.comment 	0 : { *(.comment) }
	.debug          0 : { *(.debug) }
	.line           0 : { *(.line) }
	.debug_srcinfo  0 : { *(.debug_srcinfo) }
	.debug_sfnames  0 : { *(.debug_sfnames) }
	.debug_aranges  0 : { *(.debug_aranges) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
}

PROVIDE(memcpy = _dummy_memcpy);
