/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: time_event.h,v 1.8 2003/06/18 12:48:24 hiro Exp $
 */

/*
 *	タイムイベント管理モジュール
 */

#ifndef _TIME_EVENT_H_
#define _TIME_EVENT_H_

typedef struct time_event_control_block TEVTCB;

/*
 *  イベント発生時刻のデータ型の定義
 *
 *  EVTTIM は，RELTIM として指定できる範囲よりも広い範囲を表現できる必
 *  要がある．スタンダードプロファイルでは，RELTIM が 16bit 以上でなけ
 *  ればならないため，EVTTIM は 17bit 以上であることが必要である．その
 *  ため，16bit の場合がある UINT ではなく，UW に定義している．
 */
typedef UW	EVTTIM;

/*
 *  相対時間（RELTIM）に指定できる最大値
 */
#define	TMAX_RELTIM	((((EVTTIM) 1) << (sizeof(EVTTIM) * CHAR_BIT - 1)) - 1)

/* 
 *  タイムイベントブロックのデータ型の定義
 */
typedef void	(*CBACK)(VP);	/* コールバック関数の型 */

typedef struct time_event_block {
	UINT	index;		/* タイムイベントヒープ中での位置 */
	CBACK	callback;	/* コールバック関数 */
	VP	arg;		/* コールバック関数へ渡す引数 */
} TMEVTB;

/*
 *  タイムイベントヒープ中のノードのデータ型の定義
 */
typedef struct time_event_node {
	EVTTIM	time;		/* イベント発生時刻 */
	TMEVTB	*tmevtb;	/* 対応するタイムイベントブロック */
} TMEVTN;

/*
 *  タイムイベントコントロールブロック
 */
struct time_event_control_block{
	/*
	 *  現在のシステム時刻（単位: 1ミリ秒）
	 *
	 *  システム起動時に0に初期化され，以降，タイムティックが供給される度に
	 *  単調に増加する．
	 */
	SYSTIM	current_time;

	/*
	 *  システム時刻のオフセット
	 */
	SYSTIM	systim_offset;

	/*
	 *  次のタイムティックのシステム時刻（単位: 1ミリ秒）
	 */
	SYSTIM	next_time;

	/*
	 *  システム時刻積算用変数（単位: 1/TIC_DENOミリ秒）
	 *
	 *  次のタイムティックのシステム時刻の下位桁を示す（上位桁はnext_time）．
	 *  TIC_DENOが1の時は，下位桁は常に0であるため，この変数は必要ない．
	 */
#if TIC_DENO != 1U
	UINT	next_subtime;
#endif /* TIC_DENO != 1U */

	/*
	 *  タイムイベントヒープの最後の使用領域のインデックス
	 *
	 *  タイムイベントヒープに登録されているタイムイベントの数に一致する．
	 */
	UINT	last_index;

	/*
	 *  タイムイベントヒープへのポインタ
	 */
	TMEVTN  *ptmevt_heap;
};

/*
 *  相対時間のベース時刻（単位: 1ミリ秒）
 *
 *  次のタイムティックのシステム時刻を切り上げた時刻．TIC_DENO が 1 の
 *  時は，next_time を EVTTIM に変換したものに一致する．
 */
#if TIC_DENO == 1
#define	base_time(t)	((EVTTIM) (t)->next_time)
#else /* TIC_DENO == 1 */
#define	base_time(t)	((EVTTIM)((t)->next_time + ((t)->next_subtime > 0 ? 1 : 0)))
#endif /* TIC_DENO == 1 */

/*
 *  タイムイベントヒープ操作マクロ
 */
#define	PARENT(index)	((index) >> 1)		/* 親ノードを求める */
#define	LCHILD(index)	((index) << 1)		/* 右の子ノードを求める */
#define	TMEVT_NODE(t, index)	(((t)->ptmevt_heap)[(index) - 1])

/*
 *  使用するTEVTCBの数を定義する
 */
#ifndef TOPPERS_SYSTIM_GLOBAL
#define	TNUM_TEVTCB	TNUM_PRCID
#else
#define	TNUM_TEVTCB	1
#endif

/*
 *  タイムイベントヒープ（kernel_cfg.c）
 */
extern TMEVTN	tmevt_heap[];

/*
 *  TEVTCBテーブル
 */
extern TEVTCB p_tevtcb[TNUM_TEVTCB];

/*
 *  TEVTCB へのアクセステーブル
 */
extern TEVTCB* const p_tevtcb_table[];


/*
 *  タイムイベント管理モジュールの初期化
 */
extern void	tmevt_initialize(void);

/*
 *  タイムイベントの挿入位置の探索
 */
extern UINT	tmevt_up(TEVTCB *tevtcb, UINT index, EVTTIM time);
extern UINT	tmevt_down(TEVTCB *tevtcb, UINT index, EVTTIM time);

/*
 *  タイムイベントヒープへの登録と削除
 */
extern void	tmevtb_insert(TEVTCB *tevtcb, TMEVTB *tmevtb, EVTTIM time);
extern void	tmevtb_delete(TEVTCB *tevtcb, TMEVTB *tmevtb);

/*
 *  タイムイベントブロックの登録（相対時間指定）
 *
 *  time で指定した相対時間が経過後に，arg を引数として callback が呼
 *  び出されるように，タイムイベントブロック tmevtb を登録する．
 */
Inline void
tmevtb_enqueue(TEVTCB *tevtcb, TMEVTB *tmevtb, RELTIM time, CBACK callback, VP arg)
{
	assert(time <= TMAX_RELTIM);

	tmevtb->callback = callback;
	tmevtb->arg = arg;
	tmevtb_insert(tevtcb, tmevtb, base_time(tevtcb) + time);
}

/*
 *  タイムイベントブロックの登録（イベント発生時刻指定）
 *
 *  time で指定したイベント発生時刻に，arg を引数として callback が呼
 *  び出されるように，タイムイベントブロック tmevtb を登録する．
 */
Inline void
tmevtb_enqueue_evttim(TEVTCB *tevtcb, TMEVTB *tmevtb, EVTTIM time, CBACK callback, VP arg)
{
	tmevtb->callback = callback;
	tmevtb->arg = arg;
	tmevtb_insert(tevtcb, tmevtb, time);
}

/*
 *  タイムイベントブロックの登録解除
 */
Inline void
tmevtb_dequeue(TEVTCB *tevtcb, TMEVTB *tmevtb)
{
	tmevtb_delete(tevtcb, tmevtb);
}

#endif /* _TIME_EVENT_H_ */
