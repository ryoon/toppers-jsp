/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: rp2350.c 2246 2025-12-10 20:41:48Z roi $
 */

/*
 *		RP2350初期化プログラム
 */
#include <s_services.h>

#ifdef TOPPERS_PROJECT_DEFINE
#include "project_define.h"
#else
#define PROGRAM_NAME    "jsp"
#define VERSION_STRING  "1.4.4"
#endif

/*
 *  SIL関数のマクロ定義
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))

#ifndef XOSC_HZ
#define XOSC_HZ                   12000000
#endif

#define CLOCK_SETUP_TIMEOUT       (5*1000*1000)	/* 5sec */
#define PLL_SETUP_TIMEOUT         (5*1000*1000)	/* 5sec */

#define PLL_DIV1_SHIFT  (16)
#define PLL_DIV2_SHIFT  (12)
#define CLOCK_DIV_SHIFT (16)

#ifndef PICO_XOSC_STARTUP_DELAY_MULTIPLIER
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 1
#endif

#define REG_ALIAS_RW        0x00000000
#define REG_ALIAS_XOR       0x00001000
#define REG_ALIAS_SET       0x00002000
#define REG_ALIAS_CLR       0x00003000

/*
 *  CLOCKS
 */
#define TADR_CLOCKS_CLK_HW  0x40010000
#define TADR_CLOCKS_CLK_SYS_RESUS_CTRL  0x40010084	/* (RW) */
#define TOFF_CLOCKS_CLK_CTRL     0x0000
  #define CLOCKS_CLK_SYS_CTRL_SRC    0x00000001	/* Selects the clock source glitchlessly, can be changed */
  #define CLOCKS_CLK_SYS_CTRL_AUXSRC 0x000000E0	/* Selects the auxiliary clock source, will glitch when switching */
  #define CLOCKS_CLK_REF_CTRL_SRC    0x00000003	/* Selects the clock source glitchlessly, can be changed */
  #define CLOCKS_CLK_CTRL_ENABLE     0x00000800	/* ENABLE (0): Starts and stops the clock generator cleanly */
#define TOFF_CLOCKS_CLK_DIV      0x0004
#define TOFF_CLOCKS_CLK_SELECTED 0x0008

/*
 *  RESET HARDWARE
 */
#define TADR_RESETS_RESET       0x40020000	/* (RW) Reset control */
  #define RESETS_RESET              0x1FFFFFFF	/* reset bits */
  #define RESETS_RESET_ADC          0x00000001	/* adc */
  #define RESETS_RESET_HSTX         0x00000008	/* hstx */
  #define RESETS_RESET_IO_QSPI      0x00000080	/* io_qspi */
  #define RESETS_RESET_PADS_QSPI    0x00000400	/* pads_qspi */
  #define RESETS_RESET_PLL_SYS      0x00004000	/* pll_sys */
  #define RESETS_RESET_PLL_USB      0x00008000	/* pll_usb */
  #define RESETS_RESET_SPI0         0x00040000	/* spi0 */
  #define RESETS_RESET_SPI1         0x00080000	/* spi1 */
  #define RESETS_RESET_SYSCFG       0x00100000	/* syscfg */
  #define RESETS_RESET_UART0        0x04000000	/* uart0 */
  #define RESETS_RESET_UART1        0x08000000	/* uart1 */
  #define RESETS_RESET_USBCTRL      0x10000000	/* usbctrl */
#define TADR_RESETS_RESET_DONE  0x40020008	/* (RW) Reset done */

/*
 *  IO BANK0
 */
#define TADR_IO_BANK0_BASE  0x40028000
#define TOFF_IO_BANK0_GPIO_CTRL 0x0004	/* GPIO control including function select and overrides */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART   0x02
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_SIO    0x05
#define GPIO_SIZE               8
#define NUM_BANK0_GPIO          48

#define TOFF_IO_BANK0_IO                 0x0000
  #define IO_BANK0_GPIO_CTRL_OUTOVER           0x00000300	/* drive output registers */
  #define IO_BANK0_GPIO_CTRL_OEOVERS           0x00003000	/* drive output enable registers */

  #define PADS_BANK0_GPIO_PMASK        0x0000000C	/* PUE/PUD mask */

/*
 *  PADS BANK0
 */
