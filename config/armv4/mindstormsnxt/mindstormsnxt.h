/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2009      by GJ Designing Center Print Business Group
 *                             RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id: $
 */

#ifndef _MINDSTORMSNXT_H_
#define _MINDSTORMSNXT_H_

#ifndef _MACRO_ONLY
#include <itron.h>
#include <sil.h>
#endif /* _MACRO_ONLY */

#include "armv4.h"
#include "sys_config.h"
#include "cpu_config.h"

#define CLOCK_FREQUENCY 48000000

/*
 *  ADVANCED INTERRUPT CONTROLLER
 */
#define TADR_AIC_BASE   0xFFFFF000	/* AIC base address */
#define TOFF_AIC_SMR    0x0000		/* Source Mode Register0-31 (R/W) */
  #define AIC_PRIOR                        (7<<0)	/* Priority Level */
  #define AIC_PRIOR_LOWEST                 (0)		/* Lowest priority level */
  #define AIC_PRIOR_HIGHEST                (7)		/* Highest priority level */
  #define AIC_SRCTYPE                      (3<<5)	/* Interrupt Source Type */
  #define AIC_SRCTYPE_INT_LEVEL_SENSITIVE  (0<<5)	/* Internal Sources Code Label Level Sensitive */
  #define AIC_SRCTYPE_INT_EDGE_TRIGGERED   (1<<5)	/* Internal Sources Code Label Edge triggered */
  #define AIC_SRCTYPE_EXT_HIGH_LEVEL       (2<<5)	/* External Sources Code Label High-level Sensitive */
  #define AIC_SRCTYPE_EXT_POSITIVE_EDGE    (3<<5)	/* External Sources Code Label Positive Edge triggered */
#define TOFF_AIC_SVR    0x0080		/* Source Vector Register0-31 (R/W) */
#define TOFF_AIC_IVR    0x0100		/* Interrupt Vector Register (R) */
#define TOFF_AIC_FVR    0x0104		/* Fast Interrupt Vector Register (R) */
#define TOFF_AIC_ISR    0x0108		/* Interrupt Status Register (R) */
#define TOFF_AIC_IPR    0x010C		/* Interrupt Pending Register (R) */
#define TOFF_AIC_IMR    0x0110		/* Interrupt Mask Register (R) */
#define TOFF_AIC_CISR   0x0114		/* Core Interrupt Status Register (R) */
#define TOFF_AIC_IECR   0x0120		/* Interrupt Enable Command Register (W) */
#define TOFF_AIC_IDCR   0x0124		/* Interruot Disable Command Register (W) */
#define TOFF_AIC_ICCR   0x0128		/* Interrupt Clear Command Register (W) */
#define TOFF_AIC_ISCR   0x012C		/* Interrupt Set Command Register (W) */
#define TOFF_AIC_EOICR  0x0130		/* End of Interrupt Command Register (W) */
#define TOFF_AIC_SPU    0x0134		/* Spurios Interrupt Vector Register (R/W */
#define TOFF_AIC_DCR    0x0138		/* Debug Control Register (R/W) */
#define TOFF_AIC_FFER   0x0140		/* Fast Forcing Enable Register (W) */
#define TOFF_AIC_FFDR   0x0144		/* Fast Forcing Disable Register (W) */
#define TOFF_AIC_FFSR   0x0148		/* Fast Forcing Status Register (R) */

#define IRQ_FIQ_PID     0
#define IRQ_SYSIRQ_PID  1
#define IRQ_PIOA_PID    2
#define IRQ_ADC_PID     4
#define IRQ_SPI_PID     5
#define IRQ_US0_PID     6
#define IRQ_US1_PID     7
#define IRQ_SSC_PID     8
#define IRQ_TWI_PID     9
#define IRQ_PWM_PID     10
#define IRQ_UDP_PID     11
#define IRQ_TC0_PID     12
#define IRQ_TC1_PID     13
#define IRQ_TC2_PID     14
#define IRQ_IRQ0_PID    30
#define IRQ_IRQ1_PID    31

#ifndef SMR_SYSIRQ
#define SMR_SYSIRQ      (AIC_SRCTYPE_INT_EDGE_TRIGGERED|3)
#endif
#ifndef SMR_PIOA
#define SMR_PIOA        (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|4)
#endif
#ifndef SMR_SPI
#define SMR_SPI         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|4)
#endif
#ifndef SMR_TWI
#define SMR_TWI         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|5)
#endif
#ifndef SMR_US0
#define SMR_US0         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|3)
#endif
#ifndef SMR_US1
#define SMR_US1         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|3)
#endif
#ifndef SMR_SSC
#define SMR_SSC         (AIC_SRCTYPE_INT_EDGE_TRIGGERED|0)
#endif
#ifndef SMR_PMW
#define SMR_PMW         (AIC_SRCTYPE_INT_EDGE_TRIGGERED|2)
#endif
#ifndef SMR_TC0
#define SMR_TC0         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|4)
#endif
#ifndef SMR_TC1
#define SMR_TC1         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|3)
#endif
#ifndef SMR_TC2
#define SMR_TC2         (AIC_SRCTYPE_INT_LEVEL_SENSITIVE|3)
#endif


/*
 *  DEBUG UNIT Debug Unit
 */
#define TADR_DBGU_BASE  0xFFFFF200		/* Debug Unit BASE address */
#define TOFF_DBGU_CR    0x0000			/* Control Register (W):TOFF_US_CR */
#define TOFF_DBGU_MR    0x0004			/* Mode Register (R/W):TOFF_US_MR */
#define TOFF_DBGU_IER   0x0008			/* Interrupt Enable Register (W):TOFF_US_IER */
#define TOFF_DBGU_IDR   0x000C			/* Interrupt Disable Register (W):TOFF_US_IDR */
#define TOFF_DBGU_IMR   0x0010			/* Interrupt Mask Regiser (R):TOFF_US_IMR */
#define TOFF_DBGU_SR    0x0014			/* Status Register (R):TOFF_US_CSR */
#define TOFF_DBGU_RHR   0x0018			/* Receive Holding Register (R):TOFF_US_RHR */
#define TOFF_DBGU_THR   0x001C			/* Transmit Holding Register (W):TOFF_US_THR */
#define TOFF_DBGU_BRGR  0x0020			/* Baud Rate Generator Register (R/W):TOFF_US_BRGR */
#define TOFF_DBGU_CIDR  0x0040			/* Chip ID Register (R) */
#define TOFF_DBGU_EXID  0x0044			/* Chip ID Extension Register (R) */
#define TOFF_DBGU_FNR   0x0048			/* Force NTRST Register (R/W) */


/*
 *  PARALLEL INPUT/OUTPUT CONTROLLER
 */
