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
 *  @(#) $Id: hw_serial.c 2246 2025-12-09 12:18:58Z roi $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（OM13058用）
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

/*
 * レジスタ設定値
 */
#if !defined(SYSCTL_IRC_FREQ)
#define SYSCTL_IRC_FREQ (12000000)
#endif /* HSI_VALUE */

#ifdef DEBUG_SIOPORT
#define RX1_PINPORT     0
#define RX1_PINPOS      18
#define TX1_PINPORT     0
#define TX1_PINPOS      19
#else
#define RX1_PINPORT     1
#define RX1_PINPOS      26
#define TX1_PINPORT     1
#define TX1_PINPOS      27
#endif

#define TADR_SYSCTL_USART0CLKDIV	0x40048098	/* (RW) UART clock divider register */

#define IOCON_SIO1      0x00000082		/* (IOCON_FUNC2 | IOCON_MODE_INACT | IOCON_DIGMODE_EN) */

/*
 *  UNIVERSAL SYNCHRONOUS ASYNCHORONOUS RECEIVER TRANSMITTER 0
 */
#define TADR_USART0_BASE    0x40008000
#define TOFF_USART_DLL          0x0000		/* (RW) Divisor Latch LSB. Least significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
#define TOFF_USART_THR          0x0000		/* (W ) Transmit Holding Register. The next character to be transmitted is written here (DLAB = 0). */
#define TOFF_USART_RBR          0x0000		/* (R)  Receiver Buffer Register. Contains the next received character to be read (DLAB = 0). */
  #define USART0_RBR_MASKBIT        0x000000FF	/* UART Received Buffer mask bit (8 bits) */
#define TOFF_USART_IER          0x0004		/* (RW) Interrupt Enable Register. Contains individual interrupt enable bits for the 7 potential UART interrupts (DLAB = 0). */
  #define USART0_IER_RBRINT         0x00000001	/* RBR Interrupt enable */
  #define USART0_IER_THREINT        0x00000002	/* THR Interrupt enable */
  #define USART0_IER_RLSINT         0x00000004	/* RX line status interrupt enable */
  #define USART0_IER_MSINT          0x00000008	/* Modem status interrupt enable - valid for 11xx, 17xx/40xx UART1, 18xx/43xx UART1  only */
  #define USART0_IER_CTSINT         0x00000080	/* CTS signal transition interrupt enable - valid for 17xx/40xx UART1, 18xx/43xx UART1 only */
  #define USART0_IER_ABEOINT        0x00000100	/* Enables the end of auto-baud interrupt */
  #define USART0_IER_ABTOINT        0x00000200	/* Enables the auto-baud time-out interrupt */
  #define USART0_IER_BITMASK        0x00000387	/* UART interrupt enable register bit mask  - valid for 13xx, 17xx/40xx UART0/2/3, 18xx/43xx UART0/2/3 only*/
#define TOFF_USART_DLM          0x0004		/* (RW) Divisor Latch MSB. Most significant byte of the baud rate divisor value. The full divisor is used to generate a baud rate from the fractional rate divider (DLAB = 1). */
#define TOFF_USART_FCR          0x0008		/* (RW) FIFO Control Register. Controls UART FIFO usage and modes. */
  #define USART0_FCR_FIFO_EN        0x00000001	/* UART FIFO enable */
  #define USART0_FCR_RX_RS          0x00000002	/* UART RX FIFO reset */
  #define USART0_FCR_TX_RS          0x00000004	/* UART TX FIFO reset */
  #define USART0_FCR_DMAMODE_SEL    0x00000008	/* UART DMA mode selection */
  #define USART0_FCR_TRG_LEV0       0x00000000	/* UART FIFO trigger level 0: 1 character */
  #define USART0_FCR_TRG_LEV1       0x00000040	/* UART FIFO trigger level 1: 4 character */
  #define USART0_FCR_TRG_LEV2       0x00000080	/* UART FIFO trigger level 2: 8 character */
  #define USART0_FCR_TRG_LEV3       0x000000C0	/* UART FIFO trigger level 3: 14 character */
  #define USART0_TX_FIFO_SIZE     (16)
#define TOFF_USART_LCR          0x000C	/* (RW) Line Control Register. Contains controls for frame formatting and break generation. */
  #define USART0_LCR_WLEN           0x00000003	/* UART word length select bit mask */
  #define USART0_LCR_WLEN5          0x00000000	/* UART word length select: 5 bit data mode */
  #define USART0_LCR_WLEN6          0x00000001	/* UART word length select: 6 bit data mode */
  #define USART0_LCR_WLEN7          0x00000002	/* UART word length select: 7 bit data mode */
  #define USART0_LCR_WLEN8          0x00000003	/* UART word length select: 8 bit data mode */
  #define USART0_LCR_SBS_MASK       0x00000004	/* UART stop bit select: bit mask */
  #define USART0_LCR_SBS_1BIT       0x00000000	/* UART stop bit select: 1 stop bit */
  #define USART0_LCR_SBS_2BIT       0x00000004	/* UART stop bit select: 2 stop bits (in 5 bit data mode, 1.5 stop bits) */
  #define USART0_LCR_PARITY_EN      0x00000008	/* UART Parity Enable */
  #define USART0_LCR_PARITY_DIS     0x00000000	/* UART Parity Disable */
  #define USART0_LCR_PARITY_ODD     0x00000000	/* UART Parity select: Odd parity */
  #define USART0_LCR_PARITY_EVEN    0x00000010	/* UART Parity select: Even parity */
  #define USART0_LCR_PARITY_F_1     0x00000020	/* UART Parity select: Forced 1 stick parity */
  #define USART0_LCR_PARITY_F_0     0x00000030	/* UART Parity select: Forced 0 stick parity */
  #define USART0_LCR_BREAK_EN       0x00000040	/* UART Break transmission enable */
  #define USART0_LCR_DLAB_EN        0x00000080	/* UART Divisor Latches Access bit enable */
#define TOFF_USART_LSR          0x0014	/* (R)  Line Status Register. Contains flags for transmit and receive status, including line errors. */
  #define USART0_LSR_RDR            0x00000001	/* Line status: Receive data ready */
  #define USART0_LSR_OE             0x00000002	/* Line status: Overrun error */
  #define USART0_LSR_PE             0x00000004	/* Line status: Parity error */
  #define USART0_LSR_FE             0x00000008	/* Line status: Framing error */
  #define USART0_LSR_BI             0x00000010	/* Line status: Break interrupt */
  #define USART0_LSR_THRE           0x00000020	/* Line status: Transmit holding register empty */
  #define USART0_LSR_TEMT           0x00000040	/* Line status: Transmitter empty */
  #define USART0_LSR_RXFE           0x00000080	/* Line status: Error in RX FIFO */
  #define USART0_LSR_TXFE           0x00000100	/* Line status: Error in RX FIFO */
#define TOFF_USART_FDR          0x0028	/* (RW) Fractional Divider Register. Generates a clock input for the baud rate divider. */
  #define USART0_FDR_DIVADDVA       0x0000000F	/* Baud-rate generation pre-scaler divisor */
  #define USART0_FDR_MULVAL         0x000000F0	/* Baud-rate pre-scaler multiplier value */
#define TOFF_USART_TER          0x0030	/* (RW) Transmit Enable Register. Turns off USART transmitter for use with software flow control. */
  #define USART0_TER1_TXEN          0x00000080	/* Transmit enable bit */

/*
 *  SYSTEM CONFIGURATION CONTROLLER
 */
#define TADR_SYSCTL_PRESETCTRL		0x40048004	/* (RW) Peripheral reset Control register */
#define TADR_SYSCTL_SYSPLLCTRL		0x40048008	/* (RW) System PLL control register */
#define TADR_SYSCTL_WDTOSCCTRL		0x40048024	/* (RW) Watchdog Oscillator control register */
#define TADR_SYSCTL_MAINCLKSEL		0x40048070	/* (RW) Main clock source select register */
#define TADR_SYSCTL_SYSAHBCLKDIV	0x40048078	/* (RW) System Clock divider register */
#define TADR_SYSCTL_SYSAHBCLKCTRL	0x40048080	/* (RW) System clock control register */
#define TADR_SYSCTL_FRGCLKDIV		0x400480A0	/* (RW) FRG clock divider (USARTS 1 - 4) register */
#define TADR_SYSCTL_UARTFRGDIV		0x400480F0	/* (RW) USART fractional generator divider (USARTS 1 - 4) register */
#define TADR_SYSCTL_UARTFRGMULT		0x400480F4	/* (RW) USART fractional generator multiplier (USARTS 1 - 4) register */

#define MAINCLKSRC_IRC              0x00000000	/* Internal oscillator */
#define MAINCLKSRC_PLLIN            0x00000001	/* System PLL input */
#define MAINCLKSRC_WDTOSC           0x00000002	/* Watchdog oscillator rate */
#define MAINCLKSRC_PLLOUT           0x00000003	/* System PLL output */
#define MAINCLKSRC_MASK             0x00000003

#define SYSCTL_RESET_FRG            0x00000010	/* FRG reset control */
#define SYSCTL_RESET_USART1         0x00000020	/* USART1 reset control */
#define SYSCTL_CLOCK_USART1         0x00100000	/* 20: USART1 clock */

/*
 *  IO CONFIGURATION UNIT
 */
#define TADR_IOCON_PIO0             0x40044000	/* (RW) PIO0 Configuraion register 0-23 */
#define TADR_IOCON_PIO1             0x40044060	/* (RW) PIO1 Configuraion register 0-31 */
#define TADR_IOCON_PIO2A            0x400440F0	/* (RW) PIO2 Configuraion register 0-1 */
#define TADR_IOCON_PIO2B            0x400440FC	/* (RW) PIO2 Configuraion register 2-23 */


/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	UW base;
	INTNO    intno_usart;
	UW intpri;
	UW txintbase;
	UW txintcheck;
	UW rxintbase;
	UW rxintcheck;
	UW putrdybase;
	UW putrdycheck;
	UW txenavalue;
	UW rxenavalue;
	UW putbase;
	UW getbase;

	UW clockbase;
	UW clock_set;
	UW dividbase;
	UW clock_divid;

	UW txport;
	UW txpin;
	UW txmode_set;

	UW rxport;
	UW rxpin;
	UW rxmode_set;
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
	{(UW)TADR_USART0_BASE,    (INTNO)INTNO_SIO1, 2,
	 (TADR_USART0_BASE+TOFF_USART_IER),  USART0_IER_THREINT,
	 (TADR_USART0_BASE+TOFF_USART_LSR),  USART0_LSR_RDR,
	 (TADR_USART0_BASE+TOFF_USART_LSR),  USART0_LSR_THRE,
	 USART0_IER_THREINT,                 USART0_IER_RBRINT,
	 (TADR_USART0_BASE+TOFF_USART_THR),  (TADR_USART0_BASE+TOFF_USART_RBR),

	 (TADR_SYSCTL_SYSAHBCLKCTRL), 0x00001000,
	 (TADR_SYSCTL_USART0CLKDIV), (UW)1,

     (UW)TX1_PINPORT, (UW)TX1_PINPOS,
     (UW)IOCON_SIO1,
     (UW)RX1_PINPORT, (UW)RX1_PINPOS,
     (UW)IOCON_SIO1}
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];

