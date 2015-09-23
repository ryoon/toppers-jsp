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
 *  割込み用のテーブル
 */
#define VICVectAddress ((volatile FP **)0xFFFFF100)




/*
 *  割込み関連の初期化(LPC2388共通)
 */
void
init_IRQ()
{
	int i;

	for( i=0; i<32; i++ )
		VICVectAddress[i] = (volatile FP *)undef_interrupt;
}

extern unsigned int __data_start;

/*
 * LPC2388のSRAM領域にFLASHのベクトルをコピーし、
 * リマップする。LPC2388共通で必要。
 */
void
init_vector()
{
	unsigned char *flash, *sram;
	int i;

		/* ROM化コードとSRAM上で動くコードでは、例外ベクトルの扱いが違う。
		 * ROM化する場合は、SRAM REMAPの前にFlash ROM上の例外ベクトルの値を
		 * SRAM上のリマップ領域に移さなければならない。一方、SRAM上で動作する
		 * 場合は、リマップ領域へはデバッガが初期値を書き込む。
		 * このため、リマップ領域の初期化手順が必要か否かの判別を行う。判別は
		 * __data_startの値を使う。この値はlpc2388_sram.ldの中で0に初期化して
		 * いる。lpc2388.ldの中では非ゼロのアドレスが代入される。したがって、
		 * リマップ領域の初期化は__data_startが非ゼロのときだけ行う。 */
	if ( __data_start != 0 )
	{
		flash =(unsigned char*) 0;				/* FLASHの先頭アドレス */
		sram = (unsigned char*) 0x40000000;	/* SRAMの先頭アドレス */

			/* FLASHの先頭64バイトをSRAMにコピー */
		for (i=0; i<64; i++)
		{
			*sram = *flash;
			sram++;
			flash++;
		}
	}
		/* SRAM をベクトルエリアにリマップする */
	MEMMAP = 0x02;
}

/*
 * LPC2388依存のUART初期化ルーチン。sys_putc()用にuart0を初期化する。
 */
#define	DLAB 0x80
void
init_uart0()
{
    /*
     *  分周比の設定
     */
    /* Divisor Enable */
	U0LCR |= DLAB;
    U0DLL = UART0_DIVISOR & 0xFF ;
    U0DLM = UART0_DIVISOR >> 8;
    /* Divisor Disable */
    U0LCR &= ~DLAB;

    /* モード設定, パリティ無し 8bit data, 1 stop bit */
    U0LCR = 0x03;

    /* 割込み禁止 */
    U0IER = 0;
}

/*
 *  システム終了フック。ファームウェアにシステム終了APIがあるなら
 *  それを呼ぶとよい。
 */
void
sys_exit()
{
	while(1)
		;
}



/*
 *  ターゲットシステムの文字出力。syslogタスクが起動する前にプリントを行うための
 *  ルーチンであり、内部ではポーリングを行っている。LF記号が送られてきたら、
 *  CR LFに変換してプリントする。sys_putcはデバイスを初期化しない。
 *  初期化はkernel_start()の中で終わっていると仮定している。
 */

void
sys_putc(char c)
{
	if ( c== 0x0A )			/* もし LF ならば */
		sys_putc( 0x0D );	/* CRを一文字送信 */

	while( !( U0LSR & (1<<5)) )
		;		/* UART0 LSRのTHREが1になるまで待つ。1ならば送信レジスタ空き。*/

	U0THR = c;	/* 一文字送信 */
}


/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 *  VICVectAddress[]は同名のレジスタ列にマップされている。したがって、
 *  define_inhを呼ぶと、VICVectAddressXレジスタが初期化される。
 */
void
define_inh(INHNO inhno, FP inthdr)
{
    assert(inhno <= MAX_INT_NUM);
    VICVectAddress[inhno] = (volatile FP*)inthdr;

}

/*
 *  未定義の割込みが入った場合の処理
 */
void
undef_interrupt(){
    syslog(LOG_EMERG, "Unregistered Interrupt occurs.");
    while(1);
}


/*
 * 割り込みのイネーブル、ディセーブル
 */
/*
 * intnoであらわされる割り込みがディセーブルになる。
 */
ER dis_int(INTNO intno)
{
	if ( intno > MAX_INT_NUM )
		return E_PAR;
	VICIntEnClr = 1<<intno;
	return 0;
}

/*
 * intnoであらわされる割り込みがイネーブルになる。
 */
ER ena_int(INTNO intno)
{
	if ( intno > MAX_INT_NUM )
		return E_PAR;
	VICIntEnable = 1<<intno;
	return 0;
}

/*
 * 割り込みイネーブルマスクを指定する。マスクはVICIntEnableに代入される
 */
ER chg_ims(unsigned int enableMask)
{
	VICIntEnClr = 0xFFFFFFFF;
	VICIntEnable = enableMask;
	return 0;
}

/*
 * 割り込みイネーブルマスクを取得。
 */
ER get_ims(unsigned int *p_enableMask)
{
	*p_enableMask = VICIntEnable;
	return 0;
}


