/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *   CQ-FRK-NXP-ARM UART用 簡易SIOドライバ
 *
 *   このファイルは、TOPPERS/JSP 1.4.2の pdic/simple_sio/st16c2550.cを
 *   リネームし、内部の識別子を変更したものである。
 */
#include <s_services.h>
#include "lpc2388uart.h"

/*
 * 8250, 16450, 16550各レジスタのアドレス・オフセット。UART_RHRを基点とする。
 * システムごとに異なるのでUART_BOUNDARYによってレジスタの配置境界を指定する。
 * たとえばすべてのレジスタが8ビット境界で配置されているなら、UART_BOUNDARYは1,
 * 16bit境界なら2, 32ビット境界なら4を指定する。
 * UART_BOUNDARYはsys_config.hで定義する。
 */


#define UART_RBR   (0x00 * UART_BOUNDARY)   /*  受信バッファレジスタ  */
#define UART_THR   (0x00 * UART_BOUNDARY)   /*  送信データ・ホールド・レジスタ */
#define UART_DLL   (0x00 * UART_BOUNDARY)   /*  分周レジスタ下位バイト       */
#define UART_IER   (0x01 * UART_BOUNDARY)   /*  割込みイネーブルレジスタ       */
#define UART_DLM   (0x01 * UART_BOUNDARY)   /*  分周レジスタ上位バイト       */
#define UART_IIR   (0x02 * UART_BOUNDARY)   /*  割込み識別レジスタ  */
#define UART_FCR   (0x02 * UART_BOUNDARY)   /*  FIFO制御レジスタ  */
#define UART_LCR   (0x03 * UART_BOUNDARY)   /*  ライン制御レジスタ */
#define UART_MCR   (0x04 * UART_BOUNDARY)   /*  モデム制御レジスタ */
#define UART_LSR   (0x05 * UART_BOUNDARY)   /*  ライン・ステータス・レジスタ */
#define UART_MSR   (0x06 * UART_BOUNDARY)   /*  モデム・ステータス・レジスタ */
#define UART_SCR   (0x07 * UART_BOUNDARY)   /*  スクラッチ・パッド・レジスタ */
#define UART_GCTL  (0x09 * UART_BOUNDARY)	  /*  Blackfinのみ。グローバル制御レジスタ */

/*
 * レジスタのビットマスク。特殊機能を追加しない限り、書き換えなくてよい。
 */

#define ISR_TX            0x02       /* 送信割り込み発生 */
#define IER_TX            0x02       /* 送信割り込み許可 */
#define ISR_RX            0x01       /* 受信割り込み発生 */
#define IER_RX            0x01       /* 受信割り込み許可 */

#define LCR_DL_MODE       0x80       /* Divisor Enable */
#define LCR_NP_8_1        0x03       /* 8bit,1stop,Noparity,No break */
#define FCR_FIFO_DISABLE  0x00

#define LSR_RX_DATA_READY 0x01
#define LSR_TX_EMPTY      0x20

#define MCR_INT_ENABLE    0x08

#define GCTL_UCEN		  0x01		/* Blackfinのみ */


/*
 * TNUM_SIOP_UART, UART0_ADDRESS, UART0_DIVISOR はsys_config.hで定義する。
 *	TNUM_SIOP_UART : PDICが管理するUARTの個数
 *  以下、PDICが管理する最初のUARTの情報
 *  	UART0_ADDRESS : 		THRのアドレス
 *  	UART0_DIVISOR : 		DLM, DLLの初期値(16bit表記)
 *      UART0_14550_FIFO : 		14550互換のFIFOを持つときにマクロを宣言する
 *		UART0_BLACKFIN_UCEN : 	Blackfin専用。マクロを宣言すると初期化コードがUART_GCTLのUCENを1にする。
 *	二つ目以降のUARTがあるなら以下のように続ける。
 *  	UART1_ADDRESS : 		THRのアドレス
 *  	UART1_DIVISOR : 		DLM, DLLの初期値(16bit表記)
 *      UART1_14550_FIFO : 		14550互換のFIFOを持つときにマクロを宣言する
 *		UART1_BLACKFIN_UCEN : 	Blackfin専用。マクロを宣言すると初期化コードがUART_GCTLのUCENを1にする。
 *	...
 */


/******************************************************************************************
 * シリアルI/Oポート初期化ブロック
 */
