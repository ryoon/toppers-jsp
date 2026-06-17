/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
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
 *  @(#) $Id: time_event.c,v 1.9 2006/02/12 05:28:07 hiro Exp $
 */

/*
 *	タイムイベント管理モジュール
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "time_event.h"
#include "cyclic.h"

/*
 *  イベント発生時刻比較マクロ
 *
 *  イベント発生時刻は，current_time からの相対値で比較する．すなわち，
 *  current_time を最小値（最も近い時刻），current_time - 1 が最大値
 *  （最も遠い時刻）とみなして比較する．
 */
#define	EVTTIM_LT(t, t1, t2) (((t1) - (t)->current_time) < ((t2) - (t)->current_time))
#define	EVTTIM_LE(t, t1, t2) (((t1) - (t)->current_time) <= ((t2) - (t)->current_time))

/*
 *  TEVTCBの数とインデックス番号を定義
 */
#ifndef TOPPERS_SYSTIM_GLOBAL
#if TNUM_PRCID > 1
#define	TEVTCB_IDX1	1
#if TNUM_PRCID > 2
#define	TEVTCB_IDX2	2
#if TNUM_PRCID > 3
#define	TEVTCB_IDX3	3
#endif	/* (3) */
#endif	/* (2) */
#endif	/* (1) */

#else	/* TOPPERS_SYSTIM_GLOBAL */
#define	TEVTCB_IDX1	0
#define	TEVTCB_IDX2	0
#define	TEVTCB_IDX3	0

#endif	/* TOPPERS_SYSTIM_GLOBAL */

#ifdef __tmeini

/*
 *  タイムイベントコントロールブロック
 */
TEVTCB p_tevtcb[TNUM_TEVTCB];
TEVTCB* const p_tevtcb_table[TNUM_PRCID] = {
	&p_tevtcb[0],
#if TNUM_PRCID > 1
	&p_tevtcb[TEVTCB_IDX1],
#endif
#if TNUM_PRCID > 2
	&p_tevtcb[TEVTCB_IDX2],
#endif
#if TNUM_PRCID > 3
	&p_tevtcb[TEVTCB_IDX3]
#endif
};

/*
 *  タイマモジュールの初期化
 */
void
tmevt_initialize(void)
{
	TPCB	*my_tpcb = get_my_tpcb();
	TEVTCB	*my_tevtcb;
	UINT	unum = tmax_tskid + tmax_cycid;
	UINT	idx = get_prc_index();

	my_tpcb->tevtcb = (TEVTCB*)p_tevtcb_table[idx];
	my_tevtcb = my_tpcb->tevtcb;

#ifdef TOPPERS_SYSTIM_GLOBAL
	if (!is_master_proc()) {
		return;
	}
#endif	/* TOPPERS_SYSTIM_GLOBAL */

	my_tevtcb->ptmevt_heap = &tmevt_heap[unum * idx];
	my_tevtcb->systim_offset = 0;
	my_tevtcb->current_time = 0;
#if TIC_DENO == 1
	my_tevtcb->next_time = my_tevtcb->current_time + TIC_NUME;
#else /* TIC_DENO == 1 */
	my_tevtcb->next_subtime += TIC_NUME;
	my_tevtcb->next_time = my_tevtcb->current_time + my_tevtcb->next_subtime / TIC_DENO;
	my_tevtcb->next_subtime %= TIC_DENO;
#endif /* TIC_DENO == 1 */
	my_tevtcb->last_index = 0;
}

#endif /* __tmeini */

/*
 *  タイムイベントの挿入位置を上向きに探索
 *
 *  時刻 time に発生するタイムイベントを挿入するノードを空けるために，
 *  ヒープの上に向かって空ノードを移動させる．移動前の空ノードの位置を 
 *  index に渡すと，移動後の空ノードの位置（すなわち挿入位置）を返す．
 */
#ifdef __tmeup

UINT
tmevt_up(TEVTCB *tevtcb, UINT index, EVTTIM time)
{
	UINT	parent;

	while (index > 1) {
		/*
		 *  親ノードのイベント発生時刻の方が早い（または同じ）
		 *  ならば，index が挿入位置なのでループを抜ける．
		 */
		parent = PARENT(index);
		if (EVTTIM_LE(tevtcb, TMEVT_NODE(tevtcb, parent).time, time)) {
			break;
		}

		/*
		 *  親ノードを index の位置に移動させる．
		 */
		TMEVT_NODE(tevtcb, index) = TMEVT_NODE(tevtcb, parent);
		TMEVT_NODE(tevtcb, index).tmevtb->index = index;

		/*
		 *  index を親ノードの位置に更新．
		 */
		index = parent;
	}
	return(index);
}

#endif /* __tmeup */

/*
 *  タイムイベントの挿入位置を下向きに探索
 *
 *  時刻 time に発生するタイムイベントを挿入するノードを空けるために，
 *  ヒープの下に向かって空ノードを移動させる．移動前の空ノードの位置を 
 *  index に渡すと，移動後の空ノードの位置（すなわち挿入位置）を返す．
 */
#ifdef __tmedown

UINT
tmevt_down(TEVTCB *tevtcb, UINT index, EVTTIM time)
{
	UINT	child;

	while ((child = LCHILD(index)) <= tevtcb->last_index) {
		/*
		 *  左右の子ノードのイベント発生時刻を比較し，早い方の
		 *  子ノードの位置を child に設定する．以下の子ノード
		 *  は，ここで選ばれた方の子ノードのこと．
		 */
		if (child + 1 <= tevtcb->last_index
			&& EVTTIM_LT(tevtcb, TMEVT_NODE(tevtcb, child + 1).time,
				  TMEVT_NODE(tevtcb, child).time)) {
			child = child + 1;
		}

		/*
		 *  子ノードのイベント発生時刻の方が遅い（または同じ）
		 *  ならば，index が挿入位置なのでループを抜ける．
		 */
		if (EVTTIM_LE(tevtcb, time, TMEVT_NODE(tevtcb, child).time)) {
			break;
		}

		/*
		 *  子ノードを index の位置に移動させる．
		 */
		TMEVT_NODE(tevtcb, index) = TMEVT_NODE(tevtcb, child);
		TMEVT_NODE(tevtcb, index).tmevtb->index = index;

		/*
		 *  index を子ノードの位置に更新．
		 */
		index = child;
	}
	return(index);
}

#endif /* __tmedown */

/*
 * タイムイベントヒープへの登録
 *
 *  タイムイベントブロック tmevtb を，time で指定した時間が経過後にイ
 *  ベントが発生するように，タイムイベントヒープに登録する．
 */
#ifdef __tmeins

void
tmevtb_insert(TEVTCB *tevtcb, TMEVTB *tmevtb, EVTTIM time)
{
	UINT	index;

	/*
	 *  last_index をインクリメントし，そこから上に挿入位置を探す．
	 */
	index = tmevt_up(tevtcb, ++tevtcb->last_index, time);

	/*
	 *  タイムイベントを index の位置に挿入する．
	 */ 
	TMEVT_NODE(tevtcb, index).time = time;
	TMEVT_NODE(tevtcb, index).tmevtb = tmevtb;
	tmevtb->index = index;
}

#endif /* __tmeins */

/*
 *  タイムイベントヒープからの削除
 */
#ifdef __tmedel

void
tmevtb_delete(TEVTCB *tevtcb, TMEVTB *tmevtb)
{
	UINT	index = tmevtb->index;
	UINT	parent;
	EVTTIM	event_time = TMEVT_NODE(tevtcb, tevtcb->last_index).time;

	/*
	 *  削除によりタイムイベントヒープが空になる場合は何もしない．
	 */
	if (--tevtcb->last_index == 0) {
		return;
	}

	/*
	 *  削除したノードの位置に最後のノード（last_index + 1 の位置
	 *  のノード）を挿入し，それを適切な位置へ移動させる．実際には，
	 *  最後のノードを実際に挿入するのではなく，削除したノードの位
	 *  置が空ノードになるので，最後のノードを挿入すべき位置へ向け
	 *  て空ノードを移動させる．
	 *  最後のノードのイベント発生時刻が，削除したノードの親ノード
	 *  のイベント発生時刻より前の場合には，上に向かって挿入位置を
	 *  探す．そうでない場合には，下に向かって探す．
	 */
	if (index > 1 && EVTTIM_LT(tevtcb, event_time,
				TMEVT_NODE(tevtcb, parent = PARENT(index)).time)) {
		/*
		 *  親ノードを index の位置に移動させる．
		 */
		TMEVT_NODE(tevtcb, index) = TMEVT_NODE(tevtcb, parent);
		TMEVT_NODE(tevtcb, index).tmevtb->index = index;

		/*
		 *  削除したノードの親ノードから上に向かって挿入位置を
		 *  探す．
		 */
		index = tmevt_up(tevtcb, parent, event_time);
	}
	else {
		/*
		 *  削除したノードから下に向かって挿入位置を探す．
		 */
		index = tmevt_down(tevtcb, index, event_time);
	}

	/*
	 *  最後のノードを index の位置に挿入する．
	 */ 
	TMEVT_NODE(tevtcb, index) = TMEVT_NODE(tevtcb, tevtcb->last_index + 1);
	TMEVT_NODE(tevtcb, index).tmevtb->index = index;
}

#endif /* __tmedel */

/*
 *  タイムイベントヒープの先頭のノードの削除
 */
Inline void
tmevtb_delete_top(TEVTCB *tevtcb)
{
	UINT	index;
	EVTTIM	event_time = TMEVT_NODE(tevtcb, tevtcb->last_index).time;

	/*
	 *  削除によりタイムイベントヒープが空になる場合は何もしない．
	 */
	if (--tevtcb->last_index == 0) {
		return;
	}

	/*
	 *  ルートノードに最後のノード（last_index + 1 の位置のノード）
	 *  を挿入し，それを適切な位置へ移動させる．実際には，最後のノー
	 *  ドを実際に挿入するのではなく，ルートノードが空ノードになる
	 *  ので，最後のノードを挿入すべき位置へ向けて空ノードを移動さ
	 *  せる．
	 */
	index = tmevt_down(tevtcb, 1, event_time);

	/*
	 *  最後のノードを index の位置に挿入する．
	 */ 
	TMEVT_NODE(tevtcb, index) = TMEVT_NODE(tevtcb, tevtcb->last_index + 1);
	TMEVT_NODE(tevtcb, index).tmevtb->index = index;
}

/*
 *  タイムティックの供給
 *
 *  TIC_NUME < TIC_DENO の時は，除算を使わずに時刻の更新ができるが，ソー
 *  スコードを読みやすくにするために #if の多用を避けている．
 */
#ifdef __isig_tim

SYSCALL ER
isig_tim(void)
{
	TEVTCB	*tevtcb;
	TMEVTB	*tmevtb;
	ER	ercd;

	LOG_ISIG_TIM_ENTER();
	CHECK_INTCTX_UNL();
	i_acquire_glock();

	tevtcb = get_my_tpcb()->tevtcb;

	/*
	 *  next_time よりイベント発生時刻の早い（または同じ）タイムイ
	 *  ベントを，タイムイベントヒープから削除し，コールバック関数
	 *  を呼び出す．
	 */
	while (tevtcb->last_index > 0 && EVTTIM_LE(tevtcb, TMEVT_NODE(tevtcb, 1).time, tevtcb->next_time)) {
		tmevtb = TMEVT_NODE(tevtcb, 1).tmevtb;
		tmevtb_delete_top(tevtcb);
		(*(tmevtb->callback))(tmevtb->arg);

		/*
		 *  ここで優先度の高い割込みを受け付ける．
		 */
		i_release_glock();
		i_acquire_glock();
	}

	/*
	 *  current_time を更新する．
	 */
	tevtcb->current_time = tevtcb->next_time;

	/*
	 *  next_time，next_subtime を更新する．
	 */
#if TIC_DENO == 1
	tevtcb->next_time = tevtcb->current_time + TIC_NUME;
#else /* TIC_DENO == 1 */
	tevtcb->next_subtime += TIC_NUME % TIC_DENO;
	tevtcb->next_time = tevtcb->current_time + TIC_NUME / TIC_DENO;
	if (tevtcb->next_subtime >= TIC_DENO) {
		tevtcb->next_subtime -= TIC_DENO;
		tevtcb->next_time += 1u;
	}
#endif /* TIC_DENO == 1 */

	ercd = E_OK;
	i_release_glock();

    exit:
	LOG_ISIG_TIM_LEAVE(ercd);
	return(ercd);
}

#endif /* __isig_tim */
