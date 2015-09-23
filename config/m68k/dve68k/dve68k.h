/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: dve68k.h,v 1.3 2002/04/10 11:08:33 hiro Exp $
 */

/*
 *	DVE68K/40 CPUボードのハードウェア資源の定義
 */

#ifndef _DVE68K_H_
#define _DVE68K_H_

typedef	unsigned char	byte;
typedef volatile byte	IOREG;		/* I/Oレジスタの型 */
typedef volatile int	LIOREG;

/*
 *  低速の I/Oデバイスを操作するための関数
 */

Inline void
cpu_wait_io(void)
{
	Asm("nop");
	Asm("nop");
	Asm("nop");
	Asm("nop");
}

Inline byte
io_read(IOREG *addr)
{
	byte	val;

	val = *addr;
	cpu_wait_io();
	return(val);
}

Inline void
io_write(IOREG *addr, byte val)
{
	*addr = val;
	cpu_wait_io();
}

/*
 *  CPUボード上のレジスタ
 */

#define BOARD_REG0	((LIOREG *) 0xfff48000)
#define BOARD_REG1	((LIOREG *) 0xfff48004)
#define BOARD_REG2	((LIOREG *) 0xfff48008)

/*
 *  DGA-001 のレジスタ
 */

#define DGA_CSR0_1	((IOREG *) 0xfff44001)
#define DGA_CSR1	((LIOREG *) 0xfff44004)
#define DGA_CSR3	((LIOREG *) 0xfff4400c)
#define DGA_CSR4	((LIOREG *) 0xfff44010)
#define DGA_CSR5	((LIOREG *) 0xfff44014)
#define DGA_CSR12	((LIOREG *) 0xfff44030)
#define DGA_CSR13	((LIOREG *) 0xfff44034)
#define	DGA_CSR19	((LIOREG *) 0xfff4404c)
#define	DGA_CSR20	((LIOREG *) 0xfff44050)
#define	DGA_CSR21	((LIOREG *) 0xfff44054)
#define DGA_CSR23	((LIOREG *) 0xfff4405c)
#define DGA_CSR24	((LIOREG *) 0xfff44060)
#define	DGA_CSR25	((LIOREG *) 0xfff44064)
#define	DGA_IFR0	((LIOREG *) 0xfff44070)
#define DGA_IFR3	((LIOREG *) 0xfff4407c)

/*
 *  DGA の割込みレベル設定のための定義
 */

#define	IRQ_NMI		(0x7)		/* ノンマスカブル割込み */
#define	IRQ_LEVEL6	(0x6)		/* 割込みレベル6 */
#define	IRQ_LEVEL5	(0x5)		/* 割込みレベル5 */
#define	IRQ_LEVEL4	(0x4)		/* 割込みレベル4 */
#define	IRQ_LEVEL3	(0x3)		/* 割込みレベル3 */
#define	IRQ_LEVEL2	(0x2)		/* 割込みレベル2 */
#define	IRQ_LEVEL1	(0x1)		/* 割込みレベル1 */

#define ABTIL_BIT	(24)		/* アボート割込み */
#define SQRIL_BIT	(8)		/* SRQ 割込み */

#define TT0IL_BIT	(16)		/* タイマ0 割込み */
#define GP0IL_BIT	(0)		/* シリアルI/O 割込み */

Inline void
dga_set_ilv(LIOREG *addr, int shift, int val)
{
	*addr = (*addr & ~(0x07 << shift)) | (val << shift);
} 

/*
 *  割込みベクトルの定義
 */

#define	G0I_VEC		(0x40)		/* グループ0 割込みベクトル */
#define	G1I_VEC		(0x48)		/* グループ1 割込みベクトル */
#define	SWI_VEC		(0X50)		/* ソフトウェア割込みベクトル */
#define	SPRI_VEC	(0x40)		/* スプリアス割込みベクトル */

#define ABT_VEC		(G0I_VEC + 6)	/* アボート割込みベクトル */
#define SQR_VEC		(G0I_VEC + 2)	/* SQR 割込みベクトル */
#define	TT0_VEC		(G1I_VEC + 4)	/* タイマ0 割込みベクトル */
#define	GP0_VEC		(G1I_VEC + 0)	/* シリアルI/O 割込みベクトル */

/*
 *  割込み制御ビットの定義
 */

#define ABT_BIT		(0x40000000)	/* アボート割込みビット */
#define SQR_BIT		(0x04000000)	/* SQR 割込みビット */
#define	TT0_BIT		(0x00100000)	/* タイマ0 割込みビット */
#define	GP0_BIT		(0x00010000)	/* シリアルI/O 割込みビット */

/*
 *  MPSC (μPD72001-11) のレジスタ
 */

#define	MPSC_DATAA	((IOREG *) 0xfff45003)
#define	MPSC_CNTRLA	((IOREG *) 0xfff45007)
#define	MPSC_DATAB	((IOREG *) 0xfff4500b)
#define	MPSC_CNTRLB	((IOREG *) 0xfff4500f)

#define	MPSC_CR0	0x00		/* MPSC コントロールレジスタ */
#define	MPSC_CR1	0x01
#define	MPSC_CR2	0x02
#define	MPSC_CR3	0x03
#define	MPSC_CR4	0x04
#define	MPSC_CR5	0x05
#define	MPSC_CR10	0x0a
#define	MPSC_CR12	0x0c
#define	MPSC_CR14	0x0e
#define	MPSC_CR15	0x0f

#define	MPSC_SR0	0x00		/* MPSC ステータスレジスタ */

Inline byte
mpsc_read(IOREG *addr, int reg)
{
	io_write(addr, reg);
	return(io_read(addr));
}

Inline void
mpsc_write(IOREG *addr, int reg, int val)
{
	io_write(addr, reg);
	io_write(addr, val);
}

Inline void
mpsc_write_brg(IOREG *addr, int reg, int val, int brg2, int brg1)
{
	io_write(addr, reg);
	io_write(addr, val);
	io_write(addr, brg2);
	io_write(addr, brg1);
	(void) io_read(addr);		/* ダミーリード */
}

/*
 *  モニタ呼び出しルーチン
 */

Inline void
dve68k_exit(void)
{
	Asm("clr.l %%d0; trap #3"
	  : /* no output */
	  : /* no input */
	  : "d0", "d1", "d2", "d6", "d7");
}

Inline int
dve68k_getc(void)
{
	int	c;

	Asm("moveq.l #3, %%d0; trap #3; move.l %%d0, %0"
	  : "=g"(c)
	  : /* no input */
	  : "d0", "d1", "d2", "d6", "d7");
	  return(c);
}

Inline int
dve68k_putc(int c)
{
	Asm("moveq.l #5, %%d0; move.l %0, %%d1; trap #3"
	  : /* no output */
	  : "g"(c)
	  : "d0", "d1", "d2", "d6", "d7");
	  return(c);
}

#endif /* _DVE68K_H_ */
