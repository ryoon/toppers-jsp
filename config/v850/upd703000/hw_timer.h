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
 *  @(#) $Id: hw_timer.h,v 1.1 2000/11/24 03:41:21 takayuki Exp $
 */

/*
 *	ターゲット依存タイマモジュール（SM850 uPD703000用）
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include "sys_defs.h"

/*
 *  タイマ割込みハンドラのベクタ番号
 */
#define	INHNO_TIMER	0xd

/*
 *  タイマ値の内部表現の型
 */
typedef UW	CLOCK;

/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
hw_timer_initialize()
{
		/* タイマ初期化 */
	*(unsigned short *)COMPAREREGISTER = COMPAREVALUE;
	*(unsigned char *)TIMERMODECONTROL = 0x81;
	
		/* 割込みマスクとペンディングフラグを解除 */
	*(unsigned char *)TIMERINTCONTROL &= ~0xc0;
}

/*
 *  タイマ割込み要求のクリア
 */
Inline void
hw_timer_int_clear()
{
		/* ペンディングをマスクする */
	*(unsigned char *)TIMERINTCONTROL &= 0x7f;
}

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
Inline void
hw_timer_terminate()
{
		/* 割込みをマスクする */
	*(unsigned char *)TIMERINTCONTROL |= 0x40;
}

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
hw_timer_get_current(void)
{
	return *(unsigned short *)TIMERCOUNTER;
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
hw_timer_fetch_interrupt(void)
{
	return (*(unsigned char *)TIMERINTCONTROL & 0x80) != 0;
}

#endif /* _HW_TIMER_H_ */
