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
 *  @(#) $Id: nucleog0b1re.c 698 2025-11-04 10:48:17Z roi $
 */

#include <s_services.h>
#include <t_config.h>

/*
 *  初期化プログラム（NUCLEO-G0B1RE用）
 */

#define sil_modw_mem(addr, mask, val)	sil_wrw_mem((addr), ((sil_rew_mem(addr) & ~(mask)) | (val)))
#define sil_andw_mem(addr, mask)		sil_wrw_mem((addr), (sil_rew_mem(addr) & ~(mask)))
#define sil_orw_mem(addr, val)			sil_wrw_mem((addr), (sil_rew_mem(addr) | (val)))

#define HSI_TIMEOUT_VALUE          (2000U)      /* 2 ms (minimum Tick + 1U) */
#define PLL_TIMEOUT_VALUE          (2000U)      /* 2 ms (minimum Tick + 1U) */
#define MSI_TIMEOUT_VALUE          (2000U)      /* 2 ms (minimum Tick + 1U) */
#define RCC_DBP_TIMEOUT_VALUE      (2000U)		/* 2 ms (minimum Tick + 1)  */
#define RCC_LSE_TIMEOUT_VALUE      (5000000U)	/* 5 s    */
#define CLOCKSWITCH_TIMEOUT_VALUE  (5000000U)	/* 5 s    */

#if !defined(HSE_VALUE)
#define HSE_VALUE                  8000000  /* Value of the External oscillator in Hz */
#endif /* HSE_VALUE */
#if !defined(HSI_VALUE)
#define HSI_VALUE                  16000000	/* Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */
#if !defined  (LSI_VALUE)
#define LSI_VALUE                  32000	/* Value of the Internal Low Speed oscillator in Hz */
#endif /* LSI_VALUE */
#if !defined(LSE_VALUE)
#define LSE_VALUE                  32768	/* Value of the External low oscillator in Hz*/
#endif

/* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLL_M) * PLL_N */
#define PLL_M                      1		/* Possible value 0 and 63 */
#define PLL_N                      8		/* 80 per 64MHz */

/* SYSCLK = PLL_VCO / PLL_P */
#define PLL_P                      2

#define PLL_Q                      2
#define PLL_R                      2	/* Possible value between 2 and 7 */


#define PLLM_SHIFT                 4
#define PLLN_SHIFT                 8
#define PLLR_SHIFT                 29
#define HSIDIV_SHIFT               11
#define ICSCR_HSITRIM_SHIFT        8
#define CFGR_SWS_SHIFT             3
#define CFGR_HPRE_SHIFT            8


/*
 *  ROM/RAMアドレス
 */
#define FLASH_BASE          0x08000000	/* FLASH base address in the alias region */
#define SRAM_BASE           0x20000000	/* SRAM base address in the alias region */

/*
 *  POWER CONTROL
 */
#define TADR_PWR_BASE       0x40007000
#define TOFF_PWR_CR1        0x0000		/* (RW) PWR Power Control Register 1 */
  #define PWR_CR1_VOS        0x00000600	/* Voltage scaling */
  #define PWR_CR1_VOS_0      0x00000200	/* Voltage scaling bit 0 */
  #define PWR_CR1_VOS_1      0x00000400	/* Voltage scaling bit 1 */
#define TOFF_PWR_SR2        0x0014		/* (RW) PWR Power Status Register 2 */
  #define PWR_SR2_FLASH_RDY  0x00000080	/* Flash Ready */
  #define PWR_SR2_REGLPS     0x00000100	/* Regulator Low Power started */
  #define PWR_SR2_REGLPF     0x00000200	/* Regulator Low Power flag    */
  #define PWR_SR2_VOSF       0x00000400	/* Voltage Scaling Flag */
  #define PWR_SR2_PVDO       0x00000800	/* Power voltage detector output */

/*
 *  CLOCK RECOVERY SYSTEM
 */
#define TADR_CRS_BASE       0x40006C00
#define TOFF_CRS_CR         0x0000		/* (RW) CRS ccontrol register */
  #define CRS_CR_CEN         0x00000020	/* Frequency error counter enable */
  #define CRS_CR_AUTOTRIMEN  0x00000040	/* Automatic trimming enable */
  #define CRS_CR_TRIM        0x00003F00	/* HSI48 oscillator smooth trimming */
#define TOFF_CRS_CFGR       0x0004		/* (RW) CRS configuration register */
  #define CRS_CFGR_SYNCSRC   0x30000000	/* SYNC signal source selection */
  #define CRS_CFGR_SYNCSRC_0 0x10000000
  #define CRS_CFGR_SYNCSRC_1 0x20000000

/*
 *  RESET AND CLOCK CONTROL
 */
#define TADR_RCC_BASE       0x40021000
#define TOFF_RCC_CR         0x0000		/* (RW) RCC clock control register */
  #define RCC_CR_HSION             0x00000100	/* Internal High Speed clock enable */
  #define RCC_CR_HSIKERON          0x00000200	/* Internal High Speed clock enable for some IPs Kernel */
  #define RCC_CR_HSIRDY            0x00000400	/* Internal High Speed clock ready flag */
  #define RCC_CR_HSIDIV            0x00003800	/* HSIDIV[13:11] Internal High Speed clock division factor */
  #define RCC_CR_HSEON             0x00010000	/* External High Speed clock enable */
  #define RCC_CR_HSERDY            0x00020000	/* External High Speed clock ready */
  #define RCC_CR_HSEBYP            0x00040000	/* External High Speed clock Bypass */
  #define RCC_CR_CSSON             0x00080000	/* HSE Clock Security System enable */
  #define RCC_CR_HSI48ON           0x00400000	/* RC48 clock enable */
  #define RCC_CR_HSI48RDY          0x00800000	/* RC48 clock ready */
  #define RCC_CR_PLLON             0x01000000	/* System PLL clock enable */
  #define RCC_CR_PLLRDY            0x02000000	/* System PLL clock ready */
#define TOFF_RCC_ICSCR      0x0004		/* (RW) RCC Internal clock sources calibration register */
  #define RCC_ICSCR_HSICAL         0x000000FF	/* HSICAL[7:0] bits */
  #define RCC_ICSCR_HSITRIM        0x00007F00	/* HSITRIM[14:8] bits */
#define TOFF_RCC_CFGR       0x0008		/* (RW) RCC clock configuration register */
  #define RCC_CFGR_SW              0x00000007	/* SW[2:0] bits (System clock Switch) */
  #define RCC_CFGR_SW_0            0x00000001
  #define RCC_CFGR_SW_1            0x00000002
  #define RCC_CFGR_SW_2            0x00000004
  #define RCC_CFGR_SW_HSI          0x00000000	/* HSI selected as system clock */
  #define RCC_CFGR_SW_HSE          0x00000001	/* HSE selected as system clock */
  #define RCC_CFGR_SW_PLL          0x00000002	/* PLL selected as system clock */
  #define RCC_CFGR_SW_LSI          0x00000003	/* LSI selected as system clock */
  #define RCC_CFGR_SW_LSE          0x00000004	/* LSE selected as system clock */
  #define RCC_CFGR_SWS             0x00000038	/* SWS[2:0] bits (System Clock Switch Status) */
  #define RCC_CFGR_SWS_HSI         0x00000000	/* HSI used as system clock */
  #define RCC_CFGR_SWS_HSE         0x00000008	/* HSE used as system clock */
  #define RCC_CFGR_SWS_PLL         0x00000010	/* PLL used as system clock */
  #define RCC_CFGR_SWS_LSI         0x00000018	/* LSI used as system clock */
  #define RCC_CFGR_SWS_LSE         0x00000020	/* LSE used as system clock */
  #define RCC_CFGR_HPRE            0x00000F00	/* HPRE[3:0] bits (AHB prescaler) */
  #define RCC_CFGR_PPRE            0x00007000	/* PRE1[2:0] bits (APB prescaler) */
  #define RCC_CFGR_PPRE_0          0x00001000
  #define RCC_CFGR_PPRE_1          0x00002000
  #define RCC_CFGR_PPRE_2          0x00004000
#define TOFF_RCC_PLLCFGR    0x000C		/* (RW) RCC System PLL configuration Register */
  #define RCC_PLLCFGR_PLLSRC       0x00000003
  #define RCC_PLLCFGR_PLLSRC_HSI   0x00000002	/* HSI source clock selected */
  #define RCC_PLLCFGR_PLLSRC_HSE   0x00000003	/* HSE source clock selected */
  #define RCC_PLLCFGR_PLLM         0x00000070
  #define RCC_PLLCFGR_PLLN         0x00007F00
  #define RCC_PLLCFGR_PLLPEN       0x00010000
  #define RCC_PLLCFGR_PLLP         0x003E0000
  #define RCC_PLLCFGR_PLLQEN       0x01000000
  #define RCC_PLLCFGR_PLLQ         0x0E000000
  #define RCC_PLLCFGR_PLLREN       0x10000000
  #define RCC_PLLCFGR_PLLR         0xE0000000
#define TOFF_RCC_APBRSTR1   0x002C		/* (RW) RCC APB peripheral reset register 1 */
  #define RCC_APBRSTR1_CRSRST      0x00010000	/* CRS clock reset */
#define TOFF_RCC_APBENR1    0x003C		/* (RW) RCC APB peripheral enable register 1 */
  #define RCC_APBENR1_CRSEN        0x00010000	/* CRS clock enable */
  #define RCC_APBENR1_PWREN        0x10000000	/* PWR clock enable */
#define TOFF_RCC_APBENR2    0x0040		/* (RW) RCC APB peripheral enable register 2 */
  #define RCC_APBENR2_SYSCFGEN     0x00000001	/* SYSCFG clock enable */
#define TOFF_RCC_CCIPR2      0x0058		/* (RW) RCC clock configuration register #2 */
  #define RCC_CCIPR2_USBSEL        0x00003000	/* USBSEL [1:0] bits */
  #define RCC_CCIPR2_USBSEL_0      0x00001000
  #define RCC_CCIPR2_USBSEL_1      0x00002000
  #define RCC_USBCLKSOURCE_HSI48   0x00000000	/* HSI48 oscillator clock selected as USB clock */

/*
 *  FLASH REGISTERS
 */
#define TADR_FLASH_R_BASE   0x40022000
#define TOFF_FLASH_ACR      0x0000		/* (RW) FLASH access control register */
  #define FLASH_ACR_LATENCY  0x00000007	/* LATENCY bit (Latency) */
  #define FLASH_ACR_LATENCY_0 0x00000001
  #define FLASH_ACR_LATENCY_1 0x00000002
  #define FLASH_ACR_LATENCY_2 0x00000004

#define TADR_SCB_BASE       0xE000ED00UL	/* System Control Block Base Address  */
  #define TOFF_SCB_VTOR     0x0008		/* (RW) Vector Table Offset Register */


#define PWR_REG_VOLTAGE_SCALE1     PWR_CR1_VOS_0
#define PWR_VOSF_SETTING_DEALY_6US 6U

#define RCC_PLLSOURCE_NONE         0x00000000				/* No clock selected as PLL entry clock source  */
#define RCC_PLLSOURCE_HSI          RCC_PLLCFGR_PLLSRC_HSI	/* HSI clock selected as PLL entry clock source */
#define RCC_PLLSOURCE_HSE          RCC_PLLCFGR_PLLSRC_HSE	/* HSE clock selected as PLL entry clock source */

#define RCC_HSICALIBRATION_DEFAULT 64						/* Default HSI calibration trimming value */


/*
 *  FLASH_Latency設定
 */ 
#define FLASH_LATENCY_0            0x00000000			/* FLASH Zero wait state */
#define FLASH_LATENCY_1            FLASH_ACR_LATENCY_0	/* FLASH One wait state */
#define FLASH_LATENCY_2            FLASH_ACR_LATENCY_1	/* FLASH Two wait states */

/*
 *  RCC System Clock Sourceの定義
 */
#define RCC_SYSCLKSOURCE_HSI       0x00000000						/* HSI selection as system clock */
#define RCC_SYSCLKSOURCE_HSE       RCC_CFGR_SW_0					/* HSE selection as system clock */
#define RCC_SYSCLKSOURCE_PLLCLK    RCC_CFGR_SW_1					/* PLL selection as system clock */
#define RCC_SYSCLKSOURCE_LSI       (RCC_CFGR_SW_1 | RCC_CFGR_SW_0)	/* LSI selection as system clock */
#define RCC_SYSCLKSOURCE_LSE       RCC_CFGR_SW_2					/* LSE selection as system clock */

/*
 *  Div HSI Dividedの定義
 */
#define RCC_HSI_DIV1               0x00000000						/* HSI clock is not divided */
#define RCC_HSI_DIV2               RCC_CR_HSIDIV_0					/* HSI clock is divided by 2 */
#define RCC_HSI_DIV4               RCC_CR_HSIDIV_1					/* HSI clock is divided by 4 */
#define RCC_HSI_DIV8               (RCC_CR_HSIDIV_1|RCC_CR_HSIDIV_0)/* HSI clock is divided by 8 */
#define RCC_HSI_DIV16              RCC_CR_HSIDIV_2					/* HSI clock is divided by 16 */
#define RCC_HSI_DIV32              (RCC_CR_HSIDIV_2|RCC_CR_HSIDIV_0)/* HSI clock is divided by 32 */
#define RCC_HSI_DIV64              (RCC_CR_HSIDIV_2|RCC_CR_HSIDIV_1)/* HSI clock is divided by 64 */
#define RCC_HSI_DIV128             (RCC_CR_HSIDIV_2|RCC_CR_HSIDIV_1|RCC_CR_HSIDIV_0) /* HSI clock is divided by 128 */

/*
 *  RCC APB Clock Sourceの定義
 */
#define RCC_HCLK_DIV1              0x00000000							/* HCLK not divided */
#define RCC_HCLK_DIV2              RCC_CFGR_PPRE_2						/* HCLK divided by 2 */
#define RCC_HCLK_DIV4              (RCC_CFGR_PPRE_2 | RCC_CFGR_PPRE_0)	/* HCLK divided by 4 */
#define RCC_HCLK_DIV8              (RCC_CFGR_PPRE_2 | RCC_CFGR_PPRE_1)	/* HCLK divided by 8 */
#define RCC_HCLK_DIV16             (RCC_CFGR_PPRE_2 | RCC_CFGR_PPRE_1 | RCC_CFGR_PPRE_0) /* HCLK divided by 16 */

/*
 *  RCC AHB Clock Sourceの定義
 */
#define RCC_SYSCLK_DIV1            0x00000000							/* SYSCLK not divided */
#define RCC_SYSCLK_DIV2            RCC_CFGR_HPRE_3						/* SYSCLK divided by 2 */
#define RCC_SYSCLK_DIV4            (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_0)	/* SYSCLK divided by 4 */
#define RCC_SYSCLK_DIV8            (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_1)	/* SYSCLK divided by 8 */
#define RCC_SYSCLK_DIV16           (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_0)	/* SYSCLK divided by 16 */
#define RCC_SYSCLK_DIV64           (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2)	/* SYSCLK divided by 64 */
#define RCC_SYSCLK_DIV128          (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_0)	/* SYSCLK divided by 128 */
#define RCC_SYSCLK_DIV256          (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_1)	/* SYSCLK divided by 256 */
#define RCC_SYSCLK_DIV512          (RCC_CFGR_HPRE_3 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_0)	/* SYSCLK divided by 512 */

/*
 *  CRSの定義
 */
#define CRS_CR_TRIM_BITNUMBER      8
#define CRS_CFGR_FELIM_BITNUMBER   16
#define RCC_CRS_ERRORLIMIT_DEFAULT 0x00000022			/* Default Frequency error limit */

#define RCC_CRS_SYNC_SOURCE_GPIO   0x00000000			/* Synchro Signal source GPIO */
#define RCC_CRS_SYNC_SOURCE_LSE    CRS_CFGR_SYNCSRC_0	/* Synchro Signal source LSE */
#define RCC_CRS_SYNC_SOURCE_USB    CRS_CFGR_SYNCSRC_1	/* Synchro Signal source USB SOF (default)*/

#define RCC_CRS_SYNC_DIV1          0x00000000			/* Synchro Signal not divided (default) */
#define RCC_CRS_SYNC_DIV2          CRS_CFGR_SYNCDIV_0	/* Synchro Signal divided by 2 */
#define RCC_CRS_SYNC_DIV4          CRS_CFGR_SYNCDIV_1	/* Synchro Signal divided by 4 */
#define RCC_CRS_SYNC_DIV8          (CRS_CFGR_SYNCDIV_1 | CRS_CFGR_SYNCDIV_0)	/* Synchro Signal divided by 8 */
#define RCC_CRS_SYNC_DIV16         CRS_CFGR_SYNCDIV_2	/* Synchro Signal divided by 16 */
#define RCC_CRS_SYNC_DIV32         (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_0)	/* Synchro Signal divided by 32 */
#define RCC_CRS_SYNC_DIV64         (CRS_CFGR_SYNCDIV_2 | CRS_CFGR_SYNCDIV_1)	/* Synchro Signal divided by 64 */
#define RCC_CRS_SYNC_DIV128        CRS_CFGR_SYNCDIV		/* Synchro Signal divided by 128 */


static const UW AHBPrescTable[16] = {0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 2U, 3U, 4U, 6U, 7U, 8U, 9U};

UW SystemFrequency;

/*
 *  起動クロックの取り出し
 */
UW
sysclock_init_value(void)
{
	UW ssource = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS;
	UW sysclockfreq = 0;

	if(ssource == RCC_CFGR_SWS_HSI){		/* HSI source */
		UW hsidiv = (sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSIDIV) >> 11;
		/*
		 *  HSI_DIVで割る
		 */
		sysclockfreq = HSI_VALUE / (1 << hsidiv);
	}
	else if(ssource == RCC_CFGR_SWS_HSE){	/* HSE source */
		sysclockfreq = HSE_VALUE;
	}
	else if(ssource == RCC_CFGR_SWS_PLL){	/* PLL1 source */
		UW pllcfgr = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR));
		UW pllsource = (pllcfgr & RCC_PLLCFGR_PLLSRC);
		UW pllm = ((pllcfgr & RCC_PLLCFGR_PLLM) >> PLLM_SHIFT) + 1;
		UW pllvco, pllr;

		switch (pllsource){
		case RCC_PLLSOURCE_HSE:  /* HSE used as PLL clock source */
			pllvco =  (HSE_VALUE / pllm) * ((pllcfgr & RCC_PLLCFGR_PLLN) >> PLLN_SHIFT);
			break;

		case RCC_PLLSOURCE_HSI:  /* HSI16 used as PLL clock source */
		default:                 /* HSI16 used as PLL clock source */
			pllvco = (HSI_VALUE / pllm) * ((pllcfgr & RCC_PLLCFGR_PLLN) >> PLLN_SHIFT) ;
			break;
		}
		pllr = (((pllcfgr & RCC_PLLCFGR_PLLR) >> PLLR_SHIFT) + 1);
		sysclockfreq = pllvco / pllr;
	}
	else if(ssource == RCC_CFGR_SWS_LSE){	/* LSE source */
		sysclockfreq = LSE_VALUE;
	}
	else if(ssource == RCC_CFGR_SWS_LSI){	/* LSI source */
		sysclockfreq = LSI_VALUE;
	}
	else{
		sysclockfreq = LSI_VALUE;
	}
	return sysclockfreq;
}

/*
 *  最大システムクロック設定
 *  設定値は以下の通り
 *      System Clock source            = PLL (HSI)
 *      SYSCLK(Hz)                     = 64000000
 *      HCLK(Hz)                       = 64000000
 *      AHB Prescaler                  = 1
 *      APB1 Prescaler                 = 1
 *      HSI Frequency(Hz)              = 16000000
 *      PLL_MUL                        = 1
 *      PLL_N                          = 8
 *      PLL_P                          = 2
 *      PLL_Q                          = 2
 *      PLL_R                          = 2
 *      Flash Latency(WS)              = 2
 *  return  正常終了なら0を返す
 */
static ER
SystemClock_Config(void)
{
	UW tick;
	UW cfgr = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR));
	UW ssource, psource, tmp;
	UW FLatency = FLASH_LATENCY_2;
#ifdef USB_DEVICE_ID
	UW value = 0;
#endif

	sil_modw_mem((UW *)(TADR_PWR_BASE+TOFF_PWR_CR1), PWR_CR1_VOS, PWR_REG_VOLTAGE_SCALE1);
	tick = ((PWR_VOSF_SETTING_DEALY_6US * HSI_VALUE) / 1000000U) + 1;
	while((sil_rew_mem((UW *)(TADR_PWR_BASE+TOFF_PWR_SR2)) & PWR_SR2_VOSF) != 0){
		if(tick == 0)
			return -1;
		tick--;
	}

	/*
	 *  HSI初期設定(HSIをシステムクロックとして使用する)
	 */
	ssource = cfgr & RCC_CFGR_SWS;
	psource = cfgr & RCC_PLLCFGR_PLLSRC;
	if(((ssource == RCC_CFGR_SWS_PLL) && (psource == RCC_PLLSOURCE_HSI)) || (ssource == RCC_CFGR_SWS_HSI)){
		/*
		 *  HSIキャリブレーション値を設定
		 */
		sil_modw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_HSITRIM, RCC_HSICALIBRATION_DEFAULT << ICSCR_HSITRIM_SHIFT);

		/*
		 *  HSIがシステムクロックの場合、デバイダー値を1にしてシステムクロックを算定
		 */
		if(ssource == RCC_CFGR_SWS_HSI){
			sil_modw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSIDIV, RCC_HSI_DIV1);
			SystemFrequency = (HSI_VALUE / (1 << (((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSIDIV)) >> HSIDIV_SHIFT)));
		}
	}
	else{
		/*
		 *  HSIがシステムクロックでない場合は、HSIの初期化を行う
		 */
		sil_modw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSIDIV, RCC_HSI_DIV1);
		sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSION);

		/*
		 *  HSIレディ待ち
		 */
		tick = 0;
		while((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSIRDY) == 0){
			if(tick++ > HSI_TIMEOUT_VALUE)
				return -1;
			sil_dly_nse(1000);
		}

		/*
		 *  HSIキャリブレーション値を設定
		 */
		sil_modw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_ICSCR), RCC_ICSCR_HSITRIM, RCC_HSICALIBRATION_DEFAULT << ICSCR_HSITRIM_SHIFT);
    }

