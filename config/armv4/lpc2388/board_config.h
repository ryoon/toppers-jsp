/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *                2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2006-2010 by GJ Business Division RICOH COMPANY,LTD. JAPAN
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
 *  @(#) $Id: board_config.h,v 1.1 2010/08/11 10:38:40 roi Exp $
 */


/*
 *  ボード依存モジュール（lpc23xx用）
 */


#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#include <sil.h>

/*
 *  MCIに関する関数設定
 */
#define mci_set_mask    lpc23xx_mci_set_mask
#define mci_set_clock   lpc23xx_mci_set_clock
#define mci_ini_por     lpc23xx_mci_ini_por
#define mci_opn_por     lpc23xx_mci_opn_por
#define mci_cls_por     lpc23xx_mci_cls_por
#define mci_snd_acmd    lpc23xx_mci_snd_acmd
#define mci_snd_cmd     lpc23xx_mci_snd_cmd
#define mci_get_resp    lpc23xx_mci_get_resp
#define mci_wri_blk     lpc23xx_mci_wri_blk
#define mci_rea_blk     lpc23xx_mci_rea_blk
#define mci_wai_trn     lpc23xx_mci_wai_trn

/*
 *  RTCに関する関数設定
 */
#define rtc_start       lpc23xx_rtc_start
#define rtc_terminate   lpc23xx_rtc_terminate
#define rtc_set_time    lpc23xx_rtc_set_time
#define rtc_get_time    lpc23xx_rtc_get_time
#define rtc_set_alarm   lpc23xx_rtc_set_alarm
#define rtc_set_event   lpc23xx_rtc_set_event

#endif /* _BOARD_CONFIG_H_ */
