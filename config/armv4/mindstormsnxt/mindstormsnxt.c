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
 *  Copyright (C) 2006-2007 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2009      by GJ Designing Center Print Business Group
 *                             RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id: mindstormsnxt.c,v 1.1 2008/08/11 19:32:22 roi Exp $
 *  
 */

/*
 * At91sam7s 内部機能用ドライバ
 */
#include <s_services.h>
#include <mindstormsnxt.h>

#define	TADR_US1_BASE   (TADR_US_BASE+US_WINDOW)
#define TADR_TC1_BASE   (TADR_TC_BASE+TC_WINDOW)
#define TADR_TC2_BASE   (TADR_TC_BASE+TC_WINDOW*2)

/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
    {(VP)(TADR_US1_BASE), (VP)(TADR_TC2_BASE)
    }
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

#define get_us1_rcvcnt1()   sil_rew_mem((VP)(TADR_US1_BASE+TOFF_PDC_RCR))
#define get_us1_rcvcnt2()   sil_rew_mem((VP)(TADR_US1_BASE+TOFF_PDC_RNCR))
#define set_us1_rcvcnt2(a)  sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RNCR), a)
#define set_us1_rcvptr2(a)  sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RNPR), a)


#define BT_RX_PIN        AT91S_PIO_PA21
#define BT_TX_PIN        AT91S_PIO_PA22
#define BT_SCK_PIN       AT91S_PIO_PA23
#define BT_RTS_PIN       AT91S_PIO_PA24
#define BT_CTS_PIN       AT91S_PIO_PA25
#define BT_CS_PIN        AT91S_PIO_PA31
#define BT_RST_PIN       AT91S_PIO_PA11
#define BT_ARM7_CMD_PIN  AT91S_PIO_PA27


#define MSG_RESET_INDICATION 20


#define BAUD_RATE 460800


/*
 * 文字を受信したか?
 */ 
Inline INT
uart_getready(SIOPCB *siopcb)
{
	int     bytes_ready;

	if(get_us1_rcvcnt2() == 0)
		bytes_ready = SIORBUFSIZE*2 - get_us1_rcvcnt1();
	else
		bytes_ready = SIORBUFSIZE - get_us1_rcvcnt1();
	return bytes_ready - siopcb->rcv_idx;
}

/*
 * 文字を送信できるか?
 */
Inline BOOL
uart_putready(SIOPCB *siopcb)
{
	return (sil_rew_mem((VP)(siopcb->siopinib->uart_base+TOFF_PDC_TNCR)) == 0
				&& sil_rew_mem((VP)(siopcb->siopinib->uart_base+TOFF_PDC_TCR)) == 0);
}

/*
 *  送信する文字の書き込み
 */
Inline void
uart_puts(SIOPCB *siopcb, UB *buf, H len)
{
	/*
	 *  DMAコントローラに，送信データのアドレスとサイズを知らせる
	 */
	sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_PDC_TNPR), (unsigned long)buf);
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_PDC_TNCR), len);
}

/*
 *  送信割込み許可
 */
Inline void
uart_enable_send(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_IER), US_TXBUFE);
}

/*
 *  送信割込み禁止
 */
Inline void
uart_disable_send(SIOPCB *siopcb)
{
	sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_IDR), US_TXBUFE);
}


/*
 *  受信割込み許可
 *  BLUETOOTH用受信DMAには適切な受信割込みがないため
 *  TC2のタイマによる10MSのポーリングを行う
 *  TC2の割込み許可設定
 */
Inline void
uart_enable_rcv(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->time_base+TOFF_TC_IER), TC_CPCS);
}

/*
 *  受信割込み禁止
 *  BLUETOOTH用受信DMAには適切な受信割込みがないため
 *  TC2のタイマによる10MSのポーリングを行う
 *  TC2の割込み禁止設定
 */
Inline void
uart_disable_rcv(SIOPCB *siopcb)
{
    sil_wrw_mem((VP)(siopcb->siopinib->time_base+TOFF_TC_IDR), TC_CPCS);
}

