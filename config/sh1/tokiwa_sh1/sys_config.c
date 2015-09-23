/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: sys_config.c,v 1.1 2001/05/04 09:10:29 imai Exp $
 */


#include "jsp_kernel.h"
#include "tokiwa_sh1.h"


/*
 *  プロセッサ識別のための変数 (マルチプロセッサ対応)
 */
INT	board_id;			/* ボードID */
INT	board_addr;			/* ローカルメモリの先頭アドレス */


/*
 *  ターゲットシステム依存 初期化ルーチン
 */
void
sys_initialize()
{
			/*  エリア0：リードサイクルでWAIT信号による  	*/
			/*  ウェートステート挿入なし  			*/
	*WCR1 &= ~WCR1_RW0;
			/*  エリア0,2：1ステートロングウェイト  	*/
	*WCR3 &= WCR3_A02LW_MASK;
}


/*
 *  ターゲットシステムの終了ルーチン
 *
 *  システムを終了する時に使う．通常はモニタ呼び出しで実現する．
 *  	注意：モニタ完成後に実装
 */
void
sys_exit(void)
{
#ifdef WITH_STUB
  Asm("trapa #0xff"::);
#else
	while(1);
#endif /* WITH_STUB */
}

/*
 *   システム文字出力先の指定
 */

#ifdef WITH_STUB
#define SYS_PUT_CHAR(c) stub_putc(c)
#else
#include "serial.h"	/*  serial_write( )  */
#define SYS_PUT_CHAR(c) {		\
	char cc=c;			\
	serial_write(1, &cc, 1);	\
}
#endif /* WITH_STUB */


void
sys_putc(char c)
{
    if (c == '\n') {
        SYS_PUT_CHAR('\r');
    }
    SYS_PUT_CHAR(c);
}


