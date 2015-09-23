/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2007 by KURUSUGAWA Electronics Industry Inc, JAPAN
 *  Copyright (C) 2008 by Takahisa Yokota
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
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: start.S,v 1.6 2003/06/04 01:51:39 hiro Exp $
 */

/*
 *	カーネル用のスタートアップモジュール（COLDFIRE用）
 */

#define	_MACRO_ONLY
#include <t_config.h>
/*
 *　　例外ベクタの定義（COLDFIRE用）
 *　　　ここではリセット時に必要な最低限のベクタしか用意しない
 *　　　cpu_initialize()で例外ベクタの移動（VBRの書き換え）を行う
 */
#ifdef KERNEL_HAS_A_VECTOR_TABLE

/*
 *   以下の例外要因でデフォルトとは異なる例外ベクタを定義
 *   する場合は、sys_config.hで該当するマクロを定義する。
 */
	.section ".vector","ax"
	.align 2
	.long INIT_STACK		/*   0:スタックポインタの初期値  */
        .long TO_ASM(RESET_VECTOR)	/*   1:パワーオンリセット  */
	.long TO_ASM(ACC_VECTOR)	/*   2:アクセスエラー      */
  	.long TO_ASM(ADD_VECTOR)	/*   3:アドレスエラー      */
  	.long TO_ASM(II_VECTOR)		/*   4:一般不当命令        */
	.long TO_ASM(RESERVED_VECTOR)	/*   5:d zero              */
	.long TO_ASM(RESERVED_VECTOR)   /*   6:予約                */
        .long TO_ASM(II_VECTOR)         /*   7:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*   8:pvシステム予約        */
        .long TO_ASM(RESERVED_VECTOR)   /*   9:trace               */
	.long TO_ASM(RESERVED_VECTOR)	/*  10:line a emulator     */
	.long TO_ASM(RESERVED_VECTOR)	/*  11:line f emulator     */
	.long TO_ASM(RESERVED_VECTOR)	/*  12:システム予約        */
        .long TO_ASM(RESERVED_VECTOR)   /*  13:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  14:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  15:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  16:システム予約        */
        .long TO_ASM(RESERVED_VECTOR)   /*  17:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  18:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  19:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  20:システム予約        */
        .long TO_ASM(RESERVED_VECTOR)   /*  21:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  22:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  23:システム予約        */
	.long TO_ASM(RESERVED_VECTOR)	/*  24:スプリアス割り込み  */
	.long TO_ASM(RESERVED_VECTOR)	/*  残りはデフォルトの例外処理  */
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
	.long TO_ASM(RESERVED_VECTOR)	
        .long TO_ASM(RESERVED_VECTOR)   
	.long TO_ASM(RESERVED_VECTOR)

	/* CF-Flash Protection Field */
	.org 0x400
	.long 0xFFFFFFFF
	.long 0xFFFFFFFF
	.long 0x00000000
	.long 0x00000000
	.long 0x00000000
	.long 0xFFFFFFFF
	
#endif  /* KERNEL_HAS_A_VECTOR_TABLE */
	.text
	.globl start
	.align 2
start:
	/*
	 *  プロセッサモードとスタックポインタの初期化
	 */
	move.l #0x2700, %d0
	move.w  %d0, %sr

	/* SRAMベースアドレスの初期化 */
	move.l	#0x20000000, %d0
	add.l   #0x21, %d0
	.long   0x4e7b0C05  /* movec   %d0, %rambar1 */
	
	/* ROMベースアドレスの初期化 */
	move.l	#0x00000000, %d0
	add.l   #0x21, %d0
	.long   0x4e7b0C04  /* movec   %d0, %flashbar */
	
	movea.l #INIT_STACK, %sp	/* フレームポインタの初期化 */
	movea.l #0, %fp	
	/*
	 *  hardware_init_hook の呼出し（0 でない場合）
	 *
	 *  ターゲットハードウェアに依存して必要な初期化処理がある場合
	 *  は，hardware_init_hook という関数を用意する．
	 */
	lea hardware_init_hook, %a0
	tst.l %a0
	jbeq start_1
	jsr %a0@
start_1:
	move.l #__bss_start, %a0
	move.l #_end, %d0
	sub.l %a0, %d0
	jbeq start_3
start_2:
	clr.b (%a0)+
	subq.l	#1, %d0
	bne start_2
	clr.w %d0
	subq.l #1, %d0
	jbcc start_2
	
	/*
	 *  dataセクションの初期化（ROM化対応）
	 *
	 *  __idata_start から __idata_end までを，__data_start 以降に
	 *  コピーする．
	 */
start_3:
	move.l #__idata_end, %d0
	sub.l #__idata_start, %d0
	jbls start_5
	move.l #__idata_start, %a0
	move.l #__data_start, %a1
start_4:
	move.b (%a0)+, (%a1)+
	subq.l	#1, %d0
	bne start_4
	clr.w %d0
	subq.l #1, %d0
	jbcc start_4
	
	/*
	 *  software_init_hook を呼出し（0 でない場合）
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処
	 *  理がある場合は，software_init_hook という関数を用意すれば
	 *  よい．
	 */
start_5:
	lea software_init_hook, %a0
	tst.l %a0
	jbeq start_6
	jsr %a0@

	/*
	 *  カーネルを起動する．
	 */
start_6:
	jmp kernel_start
