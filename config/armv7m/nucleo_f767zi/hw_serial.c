/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: hw_serial.c 2246 2026-03-20 21:39:12Z roi $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（NUCLEO-F767ZI用）
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

#define TOFF_USART_CR1      0x0000		/* (RW) USART Control register 1 */
  #define USART_CR1_UE       0x00000001	/* USART Enable */
  #define USART_CR1_RE       0x00000004	/* Receiver Enable */
  #define USART_CR1_TE       0x00000008	/* Transmitter Enable */
  #define USART_CR1_RXNEIE   0x00000020	/* RXNE Interrupt Enable */
  #define USART_CR1_TXEIE    0x00000080	/* TXE Interrupt Enable */
  #define USART_CR1_PEIE     0x00000100	/* PE Interrupt Enable */
  #define USART_CR1_PS       0x00000200	/* Parity Selection */
  #define USART_CR1_PCE      0x00000400	/* Parity Control Enable */
  #define USART_CR1_M        0x10001000	/* Word length */
  #define USART_CR1_OVER8    0x00008000	/* Oversampling by 8-bit or 16-bit mode */

#define TOFF_USART_CR2      0x0004		/* (RW) USART Control register 2 */
  #define USART_CR2_CLKEN    0x00000800	/* Clock Enable */
  #define USART_CR2_STOP     0x00003000	/* STOP[1:0] bits (STOP bits) */
  #define USART_CR2_LINEN    0x00004000	/* LIN mode enable */

#define TOFF_USART_CR3      0x0008		/* (RW) USART Control register 3 */
  #define USART_CR3_EIE      0x00000001	/* Error Interrupt Enable */
  #define USART_CR3_IREN     0x00000002	/* IrDA mode Enable */
  #define USART_CR3_HDSEL    0x00000008	/* Half-Duplex Selection */
  #define USART_CR3_SCEN     0x00000020	/* SmartCard mode enable */
  #define USART_CR3_RTSE     0x00000100	/* RTS Enable */
  #define USART_CR3_CTSE     0x00000200	/* CTS Enable */
  #define USART_CR3_ONEBIT   0x00000800	/* One sample bit method enable */
#define TOFF_USART_BRR      0x000C		/* (RW) USART Baud rate register */

#define TOFF_USART_ISR      0x001C		/* (RW) USART Interrupt and status register */
  #define USART_ISR_PE       0x00000001	/* Parity Error */
  #define USART_ISR_FE       0x00000002	/* Framing Error */
  #define USART_ISR_NE       0x00000004	/* Noise detected Flag */
  #define USART_ISR_ORE      0x00000008	/* OverRun Error */
  #define USART_ISR_TXE      0x00000080	/* Transmit Data Register Empty */
  #define USART_ISR_RXNE     0x00000020	/* Read Data Register Not Empty */
  #define USART_ISR_TEACK    0x00200000	/* Transmit Enable Acknowledge Flag */
  #define USART_ISR_REACK    0x00400000	/* Receive Enable Acknowledge Flag */
#define TOFF_USART_ICR      0x0020		/* (RW) USART Interrupt flag Clear register */
  #define USART_ICR_PECF     0x00000001	/* Parity Error Clear Flag */
  #define USART_ICR_FECF     0x00000002	/* Framing Error Clear Flag */
  #define USART_ICR_NCF      0x00000004	/* Noise detected Clear Flag */
  #define USART_ICR_ORECF    0x00000008	/* OverRun Error Clear Flag */
#define TOFF_USART_RDR      0x0024		/* (RW) USART Receive Data register */
#define TOFF_USART_TDR      0x0028		/* (RW) USART Transmit Data register */


#define TOFF_GPIO_MODER     0x0000		/* (RW) GPIO port mode register */
  #define GPIO_MODER_MODER0  0x00000003
#define TOFF_GPIO_OTYPER    0x0004		/* (RW) GPIO port output type register */
  #define GPIO_OTYPER_OT     0x00000001
#define TOFF_GPIO_OSPEEDR   0x0008		/* (RW) GPIO port output speed register */
  #define GPIO_OSPEEDER_OSPEEDR0 0x00000003
#define TOFF_GPIO_PUPDR     0x000C		/* (RW) GPIO port pull-up/pull-down register */
  #define GPIO_PUPDR_PUPDR0  0x00000003
#define TOFF_GPIO_AFR0      0x0020		/* (RW) GPIO alternate function registers */


