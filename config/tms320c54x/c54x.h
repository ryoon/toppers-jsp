/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: c54x.h,v 1.3 2002/04/14 14:19:50 reiji Exp $
 */


#ifndef _C54X_H_
#define _C54X_H_

#ifndef _MACRO_ONLY
typedef unsigned char byte;
typedef unsigned int  word;
typedef volatile word  HIOREG;
#endif

/******************************************
 *  ＣＰＵ メモリ・マップド・レジスタ
 ******************************************/
#define IMR  (*(HIOREG *)0x0)
#define IFR  (*(HIOREG *)0x1)
#define ST0  (*(HIOREG *)0x6)
#define ST1  (*(HIOREG *)0x7)
#define AL   (*(HIOREG *)0x8)
#define AH   (*(HIOREG *)0x9)
#define AG   (*(HIOREG *)0xA)
#define BL   (*(HIOREG *)0xB)
#define BH   (*(HIOREG *)0xC)
#define BG   (*(HIOREG *)0xD)
#define T    (*(HIOREG *)0xE)
#define TRN  (*(HIOREG *)0xF)
#define AR0  (*(HIOREG *)0x10)
#define AR1  (*(HIOREG *)0x11)
#define AR2  (*(HIOREG *)0x12)
#define AR3  (*(HIOREG *)0x13)
#define AR4  (*(HIOREG *)0x14)
#define AR5  (*(HIOREG *)0x15)
#define AR6  (*(HIOREG *)0x16)
#define AR7  (*(HIOREG *)0x17)
#define SP   (*(HIOREG *)0x18)
#define BK   (*(HIOREG *)0x19)
#define BRC  (*(HIOREG *)0x1A)
#define RSA  (*(HIOREG *)0x1B)
#define REA  (*(HIOREG *)0x1C)
#define PMST (*(HIOREG *)0x1D)
#define XPC  (*(HIOREG *)0x1E)


/********************************************
 *   ペリフェラル メモリ・マップド・レジスタ
 *******************************************/
/*
 * タイマーレジスタ
 */
#ifndef _MACRO_ONLY
typedef struct{
  HIOREG TIM;
  HIOREG PRD;
  HIOREG TCR;
}tmu;
#endif

#define TMU0 (*(volatile tmu *)0x24)
#define TMU1 (*(volatile tmu *)0x30)

#define TCR_TSS 0x0010
#define TCR_TRB 0x0020



/***********************************************
 *  I/O  メモリ・マップド・レジスタ            *
***********************************************/
/*
 *  complex Programmable Logic Device (CPLD)   *
 *  I/O メモリ・マップド・レジスタ                 *
 */

/*  レジスタ　アドレス　*/
#ifndef _MACRO_ONLY
volatile ioport word port0;
volatile ioport word port1;
volatile ioport word port2;
volatile ioport word port3;
volatile ioport word port4;
volatile ioport word port5;
volatile ioport word port6;
#endif

/*  CPLD　レジスタ */
#define CPLD_CTRL1            port0
#define CPLD_STAT             port1
#define CPLD_DMCTRL           port2
#define CPLD_DBIO             port3
#define CPLD_CTRL2            port4
#define CPLD_SEM0             port5
#define CPLD_SEM1             port6

/*
 *  UART I/O メモリ・マップド・レジスタ *
 */
/*  レジスタ アドレス */
#ifndef _MACRO_ONLY
volatile ioport word   port4000;
volatile ioport word   port4001;
volatile ioport word   port4002;
volatile ioport word   port4003;
volatile ioport word   port4004;
volatile ioport word   port4005;
volatile ioport word   port4006;
volatile ioport word   port4007;
#endif

/*  UART レジスタ  */
#define UART_RBR  port4000
#define UART_THR  port4000
#define UART_IER  port4001
#define UART_IIR  port4002
#define UART_FCR  port4002
#define UART_LCR  port4003
#define UART_MCR  port4004
#define UART_LSR  port4005
#define UART_MSR  port4006
#define UART_SCR  port4007
/*  以下のレジスタにアクセスする前に、
 *   DLAB=1 (b7 of LCR) をセットする必要がある
 */