/*
 *  IDの領域の空き送信パケット番号を返す
 */
static INT
bluetooth_search_packet(SIOPCB *siopcb, UINT id)
{
	INT no;

	if(siopcb->snd_stat[id] == 0)
		return -1;
	for(no = 0 ; no < 8 ; no++){
		if(siopcb->snd_stat[id] & (1<<no))
			return no;
	}
	return -1;
}

/*
 *  送信待ちパケットを送信する
 */
static void
bluetooth_tx_int(SIOPCB *siopcb)
{
	SIOPACKET *p;
	INT       no;

	siopcb->snd_stat[SND_EMPY] |= siopcb->snd_stat[SND_OUT];
	siopcb->snd_stat[SND_OUT] = 0;
	if((no = bluetooth_search_packet(siopcb, SND_RDY)) >= 0){
		siopcb->snd_stat[SND_OUT] |= 1<<no;
		siopcb->snd_stat[SND_RDY] &= ~(1<<no);
		p = &siopcb->snd_buf[no];
		uart_puts(siopcb, p->buf, p->len);
	}
	else{
		uart_disable_send(siopcb);
	}
}

/*
 *  BLUETOOTHコマンド送信の空きパケットの状態を返す
 */
ER_BOOL
bluetooth_send_ready(ID portid)
{
	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	return(get_siopcb(portid)->snd_stat[SND_EMPY] != 0);
}

/*
 *  BLUETOOTHのコマンド受信待ちデータ数を返す
 */
ER_UINT
bluetooth_received_count(ID portid)
{
    SIOPCB  *siopcb;

	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	siopcb = get_siopcb(portid);
	return uart_getready(siopcb);
}

void
bluetooth_clear_arm7_cmd(void)
{
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_CODR), BT_ARM7_CMD_PIN);
}

void
bluetooth_set_arm7_cmd(void)
{
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_SODR), BT_ARM7_CMD_PIN);
}

/*
 *  BLUETOOTH用のコマンド用空き送信パケットを返す
 */
ER
bluetooth_get_packet(ID portid, SIOPACKET **ppkt, INT *plen)
{
    SIOPCB *siopcb;
	INT    no;

	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	siopcb = get_siopcb(portid);
	if((no = bluetooth_search_packet(siopcb, SND_EMPY)) >= 0){
		siopcb->snd_stat[SND_EMPY] &= ~(1<<no);
		*ppkt = &siopcb->snd_buf[no];
		*plen = SIORBUFSIZE;
		return E_OK;
	}
	else{
		*plen = 0;
		return E_NOMEM;
	}
}

/*
 *  BLUETOOTHのコマンド用送信パケットを送信要求する
 */
ER
bluetooth_send(ID portid, SIOPACKET *p)
{
    SIOPCB  *siopcb;

	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	siopcb = get_siopcb(portid);
	if(uart_putready(siopcb)){
		siopcb->snd_stat[SND_RDY] |= 1<<p->id;
		uart_enable_send(siopcb);
	}
	return E_OK;
}

/*
 *  BLUETOOTH受信コマンドを取り出す
 */
