/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001,2002 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
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
 *  @(#) $Id: cpu_config.c,v 1.4 2002/04/14 11:36:50 hiro Exp $
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

#include "hw_serial.h"

/*
 *  プロセッサ依存モジュール（H8用）
 */

/*
 *  非タスクコンテキストでの割込みマスク
 */

UB	int_intmask = 0;

/*
 *  割込みネストカウンタ
 */

UW	intnest = 0;

/*
 *  プロセッサ依存の初期化
 */

void
cpu_initialize()
{
    SCI_initialize(SYSTEM_SCI);

#if NUM_PORT >= 2

    SCI_initialize(USER_SCI);

#endif	/* of #if NUM_PORT >= 2 */
}

/*
 *  プロセッサ依存の終了処理
 */

void
cpu_terminate()
{
}

/*
 * 登録されていない割り込みが発生すると呼び出される
 */

void cpu_experr(EXCSTACK *sp)
{
    syslog(LOG_EMERG, "Unexpected interrupt.");
    syslog(LOG_EMERG, "PC  = %08x SP  = %08x", sp->pc, sp - 32);
    syslog(LOG_EMERG, "ER0 = %08x ER1 = %08x ER2 = %08x ER3 = %08x",
                       sp->er0, sp->er1, sp->er2, sp->er3);
    syslog(LOG_EMERG, "ER4 = %08x ER5 = %08x ER6 = %08x",
                       sp->er4, sp->er5, sp->er6);
    while(1)
    	;
}

/*
 *   システム文字出力先の指定
 */

void
cpu_putc(char c)
{
    if (c == '\n') {
        SCI_putchar(SYSTEM_SCI, '\r');
    }
    SCI_putchar(SYSTEM_SCI, c);
}

/*
 *  local_memcpy
 *
 *    標準 C ライブラリの memcpy と同じ、低水準コピー関数
 *    リンク時にエラーが発生するため、名前を変えている。
 *    また、リンクスクリプトの最後の行に
 *
 *       PROVIDE(_memcpy = _local_memcpy);
 *
 *    を追加した。
 */

void *
local_memcpy (void *out, const void *in, unsigned int n)
{
    char *o = out;
    const char *i = in;

    while (n -- > 0)
        *o ++ = *i ++;
    return out;
}
