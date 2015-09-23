/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: sys_config.c,v 1.2 2002/04/05 09:01:29 takayuki Exp $
 */

/*
 *	ターゲットシステム依存モジュール（M3A-2131G50用）
 */

#include "jsp_kernel.h"
#include "hw_serial.h"


/*
 *  ターゲットシステム依存の初期化
 */
void
sys_initialize()
{
	*(volatile unsigned short *)0xef106e = 0;		//ポート7を使用する
	*(volatile unsigned char  *)0xef1047 = 0xff;	//出力モード
	*(volatile unsigned char *)LED = 0;
	hw_port_initialize(0);
}

/*
 *  ターゲットシステムの終了
 */
void
sys_exit(void)
{
	dis_int();
	*(volatile unsigned char *)(ICUIMASK+1) = 0;
	while(1==1)
	{
		*(volatile unsigned char *)LED = (*(volatile unsigned char *)LED) +1;
			/* 低消費電力モードへ移行 */
		__asm(	"stb	%0, @%1	\n"
				"ldb	%0, @%1 \n"
			  	"nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop"
			: : "r"(1), "r"(CLKMOD+3) );
	}
}

/*
 *  ターゲットシステムの文字出力
 */
void
sys_putc(char c)
{
//	if(c == '\n')
//	{
//	    while( ( *(volatile char *)(SIOSTS(SERIAL_PORT)+3) & 0x1) == 0);
//	    *(volatile char *)(SIOTXB(SERIAL_PORT)+3) = '\r';
//	}
    while( ( *(volatile char *)(SIOSTS(SERIAL_PORT)+3) & 0x1) == 0);
    *(volatile char *)(SIOTXB(SERIAL_PORT)+3) = c;
}