#define TADR_PADS_BANK0_BASE 0x40038000
#define TOFF_PADS_BANK0_GPIO    0x0004	/* (RW) Pad control register */
  #define PADS_BANK0_GPIO_IE        0x00000040	/* IE (1): Input enable */
  #define PADS_BANK0_GPIO_OD        0x00000080	/* OD (0): Output disable */
  #define PADS_BANK0_GPIO_ISO       0x00000100	/* Pad isolation control. */

/*
 *  CRYSTAL OSCILLATOR
 */
#define TADR_XOSC_BASE      0x40048000
#define TOFF_XOSC_CTRL          0x0000	/* (RW) Crystal Oscillator Control */
  #define XOSC_CTRL_FREQ_RANGE      0x00000FFF	/* FREQ_RANGE (0): Frequency range */
  #define XOSC_CTRL_ENABLE          0x00FFF000	/* ENABLE (0): On power-up this field is initialised to DISABLE and the chip runs from the ROSC */
  #define XOSC_CTRL_ENABLE_VALUE_DISABLE 0x00D1E000
  #define XOSC_CTRL_ENABLE_VALUE_ENABLE  0x00FAB000
#define TOFF_XOSC_STATUS        0x0004	/* (R)  Crystal Oscillator Status */
  #define XOSC_STATUS_FREQ_RANGE    0x00000003	/* FREQ_RANGE (0): The current frequency range setting, always reads 0 */
  #define XOSC_STATUS_ENABLED       0x00001000	/* ENABLED (0): Oscillator is enabled but not necessarily running and stable, resets to 0 */
  #define XOSC_STATUS_BADWRITE      0x01000000	/* BADWRITE (0): An invalid value has been written to CTRL_ENABLE or CTRL_FREQ_RANGE or DORMANT */
  #define XOSC_STATUS_STABLE        0x80000000	/* STABLE (0): Oscillator is running and stable */
#define TOFF_XOSC_STARTUP       0x000C		/* (RW) Controls the startup delay */
  #define XOSC_STARTUP_DELAY        0x00003FFF	/* DELAY (0xc4): in multiples of 256*xtal_period */
  #define XOSC_STARTUP_X4           0x00100000	/* X4 (0): Multiplies the startup_delay by 4 */

/*
 *  PLL
 */
#define TADR_PLL_SYS_BASE   0x40050000
#define TADR_PLL_USB_BASE   0x40058000
#define TOFF_PLL_CS             0x0000		/* (RW) Control and Status */
  #define PLL_CS_REFDIV             0x0000003F	/* REFDIV (1): Divides the PLL input reference clock */
  #define PLL_CS_BYPASS             0x00000100	/* BYPASS (0): Passes the reference clock to the output instead of the divided VCO */
  #define PLL_CS_LOCK               0x80000000	/* LOCK (0): PLL is locked */
#define TOFF_PLL_PWR            0x0004		/* (RW) Controls the PLL power modes */
  #define PLL_PWR_PD                0x00000001	/* PD (1): PLL powerdown */
  #define PLL_PWR_DSMPD             0x00000004	/* DSMPD (1): PLL DSM powerdown */
  #define PLL_PWR_POSTDIVPD         0x00000008	/* POSTDIVPD (1): PLL post divider powerdown */
  #define PLL_PWR_VCOPD             0x00000020	/* VCOPD (1): PLL VCO powerdown */
#define TOFF_PLL_FBDIV_INT      0x0008		/* (RW) Feedback divisor */
  #define PLL_FBDIV_INT             0x00000FFF	/* FBDIV_INT (0): see ctrl reg description for constraints */
#define TOFF_PLL_PRIM           0x000C		/* Controls the PLL post dividers for the primary output */
  #define PLL_PRIM_POSTDIV2         0x00007000	/* POSTDIV2 (0x7): divide by 1-7 */
  #define PLL_PRIM_POSTDIV1         0x00070000	/* POSTDIV1 (0x7): divide by 1-7 */

/*
 *  TICKS
 */
#define TADR_TICKS_BASE     0x40108000
#define TOFF_TICKS_CTRL         0x0000	/* Controls the tick generator */
  #define TICKS_CTRL_ENABLE         0x00000001	/* start / stop tick generation */
  #define TICKS_CTRL_RUNNING        0x00000002	/* Is the tick generator running? */
