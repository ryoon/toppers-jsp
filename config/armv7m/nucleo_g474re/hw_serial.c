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
 *  @(#) $Id: hw_serial.c 2246 2025-12-09 11:48:54Z roi $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（NUCLEO-G474RE用）
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

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((UINT)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

#define DEFAULT_PORT    1

#if !defined(HSI_VALUE)
#define HSI_VALUE		16000000				/* Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */
#if !defined(LSE_VALUE)
#define LSE_VALUE		32768				/* Value of the LSE oscillator in Hz */
#endif /* LSE_VALUE */

#define U_GPIOSPEED		GPIO_OSPEEDER_OSPEEDR2

/*
 *  UNIVERSAL SYNCHRONOUS ASYNCHORONOUS RECEIVER TRANSMITTER
 */
#define TADR_USART1_BASE    0x40013800
#define TADR_LPUART1_BASE   0x40008000
#define TADR_UART5_BASE     0x40005000
#define TOFF_USART_CR1      0x0000		/* (RW-32) USART Control register 1 */
  #define USART_CR1_UE       0x00000001	/* USART Enable */
  #define USART_CR1_RE       0x00000004	/* Receiver Enable */
  #define USART_CR1_TE       0x00000008	/* Transmitter Enable */
  #define USART_CR1_RXNEIE   0x00000020	/* RXNE Interrupt Enable */
  #define USART_CR1_PS       0x00000200	/* Parity Selection */
  #define USART_CR1_PCE      0x00000400	/* Parity Control Enable */
  #define USART_CR1_TXEIE    0x00000080	/* PE Interrupt Enable */
  #define USART_CR1_PEIE     0x00000100	/* PE Interrupt Enable */
  #define USART_CR1_OVER8    0x00008000	/* Oversampling by 8-bit or 16-bit mode */
  #define USART_CR1_M0       0x00001000	/* Word length bit 0 */
  #define USART_CR1_M        0x10001000	/* [M1:M0] Word length */
  #define USART_CR1_M1       0x10000000	/* Word length bit 1 */
  #define USART_CR1_FIFOEN   0x20000000	/* FIFO mode enable */
#define TOFF_USART_CR2      0x0004		/* (RW-32) USART Control register 2 */
  #define USART_CR2_STOP     0x00003000	/* STOP[1:0] bits (STOP bits) */
  #define USART_CR2_STOP_0   0x00001000	/* Bit 0 */
  #define USART_CR2_STOP_1   0x00002000	/* Bit 1 */
#define TOFF_USART_CR3      0x0008		/* (RW-32) USART Control register 3 */
  #define USART_CR3_EIE      0x00000001	/* Error Interrupt Enable */
  #define USART_CR3_RTSE     0x00000100	/* RTS Enable */
  #define USART_CR3_CTSE     0x00000200	/* CTS Enable */
  #define USART_CR3_ONEBIT   0x00000800	/* USART One bit method enable */
  #define USART_CR3_TXFTIE   0x00800000	/* TXFIFO threshold interrupt enable */
  #define USART_CR3_RXFTIE   0x10000000	/* RXFIFO threshold interrupt enable */
  #define USART_CR3_RXFTCFG  0x0E000000	/* RXFIFO FIFO threshold configuration */
  #define USART_CR3_RXFTCFG_0 0x02000000
  #define USART_CR3_RXFTCFG_1 0x04000000
  #define USART_CR3_RXFTCFG_2 0x08000000
  #define USART_CR3_TXFTCFG  0xE0000000	/* TXFIFO threshold configuration */
  #define USART_CR3_TXFTCFG_0 0x20000000
  #define USART_CR3_TXFTCFG_1 0x40000000
  #define USART_CR3_TXFTCFG_2 0x80000000
#define TOFF_USART_BRR      0x000C		/* (RW-32) USART Baud rate register */
#define TOFF_USART_ISR      0x001C		/* (RW) USART Interrupt and status register */
  #define USART_ISR_PE       0x00000001	/* Parity Error */
  #define USART_ISR_FE       0x00000002	/* Framing Error */
  #define USART_ISR_NE       0x00000004	/* Noise detected Flag */
  #define USART_ISR_ORE      0x00000008	/* OverRun Error */
  #define USART_ISR_RXNE     0x00000020	/* Read Data Register Not Empty */
  #define USART_ISR_TC       0x00000040	/* Transmission Complete */
#define TOFF_USART_RDR      0x0024		/* (RW-16) USART Receive Data register */
#define TOFF_USART_TDR      0x0028		/* (RW-16) USART Transmit Data register */
#define TOFF_USART_PRESC    0x002C		/* Prescaler register */
  #define USART_PRESC_PRESCALER   0x0000000F	/* PRESCALER[3:0] bits (Clock prescaler) */
  #define USART_PRESC_PRESCALER_0 0x00000001
  #define USART_PRESC_PRESCALER_1 0x00000002
  #define USART_PRESC_PRESCALER_2 0x00000004
  #define USART_PRESC_PRESCALER_3 0x00000008

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
 *  RESET AND CLOCK CONTROL
 */
#define TADR_RCC_BASE       0x40021000
#define TOFF_RCC_CCIPR      0x0088		/* (RW) RCC peripherals independent clock configuration register */
  #define RCC_CCIPR_USART1SEL   0x00000003	/* USART1SEL[1:0] bits */
  #define RCC_CCIPR_USART1SEL_0 0x00000001
  #define RCC_CCIPR_USART1SEL_1 0x00000002


/*
 *  USART CLOCK SOURCE
 */
#define RCC_USARTCLKSOURCE_PCLK2      0x00000000			/* PCLK2 selected as USART clock */
#define RCC_USARTCLKSOURCE_SYSCLK     RCC_CCIPR_USART1SEL_0	/* SYSCLK selected as USART clock */
#define RCC_USARTCLKSOURCE_HSI        RCC_CCIPR_USART1SEL_1	/* HSI selected as USART clock */
#define RCC_USARTCLKSOURCE_LSE        RCC_CCIPR_USART1SEL	/* LSE selected as USART clock */

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
 *  USART Word Length
 */
#define USART_WordLegth_7b      USART_CR1_M1	/* 7-bit long UART frame */
#define USART_WordLength_8b     0x00000000		/* 8-bit long UART frame */
#define USART_WordLength_9b     USART_CR1_M0	/* 9-bit long UART frame */

/*
 *  USART Stop Bits
 */
#define USART_StopBits_0_5      USART_CR2_STOP_0
#define USART_StopBits_1        0x00000000
#define USART_StopBits_1_5      (USART_CR2_STOP_0 | USART_CR2_STOP_1)
#define USART_StopBits_2        USART_CR2_STOP_1

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

/*
 * USART HARDWARE FLOW制御
 */
#define USART_HardwareFlowControl_None       0x00000000
#define USART_HardwareFlowControl_RTS        USART_CR3_RTSE
#define USART_HardwareFlowControl_CTS        USART_CR3_CTSE
#define USART_HardwareFlowControl_RTS_CTS    (USART_CR3_RTSE | USART_CR3_CTSE)

#define UART_CLOCKPRESCALER     0x00000000
#define UART_CLOCK_DIV_FACTOR   1

/*
 *  USARTクロックプレスケール設定値
 */
#define UART_PRESCALER_DIV1     0x00000000
#define UART_PRESCALER_DIV2     (USART_PRESC_PRESCALER_0)
#define UART_PRESCALER_DIV4     (USART_PRESC_PRESCALER_1)
#define UART_PRESCALER_DIV6     (USART_PRESC_PRESCALER_0 | USART_PRESC_PRESCALER_1)
#define UART_PRESCALER_DIV8     (USART_PRESC_PRESCALER_2)
#define UART_PRESCALER_DIV10    (USART_PRESC_PRESCALER_0 | USART_PRESC_PRESCALER_2)
#define UART_PRESCALER_DIV12    (USART_PRESC_PRESCALER_1 | USART_PRESC_PRESCALER_2)
#define UART_PRESCALER_DIV16    (USART_PRESC_PRESCALER_0 | USART_PRESC_PRESCALER_1 | USART_PRESC_PRESCALER_2)
#define UART_PRESCALER_DIV32    (USART_PRESC_PRESCALER_3)
#define UART_PRESCALER_DIV64    (USART_PRESC_PRESCALER_0 | USART_PRESC_PRESCALER_3)
#define UART_PRESCALER_DIV128   (USART_PRESC_PRESCALER_1 | USART_PRESC_PRESCALER_3)
#define UART_PRESCALER_DIV256   (USART_PRESC_PRESCALER_0 | USART_PRESC_PRESCALER_1 | USART_PRESC_PRESCALER_3)

/*
 *  RXFIFOスレッシュホールド設定値
 */
#define UART_TXFIFO_THRESHOLD_1_8   0x00000000									/* TXFIFO reaches 1/8 of its depth */
#define UART_TXFIFO_THRESHOLD_1_4   USART_CR3_TXFTCFG_0							/* TXFIFO reaches 1/4 of its depth */
#define UART_TXFIFO_THRESHOLD_1_2   USART_CR3_TXFTCFG_1							/* TXFIFO reaches 1/2 of its depth */
#define UART_TXFIFO_THRESHOLD_3_4   (USART_CR3_TXFTCFG_0|USART_CR3_TXFTCFG_1)	/* TXFIFO reaches 3/4 of its depth */
#define UART_TXFIFO_THRESHOLD_7_8   USART_CR3_TXFTCFG_2							/* TXFIFO reaches 7/8 of its depth */
#define UART_TXFIFO_THRESHOLD_8_8   (USART_CR3_TXFTCFG_2|USART_CR3_TXFTCFG_0)	/* TXFIFO becomes empty */

/*
 *  RXFIFOスレッシュホールド設定値
 */
#define UART_RXFIFO_THRESHOLD_1_8   0x00000000									/* RXFIFO FIFO reaches 1/8 of its depth */
#define UART_RXFIFO_THRESHOLD_1_4   USART_CR3_RXFTCFG_0							/* RXFIFO FIFO reaches 1/4 of its depth */
#define UART_RXFIFO_THRESHOLD_1_2   USART_CR3_RXFTCFG_1							/* RXFIFO FIFO reaches 1/2 of its depth */
#define UART_RXFIFO_THRESHOLD_3_4   (USART_CR3_RXFTCFG_0|USART_CR3_RXFTCFG_1)	/* RXFIFO FIFO reaches 3/4 of its depth */
#define UART_RXFIFO_THRESHOLD_7_8   USART_CR3_RXFTCFG_2							/* RXFIFO FIFO reaches 7/8 of its depth */
#define UART_RXFIFO_THRESHOLD_8_8   (USART_CR3_RXFTCFG_2|USART_CR3_RXFTCFG_0)	/* RXFIFO FIFO becomes full */

#define CR1_CLEAR_MASK     (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE)
#define CR3_CLEAR_MASK     (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT)

#define LPUART_BRR_MIN     0x00000300U  /* LPUART BRR minimum authorized value */
#define LPUART_BRR_MAX     0x000FFFFFU  /* LPUART BRR maximum authorized value */


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
	UW resetbase;
	UW reset_bit;
	UH gpio_af;
	UH srcindex;

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
	{0x40008000, (INTNO)INTNO_SIO1, 7,	/* LPUART1 */
	 (UW)0x4002105C, (UW)0x00000001,	/* RCC_APB1ENR2/RCC_APB1ENR2_LPUART1EN */
     (UW)0x4002104C, (UW)0x00000001,	/* RCC_AHB2ENR/RCC_AHB2ENR_GPIOAEN */
	 (UW)0x4002103C, (UW)0x00000001,	/* RCC_APB1RSTR2/RCC_APB1RSTR2_LPUART1RST */
	 (UH)0x0CU, (UH)10,					/* AF12 */
     (UW)0x48000000, (UW)2,				/* GPIOA_BASE/TX */
     (UW)0x48000000, (UW)3,				/* GPIOA_BASE/RX */
#if TNUM_SIOP >= 2
    },
	{0x40013800, (INTNO)INTNO_SIO2, 7,	/* USART1 */
	 (UW)0x40021060, (UW)0x00004000,	/* RCC_APB2ENR/RCC_APB2ENR_USART1EN */
     (UW)0x4002104C, (UW)0x00000004,	/* RCC_AHB2ENR/RCC_AHB1ENR_GPIOCEN */
	 (UW)0x40021040, (UW)0x00004000,	/* RCC_APB2RSTR/RCC_APB2RSTR_USART1RST */
	 (UH)0x07U, (UH)0,					/* GPIO_AF7 */
     (UW)0x48000800, (UW)4,				/* GPIOC_BASE/TX */
     (UW)0x48000800, (UW)5,				/* GPIOC_BASE/RX */
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
	UW	regoff = TOFF_GPIO_AFR0+((source>>1) & 0x4);
	UW	tmpreg = (sil_rew_mem((VP)(base+regoff)) & ~(0xF << ((source & 0x07) * 4)))
			    | (select << ((source & 0x07) * 4));
	sil_wrw_mem((VP)(base+regoff), tmpreg);
}

void put_hex(char a, int val)
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
	const SIOPINIB	*siopinib;
	BOOL	opnflg;
	ER		ercd;
	UW	base, txbase, rxbase, txpin, rxpin;
	UW	csource, tmp;

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
	sil_andw_mem((VP)(siopinib->resetbase), siopinib->reset_bit);

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

	sil_andw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_UE);
	sil_modw_mem((VP)(base+TOFF_USART_CR1), CR1_CLEAR_MASK, (USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx));
	sil_modw_mem((VP)(base+TOFF_USART_CR2), USART_CR2_STOP, USART_StopBits_1);
	sil_modw_mem((VP)(base+TOFF_USART_CR3), CR3_CLEAR_MASK, USART_HardwareFlowControl_None);
	sil_modw_mem((VP)(base+TOFF_USART_PRESC), USART_PRESC_PRESCALER, UART_PRESCALER_DIV1);

	/*
	 *  USART BAUD設定
	 */
	csource = (sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CCIPR)) >> siopinib->srcindex) & 3;
	switch(csource){
	case RCC_USARTCLKSOURCE_SYSCLK:
		tmp = SystemFrequency;
		break;
	case RCC_USARTCLKSOURCE_HSI:
		tmp = HSI_VALUE;
		break;
	case RCC_USARTCLKSOURCE_LSE:
		tmp = LSE_VALUE;
		break;
	default:
		if(base == TADR_USART1_BASE || base == TADR_UART5_BASE)
			tmp = SysFrePCLK2;
		else
			tmp = SysFrePCLK1;
		break;
	}
	if(base == TADR_LPUART1_BASE){
		UW	usartdiv = (((UD)(tmp/UART_CLOCK_DIV_FACTOR)) * 256 + (DEF_BAUDRATE/2)) / DEF_BAUDRATE;
		/*
		 *  Divider 上限、下限補正、補正が入った場合、正しく動作しない可能性がある
		 */
        if(usartdiv < LPUART_BRR_MIN)
			usartdiv = LPUART_BRR_MIN;
		else if(usartdiv > LPUART_BRR_MAX)
			usartdiv = LPUART_BRR_MAX;
		tmp = usartdiv;
	}
	else if((sil_rew_mem((VP)(base+TOFF_USART_CR1)) & USART_CR1_OVER8) != 0){
		UW	usartdiv = (((tmp*2)/UART_CLOCK_DIV_FACTOR) + (DEF_BAUDRATE/2)) / DEF_BAUDRATE;
		tmp = usartdiv & 0xFFF0U;
		tmp |= (UH)((usartdiv & 0x0000000F) >> 1);
	}
	else{
		tmp = ((tmp/UART_CLOCK_DIV_FACTOR) + (DEF_BAUDRATE/2)) / DEF_BAUDRATE;
	}
	sil_wrw_mem((VP)(base+TOFF_USART_BRR), tmp);

	/*
	 *  FIFO設定処理
	 */
	sil_modw_mem((VP)(base+TOFF_USART_CR3), USART_CR3_TXFTCFG, UART_TXFIFO_THRESHOLD_1_8);
