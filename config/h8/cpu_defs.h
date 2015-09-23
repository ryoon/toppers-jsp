/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: cpu_defs.h,v 1.9 2004/09/03 15:39:07 honda Exp $
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

/*
 *  プロセッサに依存する定義
 */

#define H8

/*
 *  タイムティックの定義
 */

#define	TIC_NUME	1		/* タイムティックの周期の分子 */
#define	TIC_DENO	1		/* タイムティックの周期の分母 */

#define	STACK_SIZE	1024		/* タスクのスタックサイズ */

/*
 *  CPU のバイト順に関する定義
 */

#define SIL_ENDIAN		SIL_ENDIAN_BIG

#ifndef _MACRO_ONLY

typedef	UINT	INHNO;			/* 割込みハンドラ番号 */
typedef	UINT	EXCNO;			/* CPU例外ハンドラ番号 */

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

/*
 *  システムの中断処理
 */

Inline void
kernel_abort (void)
{
	while (1)
		;
	}

#endif /* _MACRO_ONLY */

#endif /* _CPU_DEFS_H_ */
