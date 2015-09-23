/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: card_e09a.h,v 1.2 2001/10/19 14:20:47 honda Exp $
 */

#ifndef _CARD_E09A_ 
#define _CARD_E09A_ 

#include "sh3.h"




/*
 *  ISHボード上のデバッグ用LEDとコントローラーのアドレス
 */

#define ISH_LED1         (*(IOREG *)0xBBC00080)
#define ISH_LED2         (*(IOREG *)0xBBC00081)
#define ISH_LED_CONTROL  (*(HIOREG *)0xB1000780)


/*
 *  LEDのコントローラーイニシャライズ
 */
Inline void
led_init(void)
{
    ISH_LED_CONTROL = 0x0080;
}

/*
 *LEDポートへの出力
 */

Inline void
led_set(HIOREG led_data)
{
    ISH_LED1 = (IOREG)led_data;
    ISH_LED2 = (IOREG)(led_data >> 8);
}



/*
 *  CARD-E09Aのハードウェア資源の定義
 */



/*
 * CARD-E09A上の割込みコントローラ
 */

#define CARD_E09A_IER1    ((HIOREG *)(0xb1000000))  
#define CARD_E09A_IER2    ((HIOREG *)(0xb1000002))  
#define CARD_E09A_SR1     ((HIOREG *)(0xb1000004))  
#define CARD_E09A_SR2     ((HIOREG *)(0xb1000006))  

    
/*
 *  CARD-E09A上の16550互換のserial3
 */

typedef struct{
    HIOREG  data;
    HIOREG  IE;
    HIOREG  II;
    HIOREG  LC;
    HIOREG  MC;
    HIOREG  LS;
    HIOREG  MS;
    HIOREG  scratch;
} CARD_E09A_serial_t;

#define CARD_E09A_SERIAL     (*(volatile CARD_E09A_serial_t *)0xb1000200)

/*
 *  For IE
 */
#define IER_EDSSI     0x08
#define IER_ELSI      0x04
#define IER_ETBEI     0x02
#define IER_ERBFI     0x01

/*
 *  For II
 */
#define IIR_ID        0x06
#define IIR_IP        0x01

/*
 *  For LC
 */
#define LCR_DLAB      0x80
#define LCR_SB        0x40
#define LCR_SP        0x20
#define LCR_EPS       0x10
#define LCR_PEN       0x08
#define LCR_STB       0x06
#define LCR_WLS       0x01

 /*
  *  For LS 
  */
#define LSR_TEMT      0x40
#define LSR_THRE      0x20
#define LSR_BI        0x10
#define LSR_FE        0x08
#define LSR_PE        0x04
#define LSR_OE        0x02
#define LSR_DR        0x01


#endif /*  _CARD-E09A_ */



