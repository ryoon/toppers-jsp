/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2003      by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 */

#define CHIP_NAME "LC88F0099"
#define RAM_SIZE 0x7800
#define ROM_SIZE 0x78000
#define VRAM_SIZE 0x0000
#define VRAM_ROW 0
#define VRAM_COLUMN 0
#define CGROM_SIZE 0x0000
#define BCD(x)  (((x)/10)%10)*0x10+((x)%10)
#define BCD4(x) BCD((x)/100),BCD(x)
#define BCD6(x) BCD((x)/10000),BCD((x)/100),BCD(x)

/*
 *  EVAデバッガ用機種判別情報
 *  (このsectionは0x7f00-0x7fffにリンクされる)
 */

        .section .chip_info,"a"
        .global _chip_info
_chip_info:
        .org    0xc0
        .ascii  "CHIPINFORMATION"               /* C0-CF : fixed string */
        .org    0xd0
        .ascii  CHIP_NAME                       /* D0-DF : chip name */
        .org    0xe0
        .byte   0x10, 0x00                      /* E0-E1 : format version */
	.org	0xe8
        .byte   BCD4(ROM_SIZE/1024)             /* E8-F5 : Memory sizes */
        .byte   BCD6(RAM_SIZE)
        .byte   BCD6(VRAM_SIZE)
        .byte   BCD4(VRAM_ROW)
        .byte   BCD4(VRAM_COLUMN)
        .byte   BCD4(CGROM_SIZE/1024)
	.org	0xf9
        .byte   0x01                            /* F9 : package type */
        .space  6, 0x00

/*
 *  ユーザーオプションコード
 *  (このsectionは0x77f00-0x77fffにリンクされる)
 */

	.section .user_option,"a"
	.org	0xbc
	jmpf	0x08000				/* BC-BF : Jump to reset vector */
	.org	0xd0
        .ascii  CHIP_NAME                       /* D0-DF : chip name */
	.org	0xe0
	.byte   0x02, 0x00			/* E0 : Su script version */
	.org	0xe8
        .byte   BCD4(ROM_SIZE/1024)             /* E8-F5 : Memory sizes */
        .byte   BCD6(RAM_SIZE)
        .byte   BCD6(VRAM_SIZE)
        .byte   BCD4(VRAM_ROW)
        .byte   BCD4(VRAM_COLUMN)
        .byte   BCD4(CGROM_SIZE/1024)
	.org	0xf9
        .byte   0x01                            /* F9 : package type */
	.space	6, 0x00

        .end
