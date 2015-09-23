/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: serial.c,v 1.7 2001/11/12 13:23:22 hiro Exp $
 */

/*
 *	シリアルインタフェースドライバ
 */

#include <jsp_services.h>
#include "kernel_id.h"

/*
 *  シリアルインタフェースドライバの起動
 */

static BOOL	in_inirtn = FALSE;	/* 初期化ルーチン実行中を示す */

void
serial_initialize(VP_INT portid)
{
	in_inirtn = TRUE;
	syscall(serial_open((ID) portid));
	in_inirtn = FALSE;
	syslog_1(LOG_NOTICE, "Serial driver service starts on port %d.",
							portid);
}

/*
 *  シリアルポート管理ブロックの定義
 */

#define	SERIAL_BUFSZ	256	/* シリアルインタフェース用バッファのサイズ */

#define	inc(x)		(((x)+1 < SERIAL_BUFSZ) ? (x)+1 : 0)
#define	INC(x)		((x) = inc(x))

typedef struct serial_port_control_block {
	BOOL	init_flag;	/* 初期化済か？ */
	HWPORT	hwport;		/* ハードウェア依存情報 */
	ID	in_semid;	/* 受信バッファ管理用セマフォの ID */
	ID	out_semid;	/* 送信バッファ管理用セマフォの ID */

	int	in_read_ptr;	/* 受信バッファ読み出しポインタ */
	int	in_write_ptr;	/* 受信バッファ書き込みポインタ */
	int	out_read_ptr;	/* 送信バッファ読み出しポインタ */
	int	out_write_ptr;	/* 送信バッファ書き込みポインタ */
	UINT	ioctl;		/* ioctl による設定内容 */
	BOOL	send_enabled;	/* 送信をイネーブルしてあるか？ */
	BOOL	ixon_stopped;	/* STOP を受け取った状態か？ */
	BOOL	ixoff_stopped;	/* 相手に STOP を送った状態か？ */
	char	ixoff_send;	/* 相手に START/STOP を送るか？ */

	char	in_buffer[SERIAL_BUFSZ];	/* 受信バッファエリア */
	char	out_buffer[SERIAL_BUFSZ];	/* 受信バッファエリア */
} SPCB;

#define	IN_BUFFER_EMPTY(spcb) \
		((spcb)->in_read_ptr == (spcb)->in_write_ptr)
#define	IN_BUFFER_FULL(spcb) \
		((spcb)->in_read_ptr == inc((spcb)->in_write_ptr))
#define	OUT_BUFFER_FULL(spcb) \
		((spcb)->out_read_ptr == inc((spcb)->out_write_ptr))

/*
 *  モジュール内で使う関数
 */
static BOOL	serial_getc(SPCB *spcb, char *c);
static BOOL	serial_putc(SPCB *spcb, char c);

/*
 *  シリアルポート管理ブロックの定義と初期化
 */

static SPCB spcb_table[NUM_PORT] = {
	{ 0, HWPORT1, SEM_SERIAL1_IN, SEM_SERIAL1_OUT },
#if NUM_PORT > 1
	{ 0, HWPORT2, SEM_SERIAL2_IN, SEM_SERIAL2_OUT },
#endif
};

#define get_spcb(portid)	(&(spcb_table[(portid)-1]))
#define get_spcb_def(portid)	get_spcb((portid) ? (portid) : CONSOLE_PORTID)

/*
 *  ポートのオープン
 *
 *  複数のタスクが同時に呼ぶ状況には対応していない．
 */
ER
serial_open(ID portid)
{
	SPCB	*spcb;
	ER	ercd;

	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_ID);
	}
	spcb = get_spcb(portid);

	if (spcb->init_flag) {		/* 初期化済かのチェック */
		return(E_OK);
	}

	/*
	 *  変数の初期化
	 */
	spcb->in_read_ptr = spcb->in_write_ptr = 0;
	spcb->out_read_ptr = spcb->out_write_ptr = 0;
	spcb->ioctl = (IOCTL_ECHO | IOCTL_CRLF | IOCTL_CANONICAL
				| IOCTL_IXON | IOCTL_IXOFF);
	spcb->send_enabled = FALSE;
	spcb->ixon_stopped = spcb->ixoff_stopped = FALSE;
	spcb->ixoff_send = 0;

	/*
	 *  ハードウェア依存の初期化
	 */
	if (!in_inirtn) {
		syscall(loc_cpu());
	}
	if (hw_port_initialize(&(spcb->hwport))) {
		ercd = E_SYS;
	}
	else {
		spcb->init_flag = TRUE;
		ercd = E_OK;
	}
	if (!in_inirtn) {
		syscall(unl_cpu());
	}
	return(ercd);
}