#define TOFF_TICKS_CYCLES       0x0004	/* PROC0_CYCLES */
#define TOFF_TICKS_COUNT        0x0008	/* PROC0_COUNT */

#define TICKS_HW_SIZE   12
#define NUM_TICKS       6

/*
 *  CMSIS-M33+
 */
#define TADR_PPB_BASE       0xE0000000
  #define M33_ACTLR_EXTEXCLALL    0x20000000
#define TOFF_M33_ACTLR          0xE008		/* (RW) Provides IMPLEMENTATION DEFINED configuration and control options */
#define TOFF_M33_CPACR          0xED88		/* (RW) Coprocessor Access Control Register */

/*
 *  USB
 */
#define TADR_USB_SIE_CTRL       0x5011004C	/* (RW) SIE control register */
  #define USB_SIE_CTRL_RESET            0x00008000
  #define USB_SIE_CTRL_TRANSCEIVER_PD   0x00040000	/* Power down bus transceiver */

  #define M33_CPACR_CP0           0x00000003
  #define M33_CPACR_CP4           0x00000300
  #define M33_CPACR_CP10          0x00300000


#define XOSC_CTRL_FREQ_RANGE_VALUE_1_15MHZ    0xaa0
#define STARTUP_DELAY ((((XOSC_HZ / 1000) + 128) / 256) * PICO_XOSC_STARTUP_DELAY_MULTIPLIER)

#define EARLY_RESET_VALUE   ~(RESETS_RESET_IO_QSPI | RESETS_RESET_PADS_QSPI | \
                              RESETS_RESET_PLL_USB | RESETS_RESET_USBCTRL   | \
                              RESETS_RESET_SYSCFG  | RESETS_RESET_PLL_SYS)


#define EARLY_UNRESET_VALUE (RESETS_RESET & \
                            ~(RESETS_RESET_HSTX | RESETS_RESET_ADC | \
                              RESETS_RESET_SPI0 | RESETS_RESET_SPI1 | \
                              RESETS_RESET_UART0 | RESETS_RESET_UART1 | \
                              RESETS_RESET_USBCTRL))

#define CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC        0x2
#define CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX 0x1


#define CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS  0x0
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS        0x0
#define CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB  0x0
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB  0x0
#define CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB  0x0

typedef void *(*fn)(UW code, UW mask);
typedef void (*rom_bootrom_state_reset_fn)(UW flags);

#define RT_FLAG_FUNC_ARM_SEC            0x0004
#define RT_FLAG_FUNC_ARM_NONSEC         0x0010
#define RT_FLAG_FUNC_RISCV              0x0001

#define BOOTROM_STATE_RESET_CURRENT_CORE 0x01
#define BOOTROM_STATE_RESET_GLOBAL_STATE 0x04

#define BOOTROM_FUNC_TABLE_OFFSET       0x14
#define BOOTROM_IS_A2()                 ((*(volatile UB *)0x13) == 2)
#define BOOTROM_WELL_KNOWN_PTR_SIZE     (BOOTROM_IS_A2() ? 2 : 4)

#ifdef __riscv
#define BOOTROM_ENTRY_OFFSET            0x7dfc
#define BOOTROM_TABLE_LOOKUP_ENTRY_OFFSET (BOOTROM_ENTRY_OFFSET - BOOTROM_WELL_KNOWN_PTR_SIZE)
#define BOOTROM_TABLE_LOOKUP_OFFSET     (BOOTROM_ENTRY_OFFSET - BOOTROM_WELL_KNOWN_PTR_SIZE*2)
#else
#define BOOTROM_TABLE_LOOKUP_OFFSET     (BOOTROM_FUNC_TABLE_OFFSET + BOOTROM_WELL_KNOWN_PTR_SIZE)
#endif

#define BOOTRAM_BASE                    0x400E0000
#define TOFF_BOOTRAM_BOOTLOCK0          0x0000080C
#define BOOTROM_LOCK_ENABLE             7
#define NUM_BOOTRAM_BOOTLOCK            8

#define PICO_DEFAULT_IRQ_PRIORITY       0x80

extern const UW __binary_info_header;
extern const UW __flash_binary_end;

