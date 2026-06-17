/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: hw_serial.c 2246 2025-12-09 11:44:59Z roi $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（NUCLEO-F446RE用）
 */

#include <t_services.h>
#include <sys_config.h>
#include <hw_serial.h>
#include <t_syslog.h>

/*
 *  SIL関数のマクロ定義
 */
#define sil_orw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) | (b))
#define sil_andw_mem(a, b)		sil_wrw_mem((a), sil_rew_mem(a) & ~(b))
#define sil_modw_mem(a, b, c)	sil_wrw_mem((a), (sil_rew_mem(a) & (~b)) | (c))
#define sil_orh_mem(a, b)		sil_wrh_mem((a), sil_reh_mem(a) | (b))
#define sil_andh_mem(a, b)		sil_wrh_mem((a), sil_reh_mem(a) & ~(b))
#define sil_modh_mem(a, b, c)	sil_wrh_mem((a), (sil_reh_mem(a) & (~b)) | (c))

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((UINT)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

#define DEFAULT_PORT    1

/*
 *  UNIVERSAL SYNCHRONOUS ASYNCHORONOUS RECEIVER TRANSMITTER
 */
#define TADR_USART1_BASE    0x40011000
#define TADR_USART6_BASE    0x40011400
#define TOFF_USART_SR       0x0000		/* (RW-16) USART Status register */
  #define USART_SR_PE        0x0001		/* Parity Error */
  #define USART_SR_FE        0x0002		/* Framing Error */
  #define USART_SR_NE        0x0004		/* Noise Error Flag */
  #define USART_SR_ORE       0x0008		/* OverRun Error */
  #define USART_SR_RXNE      0x0020		/* Read Data Register Not Empty */
  #define USART_SR_TC        0x0040		/* Transmission Complete */
#define TOFF_USART_DR       0x0004		/* (RW-16) USART Data register */
#define TOFF_USART_BRR      0x0008		/* (RW-16) USART Baud rate register */
#define TOFF_USART_CR1      0x000C		/* (RW-16) USART Control register 1 */
  #define USART_CR1_RE       0x0004		/* Receiver Enable */
  #define USART_CR1_TE       0x0008		/* Transmitter Enable */
  #define USART_CR1_RXNEIE   0x0020		/* RXNE Interrupt Enable */
  #define USART_CR1_TXEIE    0x0080		/* PE Interrupt Enable */
  #define USART_CR1_PS       0x0200		/* Parity Selection */
  #define USART_CR1_PCE      0x0400		/* Parity Control Enable */
  #define USART_CR1_M        0x1000		/* Word length */
  #define USART_CR1_UE       0x2000		/* USART Enable */
  #define USART_CR1_OVER8    0x8000		/* USART Oversampling by 8 enable */
#define TOFF_USART_CR2      0x0010		/* (RW-16) USART Control register 2 */
  #define USART_CR2_STOP     0x3000		/* STOP[1:0] bits (STOP bits) */
  #define USART_CR2_STOP_0   0x1000		/* Bit 0 */
  #define USART_CR2_STOP_1   0x2000		/* Bit 1 */
#define TOFF_USART_CR3      0x0014		/* (RW-16) USART Control register 3 */
  #define USART_CR3_RTSE     0x0100		/* RTS Enable */
  #define USART_CR3_CTSE     0x0200		/* CTS Enable */

/*
 *  GENERAL PURPOSE I/O
 */
#define TOFF_GPIO_MODER     0x0000		/* (RW) GPIO port mode register */
  #define GPIO_MODER_MODER0  0x1
  #define GPIO_MODER_MODER1  0x2
  #define GPIO_MODER_MODER2  0x3
#define TOFF_GPIO_OTYPER    0x0004		/* (RW) GPIO port output type register */
  #define GPIO_OTYPER_OT     0x1
#define TOFF_GPIO_OSPEEDR   0x0008		/* (RW) GPIO port output speed register */
  #define GPIO_OSPEEDER_OSPEEDR0 0x1
  #define GPIO_OSPEEDER_OSPEEDR1 0x2
  #define GPIO_OSPEEDER_OSPEEDR2 0x3
#define TOFF_GPIO_PUPDR     0x000C		/* (RW) GPIO port pull-up/pull-down register */
  #define GPIO_PUPDR_PUPDR0  0x1
  #define GPIO_PUPDR_PUPDR1  0x2
  #define GPIO_PUPDR_PUPDR2  0x3
#define TOFF_GPIO_AFR0      0x0020		/* (RW) GPIO alternate function registers */


/*
 *  GPIO Configuration Mode enumeration
 */
#define GPIO_Mode_IN    0x0					/* GPIO Input Mode */
#define GPIO_Mode_OUT   GPIO_MODER_MODER0	/* GPIO Output Mode */
#define GPIO_Mode_AF    GPIO_MODER_MODER1	/* GPIO Alternate function Mode */
#define GPIO_Mode_AN    GPIO_MODER_MODER2	/* GPIO Analog Mode */

/*
 *  GPIO Output Maximum frequency enumeration
 */
#define GPIO_Speed_2MHz   0x0						/* Low speed */
#define GPIO_Speed_25MHz  GPIO_OSPEEDER_OSPEEDR0	/* Medium speed */
#define GPIO_Speed_50MHz  GPIO_OSPEEDER_OSPEEDR1	/* Fast speed */
#define GPIO_Speed_100MHz GPIO_OSPEEDER_OSPEEDR2	/* High speed on 30 pF (80 MHz Output max speed on 15 pF) */

/*
 *  GPIO Output type enumeration
 */
#define GPIO_OType_PP   0x0
#define GPIO_OType_OD   0x1

/*
 *  GPIO Configuration PullUp PullDown enumeration
 */
#define GPIO_PuPd_NOPULL 0x0
#define GPIO_PuPd_UP     GPIO_PUPDR_PUPDR0
#define GPIO_PuPd_DOWN   GPIO_PUPDR_PUPDR1

/*
 *  AF 7 selection
 */
#define GPIO_AF7                0x07	/* AF7 value */
#define GPIO_AF8                0x08	/* AF8 value */

/*
 *  USART Word Length
 */
#define USART_WordLength_8b     0x0000
#define USART_WordLength_9b     USART_CR1_M

/*
 *  USART Stop Bits
 */ 
#define USART_StopBits_1        0x0000
#define USART_StopBits_0_5      USART_CR2_STOP_0
#define USART_StopBits_2        USART_CR2_STOP_1
#define USART_StopBits_1_5      USART_CR2_STOP

/*
 *  USART Parity
 */
#define USART_Parity_No         0x0000
#define USART_Parity_Even       USART_CR1_PCE
#define USART_Parity_Odd        (USART_CR1_PCE | USART_CR1_PS) 

/*
 *  USART MODE
 */
#define USART_Mode_Rx           USART_CR1_RE
#define USART_Mode_Tx           USART_CR1_TE

/** @defgroup USART_Hardware_Flow_Control 
  * @{
  */ 
#define USART_HardwareFlowControl_None       0x0000
#define USART_HardwareFlowControl_RTS        USART_CR3_RTSE
#define USART_HardwareFlowControl_CTS        USART_CR3_CTSE
#define USART_HardwareFlowControl_RTS_CTS    (USART_CR3_RTSE | USART_CR3_CTSE)

#define CR1_CLEAR_MASK          (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE)
#define CR3_CLEAR_MASK          (USART_CR3_RTSE | USART_CR3_CTSE)

#ifndef U_GPIOSPEED
#define U_GPIOSPEED             GPIO_Speed_50MHz
#endif


/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	UW base;
	INTNO    intno_usart;
	UW intpri;
	UW clockbase/* pfr*/;		/* PFRxレジスタアドレス */
	UW clock_set;
	UW portbase;
	UW port_set;
	UW gpio_af;

	UW txportbase;
	UW txpin;
	UW rxportbase;
	UW rxpin;
} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block {
	const SIOPINIB  *siopinib;	/* シリアルI/Oポート初期化ブロック */
	VP_INT          exinf;		/* 拡張情報 */
	BOOL            opnflg;		/* オープン済みフラグ */
};

