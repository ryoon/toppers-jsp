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
 *  @(#) $Id: om13058.c 698 2025-11-04 10:27:10Z roi $
 */

#include <s_services.h>
#include <t_config.h>

/*
 *  初期化プログラム（OM13058用）
 */

#define sil_modw_mem(addr, mask, val)	sil_wrw_mem((addr), ((sil_rew_mem(addr) & ~(mask)) | (val)))
#define sil_andw_mem(addr, mask)		sil_wrw_mem((addr), (sil_rew_mem(addr) & ~(mask)))
#define sil_orw_mem(addr, val)			sil_wrw_mem((addr), (sil_rew_mem(addr) | (val)))

#define SYSCTL_IRC_FREQ (12000000)
#define OSC_FREQ        (12000000)
#define RTCOSC_FREQ     (32768)

/*
 * PLL設定値
 */
#define PLL_MSEL		3
#define PLL_PSEL        1
#define SYSCLOCKDIVID   1

/*
 *  PDRUNCFGレジスタマスク値
 */
#define PDRUNCFGUSEMASK 0x0000C800
#define PDRUNCFGMASKTMP 0x000025FF

/*
 *  FLASH REGISTERS
 */
#define TADR_FLASH_FLASHTIM     0x4003C010
#define FLASH_FLASHTIM_1CLK_CPU 0x00000000	/* Flash accesses use 1 CPU clocks */
#define FLASH_FLASHTIM_2CLK_CPU 0x00000001	/* Flash accesses use 2 CPU clocks */
#define FLASH_FLASHTIM_3CLK_CPU 0x00000002	/* Flash accesses use 3 CPU clocks */

/*
 *  SYSTEM CONFIGURATION CONTROLLER
 */
#define TADR_SYSCTL_SYSPLLCTRL		0x40048008	/* (RW) System PLL control register */
#define TADR_SYSCTL_SYSPLLSTAT		0x4004800C	/* (R)  System PLL status register */
#define TADR_SYSCTL_WDTOSCCTRL		0x40048024	/* (RW) Watchdog Oscillator control register */
#define TADR_SYSCTL_SYSPLLCLKSEL	0x40048040	/* (RW) System PLL clock source select register */
#define TADR_SYSCTL_SYSPLLCLKUEN	0x40048044	/* (RW) System PLL clock source update enable register*/
#define TADR_SYSCTL_MAINCLKSEL		0x40048070	/* (RW) Main clock source select register */
#define TADR_SYSCTL_MAINCLKUEN		0x40048074	/* (RW) Main clock source update enable register */
#define TADR_SYSCTL_SYSAHBCLKDIV	0x40048078	/* (RW) System Clock divider register */
#define TADR_SYSCTL_SYSAHBCLKCTRL	0x40048080	/* (RW) System clock control register */
#define TADR_SYSCTL_PDRUNCFG		0x40048238	/* (RW) Power configuration register*/

#define WDTLFO_OSC_MAX          16
#define CLOCK_IOCON             0x00010000	/* 16: IOCON block clock */

#define PLLCLKSRC_IRC           0x00	/* Internal oscillator in */
#define PLLCLKSRC_MAINOSC       0x01	/* Crystal (main) oscillator in */
#define PLLCLKSRC_SYSOSC        PLLCLKSRC_MAINOSC
#define PLLCLKSRC_RTC32K        0x03	/* RTC 32KHz clock */
#define PLLCLKSRC_MASK          0x03

#define MAINCLKSRC_IRC          0x00	/* Internal oscillator */
#define MAINCLKSRC_PLLIN        0x01	/* System PLL input */
#define MAINCLKSRC_WDTOSC       0x02	/* Watchdog oscillator rate */
#define MAINCLKSRC_PLLOUT       0x03	/* System PLL output */
#define MAINCLKSRC_MASK         0x03

#define POWERDOWN_SYSOSC_PD     0x00000020	/* System oscillator power down */
#define POWERDOWN_SYSPLL_PD     0x00000080	/* System PLL power down */

/*
 * XTALIN/OUT USB/VBUS モード設定値
 */
#define TADR_IOCON_PIO0         0x40044000
#define TADR_IOCON_PIO2A        0x400440F0
#define XTAL_MODE               0x00000001	/* (IOCON_FUNC1 | IOCON_MODE_INACT) */
#define USBVBUS_MODE            0x00000081	/* (IOCON_FUNC1 | IOCON_MODE_INACT | IOCON_DIGMODE_EN)) */

/*
 * WDT用クロックレートテーブル
 */
static const UW wdtOSCRate[WDTLFO_OSC_MAX] = {
	0,					/* WDT_OSC_ILLEGAL */
	600000,				/* WDT_OSC_0_60 */
	1050000,			/* WDT_OSC_1_05 */
	1400000,			/* WDT_OSC_1_40 */
	1750000,			/* WDT_OSC_1_75 */
	2100000,			/* WDT_OSC_2_10 */
	2400000,			/* WDT_OSC_2_40 */
	2700000,			/* WDT_OSC_2_70 */
	3000000,			/* WDT_OSC_3_00 */
	3250000,			/* WDT_OSC_3_25 */
	3500000,			/* WDT_OSC_3_50 */
	3750000,			/* WDT_OSC_3_75 */
	4000000,			/* WDT_OSC_4_00 */
	4200000,			/* WDT_OSC_4_20 */
	4400000,			/* WDT_OSC_4_40 */
	4600000				/* WDT_OSC_4_60 */
};

UW SystemFrequency;

/*
 * WDTクロックレイトを算出する
 */
UW
get_WDTrate(UW reg)
{
	UW div, clk;

	clk = ((reg >> 5) & 0xF);
	div = reg & 0x1F;
	return wdtOSCRate[clk] / ((div + 1) << 1);
}

/*
 * PLLソースクロックを取得する
 */
UW
get_PLLinclockrate(void)
{
	UW clkRate;

	switch(sil_rew_mem((UW *)(TADR_SYSCTL_SYSPLLCLKSEL)) & PLLCLKSRC_MASK){
	case PLLCLKSRC_IRC:
		clkRate = SYSCTL_IRC_FREQ;
		break;

	case PLLCLKSRC_MAINOSC:
		clkRate = OSC_FREQ;
		break;

	case PLLCLKSRC_RTC32K:
		clkRate = RTCOSC_FREQ;
		break;

	default:
		clkRate = 0;
	}

	return clkRate;
}

/*
 *  起動クロックの取り出し
 */
UW
sysclock_init_value(void)
{
	UW pllin = get_PLLinclockrate();
	UW tmp, sysclock;

	tmp = sil_rew_mem((UW *)(TADR_SYSCTL_SYSAHBCLKDIV));
	switch (sil_rew_mem((UW *)(TADR_SYSCTL_MAINCLKSEL)) & MAINCLKSRC_MASK){
	case MAINCLKSRC_IRC:
		sysclock = SYSCTL_IRC_FREQ / tmp;
		break;

	case MAINCLKSRC_PLLIN:
		sysclock = pllin / tmp;
		break;

	case MAINCLKSRC_WDTOSC:
		sysclock = get_WDTrate(sil_rew_mem((UW *)(TADR_SYSCTL_WDTOSCCTRL))) / tmp;
		break;

	case MAINCLKSRC_PLLOUT:
		sysclock = (((sil_rew_mem((UW *)(TADR_SYSCTL_SYSPLLCTRL)) & 0x1F) +1) * pllin) / tmp;
		break;
	}
	return sysclock;
}

#ifdef TOPPERS_ROM_EXEC

/*
 * ペリフェラルのパワーダウン
 */
static void
peripheral_powerdown(UW powerdownmask)
{
	UW tmp;

	tmp = sil_rew_mem((UW *)(TADR_SYSCTL_PDRUNCFG)) & PDRUNCFGMASKTMP;
	tmp |= (powerdownmask & PDRUNCFGMASKTMP);
	sil_wrw_mem((UW *)(TADR_SYSCTL_PDRUNCFG), (tmp | PDRUNCFGUSEMASK));
}

/*
 * ペリフェラルのパワーアップ
 */
static void
peripheral_powerup(UW powerupmask)
{
	UW tmp;

	tmp = sil_rew_mem((UW *)(TADR_SYSCTL_PDRUNCFG)) & PDRUNCFGMASKTMP;
	tmp &= ~(powerupmask & PDRUNCFGMASKTMP);
	sil_wrw_mem((UW *)(TADR_SYSCTL_PDRUNCFG), (tmp | PDRUNCFGUSEMASK));
}

/*
 *  低レベルのターゲット依存の初期化
 *  最大システムクロック設定
 *  設定値は以下の通り
 *      System Clock source            = 12000000
 *      SYSCLK(Hz)                     = 48000000
 *      SYSCLOCKDIVID                  = 1
 *      PLL_MSEL                       = 3
 *      PLL_PSEL                       = 1
 *      Flash Latency(WS)              = 3 clocks
 *  return  正常終了なら0を返す
 */
/*
 *  低レベルのターゲット依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */
void hardware_init_hook(void)
{
	UW tmp;

	/*
	 *  システムクロックのクロックを入れる
	 */
	sil_orw_mem((UW *)(TADR_SYSCTL_SYSAHBCLKCTRL), CLOCK_IOCON);

	/* PIO2_0 - XTALIN */
	sil_wrw_mem((UW *)(TADR_IOCON_PIO2A+0*4), XTAL_MODE);
	/* PIO2_1 - XTALOUT */
	sil_wrw_mem((UW *)(TADR_IOCON_PIO2A+1*4), XTAL_MODE);
	/* PIO0_3 - USB VBUS */
	sil_wrw_mem((UW *)(TADR_IOCON_PIO0+3*4), USBVBUS_MODE);

	/*
	 * main oscillatorパワーアップ
	 */
	peripheral_powerup(POWERDOWN_SYSOSC_PD);

	/*
	 * 580usの待ち（正しくない）
	 */
	sil_dly_nse(580*1000);

	/*
	 * main oscillatorをPLLの入力に設定
	 */
	sil_wrw_mem((UW *)(TADR_SYSCTL_SYSPLLCLKSEL), PLLCLKSRC_MAINOSC);
	sil_wrw_mem((UW *)(TADR_SYSCTL_SYSPLLCLKUEN), 0);
	sil_wrw_mem((UW *)(TADR_SYSCTL_SYSPLLCLKUEN), 1);

	/*
	 * FLASHアクセスを3クロックに設定
	 */
	tmp = sil_rew_mem((UW *)(TADR_FLASH_FLASHTIM)) & ~3;
	sil_wrw_mem((UW *)(TADR_FLASH_FLASHTIM), (tmp | FLASH_FLASHTIM_3CLK_CPU));

	/*
	 * PLL DIVIDERパワーダウン
	 */
	peripheral_powerdown(POWERDOWN_SYSPLL_PD);

	/* メインクロックのoscillator rateを設定
	 * (FCLKIN = 12MHz) * 4 = 48MHz
	 * MSEL = 3 (this is pre-decremented), PSEL = 1 (for P = 2)
	 * FCLKOUT = FCLKIN * (MSEL + 1) = 12MHz * 4 = 48MHz
	 *  FCCO = FCLKOUT * 2 * P = 48MHz * 2 * 2 = 192MHz (within FCCO range)
	 */
	sil_wrw_mem((UW *)(TADR_SYSCTL_SYSPLLCTRL), (PLL_MSEL | (PLL_MSEL << 5)));

	/*
	 * システムPLLを起動
	 */
	peripheral_powerup(POWERDOWN_SYSPLL_PD);

	/*
	 * PLLクロックのロック待ち
	 */
	while((sil_rew_mem((UW *)(TADR_SYSCTL_SYSPLLSTAT)) & 1) == 0){}

	/*
	 * システムクロックのデバイダーを1に設定
	 */
	sil_wrw_mem((UW *)(TADR_SYSCTL_SYSAHBCLKDIV), SYSCLOCKDIVID);


	/*
	 *  メインクロックをPLLのソースに設定
	 */
	sil_wrw_mem((UW *)(TADR_SYSCTL_MAINCLKSEL), MAINCLKSRC_PLLOUT);
	sil_wrw_mem((UW *)(TADR_SYSCTL_MAINCLKUEN), 0);
	sil_wrw_mem((UW *)(TADR_SYSCTL_MAINCLKUEN), 1);
}
#endif

