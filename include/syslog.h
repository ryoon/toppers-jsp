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
 *  @(#) $Id: syslog.h,v 1.5 2001/10/08 01:46:03 hiro Exp $
 */

/*
 *	システムログ機能
 *
 *  システムログサービスは，システムのログ情報を出力するためのサービス
 *  である．カーネルからのログ情報の出力にも用いるため，内部で待ち状態
 *  にはいることはない．
 *  ログ情報は，カーネル内のログバッファに書き込むか，低レベルの文字出
 *  力関数を用いて出力する．どちらを使うかは，拡張サービスコールで切り
 *  換えることができる．
 *  ログバッファ領域がオーバフローした場合には，古いログ情報を消して上
 *  書きする．
 */

#ifndef _SYSLOG_H_
#define _SYSLOG_H_

#include <itron.h>

/*
 *  ログ情報のデータ構造
 */

#define TMAX_LOGINFO	6

typedef struct {
		UINT	logtype;		/* ログ情報の種別 */
		SYSTIM	logtim;			/* ログ時刻 */
		VP_INT	loginfo[TMAX_LOGINFO];	/* その他のログ情報 */
	} SYSLOG;

/*
 *  ログ情報の重要度の定義
 */
#define LOG_EMERG	0		/* シャットダウンに値するエラー */
#define LOG_ALERT	1
#define LOG_CRIT	2
#define LOG_ERR		3		/* システムエラー */
#define LOG_WARNING	4		/* 警告メッセージ */
#define LOG_NOTICE	5
#define LOG_INFO	6
#define LOG_DEBUG	7		/* デバッグ用メッセージ */

/*
 *  ログ情報の重要度のビットマップを作るためのマクロ
 */
#define LOG_MASK(prio)	(1 << (prio))
#define LOG_UPTO(prio)	((1 << ((prio) + 1)) - 1)

/*
 *  ログ情報の種別の定義
 */
#define LOG_TYP_INTERRUPT	0x01	/* 割込みハンドラ */
#define LOG_TYP_ISR		0x02	/* 割込みサービスハンドラ */
#define LOG_TYP_TIMERHDR	0x03	/* タイムイベントハンドラ */
#define LOG_TYP_CPUEXC		0x04	/* CPU例外ハンドラ */
#define LOG_TYP_TSKEXC		0x05	/* タスク例外処理ルーチン */
#define LOG_TYP_TSKSTAT		0x06	/* タスク状態変化 */
#define LOG_TYP_DISPATCH	0x07	/* ディスパッチャ */
#define LOG_TYP_SVC		0x08	/* サービスコール */
#define LOG_TYP_COMMENT		0x09	/* コメント（文字列のみのログ） */
#define LOG_ENTER		0x00	/* 入口／開始 */
#define LOG_LEAVE		0x80	/* 出口／終了 */

/*
 *  ログ情報の出力
 */
extern ER	syslog_write(UINT prio, SYSLOG *p_syslog);

/*
 *  ログバッファからのログ情報の読出し
 */
extern ER_UINT	syslog_read(SYSLOG *p_syslog);

/* 
 *  出力すべきログ情報の重要度の設定
 */
extern ER	syslog_setmask(UINT logmask, UINT lowmask);

/*
 *  ログ情報を出力するためのマクロ
 *
 *  format および後続の引数から作成したメッセージを，重大度 prio で
 *  ログ情報として出力するためのマクロ．arg1〜argn は VP_INT型にキャ
 *  ストするため，VP_INT型に型変換できる任意の型でよい．
 */

#define	syslog_0(prio, format)	_syslog_0(prio, format)

#define	syslog_1(prio, format, arg1) \
		_syslog_1(prio, format, (VP_INT)(arg1))

#define	syslog_2(prio, format, arg1, arg2) \
		_syslog_2(prio, format, (VP_INT)(arg1), (VP_INT)(arg2))

#define	syslog_3(prio, format, arg1, arg2, arg3) \
		_syslog_3(prio, format, (VP_INT)(arg1), (VP_INT)(arg2),	\
						(VP_INT)(arg3))

#define	syslog_4(prio, format, arg1, arg2, arg3, arg4) \
		_syslog_4(prio, format, (VP_INT)(arg1), (VP_INT)(arg2),	\
				(VP_INT)(arg3), (VP_INT)(arg4))

#define	syslog_5(prio, format, arg1, arg2, arg3, arg4, arg5) \
		_syslog_5(prio, format, (VP_INT)(arg1), (VP_INT)(arg2),	\
			(VP_INT)(arg3), (VP_INT)(arg4), (VP_INT)(arg5))

/*
 *  ログ情報を出力するためのライブラリ関数
 */

Inline ER
_syslog_0(UINT prio, const char *format)
{
	SYSLOG	syslog;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	return(syslog_write(prio, &syslog));
}

Inline ER
_syslog_1(UINT prio, const char *format, VP_INT arg1)
{
	SYSLOG	syslog;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	syslog.loginfo[1] = arg1;
	return(syslog_write(prio, &syslog));
}

Inline ER
_syslog_2(UINT prio, const char *format, VP_INT arg1, VP_INT arg2)
{
	SYSLOG	syslog;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	syslog.loginfo[1] = arg1;
	syslog.loginfo[2] = arg2;
	return(syslog_write(prio, &syslog));
}

Inline ER
_syslog_3(UINT prio, const char *format, VP_INT arg1,
				VP_INT arg2, VP_INT arg3)
{
	SYSLOG	syslog;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	syslog.loginfo[1] = arg1;
	syslog.loginfo[2] = arg2;
	syslog.loginfo[3] = arg3;
	return(syslog_write(prio, &syslog));
}

Inline ER
_syslog_4(UINT prio, const char *format, VP_INT arg1, VP_INT arg2,
				VP_INT arg3, VP_INT arg4)
{
	SYSLOG	syslog;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	syslog.loginfo[1] = arg1;
	syslog.loginfo[2] = arg2;
	syslog.loginfo[3] = arg3;
	syslog.loginfo[4] = arg4;
	return(syslog_write(prio, &syslog));
}

Inline ER
_syslog_5(UINT prio, const char *format, VP_INT arg1, VP_INT arg2,
				VP_INT arg3, VP_INT arg4, VP_INT arg5)
{
	SYSLOG	syslog;

	syslog.logtype = LOG_TYP_COMMENT;
	syslog.loginfo[0] = (VP_INT) format;
	syslog.loginfo[1] = arg1;
	syslog.loginfo[2] = arg2;
	syslog.loginfo[3] = arg3;
	syslog.loginfo[4] = arg4;
	syslog.loginfo[5] = arg5;
	return(syslog_write(prio, &syslog));
}

/*
 *  ログ情報を出力するためのライブラリ関数（可変数引数版）
 */
extern ER	syslog(UINT prio, const char *format, ...);

/* 
 *  ログ情報のフォーマット出力
 */
extern void	syslog_printf(SYSLOG *p_syslog, void (*putc)(char));

#endif /* _SYSLOG_H_ */