/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{0x40004400, (INTNO)INTNO_SIO1, 7,	/* USART2 */
	 (UW)0x40023840, (UW)0x00020000,	/* RCC_APB1ENR/RCC_APB1ENR_USART2EN */
     (UW)0x40023830, (UW)0x00000001,	/* RCC_AHB1ENR/RCC_AHB1ENR_GPIOAEN */
	 (UW)GPIO_AF7,
     (UW)0x40020000, (UW)2,				/* GPIOA_BASE/TX */
     (UW)0x40020000, (UW)3,				/* GPIOA_BASE/RX */
#if TNUM_SIOP >= 2
    },
	{0x40011000, (INTNO)INTNO_SIO2, 7,	/* USART1 */
	 (UW)0x40023844, (UW)0x00000010,	/* RCC_APB2ENR/RCC_APB2ENR_USART1EN */
     (UW)0x40023830, (UW)0x00000001,	/* RCC_AHB1ENR/RCC_AHB1ENR_GPIOAEN */
	 (UW)GPIO_AF7,
     (UW)0x40020000, (UW)9,				/* GPIOA_BASE/TX */
     (UW)0x40020000, (UW)10,			/* GPIOA_BASE/RX */
#endif
    }
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];

static void
setup_gpio_source(UW base, UB source, UB select)
{
	UW tmpreg, regoff;
	regoff = TOFF_GPIO_AFR0+((source>>1) & 0x4);
	tmpreg = (sil_rew_mem((VP)(base+regoff)) & ~(0xF << ((source & 0x07) * 4)))
			    | (select << ((source & 0x07) * 4));
	sil_wrw_mem((VP)(base+regoff), tmpreg);
}

void
put_hex(char a, int val)
{
	int i, j;
	sys_putc(a);
	sys_putc(' ');
	for(i = 28 ; i >= 0 ; i-= 4){
		j = (val >> i) & 0xf;;
		if(j > 9)
			j += ('A'-10);
		else
			j += '0';
		sys_putc(j);
	}
	sys_putc('\n');
}

/*
 *  SIOドライバの初期化
 */
void
sio_initialize(void)
{
	SIOPCB	*siopcb;
	UINT	i;

	/*
	 *  シリアルI/Oポート管理ブロックの初期化
	 */
	for (siopcb = siopcb_table, i = 0; i < TNUM_SIOP; siopcb++, i++) {
		siopcb->siopinib = &(siopinib_table[i]);
		siopcb->opnflg = FALSE;
	}
}


/*
 *  シリアルI/Oポートのオープン
 */
SIOPCB *
sio_opn_por(ID siopid, VP_INT exinf)
{
	SIOPCB	*siopcb;
	const SIOPINIB  *siopinib;
	BOOL	opnflg;
	ER	ercd;
	UW	base, txbase, rxbase, txpin, rxpin;
	UW	apbclock, integerdivider, fractionaldivider, tmp;

	siopcb = get_siopcb(siopid);
	siopinib = siopcb->siopinib;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = siopcb->opnflg;

	siopcb->exinf = exinf;
	txbase = siopinib->txportbase;
	rxbase = siopinib->rxportbase;
	txpin  = siopinib->txpin;
	rxpin  = siopinib->rxpin;
	base = siopinib->base;

	if(txbase == 0 || rxbase == 0)	/* no usart port */
		goto sio_opn_exit;

	/*
	 *  ハードウェアの初期化
	 */
	sil_orw_mem((VP)(siopinib->clockbase), siopinib->clock_set);
	sil_orw_mem((VP)(siopinib->portbase), siopinib->port_set);
	sil_modw_mem((VP)(txbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (txpin*2)), (GPIO_Mode_AF << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (txpin*2)), (U_GPIOSPEED << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << txpin),  (GPIO_OType_PP << txpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (txpin*2)), (GPIO_PuPd_UP << (txpin*2)));
	sil_modw_mem((VP)(rxbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (rxpin*2)), (GPIO_Mode_AF << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (rxpin*2)), (U_GPIOSPEED << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << rxpin),  (GPIO_OType_PP << rxpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (rxpin*2)), (GPIO_PuPd_UP << (rxpin*2)));
	setup_gpio_source(txbase, txpin, siopinib->gpio_af);
	setup_gpio_source(rxbase, rxpin, siopinib->gpio_af);

	sil_andh_mem((VP)(base+TOFF_USART_CR1), USART_CR1_UE);
	sil_modh_mem((VP)(base+TOFF_USART_CR2), USART_CR2_STOP, USART_StopBits_1);
	sil_modh_mem((VP)(base+TOFF_USART_CR1), CR1_CLEAR_MASK, (USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx));
	sil_modh_mem((VP)(base+TOFF_USART_CR3), CR3_CLEAR_MASK, USART_HardwareFlowControl_None);

	if ((base == TADR_USART1_BASE) || (base == TADR_USART6_BASE))
		apbclock = SysFrePCLK2;
	else
		apbclock = SysFrePCLK1;

	if ((sil_reh_mem((VP)(base+TOFF_USART_CR1)) & USART_CR1_OVER8) != 0){
		integerdivider = ((25 * apbclock) / (2 * DEF_BAUDRATE));	/* 8 Samples */
		tmp = (integerdivider / 100) << 4;
		fractionaldivider = integerdivider - (100 * (tmp >> 4));
		tmp |= ((((fractionaldivider * 8) + 50) / 100)) & 0x07;
	}
	else{ /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
		integerdivider = ((25 * apbclock) / (4 * DEF_BAUDRATE));	/* 16 Samples */
		tmp = (integerdivider / 100) << 4;
		fractionaldivider = integerdivider - (100 * (tmp >> 4));
		tmp |= ((((fractionaldivider * 16) + 50) / 100)) & 0x0F;
	}
	sil_wrh_mem((VP)(base+TOFF_USART_BRR), tmp);
	sil_orh_mem((VP)(base+TOFF_USART_CR1), USART_CR1_RXNEIE);
	siopcb->opnflg = TRUE;

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		x_config_int(siopinib->intno_usart, TRUE, siopinib->intpri);
	}
	sil_orh_mem((VP)(base+TOFF_USART_CR1), USART_CR1_UE);