ER
bluetooth_receive(ID portid, unsigned char * buf)
{
    SIOPCB   *siopcb;
	int bytes_ready, total_bytes_ready;
	int cmd_len = 0;
	int i;
	unsigned char *buf_ptr, *tmp_ptr;

	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	siopcb = get_siopcb(portid);
	buf_ptr = &(siopcb->rcv_buf[siopcb->rcv_no&1][0]);
	buf[0] = 0;
	buf[1] = 0;

	if (get_us1_rcvcnt2() == 0){
	  	bytes_ready = SIORBUFSIZE;
	  	total_bytes_ready = SIORBUFSIZE*2 - get_us1_rcvcnt1();
	}
	else{
		total_bytes_ready = bytes_ready = SIORBUFSIZE - get_us1_rcvcnt1();
	}
	/*
	 *  2バイト以上の受信データがある場合は
	 *  コマンドの取り込みを行う
	 */
	if (total_bytes_ready > siopcb->rcv_idx + 1) {
	  	cmd_len = (int) buf_ptr[siopcb->rcv_idx];
  	
	  	// Data mode kludge - data cannot be more than 255 bytes
  	
		if (siopcb->rcv_idx < (SIORBUFSIZE-1)){
	  		if (buf_ptr[siopcb->rcv_idx+1] == 0) cmd_len++;
		}
		else{
			tmp_ptr = &(siopcb->rcv_buf[(siopcb->rcv_no+1)&1][0]);
			if (tmp_ptr[0] == 0) cmd_len++;
		}
		/*
		 *  コマンドの取り込みを行う
		 */
		if (bytes_ready >= siopcb->rcv_idx + cmd_len + 1) {
			for(i=0;i<cmd_len+1;i++) buf[i] = buf_ptr[siopcb->rcv_idx++];
		}
		else if (total_bytes_ready >= siopcb->rcv_idx + cmd_len + 1){
			for(i=0;i<cmd_len+1 && siopcb->rcv_idx < 128;i++) buf[i] = buf_ptr[siopcb->rcv_idx++];
			siopcb->rcv_idx = 0;
			tmp_ptr = &(siopcb->rcv_buf[(siopcb->rcv_no+1)&1][0]);
			for(;i<cmd_len+1;i++) buf[i] = tmp_ptr[siopcb->rcv_idx++];
			siopcb->rcv_idx += SIORBUFSIZE;
		}
		else{	/* コマンドを全て受信していない */
			return E_NOMEM;
		}
	}
	/*
	 *  受信インデックスがSIORBUFSIZEを越えた場合
	 *  トグルの受信バッファと受信インデックスを再設定する
	 */
	if (siopcb->rcv_idx >= SIORBUFSIZE && get_us1_rcvcnt2() == 0){
 		siopcb->rcv_idx -= SIORBUFSIZE;
		set_us1_rcvptr2((unsigned int) buf_ptr);
		set_us1_rcvcnt2(SIORBUFSIZE);
		siopcb->rcv_no++;
	}
	return cmd_len;
}

/*
 *  BLUETOOTH接続状態を取り出す
 */
ER_UINT
bluetooth_get_connect_state(ID portid)
{
	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	return(get_siopcb(portid)->connect_state);
}

/*
 *  BLUETOOTHの接続状態を変更する
 */
ER
bluetooth_set_connect_state(ID portid, INT state)
{
	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	get_siopcb(portid)->connect_state = state;
	return E_OK;
}

/*
 *  BLUETOOTHデバイスのリセット要求
 *  割込みハンドラからも実行可能
 */
ER
bluetooth_reset(ID portid)
{
    SIOPCB *siopcb;
	UB     *buf;
	INT    cnt;

	if (!(1 <= portid && portid <= TNUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}
	siopcb = get_siopcb(portid);
	/*
	 *  電源オフ状態では実行しない
	 */
	if((sil_rew_mem((VP)(TADR_PIO_BASE+TOFF_PIO_ODSR)) & BT_RST_PIN) == 0){
		return E_OK;
	}
	/*
	 *  コマンドモード設定
	 */
	bluetooth_clear_arm7_cmd();
	/*
     *  BC4チップに対して100ms間リセット設定
	 */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_CODR), BT_RST_PIN);
	buf = siopcb->siobuffer;
	for(cnt = 0 ; cnt < 100 ; cnt++){
		bluetooth_receive(portid, buf);
		sil_dly_nse(1000*1000);	/* 1msの待ち */
	}
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_SODR), BT_RST_PIN);
	/*
	 * 5秒間のRESET INDICATION待ちを行う
	 */
	for(cnt = 0 ; cnt < 5000 ; cnt++){
		bluetooth_receive(portid, buf);
		/*
		 *  RESET INDICATIONとチェックサムの確認
		 */
		if ((buf[0] == 3) && (buf[1] == MSG_RESET_INDICATION) &&
			(buf[2] == 0xff) && (buf[3] == 0xe9))
			break;
		sil_dly_nse(1000*1000);	/* 1msの待ち */
	}
	/*
	 *  コマンドモード再設定
	 */
	bluetooth_clear_arm7_cmd();
	bluetooth_set_connect_state(portid, BLUETOOTH_NO_INIT);
	return E_OK;
}