#define TADR_RCC_DCKCFGR2   0x40023890	/* (RW) RCC Dedicated Clocks configuration register2 */
  #define RCC_DCKCFGR2_USART1SEL   0x00000003
  #define RCC_DCKCFGR2_USART1SEL_0 0x00000001
  #define RCC_DCKCFGR2_USART6SEL   0x00000C00
  #define RCC_DCKCFGR2_USART6SEL_0 0x00000400


/*
 *  GPIOモードパラメータ
 */
#define GPIO_MODE_INPUT     0x00000000	/* Input Floating Mode */
#define GPIO_MODE_OUTPUT    0x00000001	/* Output Mode */
#define GPIO_MODE_AF        0x00000002	/* Alternate Function Mode */
#define GPIO_MODE_ANALOG    0x00000003	/* Analog Mode  */

/*
 *  GPIOアウトプット設定パラメータ
 */
#define GPIO_OTYPE_PP       0x0
#define GPIO_OTYPE_OD       0x1

/*
 *  GPIOアウトプット最大周波数パラメータ
 */
#define GPIO_SPEED_LOW      0x00000000	/* Low speed     */
#define GPIO_SPEED_MEDIUM   0x00000001	/* Medium speed  */
#define GPIO_SPEED_FAST     0x00000002	/* Fast speed    */
#define GPIO_SPEED_HIGH     0x00000003	/* High speed    */

/*
 *  GPIOプルアップダウンパラメータ
 */
#define GPIO_NOPULL         0x00000000	/* No Pull-up or Pull-down activation  */
#define GPIO_PULLUP         0x00000001	/* Pull-up activation                  */
#define GPIO_PULLDOWN       0x00000002	/* Pull-down activation                */

/*
 *  CLOCKソース
 */
#define SOURCE_PCLK2        0
#define SOURCE_SYSCLK       1
#define SOURCE_HSI          2
#define SOURCE_LSE          3

/*
 *  ビット長パラメータ
 */
#define USART_WordLength_8b 0x0000
#define USART_WordLength_9b USART_CR1_M

/*
 *  ストップビットパラメータ
 */ 
#define USART_StopBits_1    0x0000
#define USART_StopBits_0_5  USART_CR2_STOP_0
#define USART_StopBits_2    USART_CR2_STOP_1
#define USART_StopBits_1_5  USART_CR2_STOP

/*
 *  パリティパラメータ
 */
#define USART_Parity_No     0x0000
#define USART_Parity_Even   USART_CR1_PCE
#define USART_Parity_Odd    (USART_CR1_PCE | USART_CR1_PS) 

/*
 * ハードウェアフロー制御パラメータ
 */
#define USART_HardwareFlowControl_None       0x0000
#define USART_HardwareFlowControl_RTS        USART_CR3_RTSE
#define USART_HardwareFlowControl_CTS        USART_CR3_CTSE
#define USART_HardwareFlowControl_RTS_CTS    (USART_CR3_RTSE | USART_CR3_CTSE)

#define ISR_ACK             (USART_ISR_TEACK | USART_ISR_REACK)
#define CR1_CLEAR_MASK      (USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE | USART_CR1_OVER8)
#define CR3_CLEAR_MASK      (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT)