const UW SrcClock[NUM_CLK] = {
	0,					/* GPIO Muxing 0 */
	0,					/* GPIO Muxing 1 */
	0,					/* GPIO Muxing 2 */
	0,					/* GPIO Muxing 3 */
	12000000,			/* Watchdog and timers reference clock */
	150000000,			/* Processors, bus fabric, memory, memory mapped registers */
	150000000,			/* Peripheral clock for UART and SPI */
	150000000,			/* HSTX clock */
	48000000,			/* USB clock */
	48000000			/* ADC clock */
};

static UW configured_freq[NUM_CLK];


inline static int
pico_processor_state_is_nonsecure(void)
{
#ifndef __riscv
	// todo add a define to disable NS checking at all?
	// IDAU-Exempt addresses return S=1 when tested in the Secure state,
	// whereas executing a tt in the NonSecure state will always return S=0.
	UW	tt;
	asm volatile (
		"movs %0, #0\n"
		"tt %0, %0\n"
		: "=r" (tt) : : "cc"
	);
	return !(tt & (1u << 22));
#else
	return FALSE;
#endif
}

/*
 *  バイナリ情報設定
 */
static const char bulid_date[]    = __DATE__;
static const char build_program[] = PROGRAM_NAME;
static const char build_version[] = VERSION_STRING;

static const data_info_t size_info = {
	BINARY_INFO_TYPE_ID_AND_INT,
	('P' << 8 | 'R'),
	BINARY_INFO_ID_RP_BINARY_END,
	(UW)&__flash_binary_end
};

static const string_info_t date_info = {
	BINARY_INFO_TYPE_ID_AND_STRING,
	('P' << 8 | 'R'),
	BINARY_INFO_ID_RP_PROGRAM_BUILD_DATE_STRING,
	(void *)&bulid_date
};

static const string_info_t program_info = {
	BINARY_INFO_TYPE_ID_AND_STRING,
	('P' << 8 | 'R'),
	BINARY_INFO_ID_RP_PROGRAM_NAME,
	(void *)&build_program
};

static const string_info_t version_info = {
	BINARY_INFO_TYPE_ID_AND_STRING,
	('P' << 8 | 'R'),
	BINARY_INFO_ID_RP_PROGRAM_VERSION_STRING,
	(void *)&build_version
};

const void * __binary_info[] __attribute__((section(".binary_info.keep"))) = {
	&size_info,
	&date_info,
	&program_info,
	&version_info
};

/*
 *  PLL設定関数
 */
