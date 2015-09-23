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
 *  @(#) $Id: api.h,v 1.1 2006/04/10 08:19:25 honda Exp $
 */

#ifndef API_H
#define API_H

#define __cycini
#define __cycenq
#define __sta_cyc
#define __stp_cyc
#define __cyccal
#define __dtqini
#define __dtqenq
#define __dtqfenq
#define __dtqdeq
#define __dtqsnd
#define __dtqrcv
#define __snd_dtq
#define __psnd_dtq
#define __ipsnd_dtq
#define __tsnd_dtq
#define __fsnd_dtq
#define __ifsnd_dtq
#define __rcv_dtq
#define __prcv_dtq
#define __trcv_dtq
#define __flgini
#define __flgcnd
#define __set_flg
#define __iset_flg
#define __clr_flg
#define __wai_flg
#define __pol_flg
#define __twai_flg
#define __excini
#define __vxsns_ctx
#define __vxsns_loc
#define __vxsns_dsp
#define __vxsns_dpn
#define __vxsns_tex
#define __inhini
#define __mbxini
#define __snd_mbx
#define __rcv_mbx
#define __prcv_mbx
#define __trcv_mbx
#define __mpfini
#define __mpfget
#define __get_mpf
#define __pget_mpf
#define __tget_mpf
#define __rel_mpf
#define __semini
#define __sig_sem
#define __isig_sem
#define __wai_sem
#define __pol_sem
#define __twai_sem
#define __rot_rdq
#define __irot_rdq
#define __get_tid
#define __iget_tid
#define __loc_cpu
#define __iloc_cpu
#define __unl_cpu
#define __iunl_cpu
#define __dis_dsp
#define __ena_dsp
#define __sns_ctx
#define __sns_loc
#define __sns_dsp
#define __sns_dpn
#define __logini
#define __vwri_log
#define __vrea_log
#define __vmsk_log
#define __logter
#define __tskini
#define __tsksched
#define __tskrun
#define __tsknrun
#define __tskdmt
#define __tskact
#define __tskext
#define __tskpri
#define __tskrot
#define __tsktex
#define __ras_tex
#define __iras_tex
#define __dis_tex
#define __ena_tex
#define __sns_tex
#define __act_tsk
#define __iact_tsk
#define __can_act
#define __ext_tsk
#define __ter_tsk
#define __chg_pri
#define __get_pri
#define __slp_tsk
#define __tslp_tsk
#define __wup_tsk
#define __iwup_tsk
#define __can_wup
#define __rel_wai
#define __irel_wai
#define __sus_tsk
#define __rsm_tsk
#define __frsm_tsk
#define __dly_tsk
#define __tmeini
#define __tmeup
#define __tmedown
#define __tmeins
#define __tmedel
#define __isig_tim
#define __set_tim
#define __get_tim
#define __vxget_tim
#define __waimake
#define __waicmp
#define __waitmo
#define __waitmook
#define __waican
#define __wairel
#define __wobjwai
#define __wobjwaitmo
#define __wobjpri


#endif

