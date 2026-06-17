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
 *  @(#) $Id: hw_serial.c 2246 2025-12-09 11:14:50Z roi $
 */

/*
 *		シリアルI/Oデバイス（SIO）ドライバ（SAPPHIRESOC用）
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

/*
 *  シリアルI/OポートIDから管理ブロックを取り出すためのマクロ
 */
#define INDEX_SIOP(siopid)	((UINT)((siopid) - 1))
#define get_siopcb(siopid)	(&(siopcb_table[INDEX_SIOP(siopid)]))

#define DEFAULT_PORT    1

/*
 *  レジスタ設定値
 */
/*
 *  UARTレジスタ定義
 */
#define TOFF_UART_DATA          0x0000
#define TOFF_UART_STATUS        0x0004
  #define UART_TX_EMPTY_INTERRUPT_ENABLE     0x00000001
  #define UART_RX_NOTEMPTY_INTERRUPT_ENABLE  0x00000002
  #define UART_TXFIFO_EMPTY_INTERRUPT        0x00000100
  #define UART_RXFIFO_NOTEMPTY_INTERRUPT     0x00000200
  #define UART_WRITEAVAILABILITY             0x00FF0000
  #define UART_READOCCUPANCY                 0xFF000000
#define TOFF_UART_CLOCK_DIVIDER 0x0008
#define TOFF_UART_FRAME_CONFIG  0x000C
  #define UART_CONFIG_DATALENGTH             0x000000FF
  #define UART_CONFIG_PARITY                 0x00000300
  #define UART_CONFIG_STOPBIT                0x00010000

#define UART_A_SAMPLE_PER_BAUD  SYSTEM_UART_0_IO_PARAMETER_UART_CTRL_CONFIG_RX_SAMPLE_PER_BIT

#define STOP_1                  0
#define STOP_2                  1
#define PARITY_NONE             0
#define PARITY_EVEN             1
#define PARITY_ODD              2
#define CONFIG_VALUE            ((STOP_1<<16) | (PARITY_NONE<<8) | (8-1))

/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	UW      base;
	INTNO   intno_usart;
	PRI     intpri;
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
	{(UW)SYSTEM_UART_0_IO_CTRL, (INTNO)SYSTEM_PLIC_SYSTEM_UART_0_IO_INTERRUPT, 1}
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];


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
	SIOPCB          *siopcb;
	const SIOPINIB  *siopinib;
	UW	  divider = SYSTEM_CLINT_HZ/(DEF_BAUDRATE*UART_A_SAMPLE_PER_BAUD)-1;
	BOOL  opnflg;
	UW    base;

	siopcb = get_siopcb(siopid);
	siopinib = siopcb->siopinib;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = siopcb->opnflg;

	siopcb->exinf = exinf;
	base = siopinib->base;
	if(base == 0)				/* no uart port */
		goto sio_opn_exit;

	/*
	 *  ハードウェアの初期化
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_CLOCK_DIVIDER), divider);
	sil_wrw_mem((UW *)(base+TOFF_UART_FRAME_CONFIG), CONFIG_VALUE);
	siopcb->opnflg = TRUE;

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
	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	if ((siopcb->opnflg)) {
		dis_int(siopcb->siopinib->intno_usart);
	}
	siopcb->opnflg = FALSE;
}

/*
 *  SIOの割込みサービスルーチン
 */

Inline BOOL
sio_putintready(SIOPCB* siopcb)
{
	UW status = sil_rew_mem((UW *)(siopcb->siopinib->base+TOFF_UART_STATUS));

	if((status & UART_TX_EMPTY_INTERRUPT_ENABLE) != 0 && (status & UART_TXFIFO_EMPTY_INTERRUPT) != 0)
		return 1;
	else
		return 0;
}

Inline BOOL
sio_getintready(SIOPCB* siopcb)
{
	UW status = sil_rew_mem((UW *)(siopcb->siopinib->base+TOFF_UART_STATUS));

	if((status & UART_RXFIFO_NOTEMPTY_INTERRUPT) != 0)
		return 1;
	else
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

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sio_snd_chr(SIOPCB *siopcb, char c)
{
	UW base = siopcb->siopinib->base;

	base = SYSTEM_UART_0_IO_CTRL;
	if((sil_rew_mem((UW *)(base+TOFF_UART_STATUS)) & UART_WRITEAVAILABILITY) != 0){
		sil_wrw_mem((UW *)(base+TOFF_UART_DATA), (UW)c);
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

	if((sil_rew_mem((UW *)(base+TOFF_UART_STATUS)) & UART_READOCCUPANCY) != 0){
		c = sil_rew_mem((UW *)(base+TOFF_UART_DATA)) & 0xFF;
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
		sil_orw_mem((UW *)(siopcb->siopinib->base+TOFF_UART_STATUS), UART_TX_EMPTY_INTERRUPT_ENABLE);
		break;
	case SIO_ERDY_RCV:
		sil_orw_mem((UW *)(siopcb->siopinib->base+TOFF_UART_STATUS), UART_RX_NOTEMPTY_INTERRUPT_ENABLE);
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
		sil_andw_mem((UW *)(siopcb->siopinib->base+TOFF_UART_STATUS), UART_TX_EMPTY_INTERRUPT_ENABLE);
		break;
	case SIO_ERDY_RCV:
		sil_andw_mem((UW *)(siopcb->siopinib->base+TOFF_UART_STATUS), UART_RX_NOTEMPTY_INTERRUPT_ENABLE);
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void sio_snd_chr_pol(char c)
{
	UW base = siopinib_table[INDEX_SIOP(DEFAULT_PORT)].base;

	sil_wrw_mem((UW *)(base+TOFF_UART_DATA), (UW)c);
	while(0 == (sil_rew_mem((UW *)(base+TOFF_UART_STATUS)) & UART_WRITEAVAILABILITY));

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = SYSTEM_CLINT_HZ/DEF_BAUDRATE;
	while(n--);
}

/*
 *  ターゲットのシリアル初期化
 */
void sio_init(void)
{
	const SIOPINIB  *siopinib;
	UW divider = SYSTEM_CLINT_HZ/(DEF_BAUDRATE*UART_A_SAMPLE_PER_BAUD)-1;
	UW base;

	siopinib = &siopinib_table[INDEX_SIOP(DEFAULT_PORT)];
	base = siopinib->base;

	/*
	 *  ハードウェアの初期化
	 */
	sil_wrw_mem((UW *)(base+TOFF_UART_CLOCK_DIVIDER), divider);
	sil_wrw_mem((UW *)(base+TOFF_UART_FRAME_CONFIG), CONFIG_VALUE);
}