#define TADR_PIO_BASE	0xFFFFF400		/* PIO BASE ADDRESS */
#define TOFF_PIO_PER    0x0000			/* PIO Enable Register (W) */
#define TOFF_PIO_PDR    0x0004			/* PIO Disable Register (W) */
#define TOFF_PIO_PSR    0x0008			/* PIO Status Register (R) */
#define TOFF_PIO_OER    0x0010			/* Output Enable Register (W) */
#define TOFF_PIO_ODR    0x0014			/* Output Disable Register (W) */
#define TOFF_PIO_OSR    0x0018			/* Output Status Register (R) */
#define TOFF_PIO_IFER   0x0020			/* Glitch Input Filter Enable Register (W) */
#define TOFF_PIO_IFDR   0x0024			/* Glitch Input Filter Disable Register (W) */
#define TOFF_PIO_IFSR   0x0028			/* Glitch Input Filter Status Register (R) */
#define TOFF_PIO_SODR   0x0030			/* Set Output Data Register (W) */
#define TOFF_PIO_CODR   0x0034			/* Clear Output Data Register (W) */
#define TOFF_PIO_ODSR   0x0038			/* Output Data Status Register (R) */
#define TOFF_PIO_PDSR   0x003C			/* Pin Data Status Register (R) */
#define TOFF_PIO_IER    0x0040			/* Interrupt Enable Register (W) */
#define TOFF_PIO_IDR    0x0044			/* Interrupt Disable Register (W) */
#define TOFF_PIO_IMR    0x0048			/* Interrupt Mask Register (R) */
#define TOFF_PIO_ISR    0x004C			/* Interrupt Status Register (R) */
#define TOFF_PIO_MDER   0x0050			/* Multi-driver Enable Register (W) */
#define TOFF_PIO_MDDR   0x0054			/* Multi-driver Disable Register (W) */
#define TOFF_PIO_MDSR   0x0058			/* Multi-driver Status Register (R) */
#define TOFF_PIO_PUDR   0x0060			/* Pull-up Disable Register (W) */
#define TOFF_PIO_PUER   0x0064			/* Pull-up Enable Register (W) */
#define TOFF_PIO_PUSR   0x0068			/* Pad Pull-up Statuse Register (R) */
#define TOFF_PIO_ASR    0x0070			/* Peripheral A Select Register (W) */
#define TOFF_PIO_BSR    0x0074			/* Peripheral B Select Register (W) */
#define TOFF_PIO_ABSR   0x0078			/* AB Status Register (R) */
#define TOFF_PIO_OWER   0x00A0			/* Output Write Enable (W) */
#define TOFF_PIO_OWDR   0x00A4			/* Output Write Disable (W) */
#define TOFF_PIO_OWSR   0x00A8			/* Output Write Status Register (R) */
  #define AT91S_PIO_PA2      (1<<2)		/* Pin Controlled by PA2 */
  #define AT91S_PIO_PA3      (1<<3)		/* Pin Controlled by PA3 */
  #define AT91S_PIO_PA4      (1<<4)		/* Pin Controlled by PA4 */
  #define AT91S_PIO_PA5      (1<<5)		/* Pin Controlled by PA5 */
  #define AT91S_PIO_PA6      (1<<6)		/* Pin Controlled by PA6 */
  #define AT91S_PIO_PA7      (1<<7)		/* Pin Controlled by PA7 */
  #define AT91S_PIO_PA10     (1<<10)	/* Pin Controlled by PA10 */
  #define AT91S_PIO_PA11     (1<<11)	/* Pin Controlled by PA11 */
  #define AT91S_PIO_PA12     (1<<12)	/* Pin Controlled by PA12 */
  #define AT91S_PIO_PA13     (1<<13)	/* Pin Controlled by PA13 */
  #define AT91S_PIO_PA14     (1<<14)	/* Pin Controlled by PA14 */
  #define AT91S_PIO_PA17     (1<<17)	/* Pin Controlled by PA17 */
  #define AT91S_PIO_PA18     (1<<18)	/* Pin Controlled by PA18 */
  #define AT91S_PIO_PA19     (1<<19)	/* Pin Controlled by PA19 */
  #define AT91S_PIO_PA20     (1<<20)	/* Pin Controlled by PA20 */
  #define AT91S_PIO_PA21     (1<<21)	/* Pin Controlled by PA21 */
  #define AT91S_PIO_PA22     (1<<22)	/* Pin Controlled by PA22 */
  #define AT91S_PIO_PA23     (1<<23)	/* Pin Controlled by PA23 */
  #define AT91S_PIO_PA24     (1<<24)	/* Pin Controlled by PA24 */
  #define AT91S_PIO_PA25     (1<<25)	/* Pin Controlled by PA25 */
  #define AT91S_PIO_PA27     (1<<27)	/* Pin Controlled by PA27 */
  #define AT91S_PIO_PA28     (1<<28)	/* Pin Controlled by PA28 */
  #define AT91S_PIO_PA29     (1<<29)	/* Pin Controlled by PA29 */
  #define AT91S_PIO_PA30     (1<<30)	/* Pin Controlled by PA30 */
  #define AT91S_PIO_PA31     (1<<31)	/* Pin Controlled by PA31 */

/*
 *  POWER MANAGMENT CONTROLLER
 */
#define TADR_PMC_BASE   0xFFFFFC00		/* PMC BASE ADDRESS */
#define TOFF_PMC_SCER   0x0000			/* System Clock Enable Register (W) */
#define TOFF_PMC_SCDR   0x0004			/* System Clock Disable Register (W) */
#define TOFF_PMC_SCSR   0x0008			/* System Clock Status Register (R) */
#define TOFF_PMC_PCER   0x0010			/* Peripheral Clock Enable Register (W) */
#define TOFF_PMC_PCDR   0x0014			/* Peripheral Clock Disable Register (W) */
#define TOFF_PMC_PCSR   0x0018			/* Peripheral Clock Status Register (W) */
#define TOFF_CKGR_MOR   0x0020			/* Main Oscillator Register (W) */
  #define CKGR_MOR_MOSCEN         (1<<0)
  #define CKGR_MOR_OSCBYPASS      (1<<1)
  #define CKGR_MOR_OSCOUNT_SHIFT  8
#define TOFF_CKGR_MCFR  0x0024			/* Main Clock Frequency Register (R) */
#define TOFF_CKGR_PLLR  0x002C			/* PLL Register (R/W) */
  #define CKGR_PLLR_DIV_SHIFT      0
  #define CKGR_PLLR_PLLCOUNT_SHIFT 8
  #define CKGR_PLLR_MUL_SHIFT      16
#define TOFF_PMC_MCKR   0x0030			/* Master Clock Register (R/W) */
  #define PMC_MCKR_CSS_PLL_CLOCK  (3<<0)
  #define PMC_MCKR_PRES_CLK_2     (1<<2)
#define TOFF_PMC_PCK0   0x0040			/* Programmable Clock 0 Register (R/W) */
#define TOFF_PMC_PCK1   0x0044			/* Programmable Clock 1 Register (R/W) */
#define TOFF_PMC_IER    0x0060			/* Interrupt Enable Register (W) */
#define TOFF_PMC_IDR    0x0064			/* Interrupt Disable Register (W) */
#define TOFF_PMC_SR     0x0068			/* Status Register (R) */
  #define PMC_SR_MOSCS            (1<<0)
  #define PMC_SR_LOCK             (1<<2)
#define TOFF_PMC_IMR    0x006C			/* Interrupt Mask Register (R) */

/*
 *  RESET CONTROLLER (RSTC)
 */
#define TADR_BASE_RSTC  0xFFFFFD00		/* RSTC BASE Address */
#define TOFF_RSTC_CR    0x0000			/* Reset Controller Control Register (W) */
#define TOFF_RSTC_SR    0x0004			/* Reset Controller Status Register (R) */
#define TOFF_RSTC_MR    0x0008			/* Reset Controller Mode Register (R/W) */