sio_opn_exit:;
	return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *siopcb)
{
	const SIOPINIB  *siopinib;

	siopinib = siopcb->siopinib;

	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	if ((siopcb->opnflg)) {
		dis_int(siopinib->intno_usart);
	}
	siopcb->opnflg = FALSE;
}

/*
 *  SIOの割込みサービスルーチン
 */

Inline BOOL
sio_putready(SIOPCB* siopcb)
{
	UH cr1 = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1));
	UH ssr = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_SR));

	if ((cr1 & USART_CR1_TXEIE) != 0 && (ssr & USART_SR_TC) != 0)
	{
		return TRUE;
	}
	return FALSE;
}

Inline BOOL
sio_getready(SIOPCB* siopcb)
{
	UH cr1 = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1));
	UH ssr = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_SR));
	UH temp;

	if ((ssr & (USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE)) != 0) {
		temp = 0;
		sil_dly_nse(100*1000);
		syslog(LOG_ERROR, "sio_usart_isr error (%d) ssr[%04x] !", siopcb->siopinib->intno_usart, ssr);
		temp = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_DR));
		(void)(temp);
		return FALSE;
	}
	if ((cr1 & USART_CR1_RXNEIE) != 0 && (ssr & USART_SR_RXNE) != 0) {
		return TRUE;
	}
	return FALSE;
}

void
sio_handler1(void)
{
	SIOPCB	*siopcb = get_siopcb(1);

	if (sio_getready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putready(siopcb)) {
		sio_ierdy_snd(siopcb->exinf);
	}
}

#if TNUM_SIOP >= 2
void
sio_handler2(void)
{
	SIOPCB	*siopcb = get_siopcb(2);

	if (sio_getready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putready(siopcb)) {
		sio_ierdy_snd(siopcb->exinf);
	}
}
#endif

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sio_snd_chr(SIOPCB *siopcb, char c)
{
	if (sio_putready(siopcb)) {
		sil_wrh_mem((VP)(siopcb->siopinib->base+TOFF_USART_DR), (UH)c);
		return TRUE;
	}
	return FALSE;
}

/*
 *  シリアルI/Oポートからの文字受信
 */
