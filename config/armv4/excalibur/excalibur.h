/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: excalibur.h,v 1.7 2003/12/11 00:44:27 honda Exp $
 */

#ifndef _EXCALIBUR_H_
#define _EXCALIBUR_H_

#ifndef _MACRO_ONLY
#include <itron.h>
#include <sil.h>
#endif /* _MACRO_ONLY */

#include <armv4.h>

/*
 *  Device Registers
 */
/*
 * Interrupt controller
 */
#define INT_MASK_SET        (EXCALIBUR_DEV_BASE + 0xC00)
#define INT_MASK            (EXCALIBUR_DEV_BASE + 0xC00)
#define INT_MASK_CLEAR      (EXCALIBUR_DEV_BASE + 0xC04)
#define INT_SOURCE_STATUS   (EXCALIBUR_DEV_BASE + 0xC08)
#define INT_REQUEST_STATUS  (EXCALIBUR_DEV_BASE + 0xC0C)

#define INT_ID              (EXCALIBUR_DEV_BASE + 0xC10)
#define INT_PLD_PRIORITY    (EXCALIBUR_DEV_BASE + 0xC14)
#define INT_MODE            (EXCALIBUR_DEV_BASE + 0xC18)

#define INT_PRIORITY_PLD0       (EXCALIBUR_DEV_BASE + 0xC80)
#define INT_PRIORITY_PLD1       (EXCALIBUR_DEV_BASE + 0xC84)
#define INT_PRIORITY_PLD2       (EXCALIBUR_DEV_BASE + 0xC88)
#define INT_PRIORITY_PLD3       (EXCALIBUR_DEV_BASE + 0xC8C)
#define INT_PRIORITY_PLD4       (EXCALIBUR_DEV_BASE + 0xC90)
#define INT_PRIORITY_PLD5       (EXCALIBUR_DEV_BASE + 0xC94)
#define INT_PRIORITY_EXTPIN     (EXCALIBUR_DEV_BASE + 0xC98)
#define INT_PRIORITY_UART       (EXCALIBUR_DEV_BASE + 0xC9C)
#define INT_PRIORITY_TIMER0     (EXCALIBUR_DEV_BASE + 0xCA0)
#define INT_PRIORITY_TIMER1     (EXCALIBUR_DEV_BASE + 0xCA4)
#define INT_PRIORITY_PLL        (EXCALIBUR_DEV_BASE + 0xCA8)
#define INT_PRIORITY_EBI        (EXCALIBUR_DEV_BASE + 0xCAC)
#define INT_PRIORITY_STRIPE_PLD (EXCALIBUR_DEV_BASE + 0xCB0)
#define INT_PRIORITY_AHB1_2     (EXCALIBUR_DEV_BASE + 0xCB4)
#define INT_PRIORITY_TX         (EXCALIBUR_DEV_BASE + 0xCB8)
#define INT_PRIORITY_RX         (EXCALIBUR_DEV_BASE + 0xCBC)
#define INT_PRIORITY_FASTCOMMS  (EXCALIBUR_DEV_BASE + 0xCC0)

#define INT_MASK_PLD0      0x00001
#define INT_MASK_PLD1      0x00002
#define INT_MASK_PLD2      0x00004
#define INT_MASK_PLD3      0x00008
#define INT_MASK_PLD4      0x00010
#define INT_MASK_PLD5      0x00020
#define INT_MASK_IP        0x00040
#define INT_MASK_UA        0x00080
#define INT_MASK_T0        0x00100
#define INT_MASK_T1        0x00200
#define INT_MASK_PS        0x00400
#define INT_MASK_EE        0x00800
#define INT_MASK_PE        0x01000
#define INT_MASK_AE        0x02000
#define INT_MASK_CT        0x04000
#define INT_MASK_CR        0x08000
#define INT_MASK_FC        0x10000

#define INT_MODE_SII       0x03   /* Six individual interrupts */
#define INT_MODE_SPV       0x00   /* Six-bit priority value    */
#define INT_MODE_FP1I      0x01   /* Five-bit priority value plus individual interrupt */

#define INT_PRIORITY_FIQ   0x40   /* Generate FIQ interrupt */

/*
 * PLL関連のレジスタ
 */
#define CLK_PLL1_NCNT       (EXCALIBUR_DEV_BASE + 0x300)
#define CLK_PLL1_MCNT       (EXCALIBUR_DEV_BASE + 0x304)
#define CLK_PLL1_KCNT       (EXCALIBUR_DEV_BASE + 0x308)
#define CLK_PLL1_CTRL       (EXCALIBUR_DEV_BASE + 0x30C)
#define CLK_PLL1_CTRL_VAL   0x01035
#define CLK_PLL2_NCNT       (EXCALIBUR_DEV_BASE + 0x310)
#define CLK_PLL2_MCNT       (EXCALIBUR_DEV_BASE + 0x314)
#define CLK_PLL2_KCNT       (EXCALIBUR_DEV_BASE + 0x318)
#define CLK_PLL2_CTRL       (EXCALIBUR_DEV_BASE + 0x31C)
#define CLK_PLL2_CTRL_VAL   0x01035

#define CLK_DERIVE          (EXCALIBUR_DEV_BASE + 0x320)
#define CLK_STATUS          (EXCALIBUR_DEV_BASE + 0x324)

