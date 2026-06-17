/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *                2003      by Advanced Data Controls, Corp
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2007-     by Monami Software Limited Partnership, JAPAN
 *  Copyright (C) 2009      by GJ Designing Center Print Business Group
 *                             RICOH COMPANY,LTD. JAPAN
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: sys_config.h,v 1.1 2009/08/17 11:38:40 roi Exp $
 */


/*
 *  ターゲットシステム依存モジュール（mindstrmsnxt用）
 *
 *  このインクルードファイルは，t_config.h のみからインクルードされる．
 *  他のファイルから直接インクルードしてはならない．
 */


#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include <mindstormsnxt.h>

/*
 * 割込みハンドラの最大数
 */
#define MAX_INT_NUM 32

/*
 *  vxget_tim をサポートするかどうかの定義
 */
/* #define  SUPPORT_VXGET_TIM */

/*
 *  シリアルポート番号に関する定義
 */
#define TNUM_PORT   1   /* サポートするシリアルポートの数 */
#define TNUM_SIOP   1   /* サポートするシリアルI/Oポートの数 */

/*
 *  シリアルポート番号に関する定義
 */
#define	CONSOLE_PORTID  1	/* コンソールに用いるシリアルポート番号   */
#define LOGTASK_PORTID  1   /* システムログに用いるシリアルポート番号 */

/*
 *  割込み待ち状態での実行ルーチン
 */
#ifdef ARM920T_CORE
#define WAIT_INTERRUPT  mcr p15, 0, r3, c7, c0, 4
#else
#define WAIT_INTERRUPT  nop
#endif

/*
 *  プロセッサのエンディアン
 */
#define SIL_ENDIAN  SIL_ENDIAN_LITTLE  /* リトルエンディアン */

/*
 *  RAMの最終アドレス
 */
#define HEAP_TOP    (0x00200000+64*1024)

#ifndef _MACRO_ONLY
/*
 *  割込みレベル設定テーブル
 */
extern UW int_mode_table[MAX_INT_NUM];

/*
 *  ターゲットシステム依存の初期化
 */
extern void    sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．ROMモニタ呼出しで実現することを想定し
 *  ている．
 */
