/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2001-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001-2003 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2003 by Katsuhiro Amano
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
 *  @(#) $Id: cpu_config.c,v 1.9 2003/12/11 07:00:10 honda Exp $
 */

#include "jsp_kernel.h"
#include "check.h"
#include "task.h"

#include <hw_serial.h>

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
 *  ベクタテーブルの初期化（for RedBoot）
 */
#ifdef REDBOOT
#define VECTOR_TABLE_ADDR 	0x00fffd20
#define JMP_OPECODE		0x5a000000
extern void	vectors(void);

static void
vector_table_copy(void)
{
	unsigned int n = 0x100/sizeof(unsigned int);		/* ベクタテーブルサイズ */
	unsigned int *o = (unsigned int *)VECTOR_TABLE_ADDR;	/* ベクタテーブルコピー先 */
	unsigned int *i = (unsigned int *)vectors;		/* ベクタテーブルコピー元 */

    while (n -- > 0)
        *o ++ = JMP_OPECODE | (*i ++);		/* jmp命令の付加 */

}
#endif	/* of #ifdef REDBOOT */

/*
 *  プロセッサ依存の初期化
 */

void
cpu_initialize()
{
	SCI_initialize(SYSTEM_PORTID);

#ifdef REDBOOT
	vector_table_copy();
#endif	/* of #ifdef REDBOOT */
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
        SCI_wait_putchar(SYSTEM_SCI, '\r');
    }
    SCI_wait_putchar(SYSTEM_SCI, c);
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