#ifdef USB_DEVICE_ID
	/* Enable HSI48 */
	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSI48ON);
	tick = 0;
	while((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_HSI48RDY) == 0){
		if(tick  > PLL_TIMEOUT_VALUE)
			return E_TMOUT;
		tick++;
		sil_dly_nse(1000);
	}
	sil_modw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CCIPR2), RCC_CCIPR2_USBSEL, RCC_USBCLKSOURCE_HSI48);
#endif

	/*
	 *  PLL1初期設定
	 *  PLL1をCPUクロックで使用中はHSIに切り替える
	 */
	if(ssource == RCC_CFGR_SWS_PLL){
		cfgr = (cfgr & ~RCC_CFGR_SW) | RCC_SYSCLKSOURCE_HSI;
		sil_wrw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR), cfgr);

		/* Get Start Tick*/
		tick = 0;
		while((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != (RCC_SYSCLKSOURCE_HSI << CFGR_SWS_SHIFT)){
			if(tick++ > CLOCKSWITCH_TIMEOUT_VALUE)
				return -1;
			sil_dly_nse(1000);
		}
	}

	/*
	 *  PLL1を停止
	 */
	sil_andw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLON);

	tick = 0;
	while((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) != 0){
		if(tick++ > PLL_TIMEOUT_VALUE)
			return -1;
		sil_dly_nse(1000);
	}

	/*
	 *  PLL1のソース、ファクター設定
	  */
	sil_wrw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), (RCC_PLLSOURCE_HSI | \
			((PLL_R-1)<<PLLR_SHIFT) | ((PLL_Q-1)<<24) | ((PLL_P-1)<<17) | (PLL_N << PLLN_SHIFT) | ((PLL_M-1)<<PLLM_SHIFT)));

	/*
	 *  PLL1を再開/PLLRクロック設定
	 */
	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_PLLON);
	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_PLLCFGR_PLLREN);

	/*
	 *  PLL1レディ待ち
	 */
	tick = 0;
	while((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
		if(tick++ > PLL_TIMEOUT_VALUE)
			return -1;
		sil_dly_nse(1000);
	}

	/*
	 *  CPU/SYSCLK/HCLK/PLL設定
	 */
	if(FLatency > (sil_rew_mem((UW *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY)){
		/*
		 *  FLASHレイテンシィの設定(要求は現状より高い場合
		 */
		sil_modw_mem((UW *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_LATENCY, FLatency);
		tick = 0;
		while((sil_rew_mem((UW *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLatency){
			if(tick++ > CLOCKSWITCH_TIMEOUT_VALUE)
				return -1;
			sil_dly_nse(1000);
		}
	}

	/*
	 *  HCLK初期設定
	 */
	cfgr  = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & ~RCC_CFGR_PPRE;
	cfgr |= RCC_HCLK_DIV16;
	sil_wrw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR), cfgr);

	/* Set the new HCLK clock divider */
	cfgr = (cfgr & ~RCC_CFGR_HPRE) | RCC_SYSCLK_DIV1;
	sil_wrw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR), cfgr);

	/*
	 *  SYSCLK初期設定(PCK1)
	 */
	cfgr = (cfgr & ~RCC_CFGR_SW) | RCC_SYSCLKSOURCE_PLLCLK;
	sil_wrw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR), cfgr);

	/*
	 *  SYSCLK設定待ち
	 */
	tick = 0;
	while((sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != (RCC_SYSCLKSOURCE_PLLCLK << CFGR_SWS_SHIFT)){
		if(tick++ > CLOCKSWITCH_TIMEOUT_VALUE)
			return -1;
		sil_dly_nse(1000);
	}

	if(FLatency < (sil_rew_mem((UW *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY)){
		/*
		 *  FLASHレイテンシィの設定(要求は現状より低い場合
		 */
		sil_modw_mem((UW *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR), FLASH_ACR_LATENCY, FLatency);
		while((sil_rew_mem((UW *)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLatency){
			if(tick++ > CLOCKSWITCH_TIMEOUT_VALUE)
				return -1;
			sil_dly_nse(1000);
		}
	}

	/*
	 *  PCLK1 HCLK分周設定
	 */
	sil_modw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR), RCC_CFGR_PPRE, RCC_HCLK_DIV1);

#ifdef USB_DEVICE_ID
	/* Enable CRS Clock */
	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBENR1), RCC_APBENR1_CRSEN);

	/* CRSクロックリセット */
	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBRSTR1), RCC_APBRSTR1_CRSRST);
	sil_andw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBRSTR1), RCC_APBRSTR1_CRSRST);

	/* Set the SYNCDIV[2:0] bits according to Prescaler value */
	/* Set the SYNCSRC[1:0] bits according to Source value */
	/* Set the SYNCSPOL bit according to Polarity value */
	value = (RCC_CRS_SYNC_DIV1 | RCC_CRS_SYNC_SOURCE_USB | /*pInit->Polarity*/ 0);
	/* Set the RELOAD[15:0] bits according to ReloadValue value */
	value |= 48000000/1000 - 1;
	/* Set the FELIM[7:0] bits according to ErrorLimitValue value */
	value |= (RCC_CRS_ERRORLIMIT_DEFAULT << CRS_CFGR_FELIM_BITNUMBER);
	sil_wrw_mem((UW *)(TADR_CRS_BASE+TOFF_CRS_CFGR), value);

	/* Adjust HSI48 oscillator smooth trimming */
	/* Set the TRIM[5:0] bits according to RCC_CRS_HSI48CalibrationValue value */
	sil_modw_mem((UW *)(TADR_CRS_BASE+TOFF_CRS_CR), CRS_CR_TRIM, (0x20 << CRS_CR_TRIM_BITNUMBER));

	/* Enable Automatic trimming & Frequency error counter */
	sil_orw_mem((UW *)(TADR_CRS_BASE+TOFF_CRS_CR), (CRS_CR_AUTOTRIMEN | CRS_CR_CEN));
