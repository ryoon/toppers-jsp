/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2007-2011 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT
 *                             Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id: spinlock.c 1357 2025-12-02 15:30:53Z roi $
 */

/*
 *		スピンロック機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "spinlock.h"

/*
 *  カーネル・ビルドの条件判定
 */
#if TNUM_PRCID > 1 && !defined(TNUM_SPINLOCKID)
#error	"Illegal number of spin lock (TNUM_SPINLOCKID must be defined)";
#endif

/*
 *  スピンロックの数
 */
#define tnum_spn		((UINT)(tmax_spnid - TMIN_SPNID + 1))
#define INDEX_SPN(spnid)	((UINT)((spnid) - TMIN_SPNID))

/*
 *  ジャイアントロックの取得（タスクコンテキスト）
 */
#if TNUM_PRCID > 1
Inline void
t_acquire_pure_lock(LOCK *p_spn_lock)
{
	t_acquire_lock(p_spn_lock);
}
#else /* TNUM_PRCID > 1 */
#define t_acquire_pure_lock(l)
#endif /* TNUM_PRCID > 1 */

/*
 *  ジャイアントロックの取得（非タスクコンテキスト）
 */
#if TNUM_PRCID > 1
Inline void
i_acquire_pure_lock(LOCK *p_spn_lock)
{
	i_acquire_lock(p_spn_lock);
}
#else /* TNUM_PRCID > 1 */
#define i_acquire_pure_lock(l)
#endif /* TNUM_PRCID > 1 */

#if TNUM_PRCID > 1
/*
 *  ジャイアントロックの解放（タスクコンテキスト）
 */
Inline void
x_release_pure_lock(LOCK *p_spn_lock)
{
	x_release_lock(p_spn_lock);
}
#else	/* TNUM_PRCID > 1 */
#define x_release_pure_lock(l)
#endif	/* TNUM_PRCID > 1 */

/*
 *  スピンロックIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_spnid;


/*
 *  スピンロック初期化ブロックのエリア（kernel_cfg.c）
 */
extern const SPNINIB	spninib_table[];

/*
 *  スピンロック管理ブロックへのポインタテーブル（kernel_cfg.c）
 */
extern SPNCB spncb_table[];


/*
 *  スピンロック機能の初期化と取り出し関数
 */
#ifdef __spnini

/*
 *  スピンロックIDのより、スピンロック管理ブロックを取得
 */
SPNCB *
get_spncb(ID spnid)
{
	return (&spncb_table[INDEX_SPN(spnid)]);
}

/*
 *  スピンロック機能の初期化
 */
void
spinlock_initialize(void)
{
	TPCB	*my_tpcb = get_my_tpcb();
#if TNUM_PRCID > 1
	UINT	i;
	SPNCB	*spncb;

	assert(tmax_spnid <= TNUM_SPINLOCKID);
#endif /* TNUM_PRCID > 1 */

	my_tpcb->locspnid = 0;
	if (!is_master_proc()) {
		return;
	}

#if TNUM_PRCID > 1
	for (i = 0; i < tmax_spnid;  i++) {
		spncb = &spncb_table[i];
		spncb->spninib = &(spninib_table[i]);
		spncb->lockcount = 0;
		x_initialize_spin((i + 1), &(spncb->spn_lock));
	}
#endif /* TNUM_PRCID > 1 */
}

#endif /* __spnini */
/*
 *  スピンロックの取得
 */
#ifdef __loc_spn

ER
loc_spn(ID spnid)
{
	ER	ercd;
	TPCB	*my_tpcb;
	SPNCB	*spncb;

	LOG_LOC_SPN_ENTER(spnid);
	CHECK_TSKCTX_UNL();
	CHECK_SPNID(spnid);
	my_tpcb = get_my_tpcb();
	CHECK_ILUSE(my_tpcb->locspnid == 0);
	spncb = get_spncb(spnid);

	t_lock_cpu();
	t_acquire_pure_lock(&(spncb->spn_lock));
	spncb->lockcount++;
	my_tpcb->locspnid = spnid;
	my_tpcb->runtsk->locspnid = spnid;
	ercd = E_OK;

	exit:
	LOG_LOC_SPN_LEAVE(ercd);
	return(ercd);
}

#endif /* __loc_spn */

/*
 *  スピンロックの取得（非タスクコンテキスト用）
 */
#ifdef __iloc_spn

ER
iloc_spn(ID spnid)
{
	ER		ercd;
	TPCB	*my_tpcb;
	SPNCB	*spncb;

	LOG_ILOC_SPN_ENTER(spnid);
	CHECK_INTCTX_UNL();
	CHECK_SPNID(spnid);
	my_tpcb = get_my_tpcb();
	CHECK_ILUSE(my_tpcb->locspnid == 0);
	spncb = get_spncb(spnid);

	i_lock_cpu();
	i_acquire_pure_lock(&(spncb->spn_lock));
	spncb->lockcount++;
	my_tpcb->locspnid = spnid;
	ercd = E_OK;

	exit:
	LOG_ILOC_SPN_LEAVE(ercd);
	return(ercd);
}

#endif /* __iloc_spn */

/*
 *  スピンロックの返却
 */
#ifdef __unl_spn

ER
unl_spn(ID spnid)
{
	ER		ercd;
	SPNCB	*spncb;
	TPCB	*my_tpcb;
	BOOL	locked;

	LOG_UNL_SPN_ENTER(spnid);
	CHECK_TSKCTX();
	CHECK_SPNID(spnid);
	spncb = get_spncb(spnid);

	locked = t_sense_lock();
	if (!locked) {
		t_lock_cpu();
	}
	my_tpcb = get_my_tpcb();
	if (my_tpcb->locspnid == 0 || my_tpcb->locspnid != spnid) {
		if (!locked) {
			t_unlock_cpu();
		}
		ercd = E_ILUSE;
	}
	else {
		/*
		 *  スピンロック取得とのレーシングを避けるため，スピンロックを
		 *  解放する前に設定．
		 */
		my_tpcb->runtsk->locspnid = 0;
		my_tpcb->locspnid = 0;
		spncb->lockcount--;
		x_release_pure_lock(&(spncb->spn_lock));
		t_unlock_cpu();
		ercd = E_OK;
	}

	exit:
	LOG_UNL_SPN_LEAVE(ercd);
	return(ercd);
}

#endif /* __unl_spn */

/*
 *  スピンロックの返却（非タスクコンテキスト用）
 */
#ifdef __iunl_spn

ER
iunl_spn(ID spnid)
{
	ER		ercd;
	SPNCB	*spncb;
	TPCB	*my_tpcb;
	BOOL	locked;

	LOG_IUNL_SPN_ENTER(spnid);
	CHECK_INTCTX();
	CHECK_SPNID(spnid);
	spncb = get_spncb(spnid);

	locked = i_sense_lock();
	if (!locked) {
		i_lock_cpu();
	}
	my_tpcb = get_my_tpcb();
	if (my_tpcb->locspnid == 0 || my_tpcb->locspnid != spnid) {
		if (!locked) {
			i_unlock_cpu();
		}
		ercd = E_ILUSE;
	}
	else {
		/*
		 *  スピンロック取得とのレーシングを避けるため，スピンロックを
		 *  解放する前に設定．
		 */
		spncb->lockcount--;
		my_tpcb->locspnid = 0;
		x_release_pure_lock(&(spncb->spn_lock));
		i_unlock_cpu();
		ercd = E_OK;
	}

	exit:
	LOG_IUNL_SPN_LEAVE(ercd);
	return(ercd);
}

#endif /* __iunl_spn */