#ifdef USE_USART_FIFO
	sil_modw_mem((VP)(base+TOFF_USART_CR3), USART_CR3_RXFTCFG, UART_RXFIFO_THRESHOLD_8_8);
	sil_orw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_FIFOEN);
#else
	sil_modw_mem((VP)(base+TOFF_USART_CR3), USART_CR3_RXFTCFG, UART_RXFIFO_THRESHOLD_1_8);
	sil_andw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_FIFOEN);
#endif
	sil_orw_mem((VP)(base+TOFF_USART_CR1), (USART_CR1_PEIE | USART_CR1_RXNEIE));
	siopcb->opnflg = TRUE;

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		x_config_int(siopinib->intno_usart, TRUE, siopinib->intpri);
	}
	sil_orw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_UE);

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
		dis_int(siopcb->siopinib->intno_usart);
		sil_andw_mem((VP)(siopinib->base+TOFF_USART_CR1), USART_CR1_UE);
		sil_andw_mem((VP)(siopinib->clockbase), siopinib->clock_set);
	}
	siopcb->opnflg = FALSE;
}

/*
 *  SIOの割込みサービスルーチン
 */

Inline BOOL
sio_putiready(SIOPCB* siopcb)
{
	UW	cr1 = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1));
	UW	isr = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_ISR));
	UW	cr3 = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR3));

	if (((cr1 & USART_CR1_TXEIE) != 0 && (isr & USART_ISR_TC) != 0) || (cr3 & USART_CR3_TXFTIE) != 0) {
		return 1;
	}
	return 0;
}

