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
 *  @(#) $Id: stm32f401re.c 698 2025-11-01 09:22:30Z roi $
 */

#include <s_services.h>
#include <t_config.h>

/*
 *  初期化プログラム（NUCLEO-F401RE用）
 */

#define PERIPH_BASE         0x40000000	/* Peripheral base address in the alias region                                */
#define PERIPH_BB_BASE      0x42000000	/* Peripheral base address in the bit-band region                                */

/*
 *  POWER CONTROL
 */
#define TADR_PWR_BASE       0x40007000
#define TOFF_PWR_CR         0x0000		/* (RW) PWR power control register */
  #define PWR_CR_VOS               0x0000C000   /* VOS[1:0] bits (Regulator voltage scaling output selection) */ 
  #define PWR_CR_VOS_0             0x00004000   /* Bit 0 */
  #define PWR_CR_VOS_1             0x00008000   /* Bit 1 */

/*
 *  RESET AND CLOCK CONTROL
 */
#define TADR_RCC_BASE       0x40023800
#define TOFF_RCC_CR         0x0000		/* (RW) RCC clock control register */
  #define RCC_CR_HSION             0x00000001
  #define RCC_CR_HSITRIM_0         0x00000008	/* Bit 0 */
  #define RCC_CR_HSITRIM_1         0x00000010	/* Bit 1 */
  #define RCC_CR_HSITRIM_2         0x00000020	/* Bit 2 */
  #define RCC_CR_HSITRIM_3         0x00000040	/* Bit 3 */
  #define RCC_CR_HSITRIM_4         0x00000080	/* Bit 4 */
  #define RCC_CR_HSEON             0x00010000
  #define RCC_CR_HSEBYP            0x00040000
  #define RCC_CR_CSSON             0x00080000
  #define RCC_CR_PLLON             0x01000000
  #define RCC_CR_PLLRDY            0x02000000
#define TOFF_RCC_PLLCFGR    0x0004		/* (RW) RCC PLL configuration register */
  #define  RCC_PLLCFGR_PLLSRC_HSE  0x00400000
  #define  RCC_PLLCFGR_PLLSRC_HSI  0x00000000
#define TOFF_RCC_CFGR       0x0008		/* (RW) RCC clock configuration register */
  #define RCC_CFGR_SW_0            0x00000001	/* Bit 0 */
  #define RCC_CFGR_SW_1            0x00000002	/* Bit 1 */
  #define RCC_CFGR_SW_PLL          0x00000002	/* PLL selected as system clock */
  #define RCC_CFGR_SWS_0           0x00000004	/* Bit 0 */
  #define RCC_CFGR_SWS_1           0x00000008	/* Bit 1 */
  #define RCC_CFGR_SWS_HSI         0x00000000	/* HSI oscillator used as system clock */
  #define RCC_CFGR_SWS_PLL         0x00000008	/* PLL used as system clock */
  #define RCC_CFGR_HPRE_0          0x00000010	/* Bit 0 */
  #define RCC_CFGR_HPRE_1          0x00000020	/* Bit 1 */
  #define RCC_CFGR_HPRE_2          0x00000040	/* Bit 2 */
  #define RCC_CFGR_HPRE_3          0x00000080	/* Bit 3 */
  #define RCC_CFGR_HPRE_DIV1       0x00000000	/* SYSCLK not divided */
  #define RCC_CFGR_PPRE1_0         0x00000400	/* Bit 0 */
  #define RCC_CFGR_PPRE1_1         0x00000800	/* Bit 1 */
  #define RCC_CFGR_PPRE1_2         0x00001000	/* Bit 2 */
  #define RCC_CFGR_PPRE1_DIV1      0x00000000	/* HCLK not divided */
  #define RCC_CFGR_PPRE1_DIV2      0x00001000	/* HCLK divided by 2 */
  #define RCC_CFGR_PPRE2_0         0x00002000	/* Bit 0 */
  #define RCC_CFGR_PPRE2_1         0x00004000	/* Bit 1 */
  #define RCC_CFGR_PPRE2_2         0x00008000	/* Bit 2 */
  #define RCC_CFGR_PPRE2_DIV2      0x00008000	/* HCLK divided by 2 */
  #define RCC_CFGR_MCO1PRE_0       0x01000000
  #define RCC_CFGR_MCO1PRE_1       0x02000000
  #define RCC_CFGR_MCO1PRE_2       0x04000000
  #define RCC_CFGR_MCO2PRE_0       0x08000000
  #define RCC_CFGR_MCO2PRE_1       0x10000000
  #define RCC_CFGR_MCO2PRE_2       0x20000000
#define TOFF_RCC_CIR        0x000C		/* (RW) RCC clock interrupt register */
#define TOFF_RCC_APB1ENR    0x0040		/* (RW) RCC APB1 peripheral clock enable register */
  #define RCC_APB1ENR_PWREN        0x10000000

/*
 *  FLASH REGISTERS
 */
#define TADR_FLASH_R_BASE   0x40023C00
#define TOFF_FLASH_ACR      0x0000		/* (RW) FLASH access control register */
  #define FLASH_ACR_LATENCY_0WS    0x00000000
  #define FLASH_ACR_LATENCY_1WS    0x00000001
  #define FLASH_ACR_LATENCY_2WS    0x00000002
  #define FLASH_ACR_LATENCY_3WS    0x00000003
  #define FLASH_ACR_LATENCY_4WS    0x00000004
  #define FLASH_ACR_LATENCY_5WS    0x00000005
  #define FLASH_ACR_LATENCY_6WS    0x00000006
  #define FLASH_ACR_LATENCY_7WS    0x00000007


#define HSE_STARTUP_TIMEOUT 0x0500		/* HSE用タイムアウト設定値 */

/*
 *  PLL設定値
 */
#define PLL_M      16
#define PLL_N      336
#define PLL_P      4
#define PLL_Q      7

#define PWR_REGULATOR_VOLTAGE_SCALE1   PWR_CR_VOS				/* VOLTAGEスケール1:168MHz、オーバードライブ時180MHz */
#define PWR_REGULATOR_VOLTAGE_SCALE2   PWR_CR_VOS_1				/* VOLTAGEスケール2:144MHz、オーバードライブ時168MHz */
#define PWR_REGULATOR_VOLTAGE_SCALE3   PWR_CR_VOS_0				/* VOLTAGEスケール3:120MHz. */

#define FLASH_LATENCY_2                FLASH_ACR_LATENCY_2WS	/* FLASH Two Latency cycles      */

#define RCC_CFGR_SW        (RCC_CFGR_SW_0 | RCC_CFGR_SW_1)
#define RCC_CFGR_SWS       (RCC_CFGR_SWS_0 | RCC_CFGR_SWS_1)
#define RCC_CFGR_HPRE      (RCC_CFGR_HPRE_0 | RCC_CFGR_HPRE_1 | RCC_CFGR_HPRE_2 | RCC_CFGR_HPRE_3)
#define RCC_CFGR_PPRE1     (RCC_CFGR_PPRE1_0 | RCC_CFGR_PPRE1_1 | RCC_CFGR_PPRE1_2)
#define RCC_CFGR_PPRE2     (RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE2_1 | RCC_CFGR_PPRE2_2)
#define RCC_PLLCFGR_PLLM   (RCC_PLLCFGR_PLLM_0 | RCC_PLLCFGR_PLLM_1 | RCC_PLLCFGR_PLLM_2 \
                           | RCC_PLLCFGR_PLLM_3 | RCC_PLLCFGR_PLLM_4 | RCC_PLLCFGR_PLLM_5)
