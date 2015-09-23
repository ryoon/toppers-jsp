/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
 *  Copyright (C) 2008-     by Monami Software Limited Partnership, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  @(#) $Id$
 */

/*
 *	COMドライバ（PC/AT用）
 */

#include <s_services.h>
#include <pcat_com.h>

#define PCAT_COM_IIR_RLS (0x06)
#define PCAT_COM_IIR_RXD (0x04)
#define PCAT_COM_IIR_TXD (0x02)

#define PCAT_COM_IER_LS (4)
#define PCAT_COM_IER_TX (2)
#define PCAT_COM_IER_RX (1)

#define PCAT_COM_RBR 0
#define PCAT_COM_THR 0
#define PCAT_COM_DLL 0
#define PCAT_COM_IER 1
#define PCAT_COM_DLM 1
#define PCAT_COM_IIR 2
#define PCAT_COM_FCR 2
#define PCAT_COM_LCR 3
#define PCAT_COM_MCR 4
#define PCAT_COM_LSR 5
#define PCAT_COM_MSR 6
#define PCAT_COM_SCR 7

#define PCAT_COM_DIVISOR_LATCH(a) (24000000 / 13 / 16 / (a))

#ifndef PCAT_COM1_BAUD_RATE
#define PCAT_COM1_BAUD_RATE 9600
#endif
#if TNUM_SIOP >= 2
#ifndef PCAT_COM2_BAUD_RATE
#define PCAT_COM2_BAUD_RATE 9600
#endif
#endif

/*
 *  低レベルポート情報管理ブロックの初期値
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{
		PCAT_COM1_BASE,
		TO_INTNO(PCAT_COM1_INHNO),
		PCAT_COM_DIVISOR_LATCH(PCAT_COM1_BAUD_RATE)
	},
#if TNUM_SIOP >= 2
	{
		PCAT_COM2_BASE,
		TO_INTNO(PCAT_COM2_INHNO),
		PCAT_COM_DIVISOR_LATCH(PCAT_COM2_BAUD_RATE)
	},
#endif
};
SIOPCB siopcb_table[TNUM_SIOP];

Inline UB pcat_com_read(UH base, UH offset) {
	return sil_reb_iop((VP)(base + offset));
}

Inline void pcat_com_write(UH base, UH offset, UB data) {
	sil_wrb_iop((VP)(base + offset), data);
}

/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(SIOPCB *p)
{
  UB stat;
  stat = pcat_com_read(p->siopinib->base_address, PCAT_COM_LSR);

  return ((stat & 0x01) != 0);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(SIOPCB *p)
{
  UB stat;
  stat = pcat_com_read(p->siopinib->base_address, PCAT_COM_LSR);

  return ((stat & 0x20) != 0);
}

SIOPCB *
pcat_com_set_mode(ID portid) {
  SIOPCB *p;

  p = &(siopcb_table[portid - 1]);
	if (p->initflag) {
		return p;
	}
  pcat_com_write(p->siopinib->base_address, PCAT_COM_IER, 0); // Disable interrupt.
  pcat_com_write(p->siopinib->base_address, PCAT_COM_LCR, 0x83); // DLAB=1, 8bit, stop=1, Non parity.
  pcat_com_write(p->siopinib->base_address, PCAT_COM_DLL, (UB)p->siopinib->divisor_latch);
  pcat_com_write(p->siopinib->base_address, PCAT_COM_DLM, (UB)(p->siopinib->divisor_latch >> 8));
  pcat_com_write(p->siopinib->base_address, PCAT_COM_LCR, 0x03); // DLAB=0, 8bit, stop=1, Non parity.
  pcat_com_write(p->siopinib->base_address, PCAT_COM_FCR, 0x07);  // Enable FIFO.


  pcat_com_read(p->siopinib->base_address, PCAT_COM_LSR);
  pcat_com_read(p->siopinib->base_address, PCAT_COM_RBR);
  pcat_com_read(p->siopinib->base_address, PCAT_COM_IIR);
  pcat_com_read(p->siopinib->base_address, PCAT_COM_MCR);

  p->initflag = TRUE;			/* 初期化フラグ設定 */

  return p;
}