int
init_PLL(UW base, UW refdiv, UW fbdiv, UW post_div1, UW post_div2)
{
	UW pdiv, pll_reset, tick;

	/*
	 *  fbdivパラメータチェック
	 */
	if(fbdiv < 16 || fbdiv > 320)
		return 0;

	/*
	 *  post_divパラメータチェック
	 */
	if(post_div1 < 1 || post_div1 > 7)
		return 0;
	if(post_div2 < 1 || post_div2 > 7)
		return 0;
	if(post_div2 > post_div1)
		return 0;

	/*
	 *  DIVIDER設定値作成
	 */
	pdiv = (post_div1 << PLL_DIV1_SHIFT) | (post_div2 << PLL_DIV2_SHIFT);

	/*
	 *  同一設定ならば、そのまま、終了
	 */
	if((sil_rew_mem((UW *)(base+TOFF_PLL_CS)) & PLL_CS_LOCK) &&
		(refdiv == (sil_rew_mem((UW *)(base+TOFF_PLL_CS)) & PLL_CS_REFDIV)) &&
		(fbdiv  == (sil_rew_mem((UW *)(base+TOFF_PLL_FBDIV_INT)) & PLL_FBDIV_INT)) &&
		(pdiv   == (sil_rew_mem((UW *)(base+TOFF_PLL_PRIM)) & (PLL_PRIM_POSTDIV1 | PLL_PRIM_POSTDIV2)))){
		return 1;
	}

	/*
	 *  PLLリセット
	 */
	pll_reset = (TADR_PLL_USB_BASE == base) ? RESETS_RESET_PLL_USB : RESETS_RESET_PLL_SYS;
	sil_wrw_mem((UW *)(TADR_RESETS_RESET+REG_ALIAS_SET), pll_reset);
	sil_wrw_mem((UW *)(TADR_RESETS_RESET+REG_ALIAS_CLR), pll_reset);
	tick = 0;
	while((sil_rew_mem((UW *)(TADR_RESETS_RESET_DONE)) & pll_reset) == 0){
		sil_dly_nse(1000);
		if(++tick > PLL_SETUP_TIMEOUT)
			return 0;
	}

	/*
	 *  PLL設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_PLL_CS), refdiv);
	sil_wrw_mem((UW *)(base+TOFF_PLL_FBDIV_INT), fbdiv);

	/*
	 *  電源供給
	 */
	sil_wrw_mem((UW *)(base+TOFF_PLL_PWR+REG_ALIAS_CLR), (PLL_PWR_PD | PLL_PWR_VCOPD));

	/*
	 *  PLLロック待ち
	 */
	tick = 0;
	while((sil_rew_mem((UW *)(base+TOFF_PLL_CS)) & PLL_CS_LOCK) == 0){
		sil_dly_nse(1000);
		if(++tick > PLL_SETUP_TIMEOUT)
			return 0;
	}

	/*
	 *  DIVIDER設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_PLL_PRIM), pdiv);
	sil_wrw_mem((UW *)(base+TOFF_PLL_PWR+REG_ALIAS_CLR), PLL_PWR_POSTDIVPD);
	return 1;
}

/*
 *  クロック設定関数
 */
int
configure_Clock(UB index, UB src, UB auxsrc, UW div)
{
	UW cbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * index;
	UW tick, mask, delay;

	/*
	 *  DIVIDOR設定が大きくなる場合は、事前にDIVIDORを設定
	 */
	if(div > sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_DIV)))
		sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_DIV), div);

	if((index == CLK_SYS || index == CLK_REF) && src == CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX){
		/*
		 *  REF CTRLソースを停止
		 */
		sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_REF_CTRL_SRC);
		tick = 0;
		while(sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_SELECTED)) != 0x1){
			sil_dly_nse(1000);
			if(++tick > CLOCK_SETUP_TIMEOUT)
				return 0;
		}
	}
	else{
		/*
		 *  クロックを停止
		 */
		sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_CTRL_ENABLE);
		if(configured_freq[index] > 0){
			/*
			 *  イネーブルクリアのためのサイクル遅延設定
			 */
			delay = configured_freq[CLK_SYS] / configured_freq[index] + 1;
			sil_dly_nse(delay*SIL_DLY_TIM2);
		}
	}

	/*
	 *  AUX-MUXを設定
	 */
	mask = ((sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL)) ^ (auxsrc << 5)) & CLOCKS_CLK_SYS_CTRL_AUXSRC);
	sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_XOR), mask);

	if(index == CLK_SYS || index == CLK_REF){
		/*
		 *  クロックソースを設定
		 */
		mask = ((sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL)) ^ (src << 0)) & CLOCKS_CLK_REF_CTRL_SRC);
		sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_XOR), mask);
		tick = 0;
		while(!(sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_SELECTED)) & (1u << src))){
			sil_dly_nse(1000);
			if(++tick > CLOCK_SETUP_TIMEOUT)
				return 0;
		}
	}

	/*
	 *  クロックをイネーブルにする
	 */
	sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_SET), CLOCKS_CLK_CTRL_ENABLE);

	/*
	 *  DIVIORを設定
	 */
	sil_wrw_mem((UW *)(cbase+TOFF_CLOCKS_CLK_DIV), div);

	/*
	 *  設定後の周波数を保存する
	 */
	configured_freq[index] = (UW)(((UD) SrcClock[index] << CLOCK_DIV_SHIFT) / div);
	return 1;
}

void
set_configured_freq(UB index, UW hz)
{
	if(index < NUM_CLK)
	    configured_freq[index] = hz;
}

UW
get_Frequency(UB index)
{
	UW cbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * index;
	UW div = sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_DIV));

	if(div == 0)
		div = 256;
	return ((unsigned long long)SrcClock[index] << 8) / div;
}

/*
 *  クロックを取得する
 */
UW
get_Clock(UB index)
{
	return configured_freq[index];
}

/*
 *  実行コアを取得する
 */
UW
get_CoreNo(void)
{
	return ((sil_rew_mem((VP)(TADR_SIO_CPUID)) == 0) ? 0 : 1);
}

