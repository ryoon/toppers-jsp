/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
 *
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: m52235.h,v 1.8 2003/06/18 12:40:08 hiro Exp $
 */

/*
 *	M52235 CPUボードのハードウェア資源の定義
 */

#ifndef _M52235_H_
#define _M52235_H_

#include <sil.h>

#define	MCF_UART0    77
#define	MCF_UART1    78
#define	MCF_UART2    79
#define MCF_PIT0     119

#define TBIT_GP0     (13)
#define TBIT_GP1     (14)

#define TBIT_GP2     (15)

#define TBIT_TT0     (55 - 32)
#define TBIT_TT1     (56 - 32)

#define REF_CLK_MHZ         (25)
#define SYS_CLK_MHZ         (60)
#define REF_CLK_KHZ         (REF_CLK_MHZ * 1000)
#define SYS_CLK_KHZ         (SYS_CLK_MHZ * 1000)
#define SYSTEM_CLOCK        SYS_CLK_MHZ

#define IPSBAR       (0x40000000)

#define MCF_SCM_RAMBAR            (IPSBAR + 0x000008)
#define MCF_SCM_RAMBAR_BDE        (0x00000200)
#define MCF_SCM_RAMBAR_BA(x)      ((x)&0xFFFF0000)

#define MCF_CLK_SYNCR  (IPSBAR + 0x00120000)
#define MCF_CLK_SYNSR  (IPSBAR + 0x00120002)

#define MCF_CLK_SYNCR_PLLEN     (0x0001)
#define MCF_CLK_SYNCR_PLLMODE   (0x0002)
#define MCF_CLK_SYNCR_CLKSRC    (0x0004)
#define MCF_CLK_SYNCR_FWKUP     (0x0020)
#define MCF_CLK_SYNCR_DISCLK    (0x0040)
#define MCF_CLK_SYNCR_LOCEN     (0x0080)
#define MCF_CLK_SYNCR_RFD(x)    (((x)&0x0007)<<8)
#define MCF_CLK_SYNCR_LOCRE     (0x0800)
#define MCF_CLK_SYNCR_MFD(x)    (((x)&0x0007)<<12)
#define MCF_CLK_SYNCR_LOLRE     (0x8000)

#define MCF_CLK_SYNSR_LOCS      (0x04)
#define MCF_CLK_SYNSR_LOCK      (0x08)
#define MCF_CLK_SYNSR_LOCKS     (0x10)
#define MCF_CLK_SYNSR_CRYOSC    (0x20)
#define MCF_CLK_SYNSR_OCOSC     (0x40)
#define MCF_CLK_SYNSR_EXTOSC    (0x80)

#define MCF_CLK_LPCR_LPD(x)     (((x)&0x0F)<<0)

#define MCF_CLK_CCHR_PFD(x)     (((x)&0x07)<<0)

#define MCF_CLK_RTCDR_RTCDF(x)  (((x)&0xFFFFFFFF)<<0)

/* 
 * PLL min/max specifications
 */
#define MAX_FVCO    60000   /* KHz */
#define MAX_FSYS    60000   /* KHz */
#define MAX_FREF    48000   /* KHz */
#define MIN_FREF    1000    /* KHz */
#define MAX_MFD     18      /* Multiplier (not encoded) */
#define MIN_MFD     4       /* Multiplier (not encoded) */
#define MAX_RFD     128     /* Divider (not encoded) */
#define MIN_RFD     1       /* Divider (not encoded) */

/*
 * Low Power Divider specifications
 */
#define MIN_LPD     (1 << 0)    /* Divider (not encoded) */
#define MAX_LPD     (1 << 15)   /* Divider (not encoded) */

#define MCF_CLK_LPCR            (IPSBAR + 0x120007)
#define MCF_CLK_CCHR            (IPSBAR + 0x120008)
#define MCF_CLK_RTCDR           (IPSBAR + 0x12000C)

#define MCF_INTC0    (IPSBAR + 0x0C00)
#define MCF_INTC1    (IPSBAR + 0x0D00)

#define MCF_INTC_IMRL(ch)  (ch + 0x0c)
#define MCF_INTC_IMRH(ch)  (ch + 0x08)
#define MCF_INTC_ICR(ch, n)  (ch + 0x40 + n)

#define MCF_INTC_IMRH_ALL  (0x00000000)
#define MCF_INTC_IMRL_ALL  (0x00000001)

#define MCF_GPIO_PUAPAR     (IPSBAR + 0x100071)
#define MCF_GPIO_PUBPAR     (IPSBAR + 0x100072)
#define MCF_GPIO_PUCPAR     (IPSBAR + 0x100073)
#define MCF_GPIO_PUBPAR_TXD1_TXD1      (0x01)
#define MCF_GPIO_PUBPAR_RXD1_RXD1      (0x04)
#define MCF_GPIO_PUCPAR_TXD2_TXD2      (0x01)
#define MCF_GPIO_PUCPAR_RXD2_RXD2      (0x02)
#define MCF_GPIO_PUAPAR_TXD0_TXD0      (0x01)
#define MCF_GPIO_PUAPAR_RXD0_RXD0      (0x04)

#endif /* _M52235_H_ */
