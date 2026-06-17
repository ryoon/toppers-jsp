/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2008 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2016-2024 by TOPPERS PROJECT Educational Working Group.
 * 
 *  上記著作権者は，以下の(1)～(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
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
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  @(#) $Id$
 */

#define	_MACRO_ONLY
#include <t_services.h>

  .syntax unified
  .cpu cortex-m33
  .thumb


/*
 *  binary start header and embedded start block
 */
.section .binary_info_header, "a"

#if !PICO_NO_BINARY_INFO
binary_info_header:
  .word BINARY_INFO_MARKER_START
  .word __binary_info_start
  .word __binary_info_end
  .word data_cpy_table // we may need to decode pointers that are in RAM at runtime.
  .word BINARY_INFO_MARKER_END
#endif

embedded_block:
  .word PICOBIN_BLOCK_MARKER_START
  .word 0x10210142
  .byte PICOBIN_BLOCK_ITEM_2BS_LAST
  .hword (embedded_block_end - embedded_block - 16 ) / 4 // total size of all
  .byte 0
  .word embedded_end_block - embedded_block
  .word PICOBIN_BLOCK_MARKER_END
embedded_block_end:

/*
 *  data copy table
 */
.align 2
data_cpy_table:
  .word __idata_start
  .word __data_start
  .word _edata

  .word __scratch_x_source__
  .word __scratch_x_start__
  .word __scratch_x_end__

  .word __scratch_y_source__
  .word __scratch_y_start__
  .word __scratch_y_end__

  .word 0


/*
 *  embedded end block
 */
  .section .embedded_end_block, "a"
embedded_end_block:
  .word PICOBIN_BLOCK_MARKER_START

  .byte PICOBIN_BLOCK_ITEM_2BS_IGNORED
  .byte 0x1 // 1 word
  .hword 0

  .byte PICOBIN_BLOCK_ITEM_2BS_LAST
  .hword (embedded_end_block_end - embedded_end_block - 16 ) / 4
  .byte 0
// offset from this block to first block
  .word embedded_block - embedded_end_block
  .word PICOBIN_BLOCK_MARKER_END
embedded_end_block_end:

