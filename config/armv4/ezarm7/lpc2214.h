/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005- by Monami software, LP.
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
 *  @(#) $Id$
 */

#ifndef _LPC2214_H_
#define _LPC2214_H_

#ifndef _MACRO_ONLY
#include <itron.h>
#include <sil.h>
#endif /* _MACRO_ONLY */

#include <armv4.h>

/*
 *  Device Registers
 */

/*
 * PXA250 RTC
 */
#define	RTC_RCNR	((VP)0x40900000)
#define	RTC_RTAR	((VP)0x40900004)
#define	RTC_RTSR	((VP)0x40900008)
#define	RTC_RTTR	((VP)0x4090000c)

/*
 * LPC2214 TIMER0/TIMER1
 */
#define TIMER_T0IR	((VP)0xe0004000)
#define TIMER_T0TCR	((VP)0xe0004004)
#define TIMER_T0TC	((VP)0xe0004008)
#define TIMER_T0PR	((VP)0xe000400c)
#define TIMER_T0PC	((VP)0xe0004010)
#define TIMER_T0MCR	((VP)0xe0004014)
#define TIMER_T0MR0	((VP)0xe0004018)
#define TIMER_T0MR1	((VP)0xe000401c)
#define TIMER_T0MR2	((VP)0xe0004020)
#define TIMER_T0MR3	((VP)0xe0004024)
#define TIMER_T0CCR	((VP)0xe0004028)
#define TIMER_T0CR0	((VP)0xe000402c)
#define TIMER_T0CR1	((VP)0xe0004030)
#define TIMER_T0CR2	((VP)0xe0004034)
#define TIMER_T0CR3	((VP)0xe0004038)
#define TIMER_T0EMR	((VP)0xe000403c)

#define TIMER_T1IR	((VP)0xe0008000)
#define TIMER_T1TCR	((VP)0xe0008004)
#define TIMER_T1TC	((VP)0xe0008008)
#define TIMER_T1PR	((VP)0xe000800c)
#define TIMER_T1PC	((VP)0xe0008010)
#define TIMER_T1MCR	((VP)0xe0008014)
#define TIMER_T1MR0	((VP)0xe0008018)
#define TIMER_T1MR1	((VP)0xe000801c)
#define TIMER_T1MR2	((VP)0xe0008020)
#define TIMER_T1MR3	((VP)0xe0008024)
#define TIMER_T1CCR	((VP)0xe0008028)
#define TIMER_T1CR0	((VP)0xe000802c)
#define TIMER_T1CR1	((VP)0xe0008030)
#define TIMER_T1CR2	((VP)0xe0008034)
#define TIMER_T1CR3	((VP)0xe0008038)
#define TIMER_T1EMR	((VP)0xe000803c)

#define MCR_INT_MR0 (1u)
#define MCR_RST_MR0 (2u)
#define MCR_STP_MR0 (4u)

/*
 * intrrupt control
 */
#define INTC_ICIP	0x40d00000
#define INTC_ICMR	0x40d00004
#define INTC_ICFP	0x40d0000c /* should not be used in the kenrel. */
#define INTC_ICPR	0x40d00010
#define INTC_ICLR	((VP)0x40d00008)
#define INTC_ICCR	((VP)0x40d00014)
#define INT_MASK_UA     (1<<22)
#define INT_MASK_T0     (1<<26)


/*
 *  SCI Registers
 */
#define	UART_BASE	(0xE0010000)
#define	UART_RBR	(VP)(UART_BASE+ 0)
#define	UART_THR	(VP)(UART_BASE+ 0)
#define	UART_IER	(VP)(UART_BASE+ 4)
#define	UART_IIR	(VP)(UART_BASE+ 8)
#define	UART_FCR	(VP)(UART_BASE+ 8)
#define	UART_LCR	(VP)(UART_BASE+ 0xc)
#define	UART_MCR	(VP)(UART_BASE+ 0x10)
#define	UART_LSR	(VP)(UART_BASE+ 0x14)
#define	UART_MSR	(VP)(UART_BASE+ 0x18)
#define	UART_SPR	(VP)(UART_BASE+ 0x1c)
#define	UART_DLL	(VP)(UART_BASE+ 0)
#define	UART_DLH	(VP)(UART_BASE+ 4)
/* #define	UART_ISR	(VP)(UART_BASE+ 0x20) -- LP2214 doesn't have ISR register. */

#define	GPIO_GPLR_x	(VP)(0x40e00000)
#define	GPIO_GPLR_y	(VP)(0x40e00004)
#define	GPIO_GPLR_z	(VP)(0x40e00008)
#define	GPIO_GPDR_x	(VP)(0x40e0000c)
#define	GPIO_GPDR_y	(VP)(0x40e00010)
#define	GPIO_GPDR_z	(VP)(0x40e00014)
#define	GPIO_GPSR_x	(VP)(0x40e00018)
#define	GPIO_GPSR_y	(VP)(0x40e0001c)
#define	GPIO_GPSR_z	(VP)(0x40e00020)
#define	GPIO_GPCR_x	(VP)(0x40e00024)
#define	GPIO_GPCR_y	(VP)(0x40e00028)
#define	GPIO_GPCR_z	(VP)(0x40e0002c)
#define	GPIO_GRER_x	(VP)(0x40e00030)
#define	GPIO_GRER_y	(VP)(0x40e00034)
#define	GPIO_GRER_z	(VP)(0x40e00038)
#define	GPIO_GFER_x	(VP)(0x40e0003c)
#define	GPIO_GFER_y	(VP)(0x40e00040)
#define	GPIO_GFER_z	(VP)(0x40e00044)
#define	GPIO_GEDR_x	(VP)(0x40e00048)
#define	GPIO_GEDR_y	(VP)(0x40e0004c)
#define	GPIO_GEDR_z	(VP)(0x40e00050)
#define	GPIO_GAFR0_x	(VP)(0x40e00054)
#define	GPIO_GAFR1_x	(VP)(0x40e00058)
#define	GPIO_GAFR0_y	(VP)(0x40e0005c)
#define	GPIO_GAFR1_y	(VP)(0x40e00060)
#define	GPIO_GAFR0_z	(VP)(0x40e00064)
#define	GPIO_GAFR1_z	(VP)(0x40e00068)

#define CQ_PXA250_CONFIG	(VP)(0x0a000000)

/*
 *  SCI Registers
 */
#define CAL_BPS(bps,clk) ((clk)/(16*(bps)))

#define UART_FCR_TC        0x01
#define UART_FCR_RC        0x02

#define UART_INT_R        0x01
#define UART_INT_T        0x02 
#define UART_INT_TI       0x04
#define UART_INT_M        0x08

#ifndef _MACRO_ONLY
/*
 * 内蔵UART用 簡易SIOドライバ
 */


/*
 * カーネル起動時用の初期化(sys_putcを使用するため)
 */
extern void uart_init(void);

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
typedef struct sio_port_control_block   SIOPCB;

/*
 *  コールバックルーチンの識別番号
 */
#define SIO_ERDY_SND    1u      /* 送信可能コールバック */
#define SIO_ERDY_RCV    2u      /* 受信通知コールバック */


/*
 *  オンチップのUARTからのポーリング出力
 */
extern void uart_putc(char c);



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
extern void uart_isr(void);


/*
 *  シリアルI/Oポートからの送信可能コールバック
 */
extern void uart_ierdy_snd(VP_INT exinf);


/*
 *  シリアルI/Oポートからの受信通知コールバック
 */
extern void uart_ierdy_rcv(VP_INT exinf);


#endif /* _MACRO_ONLY */


#endif  /*  _LPC2214_H_ */
