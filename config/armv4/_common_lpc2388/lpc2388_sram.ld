/*
 */

OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm","elf32-littlearm") 
OUTPUT_ARCH(arm)

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);
PROVIDE(__data_start = 0);	/* この値はchip_config.cの init_vector中で使う */
PROVIDE(__data_end = 0);

MEMORY
  {
    MEM_VECTOR (XW)   : ORIGIN = 0x00000000, LENGTH = 64
    MEM_FIO (RW)	  : ORIGIN = 0x3FFF8000, LENGTH = 32K
    MEM_SRAM (XW)     : ORIGIN = 0x40000040, LENGTH = 0xFFC0
    MEM_USBSRAM (XW)  : ORIGIN = 0x7FD00000, LENGTH = 16K
    MEM_ETHERSRAM (XW): ORIGIN = 0x7FE00000, LENGTH = 16K
    MEM_EXTSRAM_0 (XW): ORIGIN = 0x80000000, LENGTH = 64K
    MEM_EXTSRAM_1 (XW): ORIGIN = 0x81000000, LENGTH = 64K
    MEM_MMR (RW)	  : ORIGIN = 0xE0000000, LENGTH = 1024M
  }


SECTIONS
{
		/* SRAMで動作する場合、ベクトルの初期値はリマップ領域にデバッガが配置する。 */
    .vector : AT(0x40000000)
    {
        KEEP(start.o(.vector))
    } > MEM_VECTOR
    

    
	.bss.firmmr (NOLOAD) : { chip_mmr.o(.MMR0)} > MEM_FIO
	.bss.firmmr (NOLOAD) : { . = 0x4000; chip_mmr.o(.MMR1)} > MEM_FIO
	.bss.firmmr (NOLOAD) : { . = 0x4010; chip_mmr.o(.MMR2)} > MEM_FIO
	.bss.firmmr (NOLOAD) : { . = 0x4030; chip_mmr.o(.MMR3)} > MEM_FIO
	.bss.firmmr (NOLOAD) : { . = 0x4050; chip_mmr.o(.MMR4)} > MEM_FIO
	.bss.firmmr (NOLOAD) : { . = 0x4070; chip_mmr.o(.MMR5)} > MEM_FIO
	.bss.firmmr (NOLOAD) : { . = 0x4090; chip_mmr.o(.MMR6)} > MEM_FIO

    .text :
    {
        __text = . ; 	
	    start.o(.text)
        *(.text)
        *(.glue_7t)
        *(.glue_7)
    } > MEM_SRAM

    _etext = .	;
    PROVIDE (etext = .)	;
    .rodata : { 
		*(.rodata) 
		*(.rodata.str1.4) 
	} > MEM_SRAM

	__exidx_start = .;
	.ARM.exidx : {
		 *(.ARM.exidx* .gnu.linkonce.armexidx.*) 
	} > MEM_SRAM

    . = ALIGN(4);
    .data :
    {
        __data_start = . ;
        *(.data)
    } > MEM_SRAM
    _edata  =  . ;
    PROVIDE (edata = .);
	
    . = ALIGN(4);
    __bss_start = .	;		
    .bss       :
    {
      *(.bss)
      *(COMMON)
   . = ALIGN(32 / 8);
    } > MEM_SRAM

   . = ALIGN(32 / 8);
     _end = .		;
    __bss_end = __bss_start + SIZEOF(.bss);
    PROVIDE (end = .)	;

	.bss.sysmmr (NOLOAD) : { . = 0x00000000;  chip_mmr.o(.MMR7)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00004000;  chip_mmr.o(.MMR8)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00004070;  chip_mmr.o(.MMR9)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00008000;  chip_mmr.o(.MMR10)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00008070;  chip_mmr.o(.MMR11)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0000C000;  chip_mmr.o(.MMR12)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0000C014;  chip_mmr.o(.MMR17)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0000C01C;  chip_mmr.o(.MMR18)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0000C030;  chip_mmr.o(.MMR19)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00010000;  chip_mmr.o(.MMR20)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00010028;  chip_mmr.o(.MMR25)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00010030;  chip_mmr.o(.MMR26)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00014000;  chip_mmr.o(.MMR27)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00014070;  chip_mmr.o(.MMR28)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00018000;  chip_mmr.o(.MMR29)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00018070;  chip_mmr.o(.MMR30)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0001C000;  chip_mmr.o(.MMR31)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00020000;  chip_mmr.o(.MMR32)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0002001C;  chip_mmr.o(.MMR33)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00024000;  chip_mmr.o(.MMR34)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00024060;  chip_mmr.o(.MMR35)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00028000;  chip_mmr.o(.MMR36)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00028080;  chip_mmr.o(.MMR37)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x000280A4;  chip_mmr.o(.MMR38)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0002C000;  chip_mmr.o(.MMR39)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0002C040;  chip_mmr.o(.MMR40)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00030000;  chip_mmr.o(.MMR41)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00034000;  chip_mmr.o(.MMR42)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0003400C;  chip_mmr.o(.MMR43)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0003C000;  chip_mmr.o(.MMR44)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00040000;  chip_mmr.o(.MMR45)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00044000;  chip_mmr.o(.MMR46)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00048000;  chip_mmr.o(.MMR47)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0005C000;  chip_mmr.o(.MMR48)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00068000;  chip_mmr.o(.MMR49)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0006C000;  chip_mmr.o(.MMR50)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00070000;  chip_mmr.o(.MMR51)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00070070;  chip_mmr.o(.MMR52)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00074000;  chip_mmr.o(.MMR53)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00074070;  chip_mmr.o(.MMR54)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00078000;  chip_mmr.o(.MMR55)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00078014;  chip_mmr.o(.MMR60)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0007801C;  chip_mmr.o(.MMR61)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00078030;  chip_mmr.o(.MMR62)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0007C000;  chip_mmr.o(.MMR63)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0007C014;  chip_mmr.o(.MMR68)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0007C01C;  chip_mmr.o(.MMR69)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0007C030;  chip_mmr.o(.MMR70)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00080000;  chip_mmr.o(.MMR71)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x00088000;  chip_mmr.o(.MMR72)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0008C000;  chip_mmr.o(.MMR73)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0008C048;  chip_mmr.o(.MMR74)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x0008C080;  chip_mmr.o(.MMR75)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC000;  chip_mmr.o(.MMR76)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC040;  chip_mmr.o(.MMR77)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC080;  chip_mmr.o(.MMR78)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC0C0;  chip_mmr.o(.MMR79)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC104;  chip_mmr.o(.MMR80)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC140;  chip_mmr.o(.MMR81)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC180;  chip_mmr.o(.MMR82)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC1A0;  chip_mmr.o(.MMR83)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC1A8;  chip_mmr.o(.MMR84)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x001FC1C0;  chip_mmr.o(.MMR85)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00000;  chip_mmr.o(.MMR86)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00040;  chip_mmr.o(.MMR87)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00100;  chip_mmr.o(.MMR88)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00158;  chip_mmr.o(.MMR89)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00170;  chip_mmr.o(.MMR90)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00200;  chip_mmr.o(.MMR91)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00210;  chip_mmr.o(.MMR92)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00FE0;  chip_mmr.o(.MMR93)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00FF4;  chip_mmr.o(.MMR94)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE00FFC;  chip_mmr.o(.MMR95)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE04000;  chip_mmr.o(.MMR96)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE04100;  chip_mmr.o(.MMR97)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE04120;  chip_mmr.o(.MMR98)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08000;  chip_mmr.o(.MMR99)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08020;  chip_mmr.o(.MMR100)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08030;  chip_mmr.o(.MMR101)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08080;  chip_mmr.o(.MMR102)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08100;  chip_mmr.o(.MMR103)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08120;  chip_mmr.o(.MMR104)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08140;  chip_mmr.o(.MMR105)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08160;  chip_mmr.o(.MMR106)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08200;  chip_mmr.o(.MMR107)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08220;  chip_mmr.o(.MMR108)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08240;  chip_mmr.o(.MMR109)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE08260;  chip_mmr.o(.MMR110)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C000;  chip_mmr.o(.MMR111)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C100;  chip_mmr.o(.MMR112)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C110;  chip_mmr.o(.MMR113)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C200;  chip_mmr.o(.MMR114)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C280;  chip_mmr.o(.MMR115)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C2A0;  chip_mmr.o(.MMR116)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0C300;  chip_mmr.o(.MMR117)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FE0CFF4;  chip_mmr.o(.MMR119)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FFFF000;  chip_mmr.o(.MMR120)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FFFF100;  chip_mmr.o(.MMR121)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FFFF200;  chip_mmr.o(.MMR122)} > MEM_MMR
	.bss.sysmmr (NOLOAD) : { . = 0x1FFFFF00;  chip_mmr.o(.MMR123)} > MEM_MMR
}	

	
		
	