#define RCC_PLLCFGR_PLLN   (RCC_PLLCFGR_PLLN_0 | RCC_PLLCFGR_PLLN_1 | RCC_PLLCFGR_PLLN_2 | RCC_PLLCFGR_PLLN_3 \
                           | RCC_PLLCFGR_PLLN_4 | RCC_PLLCFGR_PLLN_5 | RCC_PLLCFGR_PLLN_6 | RCC_PLLCFGR_PLLN_7 | RCC_PLLCFGR_PLLN_8)
#define RCC_PLLCFGR_PLLP   (RCC_PLLCFGR_PLLP_0 | RCC_PLLCFGR_PLLP_1)
#define RCC_PLLCFGR_PLLSRC (RCC_PLLCFGR_PLLSRC_HSE)
#define RCC_CR_HSITRIM     (RCC_CR_HSITRIM_0 | RCC_CR_HSITRIM_1 | RCC_CR_HSITRIM_2 | RCC_CR_HSITRIM_3 | RCC_CR_HSITRIM_4)
#define FLASH_ACR_LATENCY  (FLASH_ACR_LATENCY_0WS | FLASH_ACR_LATENCY_1WS | FLASH_ACR_LATENCY_2WS | FLASH_ACR_LATENCY_3WS \
                           | FLASH_ACR_LATENCY_4WS | FLASH_ACR_LATENCY_5WS | FLASH_ACR_LATENCY_6WS | FLASH_ACR_LATENCY_7WS)

/*
 *  RCC_BitAddress_AliasRegion RCC BitAddress AliasRegion
 *  RCC registers bit address in the alias region
 */
#define RCC_OFFSET                 (TADR_RCC_BASE - PERIPH_BASE)
/* Alias word address of HSION bit */
#define RCC_CR_OFFSET              (RCC_OFFSET + 0x00)
#define RCC_HSION_BIT_NUMBER       0x00
#define RCC_CR_HSION_BB            (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_HSION_BIT_NUMBER * 4))
/* Alias word address of PLLON bit */
#define RCC_PLLON_BIT_NUMBER       0x18
#define RCC_CR_PLLON_BB            (PERIPH_BB_BASE + (RCC_CR_OFFSET * 32) + (RCC_PLLON_BIT_NUMBER * 4))