#endif

	/*
	 *  システムクロックを再計算
	 */
	cfgr  = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_CFGR));
	SystemFrequency = (sysclock_init_value() >> ((AHBPrescTable[(cfgr & RCC_CFGR_HPRE) >> CFGR_HPRE_SHIFT]) & 0x1F));
	(void)(tmp);
	return E_OK;
}

/*
 *  システムクロックコンフィギュレーション
 */
ER
sysclock_config(int mode)
{
	if(mode == 1)
		return SystemClock_Config();
	else
		return E_OK;
}


/*
 *  低レベルのターゲット依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */
void hardware_init_hook(void)
{
	UW tmp;

	/*
	 *  VECTORテーブルを設定
	 */
#ifndef TOPPERS_RAM_EXEC
	sil_wrw_mem((UW *)(TADR_SCB_BASE+TOFF_SCB_VTOR), (FLASH_BASE));
	SystemClock_Config();
#else
	sil_wrw_mem((UW *)(TADR_SCB_BASE+TOFF_SCB_VTOR), (SRAM_BASE));
#endif

	/*
	 *  SYSCFGとPWRのクロックを有効化
	 */
	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBENR2), RCC_APBENR2_SYSCFGEN);
	tmp = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBENR2));

	sil_orw_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBENR1), RCC_APBENR1_PWREN);
	tmp = sil_rew_mem((UW *)(TADR_RCC_BASE+TOFF_RCC_APBENR1));
	(void)(tmp);
}

