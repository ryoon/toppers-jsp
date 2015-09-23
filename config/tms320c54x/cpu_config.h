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
 *  @(#) $Id: cpu_config.h,v 1.6 2002/04/14 15:05:15 reiji Exp $
 */


#ifndef _CPU_CONFIG_H_
#define _CPU_CONFIG_H_

#include "c54x.h"


#define TBIT_TCB_TSTAT      6
#define TBIT_TCB_PRIORITY   4

#ifndef _MACRO_ONLY

typedef struct task_context_block{
  VP  sp;
  FP  pc;
}CTXB;

extern UH  nest;

Inline BOOL
sense_context(void)
{
  return (nest > 0);
}

Inline BOOL
sense_lock()
{
  volatile UH temp_st1;
  temp_st1 = ST1;
  return ((temp_st1 & 0x0800) > 0);
}

#endif

#define t_sense_lock sense_lock
#define i_sense_lock sense_lock


#ifndef _MACRO_ONLY

Inline void
t_lock_cpu(void)
{
  asm("\tSSBX INTM");
}

Inline void
t_unlock_cpu()
{
  asm("\tRSBX INTM");
}

#endif

#define i_lock_cpu t_lock_cpu
#define i_unlock_cpu t_unlock_cpu


#ifndef _MACRO_ONLY

extern void dispatch(void);
extern void exit_and_dispatch(void);

extern FP  int_table[32];
extern const UH  int_mask_table[32];


Inline void
define_inh(INHNO inhno, FP inthdr)
{
  int_table[inhno] = inthdr;
}

#endif


#define INTHDR_ENTRY(inthdr) extern void inthdr(void)
#define INT_ENTRY(inthdr) inthdr


/*
 *  TMS320VC54x DSPにはCPU例外が存在しないので
 *  以下は適当に埋める
 */

#ifndef _MACRO_ONLY

/*
 *   CPU例外ハンドラの設定
 *   擬似ベクターテーブルに登録
 */   

Inline void
define_exc(EXCNO excno, FP exchdr)
{
}



/*
 *  CPU例外の発生した時のコンテキスト判定
 */
Inline BOOL
exc_sense_context(VP p_excinf)
{
   return(TRUE);
}

/*
 *  CPU例外の発生した時のCPUロック状態の参照
 */
Inline BOOL
exc_sense_lock(VP p_excinf)
{
   return(TRUE);
}

extern void cpu_initialize(void);
extern void cpu_terminate(void);

#endif

#endif
