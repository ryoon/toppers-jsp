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
 *  @(#) $Id: sys_support.S,v 1.2 2001/09/25 15:13:45 hiro Exp $
 */

/*
 *	ターゲットシステム依存モジュール アセンブリ言語部（DVE68K/40用）
 */

#define	_MACRO_ONLY
#include "jsp_kernel.h"

/*
 *  DVE68K/40 CPUボード上のレジスタアドレスの定義
 */
#define BOARD_REG0	0xfff48000	/* DIPスイッチ */
#define BOARD_REG1	0xfff48004	/* ボードコントロールレジスタ */
#define BOARD_REG2	0xfff48008	/* フラッシュメモリ割込みクリア */

#define DGA_CSR0	0xfff44000	/* DGA-001 のレジスタ */
#define DGA_CSR1	0xfff44004

/*
 *  低レベルのターゲットシステム依存の初期化
 *
 *  スタートアップモジュールの中で，メモリの初期化の前に呼び出される．
 */
	.text
	.globl hardware_init_hook
hardware_init_hook:	
	move.l #0x09, BOARD_REG1	/* コントロールレジスタの初期化 */
	move.l #0x2c07e000, DGA_CSR0	/* CSR0 の初期化 */
	move.l #0x60000300, DGA_CSR1	/* CSR1 の初期化 */
	rts