/*
 *  REAL-TIME TIMER (RTT)
 */
#define TADR_RTT_BASE   0xFFFFFD20		/* Real-time Timer BASE address */
#define TOFF_RTT_MR     0x0000			/* Mode Register (R/W) */
#define TOFF_RTT_AR     0x0004			/* Alarm Register (R/W) */
#define TOFF_RTT_VR     0x0008			/* Value Register (R) */
#define TOFF_RTT_SR     0x000C			/* Status Register (R) */

/*
 *  PERIODIC INTERVAL TIMER (PIT)
 */
#define TADR_PIT_BASE    0xFFFFFD30		/* Periodic Interval Timer BASE Address */
#define TOFF_PIT_MR      0x0000			/* Mode Register (R/W) */
#define TOFF_PIT_SR      0x0004			/* Status Register (R) */
#define TOFF_PIT_PIVR    0x0008			/* Periodic Interval Value Register (R) */
#define TOFF_PIT_PIIR    0x000C			/* Periodic Interval Image Register (R) */

/*
 *  WATCHDOG TIMER (WDT)
 */
#define TADR_WDT_BASE    0xFFFFFD40		/* Watchdog Timer BASE address */
#define TOFF_WDT_CR      0x0000			/* Watchdog Timer Control Register (W) */
#define TOFF_WDT_MR      0x0004			/* Watchdog Timer Mode Register (R/W) */
  #define WDT_MR_WDDIS           (1<<15)
#define TOFF_WDT_SR      0x0008			/* Watchdog Timer Status Register (R) */

/*
 *  VOLTAGE REGULATOR POWER CONTROLLER (VREG)
 */
#define TADR_VREG_BASE   0xFFFFFD60		/* VREG BASE address */
#define TOFF_VREG_MR     0x0000			/* Voltage Regulator Mode Register (R/W) */
/*
 *  MEMORY CONTROLLER (MC)
 */
#define TADR_MC_BASE     0xFFFFFF00		/* Memory Controller BASE address */
#define TOFF_MC_RCR      0x0000			/* MC Remap Control Register (W) */
#define TOFF_MC_ASR      0x0004			/* MC Abort Status Register (R) */
#define TOFF_MC_AASR     0x0008			/* MC Abort Address Status Register (R) */
#define TOFF_MC_FMR      0x0060			/* MC Flash Mode Register(R/W) */
  #define MC_FMR_FWS_0FWS        (0<<8)
  #define MC_FMR_FWS_1FWS        (1<<8)
  #define MC_FMR_FWS_2FWS        (2<<8)
  #define MC_FMR_FWS_3FWS        (3<<8)
  #define MC_FMR_FMCN_SHIFT      16
#define TOFF_MC_FCR      0x0064			/* MC Flash Command Register (W) */
#define TOFF_MC_FSR      0x0068			/* MC Flash Status Register (R) */

/*
 *  TIMER COUNTER
 */
#define TADR_TC_BASE	0xFFFA0000		/* Timer Counter BASE ADDRESS */
#define TC_WINDOW       0x0040			/* Timer Counter window size */
#define TOFF_TC_CCR     0x0000			/* Channel Control Register (W) */
  #define TC_CLKEN              (1<<0)	/* (TC) Counter Clock Enable Command */
  #define TC_CLKDIS             (1<<1)	/* (TC) Counter Clock Disable Command */
  #define TC_SWTRG              (1<<2)	/* (TC) Software Trigger Command */
#define TOFF_TC_CMR     0x0004			/* Channel Mode Register (R/W) */
  #define TC_CLKS               0x7
  #define TC_CLKS_MCK2          0x0
  #define TC_CLKS_MCK8          0x1
  #define TC_CLKS_MCK32         0x2
  #define TC_CLKS_MCK128        0x3
  #define TC_CLKS_MCK1024       0x4
  #define TC_WAVESEL00          (0<<13)	/* (TC) UP mode without atomatic trigger on RC Compare */
  #define TC_WAVESEL01          (1<<13)	/* (TC) UPDOWN mode without automatic trigger on RC Compare */
  #define TC_WAVESEL10          (2<<13)	/* (TC) UP mode with automatic trigger on RC Compare */
  #define TC_WAVESEL11          (3<<13)	/* (TC) UPDOWN mode with automatic trigger on RC Compare */
  #define TC_WAVE               (1<<15)	/* (TC) Wave */
  #define TC_ACPA_NONE          (0<<16)	/* (TC) ACPA none */
  #define TC_ACPA_SET           (1<<16)	/* (TC) ACPA set */
  #define TC_ACPA_CLEAR         (2<<16)	/* (TC) ACPA clear */
  #define TC_ACPA_TOGGLE        (3<<16)	/* (TC) ACPA toggle */
  #define TC_ACPC_NONE          (0<<18)	/* (TC) ACPC none */
  #define TC_ACPC_SET           (1<<18)	/* (TC) ACPC set */
  #define TC_ACPC_CLEAR         (2<<18)	/* (TC) ACPC clear */
  #define TC_ACPC_TOGGLE        (3<<18)	/* (TC) ACPC toggle */
  #define TC_ASWTRG_NONE        (0<<22)	/* (TC) ASWTRG none */
  #define TC_ASWTRG_SET         (1<<22)	/* (TC) ASWTRG set */
  #define TC_ASWTRG_CLEAR       (2<<22)	/* (TC) ASWTRG clear */
  #define TC_ASWTRG_TOGGLE      (3<<22)	/* (TC) ASWTRG toggle */
#define TOFF_TC_CV      0x0010			/* Counter Value (R) */
#define TOFF_TC_RA      0x0014			/* Register A (R/W) */
#define TOFF_TC_RB      0x0018			/* Register B (R/W) */
#define TOFF_TC_RC      0x001C			/* Register C (R/W) */
#define TOFF_TC_SR      0x0020			/* Statis Register (R) */
  #define TC_COVFS              (1<<0)	/* (TC) Counter Overflow */
  #define TC_LOVRS              (1<<1)	/* (TC) Load Overrun */
  #define TC_CPAS               (1<<2)	/* (TC) RA Compare */
  #define TC_CPBS               (1<<3)	/* (TC) RB Compare */
  #define TC_CPCS               (1<<4)	/* (TC) RC Compare */
  #define TC_LDRAS              (1<<5)	/* (TC) RA Loading */
  #define TC_LDRBS              (1<<6)	/* (TC) RB Loading */
  #define TC_ETRGS              (1<<7)	/* (TC) External Trigger */
  #define TC_CLKSTA             (1<<16)	/* (TC) Clock Enabling */
  #define TC_MTIOA              (1<<17)	/* (TC) TIOA Mirror */
  #define TC_MTIOB              (1<<18)	/* (TC) TIOA Mirror */
#define TOFF_TC_IER     0x0024			/* Interrupt Enable Register (W) */
#define TOFF_TC_IDR     0x0028			/* Interrupt Disable Register (W) */
#define TOFF_TC_IMR     0x002C			/* Interrupt Mask Register (R) */
#define TOFF_TC_BCR     0x00C0			/* TC Block Control Register (W) */
#define TOFF_TC_BMR     0x00C4			/* TC Block Mode Register (R/W) */

