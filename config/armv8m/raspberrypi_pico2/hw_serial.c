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
 *  @(#) $Id: hw_serial.c 2246 2025-12-09 11:30:29Z roi $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（Raspberry Pi Pico2用）
 */

#include <t_services.h>
#include <sys_config.h>
#include <hw_serial.h>
#include <t_syslog.h>

/*
 * レジスタ設定値
 */
#define INDEX_SIOP(x)	((x) - 1)
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

#define DEFAULT_PORT    1

/*
 *  SIO用UARTの設定
 */
#define USART1_RESET_ADDR  0x40020000	/* TADR_RESETS_RESET */
#define USART1_RESET_CHECK 0x40020008	/* TADR_RESETS_RESET_DONE */
#define RESET_U1_BIT       0x04000000	/* RESETS_RESET_UART0 */
#define USART2_RESET_ADDR  0x40020000	/* TADR_RESETS_RESET */
#define USART2_RESET_CHECK 0x40020008	/* TADR_RESETS_RESET_DONE */
#define RESET_U2_BIT       0x08000000	/* RESETS_RESET_UART1 */

#define REG_ALIAS_RW       0x00000000
#define REG_ALIAS_XOR      0x00001000
#define REG_ALIAS_SET      0x00002000
#define REG_ALIAS_CLR      0x00003000

/*
 *  IO BANK0
 */
#define TADR_IO_BANK0_BASE      0x40028000
#define TOFF_IO_BANK0_GPIO_CTRL 0x0004	/* GPIO control including function select and overrides */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL           0x0000001F	/* selects pin function according to the gpio table */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_JTAG   0x00	/* gpio0-3 */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_HSTX   0x00	/* gpio12-19 */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_SPI    0x01
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART   0x02
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_I2C    0x03
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_PWM    0x04
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_SIO    0x05
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_PIO0   0x06
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_PIO1   0x07
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_PIO2   0x08
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_XIP    0x09	/* gpio0,8,19 */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_CSTR   0x09	/* gpio1-5 */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_CLKS   0x09	/* gpio12-15,20-25 */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_USB    0x0A
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART2  0x0B	/* gpio2,3,6,7,10,11,14,15,18,19,22,23,26,27,30,31,34,35,38,39,42,43,46,47 */
  #define IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_NULL   0x1F
#define GPIO_SIZE               8
#define NUM_BANK0_GPIO          48

/*
 *  PADS BANK0
 */
#define TADR_PADS_BANK0_GPIO    0x40038004	/* (RW) Pad control register */
  #define PADS_BANK0_GPIO_SLEWFAST  0x00000001	/* SLEWFAST (0): Slew rate control */
  #define PADS_BANK0_GPIO_SCHMITT   0x00000002	/* SCHMITT (1): Enable schmitt trigger */
  #define PADS_BANK0_GPIO_PDE       0x00000004	/* PDE (1): Pull down enable */
  #define PADS_BANK0_GPIO_PUE       0x00000008	/* PUE (0): Pull up enable */
  #define PADS_BANK0_GPIO_DRIVE     0x00000030	/* DRIVE (1): Drive strength */
  #define PADS_BANK0_GPIO_IE        0x00000040	/* IE (1): Input enable */
  #define PADS_BANK0_GPIO_OD        0x00000080	/* OD (0): Output disable */
  #define PADS_BANK0_GPIO_ISO       0x00000100	/* Pad isolation control. */

/*
 *  UNIVERSAL ASYNCHORONOUS RECEIVER TRANSMITTER
 */
#define TOFF_UART_UARTDR        0x0000		/* (RW) Data Register */
  #define UART_UARTDR_DATA        0x000000FF	/* DATA (0): Receive (read) data character */
  #define UART_UARTDR_FE          0x00000100	/* FE (0): Framing error */
  #define UART_UARTDR_PE          0x00000200	/* PE (0): Parity error */
  #define UART_UARTDR_BE          0x00000400	/* BE (0): Break error */
  #define UART_UARTDR_OE          0x00000800	/* OE (0): Overrun error */
#define TOFF_UART_UARTFR        0x0018		/* (R)  Flag Register */
  #define UART_UARTFR_CTS         0x00000001	/* CTS (0): Clear to send */
  #define UART_UARTFR_DSR         0x00000002	/* DSR (0): Data set ready */
  #define UART_UARTFR_DCD         0x00000004	/* DCD (0): Data carrier detect */
  #define UART_UARTFR_BUSY        0x00000008	/* BUSY (0): UART busy */
  #define UART_UARTFR_RXFE        0x00000010	/* RXFE (1): Receive FIFO empty */
  #define UART_UARTFR_TXFF        0x00000020	/* TXFF (0): Transmit FIFO full */
  #define UART_UARTFR_RXFF        0x00000040	/* RXFF (0): Receive FIFO full */
  #define UART_UARTFR_TXFE        0x00000080	/* TXFE (1): Transmit FIFO empty */
  #define UART_UARTFR_RI          0x00000100	/* RI (0): Ring indicator */
#define TOFF_UART_UARTIBRD      0x0024		/* (RW) Integer Baud Rate Register, */
#define TOFF_UART_UARTFBRD      0x0028		/* (RW) Fractional Baud Rate Register */
#define TOFF_UART_UARTLCR_H     0x002C		/* (RW) Line Control Register */
  #define UART_UARTLCR_H_BRK        0x00000001	/* BRK (0): Send break */
  #define UART_UARTLCR_H_PEN        0x00000002	/* PEN (0): Parity enable: 0 = parity is disabled and no parity bit added to the data frame 1 =... */
  #define UART_UARTLCR_H_EPS        0x00000004	/* EPS (0): Even parity select */
  #define UART_UARTLCR_H_STP2       0x00000008	/* STP2 (0): Two stop bits select */
  #define UART_UARTLCR_H_FEN        0x00000010	/* FEN (0): Enable FIFOs: 0 = FIFOs are disabled (character mode) that is, the FIFOs become... */
  #define UART_UARTLCR_H_WLEN       0x00000060	/* WLEN (0): Word length */
  #define UART_UARTLCR_H_SPS        0x00000080	/* SPS (0): Stick parity select */
#define TOFF_UART_UARTCR        0x0030	/* (RW) Control Register */
  #define UART_UARTCR_UARTEN        0x00000001	/* UARTEN (0): UART enable: 0 = UART is disabled */
  #define UART_UARTCR_SIREN         0x00000002	/* SIREN (0): SIR enable: 0 = IrDA SIR ENDEC is disabled */
  #define UART_UARTCR_SIRLP         0x00000004	/* SIRLP (0): SIR low-power IrDA mode */
  #define UART_UARTCR_LBE           0x00000080	/* LBE (0): Loopback enable */
  #define UART_UARTCR_TXE           0x00000100	/* TXE (1): Transmit enable */
  #define UART_UARTCR_RXE           0x00000200	/* RXE (1): Receive enable */
  #define UART_UARTCR_DTR           0x00000400	/* DTR (0): Data transmit ready */
  #define UART_UARTCR_RTS           0x00000800	/* RTS (0): Request to send */
  #define UART_UARTCR_OUT1          0x00001000	/* OUT1 (0): This bit is the complement of the UART Out1 (nUARTOut1) modem status output */
  #define UART_UARTCR_OUT2          0x00002000	/* OUT2 (0): This bit is the complement of the UART Out2 (nUARTOut2) modem status output */
  #define UART_UARTCR_RTSEN         0x00004000	/* RTSEN (0): RTS hardware flow control enable */
  #define UART_UARTCR_CTSEN         0x00008000	/* CTSEN (0): CTS hardware flow control enable */
#define TOFF_UART_UARTIFLS      0x0034	/* (RW) Interrupt FIFO Level Select Register */
  #define UART_UARTIFLS_TXIFLSEL    0x00000007	/* TXIFLSEL (0x2): Transmit interrupt FIFO level select */
  #define UART_UARTIFLS_RXIFLSEL    0x00000038	/* RXIFLSEL (0x2): Receive interrupt FIFO level select */
#define TOFF_UART_UARTIMSC      0x0038	/* (RW) Interrupt Mask Set/Clear Register */
  #define UART_UARTIMSC_RIMIM       0x00000001	/* RIMIM (0): nUARTRI modem interrupt mask */
  #define UART_UARTIMSC_CTSMIM      0x00000002	/* CTSMIM (0): nUARTCTS modem interrupt mask */
  #define UART_UARTIMSC_DCDMIM      0x00000004	/* DCDMIM (0): nUARTDCD modem interrupt mask */
  #define UART_UARTIMSC_DSRMIM      0x00000008	/* DSRMIM (0): nUARTDSR modem interrupt mask */
  #define UART_UARTIMSC_RXIM        0x00000010	/* RXIM (0): Receive interrupt mask */
  #define UART_UARTIMSC_TXIM        0x00000020	/* TXIM (0): Transmit interrupt mask */
  #define UART_UARTIMSC_RTIM        0x00000040	/* RTIM (0): Receive timeout interrupt mask */
  #define UART_UARTIMSC_FEIM        0x00000080	/* FEIM (0): Framing error interrupt mask */
  #define UART_UARTIMSC_PEIM        0x00000100	/* PEIM (0): Parity error interrupt mask */
  #define UART_UARTIMSC_BEIM        0x00000200	/* BEIM (0): Break error interrupt mask */
  #define UART_UARTIMSC_OEIM        0x00000400	/* OEIM (0): Overrun error interrupt mask */
#define TOFF_UART_UARTRIS       0x003C	/* (R)  Raw Interrupt Status Register */
  #define UART_UARTRIS_RIRMIS       0x00000001	/* RIRMIS (0): nUARTRI modem interrupt status */
  #define UART_UARTRIS_CTSRMIS      0x00000002	/* CTSRMIS (0): nUARTCTS modem interrupt status */
  #define UART_UARTRIS_DCDRMIS      0x00000004	/* DCDRMIS (0): nUARTDCD modem interrupt status */
  #define UART_UARTRIS_DSRRMIS      0x00000008	/* DSRRMIS (0): nUARTDSR modem interrupt status */
  #define UART_UARTRIS_RXRIS        0x00000010	/* RXRIS (0): Receive interrupt status */
  #define UART_UARTRIS_TXRIS        0x00000020	/* TXRIS (0): Transmit interrupt status */
  #define UART_UARTRIS_RTRIS        0x00000040	/* RTRIS (0): Receive timeout interrupt status */
  #define UART_UARTRIS_FERIS        0x00000080	/* FERIS (0): Framing error interrupt status */
  #define UART_UARTRIS_PERIS        0x00000100	/* PERIS (0): Parity error interrupt status */
  #define UART_UARTRIS_BERIS        0x00000200	/* BERIS (0): Break error interrupt status */
  #define UART_UARTRIS_OERIS        0x00000400	/* OERIS (0): Overrun error interrupt status */
#define TOFF_UART_UARTDMACR     0x0048	/* (RW) DMA Control Register */
  #define UART_UARTDMACR_RXDMAE     0x00000001	/* RXDMAE (0): Receive DMA enable */
  #define UART_UARTDMACR_TXDMAE     0x00000002	/* TXDMAE (0): Transmit DMA enable */
  #define UART_UARTDMACR_DMAONERR   0x00000004	/* DMAONERR (0): DMA on error */

#define UART_MODE_MASK	(UART_UARTLCR_H_WLEN | UART_UARTLCR_H_STP2 | UART_UARTLCR_H_PEN | UART_UARTLCR_H_EPS)
#define UART_MODE_VALUE ((8-5) << 5)	/* data 8bits */

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	UW      base;
	INTNO   intno_usart;
	PRI     intpri;
	UW      reset_addr;
	UW      reset_done_addr;
	UW      reset;
	UB      txpin;
	UB      rxpin;
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
	{(UW)0x40070000, (INTNO)IRQ_VECTOR_UART0, INTPRI_SIO, /* UART0 */
      USART1_RESET_ADDR, USART1_RESET_CHECK, RESET_U1_BIT, 0, 1
#if TNUM_SIOP >= 2
    },
	{(UW)0x40078000, (INTNO)IRQ_VECTOR_UART1, INTPRI_SIO, /* UART1 */
      USART2_RESET_ADDR, USART2_RESET_CHECK, RESET_U2_BIT, 8, 9
#endif
    }
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];