#define UART_DLL  port4000
#define UART_DLM  port4001

/*
 *  ビットフィールドの定義  */
#define FCR_FIFO  0x01
#define FCR_RFRST 0x02
#define FCR_TFRST 0x04
#define FIFO_TRIG      0x00

#define LCR_DLAB  0x80

/* 受信送信割り込みの定義 */
#define IER_ERBI      0x01  /* 受信割り込み */
#define IER_RIE   IER_ERBI 
#define IER_ETBEI     0x02  /* 送信割り込み */
#define IER_TIE   IER_ETBEI

#define LSR_DR   0x01
#define LSR_THRE 0x20

/*
 *   割り込みマスクレジスタ(IMR)ビットフィールド
 */
#define INT0           0
#define INT1           1
#define INT2           2
#define TINT0          3

#define RINT0          4
#define XINT0          5

#define TINT1          7

#define INT3           8
#define HPINT          9
#define RINT1         10
#define XINT1         11

#define DMAC0         6
#define DMAC1         7
#define DMAC2         10
#define DMAC3         11
#define DMAC4         12
#define DMAC5         13


/*
 *  割り込みハンドラ番号 INHNO
 */
#define RESET_VEC             0
#define NMI_VEC	              1
#define SINT17_VEC            2
#define SINT18_VEC            3
#define SINT19_VEC           4
#define SINT20_VEC        5
#define SINT21_VEC        6
#define SINT22_VEC        7
#define SINT23_VEC        8
#define SINT24_VEC        9
#define SINT25_VEC        10
#define SINT26_VEC        11
#define SINT27_VEC        12
#define SINT28_VEC        13
#define SINT29_VEC        14
#define SINT30_VEC        15
#define INT0_VEC        16
#define INT1_VEC        17
#define INT2_VEC        18
#define TINT0_VEC        19
#define RINT0_VEC        20
#define XINT0_VEC        21
#define DMAC0_VEC        22
#define TINT1_VEC        23
#define INT3_VEC        24
#define HPI_VEC            25
#define RINT1_VEC        26
#define XINT1_VEC        27
#define DMAC2_VEC        26
#define DMAC3_VEC        27
#define DMAC4_VEC        28
#define DMAC5_VEC        29

/*
 *  TRAP番号
 */
#define RS_TRAP           0
#define NMI_TRAP       1
#define INT0_TRAP       16
#define INT1_TRAP       17
#define INT2_TRAP       18
#define TINT_TRAP       19
#define RINT0_TRAP       20
#define XINT0_TRAP       21
#define DMAC0_TRAP       22
#define DMAC1_TRAP       23
#define INT3_TRAP       24
#define HPINT_TRAP       25
#define RINT1_TRAP       26
#define DMAC2_TRAP       26
#define XINT1_TRAP       27
#define DMAC3_TRAP       27
#define DMAC4_TRAP       28
#define DMAC5_TRAP       29

#define SINTR        0
#define SINT16        1
#define SINT17        2
#define SINT18        3
#define SINT19        4
#define SINT20        5
#define SINT21        6
#define SINT22        7
#define SINT23        8
#define SINT24        9
#define SINT25        10
#define SINT26        11
#define SINT27        12
#define SINT28        13
#define SINT29        14
#define SINT30        15
#define SINT0        16
#define SINT1        17
#define SINT2        18
#define SINT3        19
#define SINT4        20
#define SINT5        21
#define SINT6        22
#define SINT7        23
#define SINT8        24
#define SINT9        25
#define SINT10        26
#define SINT11        27
#define SINT12          28
#define SINT13          29



/*  sys_putcにuart_putcの代わりにprintfを
 *  用いる場合PRINTFを定義する。またその
 *  場合はライブラリrts.libをインクルードする
 */
//#define PRINTF

#ifndef PRINTF
/* ポーリングでシリアルに送信 */
#ifndef _MACRO_ONLY
Inline char
uart_putc(char c)
{
  while((UART_LSR & LSR_THRE) == 0);      /*  送信レジスタが空かどうか  */
  UART_THR = c;
  return(c);
}
#endif /* _MACRO_ONLY */
#endif /* PRINTF */
#endif
