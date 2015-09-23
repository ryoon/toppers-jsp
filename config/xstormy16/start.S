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

/*
 *      カーネル用のスタートアップモジュール（Xstormy16用）
 */

#define _MACRO_ONLY
#include <cpu_sfrs.h>

/*
 *  機種情報(sys_support.S)をリンクするため
 */
        .equ dummy,_chip_info

/*
 *      RESET時の初期化処理
 */
        .text
	.global _int_reset
_int_reset:

                                /*
                                 *  スタックポインタの初期化
                                 */
        mov sp,#__stack

                                /*
                                 *  初期値なしの変数域のゼロクリア
                                 */
        mov r0,#__bss_start
        mov r1,#__bss_end
        mov r2,#0
1:      mov.w (r0++),r2
        blt r0,r1,1b

                                /*
                                 *  初期値ありの変数域の初期値設定
                                 */
        mov r1,#__data_start
        mov r3,#__data_end
        mov r0,#@lo(__rdata)
        mov r8,#@hi(__rdata)
2:      bz r1,r3,3f
        movf.w r2,(r0)
        mov.w (r1++),r2
        add r0,#2
        bnz 2b
        add r8,#1
        br 2b
3:

                                /*
                                 *  初期化ルーチンの呼び出し
                                 */
        callf _init

                                /*
                                 *  カーネルの起動
                                 */
        jmpf kernel_start

        .end