/*
 *  ポートのクローズ
 *
 *  flush が TRUE の場合は，シリアルポートへの送信バッファが空になるま
 *  で待つ．
 *  複数のタスクが同時に呼ぶ状況には対応していない．
 */

#define	MAX_FLUSH_LOOP	1000000

ER
serial_close(ID portid, BOOL flush)
{
	SPCB	*spcb;
	int	i;

	if (!(1 <= portid && portid <= NUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}

	spcb = get_spcb(portid);
	if (!(spcb->init_flag)) {	/* 初期化済かのチェック */
		return(E_OBJ);
	}

	/*
	 *  バッファのフラッシュ処理
	 */
	if (flush) {
		for (i = 0; i < MAX_FLUSH_LOOP; i++) {
			if (spcb->out_write_ptr == spcb->out_read_ptr) {
				break;
			}
		}
	}

	/*
	 *  ハードウェア依存のシャットダウン処理
	 */
	syscall(loc_cpu());
	hw_port_terminate(&(spcb->hwport));
	syscall(unl_cpu());

	spcb->init_flag = FALSE;
	return(E_OK);
}

/*
 *  フローコントロール関係の定義
 */
#define	STOP	'\023'		/* コントロール-S */
#define	START	'\021'		/* コントロール-Q */

#define	IXOFF_STOP	64	/* STOPを送る残りバッファエリア */
#define	IXOFF_START	128	/* STARTを送る残りバッファエリア */

#define	in_buf_area(spcb)						\
		((spcb->in_read_ptr >= spcb->in_write_ptr) ?		\
		 (spcb->in_read_ptr - spcb->in_write_ptr) :		\
		 (spcb->in_read_ptr + SERIAL_BUFSZ - spcb->in_write_ptr))

/*
 *  ユーティリティルーチン
 */

Inline BOOL
enable_send(SPCB *spcb, char c)
{
	if (!(spcb->send_enabled)) {
		hw_port_sendstart(&(spcb->hwport));
		spcb->send_enabled = TRUE;
		if (hw_port_putready(&(spcb->hwport))) {
			hw_port_putchar(&(spcb->hwport), c);
			return(TRUE);
		}
	}
	return(FALSE);
}

/*
 *  シリアルポートからの受信
 */

static BOOL
serial_getc(SPCB *spcb, char *c)
{
	BOOL	buffer_empty;

	syscall(loc_cpu());
	*c = spcb->in_buffer[spcb->in_read_ptr];
	INC(spcb->in_read_ptr);

	if (*c == '\r' && (spcb->ioctl & IOCTL_RAW) == 0) {
		*c = '\n';
	}

	if (spcb->ixoff_stopped && (in_buf_area(spcb) > IXOFF_START)) {
		if (!enable_send(spcb, START)) {
			spcb->ixoff_send = START;
		}
		spcb->ixoff_stopped = FALSE;
	}
	buffer_empty = IN_BUFFER_EMPTY(spcb);
	syscall(unl_cpu());
	return(buffer_empty);
}

ER_UINT
serial_read(ID portid, char *buf, UINT len)
{
	SPCB		*spcb;
	BOOL		buffer_empty;
	char		c;
	unsigned int	i;

	if (sns_dpn()) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}

	spcb = get_spcb_def(portid);
	if (!(spcb->init_flag)) {	/* 初期化済かのチェック */
		return(E_OBJ);
	}
	if (len == 0) {
		return(len);
	}

	syscall(wai_sem(spcb->in_semid));
	buffer_empty = FALSE;
	for (i = 0; i < len; i++) {
		buffer_empty = serial_getc(spcb, &c);
		if ((spcb->ioctl & IOCTL_ECHO) != 0) {
			syscall(wai_sem(spcb->out_semid));
			if (!serial_putc(spcb, c)) {
				syscall(sig_sem(spcb->out_semid));
			}
		}
		*buf++ = c;
		if ((spcb->ioctl & IOCTL_RAW) != 0
				|| ((spcb->ioctl & IOCTL_CANONICAL) != 0
						&& c == '\n')) {
			len = i + 1;
			break;
		}
		if (buffer_empty && i < len - 1) {
			syscall(wai_sem(spcb->in_semid));
		}
	}
	if (!buffer_empty) {
		syscall(sig_sem(spcb->in_semid));
	}
	return(len);
}

/*
 *  シリアルポートへの送信
 */

static BOOL
serial_putc(SPCB *spcb, char c)
{
	BOOL	buffer_full;

	if (c == '\n' && (spcb->ioctl & IOCTL_CRLF) != 0) {
		if (serial_putc(spcb, '\r')) {
			syscall(wai_sem(spcb->out_semid));
		}
	}

	syscall(loc_cpu());
	if (!(spcb->ixon_stopped) && enable_send(spcb, c)) {
		buffer_full = FALSE;
	}
	else {
		spcb->out_buffer[spcb->out_write_ptr] = c;
		INC(spcb->out_write_ptr);
		buffer_full = OUT_BUFFER_FULL(spcb);
	}
	syscall(unl_cpu());
	return(buffer_full);
}

