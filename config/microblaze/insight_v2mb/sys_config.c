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
 *  @(#) $Id: sys_config.c,v 1.2 2002/04/10 09:48:56 honda Exp $
 */

#include "jsp_kernel.h"
#include "insight_v2mb.h"
#include "microblaze.h"

/*
 *  ターゲットシステム依存 初期化ルーチン（Insight VirtexII用）
 */
void
sys_initialize()
{
  led_seg_init(); /* 7SEG の初期化 */
  lcd_init();     /* LCDの初期化   */
  lcd_write_string("TOPPERS/JSP Kernel Start!");
}


/*
 *  ターゲットシステムの終了ルーチン
 *
 *  システムを終了する時に使う．通常はモニタ呼び出しで実現する．
 *  
 */
void
sys_exit(void)
{

}

void
sys_putc(char c)
{
    if (c == '\n') {
        uart_putc('\r');
    }
    uart_putc(c);
}


/*
 *  LCD操作ルーチン
 */
void
lcd_init(void)
{
  /* first reset */
  lcd_write_cmd(LCD_CMD_RESET);
  lcd_delay_loop(10000);

  /* second reset */
  lcd_write_cmd(LCD_CMD_RESET);
  lcd_delay_loop(10000);

  /* third reset */
  lcd_write_cmd(LCD_CMD_RESET);
  lcd_delay_loop(10000);

  /* setup interface size , row and font size */
  lcd_write_cmd(LCD_CMD_FUNC | LCD_FUNC_8BIT | LCD_FUNC_2LINE);
  lcd_delay_loop(10000);

  /* turn off */
  lcd_write_cmd(LCD_CMD_DONOFF);
  lcd_delay_loop(10000);

  /* clear display */
  lcd_write_cmd(LCD_CMD_CLRDP);
  lcd_delay_loop(10000);

  /* setup mode */
  lcd_write_cmd(LCD_CMD_MODE | LCD_MODE_IORD);
  lcd_delay_loop(10000);

  /* display on, cursor on , blink on */
  lcd_write_cmd(LCD_CMD_DONOFF | LCD_DONOFF_DON | LCD_DONOFF_CUR | LCD_DONOFF_BLK);
}



BOOL
lcd_set_cur_posi(int row, int col)
{
  unsigned char addr;

  if(row >= LCD_NUM_ROW)
    return FALSE;

  if(col >= LCD_NUM_COL)
    return FALSE;

  addr = (row == 0)? 0x00 : 0x40;
  addr += col;
  
  lcd_delay_bf();
  lcd_write_cmd(LCD_CMD_ADDDDR | addr);

  return TRUE;
}



void
lcd_write_string(char *string)
{
  int row,col;
  
  lcd_clr_disp();
  lcd_cur_at_home();

  for(row = 0; row < LCD_NUM_ROW; row++){
    for(col = 0; col < LCD_NUM_COL; col++){
      lcd_set_cur_posi(row,col);
      if(*string == '\0')
        return;
      else
        lcd_write_char(*(string++));
      
    }
  }
}

