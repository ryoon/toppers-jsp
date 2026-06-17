/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: rp2040.c 698 2025-11-22 18:43:47Z roi $
 */

#include <s_services.h>
#include <t_config.h>

/*
 *  初期化プログラム（raspberrypi_pico用）
 */

#ifndef XOSC_HZ
#define XOSC_HZ             12000000UL
#endif

#if !defined(PLLS_FBDIV) || defined(TOPPERS_RAM_EXEC)
#define PLLS_FBDIV          125
#endif
#if !defined(PLLS_PDIV1) || defined(TOPPERS_RAM_EXEC)
#define PLLS_PDIV1          6
#endif
#if !defined(PLLS_PDIV2) || defined(TOPPERS_RAM_EXEC)
#define PLLS_PDIV2          2
#endif

#ifndef SIL_DLY_TIM2
#define SIL_DLY_TIM2        32
#endif

#define CLOCK_SETUP_TIMEOUT (5*1000*1000)	/* 5sec */
#define PLL_SETUP_TIMEOUT   (5*1000*1000)	/* 5sec */

#define PLL_DIV1_SHIFT      (16)
#define PLL_DIV2_SHIFT      (12)


#define REG_ALIAS_RW        0x00000000
#define REG_ALIAS_XOR       0x00001000
#define REG_ALIAS_SET       0x00002000
#define REG_ALIAS_CLR       0x00003000

/*
 *  SIO
 */
#define TADR_SIO_FIFO_ST    0xD0000050	/* (R)  Status register for inter-core FIFOs (mailboxes) */
  #define SIO_FIFO_ST_VLD             0x00000001	/* VLD (0): Value is 1 if this core's RX FIFO is not empty (i */
  #define SIO_FIFO_ST_RDY             0x00000002	/* RDY (1): Value is 1 if this core's TX FIFO is not full (i */
  #define SIO_FIFO_ST_WOF             0x00000004	/* WOF (0): Sticky flag indicating the TX FIFO was written when full */
  #define SIO_FIFO_ST_ROE             0x00000008	/* ROE (0): Sticky flag indicating the RX FIFO was read when empty */
#define TADR_SIO_FIFO_WR    0xD0000054	/* (W)  Write access to this core's TX FIFO */
#define TADR_SIO_FIFO_RD    0xD0000058	/* (R)  Read access to this core's RX FIFO */

/*
 *  RESET HARDWARE
 */
#define TADR_RESETS_RESET   0x4000C000	/* (RW) Reset control */
  #define RESETS_RESET_ADC            0x00000001	/* adc */
  #define RESETS_RESET_IO_QSPI        0x00000040	/* io_qspi */
  #define RESETS_RESET_PADS_QSPI      0x00000200	/* pads_qspi */
  #define RESETS_RESET_PLL_SYS        0x00001000	/* pll_sys */
  #define RESETS_RESET_PLL_USB        0x00002000	/* pll_usb */
  #define RESETS_RESET_RTC            0x00008000	/* rtc */
  #define RESETS_RESET_SPI0           0x00010000	/* spi0 */
  #define RESETS_RESET_SPI1           0x00020000	/* spi1 */
  #define RESETS_RESET_UART0          0x00400000	/* uart0 */
  #define RESETS_RESET_UART1          0x00800000	/* uart1 */
  #define RESETS_RESET_USBCTRL        0x01000000	/* usbctrl */
#define TADR_RESETS_RESET_DONE  0x4000C008	/* (RW) Reset done */

/*
 *  PSM
 */
#define TADR_PSM_FRCE_ON    0x40010000	/* (RW) Force block out of reset (i.e. power it on) */
  #define PSM_FRCE_ON_PROC1           0x00010000	 /* proc1 (0) */
#define TADR_PSM_DONE       0x4001000C	/* (R)  Indicates the peripheral's registers are ready to access. */
  #define PSM_DONE_PROC1              0x00010000	 /* proc1 (0) */

/*
 *  CLOCKS
 */
#define TADR_CLOCKS_CLK_HW  0x40008000
#define TADR_CLOCKS_CLK_SYS_RESUS_CTRL  0x40008078

#define TOFF_CLOCKS_CLK_CTRL      0x0000
  #define CLOCKS_CLK_SYS_CTRL_SRC     0x00000001	/* Selects the clock source glitchlessly, can be changed */
  #define CLOCKS_CLK_SYS_CTRL_AUXSRC  0x000000E0	/* Selects the auxiliary clock source, will glitch when switching */
  #define CLOCKS_CLK_REF_CTRL_SRC     0x00000003	/* Selects the clock source glitchlessly, can be changed */
  #define CLOCKS_CLK_REF_CTRL_AUXSRC  0x00000060	/* Selects the auxiliary clock source, will glitch when switching */
  #define CLOCKS_CLK_CTRL_AUXSRC      0x000001E0	/* AUXSRC (0): Selects the auxiliary clock source, will glitch when switching */
  #define CLOCKS_CLK_CTRL_KILL        0x00000400	/* KILL (0): Asynchronously kills the clock generator */
  #define CLOCKS_CLK_CTRL_ENABLE      0x00000800	/* ENABLE (0): Starts and stops the clock generator cleanly */
  #define CLOCKS_CLK_CTRL_DC50        0x00001000	/* DC50 (0): Enables duty cycle correction for odd divisors */
  #define CLOCKS_CLK_CTRL_PHASE       0x00030000	/* PHASE (0): This delays the enable signal by up to 3 cycles of the input clock */
  #define CLOCKS_CLK_CTRL_NUDGE       0x00100000	/* NUDGE (0): An edge on this signal shifts the phase of the output by 1 cycle of the input clock */
#define TOFF_CLOCKS_CLK_DIV       0x0004
#define TOFF_CLOCKS_CLK_SELECTED  0x0008
#define CLK_HW_SIZE         12

/*
 *  CRYSTAL OSCILLATOR
 */
#define TADR_XOSC_CTRL      0x40024000		/* (RW) Crystal Oscillator Control */
  #define XOSC_CTRL_ENABLE_VALUE_DISABLE 0x00D1E000
  #define XOSC_CTRL_ENABLE_VALUE_ENABLE  0x00FAB000
#define TADR_XOSC_STATUS    0x40024004		/* (R)  Crystal Oscillator Status */
  #define XOSC_STATUS_STABLE          0x80000000	/* STABLE (0): Oscillator is running and stable */
#define TADR_XOSC_STARTUP   0x4002400C		/* (RW) Controls the startup delay */

/*
 *  PLL
 */
#define TADR_PLL_SYS_BASE   0x40028000
#define TADR_PLL_USB_BASE   0x4002C000
#define TOFF_PLL_CS               0x0000	/* (RW) Control and Status */
  #define PLL_CS_REFDIV               0x0000003F	/* REFDIV (1): Divides the PLL input reference clock */
  #define PLL_CS_BYPASS               0x00000100	/* BYPASS (0): Passes the reference clock to the output instead of the divided VCO */
  #define PLL_CS_LOCK                 0x80000000	/* LOCK (0): PLL is locked */
#define TOFF_PLL_PWR              0x0004	/* (RW) Controls the PLL power modes */
  #define PLL_PWR_PD                  0x00000001	/* PD (1): PLL powerdown */
  #define PLL_PWR_DSMPD               0x00000004	/* DSMPD (1): PLL DSM powerdown */
  #define PLL_PWR_POSTDIVPD           0x00000008	/* POSTDIVPD (1): PLL post divider powerdown */
  #define PLL_PWR_VCOPD               0x00000020	/* VCOPD (1): PLL VCO powerdown */
#define TOFF_PLL_FBDIV_INT        0x0008	/* (RW) Feedback divisor */
  #define PLL_FBDIV_INT               0x00000FFF	/* FBDIV_INT (0): see ctrl reg description for constraints */
#define TOFF_PLL_PRIM             0x000C	/* Controls the PLL post dividers for the primary output */
  #define PLL_PRIM_POSTDIV2           0x00007000	/* POSTDIV2 (0x7): divide by 1-7 */
  #define PLL_PRIM_POSTDIV1           0x00070000	/* POSTDIV1 (0x7): divide by 1-7 */

/*
 *  WATCHDOG
 */
#define TADR_WATCHDOG_TICK  0x4005802C		/* (RW) Controls the tick generator */
  #define WATCHDOG_TICK_CYCLES        0x000001FF	/* CYCLES (0): Total number of clk_tick cycles before the next tick */
  #define WATCHDOG_TICK_ENABLE        0x00000200	/* ENABLE (1): start / stop tick generation */
  #define WATCHDOG_TICK_RUNNING       0x00000400	/* RUNNING (0): Is the tick generator running? */
  #define WATCHDOG_TICK_COUNT         0x000FF800	/* COUNT (0): Count down timer: the remaining number clk_tick cycles before the next tick is generated */


#ifndef PICO_XOSC_STARTUP_DELAY_MULTIPLIER
#define PICO_XOSC_STARTUP_DELAY_MULTIPLIER 1
#endif

#define XOSC_CTRL_FREQ_RANGE_VALUE_1_15MHZ    0xaa0
#define STARTUP_DELAY ((((XOSC_HZ / 1000) + 128) / 256) * PICO_XOSC_STARTUP_DELAY_MULTIPLIER)

#define RESET_VALUE     ~(RESETS_RESET_IO_QSPI | RESETS_RESET_PADS_QSPI | \
						 RESETS_RESET_PLL_USB | RESETS_RESET_PLL_SYS)
#define UNRESET_VALUE   (0x01FFFFFF & ~(RESETS_RESET_ADC | RESETS_RESET_RTC | \
						 RESETS_RESET_SPI0 | RESETS_RESET_SPI1 | RESETS_RESET_UART0 | \
						 RESETS_RESET_UART1 | RESETS_RESET_USBCTRL))

#define CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC        0x2
#define CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX 0x1


#define CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS  0x0
#define CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS        0x0
#define CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB  0x0
#define CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB  0x0
#define CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB  0x0

const unsigned int SrcClock[NUM_CLK] = {
	0,					/* GPIO Muxing 0 */
	0,					/* GPIO Muxing 1 */
	0,					/* GPIO Muxing 2 */
	0,					/* GPIO Muxing 3 */
	12000000,			/* Watchdog and timers reference clock */
						/* Processors, bus fabric, memory, memory mapped registers */
	((XOSC_HZ * PLLS_FBDIV) / PLLS_PDIV1 / PLLS_PDIV2),
						/* Peripheral clock for UART and SPI */
	((XOSC_HZ * PLLS_FBDIV) / PLLS_PDIV1 / PLLS_PDIV2),
	48000000,			/* USB clock */
	48000000,			/* ADC clock */
	48000000			/* Real time clock */
};

static UW configured_freq[NUM_CLK];
UW SystemFrequency;

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
		sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_DIV), div);

    if((index == CLK_SYS || index == CLK_REF) && src == CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX){
		/*
		 *  REF CTRLソースを停止
		 */
		sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_REF_CTRL_SRC);
		tick = 0;
		while(sil_rew_mem((VP)(cbase+TOFF_CLOCKS_CLK_SELECTED)) != 0x1){
			sil_dly_nse(1000);
			if(++tick > CLOCK_SETUP_TIMEOUT)
				return 0;
		}
    }
    else{
		/*
		 *  クロックを停止
		 */
		sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_CTRL_ENABLE);
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
	mask = ((sil_rew_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL)) ^ (auxsrc << 5)) & CLOCKS_CLK_SYS_CTRL_AUXSRC);
	sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_XOR), mask);

    if(index == CLK_SYS || index == CLK_REF){
		/*
		 *  クロックソースを設定
		 */
		mask = ((sil_rew_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL)) ^ (src << 0)) & CLOCKS_CLK_REF_CTRL_SRC);
		sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_XOR), mask);
		tick = 0;
        while(!(sil_rew_mem((VP)(cbase+TOFF_CLOCKS_CLK_SELECTED)) & (1u << src))){
			sil_dly_nse(1000);
			if(++tick > CLOCK_SETUP_TIMEOUT)
				return 0;
		}
    }

	/*
	 *  クロックをイネーブルにする
	 */
	sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_SET), CLOCKS_CLK_CTRL_ENABLE);

	/*
	 *  DIVIORを設定
	 */
	sil_wrw_mem((VP)(cbase+TOFF_CLOCKS_CLK_DIV), div);

	/*
	 *  設定後の周波数を保存する
	 */
    configured_freq[index] = (UW)(((unsigned long long) SrcClock[index] << 8) / div);
    return 1;
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

