/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
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
 *  @(#) $Id: cpu_defs.h,v 1.3 2001/10/22 02:16:43 imai Exp $
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

/*
 *  プロセッサに依存する定義（SH1用）
 */
#define SH1

/* カーネル起動時のメッセージ   */
#define COPYRIGHT_CPU \
"Copyright (C) 2001 by Industrial Technology Institute,\n" \
"                            Miyagi Prefectural Government, JAPAN\n"


#ifndef _MACRO_ONLY

typedef	UINT	INHNO;			/* 割込みハンドラ番号 */
typedef	UINT	EXCNO;			/* CPU例外ハンドラ番号 */

typedef	UINT	IPM;			/* 割込みマスク */
extern ER	chg_ipm(IPM ipm);	/* 割込みマスクの変更 */
extern ER	get_ipm(IPM *p_ipm);	/* 割込みマスクの取得 */

/*
 *  INTNO型と割込みの禁止/許可
 */

typedef	UINT	INTNO;			/* 割込み番号 */

#if 0		/*  SH1版ではサポートしていない  */
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

typedef	UD  SYSUTIM;	/* 性能評価用システム時刻 */

extern ER   vxget_tim(SYSUTIM *pk_sysutim);

#endif /* _MACRO_ONLY */
#endif /* _CPU_DEFS_H_ */
