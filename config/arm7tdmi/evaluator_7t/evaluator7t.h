/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: evaluator7t.h,v 1.4 2001/11/09 12:47:42 hiro Exp $
 */

#ifndef _EVALUATOR7T_H_
#define _EVALUATOR7T_H_

#include "arm7tdmi.h"

/*
 *  KS32C50100
 */
#define SYSCFG_BASE     0x03ff0000
#define SYSCFG          ((LIOREG *)(SYSCFG_BASE)) /* 1K byte */

/*
 *  Cash
 */
#define CASH_TAG_RAM ((LIOREG *)0x11000000)


#ifndef _MACRO_ONLY
Inline void
cach_on(void)
{
    *SYSCFG = (*SYSCFG & 0xffffffcd) | 0x12;
}


Inline void
cach_off(void)
{
    *SYSCFG &= 0xfffffffd;
}


Inline void
cach_purge(void)
{
    int i;

    for(i = 0; i < 256; i++)
        *(CASH_TAG_RAM + i) &= 0x3fffffff;
}
#endif /* _MACRO_ONLY */



/*
 * Interrupt controller
 */
#define INTMOD         ((LIOREG *)(SYSCFG_BASE + 0x4000)) 
#define INTPND         ((LIOREG *)(SYSCFG_BASE + 0x4004))
#define INTMSK         ((LIOREG *)(SYSCFG_BASE + 0x4008)) 
#define INTPRI0        ((LIOREG *)(SYSCFG_BASE + 0x400C))
#define INTPRI1        ((LIOREG *)(SYSCFG_BASE + 0x4010))
#define INTPRI2        ((LIOREG *)(SYSCFG_BASE + 0x4014))
#define INTPRI3        ((LIOREG *)(SYSCFG_BASE + 0x4018))
#define INTPRI4        ((LIOREG *)(SYSCFG_BASE + 0x401C))
#define INTPRI5        ((LIOREG *)(SYSCFG_BASE + 0x4020))

/* inter source << 2  */
#define INTOFFSET      ((LIOREG *)(SYSCFG_BASE + 0x4024)) 
#define INTOFFSET_FIQ  ((LIOREG *)(SYSCFG_BASE + 0x4030)) 
#define INTOFFSET_IRQ  ((LIOREG *)(SYSCFG_BASE + 0x4034)) 

#define INTPNDPRI      ((LIOREG *)(SYSCFG_BASE + 0x4028)) 


#define INT_GLOBAL             21
#define IRQ_NUMBER_I2C         20
#define IRQ_NUMBER_ETH_MAC_RX  19
#define IRQ_NUMBER_ETH_MAC_TX  18
#define IRQ_NUMBER_ETH_BDMA_RX 17
#define IRQ_NUMBER_ETH_BDMA_TX 16
#define IRQ_NUMBER_HDLCB_RX    15
#define IRQ_NUMBER_HDLCB_TX    14
#define IRQ_NUMBER_HDLCA_RX    13
#define IRQ_NUMBER_HDLCA_TX    12
#define IRQ_NUMBER_TMU1        11
#define IRQ_NUMBER_TMU0        10
#define IRQ_NUMBER_GDMA1        9
#define IRQ_NUMBER_GDMA0        8
#define IRQ_NUMBER_UART1_RX     7
#define IRQ_NUMBER_UART1_TX     6
#define IRQ_NUMBER_UART0_RX     5
#define IRQ_NUMBER_UART0_TX     4
#define IRQ_NUMBER_EXI3         3
#define IRQ_NUMBER_EXI2         2
#define IRQ_NUMBER_EXI1         1
#define IRQ_NUMBER_EXI0         0