/*
 *  ピンMUX設定
 */
static void
pinmux_setup(UB pin, UB modefunc)
{
	UW pbase, gbase, mask;

	if(pin >= NUM_BANK0_GPIO)
		return;
	if(modefunc > 0x1F)
		return;
	pbase = TADR_PADS_BANK0_GPIO+pin*4;
	gbase = TADR_IO_BANK0_BASE+pin*GPIO_SIZE;

	mask = ((sil_rew_mem((UW *)(pbase)) ^ PADS_BANK0_GPIO_IE) & (PADS_BANK0_GPIO_IE | PADS_BANK0_GPIO_OD));
	sil_wrw_mem((UW *)(pbase+REG_ALIAS_XOR), mask);

	sil_wrw_mem((UW *)(gbase+TOFF_IO_BANK0_GPIO_CTRL), modefunc);
	sil_wrw_mem((UW *)(pbase+REG_ALIAS_CLR), PADS_BANK0_GPIO_ISO);
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
	BOOL  opnflg;
	UW base, reset, clock, mask;
	UW baud_rate_div, baud_ibrd, baud_fbrd;

	if((clock = get_Clock(CLK_PERI)) == 0){
		goto sio_opn_exit;
	}

	siopcb = get_siopcb(siopid);
	siopinib = siopcb->siopinib;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = siopcb->opnflg;

	siopcb->exinf = exinf;
	base  = siopinib->base;
	reset = siopinib->reset;


	/*
	 *  ハードウェアの初期化
	 */
	pinmux_setup(siopinib->txpin, IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART);
	pinmux_setup(siopinib->rxpin, IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART);

	/*
	 *  UARTをリセット
	 */
	sil_wrw_mem((UW *)(siopinib->reset_addr+REG_ALIAS_SET), reset);
	sil_wrw_mem((UW *)(siopinib->reset_addr+REG_ALIAS_CLR), reset);
	while((sil_rew_mem((UW *)(siopinib->reset_done_addr)) & reset) == 0);

	/*
	 *  BAUD設定
	 */
	baud_rate_div = (8 * clock / DEF_BAUDRATE);
    baud_ibrd = baud_rate_div >> 7;
	if(baud_ibrd == 0){
		baud_ibrd = 1;
		baud_fbrd = 0;
    }
	else if(baud_ibrd >= 65535){
		baud_ibrd = 65535;
		baud_fbrd = 0;
	}
	else{
		baud_fbrd = ((baud_rate_div & 0x7f) + 1) / 2;
	}

	/*
	 *  Baudrate Dividor設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTIBRD), baud_ibrd);
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTFBRD), baud_fbrd);

	/*
	 *  ラインコントロールレジスタにダミー設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTLCR_H+REG_ALIAS_SET), 0);

	/*
	 *  通信モード設定
	 */
	mask = ((sil_rew_mem((UW *)(base+TOFF_UART_UARTLCR_H)) ^ UART_MODE_VALUE) & UART_MODE_MASK);
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTLCR_H+REG_ALIAS_XOR), mask);

	/*
	 *  送受信有効化
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTCR), (UART_UARTCR_UARTEN | UART_UARTCR_TXE | UART_UARTCR_RXE));

	/*
	 *  RX/TX DMA設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTDMACR), (UART_UARTDMACR_TXDMAE | UART_UARTDMACR_RXDMAE));

	/*
	 *  FIFO無効化
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTLCR_H+REG_ALIAS_CLR), UART_UARTLCR_H_FEN);

	/*
	 *  割込み設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTIMSC), UART_UARTIMSC_RTIM);
	mask = (sil_rew_mem((UW *)(base+TOFF_UART_UARTIFLS)) & (UART_UARTIFLS_TXIFLSEL | UART_UARTIFLS_RXIFLSEL));
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTIFLS+REG_ALIAS_XOR), mask);

	siopcb->opnflg = TRUE;
	sil_dly_nse(1000*1000);

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		x_config_int(siopinib->intno_usart, TRUE, siopinib->intpri);
	}
	sil_dly_nse(10000);

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
	 *  UARTをリセット
	 */
	sil_wrw_mem((UW *)(siopinib->reset_addr+REG_ALIAS_SET), siopinib->reset);

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
sio_putintready(SIOPCB* siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;
	UW base = siopinib->base;
	UW imsc = sil_rew_mem((UW *)(base+TOFF_UART_UARTIMSC));

	if((imsc & UART_UARTIMSC_TXIM) != 0){
		return 1;
	}
	return 0;
}

