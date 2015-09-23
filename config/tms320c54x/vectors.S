; *
; *  TOPPERS/JSP Kernel
; *      Toyohashi Open Platform for Embedded Real-Time Systems/
; *      Just Standard Profile Kernel
; * 
; *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
; *                              Toyohashi Univ. of Technology, JAPAN
; * 
; *  上記著作権者は，Free Software Foundation によって公表されている 
; *  GNU General Public License の Version 2 に記述されている条件か，以
; *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
; *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
; *  利用と呼ぶ）することを無償で許諾する．
; *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
; *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
; *      スコード中に含まれていること．
; *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
; *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
; *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
; *      この利用条件および下記の無保証規定を掲載すること．
; *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
; *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
; *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
; *        権表示，この利用条件および下記の無保証規定を掲載すること．
; *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
; *        こと．
; *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
; *      害からも，上記著作権者を免責すること．
; * 
; *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
; *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
; *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
; *  かなる損害に関しても，その責任を負わない．
; * 
; *  @(#) $Id: vectors.S,v 1.3 2002/04/14 10:37:55 reiji Exp $
; *

       .include "c5402dsk.h"
       .sect   "vectors"
       .align  0x80
       .mmregs

RESET: B _start
       NOP
       NOP

NMI:   RETE
       NOP
       NOP
       NOP

; ソフトウェア割り込み(TRAP,INTM)
sint17 .space 4*16
sint18 .space 4*16
sint19 .space 4*16
sint20 .space 4*16
sint21 .space 4*16
sint22 .space 4*16
sint23 .space 4*16
sint24 .space 4*16
sint25 .space 4*16
sint26 .space 4*16
sint27 .space 4*16
sint28 .space 4*16
sint29 .space 4*16
sint30 .space 4*16

int0:  BD   _interrupt
       STM  #16,_INT_VECNO

int1:  BD   _interrupt
       STM  #17,_INT_VECNO

int2:  BD   _interrupt
       STM  #18,_INT_VECNO

tint0: BD  _interrupt
       STM  #19,_INT_VECNO

rint0: BD  _interrupt
       STM  #20,_INT_VECNO

xint0: BD  _interrupt
       STM  #21,_INT_VECNO

dma0:  BD  _interrupt
       STM  #22,_INT_VECNO

tint1: BD  _interrupt
       STM  #23,_INT_VECNO

int3:  BD  _interrupt
       STM  #24,_INT_VECNO

hpi:   BD  _interrupt
       STM  #25,_INT_VECNO

rint1: BD  _interrupt
       STM  #26,_INT_VECNO

xint1: BD  _interrupt
       STM  #27,_INT_VECNO

dma4:  BD  _interrupt
       STM  #28,_INT_VECNO

dma5:  BD  _interrupt
       STM  #29,_INT_VECNO