#define UART_TIMEOUT_VALUE               0x1FFFFFF


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
	{0x40004800, (INTNO)INTNO_SIO1, 7,	/* USART3_BASE */
	 (UW)0x40023840, (UW)0x00040000,	/* RCC_APB1ENR/RCC_APB1ENR_USART3EN */
     (UW)0x40023830, (UW)0x00000008,	/* RCC_AHB1ENR/RCC_AHB1ENR_GPIODEN */
	 (UW)0x40023820, (UW)0x00040000,	/* RCC_APB1RSTR/RCC_APB1RSTR_USART3RST */
	 (UH)0x07U, (UH)4,					/* AF07 */
     (UW)0x40020C00, (UW)8,				/* GPIOD_BASE/TX */
     (UW)0x40020C00, (UW)9,				/* GPIOD_BASE/RX */
#if TNUM_SIOP >= 2
    },
	{0x40011400, (INTNO)INTNO_SIO2, 7,	/* USART6_BASE */
	 (UW)0x40023844, (UW)0x00004000,	/* RCC_APB2ENR/RCC_APB2ENR_USART6EN */
     (UW)0x40023830, (UW)0x00000040,	/* RCC_AHB2ENR/RCC_AHB1ENR_GPIOGEN */
	 (UW)0x40023824, (UW)0x00000020,	/* RCC_APB2RSTR/RCC_APB2RSTR_USART6RST */
	 (UH)0x08U, (UH)10,					/* AF8 */
     (UW)0x40021800, (UW)14,			/* GPIOG_BASE/TX */
     (UW)0x40021800, (UW)9,				/* GPIOG_BASE/RX */
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
	UW	regoff = TOFF_GPIO_AFR0+((source>>3) * 4);
	UW	tmpreg = (sil_rew_mem((VP)(base+regoff)) & ~(0xF << ((source & 0x07) * 4)))
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
	SIOPCB          *siopcb;
	const SIOPINIB  *siopinib;
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

	if(siopid == DEFAULT_PORT)
		sil_dly_nse(10000);

	/*
	 *  USART停止
	 */
	sil_wrw_mem((VP)(base+TOFF_USART_ICR), 0xFFFFFFFF);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~USART_CR1_UE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), 0x0);
	sil_wrw_mem((VP)(base+TOFF_USART_CR2), 0x0);
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), 0x0);
	tmp = sil_rew_mem((VP)siopinib->resetbase);
	sil_wrw_mem((VP)siopinib->resetbase, tmp | siopinib->reset_bit);
	sil_wrw_mem((VP)siopinib->resetbase, tmp & ~(siopinib->reset_bit));

	/*
	 *  ハードウェアの初期化
	 */
	if(siopinib->srcindex == 0){	/* USART1の場合、SYSLOCKを設定 */
		sil_wrw_mem((VP)(TADR_RCC_DCKCFGR2),
			(sil_rew_mem((VP)(TADR_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART1SEL) | RCC_DCKCFGR2_USART1SEL_0);
	}
	else if(siopinib->srcindex == 10 && siopid == 1){	/* F723/USART6の場合、SYSLOCKを設定 */
		sil_wrw_mem((VP)(TADR_RCC_DCKCFGR2),
			(sil_rew_mem((VP)(TADR_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART6SEL) | RCC_DCKCFGR2_USART6SEL_0);
	}
	sil_orw_mem((VP)(siopinib->clockbase), siopinib->clock_set);
	sil_orw_mem((VP)(siopinib->portbase), siopinib->port_set);
	sil_modw_mem((VP)(txbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER0 << (txpin*2)), (GPIO_MODE_AF << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR0 << (txpin*2)), (GPIO_SPEED_HIGH << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << txpin),  (GPIO_MODE_AF << txpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR0 << (txpin*2)), (GPIO_PULLUP << (txpin*2)));
	sil_modw_mem((VP)(rxbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER0 << (rxpin*2)), (GPIO_MODE_AF << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR0 << (rxpin*2)), (GPIO_SPEED_HIGH << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << rxpin),  (GPIO_MODE_AF << rxpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR0 << (rxpin*2)), (GPIO_PULLUP << (rxpin*2)));
	setup_gpio_source(txbase, txpin, siopinib->gpio_af);
	setup_gpio_source(rxbase, rxpin, siopinib->gpio_af);

	tmp = sil_rew_mem((VP)(base+TOFF_USART_CR2)) & ~USART_CR2_STOP;
	sil_wrw_mem((VP)(base+TOFF_USART_CR2), tmp | USART_StopBits_1);
	tmp = sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~CR1_CLEAR_MASK;
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), tmp | USART_WordLength_8b | USART_Parity_No | USART_CR1_RE | USART_CR1_TE);
	tmp = sil_rew_mem((VP)(base+TOFF_USART_CR3)) & ~CR3_CLEAR_MASK;
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), tmp | USART_HardwareFlowControl_None);

	switch((sil_rew_mem((VP)(TADR_RCC_DCKCFGR2)) >> siopinib->srcindex) & 3){
	case SOURCE_HSI:
		tmp = HSI_VALUE;
		break;
	case SOURCE_SYSCLK:
		tmp = SysFreHCLK;
		break;
	case SOURCE_LSE:
		tmp = LSE_VALUE;
		break;
	case SOURCE_PCLK2:
	default:
		if(siopinib->srcindex == 10)	/* USART6,1の場合 PCLK2 */
			tmp = SysFrePCLK2;
		else
			tmp = SysFrePCLK1;
		break;
	}
	sil_wrw_mem((VP)(base+TOFF_USART_BRR), tmp/DEF_BAUDRATE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR2), sil_rew_mem((VP)(base+TOFF_USART_CR2)) & ~(USART_CR2_LINEN | USART_CR2_CLKEN));
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), sil_rew_mem((VP)(base+TOFF_USART_CR3)) & ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) | USART_CR1_UE);

	/*
	 * RX/TX ACKがセットされるまで待つ
	 */
	tmp = 0;
    while((sil_rew_mem((VP)(base+TOFF_USART_ISR)) & ISR_ACK) != ISR_ACK){
		/* Check for the Timeout */
		if(tmp >=  UART_TIMEOUT_VALUE){
			/* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
			sil_wrw_mem((VP)(base+TOFF_USART_CR1),
				sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~(USART_CR1_TXEIE | USART_CR1_RXNEIE | USART_CR1_PEIE));
			sil_wrw_mem((VP)(base+TOFF_USART_CR3),
				sil_rew_mem((VP)(base+TOFF_USART_CR3)) & ~USART_CR3_EIE);
			goto sio_opn_exit;
		}
		sil_dly_nse(1000);
		tmp++;
	}

	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) | USART_CR1_PEIE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), sil_rew_mem((VP)(base+TOFF_USART_CR3)) | USART_CR3_EIE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) | USART_CR1_RXNEIE);
	siopcb->opnflg = TRUE;

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		x_config_int(siopinib->intno_usart, TRUE, siopinib->intpri);
	}

sio_opn_exit:;
	return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;
	UW	base, tmp;

	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	if ((siopcb->opnflg)) {
		dis_int(siopinib->intno_usart);
	}

	/*
	 *  ハードウェアのリセット
	 */
	base = siopinib->base;
	sil_wrw_mem((VP)(base+TOFF_USART_ICR), 0xFFFFFFFF);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~USART_CR1_UE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), 0x0);
	sil_wrw_mem((VP)(base+TOFF_USART_CR2), 0x0);
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), 0x0);
	tmp = sil_rew_mem((VP)siopinib->resetbase);
	sil_wrw_mem((VP)siopinib->resetbase, tmp | siopinib->reset_bit);
	sil_wrw_mem((VP)siopinib->resetbase, tmp & ~(siopinib->reset_bit));
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

	if ((cr1 & USART_CR1_TXEIE) != 0 && (isr & USART_ISR_TXE) != 0) {
		return 1;
	}
	return 0;
}

