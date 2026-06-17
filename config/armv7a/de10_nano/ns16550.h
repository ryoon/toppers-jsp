/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: ns16550.h 1019 2026-03-05 18:56:56Z ertl-honda $
 */

/*
 *   NS16550 用 簡易SIOドライバ
 */
#ifndef _NS16550_H_
#define _NS16550_H_

#define UART_CLOCK	100000000

/*
 *  UART
 */
#define UART0_BASE	0xFFC02000
#define UART1_BASE	0xFFC03000

#define UART0_LCR_VAL 0
#define UART0_DLM_VAL 0
#define UART0_DLL_VAL 0

#define UART1_LCR_VAL 0
#define UART1_DLM_VAL 0
#define UART1_DLL_VAL 0

/*
 * 各レジスタのオフセット
 */
#define NS16550_RHR   0x00   /* B  LCR bit7=0 Read  */
#define NS16550_THR   0x00   /* B  LCR bit7=0 Write */
#define NS16550_DLL   0x00   /* B  LCR bit7=1       */
#define NS16550_IER   0x04   /* B  LCR bit7=0       */
#define NS16550_DLM   0x04   /* B  LCR bit7=1       */
#define NS16550_IIR   0x08   /* B  Read  */
#define	NS16550_LCR   0x0c   /* B  */
#define NS16550_MCR   0x10   /* B  */
#define NS16550_LSR   0x14   /* B  Read */
#define NS16550_MSR   0x18   /* B  Read */
#define NS16550_SCR   0x1c   /* B  */
#define NS16550_FCR   0x20   /* B  Write */

#define IIR_TX            0x02       /* 送信割り込み発生 */
#define IER_TX            0x02       /* 送信割り込み許可 */
#define IIR_RX            0x01       /* 受信割り込み発生 */
#define IER_RX            0x01       /* 受信割り込み許可 */


#define LCR_DL_MODE       0x80       /* Divisor Enable */
#define LCR_VAL           0x03       /* 8bit,1stop,Noparity,No break */
#define FCR_FIFO_DISABLE  0x00
#define FCR_FIFO_ENABLE   0x01

#define LSR_RX_DATA_READY 0x01
#define LSR_TX_THRE       0x20
#define LSR_TX_EMPTY      0x40

#ifndef _MACRO_ONLY

/*
 *  SIL関数のマクロ定義
 */
#define sil_orb_mem(a, b)		sil_wrb_mem((a), sil_reb_mem(a) | (b))
#define sil_andb_mem(a, b)		sil_wrb_mem((a), sil_reb_mem(a) & ~(b))
#define sil_modb_mem(a, b, c)	sil_wrb_mem((a), (sil_reb_mem(a) & (~b)) | (c))


/*
 * 文字を受信したか?
 */ 
Inline BOOL
ns16550_getready(const SIOPINIB *siopinib)
{
	UB	status = sil_reb_mem((VP)(siopinib->reg_base + NS16550_LSR));
	return((status & LSR_RX_DATA_READY));
}

/*
 * 文字を送信できるか?
 */
Inline BOOL
ns16550_putready(const SIOPINIB *siopinib)
{
	UB	status = sil_reb_mem((VP)(siopinib->reg_base + NS16550_LSR));
	return (status & LSR_TX_THRE) == 0;
}

/*
 *  受信した文字の取り出し
 */
Inline UB
ns16550_getchar(const SIOPINIB *siopinib)
{
	return(sil_reb_mem((VP)(siopinib->reg_base + NS16550_RHR)));
}

/*
 *  送信する文字の書き込み
 */
Inline void
ns16550_putchar(const SIOPINIB *siopinib, UB c)
{
	sil_wrb_mem((VP)(siopinib->reg_base + NS16550_THR), c);
}

/*
 *  送信割込み許可
 */
Inline void
ns16550_enable_send(const SIOPINIB *siopinib)
{
	sil_orb_mem((VP)(siopinib->reg_base + NS16550_IER), IER_TX);
}

/*
 *  送信割込み禁止
 */
Inline void
ns16550_disable_send(const SIOPINIB *siopinib)
{
	sil_andb_mem((VP)(siopinib->reg_base + NS16550_IER), IER_TX);
}

/*
 *  受信割込み許可
 */
Inline void
ns16550_enable_rcv(const SIOPINIB *siopinib)
{
	sil_orb_mem((VP)(siopinib->reg_base + NS16550_IER), IER_RX);
}

/*
 *  受信割込み禁止
 */
Inline void
ns16550_disable_rcv(const SIOPINIB *siopinib)
{
	sil_andb_mem((VP)(siopinib->reg_base + NS16550_IER), IER_RX);
}

/*
 * SIOPINIBの初期化
 */
Inline void
ns16550_init_siopinib(const SIOPINIB *siopinib)
{
	UW divisor = ((UART_CLOCK + (8 * DEF_BAUDRATE)) / (16 * DEF_BAUDRATE));

	/*
	 *  分周比の設定
	 */
	/* Divisor Enable */
	sil_orb_mem((VP)(siopinib->reg_base + NS16550_LCR), LCR_DL_MODE);
	sil_wrb_mem((VP)(siopinib->reg_base + NS16550_LCR),  divisor & 0xff);
	sil_wrb_mem((VP)(siopinib->reg_base + NS16550_LCR),  (divisor >> 8) & 0xff);

	/* Divisor Disable */
	sil_andb_mem((VP)(siopinib->reg_base + NS16550_LCR), LCR_DL_MODE);

	/* モード設定 */
	sil_wrb_mem((VP)(siopinib->reg_base + NS16550_LCR),  LCR_VAL);

	/* FIFO Disable */
	sil_wrb_mem((VP)(siopinib->reg_base + NS16550_FCR),  FCR_FIFO_DISABLE);

	/* 割込み禁止 */
	sil_wrb_mem((VP)(siopinib->reg_base + NS16550_IER),  0x80);
}

#endif	/* _MACRO_ONLY */
#endif	/* _NS16550_H_ */