/*
 *  SIOドライバの初期化ルーチン
 *  BLUETOOTHの初期化をあわせて行う
 */
void
uart_initialize()
{
    SIOPCB  *siopcb;
	unsigned long portpin, temp;
	INT     i;

    /*
     *  シリアルI/Oポート管理ブロックの初期化
     */
    siopcb = siopcb_table;
    siopcb->siopinib = &siopinib_table[0];
    siopcb->openflag = FALSE;
    siopcb->intflag  = FALSE;
	siopcb->connect_state = BLUETOOTH_DEVICE_INITIALIZED;
	siopcb->rcv_idx  = 0;
	siopcb->rcv_no   = 0;
	siopcb->snd_stat[SND_EMPY] = 0x03;
	siopcb->snd_stat[SND_RDY]  = 0x00;
	siopcb->snd_stat[SND_OUT]  = 0x00;
	for(i = 0 ; i < NUM_SNDBUF ; i++){
		siopcb->snd_buf[i].id  = i;
		siopcb->snd_buf[i].len = 0;
	}
	siopcb->bcount   = 0;
	siopcb->obcount  = 0;

	sil_wrw_mem((VP)(TADR_PMC_BASE+TOFF_PMC_PCER), 1 << IRQ_US1_PID);	/* Uart1 clock on */
	portpin = BT_RX_PIN | BT_TX_PIN | BT_SCK_PIN | BT_RTS_PIN | BT_CTS_PIN;
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_PDR), portpin);	/* disable */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_ASR), portpin);	/* select Peripheral A */

	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_CR), US_RSTSTA);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_CR), US_STTTO);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_RTOR), 10000);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_IDR), US_TIMEOUT);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_MR), US_USMODE_HWHSH |US_CLKS_MCK|US_CHRL_8|US_PAR_NO|US_NBSTOP_1|US_OVER);
	temp = (MCK/8/BAUD_RATE) | (((MCK/8) - ((MCK/8/BAUD_RATE) * BAUD_RATE)) / ((BAUD_RATE + 4)/8)) << 16;
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_BRGR), temp);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_PTCR), PDC_RXTDIS|PDC_TXTDIS);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RCR), 0);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_TCR), 0);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RNCR), 0);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_TNCR), 0);

	temp = sil_rew_mem((VP)(TADR_US1_BASE+TOFF_US_RHR));
	temp = sil_rew_mem((VP)(TADR_US1_BASE+TOFF_US_CSR));

	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RPR), (unsigned long)&siopcb->rcv_buf[0][0]);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RCR), SIORBUFSIZE);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RNPR), (unsigned long)&siopcb->rcv_buf[1][0]);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_RNCR), SIORBUFSIZE);

	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_US_CR), US_RXEN|US_TXEN);
	sil_wrw_mem((VP)(TADR_US1_BASE+TOFF_PDC_PTCR), PDC_RXTEN|PDC_TXTEN);

	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_PDR), portpin);	/* disable */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_ASR), portpin);	/* select Peripheral A */
	portpin = BT_CS_PIN | BT_RST_PIN;
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_PER), portpin);	/* enable */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_OER), portpin);	/* output enable */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_SODR), portpin);/* set output data */
	portpin = BT_ARM7_CMD_PIN;
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_PUDR), portpin);/* pull-up disable */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_PER), portpin);	/* enable */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_CODR), portpin);/* clear output */
	sil_wrw_mem((VP)(TADR_PIO_BASE+TOFF_PIO_OER), portpin);	/* output enable */

	// Configure timer 01 as trigger for ADC, sample every 0.5ms
	sil_wrw_mem((VP)(TADR_PMC_BASE+TOFF_PMC_PCER), 1 << IRQ_TC1_PID);	/* Timer1 clock on */
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_CCR), TC_CLKDIS);
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_IDR), 0xFFFFFFFF);
	temp = sil_rew_mem((VP)(TADR_TC1_BASE+TOFF_TC_SR));
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_CMR), TC_WAVE|TC_WAVESEL10|TC_ACPA_SET|TC_ACPC_CLEAR|TC_ASWTRG_SET);
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_RC), (CLOCK_FREQUENCY/2)/2000);
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_RA), (CLOCK_FREQUENCY/2)/4000);
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_CCR), TC_CLKEN);
	sil_wrw_mem((VP)(TADR_TC1_BASE+TOFF_TC_CCR), TC_SWTRG);

	sil_wrw_mem((VP)(TADR_PMC_BASE+TOFF_PMC_PCER), 1 << IRQ_ADC_PID);	/* ADC clock on */
	sil_wrw_mem((VP)(TADR_ADC_BASE+TOFF_ADC_MR), 0x00000000);
	temp = ADC_TRGEN|ADC_TRGSEL_TIOA1;
	sil_wrw_mem((VP)(TADR_ADC_BASE+TOFF_ADC_MR), temp);
	temp |= 0x00003F00;								/* set PRESCAL */
	sil_wrw_mem((VP)(TADR_ADC_BASE+TOFF_ADC_MR), temp);
	temp |= 0x00020000;								/* set STARTUP */
	sil_wrw_mem((VP)(TADR_ADC_BASE+TOFF_ADC_MR), temp);
	temp |= 0x09000000;								/* set SHTIM */
	sil_wrw_mem((VP)(TADR_ADC_BASE+TOFF_ADC_MR), temp);
	sil_wrw_mem((VP)(TADR_ADC_BASE+TOFF_ADC_CHER), ADC_CH6|ADC_CH4);

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
	unsigned long   tbase;

    siopcb = get_siopcb(siopid);
    siopinib = siopcb->siopinib;
	siopcb->exinf = exinf;
	tbase  = (unsigned long)siopcb->siopinib->time_base;

	bluetooth_reset(siopid);

    /*  クロックイネーブル */
    sil_wrw_mem((VP)(TADR_PMC_BASE+TOFF_PMC_PCER), (1<<IRQ_TC2_PID));
    /*  タイマ停止 */
    sil_wrw_mem((VP)(tbase+TOFF_TC_CCR), TC_CLKDIS);
    sil_wrw_mem((VP)(tbase+TOFF_TC_IDR), 0xFFFFFFFF);
    /*  カウント値をセット */
    sil_wrw_mem((VP)(tbase+TOFF_TC_CMR), TC_CLKS_MCK8);	/* 47,923,200Hz/8=5,990,400Hz */
    sil_wrw_mem((VP)(tbase+TOFF_TC_CMR), sil_rew_mem((VP)(tbase+TOFF_TC_CMR)) | TC_WAVESEL10);
    sil_wrw_mem((VP)(tbase+TOFF_TC_RC), 59900);			/* 10ms */
    /*  割込みの禁止  */
	uart_disable_rcv(siopcb);
	uart_disable_send(siopcb);
    /*  カウントスタート   */
    sil_wrw_mem((VP)(tbase+TOFF_TC_CCR), (TC_CLKEN|TC_SWTRG));

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
    /* 割込みの停止  */
	uart_disable_rcv(siopcb);
    /* タイマ停止 */
    sil_wrw_mem((VP)(siopcb->siopinib->time_base+TOFF_TC_CCR), TC_CLKDIS);
    sil_wrw_mem((VP)(siopcb->siopinib->time_base+TOFF_TC_IDR), 0xFFFFFFFF);
	sil_wrw_mem((VP)(siopcb->siopinib->uart_base+TOFF_US_CR), US_RSTRX|US_RSTTX|US_RXDIS|US_TXDIS);
    siopcb->openflag = FALSE;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
