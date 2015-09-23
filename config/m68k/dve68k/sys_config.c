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
 *  @(#) $Id: sys_config.c,v 1.3 2002/04/10 11:08:33 hiro Exp $
 */

/*
 *	ターゲットシステム依存モジュール（DVE68K/40用）
 */

#include "jsp_kernel.h"
#include "dve68k.h"

/*
 *  プロセッサ識別のための変数（マルチプロセッサ対応）
 */
INT	board_id;		/* ボードID */
INT	board_addr;		/* ローカルメモリの先頭アドレス */

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	/*
	 *  プロセッサ識別のための変数の初期化
	 */
	board_id = *BOARD_REG0 & 0x1f;
	board_addr = board_id << 24;

	/*
	 *  割込み関連の初期化
	 */
	*DGA_CSR21 = 0;			/* すべての割込みをマスク */
	*DGA_CSR23 = ~0;		/* 全ての割込みをクリア */

	*DGA_CSR19 = (G0I_VEC << 24) | (G1I_VEC << 16)
				| (SWI_VEC << 8) | SPRI_VEC;
					/* 割込みベクトルの設定 */

	/*
	 *  アボート割込みの設定（NMI）
	 */
	dga_set_ilv(DGA_CSR24, ABTIL_BIT, IRQ_NMI);
					/* 割込みレベル設定 */
	*DGA_CSR23 = ABT_BIT;		/* 割込み要求をクリア */
	*DGA_CSR21 |= ABT_BIT;		/* 割込みマスク解除 */

	/*
	 *  メモリ領域の設定
	 */
	*DGA_CSR4 = (board_addr | 0x00ff);	/* ローカルメモリ 16MB */
	*DGA_CSR5 = 0x0000012f;		/* VMEから拡張アドレスアクセス */

	/*
	 *  インタフェースレジスタ（IFR）の設定
	 */
	*DGA_CSR3 = ((board_id << 4) | 0x3);	/* ベースアドレス */
	*DGA_IFR0 = 0x80000000;			/* SQR フラグクリア */
	*DGA_IFR3 = board_id;			/* IFR3 を設定 */

	/*
	 *  ラウンドロビンモードに設定（マルチプロセッサ対応）
	 */
	*DGA_CSR1 = (*DGA_CSR1 & 0xffeffcff)
			| (1 << 20) | ((board_id % 4) << 8);
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
	dve68k_exit();
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		dve68k_putc('\r');
	}
	dve68k_putc(c);
}
