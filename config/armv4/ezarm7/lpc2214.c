/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005- by Monami software, LP.
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
 *    LPC2214 内部機能用ドライバ
 */
#include <s_services.h>
#include <lpc2214.h>

/*
 * 内部UART用 簡易SIOドライバ
 */

/*
 *  シリアルI/Oポート初期化ブロック
 */
typedef struct sio_port_initialization_block {

} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロック
 */
struct sio_port_control_block {
    const SIOPINIB  *siopinib; /* シリアルI/Oポート初期化ブロック */
    VP_INT          exinf;     /* 拡張情報 */
    BOOL    openflag;          /* オープン済みフラグ */
    BOOL    sendflag;          /* 送信割込みイネーブルフラグ */
    BOOL    getready;          /* 文字を受信した状態 */
    BOOL    putready;          /* 文字を送信できる状態 */
};

/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {{}};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB  siopcb_table[TNUM_SIOP];

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)  ((UINT)((siopid) - 1))
#define get_siopcb(siopid)  (&(siopcb_table[INDEX_SIOP(siopid)]))


/*
 * 文字を受信したか?
 */ 
Inline BOOL
uart_getready(SIOPCB *siopcb)
{
    return (sil_rew_mem(UART_LSR) & 0x01) != 0;
}

/*
 * 文字を送信できるか?
 */
Inline BOOL
uart_putready(SIOPCB *siopcb)
{
    return (sil_rew_mem(UART_LSR) & 0x20) != 0;
}

/*
 *  受信した文字の取り出し
 */
Inline UB
uart_getchar(SIOPCB *siopcb)
{
    return (UB)sil_rew_mem(UART_RBR);  
}

/*
 *  送信する文字の書き込み
 */
void
uart_putchar(SIOPCB *siopcb, UB c)
{
    sil_wrw_mem(UART_THR, (VW)c);
}

/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *siopcb)
{
    VW wbits;
    wbits = sil_rew_mem(UART_IER);
    wbits |= UART_INT_T;
    sil_wrw_mem(UART_IER,wbits);
}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *siopcb)
{
    VW wbits;
    wbits = sil_rew_mem(UART_IER);
    wbits &= ~UART_INT_T;
    sil_wrw_mem(UART_IER,wbits);
}

/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *siopcb)
{
    VW wbits;
    wbits = sil_rew_mem(UART_IER);
    wbits |= UART_INT_R;
    sil_wrw_mem(UART_IER,wbits);
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *siopcb)
{
    VW wbits;
    wbits = sil_rew_mem(UART_IER);
    wbits &= ~UART_INT_R;
    sil_wrw_mem(UART_IER,wbits);
}



/*
 * カーネル起動時のログ出力用の初期化
 */
void
uart_init(void){
    UW wbits;
    UH devisor;

#if 0
    /*
     *  GP34～41 を FFUART として利用
     */
    wbits = sil_rew_mem(GPIO_GAFR0_y);
    wbits = (wbits & 0xfff0000f) | 0x000a9550;
    sil_wrw_mem(GPIO_GAFR0_y, wbits);

    /*
     *  TXD, RTS/RTS を出力にする
     */
    wbits = sil_rew_mem(GPIO_GPDR_y);
    wbits = (wbits & 0xfffffc7f) | 0x00000380;
    sil_wrw_mem(GPIO_GPDR_y, wbits);
    
    /*
     * RS232C ドライバハードウェアのイネーブル
     */
    wbits = sil_rew_mem(CQ_PXA250_CONFIG);
    wbits |= 0x200;
    sil_wrw_mem(CQ_PXA250_CONFIG, wbits);
#endif

    /*
     *  UART_BPS が14以下のとき、
     *  演算結果がオーバフローする。
     *  実際にそんな数値にすることは
     *  ありえないが念のため。
     */
#if (UART_BPS < 15)
#error UART_BPS is too small or undefined.
#endif

    devisor = 921600 / UART_BPS;

    sil_wrw_mem(UART_LCR, 0x80);
    sil_wrw_mem(UART_DLL, devisor & 0xff);
    sil_wrw_mem(UART_DLH, (devisor >> 8) & 0xff);
    sil_wrw_mem(UART_LCR, 0x3);
    sil_wrw_mem(UART_FCR, 0x1);
    sil_wrw_mem(UART_MCR, 0x3);
    sil_wrw_mem(UART_IER, 0x40);
}


/*
 *  オンチップのUARTからのポーリング出力
 */
void
uart_putc(char c)
{
    UW buf_level;
    
    while((sil_rew_mem(UART_LSR) & 0x20) == 0) {
	/* spin lock */
    }
    
    sil_wrw_mem(UART_THR,(VW)c);
}

/*
 *  SIOドライバの初期化ルーチン
 *  1ポートしかないため，あまり意味はない
 */
void
uart_initialize()
{
    SIOPCB  *siopcb;
    UINT    i;

    /*
     *  シリアルI/Oポート管理ブロックの初期化
     */
    for (siopcb = siopcb_table, i = 0; i < TNUM_SIOP; siopcb++, i++) {
        siopcb->siopinib = &(siopinib_table[i]);
        siopcb->openflag = FALSE;
        siopcb->sendflag = FALSE;
    }
}

/*
 * オープンしているポートがあるか
 */
BOOL
uart_openflag(void)
{
    return(siopcb_table[0].openflag);
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

    uart_init();
    
    siopcb->exinf = exinf;
    siopcb->getready = siopcb->putready = FALSE;
    siopcb->openflag = TRUE;

    /* clear FIFO  TxThr = 0 RxThr = 1*/
    sil_wrw_mem(UART_FCR,(VW)(UART_FCR_TC | UART_FCR_RC));
  
    /* set interrupt */
    sil_wrw_mem(UART_IER, 0x42);

    return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
uart_cls_por(SIOPCB *siopcb)
{
    siopcb->openflag = FALSE;
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
uart_isr_siop(SIOPCB *siopcb)
{
    if (uart_getready(siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        uart_ierdy_rcv(siopcb->exinf);
    }
    if (uart_putready(siopcb)) {
        /*
         *  送信可能コールバックルーチンを呼び出す．
         */
        uart_ierdy_snd(siopcb->exinf);
    }
}


/*
 *  SIOの割込みサービスルーチン
 */
void
uart_isr()
{
	sil_wrw_mem((VP *)0xe002800c, 0x40000000);

    uart_isr_siop(&(siopcb_table[0]));
}