#define IRQ_BIT_I2C         (1<<IRQ_NUMBER_I2C)
#define IRQ_BIT_ETH_MAC_RX  (1<<IRQ_NUMBER_ETH_MAC_RX)
#define IRQ_BIT_ETH_MAC_TX  (1<<IRQ_NUMBER_ETH_MAC_TX)
#define IRQ_BIT_ETH_BDMA_RX (1<<IRQ_NUMBER_ETH_BDMA_RX)
#define IRQ_BIT_ETH_BDMA_TX (1<<IRQ_NUMBER_ETH_BDMA_TX)
#define IRQ_BIT_HDLCB_RX    (1<<IRQ_NUMBER_HDLCB_RX)
#define IRQ_BIT_HDLCB_TX    (1<<IRQ_NUMBER_HDLCB_TX)
#define IRQ_BIT_HDLCA_RX    (1<<IRQ_NUMBER_HDLCA_RX)
#define IRQ_BIT_HDLCA_TX    (1<<IRQ_NUMBER_HDLCA_TX)
#define IRQ_BIT_TMU1        (1<<IRQ_NUMBER_TMU1)
#define IRQ_BIT_TMU0        (1<<IRQ_NUMBER_TMU0)
#define IRQ_BIT_GDMA1       (1<<IRQ_NUMBER_GDMA1)
#define IRQ_BIT_GDMA0       (1<<IRQ_NUMBER_GDMA0)
#define IRQ_BIT_UART1_RX    (1<<IRQ_NUMBER_UART1_RX)
#define IRQ_BIT_UART1_TX    (1<<IRQ_NUMBER_UART1_TX)
#define IRQ_BIT_UART0_RX    (1<<IRQ_NUMBER_UART0_RX)
#define IRQ_BIT_UART0_TX    (1<<IRQ_NUMBER_UART0_TX)
#define IRQ_BIT_EXI3        (1<<IRQ_NUMBER_EXI3)
#define IRQ_BIT_EXI2        (1<<IRQ_NUMBER_EXI2)
#define IRQ_BIT_EXI1        (1<<IRQ_NUMBER_EXI1)
#define IRQ_BIT_EXI0        (1<<IRQ_NUMBER_EXI0)



/*
 *  I/Oレジスタ
 */
#define IOPMOD          ((LIOREG *)(SYSCFG_BASE+0x5000))
#define IOPCON          ((LIOREG *)(SYSCFG_BASE+0x5004))
#define IOPDATA         ((LIOREG *)(SYSCFG_BASE+0x5008))




/*
 *  SCI0 Control Register
 */
#ifndef _MACRO_ONLY
typedef struct{
    LIOREG ULCON;  /* Line control register      */
    LIOREG UCON;   /* Control register           */
    LIOREG USTAT;  /* Status register            */
    LIOREG UTXBUF; /* Transmit holding register  */
    LIOREG URXBUF; /* Receive buffer register    */
    LIOREG UBRDIV; /* Baud rate divisor register */
    LIOREG BRDCNT; /* Baud rate count register   */
    LIOREG RBDCLK; /* Baud rate clock monitor    */        
} uart;
#endif  /* _MACRO_ONLY */

#define UART0  (*(volatile uart *)(SYSCFG_BASE + 0xD000))


/* bit[1:0] */
#define ULCON_WL_5BIT  0x00
#define ULCON_WL_6BIT  0x01
#define ULCON_WL_7BIT  0x02
#define ULCON_WL_8BIT  0x03
/* bit[2] */
#define ULCON_STOP_1   0x00
#define ULCON_STOP_2   0x04
/* bit[5:3] */
#define ULCON_PMD_NO   0x04
#define ULCON_PMD_ODD  0x20
#define ULCON_PMD_EVEN 0x28
#define ULCON_PMD_F1   0x30
#define ULCON_PMD_F0   0x38
/* bit[6] */
#define ULCON_SCS_IN   0x00
#define ULCON_SCS_EX   0x40
/* bit[7] */
#define ULCON_IR       0x80

/* UCON  */
/* bit[1:0] */
#define UCON_RxM_MASK  0x02
#define UCON_RxM_DIS   0x00
#define UCON_RxM_INT   0x01
#define UCON_RxM_GDMA0 0x02
#define UCON_RxM_GDMA1 0x03
/* bit[2] */
#define UCON_RxSI_EN   0x04
/* bit[4:3] */
#define UCON_TxM_MASK  0x18
#define UCON_TxM_DIS   0x00
#define UCON_TxM_INT   0x08
#define UCON_TxM_GDMA0 0x10
#define UCON_TxM_GDMA1 0x18
/* bit[5] */
#define UCON_DST_EN    0x20
/* bit[6] */
#define UCON_SBK_EN    0x40
/* bit[7] */
#define UCON_LPM_EN    0x80