Inline BOOL
sio_getiready(SIOPCB* siopcb)
{
	UW	cr1 = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1));
	UW	isr = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_ISR));
	UW	cr3 = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR3));
	UH	temp;

	if ((isr & (USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE)) != 0) {
		temp = 0;
		sil_dly_nse(100*1000);
		syslog(LOG_INFO, "sio_usart_isr error (%d) isr[%04x] !", siopcb->siopinib->intno_usart, isr);
		sil_andw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR3), USART_CR3_EIE);
		temp = sil_reh_mem((VP)(siopcb->siopinib->base+TOFF_USART_RDR));
		(void)(temp);
		return 0;
	}
	if (((cr1 & USART_CR1_RXNEIE) != 0 && (isr & USART_ISR_RXNE) != 0) || (cr3 & USART_CR3_RXFTIE) != 0) {
		return 1;
	}
	return 0;
}

/*
 *  シリアルI/Oポート出力判定
 */
Inline BOOL
sio_putready(SIOPCB* siopcb)
{
#ifdef USE_USART_FIFO
	while ((sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_ISR)) & USART_ISR_TXE_TXFNF) == 0);
#else
	while ((sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_ISR)) & USART_ISR_TC) == 0);
#endif
	return 1;
}

Inline BOOL
sio_getready(SIOPCB* siopcb)
{
	UW	cr1 = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1));
	UW	cr3 = sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR3));

	if ((cr1 & USART_CR1_RXNEIE) != 0 || (cr3 & USART_CR3_RXFTIE) != 0) {
		return 1;
	}
	return 0;
}

