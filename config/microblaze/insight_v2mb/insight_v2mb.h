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
 *  @(#) $Id: insight_v2mb.h,v 1.1 2002/04/10 09:48:56 honda Exp $
 */

#ifndef _INSIGHT_VIRTEXII_
#define _INSIGHT_VIRTEXII_

#include "microblaze.h"

/*
 * Insight 用7セグメントドライバ
 * I/Oポートの先にnum to seg 回路を接続している 
 */

#define SEG_BASE 0xFFFF8200

#define SEG_DATAREG   ((LIOREG *)(SEG_BASE))
#define SEG_TRIREG    ((LIOREG *)(SEG_BASE + 0x04))

#ifndef _MACRO_ONLY
Inline void
led_seg_init(void)
{
  *SEG_TRIREG = 0x00;
}

Inline void
disp_seg(int num)
{
  *SEG_TRIREG = num;
}
#endif /* _MACRO_ONLY */

/*
 *  LCD control
 */
#define LCD_BASE 0xFFE00000

#define LCD_CTL  ((IOREG *)(LCD_BASE))
#define LCD_DATA ((IOREG *)(LCD_BASE + 0x01))


#define LCD_CMD_CLRDP   0x01
#define LCD_CMD_CAHOME  0x02
#define LCD_CMD_MODE    0x04
#define LCD_CMD_DONOFF  0x08
#define LCD_CMD_SHIFT   0x10
#define LCD_CMD_FUNC    0x20
#define LCD_CMD_ADDCGR  0x40
#define LCD_CMD_ADDDDR  0x80
#define LCD_CMD_RESET   0x30

#define LCD_MODE_IORD   0x02
#define LCD_MODE_DSHIFT 0x01

#define LCD_DONOFF_DON  0x04
#define LCD_DONOFF_CUR  0x02
#define LCD_DONOFF_BLK  0x01

#define LCD_SHIFT_DSHIFT 0x08
#define LCD_SHIFT_RIGHT  0x04

#define LCD_FUNC_8BIT    0x10
#define LCD_FUNC_2LINE   0x08
#define LCD_FUNC_BDOT    0x04

#define LCD_BF           0x80

#define LCD_NUM_ROW      2
#define LCD_NUM_COL      16


#ifndef _MACRO_ONLY

Inline void
lcd_delay_loop(int wait){
  for(; wait > 0 ; wait--);
}


Inline void
lcd_delay_bf(void){
    while((*LCD_CTL & LCD_BF) == LCD_BF);
    lcd_delay_loop(10);
}


Inline void
lcd_write_cmd(char cmd){
  *LCD_CTL = cmd;
}


Inline void
lcd_clr_disp(void){
  lcd_delay_bf();
  lcd_write_cmd(LCD_CMD_CLRDP);
}


Inline void
lcd_cur_at_home(void){
  lcd_delay_bf();
  lcd_write_cmd(LCD_CMD_CAHOME);
}


Inline void
lcd_write_char(char c){
  lcd_delay_bf();
  *LCD_DATA = c;
}

#endif /* _MACRO_ONLY */
#endif /* _INSIGHT_VIRTEXII_ */
