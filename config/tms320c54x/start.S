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
; *  @(#) $Id: start.S,v 1.3 2002/04/14 10:37:55 reiji Exp $
; *
        .c_mode
        .mmregs
        .include  "c5402dsk.h"
        .global   _main
        .align    1
;スタック領域にはデータ領域の最後の余った領域を割り当てる。
;__stack:.usect ".stack",0
        .text
_start:
        ;******************************************
        ;      ステータスレジスタの初期化
        ;******************************************
        SSBX   INTM                    ;すべてのマスカブル割り込みを禁止
        STM    #0x0FFFF,IFR            ;IFRのクリア

        SSBX   SXM                     ;符号ビットセット
        SSBX   CPL                     ;コンパイラ互換モード
        RSBX   OVM                     ;非飽和モード
        LD     #0,ARP                  ;補助レジスタポインタ
        RSBX   CMPT
        RSBX   C16
        RSBX   FRCT
        
        ;******************************************
        ;     スタックポインタの初期化
        ;******************************************
        STM    #STACKTOP,SP
        ANDM   #0x0fffe,*(SP)          ;SPを偶数アドレスにアライン
        
        ;******************************************
        ;     _hardware_init_hook の呼び出し
        ;******************************************
start_0:
        LD     #_hardware_init_hook,A
        CC     _hardware_init_hook,ANEQ
        
        ;******************************************
        ;     bssセクションの初期化
        ;******************************************
start_1:
        STM    #___bss__,AR2
        RPTZ  A,#___end__-___bss__-1
        STL    A,*AR2+

        LD     #cinit,A                ;cinit=-1ならcinitは無い
        ADD    #1,A,B
        BC     start_4,BEQ

        
        RSBX   SXM                     ;符号ビットクリア
        NOP
        LD     #cinit,A
        B      start_2
start_3:
        READA  *(AR2)                  ;AR2 = 変数のアドレス
        ADD    #1,A                    ;初期化データへのポインタを求める
        RPT    *(AR1)                  ;length+1 繰り返す 
        READA  *AR2+                   ;変数を初期化
        ADD    *(AR1),A                ;
        ADD    #1,A                    ;データ長分だけポインタを進める
start_2:
        READA  *(AR1)                  ;AR1 = length
        ADD    #1,A                    ;A += 1
        BANZ   start_3,*AR1-           ;if (length != 0) length--、ジャンプ
        
        ;*****************************************
        ;    software_init_hookを呼び出す
        ;*****************************************
start_4:
        LD     #_software_init_hook,A
        CC     _software_init_hook,ANEQ

        ST     #1,*(_nest)
        CALL   _kernel_start
        
        
        
          