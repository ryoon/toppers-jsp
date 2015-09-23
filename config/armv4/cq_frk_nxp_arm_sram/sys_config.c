/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2004 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *                2009 by Suikan
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
 */

/*
 *    ターゲットシステム依存モジュール（CQ-FRK-NXP-ARM）
 */

#include "jsp_kernel.h"
#include <lpc2388.h>



/*
 * OSCの初期化(システム固有). 4MHz の内部オシレータを使う
 */
static void
init_clock()
{
	/* 1. もしPLLを使用しているなら、ディスコネクトし、フィード・シーケンスを踏む */
	PLLCON &= 0xFFFFFFFD;	/* Clear bit 1, PLL bypassed */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 2. PLLをディセーブルし、フィード・シーケンスを踏む。 */
	PLLCON = 0;		/* Disable PLL */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 3. CPUクロック・ディバイダの設定を、PLLなしでも遅くなりすぎないように設定する。 */
	CCLKCFG = 0;	/* CCLK = PCLK / 1 */

	/* 4. PLLのクロックソースを変更する。 */
	CLKSRCSEL = 0;	/* Select 4MHz internal oscillator*/
//	CLKSRCSEL = 1;	/* Select 12MHz main oscillator*/

	/* 5. PLLCFGへの書き込みを行い、フィード・シーケンスを踏んで有効化する。
	 * PLLCFGは、PLLがディセーブルのときにだけ変更できる。 */
	PLLCFG = 35;	/* Divide by 1, Multiply by 72, PCLK = 4*72 = 288MHz */
//	PLLCFG = 11;	/* Divide by 1, Multiply by 24, PCLK = 12*24 = 288MHz */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 6. PLLをイネーブルにし、フィード・シーケンスを踏む. */
	PLLCON = 1;		/* Enable PLL */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;

	/* 7. CPUクロック・ディバイダの値を、PLL動作用に変更する。
	 * これは必ず、PLLをクロック源として接続する前に行う。 */
	CCLKCFG = 3;	/* CCLK = PCLK/4 = 288/4 = 72MHz */

	/* 8. PLLSTATレジスタのPLOCK bitを調べて、PLLがロックするまで待つ。
	 * あるいはPLOCK割り込みを使用してもいい。あるいは、PLLの入力クロックが小さい(32kHzなど)なら、
	 * 単純にウェイト・ループを回してもいい。*/
	while ( !(PLLSTAT & (1<<26)))
		;

	/* 9. PLLをシステムに接続し、フィードシーケンスを実行する */
	PLLCON = 3;		/* Select PLL as CPU clock source */
	PLLFEED = 0xAA;
	PLLFEED = 0x55;
}




/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
    int i;

    init_vector();			/* SRAMのリマップ。LPC2388共通 */

    init_clock();			/* PLLの設定。システム固有 */

    init_uart0();			/* UART0の初期化。システム固有 */

    init_IRQ();				/* VICを初期化する。LPC2388共通 */

    		/* 必要なペリフェラルの電源を入れる(システム固有) */
			/* なお、UART0とTIMER3はLPC2388共通で必要 */
	PCONP |= 0x00800008;	/* bit3:UART0, bit23:TIMER3 */

			/* UART0のピンを設定 (LPC2388共通) */
	PINSEL0 &= ~0xF0;			/* bit 4-7だけをクリア */
	PINSEL0 |= 0x50;			/* UART入出力を割り当て */

    /*
     *  ARMのベクタテーブルへ割込みハンドラを登録（どのARMv4システムでも要求される）
     */
    arm_install_handler(IRQ_Number,IRQ_Handler);

}