/*
 *  ACR register byte 0 (Bits[7:0]) base address  
 */
#define ACR_BYTE0_ADDRESS           (0x40023C00)

#define PLL_TIMEOUT_VALUE          1000  /* 100 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  50000 /* 5 s    */

#define sil_mdw_mem(addr, mask, val)	sil_wrw_mem((addr), ((sil_rew_mem(addr) & ~(mask)) | (val)))
#define sil_anw_mem(addr, mask)			sil_wrw_mem((addr), (sil_rew_mem(addr) & ~(mask)))
#define sil_orw_mem(addr, val)			sil_wrw_mem((addr), (sil_rew_mem(addr) | (val)))


/*
 *  システムクロックコンフィギュレーション
 */
ER
sysclock_config(UW mode)
{
	UINT tickstart = 0;
	UINT tmp;

 	/*
	 *  パワークロック有効化
	 */
	sil_orw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR), RCC_APB1ENR_PWREN);
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_APB1ENR));

	/*
	 *  VOLTAGE設定
	 */
	sil_mdw_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR), (PWR_CR_VOS), PWR_REGULATOR_VOLTAGE_SCALE2);
	tmp = sil_rew_mem((VP)(TADR_PWR_BASE+TOFF_PWR_CR));

	/*
	 *  HSE初期設定
	 */
	tmp = sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS;
	if((tmp == RCC_CFGR_SWS_HSI)  ||
      ((tmp == RCC_CFGR_SWS_PLL) && ((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR)) & RCC_PLLCFGR_PLLSRC) == RCC_PLLCFGR_PLLSRC_HSI)))
	{
		sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), (RCC_CR_HSITRIM), (0x10 << 3));
	}

    /*
	 *  スイッチセレクトがPLLでない場合、PLLを停止して再設定
	 */
	if((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
        /*
	　　 *  PLL停止
		 */
		sil_wrw_mem((VP)(RCC_CR_PLLON_BB), 0);

        /*
		 *  PLL停止待ち
		 */
        tickstart = 0;
        while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) != 0){
			tickstart++;
			sil_dly_nse(100*1000);
			if(tickstart > PLL_TIMEOUT_VALUE){
	            return E_TMOUT;
			}
        }

        /*
		 *  PLL設定
		 */
		sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) |
                   (RCC_PLLCFGR_PLLSRC_HSI) | (PLL_Q << 24));
        /*
		 *  PLL再開
		 */
		sil_wrw_mem((VP)(RCC_CR_PLLON_BB), 1);

        /*
		 *  PLL開始待ち
		 */
		tickstart = 0;
        while((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
			tickstart++;
			sil_dly_nse(100*1000);
			if( tickstart  > PLL_TIMEOUT_VALUE){
	            return E_TMOUT;
			}
		}
	}

	/*
	 *  FLASH設定を2WSに設定
	 */
	sil_wrb_mem((UB *)ACR_BYTE0_ADDRESS, FLASH_ACR_LATENCY_2WS);

	/*
	 *  FLASH設定が反映されなければエラー終了
	 */
    if((sil_rew_mem((VP)(TADR_FLASH_R_BASE+TOFF_FLASH_ACR)) & FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_2WS){
		return E_SYS;
    }

    /*
	 *  HCLKのクロック設定
	 */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_HPRE), RCC_CFGR_HPRE_DIV1);

	/*
	 *  PLLがREADYにならなければエラー
	 */
	if((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR)) & RCC_CR_PLLRDY) == 0){
		return E_SYS;
	}

	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_SW), RCC_CFGR_SW_PLL);

    /*
	 *  スイッチセレクトがPLLになるまで待ち
	 */
	tickstart = 0;
	while ((sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR)) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){
		tickstart++;
		sil_dly_nse(100*1000);
		if( tickstart > CLOCKSWITCH_TIMEOUT_VALUE){
            return E_TMOUT;
		}
	}

	/*
	 *  PCLK1のコンフィギュレーション
	 */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_PPRE1), RCC_CFGR_PPRE1_DIV2);

	/*
	 *  PCLK2のコンフィギュレーション
	 */
	sil_mdw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), (RCC_CFGR_PPRE2), (RCC_CFGR_PPRE1_DIV1 << 3));
	return E_OK;
}

#ifdef ROM

/*
 *  低レベルのターゲット依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */
void
hardware_init_hook(void)
{
	/*
	 * RCCのリセットとコンフィギュレーション
	 */
	sil_orw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSION);
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CFGR), 0x00000000);
	sil_anw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), (RCC_CR_PLLON | RCC_CR_CSSON | RCC_CR_HSEON));
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_PLLCFGR), RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO1PRE_2
					 | RCC_CFGR_PPRE2_0 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_HPRE_0);

	/* Reset HSEBYP bit */
	sil_anw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CR), RCC_CR_HSEBYP);

	/*
	 *  全割込み禁止
	 */
	sil_wrw_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CIR), 0x00000000);

	sysclock_config(0);
}

#endif	/* ROM */