#if TNUM_SIOP_UART == 0 
#error "If you don't use UART, please remove this file from your make file" 
#endif

#if TNUM_SIOP_UART > 3
#error "Only TNUM_SIOP_UART < 4 is supported" 
#endif /* TNUM_SIOP_UART >= 2 */    

/* ディバイザ計算マクロ */
#define DLM(divisor) (divisor/256)
#define DLL(divisor) (divisor%256)

const SIOPINIB siopinib_table[TNUM_SIOP_UART] = {
/*-----------------------------------------------------------------
 *			PDICが管理する最初のUARTの初期化パラメータ 
 */
    {UART0_ADDRESS,  DLM(UART0_DIVISOR), DLL(UART0_DIVISOR), 
#ifdef UART0_BLACKFIN_UCEN
		1	/* マクロUART0_BLACKFIN_UCENが定義されていたら、GCTLのUCENを1にしてクロックを動かす */
#else
		0
#endif
	}

/*-----------------------------------------------------------------
 *			PDICが管理する2番目のUARTの初期化パラメータ 
 */

#if TNUM_SIOP_UART > 1
    ,{UART1_ADDRESS,    DLM(UART1_DIVISOR), DLL(UART1_DIVISOR),	
#ifdef UART1_BLACKFIN_UCEN
		1	/* マクロUART1_BLACKFIN_UCENが定義されていたら、GCTLのUCENを1にしてクロックを動かす */
#else
		0
#endif


	}
#endif /* if TNUM_SIOP_UART > 1*/

/*-----------------------------------------------------------------
 *			PDICが管理する3番目のUARTの初期化パラメータ 
 */
#if TNUM_SIOP_UART > 2
    ,{UART2_ADDRESS,    DLM(UART2_DIVISOR), DLL(UART2_DIVISOR),
#ifdef UART2_BLACKFIN_UCEN
		1	/* マクロUART2_BLACKFIN_UCENが定義されていたら、GCTLのUCENを1にしてクロックを動かす */
#else
		0
#endif


	}
#endif /* if TNUM_SIOP_UART > 2*/

};

/*
 * シリアルI/Oポート初期化ブロックここまで。
 ******************************************************************************************/



/*
 *  シリアルI/Oポート初期化ブロックの取出し
 */
#define INDEX_SIOPINIB(siopid)  ((UINT)((siopid) - 1))
#define get_siopinib(siopid)  (&(siopinib_table[INDEX_SIOPINIB(siopid)]))


/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB  siopcb_table[TNUM_SIOP_UART];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)  ((UINT)((siopid) - 1))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))

Inline void
uart_write(UW addr, UW offset, UB val)
{
#ifdef UART_IOP_ACCESS
    sil_wrb_iop((VP)(addr + offset),val);
#else
    sil_wrb_mem((VP)(addr + offset),val);	
#endif
}

Inline UB
uart_read(UW addr, UW offset)
{
#ifdef UART_IOP_ACCESS
    return(sil_reb_iop((VP)(addr + offset)));
#else
    return(sil_reb_mem((VP)(addr + offset)));
#endif
}


/*
 * 文字を受信したか?
 */ 
Inline BOOL
uart_getready(SIOPCB *siopcb)
{
    UH status;

    status = uart_read(siopcb->siopinib->reg_base, UART_LSR);
    
    return((status & LSR_RX_DATA_READY));
}

/*
 * 文字を送信できるか?
 */
Inline BOOL
uart_putready(SIOPCB *siopcb)
{
    UH status;

    status = uart_read(siopcb->siopinib->reg_base, UART_LSR);
    
    return (status & LSR_TX_EMPTY);
}

/*
 *  受信した文字の取り出し
 */
Inline UB
uart_getchar(SIOPCB *siopcb)
{
    return(uart_read(siopcb->siopinib->reg_base,UART_RBR));
}

/*
 *  送信する文字の書き込み
 */
Inline void
uart_putchar(SIOPCB *siopcb, UB c)
{
    uart_write(siopcb->siopinib->reg_base, UART_THR, c);
}

/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *siopcb)
{
    uart_write(siopcb->siopinib->reg_base, UART_IER,
                (uart_read(siopcb->siopinib->reg_base,UART_IER) | IER_TX));
}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *siopcb)
{
    uart_write(siopcb->siopinib->reg_base, UART_IER,
                (uart_read(siopcb->siopinib->reg_base, UART_IER) & ~IER_TX));
                
}

