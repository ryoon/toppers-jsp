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
 *  @(#) $Id: cpu_config.c,v 1.5 2002/04/10 11:17:16 takayuki Exp $
 */

/*
 *	プロセッサ依存モジュール（V850用）
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

/*
 *  タスクコンテキストでの割込みマスク
 */
#ifdef SUPPORT_CHG_IPM
UH	task_intmask;
#endif /* SUPPORT_CHG_IPM */

/*
 *  非タスクコンテキストでの割込みマスク
 */
UH	int_intmask;


/*
 *  割込みハンドラテーブル
 */
INTVE InterruptHandlerEntry[NUM_INTERRUPT];

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize()
{
}

/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
}

/*
 * 現在処理中の割込み優先度
 */
SYSCALL ER
get_ipr(IPR *p_ipm)
{
	unsigned int work;
	CHECK_TSKCTX_UNL();

	t_lock_cpu();
	work = (unsigned int)*((unsigned char *)ISPR);
	t_unlock_cpu();
	
	*p_ipm = 0;
	while(work != 0)
	{
		(*p_ipm) ++;
		work >>= 1;
	}
	return(E_OK);
}

void * memcpy(char * dest, char * src, unsigned int sz)
{
    while(sz-- > 0)
      *(dest++) = *(src++);
    return dest - sz;
}

void unhandled_interrpt_handler(unsigned int intno)
{
    sys_putc('!');
    __asm("halt");
}
