/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: serial.h,v 1.1 2000/11/14 14:44:19 hiro Exp $
 */

/*
 *	シリアルインタフェースドライバ
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <itron.h>
#include "hw_serial.h"

/*
 *  シリアルインタフェースドライバの起動
 *
 *  カーネルに初期化ルーチンとして登録することを想定している．exinf に
 *  は，起動したいシリアルポートのID番号を渡す．
 */
extern void	serial_initialize(VP_INT portid);

/*
 *  シリアルインタフェースドライバの割込みハンドラ
 */
#ifdef SIO_INT_SEPARATE
extern void	serial_in_handler(void);
extern void	serial_out_handler(void);
#else /* SIO_INT_SEPARATE */
extern void	serial_handler(void);
#endif /* SIO_INT_SEPARATE */

/*
 *  シリアルインタフェースドライバの動作制御用のための定数
 *
 *  以下の定数は，ビット毎に論理和をとって用いる．
 */
#define	IOCTL_NULL	0		/* 指定なし */
#define	IOCTL_ECHO	0x0001		/* 入力した文字をエコーバック */
#define	IOCTL_CRLF	0x0010		/* LF を出力する際に CR を付加 */
#define	IOCTL_RAW	0x0100		/* 1文字単位で入力 */
#define	IOCTL_CANONICAL	0x0200		/* LF が来るまで入力 */
#define	IOCTL_IXON	0x1000		/* 出力に対して XON/XOFF 制御 */
#define	IOCTL_IXANY	0x2000		/* どのような文字でも出力開始 */
#define	IOCTL_IXOFF	0x4000		/* 入力に対して XON/XOFF 制御 */

/*
 *  シリアルインタフェースドライバのサービスコール
 */
extern ER	serial_open(ID portid);
extern ER	serial_close(ID portid, BOOL flush);
extern ER_UINT	serial_read(ID portid, char *buf, UINT len);
extern ER_UINT	serial_write(ID portid, char *buf, UINT len);
extern ER	serial_ioctl(ID portid, UINT ioctl);

#endif /* _SERIAL_H_ */