/*
 * Stripe I/O
 */
#define IOCR_UART           (EXCALIBUR_DEV_BASE + 0x048)
#define IOCR_UART_VAL       (EXCALIBUR_DEV_BASE + 0x07)

/*
 *   バス関連のレジスタ
 */
#define AHB12B_CR           (EXCALIBUR_DEV_BASE + 0x100)
#define AHB12B_CR_VAL       (EXCALIBUR_DEV_BASE + 0x03)


/*
 * PLDコンフィギュレーションレジスタ関連
 */
#define IDCODE              (EXCALIBUR_DEV_BASE + 0x8)
#define CONFIG_CONTROL      (EXCALIBUR_DEV_BASE + 0x140)
#define CONFIG_CONTROL_CO   0x02
#define CONFIG_CONTROL_BB   0x04
#define CONFIG_CONTROL_E    0x10

#define CONFIG_CLOCK        (EXCALIBUR_DEV_BASE + 0x144)
#define CONFIG_DATA         (EXCALIBUR_DEV_BASE + 0x148)
#define CONFIG_UNLOCK       (EXCALIBUR_DEV_BASE + 0x14C)
#define CONFIG_UNLOCK_MAGIC 0x554e4c4b

/*
 * ビジーループ回数
 */
#define PLD_LOOP 1000


/*
 *  TIMER0 Registers
 */
#define TIMER0_CR        (EXCALIBUR_DEV_BASE + 0x200)
#define TIMER0_SR        (EXCALIBUR_DEV_BASE + 0x200)
#define TIMER0_PRE       (EXCALIBUR_DEV_BASE + 0x210)
#define TIMER0_LIMIT     (EXCALIBUR_DEV_BASE + 0x220)
#define TIMER0_READ      (EXCALIBUR_DEV_BASE + 0x230)

#define TIMER0_CR_IE     0x04
#define TIMER0_CR_CI     0x08
#define TIMER0_CR_S      0x10

#define TIMER0_SR_IE     0x40
#define TIMER0_SR_IS     0x80
#define TIMER0_SR_R      0x10



/*
 *  SCI Registers
 */
#define CAL_BPS(bps,clk) ((clk)/(16*(bps)))

#define UART_RSR        (EXCALIBUR_DEV_BASE + 0x280)
#define UART_RDS        (EXCALIBUR_DEV_BASE + 0x284)
#define UART_RD         (EXCALIBUR_DEV_BASE + 0x288)

#define UART_TSR        (EXCALIBUR_DEV_BASE + 0x28C)
#define UART_TD         (EXCALIBUR_DEV_BASE + 0x290)

#define UART_FCR        (EXCALIBUR_DEV_BASE + 0x294)

#define UART_IES        (EXCALIBUR_DEV_BASE + 0x298)
#define UART_IEC        (EXCALIBUR_DEV_BASE + 0x29C)
#define UART_ISR        (EXCALIBUR_DEV_BASE + 0x2A0)
#define UART_ID         (EXCALIBUR_DEV_BASE + 0x2A4)
#define UART_MC         (EXCALIBUR_DEV_BASE + 0x2A8)
#define UART_MCR        (EXCALIBUR_DEV_BASE + 0x2AC)
#define UART_MSR        (EXCALIBUR_DEV_BASE + 0x2B0)
#define UART_DIV_LO     (EXCALIBUR_DEV_BASE + 0x2B4)
#define UART_DIV_HI     (EXCALIBUR_DEV_BASE + 0x2B8)


#define UART_RSR_RX_LEVEL  0x1F
#define UART_RSR_RE        0x80

#define UART_RDS_OE        0x01
#define UART_RDS_PE        0x02
#define UART_RDS_FE        0x04
#define UART_RDS_BI        0x08

#define UART_TSR_TX_LEVEL  0x1F
#define UART_TSR_TXI       0x80

#define UART_FCR_TC        0x01
#define UART_FCR_RC        0x02
#define UART_FCR_TX_THR    0x1C
#define UART_FCR_RX_THR    0xE0

#define UART_INT_R        0x01
#define UART_INT_T        0x02 
#define UART_INT_TI       0x04
#define UART_INT_M        0x08

#define UART_ID_RI        0x01
#define UART_ID_TI        0x02
#define UART_ID_TII       0x03
#define UART_ID_MI        0x04

#define UART_MC_CLS       0x03
#define UART_MC_ST        0x04
#define UART_MC_PE        0x08
#define UART_MC_EP        0x10
#define UART_MC_SP        0x20
#define UART_MC_OE        0x40

#define UART_MCR_RTS      0x01
#define UART_MCR_DTR      0x02
#define UART_MCR_RI       0x04
#define UART_MCR_DCD      0x08
#define UART_MCR_LB       0x10
#define UART_MCR_BR       0x20
#define UART_MCR_AR       0x40
#define UART_MCR_AC       0x80

#define UART_DIV          0xff

#define UART_TX_FIFO_MAX  0x05


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



/*
 *  キャッシュ関連
 */
#ifndef _MACRO_ONLY

#define CACHE_TAG_RAM 0x11000000 /* W */

Inline void
cache_on(void)
{

}


Inline void
cache_off(void)
{

}


Inline void
cache_purge(void)
{

}
#endif /* _MACRO_ONLY */




#endif  /*  _EXCALIBUR_H_ */