Inline BOOL
sio_getiready(SIOPCB* siopcb)
{
	UW	base = siopcb->siopinib->base;
	UW	cr1, cr3, isr;

	cr1 = sil_rew_mem((VP)(base+TOFF_USART_CR1));
	cr3 = sil_rew_mem((VP)(base+TOFF_USART_CR3));
	isr = sil_rew_mem((VP)(base+TOFF_USART_ISR));

	if ((isr & USART_ISR_PE) != 0 && (cr3 & USART_CR3_EIE) != 0) {
		sil_wrw_mem((VP)(base+TOFF_USART_ICR), USART_ICR_PECF);
	}
	if ((isr & USART_ISR_FE) != 0 && (cr3 & USART_CR3_EIE) != 0) {
		sil_wrw_mem((VP)(base+TOFF_USART_ICR), USART_ICR_FECF);
	}
	if ((isr & USART_ISR_NE) != 0 && (cr3 & USART_CR3_EIE) != 0) {
		sil_wrw_mem((VP)(base+TOFF_USART_ICR), USART_ICR_NCF);
	}
	if ((isr & USART_ISR_ORE) != 0 && (cr3 & USART_CR3_EIE) != 0) {
		sil_wrw_mem((VP)(base+TOFF_USART_ICR), USART_ICR_ORECF);
	}
	if ((cr1 & USART_CR1_RXNEIE) != 0 && (isr & USART_ISR_RXNE) != 0)
	{
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
	if ((sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_ISR)) & USART_ISR_TXE) != 0) {
		return 1;
	}
	return 0;
}

