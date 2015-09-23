/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: cpu_defs.h,v 1.4 2002/04/10 11:20:09 takayuki Exp $
 */


/*
 *	プロセッサに依存する定義（Windows用）
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

#define WINDOWS

#include "itron.h"
#include "vwindows.h"

#define INT_NUMINTERRUPTS	7	/* 割込み本数 */

#ifndef _MACRO_ONLY

typedef	UINT		INHNO;		/* 割込みハンドラ番号 */
typedef	UINT		EXCNO;		/* CPU例外ハンドラ番号 */

/*
 * 割込みエミュレーション関連
 */

#define INT_STAT_PENDING 0x01
#define INT_STAT_RUNNING 0x02
#define INT_STAT_ENABLED 0x04

struct tagInterruptLevel
{
	HANDLE	ThreadHandle;
	DWORD	ThreadID;
	void *	Routine;
	unsigned char Status;
};

extern struct tagInterruptLevel InterruptLevel[INT_NUMINTERRUPTS];
extern CRITICAL_SECTION InterruptCriticalSection;
extern unsigned int CurrentInterruptLevel;


#define EXC_MAXITEMS 32

struct tagExceptionLevel
{
	DWORD ExceptionCode;
	void * Routine;
};

extern struct tagExceptionLevel ExceptionLevel[EXC_MAXITEMS];

/*
 * 割込みエミュレーション用操作関数
 */

extern BOOL def_int( unsigned int ims, void * rtn);	/* ハンドラ定義 */
extern BOOL		ini_int( void );				/* 初期化 */
extern void		fin_int( void );
extern BOOL		ras_int(unsigned int ims);		/* 割込み要求 */
extern unsigned int sns_int( void );			/* Sense the highest pending request */
extern HANDLE	sch_int( void );				/* 割り込みのスケジュール */

 /* 許可と禁止は引数に関係なく全許可&全禁止となる */
extern ER	ena_int(unsigned int ims);			/* 許可 */
extern ER	dis_int(unsigned int ims);			/* 禁止 */

extern ER	chg_ims(unsigned int ims);			/* 割込みレベルの設定 */
extern ER	get_ims(unsigned int *p_ims);		/* 割込みレベルの取得 */

extern BOOL def_exc( DWORD exc, void * routine);
extern BOOL ini_exc(void);
extern void fin_exc(void);

#endif /* _MACRO_ONLY */

#include "hw_timer.h"

/*
 *  タイムティックの定義
 */
#define	TIC_NUME	TIMER_CLOCK_WINDOWS		/* タイムティックの周期の分子 */
#define	TIC_DENO	1		/* タイムティックの周期の分母 */


/*
 * 拡張タイマー (Pentium Timer)
 */

typedef UD SYSUTIM;

Inline ER
vxget_tim(SYSUTIM * pk_sysutim)
{
	QueryPerformanceCounter((LARGE_INTEGER *)pk_sysutim);
	return E_OK;
}

#endif /* _CPU_DEFS_H_ */
