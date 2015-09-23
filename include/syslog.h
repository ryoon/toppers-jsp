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
 *  @(#) $Id: syslog.h,v 1.1 2000/11/14 14:44:19 hiro Exp $
 */

/*
 *	システムログサービス
 *
 *  システムログサービスは，システムのログ情報を出力するためのサービス
 *  である．カーネルからのログ情報の出力にも用いるため，内部で待ち状態
 *  にはいることはない．
 *  ログ情報は，メモリ上のリングバッファに書き込むか，定レベルの出力ルー
 *  チンを使って出力する．どちらを使うかは，グローバル変数で制御するこ
 *  とができる．
 *  リングバッファがオーバーフローした場合には，古いログ情報を消して上
 *  書きする．
 */

#ifndef _SYSLOG_H_
#define _SYSLOG_H_

#include <itron.h>
#include <stdarg.h>

/*
 *  システムログサービスの初期化
 */
extern void	syslog_initialize(void);

/*
 *  システムログ出力関数
 *
 *  システムログへメッセージを出力するための関数．UNIX のインタフェー
 *  スと近いインタフェースにしてあるが，facility は使っていない．
 */
extern void	syslog(int priority, const char *format, ...);

/* 
 *  ログマスクの設定
 */
extern int	setlogmask(int maskpri);

/*
 *  priority の定義
 */
#define LOG_EMERG	0		/* システムダウンに値するエラー */
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3		/* システムエラー */
#define LOG_WARNING	4
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7		/* デバッグ用メッセージ */

/*
 *  setlogmask の引数を作るためのマクロ
 */
#define LOG_MASK(pri)	(1 << (pri))
#define LOG_UPTO(pri)	((1 << ((pri) + 1)) - 1)

/*
 *  リングバッファからの読み出し
 *
 *  システムログタスクが用いるための関数．リングバッファが空の時は，負
 *  の値を返す．
 */
extern int	syslog_read(void);

/* 
 *  ログモードの変更
 */
extern int	setlogmode(int mode);

#endif /* _SYSLOG_H_ */
