/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2006 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2006 by Logic Research Co., Ltd.
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
 * FRK_ADuC 内部機能用ドライバ
 */

#include <s_services.h>
#include "frk_aduc.h"


/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{(VP)COMRX, (VP)COMDIV0, (VP)COMDIV1,
	 (VP)COMIEN0, (VP)COMIID0, (VP)COMCON0,
	 (VP)COMSTA0, IRQ_UART}
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
	return(((sil_rew_mem((VP)(siopcb->siopinib->line_status)) & COMSTA0_DR)!=0));
}


/*
 * 文字を送信できるか?
 */
Inline BOOL
uart_putready(SIOPCB *siopcb)
{
	return((sil_rew_mem((VP)(siopcb->siopinib->line_status)) & COMSTA0_TEMT)!=0);
}


/*
 *  受信した文字の取り出し
 */
Inline UB
uart_getchar(SIOPCB *siopcb)
{
	return(sil_rew_mem((VP)(siopcb->siopinib->uart_data)));
}


/*
 *  送信する文字の書き込み
 */
Inline void
uart_putchar(SIOPCB *siopcb, UB c)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_data), c);
}


/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->int_enable),
            (sil_rew_mem((VP)(siopcb->siopinib->int_enable)) | COMIEN0_ETBEI));
}


/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->int_enable),
        (sil_rew_mem((VP)(siopcb->siopinib->int_enable)) & ~COMIEN0_ETBEI));
}


/*
 *  受信割込み許可
 */
Inline void
uart_enable_rcv(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->int_enable),
        (sil_rew_mem((VP)(siopcb->siopinib->int_enable)) | COMIEN0_ERBFI));
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->int_enable),
        (sil_rew_mem((VP)(siopcb->siopinib->int_enable)) & ~COMIEN0_ERBFI));
}



/*
 * カーネル起動時のログ出力用の初期化
 */
void
init_uart(void){
    /* Disable Interrupt */
    sil_wrw_mem((VP)COMIEN0, 0);
    /* 9600bps */
    sil_wrw_mem((VP)COMCON0, COMCON0_DLAB);
    sil_wrw_mem((VP)COMDIV0, COMDIV0_BR9600); 
    sil_wrw_mem((VP)COMDIV1, COMDIV1_BR9600);
    /* 8Data, 1Stop, No Parity */
    sil_wrw_mem((VP)COMCON0, (COMCON0_STOP|COMCON0_WLS_8b));
    /* Enabel Receive Interrupt */
    sil_wrw_mem((VP)COMIEN0, (COMIEN0_ERBFI|COMIEN0_ETBEI));      
}


/*
 *  オンチップのUARTからのポーリング出力
 */
void
uart_putc(char c)
{
    int i;

    while((sil_rew_mem((VP)COMSTA0) & COMSTA0_TEMT)!=COMSTA0_TEMT);
  
    sil_wrw_mem((VP)COMTX, (VW)c);
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
    int i;
    
    SIOPCB      *siopcb;
    const SIOPINIB  *siopinib;

    siopcb = get_siopcb(siopid);
    siopinib = siopcb->siopinib;

    /* Disable Interrupt */
    sil_wrw_mem((VP)siopinib->int_enable, 0);
    /* 9600bps */
    sil_wrw_mem((VP)siopinib->line_control, COMCON0_DLAB);
    sil_wrw_mem((VP)siopinib->divisor_lo, COMDIV0_BR9600); 
    sil_wrw_mem((VP)siopinib->divisor_hi, COMDIV1_BR9600);
    
    /* 8Data, 1Stop, No Parity */
    sil_wrw_mem((VP)siopinib->line_control, (COMCON0_STOP|COMCON0_WLS_8b));

    /*
     * 割込み関連の設定
     */
    sil_wrw_mem((VP)siopinib->int_enable, (COMIEN0_ERBFI|COMIEN0_ETBEI));	/* Enable Interrupt */
    sil_wrw_mem((VP)IRQEN, siopinib->irq_bit);	/*Enable interrupt register*/
    
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
    sil_wrw_mem((VP)(siopcb->siopinib->line_control),
        (sil_rew_mem((VP)(siopcb->siopinib->line_control))|COMCON0_BRK)); /* Disable Interrupt */
    
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
	volatile UW	iid;

    if (uart_getready(siopcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        uart_ierdy_rcv(siopcb->exinf);
    }
    if (uart_putready(siopcb)) {
		iid = sil_rew_mem((VP)COMIID0);
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
    uart_isr_siop(&(siopcb_table[0]));
}

