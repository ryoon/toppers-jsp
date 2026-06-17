/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 *
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2017 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2017-2026 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: start.S 1258 2017-12-15 07:15:19Z ertl-honda $
 */

/*
 *   カーネル用のスタートアップモジュール(ARM用)
 */
#define _MACRO_ONLY
#include <sys_defs.h>
#include <cpu_defs.h>
#include "sys_asm.inc"
#include <t_config.h>

    .text
    .align 2
    .global start
start:
    /*
     *  プロセッサモードの初期化
     *  スーパーバイザーモード，FIQ/IRQ禁止
     */
    msr  cpsr_cxsf, #(CPSR_SVC | CPSR_INTLOCK)

    /*
     *  スタックポインタと，フレームポインタの初期化
     *  sl(スタック上限レジスタr10)は設定する必要があるか?
     *  gccはディフォルトでは-mapcsなので必要ない
     */
	my_prcidx r0
	ldr  r1,=(STACKTOP-PSTACKSIZE)
	cmp  r0, #0
	bne  start_0
	ldr  r1,=STACKTOP
start_0:
	mov  sp, r1
    mov  fp, #0        /* ARMモード用フレームポインタ(r11) */
    mov  r7, #0        /* Thumbモード用フレームポインタ    */

#if defined(SUPPORT_VFPv3_NEON)
    /*
     *  FPUの有効化（全てのコア)
     *  CP15 CPACR を設定し、CP10(単精度）およびCP11（倍精度）（FPU コプロセッサ）
     *  へのアクセスを可能に設定する。
     */
    ldr r0, =(0xf << 20)
    mcr p15, 0, r0, c1, c0, 2 /* Write CPACR cp11/cp10*/
    isb sy

    /*
     * FPEXC EN ビットをセットして、FPU を有効化
     */
    mov r0, #0x40000000
    vmsr FPEXC, r0
    isb sy
#endif /* SUPPORT_VFPv3_NEON */

    /*
     *  マスタプロセッサ以外は初期化待ち
     */
    my_prcidx r0
    cmp  r0, #0
    bne  start_7

    /*
     *  hardware_init_hookの呼び出し （0でない場合）
     *
     *  ターゲットハードウェアに依存して必要な初期化処理がある場合
     *  は，hardware_init_hook という関数を用意する
     */
    ldr   r0, =hardware_init_hook
    cmp   r0, #0x00
    movne lr, pc
    bxne  r0

    /*
     *  bssセクションのクリア
     */
start_1:
    ldr  r1, =__bss_start
    ldr  r2, =__bss_end
    cmp  r1,r2
    bhs  start_3
    mov  r0,#0
start_2:
    str  r0,[r1],#4
    cmp  r1, r2
    blo  start_2

start_3:
    /*
     *  dataセクションの初期化(ROM化対応)
     *  IDATA_START から IDATA_END までを，DATA_START 以降に
     *  コピーする
     */
    ldr  r1, =__idata_start
    ldr  r3, =__idata_end
    cmp  r1, r3
    bhs  start_5
    ldr  r2, =__data_start
start_4:
    ldr  r0, [r1],#4
    str  r0, [r2],#4
    cmp  r1, r3
    blo  start_4

    /*
     *  software_init_hook を呼出し（0 でない場合）
     *
     *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
     *  理がある場合は，software_init_hook という関数を用意すれば
     *  よい．
     */
start_5:
    ldr   r0, =software_init_hook
    cmp   r0, #0x00
    movne lr, pc
    bxne  r0

#if TNUM_PRCID >= 2

	/*
	 *  サブコアを起動
	 */
    bl   mprc_initialize

#endif	/* TNUM_PRCID >= 2 */

    /* 全コアに通知 */
    sev

start_6:
    bl   kernel_start

l_end:
	b   l_end

	/*
	 *  マスターコア優先のためしばらく待つ
	 */
start_7:
	ldr r2, =200000

start_8:
	sub r2, #1
	cmp r2, #0
	beq start_6
	b start_8

