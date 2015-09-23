/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: hw_serial.h,v 1.1 2001/11/12 13:38:30 abe Exp $
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（H8用）
 *  SCI1を使用
 *  　　共通部のモジュールによって、XON/XOFFフロー制御が行わ
 *  　　れているので、信号線はTxDとRxDの2本でよい
 */

#include "sys_config.h"
#include "cpu_config.h"

#ifndef _MACRO_ONLY

/*
 *  シリアルポートのハードウェア依存情報の定義
 */

typedef struct hardware_serial_port_descripter {
	UW	base;	/* SCI のベースアドレス		*/
	UW	enaix;	/* SCI の送信割込み状態の索引	*/
} HWPORT;

#endif	/* of #ifndef _MACRO_ONLY */

/*
 *  低レベルポート情報管理ブロックの初期値
 */

#define NUM_PORT	1	/* サポートするシリアルポートの数 */
#define PORT_ID1	1	/* SCI1 */

#define HWPORT1	{ SYSTEM_SCI, 0 }

#ifndef _MACRO_ONLY

/*
 *  シリアルポートの初期化
 */

Inline void
SCI_initialize(UW base)
{
    int i;
					/* 送受信停止		*/
    outb(base + H8SCR, inb(base + H8SCR) & ~(H8SCR_TE | H8SCR_RE));

    					/* 送受信フォーマット	*/
         		/* 調歩同期式				*/
         		/* キャラクタレングス：8ビット		*/
         		/* パリティなし				*/
         		/* ストップビットレングス：1		*/
         		/* クロックセレクト（分周比）		*/
    outb(base + H8SMR, 0);

    outb(base + H8BRR, H8BRR_RATE);	/*  ボーレート設定  	*/

    			/* 割込み禁止とクロックソースの選択	*/
    outb(base + H8SCR,
     inb(base + H8SCR) & ~(H8SCR_TIE  | H8SCR_RIE  |
                           H8SCR_MPIE | H8SCR_TEIE |
                           H8SCR_CKE1 | H8SCR_CKE0 ));

    /* ボーレートの設定後、1ビット分待たなければならない。*/

    for(i = SCI_SETUP_COUNT; i -- > 0; )
    	;

    				/* エラーフラグをクリア		*/
    outb(base + H8SSR,
     inb(base + H8SSR) & ~(H8SSR_ORER | H8SSR_FER | H8SSR_PER));

    /* 受信割り込み許可, 送信割込みの許可は送信制御関数で行う	*/
    /* 送受信許可 */

    outb(base + H8SCR,
     inb(base + H8SCR) | (H8SCR_RIE | H8SCR_TE | H8SCR_RE));
}

/*
 * serial_open、serial_close [systask/serial.c] から呼ばれる
 * 関数では何もしない。
 * sys_initialize [config/h8/akih8_3048f/sys_config.c] で行う。
 */

Inline BOOL
hw_port_initialize(HWPORT *p)
{
	return (FALSE);
}

Inline void
hw_port_terminate(HWPORT *p)
{
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
hw_serial_handler_in()
{
	serial_handler_in(PORT_ID1);

    	/* エラーフラグをクリア		*/
	outb(H8SCI1 + H8SSR,
	 inb(H8SCI1 + H8SSR) & ~(H8SSR_ORER | H8SSR_FER | H8SSR_PER));

}

Inline void
hw_serial_handler_out()
{
	serial_handler_out(PORT_ID1);
}

/*
 *  文字を受信したか？
 */

Inline BOOL
hw_port_getready(HWPORT *p)
{
	/*  レシーブデータレジスタフル・フラグのチェック  */
	return(inb(p->base + H8SSR) & H8SSR_RDRF);
}

/*
 *  文字を送信できるか？
 */

Inline BOOL
hw_port_putready(HWPORT *p)
{
	/*  トランスミットデータレジスタエンプティ・フラグのチェック*/

	return(inb(p->base + H8SSR) & H8SSR_TDRE);
}

/*
 *  受信した文字の取り出し
 */

Inline UB
hw_port_getchar(HWPORT *p)
{
	UB data;
	UW addr = p->base + H8SSR;

	data = inb(p->base + H8RDR);

 	/*  レシーブデータレジスタフル・フラグのクリア  */

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(H8SSR_RDRF_BIT));
#undef  BITCLR

	return data;
}

/*
 *  送信する文字の書き込み
 */

Inline void
hw_port_putchar(HWPORT *p, UB c)
{
	UW addr = p->base + H8SSR;

	outb(p->base + H8TDR, c);

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(H8SSR_TDRE_BIT));
#undef  BITCLR
}

/*
 * 直接出力 (待ちあり)
 */

Inline void
SCI_putchar (int base, int c)
{
	UW addr = base + H8SSR;

	/* TDRE が 1 になるまで待つ */
	while ((inb(addr) & H8SSR_TDRE) == 0)
		;
	outb(base + H8TDR, c);

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(H8SSR_TDRE_BIT));
#undef  BITCLR

	}

/*
 *  送信制御関数
 */

Inline void
hw_port_sendstart(HWPORT *p)
{
	UW addr = p->base + H8SCR;

#define BITSET(bit)	Asm("bset #" bit ", @%0" : : "r"(addr))
	BITSET(_TO_STRING(H8SCR_TIE_BIT));
#undef  BITSET
}

Inline void
hw_port_sendstop(HWPORT *p)
{
	UW addr = p->base + H8SCR;

#define BITCLR(bit)	Asm("bclr #" bit ", @%0" : : "r"(addr))
	BITCLR(_TO_STRING(H8SCR_TIE_BIT));
#undef  BITCLR
}

#endif	/* of #ifndef _MACRO_ONLY */

#endif /* _HW_SERIAL_H_ */