void
sio_handler1(void)
{
	SIOPCB	*siopcb = get_siopcb(1);

	if (sio_getiready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putiready(siopcb)) {
		sio_ierdy_snd(siopcb->exinf);
	}
}

#if TNUM_SIOP >= 2
void
sio_handler2(void)
{
	SIOPCB	*siopcb = get_siopcb(2);

	if (sio_getiready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putiready(siopcb)) {
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
		sil_wrw_mem((VP)(siopcb->siopinib->base+TOFF_USART_TDR), (UW)c);
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
		c =  sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_RDR)) & 0xFF;
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
#ifdef USE_USART_FIFO
		sil_orw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR3), USART_CR3_TXFTIE);
#else
		sil_orw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_TXEIE);
#endif
		break;
	case SIO_ERDY_RCV:
		sil_orw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_RXNEIE);
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
		sil_andw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_TXEIE);
		sil_andw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR3), USART_CR3_TXFTIE);
		break;
	case SIO_ERDY_RCV:
		sil_andw_mem((VP)(siopcb->siopinib->base+TOFF_USART_CR1), USART_CR1_RXNEIE);
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

	sil_wrw_mem((VP)(base+TOFF_USART_TDR), (UW)c);
#ifdef USE_USART_FIFO
	while ((sil_rew_mem((VP)(base+TOFF_USART_ISR)) & USART_ISR_TXE_TXFNF) == 0);