/*
 *  バイナリインフォのアドレスを取り出す
 */
const void *
get_Binaryinfo(void)
{
	return (const void *)&__binary_info_header;
}

/*
 *  ROM関数参照関数
 */
void *
rom_func_lookup(UW code)
{
#ifdef __riscv
	fn rom_table_lookup = (fn) (unsigned long)*(UH*)(BOOTROM_TABLE_LOOKUP_ENTRY_OFFSET);
	return rom_table_lookup(code, RT_FLAG_FUNC_RISCV);
#else
	fn rom_table_lookup = (fn) (unsigned long)*(UH*)(BOOTROM_TABLE_LOOKUP_OFFSET);
	if(pico_processor_state_is_nonsecure())
		return rom_table_lookup(code, RT_FLAG_FUNC_ARM_NONSEC);
	else
		return rom_table_lookup(code, RT_FLAG_FUNC_ARM_SEC);
#endif
}

/*
 *  低レベルのターゲット依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される場合がある．
 *      System Clock source            = XOSC(12MHz)
 *      PLL SYS                        = 150000000(12MHz * 125) / 5 / 2 = 150MHz
 *      PLL USB                        = 48000000(12MHz * 100)  / 5 / 5 = 48MHz
 */
void
hardware_init_hook(void)
{
#ifndef TOPPERS_RAM_EXEC

	rom_bootrom_state_reset_fn state_reset = rom_func_lookup('S' | ('R' << 8));
	UW csysbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * CLK_SYS;
	UW crefbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * CLK_REF;
	UW base, addr, tick = 0, i;
#ifndef __riscv
	UW reg;
#endif

	state_reset(BOOTROM_STATE_RESET_GLOBAL_STATE);

	/*
	 *  必要なハードウェアをリセット
	 */
	sil_wrw_mem((UW *)(TADR_RESETS_RESET+REG_ALIAS_SET), EARLY_RESET_VALUE);
	sil_wrw_mem((UW *)(TADR_RESETS_RESET+REG_ALIAS_CLR), EARLY_UNRESET_VALUE);
	while((~(sil_rew_mem((UW *)(TADR_RESETS_RESET_DONE))) & EARLY_UNRESET_VALUE) != 0){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

	/*
	 *  USBパワーダウン
	 */
	if(sil_rew_mem((UW *)(TADR_USB_SIE_CTRL)) == USB_SIE_CTRL_RESET){
		sil_wrw_mem((UW *)(TADR_USB_SIE_CTRL+REG_ALIAS_SET), USB_SIE_CTRL_TRANSCEIVER_PD);
	}

	/*
	 *  RESUSの無効化
	 */
	sil_wrw_mem((UW *)(TADR_CLOCKS_CLK_SYS_RESUS_CTRL), 0);

	/*
	 *  XSOC初期化
	 */
	sil_wrw_mem((UW *)(TADR_XOSC_BASE+TOFF_XOSC_CTRL), XOSC_CTRL_FREQ_RANGE_VALUE_1_15MHZ);
	sil_wrw_mem((UW *)(TADR_XOSC_BASE+TOFF_XOSC_STARTUP), STARTUP_DELAY);
	sil_wrw_mem((UW *)(TADR_XOSC_BASE+TOFF_XOSC_CTRL+REG_ALIAS_SET), XOSC_CTRL_ENABLE_VALUE_ENABLE);
	tick = 0;
	while((sil_rew_mem((UW *)(TADR_XOSC_BASE+TOFF_XOSC_STATUS)) & XOSC_STATUS_STABLE) == 0){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

	/*
	 *  CLK_SYS/CLK_REFのAUXソースからの切り離し
	 */
	sil_wrw_mem((UW *)(csysbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_SYS_CTRL_SRC);
	tick = 0;
	while(sil_rew_mem((UW *)(csysbase+TOFF_CLOCKS_CLK_SELECTED)) != 0x1){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}
	sil_wrw_mem((UW *)(crefbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_REF_CTRL_SRC);
	tick = 0;
	while(sil_rew_mem((UW *)(crefbase+TOFF_CLOCKS_CLK_SELECTED)) != 0x1){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

	/*
	 *  PLLを起動
	 */
	init_PLL(TADR_PLL_SYS_BASE, 1, 125, 5, 2);
	init_PLL(TADR_PLL_USB_BASE, 1, 100, 5, 5);

	/*
	 *  CLK_REF(12MHz/1 = 12MHz)
	 */
	configure_Clock(CLK_REF, CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC, 0, (1<<CLOCK_DIV_SHIFT));

	/*
	 *  CLK_SYS(150MHz/1 = 150MHz)
	 */
	configure_Clock(CLK_SYS, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
					CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, (1<<CLOCK_DIV_SHIFT));

	/*
	 *  CLK USB(48MHz/1 = 48MHz)
	 */
	configure_Clock(CLK_USB, 0, CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, (1<<CLOCK_DIV_SHIFT));

	/*
	 *  CLK ADC(48MHz/1 = 48MHz)
	 */
	configure_Clock(CLK_ADC, 0, CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, (1<<CLOCK_DIV_SHIFT));

	/*
	 *  CLK_REPI(150MHz/1 = 150MHz)
	 */
	configure_Clock(CLK_PERI, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, (1<<CLOCK_DIV_SHIFT));

	/*
	 *  CLK HSTX(150MHz/1 = 150MHz)
	 */
	configure_Clock(CLK_HSTX, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, (1<<CLOCK_DIV_SHIFT));

	/*
	 *  TICKSをスタート
	 */
	tick = get_Clock(CLK_REF) / 1000000;
	base = TADR_TICKS_BASE;
	for(i = 0 ; i < NUM_TICKS ; i++, base += TICKS_HW_SIZE){
		sil_wrw_mem((UW *)(base+TOFF_TICKS_CYCLES), tick);
		sil_wrw_mem((UW *)(base+TOFF_TICKS_CTRL), TICKS_CTRL_ENABLE);
	}
	sil_dly_nse(1000*1000);

	/*
	 *  全部のハードウェアをリセット
	 */
	tick = 0;
	sil_wrw_mem((UW *)(TADR_RESETS_RESET+REG_ALIAS_CLR), RESETS_RESET);
	while (~(sil_rew_mem((UW *)(TADR_RESETS_RESET_DONE))) & RESETS_RESET){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

	/*
	 *  BOOT LOCKSリセット
	 */
	addr = (BOOTRAM_BASE + TOFF_BOOTRAM_BOOTLOCK0);
	for(i = 0 ; i < NUM_BOOTRAM_BOOTLOCK ; i++, addr += 4){
		sil_wrw_mem((UW *)addr, 0);
	}

	/*
	 *  BOOT LOCKSイネーブル待ち
	 */
	while(sil_rew_mem((UW *)(BOOTRAM_BASE + TOFF_BOOTRAM_BOOTLOCK0+(BOOTROM_LOCK_ENABLE*4))) == 0){
	}
#ifndef __riscv
	asm volatile ("dmb 0xF":::"memory");
#else
	asm volatile ("fence.i" : : : "memory");
#endif

	/*
	 *  BOOTROMプレコアリセット
	 */
	state_reset(BOOTROM_STATE_RESET_CURRENT_CORE);

#ifndef __riscv
	/*
	 *  M33コプロセッサ有効化
	 */
	reg = M33_CPACR_CP10 | M33_CPACR_CP4 | M33_CPACR_CP0;
	sil_orw_mem((UW *)(TADR_PPB_BASE+TOFF_M33_CPACR), reg);
	asm volatile ("mrc p4,#0,r0,c0,c0,#1" : : : "r0"); // clear engaged flag via RCMP

	/*
	 *  M33MPUのExternal Exclusivesを許可する
	 */
	sil_orw_mem((UW *)(TADR_PPB_BASE+TOFF_M33_ACTLR), M33_ACTLR_EXTEXCLALL);
#endif

#else	/* TOPPERS_RAM_EXEC */

	UW cbase, div;
	UW index;

	for(index = 0 ; index < NUM_CLK ; index++){
		cbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * index;
		div = sil_rew_mem((UW *)(cbase+TOFF_CLOCKS_CLK_DIV));
		if(div == 0)
			div = 1 << CLOCK_DIV_SHIFT;
		configured_freq[index] = ((UD)SrcClock[index] << CLOCK_DIV_SHIFT) / div;
	}

#endif	/* TOPPERS_RAM_EXEC */
}