/*
 *  USB DEVICE PORT (UDP)
 */
#define TADR_UDP_BASE    0xFFFB0000		/* USB Device Port BASE Address */
#define TOFF_UDP_FRM_NUM  0x0000		/* Frame Number Register (R) */
  #define UDP_FRM_NUM     (0x7FF)		/* Frame Number as Defined in the Packet Field Formats */
  #define UDP_FRM_ERR     (1<< 16)		/* Frame Error */
  #define UDP_FRM_OK      (1<< 17)		/* Frame OK */
#define TOFF_UDP_GLB_STAT 0x0004		/* Global State Register (R/W) */
  #define UDP_FADDEN      (1<<0)		/* Function Address Enable */
  #define UDP_CONFG       (1<<1)		/* Configured */
  #define UDP_ESR         (1<<2)		/* Enable Send Resume */
  #define UDP_RSMINPR     (1<<3)		/* A Resume Has Been Sent to the Host */
  #define UDP_RMWUPE      (1<<4)		/* Remote Wake Up Enable */
#define TOFF_UDP_FADDR    0x0008		/* Function Address Register (R/W) */
  #define UDP_FADD        (0xFF<<0)		/* Function Address Value */
  #define UDP_FEN         (   1<<8)		/* Function Enable */
#define TOFF_UDP_IER      0x0010		/* Interrupt Enable Register (W) */
  #define UDP_IEPINT0     (1<<0)		/* Endpoint 0 Interrupt */
  #define UDP_IEPINT1     (1<<1)		/* Endpoint 0 Interrupt */
  #define UDP_IEPINT2     (1<<2)		/* Endpoint 2 Interrupt */
  #define UDP_IEPINT3     (1<<3)		/* Endpoint 3 Interrupt */
  #define UDP_IEPINT4     (1<<4)		/* Endpoint 4 Interrupt */
  #define UDP_IEPINT5     (1<<5)		/* Endpoint 5 Interrupt */
  #define UDP_IEPINT6     (1<<6)		/* Endpoint 6 Interrupt */
  #define UDP_IEPINT7     (1<<7)		/* Endpoint 7 Interrupt */
  #define UDP_IRXSUSP     (1<<8)		/* USB Suspend Interrupt */
  #define UDP_IRXRSM      (1<<9)		/* USB Resume Interrupt */
  #define UDP_IEXTRSM     (1<<10)		/* USB External Resume Interrupt */
  #define UDP_ISOFINT     (1<<11)		/* USB Start Of frame Interrupt */
  #define UDP_IWAKEUP     (1<<13)		/* USB Walkup Interrupt */
#define TOFF_UDP_IDR      0x0014		/* Interrupt Disable Register (W) */
#define TOFF_UDP_IMR      0x0018		/* Interrupt Mask Register (R) */
#define TOFF_UDP_ISR      0x001C		/* Interrupt Status Register (R) */
  #define UDP_ENDBUSRES   (1<<12)		/* USB End Of Bus Reset Interrupt */
#define TOFF_UDP_ICR      0x0020		/* Interrupt Clear Register (W) */
#define TOFF_UDP_RST_EP   0x0028		/* Reset Endpoint Register (R/W) */
  #define UDP_EP0         (1<<0)		/* Reset Endpoint 0 */
  #define UDP_EP1         (1<<1)		/* Reset Endpoint 1 */
  #define UDP_EP2         (1<<2)		/* Reset Endpoint 2 */
  #define UDP_EP3         (1<<3)		/* Reset Endpoint 3 */
  #define UDP_EP4         (1<<4)		/* Reset Endpoint 4 */
  #define UDP_EP5         (1<<5)		/* Reset Endpoint 5 */
  #define UDP_EP6         (1<<6)		/* Reset Endpoint 6 */
  #define UDP_EP7         (1<<7)		/* Reset Endpoint 7 */
#define TOFF_UDP_CSR      0x0030		/* Endpoint Control Status Register (R/W) */
#define TOFF_UDP_CSR0     0x0030		/* Endpoint0 Control Status Register (R/W) */
#define TOFF_UDP_CSR1     0x0034		/* Endpoint1 Control Status Register (R/W) */
#define TOFF_UDP_CSR2     0x0038		/* Endpoint2 Control Status Register (R/W) */
#define TOFF_UDP_CSR3     0x003C		/* Endpoint3 Control Status Register (R/W) */
  #define UDP_TXCOMP      (1<<0)		/* Generates an IN packet with data previously written in the DPR */
  #define UDP_RX_DATA_BK0 (1<<1)		/* Receive Data Bank 0 */
  #define UDP_RXSETUP     (1<<2)		/* Sends STALL to the Host (Control endpoints) */
  #define UDP_ISOERROR    (1<<3)		/* Isochronous error (Isochronous endpoints) */
  #define UDP_TXPKTRDY    (1<<4)		/* Transmit Packet Ready */
  #define UDP_FORCESTALL  (1<<5)		/* Force Stall (used by Control, Bulk and Isochronous endpoints). */
  #define UDP_RX_DATA_BK1 (1<<6)		/* Receive Data Bank 1 (only used by endpoints with ping-pong attributes). */
  #define UDP_DIR         (1<<7)		/* Transfer Direction */
  #define UDP_EPTYPE      (7<<8)		/* Endpoint type */
  #define UDP_EPTYPE_CTRL     (0<<8)	/* Control */
  #define UDP_EPTYPE_ISO_OUT  (1<<8)	/* Isochronous OUT */
  #define UDP_EPTYPE_BULK_OUT (2<<8)	/* Bulk OUT */
  #define UDP_EPTYPE_INT_OUT  (3<<8)	/* Interrupt OUT */
  #define UDP_EPTYPE_ISO_IN   (5<<8)	/* Isochronous IN */
  #define UDP_EPTYPE_BULK_IN  (6<<8)	/* Bulk IN */
  #define UDP_EPTYPE_INT_IN   (7<<8)	/* Interrupt IN */
  #define UDP_DTGLE       (1<<11)		/* Data Toggle */
  #define UDP_EPEDS       (1<<15)		/* Endpoint Enable Disable */
  #define UDP_RXBYTECNT   (0x7FF<<16)	/* Number Of Bytes Available in the FIFO */
#define TOFF_UDP_FDR      0x0050		/* Endpoint FIFO Data Register (R/W) */
#define TOFF_UDP_FDR0     0x0050		/* Endpoint0 FIFO Data Register (R/W) */
#define TOFF_UDP_FDR1     0x0054		/* Endpoint1 FIFO Data Register (R/W) */
#define TOFF_UDP_FDR2     0x0058		/* Endpoint2 FIFO Data Register (R/W) */
#define TOFF_UDP_FDR3     0x005C		/* Endpoint3 FIFO Data Register (R/W) */
#define TOFF_UDP_TXVC     0x0074		/* Transmitter Control Register (R/W) */
  #define UDP_TXVDIS      (1<<8)		/* */
  #define UDP_PUON        (1<<9)		/* Pull-up ON */

/*
 * TWO-WIRE INTERFACE (TWI)
 */
#define TADR_TWI_BASE    0xFFFB8000		/* Two-wire Interface BASE address */
#define TOFF_TWI_CR      0x0000			/* Control Register (W) */
  #define AT91S_TWI_START       (1<<0)	/* (TWI) Send a START Condition */
  #define AT91S_TWI_STOP        (1<<1)	/* (TWI) Send a STOP Condition */
  #define AT91S_TWI_MSEN        (1<<2)	/* (TWI) TWI Master Transfer Enabled */
