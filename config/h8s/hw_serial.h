/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: hw_serial.h,v 1.2 2002/04/13 13:14:45 imai Exp $
 */

#ifndef _HW_SERIAL_H_
#define _HW_SERIAL_H_

/*
 *  ターゲット依存シリアルI/Oモジュール（H8S用）
 *  SCI0を使用
 *  　　共通部のモジュールによって、XON/XOFFフロー制御が行わ
 *  　　れているので、信号線はTxDとRxDの2本でよい
 */

#include "sys_config.h"
#include "cpu_config.h"

#ifndef _MACRO_ONLY


/*  インライン関数展開の指定  */

#pragma inline(SCI_initialize, hw_port_initialize, hw_port_terminate)
#pragma inline(hw_serial_handler_in, hw_serial_handler_out, hw_port_getready)
#pragma inline(hw_port_putready, hw_port_getchar, hw_port_putchar, SCI_putchar)
#pragma inline(hw_port_sendstart, hw_port_sendstop)

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
#define PORT_ID1	1	/* プログラム中のPORT1は、SCI0を指している */

#define HWPORT1	{ SYSTEM_SCI, 0 }

#ifndef _MACRO_ONLY

/*
 *  シリアルポートの初期化
 */

/*  
 *  SCI_initialize()でのシリアルポートの初期化は、hw_port_initialize()内で設定
 *  すべきであると思うが、H8との統合を考慮し、現段階では、変更しない
 */

Inline void
SCI_initialize(UW base)
{
    int i;
    
		   /*  モジュールストップモード解除  */
    MSTPCR &= ~MSTPCR_SCI0;
    
					/* 送受信停止 */
    outb(base + H8SSCR, (UB)(inb(base + H8SSCR) & ~(H8SSCR_TE | H8SSCR_RE)));

    					/*   送受信フォーマット	*/
         		/*  調歩同期式				*/
         		/*  キャラクタレングス：8ビット		*/
         		/*  パリティなし			*/
         		/*  ストップビットレングス：1		*/
         		/*  クロックセレクト（分周比）:分周なし	*/
    outb(base + H8SSMR, 0);

    outb(base + H8SBRR, H8SBRR_RATE);	/*  ボーレート設定  	*/

    			/* 割込み禁止とクロックソースの選択	*/
    outb(base + H8SSCR,
     (UB)(inb(base + H8SSCR) & ~(H8SSCR_TIE  | H8SSCR_RIE  |
                           H8SSCR_MPIE | H8SSCR_TEIE |
                           H8SSCR_CKE1 | H8SSCR_CKE0 )));

    /* ボーレートの設定後、1ビット分待たなければならない。*/

    for(i = SCI_SETUP_COUNT; i -- > 0; )
    	;

    				/* エラーフラグをクリア		*/
    outb(base + H8SSSR,
     (UB)(inb(base + H8SSSR) & ~(H8SSSR_ORER | H8SSSR_FER | H8SSSR_PER)));

    /* 受信割り込み許可, 送信割込みの許可は送信制御関数で行う	*/
    /* 送受信許可 */

    outb(base + H8SSCR,
     (UB)(inb(base + H8SSCR) | (H8SSCR_RIE | H8SSCR_TE | H8SSCR_RE)));

    /*
     * 割り込み関連の設定
     * エラーハンドラも登録すべき
     */
        			/* 割り込みレベル設定 */
        outb(SYSTEM_SCI_IPR,(UB)((inb(SYSTEM_SCI_IPR)&~(0x7<<SCI_BIT_SHIFT)) | (SCI_INTLVL<<SCI_BIT_SHIFT)));
}

/*
 * serial_open、serial_close [systask/serial.c] から呼ばれる
 * 関数では何もしない。
 * sys_initialize [config/h8/akih8_3048f/sys_config.c] で行う。
 *    	->要変更
 */

Inline BOOL
hw_port_initialize(HWPORT *p)
{
	return (FALSE);
}

/*
 *  H8版では、hw_port_terminate()内は、何も設定していない。H8版とH8S版の統合
 *  の際は、注意が必要である
 */
Inline void
hw_port_terminate(HWPORT *p)
{
              						/*  送受信停止  */
	outb(p->base + H8SSCR, (UB)(inb(p->base + H8SSCR) & ~(H8SSCR_TE | H8SSCR_RE)));
							/*  割込みレベルクリア  */

 	outb(SYSTEM_SCI_IPR,(UB)((inb(SYSTEM_SCI_IPR)&~(0x7<<SCI_BIT_SHIFT)) | (LEVEL0<<SCI_BIT_SHIFT)));
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
hw_serial_handler_in(void)
{
	serial_handler_in(PORT_ID1);

    	/* エラーフラグをクリア		*/
	outb(H8SSCI0 + H8SSSR,(UB)(inb(H8SSCI0 + H8SSSR) & ~(H8SSSR_ORER | H8SSSR_FER | H8SSSR_PER)));

}

Inline void
hw_serial_handler_out(void)
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
	return(inb(p->base + H8SSSR) & H8SSSR_RDRF);
}

/*
 *  文字を送信できるか？
 */

Inline BOOL
hw_port_putready(HWPORT *p)
{
	/*  トランスミットデータレジスタエンプティ・フラグのチェック*/

	return(inb(p->base + H8SSSR) & H8SSSR_TDRE);
}

/*
 *  受信した文字の取り出し
 */

Inline UB
hw_port_getchar(HWPORT *p)
{
	UB data;
	UW addr = p->base + H8SSSR;

	data = inb(p->base + H8SRDR);

 	/*  シリアルステータスレジスタ(SSR）のレシーブデータレジスタフル(RDRF)のクリア  */
        bitclr(addr, H8SSSR_RDRF_BIT);

	return data;
}

/*
 *  送信する文字の書き込み
 */

Inline void
hw_port_putchar(HWPORT *p, char c)
{
	UW addr = p->base + H8SSSR;

	outb(p->base + H8STDR, c);
  		/*  シリアルステータスレジスタ(SSR）のトランスミットデータレジスタエンプティ(TDRE)のクリア  */
	bitclr(addr, H8SSSR_TDRE_BIT);
}

/*
 * 直接出力 (待ちあり)
 */

Inline void
SCI_putchar (int base, char c)		/*  int(16bits) -> UW(32bits)  */
{
	UW addr = base + H8SSSR;

	/* TDRE が 1 になるまで待つ */
	while ((inb(addr) & H8SSSR_TDRE) == 0)
		;
	outb(base + H8STDR, c);
		/*  シリアルステータスレジスタ(SSR）のトランスミットデータレジスタエンプティ(TDRE)のクリア  */
	bitclr(addr, H8SSSR_TDRE_BIT);
}

/*
 *  送信制御関数
 */

Inline void
hw_port_sendstart(HWPORT *p)
{
	UW addr = p->base + H8SSCR;
		/*  送信割込み要求を許可  */
	bitset(addr, H8SSCR_TIE_BIT);
}

Inline void
hw_port_sendstop(HWPORT *p)
{
	UW addr = p->base + H8SSCR;
		/*  送信割込み要求を禁止  */
	bitclr(addr, H8SSCR_TIE_BIT);
}


#endif	/* of #ifndef _MACRO_ONLY */

#endif /* _HW_SERIAL_H_ */
