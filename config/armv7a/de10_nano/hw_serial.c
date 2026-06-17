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
 *  @(#) $Id: hw_serial.c 2246 2026-03-06 15:20:00Z roi $
 */

/*
 *		簡易SIOドライバ（DE10-Nano用）
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

/*
 *  シリアルI/Oポート初期化ブロックの取出し
 */
#define INDEX_SIOPINIB(siopid)  ((UINT)((siopid) - 1))
#define get_siopinib(siopid)  (&(siopinib_table[INDEX_SIOPINIB(siopid)]))

#define DEFAULT_PORT	1
#define	NS16550_DEV		0
#define FPGAUART_DEV	1


/*
 *  シリアルI/Oポート初期化ブロックの定義
 */
typedef struct sio_port_initialization_block {
	UW	reg_base;	/* レジスタのベースアドレス */
	UW	*control;
	UB	dev;		/* uart device */
	UB	intno;		/* 割込み番号 */
	UB	intpri;		/* 割込み優先度 */
} SIOPINIB;

/*
 *  シリアルI/Oポート管理ブロックの定義
 */
struct sio_port_control_block {
	const SIOPINIB	*siopinib; /* シリアルI/Oポート初期化ブロック */
	VP_INT	exinf;	/* 拡張情報 */
	BOOL	opnflg;	/* オープン済みフラグ */
};

#include "ns16550.h"
#include "fpgauart.h"

#ifdef USE_FPGAUART

/*
 * 文字を受信したか?
 */
static BOOL
uart_getready(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		return ns16550_getready(siopinib);
	}
	else {
		return fpgauart_getready(siopinib);
	}
}

/*
 * 文字を送信できるか?
 */
static BOOL
uart_putready(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		return ns16550_putready(siopinib);
	}
	else {
		return fpgauart_putready(siopinib);
	}
}

/*
 *  受信した文字の取り出し
 */
static UB
uart_getchar(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		return ns16550_getchar(siopinib);
	}
	else {
		return fpgauart_getchar(siopinib);
	}
}

/*
 *  送信する文字の書き込み
 */
static void
uart_putchar(const SIOPINIB *siopinib, UB c)
{
	if (siopinib->dev == NS16550_DEV) {
		ns16550_putchar(siopinib, c);
	}
	else {
		fpgauart_putchar(siopinib, c);
	}
}

/*
 *  送信割込み許可
 */
static void
uart_enable_send(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		ns16550_enable_send(siopinib);
	}
	else {
		fpgauart_enable_send(siopinib);
	}
}

/*
 *  送信割込み禁止
 */
static void
uart_disable_send(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		ns16550_disable_send(siopinib);
	}
	else {
		fpgauart_disable_send(siopinib);
	}
}

/*
 *  受信割込み許可
 */
static void
uart_enable_rcv(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		ns16550_enable_rcv(siopinib);
	}
	else {
		fpgauart_enable_rcv(siopinib);
	}
}

/*
 *  受信割込み禁止
 */
Inline void
uart_disable_rcv(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		ns16550_disable_rcv(siopinib);
	}
	else {
		fpgauart_disable_rcv(siopinib);
	}
}

/*
 * SIOPINIBの初期化
 */
static void
uart_init_siopinib(const SIOPINIB *siopinib)
{
	if (siopinib->dev == NS16550_DEV) {
		ns16550_init_siopinib(siopinib);
	}
	else {
		fpgauart_init_siopinib(siopinib);
	}
}

#else	/* USE_FPGAUART */

#define uart_getready	ns16550_getready
#define uart_putready	ns16550_putready
#define uart_getchar	ns16550_getchar
#define uart_putchar	ns16550_putchar
#define uart_enable_send	ns16550_enable_send
#define uart_disable_send	ns16550_disable_send
#define uart_enable_rcv	ns16550_enable_rcv
#define uart_disable_rcv	ns16550_disable_rcv
#define uart_init_siopinib	ns16550_init_siopinib

#endif	/* USE_FPGAUART */

static UW	dcontrol[TNUM_SIOP];

/*
 * シリアルI/Oポート初期化ブロック
 */
const SIOPINIB siopinib_table[TNUM_SIOP] = {
	{UART0_BASE, &dcontrol[0], NS16550_DEV, IREQ_UART0, 10},
#ifdef USE_FPGAUART
	{FUART_BASE, &dcontrol[1], FPGAUART_DEV, IREQ_F2S_FPGA_IRQ0, 10}
#else	/* USE_FPGAUART */
	{UART1_BASE, &dcontrol[1], NS16550_DEV, IREQ_UART1, 10}
#endif	/* USE_FPGAUART */
};

