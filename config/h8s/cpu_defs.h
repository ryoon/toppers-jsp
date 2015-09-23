/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001,2002 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *
 *  上記著作権者は，Free Software Foundation によって公表されている
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: cpu_defs.h,v 1.2 2002/04/13 13:14:45 imai Exp $
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

/*  共通部のインライン関数展開の指定  */
/*  （今後、定義するファイルが変更になる可能性あり。）  */



#if 0	/*  not yet !  */
#pragma inline(_jsp_perror)   /*  jsp_services.h  */
#pragma inline(_jsp_panic)   /*  jsp_services.h  */
#pragma inline(_syslog_0, _syslog_1, _syslog_2, _syslog_3)
#pragma inline(_syslog_4, _syslog_5)      /*  syslog.h  */
#pragma inline(enqueue_msg_pri)         /*  mailbox.c  */
#pragma inline(queue_initialize, queue_insert_prev, queue_delete) 
#pragma inline(queue_delete_next, queue_empty) /*  queue.h  */
#pragma inline(bitmap_search)                  /*  task.c  */
#pragma inline(tmevtb_delete_top)              /*  time_event.c  */ 
#pragma inline(tmevtb_enqueue, tmevtb_enqueue_evttim, tmevtb_dequeue) 
                                               /*  time_event.h  */
#pragma inline(make_non_wait, queue_insert_tpri, wobj_queue_insert) 
    						/*  wait.c  */
#pragma inline(make_wait) 			/*  wait.h  */
#pragma inline(enable_send) 		   	/*  serial.c  */
 /* linux_serial.c内のインライン関数の定義は行っていない。 */

#endif




/*
 *  プロセッサに依存する定義
 */

#define H8S

/* カーネル起動時のメッセージ   */
#define COPYRIGHT_CPU \
"Copyright (C) 2002 by Industrial Technology Institute,\n" \
"                            Miyagi Prefectural Government, JAPAN\n"

#ifndef _MACRO_ONLY

typedef	UINT	INHNO;			/* 割込みハンドラ番号 */
typedef	UINT	EXCNO;			/* CPU例外ハンドラ番号 */

typedef	UB	IPM;			/* 割込みマスク */
extern ER	chg_ipm(IPM ipm);	/* 割込みマスクの変更 */
extern ER	get_ipm(IPM *p_ipm);	/* 割込みマスクの取得 */

/*
 *  INTNO型と割込みの禁止/許可
 */

typedef	UINT	INTNO;			/* 割込み番号 */

#if 0		/*  現時点ではサポートしていない  */
extern ER	dis_int(INTNO intno);
extern ER	ena_int(INTNO intno);
#endif


/*
 *  ターゲットシステム依存のサービスコール
 */

   /*
    * cpu_defs.hとsys_defs.hをインクルードする順番の
    * 関係でSUPPORT_VXGET_TIMマクロでは囲まない
    * （_MACRO_ONLYマクロで囲めば、十分）
    */
/*  UD型はサポートしていないためUW型としている。  */
typedef	UW  SYSUTIM;	/* 性能評価用システム時刻 */

extern ER   vxget_tim(SYSUTIM *pk_sysutim);

#endif /* _MACRO_ONLY */

#endif /* _CPU_DEFS_H_ */
