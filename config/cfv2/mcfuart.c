/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2000-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
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
 *  @(#) $Id: mcfuart.c,v 1.3 2005/07/06 00:45:07 honda Exp $
 */

/*
 *   COLDFIRE内蔵UART用 簡易ドライバ
 */

#include <s_services.h>
#include "mcfuart.h"

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
/* 入出力ポートの設定はsys_config.c */
/* 割込みベクタ番号の設定はhw_serial.h */
/* 管理ブロックの設定はmcfuart.c */

#ifndef GDB_STUB

const SIOPINIB siopinib_table[TNUM_PORT] = {
	{IPSBAR + 0x00000200, BRR9600, 0x0, 0x27}, /* UART0 */
#if TNUM_PORT >= 2
	{IPSBAR + 0x00000240, BRR9600, 0x0, 0x27}, /* UART1 */
#endif /* TNUM_PORT >= 2 */
};

#else /* GDB_STUB */

const SIOPINIB siopinib_table[TNUM_PORT] = {
	{IPSBAR + 0x00000200, BRR9600, 0x0, 6}, /* SCIF2 */
};

#endif /* GDB_STUB */

#if defined(TTM)
/*
 *  シリアルI/Oポート管理ブロックの定義
 *  　2chサポートに拡張する場合は初期値用のデータも含める
 */
struct sio_port_control_block
{
	VP_INT exinf;				/* 拡張情報 */
	BOOL openflag;				/* オープン済みフラグ */
};
#endif
/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
static SIOPCB siopcb_table[TNUM_PORT];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((UINT)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

/*
 *  文字を受信できるか？
 */
Inline BOOL
mcfuart_getready (SIOPCB * siopcb)
{
  return (sil_reb_mem((VB *) (siopcb->siopinib->reg_base + MCF_UART_USR)) & MCF_UART_USR_RXRDY);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
mcfuart_putready (SIOPCB * siopcb)
{
  /* Wait until space is available in the FIFO */
  return (sil_reb_mem((VB*)(siopcb->siopinib->reg_base + MCF_UART_USR)) & MCF_UART_USR_TXRDY);
}

/*
 *  受信した文字の取出し
 */
Inline char
mcfuart_getchar (SIOPCB * siopcb)
{
	VB data;
        data = sil_reb_mem ((VB*)(siopcb->siopinib->reg_base + MCF_UART_URB));
	return data;
}

/*
 *  送信する文字の書込み
 */
Inline void
mcfuart_putchar (SIOPCB * siopcb, char c)
{
    /* Send the character */
    sil_wrb_mem ((VB*)(siopcb->siopinib->reg_base + MCF_UART_UTB), c);
}

/*
 *  SIOドライバの初期化ルーチン
 */
void
mcfuart_initialize ()
{
	SIOPCB *siopcb;
	UINT i;
        /*
	 *  シルアルI/Oポート管理ブロックの初期化
	 */
	for (siopcb = siopcb_table, i = 0; i < TNUM_PORT; siopcb++, i++) {
		siopcb->openflag = FALSE;
		siopcb->siopinib = (&siopinib_table[i]);
	}
}

/*
 *  オープンしているポートがあるか？
 */
BOOL
mcfuart_openflag (ID siopid)
{
	return (siopcb_table[siopid -1].openflag);
}

/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
mcfuart_opn_por (ID siopid, VP_INT exinf)
{
        volatile int i;
	SIOPCB *siopcb = get_siopcb(siopid);
        // 送信停止
  	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UCR), (VB) (MCF_UART_UCR_MISC_RR));
    	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UCR), (VB) (MCF_UART_UCR_MISC_RT));
      	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UCR), (VB) (MCF_UART_UCR_MISC_RRC));

        sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UMR1), 0x93);
        sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UMR2), 0x07);
        sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UCSR), 0xDD);

        sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UBG1), (VB) (siopcb->siopinib->brr >> 8));
    	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UBG2), (VB) (siopcb->siopinib->brr & 0x00ff));

         /*  割込み許可  */
        siopcb->imr = (VB)MCF_UART_UIMR_RXRDY;
    	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UIMR), (VB) (siopcb->imr));

        /*  送受信許可  */
  	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UCR), (VB) (MCF_UART_UCR_TC_EN | MCF_UART_UCR_RC_EN));

	siopcb->exinf = exinf;
        siopcb->openflag = TRUE;
        if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000200)) {  
          sil_wrw_mem ((VW*)MCF_INTC_IMRL(MCF_INTC0), sil_rew_mem((VW*)MCF_INTC_IMRL(MCF_INTC0)) & (~(1 << (TBIT_GP0))));
        } else if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000240)) {
          sil_wrw_mem ((VW*)MCF_INTC_IMRL(MCF_INTC0), sil_rew_mem((VW*)MCF_INTC_IMRL(MCF_INTC0)) & (~(1 << (TBIT_GP1))));
        }
        for (i = 0; i < 1000;i++);
	return (siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
mcfuart_cls_por (SIOPCB * siopcb)
{
	/*  送受信禁止  */
  	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UCSR),
				 (VB) (MCF_UART_UCR_TC_DIS | MCF_UART_UCR_RC_DIS));
	/*  割込み禁止  */
	sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UIMR), 0);
        if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000200)) {    
          sil_wrw_mem (MCF_INTC_IMRL(MCF_INTC0), sil_rew_mem(MCF_INTC_IMRL(MCF_INTC0)) | (1 << TBIT_GP0));
        }  else if (siopcb->siopinib->reg_base == (IPSBAR + 0x00000240)) {
          sil_wrw_mem (MCF_INTC_IMRL(MCF_INTC0), sil_rew_mem(MCF_INTC_IMRL(MCF_INTC0)) | (1 << TBIT_GP1));
        }
	siopcb->openflag = FALSE;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