Inline BOOL
sio_getready(SIOPCB* siopcb)
{
	if ((sil_rew_mem((VP)(siopcb->siopinib->base+TOFF_USART_ISR)) & USART_ISR_RXNE) != 0) {
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
	INT	c = -1;

	if (sio_getready(siopcb)) {
		c = sil_rew_mem((VP)((siopcb->siopinib->base)+TOFF_USART_RDR)) & 0xFF;
	}
	return c;
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	UW	base = siopcb->siopinib->base;

	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) | USART_CR1_TXEIE);
		break;
	case SIO_ERDY_RCV:
		sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) | USART_CR1_RXNEIE);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	UW	base = siopcb->siopinib->base;

	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~USART_CR1_TXEIE);
		break;
	case SIO_ERDY_RCV:
		sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~USART_CR1_RXNEIE);
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
	UW	base = siopinib->base;

	sil_wrw_mem((VP)(base+TOFF_USART_TDR), (UW)c);
	while(0 == (sil_rew_mem((VP)(base+TOFF_USART_ISR)) & USART_ISR_TXE));

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
	ER		ercd;
	UW	base, txbase, rxbase, txpin, rxpin;
	UW	csource, tmp;

	txbase = siopinib->txportbase;
	rxbase = siopinib->rxportbase;
	txpin  = siopinib->txpin;
	rxpin  = siopinib->rxpin;
	base = siopinib->base;

	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~USART_CR1_UE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), 0x0);
	sil_wrw_mem((VP)(base+TOFF_USART_CR2), 0x0);
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), 0x0);
	tmp = sil_rew_mem((VP)siopinib->resetbase);
	sil_wrw_mem((VP)siopinib->resetbase, tmp | siopinib->reset_bit);
	sil_wrw_mem((VP)siopinib->resetbase, tmp & ~(siopinib->reset_bit));

	if(siopinib->srcindex == 0){	/* USART1の場合、SYSLOCKを設定 */
		sil_wrw_mem((VP)(TADR_RCC_DCKCFGR2),
			(sil_rew_mem((VP)(TADR_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART1SEL) | RCC_DCKCFGR2_USART1SEL_0);
	}
	else if(siopinib->srcindex == 10){	/* F723/USART6の場合、SYSLOCKを設定 */
		sil_wrw_mem((VP)(TADR_RCC_DCKCFGR2),
			(sil_rew_mem((VP)(TADR_RCC_DCKCFGR2)) & RCC_DCKCFGR2_USART6SEL) | RCC_DCKCFGR2_USART6SEL_0);
	}

	sil_orw_mem((VP)(siopinib->clockbase), siopinib->clock_set);
	sil_orw_mem((VP)(siopinib->portbase), siopinib->port_set);
	sil_modw_mem((VP)(txbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER0 << (txpin*2)), (GPIO_MODE_AF << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR0 << (txpin*2)), (GPIO_SPEED_HIGH << (txpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << txpin),  (GPIO_MODE_AF << txpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR0 << (txpin*2)), (GPIO_PULLUP << (txpin*2)));
	sil_modw_mem((VP)(rxbase+TOFF_GPIO_MODER), (GPIO_MODER_MODER0 << (rxpin*2)), (GPIO_MODE_AF << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OSPEEDR), (GPIO_OSPEEDER_OSPEEDR0 << (rxpin*2)), (GPIO_SPEED_HIGH << (rxpin*2)));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_OTYPER), (GPIO_OTYPER_OT << rxpin),  (GPIO_MODE_AF << rxpin));
	sil_modw_mem((VP)(txbase+TOFF_GPIO_PUPDR), (GPIO_PUPDR_PUPDR0 << (rxpin*2)), (GPIO_PULLUP << (rxpin*2)));
	setup_gpio_source(txbase, txpin, siopinib->gpio_af);
	setup_gpio_source(rxbase, rxpin, siopinib->gpio_af);

	tmp = sil_rew_mem((VP)(base+TOFF_USART_CR2)) & ~USART_CR2_STOP;
	sil_wrw_mem((VP)(base+TOFF_USART_CR2), tmp | USART_StopBits_1);
	tmp = sil_rew_mem((VP)(base+TOFF_USART_CR1)) & ~CR1_CLEAR_MASK;
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), tmp | USART_WordLength_8b | USART_Parity_No | USART_CR1_RE | USART_CR1_TE);
	tmp = sil_rew_mem((VP)(base+TOFF_USART_CR3)) & ~CR3_CLEAR_MASK;
	sil_wrw_mem((VP)(base+TOFF_USART_CR3), tmp | USART_HardwareFlowControl_None);

	switch((sil_rew_mem((VP)(TADR_RCC_DCKCFGR2)) >> siopinib->srcindex) & 3){
	case SOURCE_HSI:
		tmp = HSI_VALUE;
		break;
	case SOURCE_SYSCLK:
		tmp = SysFreHCLK;
		break;
	case SOURCE_LSE:
		tmp = LSE_VALUE;
		break;
	case SOURCE_PCLK2:
	default:
		if(siopinib->srcindex == 10)	/* USART6,1の場合 PCLK2 */
			tmp = SysFrePCLK2;
		else
			tmp = SysFrePCLK1;
		break;
	}
	sil_wrw_mem((VP)(base+TOFF_USART_BRR), tmp/DEF_BAUDRATE);
	sil_wrw_mem((VP)(base+TOFF_USART_CR1), sil_rew_mem((VP)(base+TOFF_USART_CR1)) | USART_CR1_UE);
}