uart_snd_chr(SIOPCB *siopcb, char c)
{
	/*
	 *  bluetoothがSTREAM接続されていなければ送信しない
	 */
	if(!siopcb->intflag && siopcb->connect_state != BLUETOOTH_STREAM)
		return TRUE;
	/*
	 *  送信可能かどうかをチェックする
	 */
	if(siopcb->bcount < SIOBUFSIZE && uart_putready(siopcb)) {
		if(!siopcb->intflag)
			loc_cpu();
		siopcb->siobuffer[siopcb->bcount] = c;	/* 送信バッファに文字を入れる */
		siopcb->bcount++;						/* 送信バッファ中の文字数をインクリメント */
		if(c == '\n' || siopcb->bcount >= SIOBUFSIZE) {
			/*
			 * 	改行文字の場合 or 送信バッファがフルの場合
			 *  STREAMモードの送信を行う
			 */
			siopcb->obcount = 0;		/* 先にゼロを設定して、timerによる転送を抑制 */
			uart_puts(siopcb, siopcb->siobuffer, siopcb->bcount);
			siopcb->bcount  = 0;		/* 送信バッファ中の文字数をリセット */
		}
		if(!siopcb->intflag)
			unl_cpu();
		return TRUE;
	}
    return FALSE;
}

/*
 *  シリアルI/Oポートからの文字受信
 */
