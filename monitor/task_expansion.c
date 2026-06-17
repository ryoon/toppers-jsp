/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2003-2005 by 
 *                     GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2016-2025 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)～(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: task_expansion.c,v 1.6 2025/12/08 13:56:45 roi Exp $
 */

/*
 *	タスク管理拡張機能
 */

#include "jsp_kernel.h"
#include "check.h"
#include "wait.h"
#include "time_event.h"
#include "semaphore.h"
#include "dataqueue.h"
#include "task_expansion.h"
#include "kernel_id.h"


Weak void _kernel_break_wait(void)
{
}

/*
 *  セマフォIDの最大値（kernel_cfg.c）
 */
extern const ID tmax_semid;

/*
 *  セマフォ管理ブロックのエリア（kernel_cfg.c）
 */
extern SEMCB semcb_table[];

/*
 *  セマフォIDからセマフォ管理ブロックを取り出すためのマクロ
 */
#define INDEX_SEM(semid)	((UINT)((semid) - TMIN_SEMID))
#define get_semcb(semid)	(&(semcb_table[INDEX_SEM(semid)]))

/*
 *  セマフォの状態参照
 */
SYSCALL ER
ref_sem(ID semid, T_RSEM *pk_rsem)
{
	SEMCB	*semcb;
	QUEUE   *queue;
	ER	ercd;

	CHECK_TSKCTX_UNL();
	CHECK_SEMID(semid);
	semcb = get_semcb(semid);

	t_lock_cpu();
	queue = &(semcb->wait_queue);
	if (queue->next == queue) {
		pk_rsem->wtskid = 0;
	}
	else{
		pk_rsem->wtskid = TSKID((TCB *)queue->next);
	}
	pk_rsem->semcnt = semcb->semcnt;
	ercd = E_OK;
	t_unlock_cpu();

    exit:
	return(ercd);
}

#ifdef NEED_REFDTQ
/*
 *  データキューIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_dtqid;

/*
 *  データキュー管理ブロックのエリア（kernel_cfg.c）
 */
extern DTQCB	dtqcb_table[];

/*
 *  データキューIDからデータキュー管理ブロックを取り出すためのマクロ
 */
#define INDEX_DTQ(dtqid)	((UINT)((dtqid) - TMIN_DTQID))
#define get_dtqcb(dtqid)	(&(dtqcb_table[INDEX_DTQ(dtqid)]))

/*
 *  データキューの状態参照
 */
SYSCALL ER
ref_dtq(ID dtqid, T_RDTQ *pk_rdtq)
{
	DTQCB	*dtqcb;
	QUEUE   *queue;
	ER	ercd;

	CHECK_DISPATCH();
	CHECK_DTQID(dtqid);
	dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	pk_rdtq->sdtqcnt = dtqcb->count;
	queue = &(dtqcb->swait_queue);
	if(queue->next == queue)
		pk_rdtq->stskid = 0;
	else
		pk_rdtq->stskid = TSKID((TCB *)queue->next);
	queue = &(dtqcb->rwait_queue);
	if(queue->next == queue)
		pk_rdtq->rtskid = 0;
	else
		pk_rdtq->rtskid = TSKID((TCB *)queue->next);
	t_unlock_cpu();
	ercd = E_OK;

    exit:
	return(ercd);
}
#endif	/* NEED_REFDTQ */

/*
 *  システム時間取り出し関数
 *  割込み中でも使用でき、共通化できるように
 *  インライン関数とする．
 */
Inline SYSTIM get_systime(TEVTCB *tevtcb)
{
	return tevtcb->systim_offset + tevtcb->next_time;
}


#ifdef	LOG_ANALYSIS

/*
 *  タスク状態のロギング格納領域
 */

static T_TLOG tsk_log[MAX_TASK_LOG+1];
static ID     check_tskid[TNUM_PRCID];
static SYSTIM check_time[TNUM_PRCID];
static SYSTIM pervious_time;

/*
 *  タスク実行領域設定関数
 *  この関数はdispatchのタスク確定時、または、アイドル状態
 *  確定時、CPUロック状態で呼び出す．
 */
#include "hw_timer.h"

void iana_tsk(void)
{
	T_TLOG  *t;
	UINT	idx = get_prc_index();
	TPCB	*my_tpcb = get_pure_tpcb(idx);
	TEVTCB	*tevtcb = my_tpcb->tevtcb;
	SYSTIM  time;
#if TIC_DENO != 1
	INT     subtime;
#endif /* TIC_DENO != 1 */
	CLOCK   clock;
	BOOL    ireq;

	/*
	 * 時間の取り出しはコンテキスト中でも行われるので
	 * vxget_timと同等の記述をここにおく。
	 */
#if TIC_DENO != 1
	subtime = (INT) tevtcb->next_subtime;
#endif /* TIC_DENO != 1 */
	clock = hw_timer_get_current();
	ireq = hw_timer_fetch_interrupt();

	time = get_systime(tevtcb) * ANA_STIC;
#if TIC_DENO != 1
	time += subtime * ANA_STIC / TIC_DENO;
#endif /* TIC_DENO != 1 */
	if (!(ireq) || clock >= (TO_CLOCK(TIC_NUME, TIC_DENO)
					- GET_TOLERANCE)) {
		time -= TIC_NUME * ANA_STIC / TIC_DENO;
	}
	time += clock * ANA_STIC / TIMER_CLOCK;

	if(check_tskid[idx] < 0)
		t = &tsk_log[0];
	else if(check_tskid[idx] < MAX_TASK_LOG)
		t = &tsk_log[check_tskid[idx]];
	else
		t = &tsk_log[MAX_TASK_LOG];
	t->runcount++;
	t->runtimes += time - check_time[idx];

	if(my_tpcb->runtsk)
		check_tskid[idx] = TSKID(my_tpcb->runtsk);
	else
		check_tskid[idx] = 0;
	check_time[idx] = time;
}

/*
 *  タスクログ状態取り出し関数
 */
INT get_tsklog(T_TPRM * pprm)
{
	INT no;
	INT num_item=0;

	get_tim(&pprm->currtime);
	t_acquire_glock();
	pprm->pervtime = pervious_time;
	pervious_time  = pprm->currtime;
	for(no = 0 ; no <= tmax_tskid && no <= MAX_TASK_LOG ; no++){
		if(no < NUM_LDSP){
			pprm->tlog[no] = tsk_log[no];
			num_item = no;
		}
		else{
			pprm->tlog[NUM_LDSP-1].runcount += tsk_log[no].runcount;
			pprm->tlog[NUM_LDSP-1].runtimes += tsk_log[no].runtimes;
			num_item = NUM_LDSP-1;
		}
		tsk_log[no].runcount = 0;
		tsk_log[no].runtimes = 0;
	}
	t_release_glock();
	return num_item;
}

#endif	/* LOG_ANALYSIS */

#ifdef LOG_DEVICE

/*
 *  デバイス・ポートのログアウト要求判定用データ領域
 */

static T_PCHK port_log[NUM_PCHK];

/*
 *  デバイスログを行うかどうかの判定
 */
static UINT check_device_log(UW address, UW size)
{
	int    i;
	T_PCHK *p;

	for(i = 0 ; i < NUM_PCHK ; i++){
		p = &port_log[i];
		if(p->portaddress >= address && p->portaddress < (address+size) && p->logtype)
			return (UINT)p->logtype;
	}
	return 0;
}

/*
 *  デバイス・ポートのログ判定データの取り出し
 */
T_PCHK *get_device_log(UW no){
	if(no < NUM_PCHK)
		return &port_log[no];
	else
		return 0;
}

/*
 *  デバイス読み出し表示判定とログアウト関数
 */
void
ana_rdv(UW address, UW data, INT size)
{
	SYSTIM time = get_systime();
	ID     id;
	UINT   logtype = check_device_log(address, size);

	if(logtype == 0)
		return;
	if(sense_context() || !runtsk)
		syslog_3(logtype, "Device Read  time=%09d interrupt port=%08x data=0x%x", time, address, data);
	else{
		id = TSKID(runtsk);
		switch(size){
		case 4:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%08x", time, id, address, data);
			break;
		case 2:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%04x", time, id, address, data);
			break;
		default:
			syslog_4(logtype, "Device Read  time=%09d task=%04d port=%08x data=%02x", time, id, address, data);
			break;
		}
	}
}

/*
 *  デバイス書き込み表示判定とログアウト関数
 */
void
ana_wdv(UW address, UW data, INT size)
{
	SYSTIM time = get_systime();
	ID     id;
	UINT   logtype = check_device_log(address, size);

	if(logtype == 0)
		return;
	if(sense_context() || !runtsk)
		syslog_3(logtype, "Device Write  time=%09d interrupt port=%08x data=0x%x", time, address, data);
	else{
		id = TSKID(runtsk);
		switch(size){
		case 4:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%08x", time, id, address, data);
			break;
		case 2:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%04x", time, id, address, data);
			break;
		default:
			syslog_4(logtype, "Device Write time=%09d task=%04d port=%08x data=%02x", time, id, address, data);
			break;
		}
	}
}

#endif	/* LOG_DEVICE */

