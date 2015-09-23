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
 *  @(#) $Id: cpu_config.c,v 1.1 2002/04/05 08:47:14 honda Exp $
 */

/*
 *  プロセッサ依存モジュール（SH3用）
 */
#include "jsp_kernel.h"
#include "check.h"
#include "task.h"
#include "sys_defs.h"
#include "microblaze.h"


/*
 * 割り込みハンドラ/ビットマスクのテーブル
 */
FP  int_handler_table[32];
UW  int_bit_table[32];

/*
 * CPU例外ハンドラの疑似テーブル
 * Not Support!
 */
/* FP	exc_table[(0x1E0 >> 5) + 1]; */


/*
 * 割り込みのネスト回数のカウント
 * カーネル起動時はタスク独立部とみなすため1で初期化する
 */
UW interrupt_count = 1;

/*
 *  プロセッサ依存の初期化
 */
void
cpu_initialize()
{
  int i;
    
  /*
   * int_handler_table[],int_plevel_table[]の初期化
   * 未登録の割込み発生時にcpu_interrup()が呼び出されるように
   * no_reg_interrup()を登録する
   */
  for(i = 0; i < 32; i++){
    int_handler_table[i] = no_reg_interrupt;
    int_bit_table[i]  = 1 << i; /* ビットパターンの生成 */
  }
  
  /*
   *  INTCスタート
   */
  intc_start();
}



/*
 *  プロセッサ依存の終了処理
 */
void
cpu_terminate()
{
}


/*
 * 登録されていない例外が発生すると呼び出される
 * Not Support! 
 */
/*
void
cpu_expevt(void)
{
}
*/

/*
 * 未登録の割込みが発生した場合に呼び出される
 * Not Yet!
 */
void
cpu_interrupt(void)
{
    syslog(LOG_EMERG, "Unregisted Interrupt Occur!");

    while(1);
}