void pcat_com_isr(VP_INT exinf) {
	SIOPCB *p = (SIOPCB *)exinf;
	while (1) {
		switch (pcat_com_read(p->siopinib->base_address, PCAT_COM_IIR) & 7) {
		case PCAT_COM_IIR_TXD:
			pcat_com_ierdy_snd(p->spcb);
			break;
		case PCAT_COM_IIR_RXD:
			pcat_com_ierdy_rcv(p->spcb);
			break;
		case PCAT_COM_IIR_RLS:
			pcat_com_read(p->siopinib->base_address, PCAT_COM_LSR);
			break;
		default:
			return;
		}
	}
}

void
pcat_com_initialize()
{
	UINT i;
	for (i = 0; i < TNUM_SIOP; ++i) {
		siopcb_table[i].siopinib = &siopinib_table[i];
	}
}

/*
 *  シリアルI/Oポートの割込みハンドラ
 */
void
pcat_com1_handler()
{
	pcat_com_isr(&siopcb_table[0]);
}
#if TNUM_PORT >= 2
void
pcat_com2_handler()
{
	pcat_com_isr(&siopcb_table[1]);
}
#endif

SIOPCB *
pcat_com_opn_por(ID portid, VP_INT spcb)
{
  SIOPCB *p;

  p = pcat_com_set_mode(portid);
  p->spcb = spcb;

  pcat_com_write(p->siopinib->base_address, PCAT_COM_MCR, 0x0b); // Enable OUT2 interrupt.

  return p;
}

/*
 *  シリアルI/Oポートの終了
 */
void
pcat_com_cls_por(SIOPCB *p)
{
    
  p->initflag = FALSE;
  pcat_com_write(p->siopinib->base_address, PCAT_COM_IER, 0); // Disable interrupt.
}

/*
 *  受信した文字の取り出し
 */
INT
pcat_com_rcv_chr(SIOPCB *p)
{
  UB ch;
  
	if (!hw_port_getready(p)) {
		return -1;
	}
  ch = pcat_com_read(p->siopinib->base_address, PCAT_COM_RBR);
  return (INT)ch;
}

/*
 *  送信する文字の書き込み
 */
BOOL
pcat_com_snd_chr(SIOPCB *p, char c)
{
  if (!hw_port_putready(p)) {
    return FALSE;
  }
  pcat_com_write(p->siopinib->base_address, PCAT_COM_THR, c);
  return TRUE;
}

void
pcat_com_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	UB ier = pcat_com_read(siopcb->siopinib->base_address, PCAT_COM_IER);
	if (cbrtn == SIO_ERDY_SND) {
		ier |= PCAT_COM_IER_TX;
	} else if (cbrtn == SIO_ERDY_RCV) {
		ier |= PCAT_COM_IER_LS | PCAT_COM_IER_RX;
	}
	pcat_com_write(siopcb->siopinib->base_address, PCAT_COM_IER, ier);
}

void
pcat_com_dis_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	UB ier = pcat_com_read(siopcb->siopinib->base_address, PCAT_COM_IER);
	if (cbrtn == SIO_ERDY_SND) {
		ier &= ~PCAT_COM_IER_TX;
	} else if (cbrtn == SIO_ERDY_RCV) {
		ier &= ~(PCAT_COM_IER_LS | PCAT_COM_IER_RX);
	}
	pcat_com_write(siopcb->siopinib->base_address, PCAT_COM_IER, ier);
}

void pcat_com_init(ID siopid) {
	pcat_com_initialize();
	pcat_com_set_mode(siopid);
}

void pcat_com_putc(ID siopid, char c) {
	SIOPCB *p;
	p = &siopcb_table[siopid - 1];
	while (!pcat_com_snd_chr(p, c)) {
	}
}