Inline BOOL
sio_getintready(SIOPCB* siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;
	UW base = siopinib->base;
	UW imsc = sil_rew_mem((UW *)(base+TOFF_UART_UARTIMSC));
	UW ufr  = sil_rew_mem((UW *)(base+TOFF_UART_UARTFR));

	if ((imsc & UART_UARTIMSC_RXIM) != 0 && (ufr & UART_UARTFR_RXFE) == 0){
		return 1;
	}
	return 0;
}

void
sio_handler(void)
{
	SIOPCB          *siopcb;

	siopcb = get_siopcb(1);

	if (sio_getintready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putintready(siopcb)) {
		sio_ierdy_snd(siopcb->exinf);
	}
}

#if TNUM_SIOP >= 2
void
sio_handler2(void)
{
	SIOPCB	*siopcb = get_siopcb(2);

	if (sio_getintready(siopcb)) {
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (sio_putintready(siopcb)) {
		sio_ierdy_snd(siopcb->exinf);
	}
}
#endif

Inline BOOL
sio_putready(SIOPCB* siopcb)
{
	const SIOPINIB  *siopinib = siopcb->siopinib;
	if((sil_rew_mem((UW *)(siopinib->base+TOFF_UART_UARTFR)) & UART_UARTFR_TXFF) == 0)
		return TRUE;
	else
		return FALSE;
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sio_snd_chr(SIOPCB *siopcb, char c)
{
	if (sio_putready(siopcb)) {
		sil_wrw_mem((UW *)(siopcb->siopinib->base+TOFF_UART_UARTDR), (UW)c);
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
	UW base = siopcb->siopinib->base;
	INT c = -1;

	if((sil_rew_mem((UW *)(base+TOFF_UART_UARTFR)) & UART_UARTFR_RXFE) == 0){
		c = sil_rew_mem((UW *)(base+TOFF_UART_UARTDR)) & 0xFF;
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
	UW base = siopinib->base;

	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_wrw_mem((UW *)(base+TOFF_UART_UARTIMSC+REG_ALIAS_SET), UART_UARTIMSC_TXIM);
		break;
	case SIO_ERDY_RCV:
		sil_wrw_mem((UW *)(base+TOFF_UART_UARTIMSC+REG_ALIAS_SET), UART_UARTIMSC_RXIM);
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
	UW base = siopinib->base;

	switch (cbrtn) {
	case SIO_ERDY_SND:
		sil_wrw_mem((UW *)(base+TOFF_UART_UARTIMSC+REG_ALIAS_CLR), UART_UARTIMSC_TXIM);
		break;
	case SIO_ERDY_RCV:
		sil_wrw_mem((UW *)(base+TOFF_UART_UARTIMSC+REG_ALIAS_CLR), UART_UARTIMSC_RXIM);
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void sio_snd_chr_pol(char c)
{
	const SIOPINIB  *siopinib = &siopinib_table[INDEX_SIOP(DEFAULT_PORT)];
	UW base = siopinib->base;

	sil_wrw_mem((UW *)(base+TOFF_UART_UARTDR), (UW)c);
	while ((sil_rew_mem((UW *)(base+TOFF_UART_UARTFR)) & UART_UARTFR_TXFF) != 0);

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = SystemFrequency / (DEF_BAUDRATE/4);
	while(n--);
}

/*
 *  ターゲットのシリアル初期化
 */
void sio_init(void)
{
	const SIOPINIB  *siopinib = &siopinib_table[INDEX_SIOP(DEFAULT_PORT)];
	UW base, reset, clock, mask;
	UW baud_rate_div, baud_ibrd, baud_fbrd;

	base = siopinib->base;
	reset = siopinib->reset;

	if((clock = get_Clock(CLK_PERI)) == 0)
		return;

	/*
	 *  ハードウェアの初期化
	 */
	sil_dly_nse(100*1000000);
	pinmux_setup(siopinib->txpin, IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART);
	pinmux_setup(siopinib->rxpin, IO_BANK0_GPIO_CTRL_FUNCSEL_VALUE_UART);

	/*
	 *  UARTをリセット
	 */
	sil_wrw_mem((UW *)(siopinib->reset_addr+REG_ALIAS_SET), reset);
	sil_wrw_mem((UW *)(siopinib->reset_addr+REG_ALIAS_CLR), reset);
	while((sil_rew_mem((UW *)(siopinib->reset_done_addr)) & reset) == 0);

	/*
	 *  BAUD設定
	 */
	baud_rate_div = (8 * clock / DEF_BAUDRATE);
    baud_ibrd = baud_rate_div >> 7;
	if(baud_ibrd == 0){
		baud_ibrd = 1;
		baud_fbrd = 0;
    }
	else if(baud_ibrd >= 65535){
		baud_ibrd = 65535;
		baud_fbrd = 0;
	}
	else{
		baud_fbrd = ((baud_rate_div & 0x7f) + 1) / 2;
	}

	/*
	 *  Baudrate Dividor設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTIBRD), baud_ibrd);
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTFBRD), baud_fbrd);

	/*
	 *  ラインコントロールレジスタにダミー設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTLCR_H+REG_ALIAS_SET), 0);

	/*
	 *  通信モード設定
	 */
	mask = ((sil_rew_mem((UW *)(base+TOFF_UART_UARTLCR_H)) ^ UART_MODE_VALUE) & UART_MODE_MASK);
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTLCR_H+REG_ALIAS_XOR), mask);

	/*
	 *  送受信有効化
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTCR), (UART_UARTCR_UARTEN | UART_UARTCR_TXE | UART_UARTCR_RXE));

	/*
	 *  RX/TX DMA設定
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTDMACR), (UART_UARTDMACR_TXDMAE | UART_UARTDMACR_RXDMAE));

	/*
	 *  FIFO無効化
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTLCR_H+REG_ALIAS_CLR), UART_UARTLCR_H_FEN);

	/*
	 *  割込み設定
	 */
	mask = (sil_rew_mem((UW *)(base+TOFF_UART_UARTIFLS)) & (UART_UARTIFLS_TXIFLSEL | UART_UARTIFLS_RXIFLSEL));
	sil_wrw_mem((UW *)(base+TOFF_UART_UARTIFLS+REG_ALIAS_XOR), mask);
	sil_dly_nse(1000*1000);
}
