/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2010      by GJ Designing Center Print Business Group
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
 *  @(#) $Id: lpc23xx.h,v 1.1 2010/06/04 16:14:24 roi Exp $
 */

#ifndef _LPC23XX_H_
#define _LPC23XX_H_

#ifndef _MACRO_ONLY
#include <itron.h>
#include <sil.h>
#endif /* _MACRO_ONLY */

#include "armv4.h"
#include "sys_config.h"
#include "cpu_config.h"

/*
 *  INTERRUPT NUMBER
 */
#define INTNO_WDT           0	/* Watchdog interrupt(WDINT) */
#define INTNO_ARMCORE0      2	/* Embedded ICE, DbgCommRx */
#define INTNO_ARMCORE1      3	/* Embedded ICE, DbgCommTx */
#define INTNO_TIMER0        4	/* Timer 0 */
#define INTNO_TIMER1        5	/* Timer 1 */
#define INTNO_UART0         6	/* Universal Asynchronus Receiver/Transmitter 0 */
#define INTNO_UART1         7	/* Universal Asynchronus Receiver/Transmitter 1 */
#define INTNO_PWM1          8
#define INTNO_I2C0          9	/* I2C Interface 0 */
#define INTNO_SPI0          10	/* SPI */
#define INTNO_SSP0          10	/* SSP 0 */
#define INTNO_SSP1          11	/* SSP 1 */
#define INTNO_PLL           12	/* PLL Lock(PLOCK) */
#define INTNO_RTC           13	/* Real Time Clock */
#define INTNO_EINT0         14	/* External Interrupt 0 */
#define INTNO_EINT1         15	/* External Interrupt 1 */
#define INTNO_EINT2         16	/* External Interrupt 2 */
#define INTNO_EINT3         17	/* External Interrupt 3 */
#define INTNO_ADC0          18	/* A/D Converter 0 end of converion */
#define INTNO_I2C1          19	/* I2C Interface 1 */
#define INTNO_BOD           20	/* Brown Out detect */
#define INTNO_ETHER         21
#define INTNO_USB           22
#define INTNO_CAN           23	/* CAN Controller 1/2 */
#define INTNO_MMC           24	/* MMC */
#define INTNO_GPDMA         25	/* IntStatus of DMA channel 0-1 */
#define INTNO_TIMER2        26	/* Timer 2 */
#define INTNO_TIMER3        27	/* Timer 3 */
#define INTNO_UART2         28	/* Universal Asynchronus Receiver/Transmitter 2 */
#define INTNO_UART3         29	/* Universal Asynchronus Receiver/Transmitter 3 */
#define INTNO_I2C2          30	/* I2C Interface 2 */
#define INTNO_I2S           31	/* I2S Interface */


/*
 *  Pin Connect Block
 */
#define TADR_PINSEL_BASE    0xE002C000
#define TOFF_PINSEL0        0x0000		/* (RW) Pin function select register 0 */
#define TOFF_PINSEL1        0x0004		/* (RW) Pin function select register 1 */
#define TOFF_PINSEL2        0x0008		/* (RW) Pin function select register 2 */
#define TOFF_PINSEL3        0x000C		/* (RW) Pin function select register 3 */
#define TOFF_PINSEL4        0x0010		/* (RW) Pin function select register 4 */
#define TOFF_PINSEL5        0x0014		/* (RW) Pin function select register 5 */
#define TOFF_PINSEL6        0x0018		/* (RW) Pin function select register 6 */
#define TOFF_PINSEL7        0x001C		/* (RW) Pin function select register 7 */
#define TOFF_PINSEL8        0x0020		/* (RW) Pin function select register 8 */
#define TOFF_PINSEL9        0x0024		/* (RW) Pin function select register 9 */
#define TOFF_PINSEL10       0x0028		/* (RW) Pin function select register 10 */

#define TOFF_PINMODE0       0x0040		/* (RW) Pin mode select register 0 */
#define TOFF_PINMODE1       0x0044		/* (RW) Pin mode select register 1 */
#define TOFF_PINMODE2       0x0048		/* (RW) Pin mode select register 2 */
#define TOFF_PINMODE3       0x004C		/* (RW) Pin mode select register 3 */
#define TOFF_PINMODE4       0x0050		/* (RW) Pin mode select register 4 */
#define TOFF_PINMODE5       0x0054		/* (RW) Pin mode select register 5 */
#define TOFF_PINMODE6       0x0058		/* (RW) Pin mode select register 6 */
#define TOFF_PINMODE7       0x005C		/* (RW) Pin mode select register 7 */
#define TOFF_PINMODE8       0x0060		/* (RW) Pin mode select register 8 */
#define TOFF_PINMODE9       0x0064		/* (RW) Pin mode select register 9 */

/*
 *  General Purpose Input/Output (GPIO)
 */
#define TADR_GPIO_BASE      0xE0028000
#define TOFF_GPIO_IOPIN0    0x0000		/* (RW) GPIO Port Pin value register */
#define TOFF_GPIO_IOSET0    0x0004		/* (RW) GPIO Port Output Set register */
#define TOFF_GPIO_IODIR0    0x0008		/* (RW) GPIO Port Direction control register */
#define TOFF_GPIO_IOCLR0    0x000C		/* (WO) GPIO Port Output Clear register */
#define TOFF_GPIO_IOPIN1    0x0010		/* (RW) GPIO Port Pin value register */
#define TOFF_GPIO_IOSET1    0x0014		/* (RW) GPIO Port Output Set register */
#define TOFF_GPIO_IODIR1    0x0018		/* (RW) GPIO Port Direction control register */
#define TOFF_GPIO_IOCLR1    0x001C		/* (WO) GPIO Port Output Clear register */

/* GPIO Interrupt Registers */
#define TOFF_GPIO_IO0_INTENR 0x0090	/* (RW) GPIO Interrupt Enable for Rising edge */
#define TOFF_GPIO_IO0_INTENF 0x0094	/* (RW) GPIO Interrupt Enable for Falling edge */
#define TOFF_GPIO_IO0_INTSTATR 0x0084	/* (RO) GPIO Interrupt Status for Rising edge */
#define TOFF_GPIO_IO0_INTSTATF 0x0088	/* (RO) GPIO Interrupt Status for Falling edge */
#define TOFF_GPIO_IO0_INTCLR 0x008C	/* (RO) GPIO Interrupt Clear */

#define TOFF_GPIO_IO2_INTENR 0x00B0	/* (RW) GPIO Interrupt Enable for Rising edge */
#define TOFF_GPIO_IO2_INTENF 0x00B4	/* (RW) GPIO Interrupt Enable for Falling edge */
#define TOFF_GPIO_IO2_INTSTATR 0x00A4	/* (RO) GPIO Interrupt Status for Rising edge */
#define TOFF_GPIO_IO2_INTSTATF 0x00A8	/* (RO) GPIO Interrupt Status for Falling edge */
#define TOFF_GPIO_IO2_INTCLR 0x00AC	/* (RO) GPIO Interrupt Clear */

#define TOFF_GPIO_INTSTAT   0x0080	/* (RO) GPIO overall Interrupt Status */

#define TADR_PARTCFG_BASE   0x3FFF8000

/* Fast I/O setup */
#define TADR_FIO_BASE       0x3FFFC000
#define TOFF_FIO0_DIR       0x0000		/* (RW) Fast GPIO Port Direction control register */
#define TOFF_FIO0_MASK      0x0010		/* (RW) Fast Mask register using FIOMASK */
#define TOFF_FIO0_PIN       0x0014		/* (RW) Fast Port Pin value register using FIOMASK */
#define TOFF_FIO0_SET       0x0018		/* (RW) Fast Port Output Set register using FIOMASK */
#define TOFF_FIO0_CLR       0x001C		/* (WO) Fast Port Output Clear register using FIOMASK0 */

#define TOFF_FIO1_DIR       0x0020		/* (RW) Fast GPIO Port Direction control register */
#define TOFF_FIO1_MASK      0x0030		/* (RW) Fast Mask register using FIOMASK */
#define TOFF_FIO1_PIN       0x0034		/* (RW) Fast Port Pin value register using FIOMASK */
#define TOFF_FIO1_SET       0x0038		/* (RW) Fast Port Output Set register using FIOMASK */
#define TOFF_FIO1_CLR       0x003C		/* (WO) Fast Port Output Clear register using FIOMASK0 */

#define TOFF_FIO2_DIR       0x0040		/* (RW) Fast GPIO Port Direction control register */
#define TOFF_FIO2_MASK      0x0050		/* (RW) Fast Mask register using FIOMASK */
#define TOFF_FIO2_PIN       0x0054		/* (RW) Fast Port Pin value register using FIOMASK */
#define TOFF_FIO2_SET       0x0058		/* (RW) Fast Port Output Set register using FIOMASK */
#define TOFF_FIO2_CLR       0x005C		/* (WO) Fast Port Output Clear register using FIOMASK0 */

#define TOFF_FIO3_DIR       0x0060		/* (RW) Fast GPIO Port Direction control register */
#define TOFF_FIO3_MASK      0x0070		/* (RW) Fast Mask register using FIOMASK */
#define TOFF_FIO3_PIN       0x0074		/* (RW) Fast Port Pin value register using FIOMASK */
#define TOFF_FIO3_SET       0x0078		/* (RW) Fast Port Output Set register using FIOMASK */
#define TOFF_FIO3_CLR       0x007C		/* (WO) Fast Port Output Clear register using FIOMASK0 */

#define TOFF_FIO4_DIR       0x0080		/* (RW) Fast GPIO Port Direction control register */
#define TOFF_FIO4_MASK      0x0090		/* (RW) Fast Mask register using FIOMASK */
#define TOFF_FIO4_PIN       0x0094		/* (RW) Fast Port Pin value register using FIOMASK */
#define TOFF_FIO4_SET       0x0098		/* (RW) Fast Port Output Set register using FIOMASK */
#define TOFF_FIO4_CLR       0x009C		/* (WO) Fast Port Output Clear register using FIOMASK0 */


/*
 *  System Control Block(SCB) modules include Memory Accelerator Module,
 *  Phase Locked Loop, VPB divider, Power Control, External Interrupt, 
 *  Reset, and Code Security/Debugging
 */
#define TADR_SCB_BASE       0xE01FC000

/* Memory Accelerator Module (MAM) */
#define TOFF_SCB_MAMCR      0x0000		/* (RW) MAM Control Register */
#define TOFF_SCB_MAMTIM     0x0004		/* (RW) MAM Timing Register */

/* Phase Locked Loop (PLL) */
#define TOFF_SCB_PLLCON     0x0080		/* (RW) PLL Control Register */
#define TOFF_SCB_PLLCFG     0x0084		/* (RW) PLL Configuration Register */
#define TOFF_SCB_PLLSTAT    0x0088		/* (RO) PLL Status Register */
#define TOFF_SCB_PLLFEED    0x008C		/* (WO) PLL Feed Register */

/* Power Control */
#define TOFF_SCB_PCON       0x00C0		/* (RW) Power Control Register */
#define TOFF_SCB_INTWAKE    0x0144		/* (RW) Interrupt Wakeup Register */
#define TOFF_SCB_PCONP      0x00C4		/* (RW) Power Control for Peripherals Register */

/* Clock Divider */
#define TOFF_SCB_CCLKCFG    0x0104		/* (RW) CPU Clock Configuarion Register */
#define TOFF_SCB_USBCLKCFG  0x0108		/* (RW) USB Clock Configuarion Register */
#define TOFF_SCB_CLKSRCSEL  0x010C
#define TOFF_SCB_SCS        0x01A0
#define TOFF_SCB_IRCTRIM    0x01A4		/* (RW) IRC Trim Register */
#define TOFF_SCB_PCLKSEL0   0x01A8		/* (RW) Peripheral Clock Selection Register 0 */
#define TOFF_SCB_PCLKSEL1   0x01AC		/* (RW) Peripheral Clock Selection Register 1 */
	
/* External Interrupts */
#define TOFF_SCB_EXTINT     0x0140		/* (RW) External Interrupt Flag Register */
#define TOFF_SCB_EXTMODE    0x0148		/* (RW) External Interrupt Mode Register */
#define TOFF_SCB_EXTPOLAR   0x014C		/* (RW) External External Inerrupt Polanty Register */

/* System control Register */
#define TOFF_SCB_RSID       0x0180		/* (RW) Reset Source Indentification Register */

/* AHB configuration */
#define TOFF_SCB_AHBCFG1    0x0188		/* (RW) Configuraion the AHB1 arbter */
#define TOFF_SCB_AHBCFG2    0x018C		/* (RW) Configuraion the AHB2 arbter */

/*
 *  External Memory Controller (EMC)
 */
#define TADR_EMC_BASE       0xFFE08000
#define TOFF_EMC_CTRL       0x0000		/* (RW) Controls operation of the memory contoller */
#define TOFF_EMC_STAT       0x0004		/* (RO) Provides EMC status information */
#define TOFF_EMC_CONFIG     0x0008		/* (RW) Configures operation of the memory controller */
#define TOFF_EMC_STA_EXTWAIT   0x0080	/* (RW) Time long static memory read and write tranfers */
#define TOFF_EMC_STA_CFG0      0x0200	/* (RW) Selects the memory onfiguration for static chip select 0 */
#define TOFF_EMC_STA_WAITWEN0  0x0204	/* (RW) Selects delay from chip select write enable */
#define TOFF_EMC_STA_WAITOEN0  0x0208	/* (RW) Selects the delay from chip select 0 and address change */
#define TOFF_EMC_STA_WAITRD0   0x020C	/* (RW) Selects the delay from chip select 0 to a read sccess */
#define TOFF_EMC_STA_WAITPAGE0 0x0210	/* (RW) Selects the delay for asynchronous page mode sequential accesses for chip select 0 */
#define TOFF_EMC_STA_WAITWR0   0x0214	/* (RW) Selects the delay from chip select 0 to a write access */
#define TOFF_EMC_STA_WAITTURN0 0x0218	/* (RW) Selects the number of bus tumaround cycles for chip select 0 */
#define TOFF_EMC_STA_CFG1      0x0220	/* (RW) Selects the memory onfiguration for static chip select 1 */
#define TOFF_EMC_STA_WAITWEN1  0x0224	/* (RW) Selects delay from chip select write enable */
#define TOFF_EMC_STA_WAITOEN1  0x0228	/* (RW) Selects the delay from chip select 1 and address change */
#define TOFF_EMC_STA_WAITRD1   0x022C	/* (RW) Selects the delay from chip select 1 to a read sccess */
#define TOFF_EMC_STA_WAITPAGE1 0x0230	/* (RW) Selects the delay for asynchronous page mode sequential accesses for chip select 1 */
#define TOFF_EMC_STA_WAITWR1   0x0234	/* (RW) Selects the delay from chip select 1 to a write access */
#define TOFF_EMC_STA_WAITTURN1 0x0238	/* (RW) Selects the number of bus tumaround cycles for chip select 1 */

/*
 *  TIMER 0/1/2/3
 */
#define TADR_TMR0_BASE      0xE0004000
#define TADR_TMR1_BASE      0xE0008000
#define TADR_TMR2_BASE      0xE0070000
#define TADR_TMR3_BASE      0xE0074000
#define TOFF_TMR_IR         0x0000		/* (RW) Interrupt Register */
#define TOFF_TMR_TCR        0x0004		/* (RW) Timer Control Register */
#define TOFF_TMR_TC         0x0008		/* (RW) Timer Counter */
#define TOFF_TMR_PR         0x000C		/* (RW) Prescale Register */
#define TOFF_TMR_PC         0x0010		/* (RW) Prescale Counter */
#define TOFF_TMR_MCR        0x0014		/* (RW) Match Control Register */
#define TOFF_TMR_MR0        0x0018		/* (RW) Match Register 0 */
#define TOFF_TMR_MR1        0x001C		/* (RW) Match Register 1 */
#define TOFF_TMR_MR2        0x0020		/* (RW) Match Register 2 */
#define TOFF_TMR_MR3        0x0024		/* (RW) Match Register 3 */
#define TOFF_TMR_CCR        0x0028		/* (RW) Capture Control Register */
#define TOFF_TMR_CR0        0x002C		/* (RW) Capture Register 0 */
#define TOFF_TMR_CR1        0x0030		/* (RW) Capture Register 1 */
#define TOFF_TMR_CR2        0x0034		/* (RW) Capture Register 2 */
#define TOFF_TMR_CR3        0x0038		/* (RW) Capture Register 3 */
#define TOFF_TMR_EMR        0x003C		/* (RW) External Match Register */
#define TOFF_TMR_CTCR       0x0070		/* (RW) Count Control Register */

/*
 *  Pulse Width Modulator (PWM) 1
 */
#define TADR_PWM1_BASE      0xE0018000
#define TOFF_PWM1_IR        0x0000		/* (RW) Interrupt Register */
#define TOFF_PWM1_TCR       0x0004		/* (RW) Timer Control Register */
#define TOFF_PWM1_TC        0x0008		/* (RW) Timer Counter */
#define TOFF_PWM1_PR        0x000C		/* (RW) Prescale Register */
#define TOFF_PWM1_PC        0x0010		/* (RW) Prescale Counter */
#define TOFF_PWM1_MCR       0x0014		/* (RW) Match Control Register */
#define TOFF_PWM1_MR0       0x0018		/* (RW) Match Register 0 */
#define TOFF_PWM1_MR1       0x001C		/* (RW) Match Register 1 */
#define TOFF_PWM1_MR2       0x0020		/* (RW) Match Register 2 */
#define TOFF_PWM1_MR3       0x0024		/* (RW) Match Register 3 */
#define TOFF_PWM1_CCR       0x0028		/* (RW) Capture Control Register */
#define TOFF_PWM1_CR0       0x002C		/* (RO) Capture Register 0 */
#define TOFF_PWM1_CR1       0x0030		/* (RO) Capture Register 1 */
#define TOFF_PWM1_CR2       0x0034		/* (RO) Capture Register 2 */
#define TOFF_PWM1_CR3       0x0038		/* (RO) Capture Register 3 */
#define TOFF_PWM1_MR4       0x0040		/* (RW) Match Register 4 */
#define TOFF_PWM1_MR5       0x0044		/* (RW) Match Register 5 */
#define TOFF_PWM1_MR6       0x0048		/* (RW) Match Register 6 */
#define TOFF_PWM1_PCR       0x004C		/* (RW) PWM Control Register */
#define TOFF_PWM1_LER       0x0050		/* (RW) Load Enable Register */
#define TOFF_PWM1_CTCR      0x0070		/* (RW) Count Control Register */

/*
 *  Universal Asynchronous Receiver Transmitter (UART) 0/1/2/3
 */
#define TADR_UART0_BASE     0xE000C000
#define TADR_UART1_BASE     0xE0010000
#define TADR_UART2_BASE     0xE0078000
#define TADR_UART3_BASE     0xE007C000
#define TOFF_UART_RBR       0x0000		/* (RO) Recevier Buffer Register */
#define TOFF_UART_THR       0x0000		/* (WO) Transmit Holding Register */
#define TOFF_UART_DLL       0x0000		/* (RW) Divisor Latch LSB */
#define TOFF_UART_DLM       0x0004		/* (RW) Divisor Latch MSB */
#define TOFF_UART_IER       0x0004		/* (RW) Interrupt Enable Register */
#define TOFF_UART_IIR       0x0008		/* (RO) Interrupt ID Register */
#define TOFF_UART_FCR       0x0008		/* (WO) FIFO Control Register */
#define TOFF_UART_LCR       0x000C		/* (RW) Line Control Register */
#define TOFF_UART_LSR       0x0014		/* (RO) Line Status Register */
#define TOFF_UART_SCR       0x001C		/* (RW) Scratch Pad Register */
#define TOFF_UART_ACR       0x0020		/* (RW) Auto-baud Control Register */
#define TOFF_UART_ICR       0x0024		/* (RW) IrDA Control Register notuse UART1 */
#define TOFF_UART_FDR       0x0028		/* (RW) Fractional Divider Register */
#define TOFF_UART_TER       0x0030		/* (RW) Transmit Enbale Register */

/*
 *  I2C Interface 0/1/2
 */
#define TADR_I2C0_BASE      0xE001C000
#define TADR_I2C1_BASE      0xE005C000
#define TADR_I2C2_BASE      0xE0080000
#define TOFF_I2CONSET       0x0000		/* (RW) I2C Control Set Register */
#define TOFF_I2STAT         0x0004		/* (RO) I2C Status Register */
#define TOFF_I2DAT          0x0008		/* (RW) I2C Data Register */
#define TOFF_I2ADR          0x000C		/* (RW) I2C Address Register */
#define TOFF_I2SCLH         0x0010		/* (RW) SCH Duty Cycle Register High Half Word */
#define TOFF_I2SCLL         0x0014		/* (RW) SCH Duty Cycle Register Low Half Word */
#define TOFF_I2CONCLR       0x0018		/* (WO) I2C Control Clear Register */

/*
 *  SPI0 (Serial Peripheral Interface 0)
 */
#define TADR_SPI0_BASE      0xE0020000
#define TOFF_S0SPCR         0x0000		/* (RW) SPI Control Register */
#define TOFF_S0SPSR         0x0004		/* (RO) SPI Status Register */
#define TOFF_S0SPDR         0x0008		/* (RW) SPI Data Register */
#define TOFF_S0SPCCR        0x000C		/* (RW) SPI Clock Counter Register */
#define TOFF_S0SPINT        0x001C		/* (RW) SPI Interrupt Flag */

/*
 *  SSP Controller 0/1
 */
#define TADR_SSP0_BASE      0xE0068000
#define TADR_SSP1_BASE      0xE0030000
#define TOFF_SSP_CR0        0x0000		/* (RW) Control Register 0 */
#define TOFF_SSP_CR1        0x0004		/* (RW) Control Register 1 */
#define TOFF_SSP_DR         0x0008		/* (RW) Data Register */
#define TOFF_SSP_SR         0x000C		/* (RO) Status Register */
#define TOFF_SSP_CPSR       0x0010		/* (RW) Clock Prescale Register */
#define TOFF_SSP_IMSC       0x0014		/* (RW) Interrupt Mask Set and Clear Register */
#define TOFF_SSP_RIS        0x0018		/* (RW) Raw Interrupt Status Register */
#define TOFF_SSP_MIS        0x001C		/* (RW) Masked Interrupt Status Register */
#define TOFF_SSP_ICR        0x0020		/* (RW) SSPICR Interrupt Clear Register */
#define TOFF_SSP_DMACR      0x0024		/* (RW) DMA Control Register */

/*
 *  Real Time Clock
 */
#define TADR_RTC_BASE       0xE0024000
#define TOFF_RTC_ILR        0x0000		/* (RW) Interrupt Lcation Register */
#define TOFF_RTC_CTC        0x0004		/* (RO) Clock Tick Counter */
#define TOFF_RTC_CCR        0x0008		/* (RW) Clock Control Register */
#define TOFF_RTC_CIIR       0x000C		/* (RW) Counter Increment Interrupt Register */
#define TOFF_RTC_AMR        0x0010		/* (RW) Alarm Mask Register */
#define TOFF_RTC_CTIME0     0x0014		/* (RO) Consolidated Timer Register 0 */
#define TOFF_RTC_CTIME1     0x0018		/* (RO) Consolidated Timer Register 1 */
#define TOFF_RTC_CTIME2     0x001C		/* (RO) Consolidated Timer Register 2 */
#define TOFF_RTC_SEC        0x0020		/* (RW) Seconds Counter */
#define TOFF_RTC_MIN        0x0024		/* (RW) Minutes Register */
#define TOFF_RTC_HOUR       0x0028		/* (RW) Hours Register */
#define TOFF_RTC_DOM        0x002C		/* (RW) Day of Month Register */
#define TOFF_RTC_DOW        0x0030		/* (RW) Day of Week Register */
#define TOFF_RTC_DOY        0x0034		/* (RW) Day of Year Register */
#define TOFF_RTC_MONTH      0x0038		/* (RW) Month Register */
#define TOFF_RTC_YEAR       0x003C		/* (RW) Years Register */
#define TOFF_RTC_CISS       0x0040		/* (RW) Counter Incriment select mask for Sub-Second interrupt */
#define TOFF_RTC_ALSEC      0x0060		/* (RW) Alarm value for seconds */
#define TOFF_RTC_ALMIN      0x0064		/* (RW) Alarm value for Minutes */
#define TOFF_RTC_ALHOUR     0x0068		/* (RW) Alarm value for Hours */
#define TOFF_RTC_ALDOM      0x006C		/* (RW) Alarm value for Day of Month */
#define TOFF_RTC_ALDOW      0x0070		/* (RW) Alarm value for Day of Week */
#define TOFF_RTC_ALDOY      0x0074		/* (RW) Alarm value for Day of Year */
#define TOFF_RTC_ALMON      0x0078		/* (RW) Alarm value for Months */
#define TOFF_RTC_ALYEAR     0x007C		/* (RW) Alarm value for Year */
#define TOFF_RTC_PREINT     0x0080		/* (RW) Prescaler value, integer portion */
#define TOFF_RTC_PREFRAC    0x0084		/* (RW) Prescaler value, fractional portion */

/*
 *  A/D Converter 0 (AD0)
 */
#define TADR_AD0_BASE       0xE0034000
#define TOFF_AD0CR          0x0000		/* (RW) A/D Control Register */
#define TOFF_AD0GDR         0x0004		/* (RW) A/D Global Data Register */
#define TOFF_AD0INTEN       0x000C		/* (RW) A/D Interrupt Enable Register */
#define TOFF_AD0DR0         0x0010		/* (RW) A/D Channel 0 Data Register */
#define TOFF_AD0DR1         0x0014		/* (RW) A/D Channel 1 Data Register */
#define TOFF_AD0DR2         0x0018		/* (RW) A/D Channel 2 Data Register */
#define TOFF_AD0DR3         0x001C		/* (RW) A/D Channel 3 Data Register */
#define TOFF_AD0DR4         0x0020		/* (RW) A/D Channel 4 Data Register */
#define TOFF_AD0DR5         0x0024		/* (RW) A/D Channel 5 Data Register */
#define TOFF_AD0DR6         0x0028		/* (RW) A/D Channel 6 Data Register */
#define TOFF_AD0DR7         0x002C		/* (RW) A/D Channel 7 Data Register */
#define TOFF_AD0STAT        0x0030		/* (RO) A/D Status Register */

/*
 *  D/A Converter
 */
#define TADR_DAC_BASE       0xE006C000
#define TOFF_DACR           0x0000		/* (RW) D/A Control Register */

/*
 *  Watchdog
 */
#define TADR_WDG_BASE       0xE0000000
#define TOFF_WDMOD          0x0000		/* (RW) Watchdog mode register */
#define TOFF_WDTC           0x0004		/* (RW) Watchdog timer constant register */
#define TOFF_WDFEED         0x0008		/* (WO) Watchdog feed sequence register */
#define TOFF_WDTV           0x000C		/* (RO) Watchdog timer value register */
#define TOFF_WDCLKSEL       0x0010		/* (RW) Watchdog clock source register */

/*
 *  CAN CONTROLLERS AND ACCEPTANCE FILTER
 */
#define TADR_CAN_ACCEPT_BASE 0xE003C000
#define TOFF_CAN_AFMR       0x0000		/* (RW) Acceptance Fillter Register */
#define TOFF_CAN_SFF_SA     0x0004		/* (RW) Standard Frame Individual Start Address Register */
#define TOFF_CAN_SFF_GRP_SA 0x0008	/* (RW) Standard Frame Group Start Address Register */
#define TOFF_CAN_EFF_SA     0x000C		/* (RW) Extended Frame Start Address Register */
#define TOFF_CAN_EFF_GRP_SA 0x0010		/* (RW) Extended Frame Group Start Address Register */
#define TOFF_CAN_EOT        0x0014		/* (RW) End of AF Table Register */
#define TOFF_CAN_LUT_ERR_ADR 0x0018		/* (RO) LUT Error Address Register */
#define TOFF_CAN_LUT_ERR    0x001C		/* (RO) LUT Error Register */
#define TOFF_CAN_FCANIE     0x0020		/* (RW) FillCAN Interrupt enable Register */
#define TOFF_CAN_FCANIC0    0x0024		/* (RW) FillCAN Interrupt and Capture Register 0 */
#define TOFF_CAN_FCANIC1    0x0028		/* (RW) FillCAN Interrupt and Capture Register 1 */

#define TADR_CAN_CENTRAL_BASE 0xE0040000
#define TOFF_CAN_TX_SR      0x0000		/* (RO) CAN Central Transmit Status Register */
#define TOFF_CAN_RX_SR      0x0004		/* (RO) CAN Central Receive Status Register */
#define TOFF_CAN_MSR        0x0008		/* (RO) CAN Central Miscellaneous Register */

#define TADR_CAN1_BASE      0xE0044000
#define TADR_CAN2_BASE      0xE0048000
#define TOFF_CAN_MOD        0x0000		/* (RW) Controls the operating mode of the CAN Controller */
#define TOFF_CAN_CMR        0x0004		/* (WO) Command bits that affect the state of the CAN Controller */
#define TOFF_CAN_GSR        0x0008		/* (RO) Global Controller Status and Error Counters */
#define TOFF_CAN_ICR        0x000C		/* (RO) Interrupt status. Arbitration Last Capture, Error Code Capture */
#define TOFF_CAN_IER        0x0010		/* (RW) Interrupt Enaqble */
#define TOFF_CAN_BTR        0x0014		/* (RW) Bus Timing */
#define TOFF_CAN_EWL        0x0018		/* (RW) Error Waming Limit */
#define TOFF_CAN_SR         0x001C		/* (RO) Status Register */
#define TOFF_CAN_RFS        0x0020		/* (RW) Received frame status */
#define TOFF_CAN_RID        0x0024		/* (RW) Received Indentifier */
#define TOFF_CAN_RDA        0x0028		/* (RW) Received data bytes 1-4 */
#define TOFF_CAN_RDB        0x002C		/* (RW) Received data bytes 5-8 */
#define TOFF_CAN_TFI1       0x0030		/* (RW) Transmit frame info(Tx Buffer 1) */
#define TOFF_CAN_TID1       0x0034		/* (RW) Transmit Identifier(Tx Buffer 1) */
#define TOFF_CAN_TDA1       0x0038		/* (RW) Transmit data bytes 1-4(Tx Buffer 1) */
#define TOFF_CAN_TDB1       0x003C		/* (RW) Transmit data bytes 5-8(Tx Buffer 1) */
#define TOFF_CAN_TFI2       0x0040		/* (RW) Transmit frame info(Tx Buffer 2) */
#define TOFF_CAN_TID2       0x0044		/* (RW) Transmit Identifier(Tx Buffer 2) */
#define TOFF_CAN_TDA2       0x0048		/* (RW) Transmit data bytes 1-4(Tx Buffer 2) */
#define TOFF_CAN_TDB2       0x004C		/* (RW) Transmit data bytes 5-8(Tx Buffer 2) */
#define TOFF_CAN_TFI3       0x0050		/* (RW) Transmit frame info(Tx Buffer 3) */
#define TOFF_CAN_TID3       0x0054		/* (RW) Transmit Identifier(Tx Buffer 3) */
#define TOFF_CAN_TDA3       0x0058		/* (RW) Transmit data bytes 1-4(Tx Buffer 3) */
#define TOFF_CAN_TDB3       0x005C		/* (RW) Transmit data bytes 5-8(Tx Buffer 3) */

/*
 *  MultiMedia Card Interface(MCI) Controller
 */
#define TADR_MCI_BASE       0xE008C000
#define TOFF_MCI_POWER      0x0000	/* (RW) Power control register */
#define TOFF_MCI_CLOCK      0x0004	/* (RW) Clock control register */
#define TOFF_MCI_ARGUMENT   0x0008	/* (RW) Argument register */
#define TOFF_MCI_COMMAND    0x000C	/* (RW) Command register */
#define TOFF_MCI_RESP_CMD   0x0010	/* (RO) Response command register */
#define TOFF_MCI_RESP0      0x0014	/* (RO) Response register */
#define TOFF_MCI_RESP1      0x0018	/* (RO) Response register */
#define TOFF_MCI_RESP2      0x001C	/* (RO) Response register */
#define TOFF_MCI_RESP3      0x0020	/* (RO) Response register */
#define TOFF_MCI_DATA_TMR   0x0024	/* (RW) Data Timer */
#define TOFF_MCI_DATA_LEN   0x0028	/* (RW) Data Length register */
#define TOFF_MCI_DATA_CTRL  0x002C	/* (RW) Data control register */
#define TOFF_MCI_DATA_CNT   0x0030	/* (RO) Data counter */
#define TOFF_MCI_STATUS     0x0034	/* (RO) Status register */
#define TOFF_MCI_CLEAR      0x0038	/* (WO) Clear register */
#define TOFF_MCI_MASK0      0x003C	/* (RW) Interrupt 0 mask register */
#define TOFF_MCI_MASK1      0x0040	/* (RW) Interrupt 1 mask register */
#define TOFF_MCI_FIFO_CNT   0x0048	/* (RO) FIFO Counter */
#define TOFF_MCI_FIFO       0x0080	/* (RW) Data FIFO Register */

/*
 *  I2S Interface Controller (I2S)
 */
#define TADR_I2S_BASE       0xE0088000
#define TOFF_I2S_DAO        0x0000		/* (RW) Digital Audio Coutput Register */
#define TOFF_I2S_DAI        0x0004		/* (RW) Digital Audio Input Register */
#define TOFF_I2S_TX_FIFO    0x0008		/* (WO) Transmit FIFO. */
#define TOFF_I2S_RX_FIFO    0x000C		/* (RO) Receive FIFO. */
#define TOFF_I2S_STATE      0x0010		/* (RO) Status Feedback Register */
#define TOFF_I2S_DMA1       0x0014		/* (RW) DMA Configuration Register 1 */
#define TOFF_I2S_DMA2       0x0018		/* (RW) DMA Configuration Register 2 */
#define TOFF_I2S_IRQ        0x001C		/* (RW) Interrupt Request Control Register */
#define TOFF_I2S_TXRATE     0x0020		/* (RW) Transmit bit rate divider */
#define TOFF_I2S_RXRATE     0x0024		/* (RW) Receive bit rate divider */

/*
 *  General-purpose DMA Controller
 */
#define TADR_DMAC_BASE      0xFFE04000
#define TOFF_DMAC_INTSTAT   0x0000		/* (RO) Interrupt Status Register */
#define TOFF_DMAC_INTTCSTAT 0x0004		/* (RO) Interrupt Terminal Count Status Register */
#define TOFF_DMAC_INTTCCLR  0x0008		/* (WO) Interrupt Terminal Count Clear Register */
#define TOFF_DMAC_INTERRSTAT 0x000C		/* (RO) Interrupt Error Status Register */
#define TOFF_DMAC_INTERRCLR 0x0010		/* (WO) Interrupt Error Clear Register */
#define TOFF_DMAC_RAWINTTCSTAT 0x0014	/* (RO) Raw Interrupt Terminal Count Status Register */
#define TOFF_DMAC_RAWINTERRSTAT 0x0018	/* (RO) Raw Error Interrupt Status Register */
#define TOFF_DMAC_ENABLEDCHNS 0x001C	/* (RO) Enabled Channel Register */
#define TOFF_DMAC_SOFTBREQ  0x0020		/* (RW) Software Burst Request Register */
#define TOFF_DMAC_SOFTSREQ  0x0024		/* (RW) Software Signal Request Register */
#define TOFF_DMAC_SOFTLBREQ 0x0028		/* (RW) Software Last Burst Request Register */
#define TOFF_DMAC_SOFTLSREQ 0x002C		/* (RW) Software Last Single Request Register */
#define TOFF_DMAC_CONFIG    0x0030		/* (RW) Configuration Register */
#define TOFF_DMAC_SYNC      0x0034		/* (RW) Synchronization Register */

#define TOFF_DMAC_CH0       0x0100
#define TOFF_DMAC_CH1       0x0120

/* DMA channel n registers */
#define TOFF_DMAC_CH_SRC    0x0000		/* (RW) Channel n Source Address Register */
#define TOFF_DMAC_CH_DEST   0x0004		/* (RW) Channel n Destnation Address Register */
#define TOFF_DMAC_CH_LLI    0x0008		/* (RW) Channel n Linked List item Register */
#define TOFF_DMAC_CH_CTRL   0x000C		/* (RW) Channel n Control Register */
#define TOFF_DMAC_CH_CFG    0x0010		/* (RW) Channel n Configuration Register */

/*
 *  Ethernet MAC (32 bit data bus) -- all registers are RW unless indicated in parentheses
 */
#define TADR_MAC_BASE       0xFFE00000	/* AHB Peripheral # 0 */
#define TOFF_MAC_MAC1       0x0000		/* (RW) MAC confiurationg register 1 */
#define TOFF_MAC_MAC2       0x0004		/* (RW) MAC configuration register 2 */
#define TOFF_MAC_IPGT       0x0008		/* (RW) Back-to-Back InterPacketGap register */
#define TOFF_MAC_IPGR       0x000C		/* (RW) non Back-to-Back InterPacketGap register */
#define TOFF_MAC_CLRT       0x0010		/* (RW) CoLlision window/ReTry register */
#define TOFF_MAC_MAXF       0x0014		/* (RW) MAXimum Frame register */
#define TOFF_MAC_SUPP       0x0018		/* (RW) PHY Support register */
#define TOFF_MAC_TEST       0x001C		/* (RW) TEST register */
#define TOFF_MAC_MCFG       0x0020		/* (RW) MII Mgmt Configuration register */
#define TOFF_MAC_MCMD       0x0024		/* (RW) MII Mgmt Command register */
#define TOFF_MAC_MADR       0x0028		/* (RW) MII Mgmt Addess register */
#define TOFF_MAC_MWTD       0x002C		/* (WO) MII Mgmt Write Data register */
#define TOFF_MAC_MRDD       0x0030		/* (RO) MII Mgmt ReadData register */
#define TOFF_MAC_MIND       0x0034		/* (RO) MII Mgmt Indicators register */

#define TOFF_MAC_SA0        0x0040		/* (RW) Station Address 0 register */
#define TOFF_MAC_SA1        0x0044		/* (RW) Station Address 1 register */
#define TOFF_MAC_SA2        0x0048		/* (RW) Station Address 2 register */

/* Controll registers */
#define TOFF_MAC_COMMAND    0x0100		/* (RW) Command register */
#define TOFF_MAC_STATUS     0x0104		/* (RO) Status register */
#define TOFF_MAC_RXDESCRIPTOR 0x0108	/* (RW) Receive descriptor base address register */
#define TOFF_MAC_RXSTATUS   0x010C		/* (RW) Receive status base address register */
#define TOFF_MAC_RXDESCRIPTORNUM 0x0110	/* (RW) Receive number of descriptors register */
#define TOFF_MAC_RXPRODUCEINDEX 0x0114	/* (RO) Receive produce index register */
#define TOFF_MAC_RXCONSUMEINDEX 0x0118	/* (RW) Receive consume index register */
#define TOFF_MAC_TXDESCRIPTOR 0x011C	/* (RW) Transmit descriptor base address register */
#define TOFF_MAC_TXSTATUS   0x0120		/* (RW) Transmit status base address register */
#define TOFF_MAC_TXDESCRIPTORNUM 0x0124	/* (RW) Transmit number of descriptors register */
#define TOFF_MAC_TXPRODUCEINDEX 0x0128	/* (RW) Transmit produce index register */
#define TOFF_MAC_TXCONSUMEINDEX 0x012C	/* (RO) Transmit consume index register */

#define TOFF_MAC_TSV0       0x0158		/* (RO) Tranmit status vector 0 register */
#define TOFF_MAC_TSV1       0x015C		/* (RO) Transmit status vector 1 register */
#define TOFF_MAC_RSV        0x0160		/* (RO) Receive status vector register */

#define TOFF_MAC_FLOWCONTROLCNT 0x0170	/* (RW) Flow control counter register */
#define TOFF_MAC_FLOWCONTROLSTS 0x0174	/* (RO) Flow control status register */

/* Rx filter registers */
#define TOFF_MAC_RXFILTERCTRL 0x0200	/* (RW) Receive filter control register */
#define TOFF_MAC_RXFILTERWOLSTS 0x0204	/* (RO) Receive filter WoL status register */
#define TOFF_MAC_RXFILTERWOLCLR 0x0208	/* (WO) Receive filter WoL clear register */

#define TOFF_MAC_HASHFILTERL 0x0210		/* (??) Hash filter LSBs register */
#define TOFF_MAC_HASHFILTERH 0x0214		/* (??) Hash filter MSBs register */

/* Module control registers */
#define TOFF_MAC_INTSTATUS  0x0FE0		/* (RO) Interrupt status register */
#define TOFF_MAC_INTENABLE  0x0FE4		/* (RW) Interrupt enable register */
#define TOFF_MAC_INTCLEAR   0x0FE8		/* (WO) Interrupt clear register */
#define TOFF_MAC_INTSET     0x0FEC		/* (WO) Interrupt set register */

#define TOFF_MAC_POWERDOWN  0x0FF4		/* (RW) Power-down register */
#define TOFF_MAC_MODULEID   0x0FFC

/*
 *  USB CONTROLLER
 */
#define TADR_USB_INT_BASE   0xE01FC1C0	/* USB DEVICE INTERRUPT */
#define TOFF_USB_INT_STAT   0x0000		/* (RW) USB Interrupt Status */

#define TADR_USB_DEV_BASE   0xFFE0C200	/* USB Device Base Address */
#define TOFF_USB_DEVINTSTAT 0x0000		/* (RO) USB Device Interrupt Status register */
#define TOFF_USB_DEVINTEN   0x0004		/* (RW) USB Device Interrupt Enable */
#define TOFF_USB_DEVINTCLR  0x0008		/* (WO) USB Device Interrupt Clear */
#define TOFF_USB_DEVINTSET  0x000C		/* (WO) USB Device Interrupt Set */
#define TOFF_USB_DEVINTPRIO 0x002C		/* (WO) USB Device Interrupt Priority */

/*
 *  USB Device Command Reagisters
 */
#define TOFF_USB_CMDCODE    0x0010		/* (WO) USB Command Code */
#define TOFF_USB_CMDDATA    0x0014		/* (RO) USB Command Data */

/*
 *  USB Device Data Transfer Registers
 */
#define TOFF_USB_RXDATA     0x0018		/* (RO) USB Receive Data */
#define TOFF_USB TXDATA     0x001C		/* (WO) USB Transmit Data */
#define TOFF_USB_RXPLENGTH  0x0020		/* (RO) USB Receive Packet Length */
#define TOFF_USB_TXPLENGTH  0x0024		/* (WO) USB Transmit Packet Length */
#define TOFF_USB_CTRL       0x0028		/* (RW) USB Control */

/*
 *  USB Device Endpoint Interrupt Registers
 */
#define TOFF_USB_EPINTSTAT  0x0030		/* (RO) USB Endpoint Interrupt Status */
#define TOFF_USB_EPINTEN    0x0034		/* (RW) USB Endpoint Interrupt Enable */
#define TOFF_USB_EPINTCLR   0x0038		/* (WO) USB Endpoint Interrupt Clear */
#define TOFF_USB_EPINTSET   0x003C		/* (WO) USB Endpoint Interrupt Set */
#define TOFF_USB_EPINTPRIO  0x0040		/* (WO) USB Endpoint Priority */

/*
 *  USB Device Endpoint Realization Registers
 */
#define TOFF_USB_REALIZE_EP 0x0044		/* (RW) USB Realize Engpoint */
#define TOFF_USB_EPINDEX    0x0048		/* (WO) USB Endpoint Index */
#define TOFF_USB_MAXPSIZE   0x004C		/* (RW) USB MaxPacketSize */

/*
 *  USB Device DMA Registers
 */
#define TOFF_USB_DMAREQSTAT 0x0050		/* (RO) USB DMA Request Status */
#define TOFF_USB_DMAREQCLR  0x0054		/* (WO) USB DMA Request Clear */
#define TOFF_USB_DMAREQSET  0x0058		/* (WO) USB DMA Request Set */
#define TOFF_USB_UDCA_HEAD  0x0080		/* (RW) USB UDCA Head */
#define TOFF_USB_EP_DMASTAT 0x0084		/* (RO) USB Endpoint DMA Status */
#define TOFF_USB_EP_DMAEN   0x0088		/* (WO) USB Endpoint DMA Enable */
#define TOFF_USB_EP_DMADIS  0x008C		/* (WO) USB Endpoint DMA Disable */
#define TOFF_USB_DMAINTSTAT 0x0090		/* (RO) USB DMA Interrupt Status */
#define TOFF_USB_DMAINTEN   0x0094		/* (RW) USB DMA Interrupt Enable */
#define TOFF_USB_EOTINTSTAT 0x00A0		/* (RO) USB End of Transfer Interrupt Status */
#define TOFF_USB_EOTINTCLR  0x00A4		/* (WO) USB End of Transfer Interrupt Clear */
#define TOFF_USB_EOTINTSET  0x00A8		/* (WO) USB End of Transfer Interrupt Set */
#define TOFF_USB_NDDRINTST  0x00AC		/* (RO) USB New DD Request Interrupt Status */
#define TOFF_USB_NDDRINTCLR 0x00B0		/* (WO) USB New DD Request Interrupt Clear */
#define TOFF_USB_NDDRINTSET 0x00B4		/* (WO) USB New DD Request Interrupt Set */
#define TOFF_USB_SYSERRINTST  0x00B8	/* (RO) USB System Error Interrupt Status */
#define TOFF_USB_SYSERRINTCLR 0x00BC	/* (WO) USB System Error Interrupt Clear */
#define TOFF_USB_SYSERRINTSET 0x00C0	/* (WO) USB System Error Interrupt Set */

/*
 *  USB Host Controller
 */
#define TADR_USBHC_BASE     0xFFE0C000
#define TOFF_USBHC_REVISION 0x0000		/* (RO) BCD representation of the version of the HCI */
#define TOFF_USBHC_CONTROL  0x0004		/* (RW) Defines the operating modes of the HC. */
#define TOFF_USBHC_CMD_STAT 0x0008		/* (RW) Receive the commands from the Host Controller Driver */
#define TOFF_USBHC_INT_STAT 0x000C		/* (RW) Indicate the status on various evets that case hardware interrupt */
#define TOFF_USBHC_INT_EN   0x0010		/* (RW) Controls the bits in the bits in the HcinterruptStatus register */
#define TOFF_USBHC_INT_DIS  0x0014		/* (RW) Disbale correspoinding bits int the HCInterruptStatus register */
#define TOFF_USBHC_HCCA     0x0018		/* (RW) Contains the physical address of the host controller communcation area.*/
#define TOFF_USBHC_PERCURED 0x001C		/* (RO) Contains the physical address of the current isochronous or interrupt endpoint descriptor */
#define TOFF_USBHC_CTRLHDED 0x0020		/* (RW) Contains the physical address of the first endpoint descriptor of the control list */
#define TOFF_USBHC_CTRLCURED 0x0024		/* (RW) Conatins the physical address of the current endpoint descriptor of the control list */
#define TOFF_USBHC_BULKHDED 0x0028		/* (RW) Contains the physical address of the first endpoint descriptor of the bulk list */
#define TOFF_USBHC_BULKCURED 0x002C		/* (RW) Contains the physical address of the current endpoint descriptor of the bulk list */
#define TOFF_USBHC_DONEHEAD 0x0030		/* (RO) Contains the physical address of the last transfer descriptor added to the Done queue */
#define TOFF_USBHC_FMINTRVAL 0x0034		/* (RW) Defines the bit time interval a frame and the full speed 0x2EDF maxmum packet size */
#define TOFF_USBHC_FMREMAIN 0x0038		/* (RO) A 14-bit counter showing the bit time remaining in the current frame */
#define TOFF_USBHC_FMNUMBER 0x003C		/* (RO) Contains a 16bit counter and provides th etiming refernce among events happing in the HC and the HCD. */
#define TOFF_USBHC_PERSTART 0x0040		/* (RW) Contains a progaming 14-bit vale whitch determines 0x0 the earliest time HC shoud start procssing a periodic list */
#define TOFF_USBHC_LSTHRHLD 0x0044		/* (RW) Contains 11-bit value whitch is used by the HC to determine whether to commit to transfer a maximum of 8-byte LS packet brfore EOF */
#define TOFF_USBHC_RH_DESCA 0x0048		/* (RW) First of the two registers whitch describes the characteristics of root hub */
#define TOFF_USBHC_RH_DESCB 0x004C		/* (RW) Second of the two registers whitch describes the characteristics of root hub */
#define TOFF_USBHC_RH_STAT  0x0050		/* (RW) This register is divided into two parts. The Lower D-word 0x0 respresents the hub status field and the upper word represents the hub status change field */
#define TOFF_USBHC_RHPORTSTAT1 0x0054	/* (RW) Controls and reports the port events on a per-port basis 0x0 */
#define TOFF_USBHC_RHPORTSTAT2 0x0058	/* (RW) Controls and reports the port events on a per-port basis 0x0 */

/*
 *  USB OTG Controller
 */
#define TADR_USBOTG_BASE    0xFFE0C100
#define TOFF_USBOTG_INTSTAT 0x0000		/* (RO) OTG Interrupt Status */
#define TOFF_USBOTG_INTEN   0x0004		/* (RW) OTG Interrupt Enable */
#define TOFF_USBOTG_INTSET  0x0008		/* (WO) OTG Interrupt Set */
#define TOFF_USBOTG_INTCLR  0x000C		/* (WO) OTG Interrupt Clear */
#define TOFF_USBOTG_STCTRL  0x0010		/* (RW) OTG Status and Control */
#define TOFF_USBOTG_TIMER   0x0014		/* (RW) OTG Timer */

/*
 *  I2C Registers
 */
#define TADR_USBOTG_I2C_BASE 0xFFE0C300
#define TOFF_USBOTG_I2C_RX  0x0000		/* (RO) I2C Receive */
#define TOFF_USBOTG_I2C_TX  0x0000		/* (WO) I2C Transmit */
#define TOFF_USBOTG_I2C_STS 0x0004		/* (RO) I2C Status */
#define TOFF_USBOTG_I2C_CTL 0x0008		/* (RW) I2C Control */
#define TOFF_USBOTG_I2C_CLKHI 0x000C	/* (RW) I2C Clock High */
#define TOFF_USBOTG_I2C_CLKLO 0x0010	/* (WO) I2C Clock Low */

/*
 *  Clock control registers
 */
#define TADR_USBOTG_CLK_BASE 0xFFE0CFF0
#define TOFF_USBOTG_CLK_CTRL 0x0004		/* (RW) OTG clock controller */
#define TOFF_USBOTG_CLK_STAT 0x0008		/* (RO) OTG clock status */


/*
 *  VECTORED INTERRUPT CONTROLLER (VIC)
 */
#define TADR_VIC_BASE       0xFFFFF000	/* VIC base address */
#define TOFF_VIC_IREQSTATUS 0x0000	/* (RO) IRQ Status Register */
#define TOFF_VIC_FIQSTATUS  0x0004	/* (RO) FIQ Status Register */
#define TOFF_VIC_RAWINTR    0x0008	/* (RO) Raw Interrupt Status Register */
#define TOFF_VIC_INTSELECT  0x000C	/* (RW) Interrupt Select Register */
#define TOFF_VIC_INTENABLE  0x0010	/* (RW) Interrupt Enable Register */
#define TOFF_VIC_INTENCLR   0x0014	/* (WO) Interrupt Enable Clear Register */
#define TOFF_VIC_SOFTINT    0x0018	/* (RW) Software Interrupt Register */
#define TOFF_VIC_SOFTINTCLR 0x001C	/* (WO) Sortware Interrupt Clear Register */
#define TOFF_VIC_PROTECTION 0x0020	/* (RW) Protection enable register */
#define TOFF_VIC_SWPRIOMASK 0x0024	/* (RW) Sortware Priority Mask Register */

#define TOFF_VIC_VECTADDRB  0x0100	/* Vector Address offset */
#define TOFF_VIC_VECTADDR0  0x0100	/* (RW) Vector address 0 Register */
#define TOFF_VIC_VECTADDR1  0x0104	/* (RW) Vector address 1 Register */
#define TOFF_VIC_VECTADDR2  0x0108	/* (RW) Vector address 2 Register */
#define TOFF_VIC_VECTADDR3  0x010C	/* (RW) Vector address 3 Register */
#define TOFF_VIC_VECTADDR4  0x0110	/* (RW) Vector address 4 Register */
#define TOFF_VIC_VECTADDR5  0x0114	/* (RW) Vector address 5 Register */
#define TOFF_VIC_VECTADDR6  0x0118	/* (RW) Vector address 6 Register */
#define TOFF_VIC_VECTADDR7  0x011C	/* (RW) Vector address 7 Register */
#define TOFF_VIC_VECTADDR8  0x0120	/* (RW) Vector address 8 Register */
#define TOFF_VIC_VECTADDR9  0x0124	/* (RW) Vector address 9 Register */
#define TOFF_VIC_VECTADDR10 0x0128	/* (RW) Vector address 10 Register */
#define TOFF_VIC_VECTADDR11 0x012C	/* (RW) Vector address 11 Register */
#define TOFF_VIC_VECTADDR12 0x0130	/* (RW) Vector address 12 Register */
#define TOFF_VIC_VECTADDR13 0x0134	/* (RW) Vector address 13 Register */
#define TOFF_VIC_VECTADDR14 0x0138	/* (RW) Vector address 14 Register */
#define TOFF_VIC_VECTADDR15 0x013C	/* (RW) Vector address 15 Register */
#define TOFF_VIC_VECTADDR16 0x0140	/* (RW) Vector address 16 Register */
#define TOFF_VIC_VECTADDR17 0x0144	/* (RW) Vector address 17 Register */
#define TOFF_VIC_VECTADDR18 0x0148	/* (RW) Vector address 18 Register */
#define TOFF_VIC_VECTADDR19 0x014C	/* (RW) Vector address 19 Register */
#define TOFF_VIC_VECTADDR20 0x0150	/* (RW) Vector address 20 Register */
#define TOFF_VIC_VECTADDR21 0x0154	/* (RW) Vector address 21 Register */
#define TOFF_VIC_VECTADDR22 0x0158	/* (RW) Vector address 22 Register */
#define TOFF_VIC_VECTADDR23 0x015C	/* (RW) Vector address 23 Register */
#define TOFF_VIC_VECTADDR24 0x0160	/* (RW) Vector address 24 Register */
#define TOFF_VIC_VECTADDR25 0x0164	/* (RW) Vector address 25 Register */
#define TOFF_VIC_VECTADDR26 0x0168	/* (RW) Vector address 26 Register */
#define TOFF_VIC_VECTADDR27 0x016C	/* (RW) Vector address 27 Register */
#define TOFF_VIC_VECTADDR28 0x0170	/* (RW) Vector address 28 Register */
#define TOFF_VIC_VECTADDR29 0x0174	/* (RW) Vector address 29 Register */
#define TOFF_VIC_VECTADDR30 0x0178	/* (RW) Vector address 30 Register */
#define TOFF_VIC_VECTADDR31 0x017C	/* (RW) Vector address 31 Register */

#define TOFF_VIC_VECTPRIO   0x0200	/* Vector Priority offset */
#define TOFF_VIC_VECTPRIO0  0x0200	/* (RW) Vector Priority 0 Register */
#define TOFF_VIC_VECTPRIO1  0x0204	/* (RW) Vector Priority 1 Register */
#define TOFF_VIC_VECTPRIO2  0x0208	/* (RW) Vector Priority 2 Register */
#define TOFF_VIC_VECTPRIO3  0x020C	/* (RW) Vector Priority 3 Register */
#define TOFF_VIC_VECTPRIO4  0x0210	/* (RW) Vector Priority 4 Register */
#define TOFF_VIC_VECTPRIO5  0x0214	/* (RW) Vector Priority 5 Register */
#define TOFF_VIC_VECTPRIO6  0x0218	/* (RW) Vector Priority 6 Register */
#define TOFF_VIC_VECTPRIO7  0x021C	/* (RW) Vector Priority 7 Register */
#define TOFF_VIC_VECTPRIO8  0x0220	/* (RW) Vector Priority 8 Register */
#define TOFF_VIC_VECTPRIO9  0x0224	/* (RW) Vector Priority 9 Register */
#define TOFF_VIC_VECTPRIO10 0x0228	/* (RW) Vector Priority 10 Register */
#define TOFF_VIC_VECTPRIO11 0x022C	/* (RW) Vector Priority 11 Register */
#define TOFF_VIC_VECTPRIO12 0x0230	/* (RW) Vector Priority 12 Register */
#define TOFF_VIC_VECTPRIO13 0x0234	/* (RW) Vector Priority 13 Register */
#define TOFF_VIC_VECTPRIO14 0x0238	/* (RW) Vector Priority 14 Register */
#define TOFF_VIC_VECTPRIO15 0x023C	/* (RW) Vector Priority 15 Register */
#define TOFF_VIC_VECTPRIO16 0x0240	/* (RW) Vector Priority 16 Register */
#define TOFF_VIC_VECTPRIO17 0x0244	/* (RW) Vector Priority 17 Register */
#define TOFF_VIC_VECTPRIO18 0x0248	/* (RW) Vector Priority 18 Register */
#define TOFF_VIC_VECTPRIO19 0x024C	/* (RW) Vector Priority 19 Register */
#define TOFF_VIC_VECTPRIO20 0x0250	/* (RW) Vector Priority 20 Register */
#define TOFF_VIC_VECTPRIO21 0x0254	/* (RW) Vector Priority 21 Register */
#define TOFF_VIC_VECTPRIO22 0x0258	/* (RW) Vector Priority 22 Register */
#define TOFF_VIC_VECTPRIO23 0x025C	/* (RW) Vector Priority 23 Register */
#define TOFF_VIC_VECTPRIO24 0x0260	/* (RW) Vector Priority 24 Register */
#define TOFF_VIC_VECTPRIO25 0x0264	/* (RW) Vector Priority 25 Register */
#define TOFF_VIC_VECTPRIO26 0x0268	/* (RW) Vector Priority 26 Register */
#define TOFF_VIC_VECTPRIO27 0x026C	/* (RW) Vector Priority 27 Register */
#define TOFF_VIC_VECTPRIO28 0x0270	/* (RW) Vector Priority 28 Register */
#define TOFF_VIC_VECTPRIO29 0x0274	/* (RW) Vector Priority 29 Register */
#define TOFF_VIC_VECTPRIO30 0x0278	/* (RW) Vector Priority 30 Register */
#define TOFF_VIC_VECTPRIO31 0x027C	/* (RW) Vector Priority 31 Register */

#define TOFF_VIC_VECTADDR   0x0F00	/* (RW) Vector Address 0 Register */

#ifndef _MACRO_ONLY

/*
 * 内蔵UART用 簡易SIOドライバ
 */
/*
 * カーネル起動時用の初期化(sys_putcを使用するため)
 */
extern void init_uart(void);


/*
 *  シリアルI/Oポート初期化ブロック
 */
typedef struct sio_port_initialization_block 
{
    VP uart_base;
    UW intno;
}
SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block 
{
    const SIOPINIB  *siopinib;  /* シリアルI/Oポート初期化ブロック */
    VP_INT          exinf;      /* 拡張情報 */
    BOOL    openflag;       /* オープン済みフラグ */
    BOOL    sendflag;       /* 送信割込みイネーブルフラグ */
    BOOL    getready;       /* 文字を受信した状態 */
    BOOL    putready;       /* 文字を送信できる状態 */

}SIOPCB;


/*
 *  コールバックルーチンの識別番号
 */
#define SIO_ERDY_SND    1u      /* 送信可能コールバック */
#define SIO_ERDY_RCV    2u      /* 受信通知コールバック */

/*
 *  オンチップのUARTからのポーリング出力
 */
extern void lpc2300_uart_putc(char c);

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
#endif  /* _LPC23XX_H_ */