INT
uart_rcv_chr(SIOPCB *siopcb)
{
	int bytes_ready;
	int c = 0;
	unsigned char *p;

	p = &(siopcb->rcv_buf[siopcb->rcv_no&1][0]);

	if (get_us1_rcvcnt2() == 0)
	  	bytes_ready = SIORBUFSIZE*2 - get_us1_rcvcnt1();
	else
		bytes_ready = SIORBUFSIZE - get_us1_rcvcnt1();

	if (bytes_ready > siopcb->rcv_idx)
	  	c = (int) p[siopcb->rcv_idx++];
	/*
	 *  受信インデックスがSIORBUFSIZEを越えた場合
	 *  トグルの受信バッファと受信インデックスを再設定する
	 */
	if (siopcb->rcv_idx >= SIORBUFSIZE && get_us1_rcvcnt2() == 0){
 		siopcb->rcv_idx -= SIORBUFSIZE;
		set_us1_rcvptr2((unsigned int)p);
		set_us1_rcvcnt2(SIORBUFSIZE);
		siopcb->rcv_no++;
	}
	return c;
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
	siopcb->intflag = TRUE;
    if (uart_putready(siopcb)) {
		if(siopcb->connect_state != BLUETOOTH_STREAM){
			/*
			 *  STREAM通信でない場合コマンド送信を行う
			 */
			bluetooth_tx_int(siopcb);
		}
		else{
	        /*
    	     *  送信可能コールバックルーチンを呼び出す．
        	 */
	        uart_ierdy_snd(siopcb->exinf);
		}
    }
	siopcb->intflag = FALSE;
}


/*
 *  SIOの割込みサービスルーチン
 */
void
uart_isr1()
{
    uart_isr_siop(&(siopcb_table[0]));
}

void
int_timer2()
{
    SIOPCB *siopcb = &(siopcb_table[0]);

    sil_rew_mem((VP)(TADR_TC2_BASE+TOFF_TC_SR));
    if(siopcb->connect_state == BLUETOOTH_STREAM){
 		if(uart_getready(siopcb)) {
            /*
             *  受信通知コールバックルーチンを呼び出す．
             */
            uart_ierdy_rcv(siopcb->exinf);
        }
        if(siopcb->bcount > 0 && uart_putready(siopcb)){
			iloc_cpu();
			if(siopcb->bcount == siopcb->obcount){
				siopcb->obcount = 0;
				uart_puts(siopcb, siopcb->siobuffer, siopcb->bcount);    /* DMA転送を始める */
				siopcb->bcount  = 0;         /* 送信バッファ中の文字数をリセット */
			}
			else
				siopcb->obcount = siopcb->bcount;
			iunl_cpu();
		}
	}
}

