/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
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
 *  @(#) $Id: start.S,v 1.5 2001/11/09 12:47:34 hiro Exp $
 */
                
/*
 *    カーネル用のスタートアップモジュール(ARM7TDMI用)
 */
#define _MACRO_ONLY
#include "arm7tdmi.h"
#include "sys_config.h"          
        .text
        .code 32
        .align 	0
        .global	start
start:
        mov r1,	#(CPSR_SYS|CPSR_FIQ_BIT|CPSR_IRQ_BIT) /* To System mode */
        msr cpsr_c,r1


        /*
         * スタックポインタと，フレームポインタの初期化
         * SL(スタック上限レジスタr10)は設定する必要があるか?
         * gccはディフォルトでは-mapcsなので必要ない
         */
        ldr r3, stack
        mov sp, r3
        mov r11, #0        /* Armモード用フレームポインタ   */         
        mov r7, #0         /* Thubmモード用フレームポインタ */


        /*
         * hardware_init_hookの呼び出し （0でない場合）
         *
         * ターゲットハードウェアに依存して必要な初期化処理がある場合
         * は，hardware_init_hook という関数を用意する
         */
        ldr r0, hardware_init_hook_k
        cmp r0, #0x00
        blne hardware_init_hook


        /*
         *  bssセクションのクリア
         */        
start_1:
        ldr  r1, bss_start_k
        ldr  r2, bss_end_k
        cmp  r1,r2
        bhs  start_3
        mov  r0,#0
start_2:
        str r0,[r1],#4
        cmp r1, r2
        blo start_2

        /*
         *  dataセクションの初期化(ROM化対応)
         *  __idata_start から __idata_end までを，__data_start 以降に
         *  コピーする
         */
start_3:
        ldr r1,idata_start_k
        ldr r3,idata_end_k
        cmp  r1,r3
        bhs  start_3
        ldr r2,data_start_k
start_4:        
        ldr r0,[r1],#4
        str r0,[r2],#4
        cmp r1,r3
        blo start_4
        
        
        /*
         *  software_init_hook を呼出し（0 でない場合）
         *
         *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
         *  理がある場合は，software_init_hook という関数を用意すれば
         *  よい．
         */
start_5:
        ldr  r0,software_init_hook_k
        cmp  r0, #0x00
        blne software_init_hook

start_6:        
        bl  kernel_start


        
        .align 4
stack:
        .long   STACKTOP
hardware_init_hook_k:
        .long   hardware_init_hook
bss_start_k:	
        .long   __bss_start
bss_end_k:	
        .long   _end
idata_start_k:  
        .long   __idata_start
idata_end_k:    
        .long   __idata_end
data_start_k:   
        .long   __data_start
software_init_hook_k:
        .long   software_init_hook
.interrupt_count:       
        .long   interrupt_count
