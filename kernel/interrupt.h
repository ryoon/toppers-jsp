/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: interrupt.h,v 1.5 2003/06/04 01:46:16 hiro Exp $
 */

/*
 *	割込み管理機能
 */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

/*
 *  設定の割込み属性からプロセッサIDを取り出す
 */
#if TNUM_PRCID > 1

#define	INTNO_PRCID(a)	((((a) >> 16) == 0) ? MASTER_PRCID : ((a) >> 16))

#else /* TNUM_PRCID > 1 */

#define	INTNO_PRCID(a)	ID_PRC(get_prc_index())

#endif /* TNUM_PRCID > 1 */

/*
 *  設定の割込み属性から対応プロセッサIDを取り出す
 */
#define	INTREQ_PRCID(a)	((((a) & TA_PRCALL) != 0) ? ID_PRC(get_prc_index()) : INTNO_PRCID((a)))


/*
 *  割込みハンドラ初期化ブロック
 */
typedef struct interrupt_handler_initialization_block {
	INHNO	inhno;		/* 割込みハンドラ番号 */
	ATR	inhatr;		/* 割込みハンドラ属性 */
	FP	inthdr;		/* 割込みハンドラの起動番地 */
} INHINIB;


/*
 *  割込み設定初期化ブロック
 */
typedef struct configint_initialization_block {
	INTNO	intno;		/* 割込み番号 */
	ATR	intatr;		/* 割込み属性 */
	PRI	intpri;		/* 割込み優先順位 */
} CFGINTINIB;

/*
 *  割込み管理機能の初期化
 */
extern void	interrupt_initialize(void);

extern void configint_initialize(void);

#endif /* _INTERRUPT_H_ */