#define TOFF_TWI_MMR     0x0004			/* Master Mode Register (R/W) */
#define TOFF_TWI_IADR    0x000C			/* Internal Address Register (R/W) */
#define TOFF_TWI_CWGR    0x0010			/* Clock Wavefrom Generator Register (R/W) */
#define TOFF_TWI_SR      0x0020			/* Status Register (R) */
  #define AT91S_TWI_RXRDY       (1<<1)	/* (TWI) Receive holding register ReaDY */
  #define AT91S_TWI_TXRDY       (1<<2)	/* (TWI) Transmit holding register ReaDY */
  #define AT91C_TWI_OVRE        (1<<6)	/* (TWI) Overrun Error */
  #define AT91C_TWI_UNRE        (1<<7)	/* (TWI) Underrun Error */
  #define AT91S_TWI_NACK        (1<<8)	/* (TWI) Not Acknowledged */
#define TOFF_TWI_IER     0x0024			/* Interrupt Enable Register (W) */
#define TOFF_TWI_IDR     0x0028			/* Interrupt Disable Register (W) */
#define TOFF_TWI_IMR     0x002C			/* Interrupt Mask Register (R) */
#define TOFF_TWI_RHR     0x0030			/* Receive Holding Register (R) */
#define TOFF_TWI_THR     0x0034			/* Transmit Holding Register (R/W) */

/*
 *  UNIVERSAL SYNCHRONOUS ASYNCHRONOUS RECEIVER TRANSMITTER(USART)
 */
#define TADR_US_BASE    0xFFFC0000		/* USART BASE address */
#define US_WINDOW       0x4000			/* USART Window size */
#define TOFF_US_CR      0x0000			/* Control Register (W) */
  #define US_RSTRX      0x0004			/* Reset Receiver */
  #define US_RSTTX      0x0008			/* Reset Transmitter */
  #define US_RXEN       0x0010			/* Receiver Enable */
  #define US_RXDIS      0x0020			/* Receiver Disable */
  #define US_TXEN       0x0040			/* Transmitter Enable */
  #define US_TXDIS      0x0080			/* Transmitter Disable */
  #define US_RSTSTA     0x0100			/* Reset Status Bits */
  #define US_STTBRK     0x0200			/* Start Break */
  #define US_STPBRK     0x0400			/* Stop Break */
  #define US_STTTO      0x0800			/* Start Time-out */
  #define US_SENDA      0x1000			/* Send Address */
#define TOFF_US_MR      0x0004			/* Mode Register (R/W) */
  #define US_USMODE     0x000F			/* Uart mode */
  #define US_USMODE_NORMAL   0x0000		/* Normal */
  #define US_USMODE_RS485    0x0001		/* RS485 */
  #define US_USMODE_HWHSH    0x0002		/* Hardware Handshaking */
  #define US_USMODE_MODEM    0x0003		/* Modem */
  #define US_USMODE_ISO7816_0 0x0004	/* ISO7816 protocol: T = 0 */
  #define US_USMODE_ISO7816_1 0x0006	/* ISO7816 protocol: T = 1 */
  #define US_USMODE_IRDA     0x0008		/* IrDA */
  #define US_USMODE_SWHSH    0x000C		/* Software Handshaking */
  #define US_CLKS       0x0030			/* Clock Selection */
  #define US_CLKS_MCK        0x0000		/* Master Clock */
  #define US_CLKS_MCK8       0x0010		/* Master Clock divided by 8 */
  #define US_CLKS_SCK        0x0020		/* External Clock */
  #define US_CLKS_SLCK       0x0030		/* Slow Clock */
  #define US_CHRL       0x00C0			/* Byte Length */
  #define US_CHRL_5          0x0000		/* 5 bits */
  #define US_CHRL_6          0x0040		/* 6 bits */
  #define US_CHRL_7          0x0080		/* 7 bits */
  #define US_CHRL_8          0x00C0		/* 8 bits */
  #define US_PAR        0x0E00			/* Parity Mode */
  #define US_PAR_EVEN         0x0000	/* Even Parity */
  #define US_PAR_ODD          0x0200	/* Odd Parity */
  #define US_PAR_SPACE        0x0400	/* Space Parity to 0 */
  #define US_PAR_MARK         0x0600	/* Marked Parity to 1 */
  #define US_PAR_NO           0x0800	/* No Parity */
  #define US_PAR_MULTIDROP    0x0C00	/* Multi-drop Mode */
  #define US_NBSTOP     0x3000			/* Stop Bit Number */
  #define US_NBSTOP_1         0x0000	/* 1 Stop Bit */
  #define US_NBSTOP_1_5       0x1000	/* 1.5 Stop Bits */
  #define US_NBSTOP_2         0x2000	/* 2 Stop Bits */
  #define US_CHMODE     0xC000			/* Channel Mode */
  #define US_CHMODE_NORMAL          0x0000	/* Normal Mode */
  #define US_CHMODE_AUTOMATIC_ECHO  0x4000  /* Automatic Echo */
  #define US_CHMODE_LOCAL_LOOPBACK  0x8000  /* Local Loopback */
  #define US_CHMODE_REMOTE_LOOPBACK 0xC000  /* Remote Loopback */
  #define US_MODE9      0x020000		/* 9-bit Character length */
  #define US_CKLO       0x040000		/* Clock Output Select */
  #define US_MSBF       0x010000		/* Bit Order */
  #define US_OVER       0x080000		/* Over Sampling mode */
#define TOFF_US_IER     0x0008			/* Interrupt Enable Register (W) */
#define TOFF_US_IDR     0x000C			/* Interrupt Disable Register (W) */
#define TOFF_US_IMR     0x0010			/* Interrupt Mask Register (R) */
#define TOFF_US_CSR     0x0014			/* Channel Staus Register (R) */
  #define US_RXRDY      0x0001			/* Receiver Ready */
  #define US_TXRDY      0x0002			/* Transmitter Ready */
  #define US_RXBRK      0x0004			/* Receiver Break */
  #define US_ENDRX      0x0008			/* End of Receiver PDC Transfer */
  #define US_ENDTX      0x0010			/* End of Transmitter PDC Transfer */
  #define US_OVRE       0x0020			/* Overrun Error */
  #define US_FRAME      0x0040			/* Framing Error */
  #define US_PARE       0x0080			/* Parity Error */
  #define US_TIMEOUT    0x0100			/* Receiver Timeout */
  #define US_TXEMPTY    0x0200			/* Transmitter Empty */
  #define US_TXBUFE     0x0800			/* Transmitter Buffer Empty */
  #define US_RXBUFF     0x1000			/* Receiver Buffer full */
#define TOFF_US_RHR     0x0018			/* Receiver Holding Register (R) */
#define TOFF_US_THR     0x001C			/* Transmitter Holding Register (W) */
#define TOFF_US_BRGR    0x0020			/* Baud Rate Generator Register (R/W) */