ER_UINT
serial_write(ID portid, char *buf, UINT len)
{
	SPCB		*spcb;
	BOOL		buffer_full;
	unsigned int	i;

	if (sns_dpn()) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}

	spcb = get_spcb_def(portid);
	if (!(spcb->init_flag)) {	/* 初期化済かのチェック */
		return(E_OBJ);
	}

	syscall(wai_sem(spcb->out_semid));
	buffer_full = FALSE;
	for (i = 0; i < len; i++) {
		buffer_full = serial_putc(spcb, *buf++);
		if (buffer_full && i < len - 1) {
			syscall(wai_sem(spcb->out_semid));
		}
	}
	if (!buffer_full) {
		syscall(sig_sem(spcb->out_semid));
	}
	return(len);
}

/*
 *  シリアルポートの制御
 */
ER
serial_ioctl(ID portid, UINT ioctl)
{
	SPCB	*spcb;

	if (sns_ctx()) {		/* コンテキストのチェック */
		return(E_CTX);
	}
	if (!(0 <= portid && portid <= NUM_PORT)) {
		return(E_ID);		/* ポート番号のチェック */
	}

	spcb = get_spcb_def(portid);
	if (!(spcb->init_flag)) {	/* 初期化済かのチェック */
		return(E_OBJ);
	}

	spcb->ioctl = ioctl;
	return(E_OK);
}

/*
 *  シリアルポート割込みサービスルーチン
 */

void
serial_handler_in(ID portid)
{
	SPCB	*spcb;
	char	c;

	spcb = get_spcb(portid);
	if (hw_port_getready(&(spcb->hwport))) {
		c = hw_port_getchar(&(spcb->hwport));
		if ((spcb->ioctl & IOCTL_IXON) != 0 && c == STOP) {
			spcb->ixon_stopped = TRUE;
		}
		else if (((spcb->ioctl & IOCTL_IXON) != 0
					|| spcb->ixon_stopped)
				&& (c == START
					|| (spcb->ioctl & IOCTL_IXANY) != 0)) {
			spcb->ixon_stopped = FALSE;
			if (!(spcb->send_enabled)) {
				hw_port_sendstart(&(spcb->hwport));
				spcb->send_enabled = TRUE;
			}
		}
		else if (IN_BUFFER_FULL(spcb)) {
			/* バッファフルの場合，受信した文字を捨てる．*/
		}
		else {
			spcb->in_buffer[spcb->in_write_ptr] = c;
			if (spcb->in_read_ptr == spcb->in_write_ptr) {
				syscall(isig_sem(spcb->in_semid));
			}
			INC(spcb->in_write_ptr);

			if ((spcb->ioctl & IOCTL_IXOFF) != 0
					&& !(spcb->ixoff_stopped)
					&& (in_buf_area(spcb) < IXOFF_STOP)) {
				if (!enable_send(spcb, STOP)) {
					spcb->ixoff_send = STOP;
				}
				spcb->ixoff_stopped = TRUE;
			}
		}
	}
}

void
serial_handler_out(ID portid)
{
	SPCB	*spcb;

	spcb = get_spcb(portid);
	if (hw_port_putready(&(spcb->hwport))) {
		if (!spcb->send_enabled) {
			hw_port_sendstop(&(spcb->hwport));
		}
		else if (spcb->ixoff_send) {
			hw_port_putchar(&(spcb->hwport), spcb->ixoff_send);
			spcb->ixoff_send = 0;
		}
		else if (spcb->ixon_stopped
				|| spcb->out_read_ptr == spcb->out_write_ptr) {
			/* 送信停止 */
			hw_port_sendstop(&(spcb->hwport));
			spcb->send_enabled = FALSE;
		}
		else {
			hw_port_putchar(&(spcb->hwport),
					spcb->out_buffer[spcb->out_read_ptr]);
			if (OUT_BUFFER_FULL(spcb)) {
				syscall(isig_sem(spcb->out_semid));
			}
			INC(spcb->out_read_ptr);
		}
	}
}

/*
 *  割込みハンドラ
 */

#ifdef SEPARATE_SIO_INT

void
serial_in_handler(void)
{
	hw_serial_handler_in();
}

void
serial_out_handler(void)
{
	hw_serial_handler_out();
}

#else /* SEPARATE_SIO_INT */

void
serial_handler(void)
{
	hw_serial_handler();
}

#endif /* SEPARATE_SIO_INT */