#else
	while ((sil_rew_mem((VP)(base+TOFF_USART_ISR)) & USART_ISR_TC) == 0);
#endif

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = SystemFrequency/DEF_BAUDRATE;
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
	UW	csource, tmp;

	txbase = siopinib->txportbase;
	rxbase = siopinib->rxportbase;
	txpin  = siopinib->txpin;
	rxpin  = siopinib->rxpin;
	base = siopinib->base;

	sil_andw_mem((VP)(siopinib->resetbase), siopinib->reset_bit);

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

	sil_andw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_UE);
	sil_modw_mem((VP)(base+TOFF_USART_CR1), CR1_CLEAR_MASK, (USART_WordLength_8b | USART_Parity_No | USART_Mode_Rx | USART_Mode_Tx));
	sil_modw_mem((VP)(base+TOFF_USART_CR2), USART_CR2_STOP, USART_StopBits_1);
	sil_modw_mem((VP)(base+TOFF_USART_CR3), CR3_CLEAR_MASK, USART_HardwareFlowControl_None);
	sil_modw_mem((VP)(base+TOFF_USART_PRESC), USART_PRESC_PRESCALER, UART_PRESCALER_DIV1);

	/*
	 *  USART BAUD設定
	 */
	csource = (sil_rew_mem((VP)(TADR_RCC_BASE+TOFF_RCC_CCIPR)) >> siopinib->srcindex) & 3;
	switch(csource){
	case RCC_USARTCLKSOURCE_SYSCLK:
		tmp = SystemFrequency;
		break;
	case RCC_USARTCLKSOURCE_HSI:
		tmp = HSI_VALUE;
		break;
	case RCC_USARTCLKSOURCE_LSE:
		tmp = LSE_VALUE;
		break;
	default:
		if(base == TADR_USART1_BASE || base == TADR_UART5_BASE)
			tmp = SysFrePCLK2;
		else
			tmp = SysFrePCLK1;
		break;
	}
	if(base == TADR_LPUART1_BASE){
		UW	usartdiv = (((UD)(tmp/UART_CLOCK_DIV_FACTOR)) * 256 + (DEF_BAUDRATE/2)) / DEF_BAUDRATE;
		/*
		 *  Divider 上限、下限補正、補正が入った場合、正しく動作しない可能性がある
		 */
        if(usartdiv < LPUART_BRR_MIN)
			usartdiv = LPUART_BRR_MIN;
		else if(usartdiv > LPUART_BRR_MAX)
			usartdiv = LPUART_BRR_MAX;
		tmp = usartdiv;
	}
	else if((sil_rew_mem((VP)(base+TOFF_USART_CR1)) & USART_CR1_OVER8) != 0){
		UW	usartdiv = (((tmp*2)/UART_CLOCK_DIV_FACTOR) + (DEF_BAUDRATE/2)) / DEF_BAUDRATE;
		tmp = usartdiv & 0xFFF0U;
		tmp |= (UH)((usartdiv & 0x0000000F) >> 1);
	}
	else{
		tmp = ((tmp/UART_CLOCK_DIV_FACTOR) + (DEF_BAUDRATE/2)) / DEF_BAUDRATE;
	}
	sil_wrw_mem((VP)(base+TOFF_USART_BRR), tmp);

	/*
	 *  FIFO設定処理
	 */
	sil_modw_mem((VP)(base+TOFF_USART_CR3), USART_CR3_TXFTCFG, UART_TXFIFO_THRESHOLD_1_8);
#ifdef USE_USART_FIFO
	sil_modw_mem((VP)(base+TOFF_USART_CR3), USART_CR3_RXFTCFG, UART_RXFIFO_THRESHOLD_8_8);
	sil_orw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_FIFOEN);
#else
	sil_modw_mem((VP)(base+TOFF_USART_CR3), USART_CR3_RXFTCFG, UART_RXFIFO_THRESHOLD_1_8);
	sil_andw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_FIFOEN);
#endif
	sil_andw_mem((VP)(base+TOFF_USART_CR1), (USART_CR1_TXEIE | USART_CR1_PEIE | USART_CR1_RXNEIE));
	sil_orw_mem((VP)(base+TOFF_USART_CR1), USART_CR1_UE);
}