#define TOFF_US_RTOR    0x0024			/* Receiver Time-out Register (R/W) */
#define TOFF_US_TTGR    0x0028			/* Transmitter Timeguard Register (R/W) */
#define TOFF_US_FIDI    0x0040			/* FIDI Ratio Register (R/W) */
#define TOFF_US_NER     0x0044			/* Number of Errors Register (R) */
#define TOFF_US_IF      0x004C			/* IrDA Filter Register (R/W) */
#define TOFF_US_MAN     0x0050			/* Manchester Encoder Decoder Register (R/W) */

/*
 *  PUSE WIDTH MODULATION CONTROLLER
 */
#define TADR_PWM_BASE   0xFFFCC000		/* Pluse Widh Modulation Controller BASE address */
#define TOFF_PWM_MR     0x0000			/* PWM Mode Register (R/W) */
#define TOFF_PWM_ENA    0x0004			/* PWM Enable Register (W) */
#define TOFF_PWM_DIS    0x0008			/* PWM Disable Register (W) */
#define TOFF_PWM_SR     0x000C			/* PWM Status Register (R) */
#define TOFF_PWM_IER    0x0010			/* PWM Interrupt Enable Register (W) */
#define TOFF_PWM_IDR    0x0014			/* PWM Interrupt Disable Register (W) */
#define TOFF_PWM_IMR    0x0018			/* PWM Interrupt Mask Register (R) */
#define TOFF_PWM_ISR    0x001C			/* PWM Interrupt Status Register (R) */
#define TADR_PWMC_BASE  0xFFFCC200		/* PWM Channel0 BASE address */
#define CN_WINDOW       0x0020			/* PWM Channel Window size */
#define TADR_PWMC0_BASE (TADR_PWMC_BASE)
#define TADR_PWMC1_BASE (TADR_PWMC_BASE+CN_WINDOW)
#define TOFF_PWM_CMR    0x0000			/* PWM Channel Mode Register (R/W) */
#define TOFF_PWM_CDTY   0x0004			/* PWM Channel Duty Cycle Register (R/W) */
#define TOFF_PWM_CPRD   0x0008			/* PWM Channel Period Register (R/W) */
#define TOFF_PWM_CCNT   0x000C			/* PWM Channel Counter Register (R) */
#define TOFF_PWM_CUPD   0x0010			/* PWM Channel Update Register (W) */


/*
 *  SYNCHRONOUS SERIAL CONTROLLER (SSC)
 */
#define TADR_SSC_BASE    0xFFFD4000		/* Synchronous Serial Controller BASE Address */
#define TOFF_SSC_CR      0x0000			/* Control Register (W) */
  #define SSC_RXEN              (1<<0)	/* (SSC) Receive Enable */
  #define SSC_RXDIS             (1<<1)	/* (SSC) Receive Disable */
  #define SSC_TXEN              (1<<8)	/* (SSC) Transmit Enable */
  #define SSC_TXDIS             (1<<9)	/* (SSC) Transmit Disable */
  #define SSC_SWRST             (1<<15)	/* (SSC) Software Reset */
#define TOFF_SSC_CMR     0x0004			/* Clock Mode Register (R/W) */
#define TOFF_SSC_RCMR    0x0010			/* Receive Clock Mode Register (R/W) */
  #define SSC_CKS_DIV           (0<<0)	/* (SSC) Divided Clock */
  #define SSC_CKS_TK            (1<<0)	/* (SSC) TK Clock signal */
  #define SSC_CKS_RK            (2<<0)	/* (SSC) RK pin */
  #define SSC_CKO_NONE          (0<<2)	/* (SSC) Receive/Transmit Clock Output Mode: None RK pin: Input-only */
  #define SSC_CKO_CONTINOUS     (1<<2)	/* (SSC) Continuous Receive/Transmit Clock RK pin: Output */
  #define SSC_CKO_DATA_TX       (2<<2)	/* (SSC) Receive/Transmit Clock only during data transfers RK pin: Output */
  #define SSC_START_CONTINOUS   (0<<8)	/* (SSC) Continuous, as soon as the receiver is enabled, and immediately after the end of transfer of the previous data. */
  #define SSC_START_TX          (1<<8)	/* (SSC) Transmit/Receive start */
  #define SSC_START_LOW_RF      (2<<8)	/* (SSC) Detection of a low level on RF input */
  #define SSC_START_HIGH_RF     (3<<8)	/* (SSC) Detection of a high level on RF input */
  #define SSC_START_FALL_RF     (4<<8)	/* (SSC) Detection of a falling edge on RF input */
  #define SSC_START_RISE_RF     (5<<8)	/* (SSC) Detection of a rising edge on RF input */
  #define SSC_START_LEVEL_RF    (6<<8)	/* (SSC) Detection of any level change on RF input */
  #define SSC_START_EDGE_RF     (7<<8)	/* (SSC) Detection of any edge on RF input */
  #define SSC_START_0           (8<<8)	/* (SSC) Compare 0 */
#define TOFF_SSC_RFMR    0x0014			/* Receive Frame Mode Register (R/W) */
  #define SSC_LOOP              (1<<5)	/* (SSC) Loop Mode */
  #define SSC_MSBF              (1<<7)	/* (SSC) Most Significant Bit First */
#define TOFF_SSC_TCMR    0x0018			/* Transmit Clock Mode Register (R/W) */
#define TOFF_SSC_TFMR    0x001C			/* Transmit Frame Mode Register (R/W) */
#define TOFF_SSC_RHR     0x0020			/* Receive Holding Register (R) */
#define TOFF_SSC_THR     0x0024			/* Transmit Holding Register (R) */
#define TOFF_SSC_RSHR    0x0030			/* Receive Sync. Holding Register (R) */
#define TOFF_SSC_TSHR    0x0034			/* Transmit Sync. Holding Register (R/W) */
#define TOFF_SSC_RC0R    0x0038			/* Receive Compare 0 Register (R/W) */
#define TOFF_SSC_RC1R    0x003C			/* Receive Compare 1 Register (R/W) */
#define TOFF_SSC_SR      0x0040			/* Status Register (R) */
  #define SSC_TXRDY             (1<<0)	/* (SSC) Transmit Ready */
  #define SSC_TXEMPTY           (1<<1)	/* (SSC) Transmit Empty */
  #define SSC_ENDTX             (1<<2)	/* (SSC) End Of Transmission */
  #define SSC_TXBUFE            (1<<3)	/* (SSC) Transmit Buffer Empty */
  #define SSC_RXRDY             (1<<4)	/* (SSC) Receive Ready */
  #define SSC_OVRUN             (1<<5)	/* (SSC) Receive Overrun */
  #define SSC_ENDRX             (1<<6)	/* (SSC) End of Reception */
  #define SSC_RXBUFF            (1<<7)	/* (SSC) Receive Buffer Full */
  #define SSC_CP0               (1<<8)	/* (SSC) Compare 0 */
  #define SSC_CP1               (1<<9)	/* (SSC) Compare 1 */
  #define SSC_TXSYN             (1<<10)	/* (SSC) Transmit Sync */
  #define SSC_RXSYN             (1<<11)	/* (SSC) Receive Sync */
  #define SSC_TXENA             (1<<16)	/* (SSC) Transmit Enable */
  #define SSC_RXENA             (1<<17)	/* (SSC) Receive Enable */
#define TOFF_SSC_IER     0x0044			/* Interrupt Enable Register (W) */
#define TOFF_SSC_IDR     0x0048			/* Interrupt Disable Register (W) */
#define TOFF_SSC_IMR     0x004C			/* Interrupt Mask Register (R) */

