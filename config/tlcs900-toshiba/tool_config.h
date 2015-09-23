/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2006-2010 by Witz Corporation, JAPAN
 * 
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 * 
 *  (3) When this software is redistributed in the forms unusable for the
 *      development of other software, such as the case when the software
 *      is embedded in a piece of equipment, either of the following two
 *      conditions must be satisfied:
 * 
 *    (a) The above copyright notice, this use conditions, and the
 *           disclaimer shown below must be shown without modification in
 *        the document provided with the redistributed software, such as
 *        the user manual.
 * 
 *    (b) How the software is to be redistributed must be reported to the
 *        TOPPERS Project according to the procedure described
 *        separately.
 * 
 *  (4) The above copyright holders and the TOPPERS Project are exempt
 *      from responsibility for any type of damage directly or indirectly
 *      caused from the use of this software and are indemnified by any
 *      users or end users of this software from any and all causes of
 *      action whatsoever.
 * 
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 * 
 *  @(#) $Id: tool_config.h,v 1.1 2006/04/10 08:19:25 honda Exp $
 */

/*
 *	開発環境依存モジュール
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */

#ifndef _TOOL_CONFIG_H_
#define _TOOL_CONFIG_H_

/*
 *  開発環境依存の初期化は使用しない
 */
#define tool_initialize()

/*
 *  ラベルの別名を定義するためのマクロ
 */
#define __EMPTY_LABEL(x,y) x y[]

/*
 *  atexit の処理とデストラクタの実行は使用しない
 */
#define call_atexit()

/*
 *  トレースログの設定
 */

#define	LOG_INH_ENTER(inhno)		/* プロセッサ依存部 */
#define	LOG_INH_LEAVE(inhno)		/* プロセッサ依存部 */

#define	LOG_ISR_ENTER(intno)		/* プロセッサ依存部 */
#define	LOG_ISR_LEAVE(intno)		/* プロセッサ依存部 */

#define	LOG_CYC_ENTER(cyccb)
#define	LOG_CYC_LEAVE(cyccb)

#define	LOG_EXC_ENTER(excno)		/* プロセッサ依存部 */
#define	LOG_EXC_LEAVE(excno)		/* プロセッサ依存部 */

#define	LOG_TEX_ENTER(texptn)
#define	LOG_TEX_LEAVE(texptn)

#define	LOG_TSKSTAT(tcb)

#define	LOG_DSP_ENTER(tcb)		/* プロセッサ依存部 */
#define	LOG_DSP_LEAVE(tcb)		/* プロセッサ依存部 */

#define	LOG_ACT_TSK_ENTER(tskid)
#define	LOG_ACT_TSK_LEAVE(ercd)
#define	LOG_IACT_TSK_ENTER(tskid)
#define	LOG_IACT_TSK_LEAVE(ercd)
#define	LOG_CAN_ACT_ENTER(tskid)
#define	LOG_CAN_ACT_LEAVE(ercd)
#define	LOG_EXT_TSK_ENTER()
#define	LOG_TER_TSK_ENTER(tskid)
#define	LOG_TER_TSK_LEAVE(ercd)
#define	LOG_CHG_PRI_ENTER(tskid, tskpri)
#define	LOG_CHG_PRI_LEAVE(ercd)
#define	LOG_GET_PRI_ENTER(tskid, p_tskpri)
#define	LOG_GET_PRI_LEAVE(ercd, tskpri)
#define	LOG_SLP_TSK_ENTER()
#define	LOG_SLP_TSK_LEAVE(ercd)
#define	LOG_TSLP_TSK_ENTER(tmout)
#define	LOG_TSLP_TSK_LEAVE(ercd)
#define	LOG_WUP_TSK_ENTER(tskid)
#define	LOG_WUP_TSK_LEAVE(ercd)
#define	LOG_IWUP_TSK_ENTER(tskid)
#define	LOG_IWUP_TSK_LEAVE(ercd)
#define	LOG_CAN_WUP_ENTER(tskid)
#define	LOG_CAN_WUP_LEAVE(ercd)
#define	LOG_REL_WAI_ENTER(tskid)
#define	LOG_REL_WAI_LEAVE(ercd)
#define	LOG_IREL_WAI_ENTER(tskid)
#define	LOG_IREL_WAI_LEAVE(ercd)
#define	LOG_SUS_TSK_ENTER(tskid)
#define	LOG_SUS_TSK_LEAVE(ercd)
#define	LOG_RSM_TSK_ENTER(tskid)
#define	LOG_RSM_TSK_LEAVE(ercd)
#define	LOG_FRSM_TSK_ENTER(tskid)
#define	LOG_FRSM_TSK_LEAVE(ercd)
#define	LOG_DLY_TSK_ENTER(dlytim)
#define	LOG_DLY_TSK_LEAVE(ercd)
#define	LOG_RAS_TEX_ENTER(tskid, rasptn)
#define	LOG_RAS_TEX_LEAVE(ercd)
#define	LOG_IRAS_TEX_ENTER(tskid, rasptn)
#define	LOG_IRAS_TEX_LEAVE(ercd)
#define	LOG_DIS_TEX_ENTER()
#define	LOG_DIS_TEX_LEAVE(ercd)
#define	LOG_ENA_TEX_ENTER()
#define	LOG_ENA_TEX_LEAVE(ercd)
#define	LOG_SNS_TEX_ENTER()
#define	LOG_SNS_TEX_LEAVE(state)
#define	LOG_SIG_SEM_ENTER(semid)
#define	LOG_SIG_SEM_LEAVE(ercd)
#define	LOG_ISIG_SEM_ENTER(semid)
#define	LOG_ISIG_SEM_LEAVE(ercd)
#define	LOG_WAI_SEM_ENTER(semid)
#define	LOG_WAI_SEM_LEAVE(ercd)
#define	LOG_POL_SEM_ENTER(semid)
#define	LOG_POL_SEM_LEAVE(ercd)
#define	LOG_TWAI_SEM_ENTER(semid, tmout)
#define	LOG_TWAI_SEM_LEAVE(ercd)
#define	LOG_SET_FLG_ENTER(flgid, setptn)
#define	LOG_SET_FLG_LEAVE(ercd)
#define	LOG_ISET_FLG_ENTER(flgid, setptn)
#define	LOG_ISET_FLG_LEAVE(ercd)
#define	LOG_CLR_FLG_ENTER(flgid, clrptn)
#define	LOG_CLR_FLG_LEAVE(ercd)
#define	LOG_WAI_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn)
#define	LOG_WAI_FLG_LEAVE(ercd, flgptn)
#define	LOG_POL_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn)
#define	LOG_POL_FLG_LEAVE(ercd, flgptn)
#define	LOG_TWAI_FLG_ENTER(flgid, waiptn, wfmode, p_flgptn, tmout)
#define	LOG_TWAI_FLG_LEAVE(ercd, flgptn)
#define	LOG_SND_DTQ_ENTER(dtqid, data)
#define	LOG_SND_DTQ_LEAVE(ercd)
#define	LOG_PSND_DTQ_ENTER(dtqid, data)
#define	LOG_PSND_DTQ_LEAVE(ercd)
#define	LOG_IPSND_DTQ_ENTER(dtqid, data)
#define	LOG_IPSND_DTQ_LEAVE(ercd)
#define	LOG_TSND_DTQ_ENTER(dtqid, data, tmout)
#define	LOG_TSND_DTQ_LEAVE(ercd)
#define	LOG_FSND_DTQ_ENTER(dtqid, data)
#define	LOG_FSND_DTQ_LEAVE(ercd)
#define	LOG_IFSND_DTQ_ENTER(dtqid, data)
#define	LOG_IFSND_DTQ_LEAVE(ercd)
#define	LOG_RCV_DTQ_ENTER(dtqid, p_data)
#define	LOG_RCV_DTQ_LEAVE(ercd, data)
#define	LOG_PRCV_DTQ_ENTER(dtqid, p_data)
#define	LOG_PRCV_DTQ_LEAVE(ercd, data)
#define	LOG_TRCV_DTQ_ENTER(dtqid, p_data, tmout)
#define	LOG_TRCV_DTQ_LEAVE(ercd, data)
#define	LOG_SND_MBX_ENTER(mbxid, pk_msg)
#define	LOG_SND_MBX_LEAVE(ercd)
#define	LOG_RCV_MBX_ENTER(mbxid, ppk_msg)
#define	LOG_RCV_MBX_LEAVE(ercd, pk_msg)
#define	LOG_PRCV_MBX_ENTER(mbxid, ppk_msg)
#define	LOG_PRCV_MBX_LEAVE(ercd, pk_msg)
#define	LOG_TRCV_MBX_ENTER(mbxid, ppk_msg, tmout)
#define	LOG_TRCV_MBX_LEAVE(ercd, pk_msg)
#define	LOG_GET_MPF_ENTER(mpfid, p_blk)
#define	LOG_GET_MPF_LEAVE(ercd, blk)
#define	LOG_PGET_MPF_ENTER(mpfid, p_blk)
#define	LOG_PGET_MPF_LEAVE(ercd, blk)
#define	LOG_TGET_MPF_ENTER(mpfid, p_blk, tmout)
#define	LOG_TGET_MPF_LEAVE(ercd, blk)
#define	LOG_REL_MPF_ENTER(mpfid, blk)
#define	LOG_REL_MPF_LEAVE(ercd)
#define	LOG_SET_TIM_ENTER(p_systim)
#define	LOG_SET_TIM_LEAVE(ercd)
#define	LOG_GET_TIM_ENTER(p_systim)
#define	LOG_GET_TIM_LEAVE(ercd, systim)
#define	LOG_ISIG_TIM_ENTER()
#define	LOG_ISIG_TIM_LEAVE(ercd)
#define	LOG_STA_CYC_ENTER(cycid)
#define	LOG_STA_CYC_LEAVE(ercd)
#define	LOG_STP_CYC_ENTER(cycid)
#define	LOG_STP_CYC_LEAVE(ercd)
#define	LOG_ROT_RDQ_ENTER(tskpri)
#define	LOG_ROT_RDQ_LEAVE(ercd)
#define	LOG_IROT_RDQ_ENTER(tskpri)
#define	LOG_IROT_RDQ_LEAVE(ercd)
#define	LOG_GET_TID_ENTER(p_tskid)
#define	LOG_GET_TID_LEAVE(ercd, tskid)
#define	LOG_IGET_TID_ENTER(p_tskid)
#define	LOG_IGET_TID_LEAVE(ercd, tskid)
#define	LOG_LOC_CPU_ENTER()
#define	LOG_LOC_CPU_LEAVE(ercd)
#define	LOG_ILOC_CPU_ENTER()
#define	LOG_ILOC_CPU_LEAVE(ercd)
#define	LOG_UNL_CPU_ENTER()
#define	LOG_UNL_CPU_LEAVE(ercd)
#define	LOG_IUNL_CPU_ENTER()
#define	LOG_IUNL_CPU_LEAVE(ercd)
#define	LOG_DIS_DSP_ENTER()
#define	LOG_DIS_DSP_LEAVE(ercd)
#define	LOG_ENA_DSP_ENTER()
#define	LOG_ENA_DSP_LEAVE(ercd)
#define	LOG_SNS_CTX_ENTER()
#define	LOG_SNS_CTX_LEAVE(state)
#define	LOG_SNS_LOC_ENTER()
#define	LOG_SNS_LOC_LEAVE(state)
#define	LOG_SNS_DSP_ENTER()
#define	LOG_SNS_DSP_LEAVE(state)
#define	LOG_SNS_DPN_ENTER()
#define	LOG_SNS_DPN_LEAVE(state)
#define	LOG_VSNS_INI_ENTER()
#define	LOG_VSNS_INI_LEAVE(state)
#define	LOG_VXSNS_CTX_ENTER(p_excinf)
#define	LOG_VXSNS_CTX_LEAVE(state)
#define	LOG_VXSNS_LOC_ENTER(p_excinf)
#define	LOG_VXSNS_LOC_LEAVE(state)
#define	LOG_VXSNS_DSP_ENTER(p_excinf)
#define	LOG_VXSNS_DSP_LEAVE(state)
#define	LOG_VXSNS_DPN_ENTER(p_excinf)
#define	LOG_VXSNS_DPN_LEAVE(state)
#define	LOG_VXSNS_TEX_ENTER(p_excinf)
#define	LOG_VXSNS_TEX_LEAVE(state)
#define	LOG_VXGET_TIM_ENTER(p_sysutim)
#define	LOG_VXGET_TIM_LEAVE(ercd, sysutim)

#define	LOG_CHG_IPM_ENTER(ipm)
#define	LOG_CHG_IPM_LEAVE(ercd)
#define	LOG_GET_IPM_ENTER(p_ipm)
#define	LOG_GET_IPM_LEAVE(ercd, ipm)

#endif /* _TOOL_CONFIG_H_ */