/*
 *  ピンMUX設定
 */
static void
pinmux_setup(UB port, UB pin, UW modefunc)
{
	switch (port) {
	case 0:
		sil_wrw_mem((UW *)(TADR_IOCON_PIO0+pin*4), modefunc);
		break;

	case 1:
		sil_wrw_mem((UW *)(TADR_IOCON_PIO1+pin*4), modefunc);
		break;

	case 2:
		if (pin >= 2)
			sil_wrw_mem((UW *)(TADR_IOCON_PIO2B+(pin-2)*4), modefunc);
		else
			sil_wrw_mem((UW *)(TADR_IOCON_PIO2A+pin*4), modefunc);
		break;

	default:
		break;
	}
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
	SIOPCB          *siopcb = NULL;
	const SIOPINIB  *siopinib;
	BOOL	opnflg;
	UW	base, pllin, tmp;
	UW	div, divh, divl, clkin, sel;


	siopcb = get_siopcb(siopid);
	siopinib = siopcb->siopinib;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = siopcb->opnflg;

	siopcb->exinf = exinf;
	base  = siopinib->base;

	/*
	 *  ハードウェアの初期化
	 */
	pinmux_setup(siopinib->txport, siopinib->txpin, siopinib->txmode_set);
	pinmux_setup(siopinib->rxport, siopinib->rxpin, siopinib->rxmode_set);

	/*
	 * 通信レートを計算
	 */
	pllin = get_PLLinclockrate();
	tmp = sil_rew_mem((UW *)(TADR_SYSCTL_SYSAHBCLKDIV));
	sel = sil_rew_mem((UW *)(TADR_SYSCTL_MAINCLKSEL)) & MAINCLKSRC_MASK;
	switch(sel){
	case MAINCLKSRC_IRC:
		clkin = SYSCTL_IRC_FREQ;
		break;

	case MAINCLKSRC_PLLIN:
		clkin = pllin / tmp;
		break;

	case MAINCLKSRC_WDTOSC:
		clkin = get_WDTrate(sil_rew_mem((UW *)(TADR_SYSCTL_WDTOSCCTRL))) / tmp;
		break;

	case MAINCLKSRC_PLLOUT:
		clkin = (((sil_rew_mem((UW *)(TADR_SYSCTL_SYSPLLCTRL)) & 0x1F) +1) * pllin) / tmp;
		break;
	}

	/*
	 * USART DIVIDERセット
	 */
	if( siopinib->clock_divid == 0){
		/* Get integer divider for coarse rate */
		div = clkin / (115200 * 48);
		if (div == 0) {
			div = 1;
		}
	}
	else
		div = siopinib->clock_divid;
	sil_wrw_mem((UW *)siopinib->dividbase, div);

	/*
	 * USART0の初期化
	 */
	/*
	 * USART0 クロックイン
	 */
	sil_orw_mem((UW *)siopinib->clockbase, siopinib->clock_set);

	/*
	 * FIFOリセットと有効化
	 */
	sil_wrw_mem((UW *)(base+TOFF_USART_FCR), (USART0_FCR_FIFO_EN | USART0_FCR_RX_RS | USART0_FCR_TX_RS));

	/*
	 * fractional divider = 1
	 */
	sil_wrw_mem((UW *)(base+TOFF_USART_FDR), 0x10);

	div = clkin / (DEF_BAUDRATE * 16);

	/*
	 * 通信レートをHigh/Lowデバイダーにセット
	 */
	divh = div / 256;
	divl = div - (divh * 256);

	sil_orw_mem((UW *)(base+TOFF_USART_LCR), USART0_LCR_DLAB_EN);
	sil_wrw_mem((UW *)(base+TOFF_USART_DLL), divl);
	sil_wrw_mem((UW *)(base+TOFF_USART_DLM), divh);
	sil_andw_mem((UW *)(base+TOFF_USART_LCR), USART0_LCR_DLAB_EN);

	sil_wrw_mem((UW *)(base+TOFF_USART_LCR), (USART0_LCR_WLEN8 | USART0_LCR_SBS_1BIT));
	sil_wrw_mem((UW *)(base+TOFF_USART_FCR), (USART0_FCR_FIFO_EN | USART0_FCR_TRG_LEV2));
	sil_wrw_mem((UW *)(base+TOFF_USART_TER), USART0_TER1_TXEN);

	siopcb->opnflg = TRUE;

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		x_config_int(siopinib->intno_usart, TRUE, siopinib->intpri);
	}
	sil_orw_mem((UW *)(base+TOFF_USART_IER), (USART0_IER_RBRINT | USART0_IER_RLSINT));

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
		sil_andw_mem((UW *)(siopinib->base+TOFF_USART_IER), (USART0_IER_RBRINT | USART0_IER_RLSINT));
		sil_andw_mem((UW *)(siopinib->clockbase), siopinib->clock_set);
	}
	siopcb->opnflg = FALSE;
}

