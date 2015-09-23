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
 *  @(#) $Id: time_event.c,v 1.1 2000/11/14 14:44:21 hiro Exp $
 */

/*
 *	タイムイベント管理モジュール
 */

#include "jsp_kernel.h"
#include "check.h"
#include "time_event.h"

/*
 *  現在のシステム時刻（単位: ミリ秒）
 *
 *  厳密には，前のタイムティックのシステム時刻．
 */
SYSTIM	current_time;

/*
 *  次のタイムティックのシステム時刻（単位: 1ミリ秒）
 */
SYSTIM	next_time;

/*
 *  システム時刻積算用変数（単位: 1/TIM_DENOミリ秒）
 */
#if TIC_DENO != 1
UINT	next_subtime;
#endif /* TIC_DENO != 1 */

/*
 *  相対時間のベース時刻（単位: 1ミリ秒）
 */
#if TIC_DENO != 1
EVTTIM	base_time;
#endif /* TIC_DENO != 1 */

/*
 *  タイムイベントヒープの最初の未使用領域のインデックス
 *
 *  タイムイベントヒープに登録されているタイムイベントの数に 1 を加え
 *  たものになる．
 */
static UINT	limit_index;

/*
 *  タイムイベントヒープ操作マクロ
 */
#define	PARENT(index)	((index) >> 1)		/* 親ノードを求める */
#define	LCHILD(index)	((index) << 1)		/* 右の子ノードを求める */
#define	TMEVT_NODE(index)	(tmevt_heap[(index) - 1])

/*
 *  イベント発生時刻比較マクロ
 *
 *  イベント発生時刻は，current_time からの相対値で比較する．すなわち，
 *  current_time を最小値（最も近い時刻），current_time - 1 が最大値
 *  （最も遠い時刻）とみなして比較する．
 */
#define	EVTTIM_LT(t1, t2) (((t1) - current_time) < ((t2) - current_time))
#define	EVTTIM_LE(t1, t2) (((t1) - current_time) <= ((t2) - current_time))

/*
 *  タイマモジュールの初期化
 */
void
tmevt_initialize()
{
	limit_index = 1;
	current_time = 0;
#if TIC_DENO == 1
	next_time = current_time + TIC_NUME;
#else /* TIC_DENO == 1 */
	next_subtime += TIC_NUME;
	next_time = current_time + next_subtime / TIC_DENO;
	next_subtime %= TIC_DENO;
	base_time = (EVTTIM)(next_time + (next_subtime > 0 ? 1 : 0));
#endif /* TIC_DENO == 1 */
}

/*
 *  タイムイベントの挿入位置を上向きに探索
 *
 *  時刻 time に発生するタイムイベントを挿入するノードを空けるために，
 *  ヒープの上に向かって空ノードを移動させる．移動前の空ノードの位置を 
 *  index に渡すと，移動後の空ノードの位置（すなわち挿入位置）を返す．
 */
static UINT
tmevt_up(UINT index, EVTTIM time)
{
	UINT	parent;

	while (index > 1) {
		/*
		 *  親ノードのイベント発生時刻の方が早い（または同じ）
		 *  ならば，index が挿入位置なのでループを抜ける．
		 */
		parent = PARENT(index);
		if (EVTTIM_LE(TMEVT_NODE(parent).time, time)) {
			break;
		}

		/*
		 *  親ノードを index の位置に移動させる．
		 */
		TMEVT_NODE(index) = TMEVT_NODE(parent);
		TMEVT_NODE(index).tmevtb->index = index;

		/*
		 *  index を親ノードの位置に更新．
		 */
		index = parent;
	}
	return(index);
}

/*
 *  タイムイベントの挿入位置を下向きに探索
 *
 *  時刻 time に発生するタイムイベントを挿入するノードを空けるために，
 *  ヒープの下に向かって空ノードを移動させる．移動前の空ノードの位置を 
 *  index に渡すと，移動後の空ノードの位置（すなわち挿入位置）を返す．
 */
static UINT
tmevt_down(UINT index, EVTTIM time)
{
	UINT	child;

	while ((child = LCHILD(index)) < limit_index) {
		/*
		 *  左右の子ノードのイベント発生時刻を比較し，早い方の
		 *  子ノードの位置を child に設定する．以下の子ノード
		 *  は，ここで選ばれた方の子ノードのこと．
		 */
		if (child + 1 < limit_index
			&& EVTTIM_LT(TMEVT_NODE(child + 1).time,
				  TMEVT_NODE(child).time)) {
			child = child + 1;
		}

		/*
		 *  子ノードのイベント発生時刻の方が遅い（または同じ）
		 *  ならば，index が挿入位置なのでループを抜ける．
		 */
		if (EVTTIM_LE(time, TMEVT_NODE(child).time)) {
			break;
		}

		/*
		 *  子ノードを index の位置に移動させる．
		 */
		TMEVT_NODE(index) = TMEVT_NODE(child);
		TMEVT_NODE(index).tmevtb->index = index;

		/*
		 *  index を子ノードの位置に更新．
		 */
		index = child;
	}
	return(index);
}

/*
 * タイムイベントヒープへの登録
 *
 *  タイムイベントブロック tmevtb を，time で指定した時間が経過後にイ
 *  ベントが発生するように，タイムイベントヒープに登録する．
 */
void
tmevtb_insert(TMEVTB *tmevtb, EVTTIM time)
{
	UINT	index;

	/*
	 *  limit_index から上に挿入位置を探す．
	 */
	index = tmevt_up(limit_index, time);

	/*
	 *  タイムイベントを index の位置に挿入する．
	 */ 
	TMEVT_NODE(index).time = time;
	TMEVT_NODE(index).tmevtb = tmevtb;
	tmevtb->index = index;
	limit_index += 1;
}

/*
 *  タイムイベントヒープからの削除
 */
void
tmevtb_delete(TMEVTB *tmevtb)
{
	UINT	index = tmevtb->index;
	UINT	parent;
	EVTTIM	event_time = TMEVT_NODE(limit_index - 1).time;

	/*
	 *  削除したノードの位置に最後のノード（limit_index - 1の位置
	 *  のノード）を挿入し，それを適切な位置へ移動させる．実際には，
	 *  最後のノードを実際に挿入するのではなく，削除したノードの位
	 *  置が空ノードになるので，最後のノードを挿入すべき位置へ向け
	 *  て空ノードを移動させる．
	 *  最後のノードのイベント発生時刻が，削除したノードの親ノード
	 *  のイベント発生時刻より前の場合には，上に向かって挿入位置を
	 *  探す．そうでない場合には，下に向かって探す．
	 */
	if (index > 1 && EVTTIM_LT(event_time,
				TMEVT_NODE(parent = PARENT(index)).time)) {
		/*
		 *  親ノードを index の位置に移動させる．
		 */
		TMEVT_NODE(index) = TMEVT_NODE(parent);
		TMEVT_NODE(index).tmevtb->index = index;

		/*
		 *  削除したノードの親ノードから上に向かって挿入位置を
		 *  探す．
		 */
		index = tmevt_up(parent, event_time);
	}
	else {
		/*
		 *  削除したノードから下に向かって挿入位置を探す．
		 */
		index = tmevt_down(index, event_time);
	}

	/*
	 *  最後のノードを index の位置に挿入する．
	 */ 
	TMEVT_NODE(index) = TMEVT_NODE(limit_index - 1);
	TMEVT_NODE(index).tmevtb->index = index;
	limit_index -= 1;
}

/*
 *  タイムイベントヒープの先頭のノードの削除
 */
Inline void
tmevtb_delete_top()
{
	UINT	index;
	EVTTIM	event_time = TMEVT_NODE(limit_index - 1).time;

	/*
	 *  ルートノードに最後のノード（limit_index - 1の位置のノード）
	 *  を挿入し，それを適切な位置へ移動させる．実際には，最後のノー
	 *  ドを実際に挿入するのではなく，ルートノードが空ノードになる
	 *  ので，最後のノードを挿入すべき位置へ向けて空ノードを移動さ
	 *  せる．
	 */
	index = tmevt_down(1, event_time);

	/*
	 *  最後のノードを index の位置に挿入する．
	 */ 
	TMEVT_NODE(index) = TMEVT_NODE(limit_index - 1);
	TMEVT_NODE(index).tmevtb->index = index;
	limit_index -= 1;
}

/*
 *  タイムティックの供給
 *
 *  TIC_NUME < TIC_DENO の時は，除算を使わずに時刻の更新ができるが，ソー
 *  スコードを読みやすくにするために #if の多用を避けている．
 */
SYSCALL ER
isig_tim()
{
	TMEVTB	*tmevtb;

	CHECK_INTCTX_UNL();
	i_lock_cpu();

	/*
	 *  next_time よりイベント発生時刻の早い（または同じ）タイムイ
	 *  ベントを，タイムイベントヒープから削除し，コールバック関数
	 *  を呼び出す．
	 */
	while (limit_index > 1 && EVTTIM_LE(TMEVT_NODE(1).time, next_time)) {
		tmevtb = TMEVT_NODE(1).tmevtb;
		tmevtb_delete_top();
		(*(tmevtb->callback))(tmevtb->arg);
	}

	/*
	 *  current_time を更新する．
	 */
	current_time = next_time;

	/*
	 *  next_time，next_subtime，base_time を更新する．
	 */
#if TIC_DENO == 1
	next_time = current_time + TIC_NUME;
#else /* TIC_DENO == 1 */
	next_subtime += TIC_NUME;
	next_time = current_time + next_subtime / TIC_DENO;
	next_subtime %= TIC_DENO;
	base_time = (EVTTIM)(next_time + (next_subtime > 0 ? 1 : 0));
#endif /* TIC_DENO == 1 */

	i_unlock_cpu();
	return(E_OK);
}
