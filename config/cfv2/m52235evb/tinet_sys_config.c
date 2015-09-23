/*
 *  TINET (TCP/IP Protocol Stack)
 * 
 *  Copyright (C) 2001-2006 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
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
 *      用できない形で再配布する場合には，次の条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: $
 */

/*
 *  JSP-1.4.2 以降で変更された割込みハンドラへの対応
 */

#include <s_services.h>
#include <t_services.h>
#include "kernel_id.h"

#include <tinet_defs.h>
#include <tinet_config.h>

/*
 *  fec_inter_init -- ターゲット依存部の割込みの初期化
 */

int
memcmp(const void* s1, const void* s2, int n) {
  char* a1 = (char*)s1;
  char* a2 = (char*)s2;
  
  while (n--)
    if (*a1++ != *a2++)
      return *(a1 - 1) - *(a2 - 1);
  return 0;
}


void*
memset(char *s, int c, int n) 
{
  char* p = s;
  while (n--) {
    *p++=c;
  }
  return s;
}


void*
memcpy(void* dest, void* src, int n) {
  char *p1 = dest;
  const char *p2 = src;
  int i;

  for (i = 0; i < n; i++) {
    *(p1++) = *(p2++);
  }

  return dest;
}

void *
memmove (void *d, void *s, int n)
{
    char *dst = d;
    char *src = s;
    void *ret = dst;

    if (src < dst) {
	src += n;
	dst += n;
	while (n--)
	    *--dst = *--src;
    }
    else if (dst < src)
	while (n--)
	    *dst++ = *src++;
    return ret;
}

#if TKERNEL_PRVER >= 0x1042u	/* JSP-1.4.2 以降 */

/*
 * fec_dis_inter -- 割込みを禁止する。
 */
IPM
fec_dis_inter(void)
{
  IPM	ipm;
  
  syscall(get_ipm(&ipm));
  syscall(chg_ipm(if_fec_handler_intmask));
  return ipm;
}

/*
 *  fec_bus_init -- ターゲット依存部のバスの初期化
 */

void
fec_bus_init (void)
{
}

/*
 *  fec_inter_init -- ターゲット依存部の割込みの初期化
 */

void
fec_inter_init (void)
{
  int i;
  /* NIC の割り込みを許可する。*/
  for (i = TBIT_FEC_XINF; i <= TBIT_FEC_BABR; i++) {
    sil_wrb_mem (MCF_INTC_ICR(MCF_INTC0, i), ((FEC_INT_LEVEL) << 3) | (FEC_INT_PRI));
    if (i < 32)
      sil_wrw_mem (MCF_INTC_IMRL(MCF_INTC0), sil_rew_mem(MCF_INTC_IMRL(MCF_INTC0)) & (~(1 << i)));
    else
      sil_wrw_mem (MCF_INTC_IMRH(MCF_INTC0), sil_rew_mem(MCF_INTC_IMRH(MCF_INTC0)) & (~(1 << (i-32))));
  }
}

#endif	/* of #if TKERNEL_PRVER >= 0x1042u */
