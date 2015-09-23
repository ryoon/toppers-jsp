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
 *  @(#) $Id: sys_config.c,v 1.2 2001/03/09 05:23:01 honda Exp $
 */


#include "jsp_kernel.h"
#include "ms7709ase01.h"
#include "sh3.h"

/*
 *  プロセッサ識別のための変数 (マルチプロセッサ対応)
 */
INT	board_id;			/* ボードID */
INT	board_addr;			/* ローカルメモリの先頭アドレス */


/*
 *  SMSC Super I/O (FDC37C935A)の初期化
 */

void
smsc_init()
{
    /*
     * Enter Config mode 
     */
    *SMSC_CONFIG_PORT = 0x55 << 8;
    *SMSC_CONFIG_PORT = 0x55 << 8;


    /*
     * Init SCI0
     */
    /* Power on */
    smsc_config_write(0x22, (smsc_config_read(0x22) | 0x10));
    /* Select SCI0 */
    smsc_config_write(0x07, 0x04);
    /* Enable SCI0 */
    smsc_config_write(0x30, 0x01);
    /* Set SCI0 Base Address */
    smsc_config_write(0x60, (SMSC_SCI0_BASE_ADDR & 0xff00) >> 8);
    smsc_config_write(0x61, (SMSC_SCI0_BASE_ADDR & 0xff));    
    /* IRQ4 */
    smsc_config_write(0x70, 0x04);

    

    /*
     *  Exit Config mode
     */
    *SMSC_CONFIG_PORT = 0xAA << 8;
}



/*
 *  ターゲットシステム依存 初期化ルーチン
 */
#define  INIT_DELAY 20000000
void
sys_initialize()
{
    /* Enable IRL Interrupt */
    *ICR1 = 0x5000;

    /* Init Super I/O */
    smsc_init();
}


/*
 *  ターゲットシステムの終了ルーチン
 *
 *  システムを終了する時に使う．通常はモニタ呼び出しで実現する．
 *  
 */
void
sys_exit(void)
{
  Asm("trapa #0xff"::);
}

/*
 *   システム文字出力先の指定
 */

#ifdef WITH_STUB
#define SYS_PUT_CHAR(c) stub_putc(c)
#else
#define SYS_PUT_CHAR(c) scif_putc(c)
#endif /* WITH_STUB */


void
sys_putc(char c)
{
    if (c == '\n') {
        SYS_PUT_CHAR('\r');
    }
    SYS_PUT_CHAR(c);
}