INT
sio_rcv_chr(SIOPCB *siopcb)
{
	INT c = -1;

	if (sio_getready(siopcb)) {
		c = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_DR)) & 0xFF;
	}
	return c;
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_orh_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_TXEIE);
		break;
	case SIO_ERDY_RCV:
		sil_orh_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_RXNEIE);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_andh_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_TXEIE);
		break;
	case SIO_ERDY_RCV:
		sil_andh_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_RXNEIE);
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void
sio_snd_chr_pol(char c)
{
	const SIOPINIB  *siopinib = &siopinib_table[INDEX_SIOP(DEFAULT_PORT)];
	UW base = siopinib->base;

	sil_wrh_mem((VP)(base+TOFF_USART_DR), (UH)c);
	while(0 == (sil_reh_mem((VP)(base+TOFF_USART_SR)) & USART_SR_TC));

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = 300000000/DEF_BAUDRATE;
	while(n--);
}

/*
 *  ターゲットのシリアル初期化
 */
void
sio_init(void)
{
	const SIOPINIB  *siopinib = &siopinib_table[INDEX_SIOP(DEFAULT_PORT)];
	UW	base, txbase, rxbase, txpin, rxpin;
	UW	apbclock, integerdivider, fractionaldivider, tmp;

	txbase = siopinib->txportbase;
	rxbase = siopinib->rxportbase;
	txpin  = siopinib->txpin;
	rxpin  = siopinib->rxpin;
	base = siopinib->base;

	sil_orw_mem((VP)(siopinib->clockbase), siopinib->clock_set);
	sil_orw_mem((VP)(siopinib->portbase), siopinib->port_set);
	sil_modw_mem((VP)(txbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (txpin*2)), (GPIO_Mode_AF << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (txpin*2)), (U_GPIOSPEED << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << txpin),  (GPIO_OType_PP << txpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (txpin*2)), (GPIO_PuPd_UP << (txpin*2)));
	sil_modw_mem((VP)(rxbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER2 << (rxpin*2)), (GPIO_Mode_AF << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR2 << (rxpin*2)), (U_GPIOSPEED << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << rxpin),  (GPIO_OType_PP << rxpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR2 << (rxpin*2)), (GPIO_PuPd_UP << (rxpin*2)));
	setup_gpio_source(txbase, txpin, siopinib->gpio_af);
	setup_gpio_source(rxbase, rxpin, siopinib->gpio_af);

	sil_modh_mem((VP)(base+TOFF_USART_CR2), USART_CR2_STOP, USART_StopBits_1);
	sil_modh_mem((VP)(base+TOFF_USART_CR1), CR1_CLEAR_MASK, (USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx));
	sil_modh_mem((VP)(base+TOFF_USART_CR3), CR3_CLEAR_MASK, USART_HardwareFlowControl_None);

	if ((base == TADR_USART1_BASE) || (base == TADR_USART6_BASE))
		apbclock = SysFrePCLK2;
	else
		apbclock = SysFrePCLK1;

	if ((sil_reh_mem((VP)(base+TOFF_USART_CR1)) & USART_CR1_OVER8) != 0){
		integerdivider = ((25 * apbclock) / (2 * DEF_BAUDRATE));	/* 8 Samples */
		tmp = (integerdivider / 100) << 4;
		fractionaldivider = integerdivider - (100 * (tmp >> 4));
		tmp |= ((((fractionaldivider * 8) + 50) / 100)) & 0x07;
	}
	else{ /* if ((USARTx->CR1 & USART_CR1_OVER8) == 0) */
		integerdivider = ((25 * apbclock) / (4 * DEF_BAUDRATE));	/* 16 Samples */
		tmp = (integerdivider / 100) << 4;
		fractionaldivider = integerdivider - (100 * (tmp >> 4));
		tmp |= ((((fractionaldivider * 16) + 50) / 100)) & 0x0F;
	}
	sil_wrh_mem((VP)(base+TOFF_USART_BRR), tmp);
	sil_andh_mem((VP)(base+TOFF_USART_CR1), (USART_CR1_TXEIE | USART_CR1_RXNEIE));
	sil_orh_mem((VP)(base+TOFF_USART_CR1),  USART_CR1_UE);
}