/*
 *  シリアルI/Oポート管理ブロックのエリア
 */
SIOPCB	siopcb_table[TNUM_SIOP];

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
		dcontrol[i] = 0;
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

	siopcb = get_siopcb(siopid);
	siopinib = siopcb->siopinib;

	/*
	 *  オープンしたポートがあるかをopnflgに読んでおく．
	 */
	opnflg = siopcb->opnflg;

	/*
	 * 初期化
	 */
	uart_init_siopinib(siopinib);

	/* 受信割込み許可 */
	uart_enable_rcv(siopcb->siopinib);
	/* 受信割込み禁止 */
	uart_disable_rcv(siopcb->siopinib);

	siopcb->exinf = exinf;

	/*
	 *  シリアルI/O割込みのマスクを解除する．
	 */
	if (!opnflg) {
		x_config_int(siopinib->intno, TRUE, siopinib->intpri);
	}
	siopcb->opnflg = TRUE;

	return(siopcb);
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
sio_cls_por(SIOPCB *siopcb)
{
	/* 割込み禁止 */
	uart_disable_rcv(siopcb->siopinib);
	uart_disable_send(siopcb->siopinib);
	siopcb->opnflg = FALSE;

	/*
	 *  シリアルI/O割込みをマスクする．
	 */
	if (!(siopcb->opnflg)) {
		dis_int(siopcb->siopinib->intno);
	}
}

/*
 *  UART0割込みハンドラ
 */
void
sio_handler0(void)
{
	SIOPCB	*siopcb = get_siopcb(1);

	if (uart_getready(siopcb->siopinib)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (uart_putready(siopcb->siopinib)) {
		/*
		 *  送信可能コールバックルーチンを呼び出す．
		 */
		sio_ierdy_snd(siopcb->exinf);
	}
}

/*
 *  UART1割込みハンドラ
 */
void
sio_handler1(void)
{
	SIOPCB	*siopcb = get_siopcb(2);

	if (uart_getready(siopcb->siopinib)) {
		/*
		 *  受信通知コールバックルーチンを呼び出す．
		 */
		sio_ierdy_rcv(siopcb->exinf);
	}
	if (uart_putready(siopcb->siopinib)) {
		/*
		 *  送信可能コールバックルーチンを呼び出す．
		 */
		sio_ierdy_snd(siopcb->exinf);
	}
}

/*
 *  シリアルI/Oポートへの文字送信
 */
BOOL
sio_snd_chr(SIOPCB *siopcb, char c)
{
	if (uart_putready(siopcb->siopinib)) {
		uart_putchar(siopcb->siopinib, c);
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
	if (uart_getready(siopcb->siopinib)) {
		return (INT)uart_getchar(siopcb->siopinib);
	}
	return -1;
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
sio_ena_cbr(SIOPCB *siopcb, UINT cbrtn)
{
	switch (cbrtn) {
	case SIO_ERDY_SND:
		uart_enable_send(siopcb->siopinib);
		break;
	case SIO_ERDY_RCV:
		uart_enable_rcv(siopcb->siopinib);
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
		uart_disable_send(siopcb->siopinib);
		break;
	case SIO_ERDY_RCV:
		uart_disable_rcv(siopcb->siopinib);
		break;
	}
}

/*
 *  1文字出力（ポーリングでの出力）
 */
void sio_snd_chr_pol(char c)
{
	const SIOPINIB *siopinib;
	ID	my_port_id = x_prc_index() + 1;

	if (my_port_id > TNUM_PORT) {
		my_port_id = DEFAULT_PORT;
	}
#ifdef USE_FPGAUART
	if (my_port_id == 2) {
		my_port_id = DEFAULT_PORT;
	}
#endif	/* USE_FPGAUART */

	siopinib = get_siopinib(my_port_id);
	uart_putchar(siopinib, c);
	while (!uart_putready(siopinib));

	/*
	 *  出力が完全に終わるまで待つ
	 */
	volatile int n = SOCFPGA_CORE_CLK / DEF_BAUDRATE;
	while(n--);
}

/*
 *  ターゲットのシリアル初期化
 */
void sio_init(void)
{
	const SIOPINIB  *siopinib;
	ID	my_port_id = x_prc_index() + 1;

	if (my_port_id > TNUM_PORT) {
		return;
	}
#ifdef USE_FPGAUART
	if (my_port_id == 2) {
		return;
	}
#endif	/* USE_FPGAUART */
	siopinib = get_siopinib(my_port_id);

	/*
	 * 初期化
	 */
	uart_init_siopinib(siopinib);
}