#define TOFF_SSC_RPR     0x0100			/* (PDC_SSC) Receive Pointer Register */
#define TOFF_SSC_RCR     0x0104			/* (PDC_SSC) Receive Counter Register */
#define TOFF_SSC_TPR     0x0108			/* (PDC_SSC) Transmit Pointer Register */
#define TOFF_SSC_TCR     0x010C			/* (PDC_SSC) Transmit Counter Register */
#define TOFF_SSC_RNPR    0x0110			/* (PDC_SSC) Receive Next Pointer Register */
#define TOFF_SSC_RNCR    0x0114			/* (PDC_SSC) Receive Next Counter Register */
#define TOFF_SSC_TNPR    0x0118			/* (PDC_SSC) Transmit Next Pointer Register */
#define TOFF_SSC_TNCR    0x011C			/* (PDC_SSC) Transmit Next Counter Register */
#define TOFF_SSC_PTCR    0x0120			/* (PDC_SSC) PDC Transfer Control Register */
  #define PDC_RXTEN             (1<<0)	/* (PDC) Receiver Transfer Enable */
  #define PDC_RXTDIS            (1<<1)	/* (PDC) Receiver Transfer Disable */
  #define PDC_TXTEN             (1<<8)	/* (PDC) Transmitter Transfer Enable */
  #define PDC_TXTDIS            (1<<9)	/* (PDC) Transmitter Transfer Disable */
#define TOFF_SSC_PTSR    0x0124			/* (PDC_SSC) PDC Transfer Status Register */

/*
 *  ANALOG-TO-DIGITAL CONVERTER (ADC)
 */
#define TADR_ADC_BASE    0xFFFD8000		/* Analog-to-digital Converter BASE address */
#define TOFF_ADC_CR      0x0000			/* Control Register (W) */
#define TOFF_ADC_MR      0x0004			/* Mode Register (R/W) */
  #define ADC_TRGEN      0x0001			/* Trigger Enable */
  #define ADC_TRGSEL     0x000E			/* Trigger Select */
  #define ADC_TRGSEL_TIOA0      (0<<1)	/* Trigger Select TIOA0 */
  #define ADC_TRGSEL_TIOA1      (1<<1)	/* Trigger Select TIOA1 */
  #define ADC_TRGSEL_TIOA2      (2<<1)	/* Trigger Select TIOA2 */
  #define ADC_TRGSEL_EXT        (6<<1)	/* Trigger Select External */
#define TOFF_ADC_CHER    0x0010			/* Channel Enable Register (W) */
  #define ADC_CH0        (1<<0)			/* Channel 0 */
  #define ADC_CH1        (1<<1)			/* Channel 1 */
  #define ADC_CH2        (1<<2)			/* Channel 2 */
  #define ADC_CH3        (1<<3)			/* Channel 3 */
  #define ADC_CH4        (1<<4)			/* Channel 4 */
  #define ADC_CH5        (1<<5)			/* Channel 5 */
  #define ADC_CH6        (1<<6)			/* Channel 6 */
  #define ADC_CH7        (1<<7)			/* Channel 7 */
#define TOFF_ADC_CHDR    0x0014			/* Channel Disable Register (W) */
#define TOFF_ADC_CHSR    0x0018			/* Channel Status Register (R) */
#define TOFF_ADC_SR      0x001C			/* Status Register (R) */
#define TOFF_ADC_LCDR    0x0020			/* Last Converted Data Register (R) */
#define TOFF_ADC_IER     0x0024			/* Interrupt Enable Register (W) */
#define TOFF_ADC_IDR     0x0028			/* Interrupt Disable Register (W) */
#define TOFF_ADC_IMR     0x002C			/* Interrupt Mask Register (R) */
#define TOFF_ADC_CDR     0x0030			/* Channel Data Register (R) */
#define TOFF_ADC_CDR0    0x0030			/* Channel Data Register0 (R) */
#define TOFF_ADC_CDR1    0x0034			/* Channel Data Register1 (R) */
#define TOFF_ADC_CDR2    0x0038			/* Channel Data Register2 (R) */
#define TOFF_ADC_CDR3    0x003C			/* Channel Data Register3 (R) */
#define TOFF_ADC_CDR4    0x0040			/* Channel Data Register4 (R) */
#define TOFF_ADC_CDR5    0x0044			/* Channel Data Register5 (R) */
#define TOFF_ADC_CDR6    0x0048			/* Channel Data Register6 (R) */
#define TOFF_ADC_CDR7    0x004C			/* Channel Data Register7 (R) */

/*
 *  SERIAL PERIPHERAL INTERFACE (SPI)
 */
#define TADR_SPI_BASE    0xFFFE0000		/* Serial Peripheral Interfcae BASE Address */
#define TOFF_SPI_CR      0x0000			/* Control Register (W) */
  #define SPI_SPIEN             (1<<0)	/* (SPI) SPI Enable */
  #define SPI_SPIDIS            (1<<1)	/* (SPI) SPI Disable */
  #define SPI_SWRST             (1<<7)	/* (SPI) SPI Software reset */
  #define SPI_LASTXFER          (1<<24)	/* (SPI) SPI Last Transfer */
#define TOFF_SPI_MR      0x0004			/* Mode Register (R/W) */
  #define SPI_MSTR              (1<<0)	/* (SPI) Master/Slave Mode */
  #define SPI_PS                (1<<1)	/* (SPI) Peripheral Select */
  #define SPI_PCSDEC            (1<<2)	/* (SPI) Chip Select Decode */
  #define SPI_FDIV              (1<<3)	/* (SPI) Clock Selection */
  #define SPI_MODFDIS           (1<<4)	/* (SPI) Mode Fault Detection */
  #define SPI_LLB               (1<<7)	/* (SPI) Clock Selection */
#define TOFF_SPI_RDR     0x0008			/* Receive Data Register (R) */
#define TOFF_SPI_TDR     0x000C			/* Transmit Data Register (W) */
#define TOFF_SPI_SR      0x0010			/* Status Register (R) */
  #define SPI_ENDTX             (1<<5)	/* (SPI) End of Receiver Transfer */
#define TOFF_SPI_IER     0x0014			/* Interrupt Enable Register (W) */
#define TOFF_SPI_IDR     0x0018			/* Interrupt Disable Register (W) */
#define TOFF_SPI_IMR     0x001C			/* Interrupt Mask Register (R) */
#define TOFF_SPI_CSR0    0x0030			/* Chip Select Register0 (R/W) */
#define TOFF_SPI_CSR1    0x0034			/* Chip Select Register1 (R/W) */
#define TOFF_SPI_CSR2    0x0038			/* Chip Select Register2 (R/W) */
#define TOFF_SPI_CSR3    0x003C			/* Chip Select Register3 (R/W) */
  #define SPI_CPOL             (1<<0)	/* (SPI) Clock Polarity */
  #define SPI_NCPHA            (1<<1)	/* (SPI) Clock Phase */
  #define SPI_CSAAT            (1<<2)	/* (SPI) Chip Select Active After Transfer */