/*  USTAT  */
/* bit[0] */
#define USTAT_OV     0x01
#define USTAT_PE     0x02
#define USTAT_FE     0x04
#define USTAT_BKD    0x08
#define USTAT_DTR    0x10
#define USTAT_RDR    0x20
#define USTAT_TBE    0x40
#define USTAT_TC     0x80

/*  Baud rate  */
#define UBRDIV_9600   (162<<4)
#define UBRDIV_19200  (80<<4)
#define UBRDIV_38400  (40<<4)
#define UBRDIV_57600  (26<<4)




/*
 *   TM controller
 */
#define TMOD         ((LIOREG *)(SYSCFG_BASE + 0x6000))
#define TDATA0       ((LIOREG *)(SYSCFG_BASE + 0x6004))
#define TDATA1       ((LIOREG *)(SYSCFG_BASE + 0x6008))
#define TCNT0        ((LIOREG *)(SYSCFG_BASE + 0x600C))
#define TCNT1        ((LIOREG *)(SYSCFG_BASE + 0x6010)) 

#define TM_TE0   0x01
#define TM_TMD0  0x02
#define TM_TCLR0 0x04
#define TM_TE1   0x08
#define TM_TMD1  0x10
#define TM_TCLR1 0x20




/*
 *  Evaluator-7T depend
 */
/*
 *  LED
 */

#define LED_MASK        (0xf0)

#define LED_D4          (0x80)
#define LED_D3          (0x40)
#define LED_D2          (0x20)
#define LED_D1          (0x10)
#define LED_OFF         (0x00)


#ifndef _MACRO_ONLY
Inline void
led_init(void)
{
    *IOPMOD  |= LED_MASK;     /* To Outpu */
    *IOPDATA &= ~LED_MASK;    /* Clear */    
}


Inline
void led_set(unsigned data)
{
    *IOPDATA        &= ~LED_MASK;    /* Clear */
    *IOPDATA        |= data;         /* Set   */
}
#endif /* _MACRO_ONLY */

/*
 *   7Segment LED
 */
#define SEG_MASK        (0x1fc00)

#define SEG_A           (1<<10)
#define SEG_B           (1<<11)
#define SEG_C           (1<<12)
#define SEG_D           (1<<13)
#define SEG_E           (1<<14)
#define SEG_F           (1<<16)
#define SEG_G           (1<<15)

#define DISP_0          (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F)
#define DISP_1          (SEG_B|SEG_C)
#define DISP_2          (SEG_A|SEG_B|SEG_D|SEG_E|SEG_G)
#define DISP_3          (SEG_A|SEG_B|SEG_C|SEG_D|SEG_G)
#define DISP_4          (SEG_B|SEG_C|SEG_F|SEG_G)
#define DISP_5          (SEG_A|SEG_C|SEG_D|SEG_F|SEG_G)
#define DISP_6          (SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DISP_7          (SEG_A|SEG_B|SEG_C)
#define DISP_8          (SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DISP_9          (SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G)

#define DISP_A          (SEG_A|SEG_B|SEG_C|SEG_E|SEG_F|SEG_G)
#define DISP_B          (SEG_C|SEG_D|SEG_E|SEG_F|SEG_G)
#define DISP_C          (SEG_A|SEG_D|SEG_E|SEG_F)
#define DISP_D          (SEG_B|SEG_C|SEG_D|SEG_E|SEG_G)
#define DISP_E          (SEG_A|SEG_D|SEG_E|SEG_F|SEG_G)
#define DISP_F          (SEG_A|SEG_E|SEG_F|SEG_G)

#ifndef _MACRO_ONLY
Inline void
seg_init(void)
{
    *IOPMOD  |= SEG_MASK;  
    *IOPDATA &= ~SEG_MASK;
}


Inline void
seg_setdisplay (unsigned data)
{
    *IOPDATA        &= ~SEG_MASK;  /* Clear */
    *IOPDATA        |= data;       /* Set   */
}
#endif /* _MACRO_ONLY */

/*
 * User interrupt switch
 */
#define INT_SW3_MASK        (1)

#define IO_ENABLE_INT0      (1<<4)
#define IO_ACTIVE_HIGH_INT0 (1<<3)
#define IO_RISING_EDGE_INT0 (1)

#endif  /*  _EVALUATOR7T_H_ */


