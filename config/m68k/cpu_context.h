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
 *  @(#) $Id: cpu_context.h,v 1.1 2000/11/14 14:44:14 hiro Exp $
 */

/*
 *	タスクコンテキスト操作ルーチン
 *
 *  このファイルを cpu_config.h と分離しているのは，このファイルは TCB
 *  が定義された後に読む必要があるのに対して，cpu_config.h には TCB を
 *  定義する前に読む必要がある定義を含んでいるためである．
 */

#ifndef _CPU_CONTEXT_H_
#define _CPU_CONTEXT_H_

#include "task.h"

/*
 *  タスクコンテキストの初期化
 *
 *  タスクが休止状態に移行する時（タスクの生成時，タスクの終了時）に呼
 *  ばれる．基本的には，タスクコンテキストをタスクが起動できる状態に設
 *  定する処理を，create_context と activate_context で行えばよい．
 */
Inline void
create_context(TCB *tcb)
{
}

/*
 *  タスクの起動準備
 *
 *  タスクが休止状態から実行できる状態に移行する時に呼ばれる．
 */
extern void	activate_r(void);

Inline void
activate_context(TCB *tcb)
{
	VW	*sp;

	sp = (VW *)(((VB *) tcb->tinib->stk) + tcb->tinib->stksz);
	*--sp = (VW)(tcb->tinib->exinf);
	*--sp = (VW) ext_tsk;
	*--sp = (VW)(tcb->tinib->task);
	tcb->tskctxb.msp = sp;
	tcb->tskctxb.pc = activate_r;
}

/*
 *  ext_tsk がスタック上に確保するダミー領域のサイズ
 */
#define	ACTIVATED_STACK_SIZE	(sizeof(VW) * 3)

#endif /* _CPU_CONTEXT_H_ */