void
mprc_initialize(FP entry, UW *sp, UW vec)
{
    UW c, i = 0;

    /* Boot CPU1 */

    sil_wrw_mem((VP)(TADR_PSM_FRCE_ON), PSM_FRCE_ON_PROC1);
    while((sil_rew_mem((VP)(TADR_PSM_DONE)) & PSM_DONE_PROC1) == 0);

	/* PROC0割込みは許可していない */

    const UW cmd[] =
            {0, 0, 1, (UW) vec, (UW) sp, (UW) entry};

	while(i < sizeof(cmd) / sizeof(cmd[0])){
		c = cmd[i];
		if(c == 0){
			/* flush garbage */
			while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_VLD) != 0){
				(void)sil_rew_mem((VP)(TADR_SIO_FIFO_RD));
			}
			Asm("SEV");
		}
		/* send a command */
		while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_RDY) == 0);
		sil_wrw_mem((VP)(TADR_SIO_FIFO_WR), c);
		Asm("SEV");

		/* receive a response */
		while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_VLD) == 0){
			Asm("WFE");
		}
		if(c == sil_rew_mem((VP)(TADR_SIO_FIFO_RD)))
			i++;
    }
}

#ifndef TOPPERS_RAM_EXEC

/*
 *  PLL設定関数
 */
static int
init_PLL(UW base, UW refdiv, UW fbdiv, UB post_div1, UB post_div2)
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
    if((sil_rew_mem((VP)(base+TOFF_PLL_CS)) & PLL_CS_LOCK) &&
        (refdiv == (sil_rew_mem((VP)(base+TOFF_PLL_CS)) & PLL_CS_REFDIV)) &&
        (fbdiv  == (sil_rew_mem((VP)(base+TOFF_PLL_FBDIV_INT)) & PLL_FBDIV_INT)) &&
        (pdiv   == (sil_rew_mem((VP)(base+TOFF_PLL_PRIM)) & (PLL_PRIM_POSTDIV1 | PLL_PRIM_POSTDIV2)))){
        return 1;
    }

	/*
	 *  PLLリセット
	 */
    pll_reset = (TADR_PLL_USB_BASE == base) ? RESETS_RESET_PLL_USB : RESETS_RESET_PLL_SYS;
	sil_wrw_mem((VP)(TADR_RESETS_RESET+REG_ALIAS_SET), pll_reset);
	sil_wrw_mem((VP)(TADR_RESETS_RESET+REG_ALIAS_CLR), pll_reset);
	tick = 0;
	while((sil_rew_mem((VP)(TADR_RESETS_RESET_DONE)) & pll_reset) == 0){
		sil_dly_nse(1000);
		if(++tick > PLL_SETUP_TIMEOUT)
			return 0;
	}

	/*
	 *  PLL設定
	 */
	sil_wrw_mem((VP)(base+TOFF_PLL_CS), refdiv);
	sil_wrw_mem((VP)(base+TOFF_PLL_FBDIV_INT), fbdiv);

	/*
	 *  電源供給
	 */
	sil_wrw_mem((VP)(base+TOFF_PLL_PWR+REG_ALIAS_CLR), (PLL_PWR_PD | PLL_PWR_VCOPD));

	/*
	 *  PLLロック待ち
	 */
	tick = 0;
    while((sil_rew_mem((VP)(base+TOFF_PLL_CS)) & PLL_CS_LOCK) == 0){
		sil_dly_nse(1000);
		if(++tick > PLL_SETUP_TIMEOUT)
			return 0;
	}

	/*
	 *  DIVIDER設定
	 */
	sil_wrw_mem((VP)(base+TOFF_PLL_PRIM), pdiv);
	sil_wrw_mem((VP)(base+TOFF_PLL_PWR+REG_ALIAS_CLR), PLL_PWR_POSTDIVPD);
	return 1;
}

#endif	/* TOPPERS_RAM_EXEC */

/*
 *  低レベルのターゲット依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される場合がある．
 *      System Clock source            = XOSC(12MHz)
 *      PLLS_FBDIV                     = 125
 *      PLLS_PDIV1                     = 6
 *      PLLS_PDIV2                     = 2
 *      PLL SYS                        = 150000000(12MHz * 125) / 6 / 2 = 125MHz
 *      PLL USB                        = 48000000(12MHz * 100)  / 5 / 5 = 48MHz
 */
