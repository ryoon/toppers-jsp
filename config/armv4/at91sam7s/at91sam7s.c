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
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id$
 *  
 */

/*
 * At91sam7s 内部機能用ドライバ
 */
#include <s_services.h>
#include <at91sam7s.h>

#define BAUDRATE   115200

/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
    {(VP)(TADR_DBGU_BASE),         (VP)(TADR_PIO_BASE+TOFF_PIO_PDR),
     (VP)0,
     (VW)((1<<9)|(1<<10)),         0
    }
#if TNUM_SIOP >= 2
    ,
    {(VP)(TADR_US_BASE+US_WINDOW), (VP)(TADR_PIO_BASE+TOFF_PIO_PDR),
     (VP)(TADR_PMC_BASE+TOFF_PMC_PCER),
     (VW)((1<<21)|(1<<22)),        1<<IRQ_US1_PID
    }

#endif /* TNUM_SIOP >= 2 */
};


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
    return(((sil_rew_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_CSR)) & US_RXRDY)!=0));
}

/*
 * 文字を送信できるか?
 */
Inline BOOL
uart_putready(SIOPCB *siopcb)
{
    return((sil_rew_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_CSR)) & US_TXEMPTY)!=0);
}

/*
 *  受信した文字の取り出し
 */
Inline UB
uart_getchar(SIOPCB *siopcb)
{
    return(sil_rew_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_RHR)));
}

/*
 *  送信する文字の書き込み
 */
Inline void
uart_putchar(SIOPCB *siopcb, UB c)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_THR),c);
}

/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_IER), 1<<1);
}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_IDR), 1<<1);
}


/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_IER), 1<<0);
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_IDR), 1<<0);
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
    unsigned int baud = ((MCK * 10) / (BAUDRATE * 16));

    SIOPCB      *siopcb;
    const SIOPINIB  *siopinib;

    siopcb = get_siopcb(siopid);
    siopinib = siopcb->siopinib;

    /*
     * Wait を入れる．
     */
    sil_dly_nse(5000);

    if(siopinib->pmc_pcer)
        sil_wrw_mem((VP)siopinib->pmc_pcer, siopinib->irq_bit);
    /* Disable Interrupt */
    sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_IDR), 0xFFFFFFFF);
    sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_CR), (US_RSTRX|US_RSTTX|US_RXDIS|US_TXDIS));
    sil_wrw_mem((VP)(siopinib->uart_base+TOFF_PDC_RCR), 0);
    sil_wrw_mem((VP)(siopinib->uart_base+TOFF_PDC_TCR), 0);
    sil_wrw_mem((VP)siopinib->linectrl_pdr, siopinib->pdr_bit);
    /* 8Data, 1Stop, No Parity */
    sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_MR), (US_CLKS_MCK|US_CHRL_8|US_PAR_NO|US_NBSTOP_1|US_CHMODE_NORMAL));
    /* 115200bps */
    if((baud % 10) >= 5)
        sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_BRGR), (baud/10)+1);
    else
        sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_BRGR), (baud/10));

    /*
     * 割込み関連の設定
     */
    if(siopinib->pmc_pcer){
        sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_TTGR), 0);
        sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_CR), US_TXEN|US_RXEN);
        sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_IER), 1<<0);
    }
	else{
        /* クロックイネーブル */
        sil_wrw_mem((VP)(TADR_PMC_BASE+TOFF_PMC_PCER), (1<<IRQ_TC2_PID));
        /* タイマ停止 */
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_CCR), TC_CLKDIS);
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_IDR), 0xFFFFFFFF);
        /* カウント値をセット */
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_CMR), TC_CLKS_MCK8);	/* 47,923,200Hz/8=5,990,400Hz */
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_CMR), sil_rew_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_CMR)) | TC_WAVESEL10);
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_RC), 59900);			/* 10ms */
        /* 割込みのクリア  */
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_IER), TC_CPCS);
        /* カウントスタート   */
        sil_wrw_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_CCR), (TC_CLKEN|TC_SWTRG));

        sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_CR), US_TXEN|US_RXEN);
    }
    sil_rew_mem((VP)(siopinib->uart_base+TOFF_US_RHR));

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
    const SIOPINIB  *siopinib;

    siopinib = siopcb->siopinib;
	sil_wrw_mem((VP)(siopinib->uart_base+TOFF_US_CR), US_RSTRX|US_RSTTX|US_RXDIS|US_TXDIS);
    siopcb->openflag = FALSE;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
uart_snd_chr(SIOPCB *siopcb, char c)
{
    if(siopcb->siopinib->pmc_pcer == 0){
        while(!uart_putready(siopcb));
        uart_putchar(siopcb, c);
        return(TRUE);
    }
    else if (uart_putready(siopcb)){
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
int_timer2()
{
    SIOPCB *siopcb = &(siopcb_table[0]);

    sil_rew_mem((VP)(TADR_TC_BASE+TC_WINDOW*2+TOFF_TC_SR));
    if (uart_getready(siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        uart_ierdy_rcv(siopcb->exinf);
    }
}

#if TNUM_SIOP >= 2
void
uart_isr1()
{
    uart_isr_siop(&(siopcb_table[1]));
}
#endif