#define TOFF_SPI_RPR     0x0100			/* (PDC_SPI) Receive Pointer Register */
#define TOFF_SPI_RCR     0x0104			/* (PDC_SPI) Receive Counter Register */
#define TOFF_SPI_TPR     0x0108			/* (PDC_SPI) Transmit Pointer Register */
#define TOFF_SPI_TCR     0x010C			/* (PDC_SPI) Transmit Counter Register */
#define TOFF_SPI_RNPR    0x0110			/* (PDC_SPI) Receive Next Pointer Register */
#define TOFF_SPI_RNCR    0x0114			/* (PDC_SPI) Receive Next Counter Register */
#define TOFF_SPI_TNPR    0x0118			/* (PDC_SPI) Transmit Next Pointer Register */
#define TOFF_SPI_TNCR    0x011C			/* (PDC_SPI) Transmit Next Counter Register */
#define TOFF_SPI_PTCR    0x0120			/* (PDC_SPI) PDC Transfer Control Register */
#define TOFF_SPI_PTSR    0x0124			/* (PDC_SPI) PDC Transfer Status Register */

/*
 *  PERIPHERAL DMA CONTROLLER (PDC)
 */
#define TOFF_PDC_RPR    0x0100			/* Receive Pointer Register (R/W) */
#define TOFF_PDC_RCR    0x0104			/* Receive Counter Register (R/W) */
#define TOFF_PDC_TPR    0x0108			/* Transmit Pointer Register (R/W) */
#define TOFF_PDC_TCR    0x010C			/* Transmit Counter Register (R/W) */
#define TOFF_PDC_RNPR   0x0110			/* Receive Next Pointer Register (R/W) */
#define TOFF_PDC_RNCR   0x0114			/* Receive Next Counter Register (R/W) */
#define TOFF_PDC_TNPR   0x0118			/* Transmit Next Pointer Register (R/W) */
#define TOFF_PDC_TNCR   0x011C			/* Transmit Next Counter Register (R/W) */
#define TOFF_PDC_PTCR   0x0120			/* PDC Transfar Control Register (W) */
  #define PDC_RXTEN            (1<<0)	/* (PDC) Receiver Transfer Enable */
  #define PDC_RXTDIS           (1<<1)	/* (PDC) Receiver Transfer Disable */
  #define PDC_TXTEN            (1<<8)	/* (PDC) Transmitter Transfer Enable */
  #define PDC_TXTDIS           (1<<9)	/* (PDC) Transmitter Transfer Disable */
#define TOFF_PDC_PTSR   0x0124			/* PDC Transfar Status Register (R) */


#if 0	/* ROI DEBUG */
// 256kbytes,1024pages of 256bytes
#define  FLASH_PAGE_NB          1024
#define  FLASH_PAGE_LOCK        65
#define  FLASH_PAGE_SIZE        256
#define  FLASH_PAGE_SIZE_BYTE   256
#define  FLASH_PAGE_SIZE_LONG   64
// 16lockbits, protecting 16sectors of 64pages
#define  FLASH_LOCK_BITS_SECTOR 16
#define  FLASH_SECTOR_PAGE      64
#define  FLASH_LOCK_BITS        16

#define FLASH_BASE_ADDRESS      0x00100000


#endif	/* ROI DEBUG */

#define MCK	                    48054850

#ifndef _MACRO_ONLY

/*
 * 内蔵UART用 簡易SIOドライバ
 */
#define NUM_SNDBUF  2
#define SIORBUFSIZE 128
#define SIOSBUFSIZE 256
#define SIOBUFSIZE  126

#define SND_EMPY    0
#define SND_RDY     1
#define SND_OUT     2

/*
 * カーネル起動時用の初期化(sys_putcを使用するため)
 */
extern void init_uart(void);


/*
 *  シリアルI/Oポートパケット
 */
typedef struct sio_packet
{
	H  id;
	UH len;
	UB buf[SIOSBUFSIZE];
}
SIOPACKET;

/*
 *  シリアルI/Oポート初期化ブロック
 */
typedef struct sio_port_initialization_block 
{
    VP uart_base;
    VP time_base;
}
SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block 
{
    const SIOPINIB  *siopinib;  /* シリアルI/Oポート初期化ブロック */
    VP_INT exinf;				/* 拡張情報 */
    BOOL   openflag;			/* オープン済みフラグ */
	BOOL   intflag;				/* 割込み状態 */
	INT    connect_state;		/* BLUETOOTH 接続状態 */
	UH     rcv_idx;				/* 受信カウンタインデックス */
	UB     rcv_no;				/* 受信バッファインデックス */
    UB     snd_stat[3];			/* 送信バッファステータス */
	H      bcount;				/* バッファデータ数 */
	H      obcount;				/* ひとつ前の bcount */
	UB     rcv_buf[2][SIORBUFSIZE];	/* 受信バッファ */
	SIOPACKET  snd_buf[NUM_SNDBUF];	/* 送信バッファ */
}SIOPCB;

#define siobuffer       snd_buf[0].buf

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_ERDY_SND    1u      /* 送信可能コールバック */
#define SIO_ERDY_RCV    2u      /* 受信通知コールバック */

/*
 *  BLUETOOTH用接続状態定義
 */
#define BLUETOOTH_DEVICE_INITIALIZED    3
#define BLUETOOTH_NO_INIT               4
#define BLUETOOTH_INITIALIZED           5
#define BLUETOOTH_CONNECTED             6
#define BLUETOOTH_STREAM                7
#define BLUETOOTH_CONNECT_ABORT         8
#define BLUETOOTH_CONNECT_ERROR         9

extern ER_BOOL bluetooth_send_ready(ID portid);
extern ER_UINT bluetooth_received_count(ID portid);
extern ER      bluetooth_send(ID portid, SIOPACKET *p);
extern ER_UINT bluetooth_receive(ID portid, unsigned char *buf);
extern ER      bluetooth_get_packet(ID portid, SIOPACKET **ppkt, INT *plen);
extern ER_UINT bluetooth_get_connect_state(ID portid);
extern ER      bluetooth_set_connect_state(ID portid, INT state);
extern ER      bluetooth_reset(ID portid);
extern UB      *bluetooth_get_incode(void);
extern UB      *bluetooth_get_device_address(void);


/*
 *  SIOドライバの初期化ルーチン
 */
extern void uart_initialize(void);

/*
 *  オープンしているポートがあるか？
 */
extern BOOL uart_openflag(void);

/*
 *  シリアルI/Oポートのオープン
 */
extern SIOPCB   *uart_opn_por(ID siopid, VP_INT exinf);

/*
 *  シリアルI/Oポートのクローズ
 */
extern void uart_cls_por(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートへの文字送信
 */
extern BOOL uart_snd_chr(SIOPCB *siopcb, char c);

/*
 *  シリアルI/Oポートからの文字受信
 */
extern INT  uart_rcv_chr(SIOPCB *siopcb);

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
extern void uart_ena_cbr(SIOPCB *siopcb, UINT cbrtn);

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
extern void uart_dis_cbr(SIOPCB *siopcb, UINT cbrtn);

/*
 *  SIOの割込みサービスルーチン
 */
extern void uart_in_isr(void);
extern void uart_out_isr(void);

/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void uart_ierdy_snd(VP_INT exinf);

/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void uart_ierdy_rcv(VP_INT exinf);

#endif /* _MACRO_ONLY */

#endif /* _MINDSTORMSNXT_H_ */
