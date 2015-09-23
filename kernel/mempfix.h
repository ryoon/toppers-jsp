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
 *  @(#) $Id: mempfix.h,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	固定長メモリプール機能
 */

#ifndef _MEMPFIX_H_
#define _MEMPFIX_H_

#include "queue.h"

/*
 *  固定長メモリプール初期化ブロック
 */
typedef struct fixed_memorypool_initialization_block {
	ATR	mpfatr;		/* 固定長メモリプール属性 */
	UINT	blksz;		/* メモリブロックのサイズ（丸めた値） */
	VP	mpf;		/* 固定長メモリプール領域の先頭番地 */
	VP	limit;		/* 固定長メモリプール領域の上限番地 */
} MPFINIB;

/*
 *  空ブロックリストの定義
 */
typedef struct free_list {
	struct free_list *next;
} FREEL;

/*
 *  固定長メモリプール管理ブロック
 */
typedef struct fixed_memorypool_control_block {
	QUEUE	wait_queue;	/* 固定長メモリプール待ちキュー */
	const MPFINIB *mpfinib;	/* 固定長メモリプール初期化ブロック */
	VP	unused;		/* 未使用領域の先頭番地 */
	FREEL	*freelist;	/* 空きブロックのリスト */
} MPFCB;

/*
 *  固定長メモリプール機能の初期化
 */
extern void	mempfix_initialize(void);

#endif /* _MEMPFIX_H_ */