/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *siopcb)
{
    uart_write(siopcb->siopinib->reg_base, UART_IER,
                (uart_read(siopcb->siopinib->reg_base,UART_IER) | IER_RX));
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *siopcb)
{
    uart_write(siopcb->siopinib->reg_base, UART_IER,
                (uart_read(siopcb->siopinib->reg_base, UART_IER) & ~IER_RX));
                
}


/*
 *  SIOドライバの初期化ルーチン
 */
void
uart_initialize()
{
    SIOPCB  *siopcb;
    UINT    i;

    /*
     *  シリアルI/Oポート管理ブロックの初期化
     */
    for (siopcb = siopcb_table, i = 0; i < TNUM_SIOP_UART; siopcb++, i++) {
        siopcb->siopinib = &(siopinib_table[i]);
        siopcb->openflag = FALSE;
        siopcb->sendflag = FALSE;
    }
}

    
/*
 *
 */
void
uart_init_siopinib(const SIOPINIB *siopinib)
{

	/*
	 *  Blackfin ADSP-BF531/2/3, BF534/6/7, BF561の内蔵UARTに固有の作業。
	 *  Power Downモードの解除。
	 */
	if ( siopinib->set_ucen )
		uart_write(siopinib->reg_base, UART_GCTL, 
			   (uart_read(siopinib->reg_base, UART_GCTL) | GCTL_UCEN ));
    /*
     *  分周比の設定
     */
    /* Divisor Enable */
    uart_write(siopinib->reg_base, UART_LCR,
                (uart_read(siopinib->reg_base, UART_LCR) | LCR_DL_MODE));
    uart_write(siopinib->reg_base, UART_DLL, siopinib->dll_val);
    uart_write(siopinib->reg_base, UART_DLM, siopinib->dlm_val);
    /* Divisor Disable */
    uart_write(siopinib->reg_base, UART_LCR,
                (uart_read(siopinib->reg_base, UART_LCR) & ~LCR_DL_MODE));

    /* モード設定, パリティ無し 8bit data, 1 stop bit */
    uart_write(siopinib->reg_base, UART_LCR, LCR_NP_8_1);

    /* 割込み禁止 */
    uart_write(siopinib->reg_base, UART_IER, 0x00);
}


/*
 *  カーネル起動時のバナー出力用の初期化
 */
void 
uart_init(void)
{
    int i;
	/*
	 * すべてのuartを初期化する。id は1からTNUM_SIOP_UARTまで。
	 */
	for ( i=1; i<=TNUM_SIOP_UART; i++ )
	    uart_init_siopinib(get_siopinib(i));
}

/*
 * オープンしているポートがあるか
 */
BOOL
uart_openflag(void)
{
	int i;
	
	/*
	 * 開いているポートがあれば0を返す。なければ0を返す。
	 */
	for ( i=0; i<TNUM_SIOP_UART; i++ )
		if (siopcb_table[0].openflag)
			return( 1 );
	return( 0 );
}


/*
 * シリアルI/Oポートのオープン
 */