mcfuart_snd_chr (SIOPCB * siopcb, char c)
{
	if (mcfuart_putready (siopcb)) {
		mcfuart_putchar (siopcb, c);
		return (TRUE);
	}
	return (FALSE);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
INT
mcfuart_rcv_chr (SIOPCB * siopcb)
{
	if (mcfuart_getready (siopcb)) {
		return ((INT) (UB) mcfuart_getchar (siopcb));
	}
	return (-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
mcfuart_ena_cbr (SIOPCB * siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:			/* 送信割り込み要求を許可 */
                siopcb->imr |= MCF_UART_UIMR_TXRDY;
		break;
	case SIO_ERDY_RCV:			/* 受信割り込み要求を許可 */
                siopcb->imr |= MCF_UART_UIMR_RXRDY;
		break;
	}
        sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UIMR), siopcb->imr);
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
mcfuart_dis_cbr (SIOPCB * siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:			/* 送信割り込み要求を禁止 */
                siopcb->imr &= ~MCF_UART_UIMR_TXRDY;
		break;
	case SIO_ERDY_RCV:			/* 受信割り込み要求を禁止 */
                siopcb->imr &= ~MCF_UART_UIMR_RXRDY;
		break;
	}
        sil_wrb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UIMR), siopcb->imr);
}

/*
 * ポーリングによる文字の送信
 */
void
mcfuart_putc_pol (ID portid, char c)
{
	while (!mcfuart_putready (&siopcb_table[portid - 1]));
	mcfuart_putchar (&siopcb_table[portid - 1], c);
}

/*
 *  シリアルI/Oポートに対する送信割込み処理
 */
Inline void
mcfuart_isr_siop (SIOPCB * siopcb)
{
	VB uisr = sil_reb_mem ((VB *) (siopcb->siopinib->reg_base + MCF_UART_UISR));
        uisr &= siopcb->imr;
	if ((uisr & MCF_UART_UISR_TXRDY) && mcfuart_putready (siopcb)) {
		/*
		 *  送信通知コールバックルーチンを呼び出す．
		 */
		mcfuart_ierdy_snd (siopcb->exinf);
	}
	else if ((uisr & MCF_UART_UISR_RXRDY) && mcfuart_getready (siopcb)) {
		/*
		 *  送信通知コールバックルーチンを呼び出す．
		 */
		mcfuart_ierdy_rcv (siopcb->exinf);
        }
}

/* 割り込み検出 */
void
mcfuart_isr (void)
{
  if (siopcb_table[0].openflag) {
    mcfuart_isr_siop (get_siopcb (1));
  }
}

#if TNUM_PORT >= 2
/* 割り込み検出 */
void
mcfuart_isr2 (void)
{

  if (siopcb_table[1].openflag) {
    mcfuart_isr_siop (get_siopcb (2));
  }
}
#endif
