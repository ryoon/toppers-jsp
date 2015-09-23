/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2002 by MURANAKA Masaki
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: hw_serial.h,v 1.2 2002/04/10 12:08:17 honda Exp $
 */

/*
 *	ターゲット依存シリアルI/Oモジュール（PC/AT用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include "sys_config.h"
#include "cpu_insn.h"
#include "irc.h"

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL	 (0x24)

/*
 *  シリアルポートのハードウェア依存情報の定義
 *  Not yet! m68kのようにコントロールレジスタを構造体にする。
 */
typedef struct pcat_hardware_serial_port_descripter {
  INT	    *initflag;	   /* 初期化済フラグへのポインタ   */
  BOOL    sendflag;      /* 送信割り込みイネーブルフラグ */
  short base_address;
  short com_irq;
} HWPORT;


/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	1	/* サポートするシリアルポートの数 */

static INT	initflag[2] = { 0, 0 } ;	/* 初期化済フラグ */

#define HWPORT1	{ &initflag[0],  0, PCAT_COM1_BASE, PCAT_COM1_IRQ }
#define HWPORT2 { &initflag[1],  0, PCAT_COM2_BASE, PCAT_COM2_IRQ }

Inline short pcat_com_data_port(HWPORT *p)
{
  return p->base_address;
}

Inline short pcat_com_ier_port(HWPORT *p)
{
  return p->base_address + 1;
}

Inline short pcat_com_lsr_port(HWPORT *p)
{
  return p->base_address + 5;
}

#define PCAT_COM_IER_LS (4)
#define PCAT_COM_IER_TX (2)
#define PCAT_COM_IER_RX (1)

/*
 *  シリアルI/Oポートの初期化
 */

Inline BOOL
hw_port_initialize(HWPORT *p)
{
  outb(pcat_com_ier_port(p), 0); // Disable interrupt.
  outb(p->base_address + 3, 0x83); // DLAB=1, 8bit, stop=1, Non parity.
  outb(p->base_address, 0x0c);  // 9600bps
  outb(p->base_address + 1, 0x00);  // 9600bps
  outb(p->base_address + 3, 0x03); // DLAB=0, 8bit, stop=1, Non parity.
  outb(p->base_address + 2, 0x00);  // Disable FIFO.
  irc_ena_irq(p->com_irq);

  outb(p->base_address + 4, 0x0b); // Enable OUT2 interrupt.

  outb(pcat_com_ier_port(p),
       PCAT_COM_IER_LS |
       PCAT_COM_IER_TX |
       PCAT_COM_IER_RX);  // Enable interrupt (Ls & Tx & Rx)

  inb(pcat_com_lsr_port(p));
  inb(pcat_com_data_port(p));
  inb(p->base_address + 2);
  inb(p->base_address + 4);

  *(p->initflag) = 1;			/* 初期化フラグ設定 */
  return(FALSE);
}

/*
 *  シリアルI/Oポートの終了
 */
Inline void
hw_port_terminate(HWPORT *p)
{
    
  *(p->initflag) = -1;			/* 初期化フラグ設定 */
  outb(pcat_com_ier_port(p), 0); // Disable interrupt.
  irc_dis_irq(PCAT_COM1_IRQ);
}


/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void	serial_handler_in(int portid);
extern void	serial_handler_out(int portid);


#define PCAT_COM_IIR_RLS (0x06)
#define PCAT_COM_IIR_RXD (0x04)
#define PCAT_COM_IIR_TXD (0x02)

//static volatile UW txcount = 0;

/*
 *  シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler()
{
  UB comstat;

  irc_eoi_irq(PCAT_COM1_IRQ);

  comstat = inb(PCAT_COM1_BASE + 2) & 0x0f;

  switch (comstat)
    {
    case PCAT_COM_IIR_RLS:
      inb(PCAT_COM1_BASE + 5);
      break;
    case PCAT_COM_IIR_RXD:
      serial_handler_in(1);
      break;
    case PCAT_COM_IIR_TXD:
      serial_handler_out(1);
      break;
    default:
      break;
    }

  irc_ena_irq(PCAT_COM1_IRQ);

}


/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
  UB stat;
  stat = inb(pcat_com_lsr_port(p));

  return ((stat & 0x01) != 0);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
  UB stat;
  stat = inb(pcat_com_lsr_port(p));

  return ((stat & 0x60) != 0);
}

/*
 *  受信した文字の取り出し
 */
Inline UB
hw_port_getchar(HWPORT *p)
{
  UB ch;
  
  ch = inb(pcat_com_data_port(p));

  return ch;
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, UB c)
{
  outb(pcat_com_data_port(p), c);
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
  if (p->sendflag == 0)
    {
      UB stat;
      stat = inb(pcat_com_ier_port(p)) | PCAT_COM_IER_TX;
      outb(pcat_com_ier_port(p), stat);
      p->sendflag = 1;
    }
}

Inline void
hw_port_sendstop(HWPORT *p)
{
  if (p->sendflag)
    {
      UB stat;
      outb(p->base_address + 3, 0x03); // DLAB=0, 8bit, stop=1, Non parity.
      stat = inb(pcat_com_ier_port(p)) & ~PCAT_COM_IER_TX;
      outb(pcat_com_ier_port(p), stat);
      p->sendflag = 0;
    }
}


#endif /* _HW_SERIAL_H_ */