SIOPCB *
uart_opn_por(ID siopid, VP_INT exinf)
{
    SIOPCB      *siopcb;
    const SIOPINIB  *siopinib;

    siopcb = get_siopcb(siopid);
    siopinib = siopcb->siopinib;
    
    /*
     * 初期化
     */
    uart_init_siopinib(siopcb->siopinib);
    
    /* 受信割込み許可 */
    uart_write(siopcb->siopinib->reg_base, UART_IER, IER_RX);
    
    /* 割込み線をイネーブル */
    uart_write(siopcb->siopinib->reg_base, UART_MCR, MCR_INT_ENABLE);

    siopcb->exinf = exinf;
    siopcb->getready = siopcb->putready = FALSE;
    siopcb->openflag = TRUE;

    return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
uart_cls_por(SIOPCB *siopcb)
{
    /* 割込み禁止 */
    uart_write(siopcb->siopinib->reg_base, UART_IER, 0x00);   
    siopcb->openflag = FALSE;
}


/*
 *  シリアルI/Oポートへのポーリングでの出力
 */
void
uart_pol_putc(char c, ID siopid)
{
    const SIOPINIB *siopinib;
    
    siopinib = get_siopinib(siopid);
    
    while((uart_read(siopinib->reg_base, UART_LSR) & LSR_TX_EMPTY)
          != LSR_TX_EMPTY)
        ;
    uart_write(siopinib->reg_base, UART_THR, c);
}


/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
uart_snd_chr(SIOPCB *siopcb, char c)
{
    if (uart_putready(siopcb)){
        uart_putchar(siopcb, c);
        return(TRUE);
    }
    return(FALSE);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
INT
uart_rcv_chr(SIOPCB *siopcb)
{
    if (uart_getready(siopcb)) {
        return((INT)(UB) uart_getchar(siopcb));
    }
    return(-1);
}





/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
uart_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{

    switch (cbrtn) {
        case SIO_ERDY_SND:
            uart_enable_send(siopcb);
            break;
        case SIO_ERDY_RCV:
            uart_enable_rcv(siopcb);
            break;
    }
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
uart_dis_cbr(SIOPCB *siopcb, UINT cbrtn)
{
    switch (cbrtn) {
        case SIO_ERDY_SND:
            uart_disable_send(siopcb);
            break;
        case SIO_ERDY_RCV:
            uart_disable_rcv(siopcb);
            break;
    }
}

/*
 *  シリアルI/Oポートに対する割込み処理
 */
static void
uart_rx_isr_siop(SIOPCB *siopcb)
{
    if (uart_getready(siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        uart_ierdy_rcv(siopcb->exinf);
    }
}

static void
uart_tx_isr_siop(SIOPCB *siopcb)
{
    if (uart_putready(siopcb)) {
        /*
         *  送信可能コールバックルーチンを呼び出す．
         */
        uart_ierdy_snd(siopcb->exinf);
    }
}

/*
 *  SIOの割込みサービスルーチン
 *	ADSP-BF533の内蔵UARTは送受に別々の割込み要求を出すため、
 *  ISRも分けている。また、ADSP-BF537のように二つ以上の
 *  UARTを持つ場合を考慮して、uart0のようにポート番号を
 *  名前にハードコードしている。
 *  ただし、このポート番号とは、SIOのポート番号であって、
 *  ハードウェアのUART番号と同じだとは限らない。
 */

/*
 ***** UART0
 */
void
uart0_rx_isr()
{
		/* 下位の受信処理を呼ぶ */
    uart_rx_isr_siop(&(siopcb_table[0]));
}

void
uart0_tx_isr()
{
		/* 下位の送信処理を呼ぶ */
    uart_tx_isr_siop(&(siopcb_table[0]));
}

/*
 ***** UART1
 */
void
uart1_rx_isr()
{
		/* 下位の受信処理を呼ぶ */
    uart_rx_isr_siop(&(siopcb_table[1]));
}

void
uart1_tx_isr()
{
		/* 下位の送信処理を呼ぶ */
    uart_tx_isr_siop(&(siopcb_table[1]));
}

/*
 ***** UART2
 */
void
uart2_rx_isr()
{
		/* 下位の受信処理を呼ぶ */
    uart_rx_isr_siop(&(siopcb_table[2]));
}

void
uart2_tx_isr()
{
		/* 下位の送信処理を呼ぶ */
    uart_tx_isr_siop(&(siopcb_table[2]));
}





/*
 * UARTの送受で割込みを共有する場合（こちらのほうが一般的）は、
 * 上のルーチンではなくこちらを割込みサービスルーチンとして使う。
 */
void
uart0_isr()
{
		/* 下位の受信処理を呼ぶ */
    uart_rx_isr_siop(&(siopcb_table[0]));
		/* 下位の送信処理を呼ぶ */
    uart_tx_isr_siop(&(siopcb_table[0]));
}

void
uart1_isr()
{
		/* 下位の受信処理を呼ぶ */
    uart_rx_isr_siop(&(siopcb_table[1]));
		/* 下位の送信処理を呼ぶ */
    uart_tx_isr_siop(&(siopcb_table[1]));
}

void
uart2_isr()
{
		/* 下位の受信処理を呼ぶ */
    uart_rx_isr_siop(&(siopcb_table[2]));
		/* 下位の送信処理を呼ぶ */
    uart_tx_isr_siop(&(siopcb_table[2]));
}