/*
 *  SIOの割込みサービスルーチン
 */

Inline BOOL
sio_putintready(SIOPCB* siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;

	if((sil_rew_mem((UW *)siopinib->txintbase) & siopinib->txintcheck) != 0){
		return 1;
	}
	return 0;
}

Inline BOOL
sio_getintready(SIOPCB* siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;

	if ((sil_rew_mem((UW *)siopinib->rxintbase) & siopinib->rxintcheck) != 0){
		return 1;
	}
	return 0;
}

void
sio_handler(void)
{
	SIOPCB          *siopcb;

	siopcb = get_siopcb(DEFAULT_PORT);

	if (sio_getintready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putintready(siopcb)) {
		sio_ierdy_snd(siopcb->exinf);
	}
}

Inline BOOL
sio_putready(SIOPCB* siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;
	while ((sil_rew_mem((UW *)siopinib->putrdybase) & siopinib->putrdycheck) == 0);
	return 1;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sio_snd_chr(SIOPCB *siopcb, char c)
{
	if (sio_putready(siopcb)) {
		sil_wrw_mem((UW *)(siopcb->siopinib->putbase), (UW)c);
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
	const SIOPINIB  *siopinib = siopcb->siopinib;
	INT c = -1;

	if ((sil_rew_mem((UW *)siopinib->rxintbase) & siopinib->rxintcheck) != 0){
		c = sil_rew_mem((UW *)(siopinib->getbase)) & 0xFF;
	}
	return c;
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;

	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_orw_mem((UW *)(siopinib->txintbase), siopinib->txenavalue);
		break;
	case SIO_ERDY_RCV:
		sil_orw_mem((UW *)(siopinib->txintbase), siopinib->rxenavalue);
		break;
	}
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
sio_dis_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;

	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_andw_mem((UW *)(siopinib->txintbase), siopinib->txenavalue);
		break;
	case SIO_ERDY_RCV:
		sil_andw_mem((UW *)(siopinib->txintbase), siopinib->rxenavalue);
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

	sil_wrw_mem((UW *)(siopinib->putbase), (UW)c);
	while ((sil_rew_mem((UW *)siopinib->putrdybase) & siopinib->putrdycheck) == 0);

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = SystemFrequency / (DEF_BAUDRATE/4);
	while(n--);
}

/*
 *  ターゲットのシリアル初期化
 */
void
sio_init(void)
{
	const SIOPINIB	*siopinib = &siopinib_table[INDEX_SIOP(DEFAULT_PORT)];
	UW base, pllin, tmp;
	UW div, divh, divl, clkin, sel;

	base = siopinib->base;

	pinmux_setup(siopinib->txport, siopinib->txpin, siopinib->txmode_set);
	pinmux_setup(siopinib->rxport, siopinib->rxpin, siopinib->rxmode_set);

	/*
	 * USART DIVIDERセット
	 */
	sil_wrw_mem((UW *)siopinib->dividbase, siopinib->clock_divid);

	/*
	 * USART クロックイン
	 */
	sil_orw_mem((UW *)siopinib->clockbase, siopinib->clock_set);

	/*
	 * FIFOリセットと有効化
	 */
	sil_wrw_mem((UW *)(base+TOFF_USART_FCR), (USART0_FCR_FIFO_EN | USART0_FCR_RX_RS | USART0_FCR_TX_RS));

	/*
	 * fractional divider = 1
	 */
	sil_wrw_mem((UW *)(base+TOFF_USART_FDR), 0x10);

	/*
	 * 通信レートを計算
	 */
	pllin = get_PLLinclockrate();
	tmp = sil_rew_mem((UW *)(TADR_SYSCTL_SYSAHBCLKDIV));
	sel = sil_rew_mem((UW *)(TADR_SYSCTL_MAINCLKSEL)) & MAINCLKSRC_MASK;
	switch(sel){
	case MAINCLKSRC_IRC:
		clkin = SYSCTL_IRC_FREQ;
		break;

	case MAINCLKSRC_PLLIN:
		clkin = pllin / tmp;
		break;

	case MAINCLKSRC_WDTOSC:
		clkin = get_WDTrate(sil_rew_mem((UW *)(TADR_SYSCTL_WDTOSCCTRL))) / tmp;
		break;

	case MAINCLKSRC_PLLOUT:
		clkin = (((sil_rew_mem((UW *)(TADR_SYSCTL_SYSPLLCTRL)) & 0x1F) +1) * pllin) / tmp;
		break;
	}

	div = clkin / (DEF_BAUDRATE * 16);

	/*
	 * 通信レートをHigh/Lowデバイダーにセット
	 */
	divh = div / 256;
	divl = div - (divh * 256);

	sil_orw_mem((UW *)(base+TOFF_USART_LCR), USART0_LCR_DLAB_EN);
	sil_wrw_mem((UW *)(base+TOFF_USART_DLL), divl);
	sil_wrw_mem((UW *)(base+TOFF_USART_DLM), divh);
	sil_andw_mem((UW *)(base+TOFF_USART_LCR), USART0_LCR_DLAB_EN);

	sil_wrw_mem((UW *)(base+TOFF_USART_LCR), (USART0_LCR_WLEN8 | USART0_LCR_SBS_1BIT));
	sil_wrw_mem((UW *)(base+TOFF_USART_FCR), (USART0_FCR_FIFO_EN | USART0_FCR_TRG_LEV2));
	sil_wrw_mem((UW *)(base+TOFF_USART_TER), USART0_TER1_TXEN);
	sil_wrw_mem((UW *)(base+TOFF_USART_IER), 0);
}
