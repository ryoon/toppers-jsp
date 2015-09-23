/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: cyclic.h,v 1.2 2001/09/06 12:39:51 hiro Exp $
 */

/*
 *	周期ハンドラ機能
 */

#ifndef _CYCLIC_H_
#define _CYCLIC_H_

#include "queue.h"
#include "time_event.h"

/*
 *  周期ハンドラ初期化ブロック
 */
typedef struct cyclic_handler_initialization_block {
	ATR	cycatr;		/* 周期ハンドラ属性 */
	VP_INT	exinf;		/* 周期ハンドラの拡張情報 */
	FP	cychdr;		/* 周期ハンドラの起動番地 */
	RELTIM	cyctim;		/* 周期ハンドラの起動周期 */
	RELTIM	cycphs;		/* 周期ハンドラの起動位相 */
} CYCINIB;

/*
 *  周期ハンドラ管理ブロック
 */
typedef struct cyclic_handler_control_block {
	const CYCINIB *cycinib;	/* 周期ハンドラ初期化ブロックへのポインタ */
	BOOL	cycsta;		/* 周期起動ハンドラ動作状態 */
	EVTTIM	evttim;		/* 次に周期ハンドラを起動する時刻 */
	TMEVTB	tmevtb;		/* タイムイベントブロック */
} CYCCB;

/*
 *  周期ハンドラ機能の初期化
 */
extern void	cyclic_initialize(void);

#endif /* _CYCLIC_H_ */
