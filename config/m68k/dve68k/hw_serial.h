/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: hw_serial.h,v 1.1 2000/11/14 14:44:16 hiro Exp $
 */

/*
 *	ターゲット依存シリアルI/Oモジュール（DVE68K/40用）
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

#include "dve68k.h"
#include "sys_config.h"

/*
 *  シリアルポートのハードウェア依存情報の定義
 */
typedef struct hardware_serial_port_descripter {
	INT	*initflag;	/* 初期化済フラグへのポインタ */
	IOREG	*data;		/* データレジスタの番地 */
	IOREG	*cntrl;		/* コントロールレジスタの番地 */
	BOOL	sendflag;	/* 送信割込みイネーブルフラグ */

	byte	cr3_def;	/* デフォールトの設定値 (CR3) */
	byte	cr4_def;	/* デフォールトの設定値 (CR4) */
	byte	cr5_def;	/* デフォールトの設定値 (CR5) */
	byte	brg2_def;	/* デフォールトの設定値 (ボーレート下位) */
	byte	brg1_def;	/* デフォールトの設定値 (ボーレート上位) */
} HWPORT;

/*
 *  MPSCコントロールレジスタの設定値
 */

#define MPSC_RESET	0x18		/* ポートリセットコマンド */
#define MPSC_EOI	0x38		/* EOI (End of Interrupt) */

#define CR3_DEF		0xc1		/* データ 8bit, 受信イネーブル */
#define CR4_DEF		0x44		/* ストップビット 1bit, パリティなし */
#define CR5_DEF		0xea		/* データ 8bit, 送信イネーブル */
#define BRG2_DEF	0x1e		/* 9600bps (下位) */
#define BRG1_DEF	0x00		/* 9600bps (上位) */

#define CR10_DEF	0x00		/* NRZ */
#define CR14_DEF	0x07		/* ボーレートジェネレータイネーブル */
#define CR15_DEF	0x56		/* ボーレートジェネレータ使用 */

#define CR1_ALL		0x12		/* 全割込みを許可 */
#define CR1_RECV	0x10		/* 受信割込みのみ許可 */
#define CR1_DOWN	0x00		/* 全割込みを禁止 */

/*
 *  シリアルI/Oの割込みハンドラのベクタ番号
 */
#define INHNO_SERIAL	GP0_VEC

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	2	/* サポートするシリアルポートの数 */

static INT	initflag[2] = { 0, 0 } ;	/* 初期化済フラグ */

#define HWPORT1		{ &initflag[0], MPSC_DATAA, MPSC_CNTRLA, 0,	\
			  CR3_DEF, CR4_DEF, CR5_DEF, BRG2_DEF, BRG1_DEF }
#define HWPORT2		{ &initflag[1], MPSC_DATAB, MPSC_CNTRLB, 0,	\
			  CR3_DEF, CR4_DEF, CR5_DEF, BRG2_DEF, BRG1_DEF }

/*
 *  シリアルI/Oポートの初期化
 */
Inline BOOL
hw_port_initialize(HWPORT *p)
{
	/*
	 *  MPSC の設定
	 */
	io_write(p->cntrl, MPSC_RESET);
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		mpsc_write(MPSC_CNTRLA, MPSC_CR2, 0x18);
		mpsc_write(MPSC_CNTRLB, MPSC_CR2, 0x00);
	}
	mpsc_write(p->cntrl, MPSC_CR1, CR1_RECV);
	p->sendflag = 0;
	mpsc_write(p->cntrl, MPSC_CR4, p->cr4_def);
	mpsc_write_brg(p->cntrl, MPSC_CR12, 0x01, p->brg2_def, p->brg1_def);
	mpsc_write_brg(p->cntrl, MPSC_CR12, 0x02, p->brg2_def, p->brg1_def);
	mpsc_write(p->cntrl, MPSC_CR15, CR15_DEF);
	mpsc_write(p->cntrl, MPSC_CR14, CR14_DEF);
	mpsc_write(p->cntrl, MPSC_CR10, CR10_DEF);
	mpsc_write(p->cntrl, MPSC_CR3, p->cr3_def);
	mpsc_write(p->cntrl, MPSC_CR5, p->cr5_def);

	/*
	 *  割込み関連の設定
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		dga_set_ilv(DGA_CSR25, GP0IL_BIT, IRQ_LEVEL6);
						/* 割込みレベル設定 */
		*DGA_CSR21 |= GP0_BIT;		/* 割込みマスク解除 */
	}

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

	/*
	 *  MPSC の割込み関係の設定
	 */
	mpsc_write(p->cntrl, MPSC_CR1, CR1_DOWN);
	p->sendflag = 0;

	/*
	 *  割込み関連の設定
	 */
	if (initflag[0] <= 0 && initflag[1] <= 0) {
		*DGA_CSR21 &= ~GP0_BIT;		/* 割込みマスク設定 */
	}
}

/*
 *  シリアルポート割込みサービスルーチン（前方参照）
 */
extern void	serial_handler_in(int portid);
extern void	serial_handler_out(int portid);

/*
 *  シリアルI/Oポートの割込みハンドラ
 */
Inline void
hw_serial_handler()
{
	if (initflag[0] > 0) {
		serial_handler_in(1);
		serial_handler_out(1);
	}
	if (initflag[1] > 0) {
		serial_handler_in(2);
		serial_handler_out(2);
	}
	mpsc_write(MPSC_CNTRLA, MPSC_CR0, MPSC_EOI);
}

/*
 *  文字を受信したか？
 */
Inline BOOL
hw_port_getready(HWPORT *p)
{
	return((mpsc_read(p->cntrl, MPSC_SR0) & 0x01) != 0);
}

/*
 *  文字を送信できるか？
 */
Inline BOOL
hw_port_putready(HWPORT *p)
{
	return((mpsc_read(p->cntrl, MPSC_SR0) & 0x04) != 0);
}

/*
 *  受信した文字の取り出し
 */
Inline byte
hw_port_getchar(HWPORT *p)
{
	return(io_read(p->data));
}

/*
 *  送信する文字の書き込み
 */
Inline void
hw_port_putchar(HWPORT *p, byte c)
{
	io_write(p->data, c);
}

/*
 *  送信制御関数
 */
Inline void
hw_port_sendstart(HWPORT *p)
{
	if (!(p->sendflag)) {
		mpsc_write(p->cntrl, MPSC_CR1, CR1_ALL);
		p->sendflag = 1;
	}
}

Inline void
hw_port_sendstop(HWPORT *p)
{
	if (p->sendflag) {
		mpsc_write(p->cntrl, MPSC_CR1, CR1_RECV);
		p->sendflag = 0;
	}
}

#endif /* _HW_SERIAL_H_ */
