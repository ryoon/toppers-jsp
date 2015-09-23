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
 *  @(#) $Id: task.c,v 1.5 2001/10/07 06:16:56 hiro Exp $
 */

/*
 *	タスク管理モジュール
 */

#include "jsp_kernel.h"
#include "task.h"
#include "cpu_context.h"

/*
 *  実行状態のタスク
 */
TCB	*runtsk;

/*
 *  最高優先順位のタスク
 */
TCB	*schedtsk;

/*
 *  タスクディスパッチ／タスク例外処理ルーチン起動要求フラグ
 */
BOOL	reqflg;

/*
 *  タスクディスパッチ許可状態
 */
BOOL	enadsp;

/*
 *  レディキュー
 *
 *  レディキューは，実行できる状態のタスクを管理するためのキューで，優
 *  先度ごとのタスクキューで構成されている．タスクのTCBは，該当する優
 *  先度のキューに登録される．
 */
static QUEUE	ready_queue[TNUM_TPRI];

/*
 *  レディキューサーチのためのビットマップ
 *
 *  レディキューのサーチを効率よく行うために，優先度ごとのタスクキュー
 *  にタスクが入っているかどうかを示すビットマップを用意している．ビッ
 *  トマップを使うことで，メモリアクセスの回数を減らすことができるが，
 *  ビット操作命令が充実していないプロセッサで，優先度の段階数が少ない
 *  場合には，ビットマップ操作のオーバーヘッドのために，逆に効率が落ち
 *  る可能性もある．
 *  ビットマップを UINT で定義しているが，ビットマップサーチ関数で優先
 *  度が16段階以下であることを仮定してしている．
 */
static UINT	ready_primap;

/*
 *  タスク管理モジュールの初期化
 */
void
task_initialize()
{
	INT	i;
	TCB	*tcb;

	runtsk = schedtsk = NULL;
	reqflg = FALSE;
	enadsp = TRUE;

	for (i = 0; i < TNUM_TPRI; i++) {
		queue_initialize(&(ready_queue[i]));
	}
	ready_primap = 0;

	for (tcb = tcb_table, i = 0; i < tmax_tskid; tcb++, i++) {
		tcb->tinib = &(tinib_table[i]);
		tcb->actcnt = FALSE;
		make_dormant(tcb);
		if ((tcb->tinib->tskatr & TA_ACT) != 0) {
			make_active(tcb);
		}
	}
}

/*
 *  ビットマップサーチ関数
 *
 *  bitmap 内の 1 のビットの内，最も下位（右）のものをサーチし，そのビ
 *  ット番号を返す．ビット番号は，最下位ビットを 0 とする．bitmap に 0
 *  を指定してはならない．この関数では，優先度が16段階以下であることを
 *  仮定し，bitmapの下位 16ビットのみをサーチする．
 *  ビットサーチ命令を持つCPUでは，ビットサーチ命令を使うように書き直
 *  した方が効率が良いだろう．このような場合には，cpu_insn.h でビット
 *  サーチ命令を使った bitmap_search を定義し，CPU_BITMAP_SEARCH をマ
 *  クロ定義すればよい．また，ビットサーチ命令のサーチ方向が逆などの理
 *  由で優先度とビットとの対応を変更したい場合には，PRIMAP_BIT をマク
 *  ロ定義すればよい．
 *  また，標準ライブラリに ffs があるなら，次のように定義して標準ライ
 *  ブラリを使った方が効率が良い可能性もある．
 *	#define	bitmap_search(bitmap) (ffs(bitmap) - 1)
 */
#ifndef PRIMAP_BIT
#define	PRIMAP_BIT(pri)		(1 << (pri))
#endif /* PRIMAP_BIT */

#ifndef CPU_BITMAP_SEARCH

Inline UINT
bitmap_search(UINT bitmap)
{
	static const VB search_table[] = { 0, 1, 0, 2, 0, 1, 0,
					3, 0, 1, 0, 2, 0, 1, 0 };
	INT	n = 0;

	assert((bitmap & 0xffff) != 0);
	if ((bitmap & 0x00ff) == 0) {
		bitmap >>= 8;
		n += 8;
	}
	if ((bitmap & 0x0f) == 0) {
		bitmap >>= 4;
		n += 4;
	}
	return(n + search_table[(bitmap & 0x0f) - 1]);
}

#endif /* CPU_BITMAP_SEARCH */

/*
 *  最高優先順位タスクのサーチ
 *
 *  レディキュー中の最高優先順位のタスクをサーチし，そのTCBへのポインタ
 *  を返す．レディキューが空の場合には，この関数を呼び出してはならない．
 */
static TCB *
search_schedtsk()
{
	UINT	schedpri;

	schedpri = bitmap_search(ready_primap);
	return((TCB *)(ready_queue[schedpri].next));
}

/*
 *  実行できる状態への移行
 *
 *  最高優先順位のタスクを更新するのは，実行できるタスクがなかった場合
 *  と，tcb の優先度が最高優先順位のタスクの優先度よりも高い場合である．
 */
BOOL
make_runnable(TCB *tcb)
{
	UINT	pri = tcb->priority;

	tcb->tstat = TS_RUNNABLE;
	queue_insert_prev(&(ready_queue[pri]), &(tcb->task_queue));
	ready_primap |= PRIMAP_BIT(pri);

	if (schedtsk == (TCB *) NULL || pri < schedtsk->priority) {
		schedtsk = tcb;
		return(enadsp);
	}
	return(FALSE);
}

/*
 *  実行できる状態から他の状態への移行
 *
 *  最高優先順位のタスクを更新するのは，tcb が最高優先順位のタスクであっ
 *  た場合である．tcb と同じ優先度のタスクが他にある場合は，tcb の次の
 *  タスクが最高優先順位になる．そうでない場合は，レディキューをサーチ
 *  する必要がある．
 */
BOOL
make_non_runnable(TCB *tcb)
{
	UINT	pri = tcb->priority;
	QUEUE	*queue = &(ready_queue[pri]);

	queue_delete(&(tcb->task_queue));
	if (queue_empty(queue)) {
		ready_primap &= ~PRIMAP_BIT(pri);
		if (schedtsk == tcb) {
			schedtsk = (ready_primap == 0) ? (TCB * ) NULL
						: search_schedtsk();
			return(enadsp);
		}
	}
	else {
		if (schedtsk == tcb) {
			schedtsk = (TCB *)(queue->next);
			return(enadsp);
		}
	}
	return(FALSE);
}

/*
 *  休止状態への移行
 */
void
make_dormant(TCB *tcb)
{
	tcb->priority = tcb->tinib->ipriority;
	tcb->tstat = TS_DORMANT;
	tcb->wupcnt = FALSE;
	tcb->enatex = FALSE;
	tcb->texptn = 0;
	create_context(tcb);
}

/*
 *  休止状態から実行できる状態への移行
 */
BOOL
make_active(TCB *tcb)
{
	activate_context(tcb);
	return(make_runnable(tcb));
}

/*
 *  実行状態のタスクの終了
 */
void
exit_task()
{
	make_non_runnable(runtsk);
	make_dormant(runtsk);
	if (runtsk->actcnt) {
		runtsk->actcnt = FALSE;
		make_active(runtsk);
	}
	exit_and_dispatch();
}

/*
 *  レディキュー中のタスクの優先度の変更
 *
 *  最高優先順位のタスクを更新するのは，(1) tcb が最高優先順位のタスク
 *  であって，その優先度を下げた場合，(2) tcb が最高優先順位のタスクで
 *  はなく，変更後の優先度が最高優先順位のタスクの優先度よりも高い場合
 *  である．(1) の場合には，レディキューをサーチする必要がある．
 */
BOOL
change_priority(TCB *tcb, UINT newpri)
{
	UINT	oldpri = tcb->priority;

	tcb->priority = newpri;
	queue_delete(&(tcb->task_queue));
	if (queue_empty(&(ready_queue[oldpri]))) {
		ready_primap &= ~PRIMAP_BIT(oldpri);
	}
	queue_insert_prev(&(ready_queue[newpri]), &(tcb->task_queue));
	ready_primap |= PRIMAP_BIT(newpri);

	if (schedtsk == tcb) {
		if (newpri >= oldpri) {
			schedtsk = search_schedtsk();
			return(schedtsk != tcb && enadsp);
		}
	}
	else {
		if (newpri < schedtsk->priority) {
			schedtsk = tcb;
			return(enadsp);
		}
	}
	return(FALSE);
}

/*
 *  レディキューの回転
 *
 *  最高優先順位のタスクを更新するのは，最高優先順位のタスクがタスクキ
 *  ューの末尾に移動した場合である．
 */
BOOL
rotate_ready_queue(UINT pri)
{
	QUEUE	*queue = &(ready_queue[pri]);
	QUEUE	*entry;

	if (!(queue_empty(queue)) && queue->next->next != queue) {
		entry = queue_delete_next(queue);
		queue_insert_prev(queue, entry);
		if (schedtsk == (TCB *) entry) {
			schedtsk = (TCB *)(queue->next);
			return(enadsp);
		}
	}
	return(FALSE);
}

/*
 *  タスク例外処理ルーチンの呼出し
 */
void
call_texrtn()
{
	TEXPTN	texptn;

	do {
		texptn = runtsk->texptn;
		runtsk->enatex = FALSE;
		runtsk->texptn = 0;
		t_unlock_cpu();
		(*runtsk->tinib->texrtn)(texptn, runtsk->tinib->exinf);
		if (!t_sense_lock()) {
			t_lock_cpu();
		}
	} while (runtsk->texptn != 0);
	runtsk->enatex = TRUE;
}

/*
 *  タスク例外処理ルーチンの起動
 */
#ifndef OMIT_CALLTEX

void
calltex()
{
	if (runtsk->enatex && runtsk->texptn != 0) {
		call_texrtn();
	}
}

#endif /* OMIT_CALLTEX */
