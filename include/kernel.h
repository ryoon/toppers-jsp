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
 *  @(#) $Id: kernel.h,v 1.6 2001/09/11 03:52:27 hiro Exp $
 */

/*
 *	μITRON4.0仕様標準インクルードファイル
 *
 *  このファイルでは，スタンダードプロファイルで必要なものだけを定義し
 *  ている．データ型の定義は，スタンダードプロファイルを満たすちょうど
 *  の長さにはしていない．
 *  アセンブリ言語のソースファイルやシステムコンフィギュレーションファ
 *  イルからこのファイルをインクルードする時は，_MACRO_ONLY を定義して
 *  おくことで，マクロ定義以外の記述を除くことができる．
 */

#ifndef _KERNEL_H_
#define _KERNEL_H_

/*
 *  ITRON仕様共通規定のデータ型・定数・マクロ
 */
#include <itron.h>

/*
 *  システムやプロセッサに依存する定義
 */
#include <sys_defs.h>
#include <cpu_defs.h>

/*
 *  補助マクロ
 */
#define	TROUND_VP(sz)	(((sz) + sizeof(VP) - 1) & ~(sizeof(VP) - 1))
#define	TCOUNT_VP(sz)	(((sz) + sizeof(VP) - 1) / sizeof(VP))

/*
 *  データ型の定義
 */
#ifndef _MACRO_ONLY

typedef	UINT		TEXPTN;		/* タスク例外要因のビットパターン */
typedef	UINT		FLGPTN;		/* イベントフラグのビットパターン */

typedef	struct t_msg {			/* メールボックスのメッセージヘッダ */
	struct t_msg	*next;
} T_MSG;

typedef	struct t_msg_pri {		/* 優先度付きメッセージヘッダ */
	T_MSG		msgque;		/* メッセージヘッダ */
	PRI		msgpri;		/* メッセージ優先度 */
} T_MSG_PRI;

#endif /* _MACRO_ONLY */

/*
 *  サービスコールの宣言
 */
#ifndef _MACRO_ONLY

/*
 *  タスク管理機能
 */
extern ER	act_tsk(ID tskid);
extern ER	iact_tsk(ID tskid);
extern ER_UINT	can_act(ID tskid);
extern void	ext_tsk(void);
extern ER	ter_tsk(ID tskid);
extern ER	chg_pri(ID tskid, PRI tskpri);
extern ER	get_pri(ID tskid, PRI *p_tskpri);

/*
 *  タスク付属同期機能
 */
extern ER	slp_tsk(void);
extern ER	tslp_tsk(TMO tmout);
extern ER	wup_tsk(ID tskid);
extern ER	iwup_tsk(ID tskid);
extern ER_UINT	can_wup(ID tskid);
extern ER	rel_wai(ID tskid);
extern ER	irel_wai(ID tskid);
extern ER	sus_tsk(ID tskid);
extern ER	rsm_tsk(ID tskid);
extern ER	frsm_tsk(ID tskid);
extern ER	dly_tsk(RELTIM dlytim);

/*
 *  タスク例外処理機能
 */
extern ER	ras_tex(ID tskid, TEXPTN rasptn);
extern ER	iras_tex(ID tskid, TEXPTN rasptn);
extern ER	dis_tex(void);
extern ER	ena_tex(void);
extern BOOL	sns_tex(void);

/*
 *  同期・通信機能
 */
extern ER	sig_sem(ID semid);
extern ER	isig_sem(ID semid);
extern ER	wai_sem(ID semid);
extern ER	pol_sem(ID semid);
extern ER	twai_sem(ID semid, TMO tmout);

extern ER	set_flg(ID flgid, FLGPTN setptn);
extern ER	iset_flg(ID flgid, FLGPTN setptn);
extern ER	clr_flg(ID flgid, FLGPTN clrptn);
extern ER	wai_flg(ID flgid, FLGPTN waiptn,
			MODE wfmode, FLGPTN *p_flgptn);
extern ER	pol_flg(ID flgid, FLGPTN waiptn,
			MODE wfmode, FLGPTN *p_flgptn);
extern ER	twai_flg(ID flgid, FLGPTN waiptn,
			MODE wfmode, FLGPTN *p_flgptn, TMO tmout);

extern ER	snd_dtq(ID dtqid, VP_INT data);
extern ER	psnd_dtq(ID dtqid, VP_INT data);
extern ER	ipsnd_dtq(ID dtqid, VP_INT data);
extern ER	tsnd_dtq(ID dtqid, VP_INT data, TMO tmout);
extern ER	fsnd_dtq(ID dtqid, VP_INT data);
extern ER	ifsnd_dtq(ID dtqid, VP_INT data);
extern ER	rcv_dtq(ID dtqid, VP_INT *p_data);
extern ER	prcv_dtq(ID dtqid, VP_INT *p_data);
extern ER	trcv_dtq(ID dtqid, VP_INT *p_data, TMO tmout);

extern ER	snd_mbx(ID mbxid, T_MSG *pk_msg);
extern ER	rcv_mbx(ID mbxid, T_MSG **ppk_msg);
extern ER	prcv_mbx(ID mbxid, T_MSG **ppk_msg);
extern ER	trcv_mbx(ID mbxid, T_MSG **ppk_msg, TMO tmout);

/*
 *  メモリプール管理機能
 */
extern ER	get_mpf(ID mpfid, VP *p_blk);
extern ER	pget_mpf(ID mpfid, VP *p_blk);
extern ER	tget_mpf(ID mpfid, VP *p_blk, TMO tmout);
extern ER	rel_mpf(ID mpfid, VP blk);

/*
 *  時間管理機能
 */
extern ER	set_tim(SYSTIM *p_systim);
extern ER	get_tim(SYSTIM *p_systim);
extern ER	isig_tim(void);

extern ER	sta_cyc(ID cycid);
extern ER	stp_cyc(ID cycid);

/*
 *  システム状態管理機能
 */
extern ER	rot_rdq(PRI tskpri);
extern ER	irot_rdq(PRI tskpri);
extern ER	get_tid(ID *p_tskid);
extern ER	iget_tid(ID *p_tskid);
extern ER	loc_cpu(void);
extern ER	iloc_cpu(void);
extern ER	unl_cpu(void);
extern ER	iunl_cpu(void);
extern ER	dis_dsp(void);
extern ER	ena_dsp(void);
extern BOOL	sns_ctx(void);
extern BOOL	sns_loc(void);
extern BOOL	sns_dsp(void);
extern BOOL	sns_dpn(void);

/*
 *  実装独自サービスコール
 */
extern BOOL	vxsns_ctx(VP p_excinf);
extern BOOL	vxsns_loc(VP p_excinf);
extern BOOL	vxsns_dsp(VP p_excinf);
extern BOOL	vxsns_dpn(VP p_excinf);
extern BOOL	vxsns_tex(VP p_excinf);

#endif /* _MACRO_ONLY */

/*
 *  オブジェクト属性の定義
 */
#define TA_HLNG		0x00		/* 高級言語用インタフェース */
#define TA_ASM		0x01		/* アセンブリ言語用インタフェース */

#define TA_TFIFO	0x00		/* タスクの待ち行列をFIFO順に */
#define TA_TPRI		0x01		/* タスクの待ち行列を優先度順に */

#define TA_MFIFO	0x00		/* メッセージキューをFIFO順に */
#define TA_MPRI		0x02		/* メッセージキューを優先度順に */

#define TA_ACT		0x02		/* タスクを起動された状態で生成 */

#define TA_WSGL		0x00		/* イベントフラグの待ちタスクを1つに */
#define TA_CLR		0x04		/* イベントフラグのクリア指定 */

#define	TA_STA		0x02		/* 周期ハンドラを動作状態で生成 */

/*
 *  サービスコールの動作モードの定義
 */
#define	TWF_ANDW	0x00		/* イベントフラグのAND待ち */
#define	TWF_ORW		0x01		/* イベントフラグのOR待ち */

/*
 *  その他の定数の定義
 */
#define	TSK_SELF	0		/* 自タスク指定 */
#define	TSK_NONE	0		/* 該当するタスクがない */
#define	TPRI_SELF	0		/* 自タスクのベース優先度の指定 */
#define	TPRI_INI	0		/* タスクの起動時優先度の指定 */

/*
 *  構成定数とマクロ
 */

/*
 *  優先度の範囲
 */
#define	TMIN_TPRI	1		/* タスク優先度の最小値 */
#define	TMAX_TPRI	16		/* タスク優先度の最大値 */
#define	TMIN_MPRI	1		/* メッセージ優先度の最小値 */
#define	TMAX_MPRI	16		/* メッセージ優先度の最大値 */

/*
 *  バージョン情報
 */
#define	TKERNEL_MAKER	0x0002		/* カーネルのメーカーコード */
#define	TKERNEL_PRID	0x0101		/* カーネルの識別番号 */
#define	TKERNEL_SPVER	0x5401		/* ITRON仕様のバージョン番号 */
#define	TKERNEL_PRVER	0x1020		/* カーネルのバージョン番号 */

/*
 *  キューイング／ネスト回数の最大値
 */
#define	TMAX_ACTCNT	1		/* 起動要求キューイング数の最大値 */
#define	TMAX_WUPCNT	1		/* 起床要求キューイング数の最大値 */
#define	TMAX_SUSCNT	1		/* 強制待ち要求ネスト数の最大値 */

/*
 *  ビットパターンのビット数
 */
#define	TBIT_TEXPTN	(sizeof(TEXPTN) * 8)	/* タスク例外要因のビット数 */
#define	TBIT_FLGPTN	(sizeof(FLGPTN) * 8)	/* イベントフラグのビット数 */

#endif /* _KERNEL_H_ */