void
hardware_init_hook(void)
{
#ifdef ROM

	UW csysbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * CLK_SYS;
	UW crefbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * CLK_REF;
	UW tick = 0;

	/*
	 *  必要なハードウェアをリセット
	 */
	sil_wrw_mem((VP)(TADR_RESETS_RESET+REG_ALIAS_SET), RESET_VALUE);
	sil_wrw_mem((VP)(TADR_RESETS_RESET+REG_ALIAS_CLR), UNRESET_VALUE);
	while((sil_rew_mem((VP)(TADR_RESETS_RESET_DONE)) & UNRESET_VALUE) == 0){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

	/*
	 *  ウォッチドックスタート
	 */
	sil_wrw_mem((VP)(TADR_WATCHDOG_TICK), ((XOSC_HZ / 1000000) | WATCHDOG_TICK_ENABLE));

	/*
	 *  RESUSの無効化
	 */
	sil_wrw_mem((VP)(TADR_CLOCKS_CLK_SYS_RESUS_CTRL), 0);

	/*
	 *  XSOC初期化
	 */
	sil_wrw_mem((VP)(TADR_XOSC_CTRL), XOSC_CTRL_FREQ_RANGE_VALUE_1_15MHZ);
	sil_wrw_mem((VP)(TADR_XOSC_STARTUP), STARTUP_DELAY);
	sil_wrw_mem((VP)(TADR_XOSC_CTRL+REG_ALIAS_SET), XOSC_CTRL_ENABLE_VALUE_ENABLE);
	tick = 0;
	while((sil_rew_mem((VP)(TADR_XOSC_STATUS)) & XOSC_STATUS_STABLE) == 0){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

    /*
	 *  CLK_SYS/CLK_REFのAUXソースからの切り離し
	 */
	sil_wrw_mem((VP)(csysbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_SYS_CTRL_SRC);
	tick = 0;
	while(sil_rew_mem((VP)(csysbase+TOFF_CLOCKS_CLK_SELECTED)) != 0x1){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}
	sil_wrw_mem((VP)(crefbase+TOFF_CLOCKS_CLK_CTRL+REG_ALIAS_CLR), CLOCKS_CLK_REF_CTRL_SRC);
	tick = 0;
	while(sil_rew_mem((VP)(crefbase+TOFF_CLOCKS_CLK_SELECTED)) != 0x1){
		sil_dly_nse(1000);
		if(++tick > CLOCK_SETUP_TIMEOUT)
			return;
	}

	/*
	 *  PLLを起動
	 */
	init_PLL(TADR_PLL_SYS_BASE, 1, PLLS_FBDIV, PLLS_PDIV1, PLLS_PDIV2);
	init_PLL(TADR_PLL_USB_BASE, 1, 100, 5, 5);

	/*
	 *  CLK_REF(12MHz/1 = 12MHz)
	 */
	configure_Clock(CLK_REF, CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC, 0, (1<<8));

	/*
	 *  CLK_SYS(125MHz/1 = 125MHz)
	 */
	configure_Clock(CLK_SYS, CLOCKS_CLK_SYS_CTRL_SRC_VALUE_CLKSRC_CLK_SYS_AUX,
					CLOCKS_CLK_SYS_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS, (1<<8));

	/*
	 *  CLK USB(48MHz/1 = 48MHz)
	 */
	configure_Clock(CLK_USB, 0, CLOCKS_CLK_USB_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, (1<<8));

	/*
	 *  CLK ADC(48MHz/1 = 48MHz)
	 */
	configure_Clock(CLK_ADC, 0, CLOCKS_CLK_ADC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, (1<<8));

	/*
	 *  CLK RTC(48MHz/1024 = 46875Hz)
	 */
	configure_Clock(CLK_RTC, 0, CLOCKS_CLK_RTC_CTRL_AUXSRC_VALUE_CLKSRC_PLL_USB, (1024<<8));

	/*
	 *  CLK_REPI(125MHz/1 = 125MHz)
	 */
	configure_Clock(CLK_PERI, 0, CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLK_SYS, (1<<8));

	sil_dly_nse(1000*1000);

#else	/* ROM */

	UW cbase, div;
	UW index;

	for(index = 0 ; index < NUM_CLK ; index++){
		cbase = TADR_CLOCKS_CLK_HW + CLK_HW_SIZE * index;
		div = sil_rew_mem((VP)(cbase+TOFF_CLOCKS_CLK_DIV));
		if(div == 0)
			div = 256;
		configured_freq[index] = ((UD)SrcClock[index] << 8) / div;
	}

#endif	/* ROM */
}