extern void    sys_exit(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */
extern void    sys_putc(char c);

/*
 *  IRQハンドラ(sys_support.S)
 */
extern void IRQ_Handler(void);
extern void FIQ_Handler(void);

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
/*  C言語用のラベル生成  */
#define	INT_ENTRY(inthdr)	_kernel_##inthdr##_entry


/*
 *　　割込みの入口処理を生成するマクロの定義
 *　　　（割込み要因毎に異なる部分）
 *
 *　　　パラメータ
 *	　　entry：入口処理のラベル
 *　　　　　inthdr：C言語ルーチンの関数名（先頭の'_'は付けない）
 *
 *　　　lr-4はタスクの復帰アドレス
 *　　　　レジスタ割り当て
 *　　　　　・r0～r3ワークレジスタ
 *
 */
#define	__INTHDR_ENTRY(entry, inthdr)	 \
extern void INT_ENTRY(inthdr)(void) throw();	\
asm(".text					\n"\
"	.align 2				\n"\
"   .arm                    \n"\
"	.global "#entry"		\n"\
#entry":					\n"\
"   sub   lr,lr,#4          \n"	/* Save registers on stack */\
"	stmfd sp!,{lr}          \n"\
"	mrs   lr,spsr			\n"\
"	stmfd sp!,{lr}          \n"\
	/* Acknowledge the IVR for debugging to support Protected Mode */\
"	ldr   lr,=0xFFFFF100	\n"\
"	str   lr,[lr]			\n"\
    /* タスクの動作時モード(スーパーバイザーモード)へ */\
"	msr     cpsr_fc,#0xd3   \n"	/* (CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)*/\
"	stmfd   sp!, {r0-r3,ip,lr,pc}	\n"\
    /* spsrと戻り番地を取得するためにIRQモードへ */\
"	msr     cpsr_fc,#0xd2   \n"	/* (CPSR_IRQ|CPSR_FIQ_BIT|CPSR_IRQ_BIT) */\
"	ldmfd   sp!, {r1}       \n"	/* restore spsr */\
"	ldmfd   sp!, {r0}       \n"	/* restore lr */\
    /* スーパーバイザーモードに */\
"	and     r2,r1, #0x40    \n"	/* (CPSR_FIQ_BIT) */\
"	orr     r2,r2, #0x93    \n"	/* (CPSR_SVC|CPSR_IRQ_BIT) */\
"	msr     cpsr_fc,r2      \n"\
"	str     r0,[sp,#0x18]   \n"	/* lr into pc */\
"	stmfd   sp!,{r1}		\n"	/* save spcr */\
    /* 多重割り込みか判定 */\
"	ldr     r2, =_kernel_interrupt_count	\n"\
"	ldr     r3,[r2]         \n"\
"	add     r0,r3,#1        \n"\
"	str     r0,[r2]         \n"\
"	cmp     r3,#0x00        \n"\
	/* ネスト割り込みでない場合、スタックの変更 */\
"	moveq   r2,sp			\n"\
"	ldreq   sp,=STACKTOP	\n"\
"	stmeqfd sp!,{r2}		\n"\
	/*  C言語ルーチンの先頭アドレス  */	 \
"	ldr     r1,="#inthdr"	\n"\
	/*  割り込み許可 */\
"	mrs     r2,cpsr         \n"\
"	bic     r2,r2,#0x80     \n"	/* (CPSR_IRQ_BIT) */\
"	msr     cpsr,r2         \n"\
	/*  ハンドラの呼び出し */\
"	mov     lr,pc           \n"\
"	bx      r1              \n"\
	/*  割り込み禁止 */\
"	mrs     r2,cpsr         \n"\
"	and     r2,r2,#0x40     \n"	/* (CPSR_FIQ_BIT) */\
"	orr     r2,r2,#0x93     \n"	/* (CPSR_SVC|CPSR_IRQ_BIT) */\
"	msr     cpsr,r2			\n"\
	/*  割込みクリア */\
"	ldr     r3, =0xFFFFF130 \n"	/* TADR_AIC_BASE+TOFF_AIC_EOICR */\
"	mov     r0, #0			\n"\
"	str     r0, [r3]        \n"\
	/*  割込みネスト回数(interrupt_count) デクリメント */\
"	ldr     r2, =_kernel_interrupt_count	\n"\
"	ldr     r1, [r2]		\n"\
"	sub     r3, r1, #1		\n"\
"	str     r3, [r2]		\n"\
"	cmp     r3, #0x00		\n"\
"	bne     "#entry"_1		\n"\
	/*  タスクスタックの復帰 */\
"	ldmfd   sp!,{r0}		\n"\
"	mov     sp, r0			\n"\
	/* reqflgのチェック */\
"	ldr     r1, =_kernel_reqflg	\n"\
"	ldr     r0, [r1]		\n"\
"	cmp     r0, #0			\n"\
"	beq     "#entry"_1		\n"\
"	mov     r0, #0			\n"\
"	str     r0, [r1]		\n"\
"	b       _kernel_ret_int \n"\
#entry"_1:					\n"\
	/*  復帰処理 */\
	/*  割り込み許可となるがタスクコンテキスト上に保存しているため，*/\
	/*  問題はない */\
"	ldmfd   sp!,{r1}		\n"	/* restore spcr */\
"	mrs     r2,cpsr         \n"\
"	and     r2,r2,#0x40     \n"	/* (CPSR_FIQ_BIT) */\
"	bic     r1,r1,#0x40     \n"	/* (CPSR_FIQ_BIT) */\
"	orr     r1, r1, r2		\n"\
"	msr     spsr, r1		\n"\
	/*  タスク復帰 + 割込み許可 */\
"	ldmfd   sp!,{r0-r3,ip,lr,pc}^	")

/*  __INTHDR_ENTRY()マクロ　ここまで  */

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define _INTHDR_ENTRY(entry, inthdr)				\
	__INTHDR_ENTRY(entry, inthdr)

#define INTHDR_ENTRY(inthdr)				\
	_INTHDR_ENTRY(INT_ENTRY(inthdr), inthdr)


/*
 *  割込みハンドラの設定
 *
 *  割込み番号 inhno の割込みハンドラの起動番地を inthdr に設定する．
 */
extern void define_inh(INHNO inhno, FP inthdr);




#endif /* _MACRO_ONLY */
#endif /* _SYS_CONFIG_H_ */
