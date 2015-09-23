/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id$
 */

/*
 *    ターゲットシステム依存モジュール（EZARM7用）
 */

#include "jsp_kernel.h"
#include <lpc2214.h>



void
hardware_init_hook()
{
    extern int __ivector_end;
    int i;
    VB *sram;

    sram = (VB *)0x40000000;
    /*
     *  Remap vector to Static RAM.
     */
    for(i = 0; i < 0x40; i++) {
      sram[i] = *((VB *)i);
    }
    sil_wrw_mem((VP)(0xe01fc040), 2);
}

/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
    int i;

    sil_wrw_mem((VP *)(0xfffff034), undef_interrupt);

    /*
     *  ARMのベクタテーブルへ割込みハンドラを登録
     */
    arm_install_handler(IRQ_Number,IRQ_Handler);
  
    /*
     *  sys_putc が可能になるようにUARTを初期化
     */
    uart_init();
}


/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
    syslog(LOG_EMERG, "End Kernel.....!");
    while(1);
}




/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }
  uart_putc(c);  
}


/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
void
define_inh(INHNO inhno, FP inthdr)
{
    VW *base;
    assert(inhno < MAX_INT_NUM);

    base = (VW *)(0xfffff100);
    sil_wrw_mem((VP *)(&base[inhno]), inthdr);
    sil_wrw_mem((VP *)0xfffff010, 1 << inhno);

    base = (VW*)0xfffff200;
    sil_wrw_mem((VP *)&(base[inhno]), inhno | 0x20);
}


/*
 * 割込み優先度を設定．
 */
void
define_ipm(INHNO inhno, PRI pri)
{
    VW *base;

    assert(inhno < MAX_INT_NUM);
#if 0
    base = (VW*)0xfffff200;
    if (pri == 0) {
	sil_wrw_mem((VP *)&(base[inhno]), 0);
    } else {
	pri = 16 + pri;
	assert(0 <= pri && pri <= 15);
	sil_wrw_mem((VP *)&(base[inhno]), pri | 0x20);
    }
#endif
}


void
dis_int(INHNO inhno)
{
  sil_wrw_mem((VP *)0xfffff014, 1 << inhno);
}

void
ena_int(INHNO inhno)
{
  sil_wrw_mem((VP *)0xfffff010, 1 << inhno);
}



/*
 *  未定義の割込みが入った場合の処理
 */
void
undef_interrupt(){
    syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
    while(1);
}
