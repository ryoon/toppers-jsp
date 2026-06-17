/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: sys_config.h 1952 2025-12-09 10:58:09Z roi $
 */

/*
 *		ターゲット依存モジュール（SAPPHIRESOC用）
 */

#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

/*
 *  カーネルの内部識別名のリネーム
 */
#include <sys_rename.h>

/*
 *  RISC-Vマクロの定義
 */
#include <cpu_config.h>

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include <sil.h>
#include "soc.h"

/*
 *  システムクロックの定義
 */
#define SYS_CLOCK		    (SYSTEM_CLINT_HZ)

/*
 *  割込み番号の最大値
 */
#define TMAX_INTNO          (31)

/*
 *  カーネル管理の割込み優先度の範囲
 */
#ifndef TMIN_INTPRI
#define TMIN_INTPRI		(0)		/* 割込み優先度の最小値（最低値）*/
#endif /* TMIN_INTPRI */
#define TMAX_INTPRI		(3)		/* 割込み優先度の最大値（最高値） */

/*
 *  システムタイマ割込み優先度
 */
#define INTPRI_TIMER    2

/*
 *  MACHINE INTERRUPT NUMBER
 */
#define IRQ_MACHINE_SOFTWARE    3
#define IRQ_MACHIE_TIMER        7
#define IRQ_MACHINE_EXTERNAL    11

/*
 *  INTERRUPT NUMBER
 */
#define IRQ_VECTOR_CORE0    SYSTEM_PLIC_SYSTEM_CORES_0_EXTERNAL_INTERRUPT
#define IRQ_VECTOR_UART0    SYSTEM_PLIC_SYSTEM_UART_0_IO_INTERRUPT
#define IRQ_VECTOR_SPI0     SYSTEM_PLIC_SYSTEM_SPI_0_IO_INTERRUPT
#define IRQ_VECTOR_SPI1     SYSTEM_PLIC_SYSTEM_SPI_1_IO_INTERRUPT
#define IRQ_VECTOR_I2C0     SYSTEM_PLIC_SYSTEM_I2C_0_IO_INTERRUPT
#define IRQ_VECTOR_TIMER0   SYSTEM_PLIC_SYSTEM_USER_TIMER_0_INTERRUPTS_0
#define IRQ_VECTOR_GPIO00   SYSTEM_PLIC_SYSTEM_GPIO_0_IO_INTERRUPTS_0
#define IRQ_VECTOR_GPIO01   SYSTEM_PLIC_SYSTEM_GPIO_0_IO_INTERRUPTS_1

/*
 *  PLIC定義
 */
#define TOFF_PLIC_PRIORITY  0x00000000
#define TOFF_PLIC_PENDING   0x00001000
#define TOFF_PLIC_ENABLE    0x00002000
#define NUM_PLIC_ENABLE     128
#define TOFF_PLIC_THRESHOLD 0x00200000
#define NUM_PLIC_THRESHOLD  4096
#define TOFF_PLIC_CLAIM     0x00200004

/*
 *  ターゲット定義のオブジェクト属性
 */
#define TARGET_INHATR	TA_NONKERNEL	/* カーネル管理外の割込み */

/*
 *  chg_ipmをサポートするかどうかの定義
 */
#define SUPPORT_CHG_IPM

/*
 *  vgxet_tim をサポートするかどうかの定義
 */
#define	SUPPORT_VXGET_TIM

/*
 *  デバッグシリアルポート数の定義 (VR4131内蔵のDSIUを利用する。)
 */
#define TNUM_PORT	1u	/* サポートするシリアルポートの数 */
#define	TNUM_SIOP	1u	/* サポートするシリアルI/Oポートの数 */

/*
 *  システムタスクに関する定義
 */
#define	LOGTASK_PORTID	1u	/* システムログを出力するシリアルポート番号 */

/*
 *  シリアルのデフォルトボーレート定義
 */
#define DEF_BAUDRATE    115200

/*
 *  微少時間待ちのための定義（本来はSILのターゲット依存部）
 */
#define SIL_DLY_TIM1    10
#define SIL_DLY_TIM2    110


#ifndef _MACRO_ONLY

extern FP vector_table[TNUM_PRCID][TMAX_INTNO+1];
extern UD compare_time;
extern W  molecule;

/*
 *  PLIC割込みハンドラテーブル
 */
extern const FP *p_int_table[];

/*
 *  PLIC THRESHOLD（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline UW
current_ithreshold(void)
{
	unsigned long hard_id = x_prc_index();
	UW	off = TOFF_PLIC_THRESHOLD + (hard_id * NUM_PLIC_THRESHOLD);
	return sil_rew_mem((UW *)(SYSTEM_PLIC_CTRL+off));
}

/*
 *  PLIC THRESHOLD（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_ithreshold(UW threshold)
{
	unsigned long hard_id = x_prc_index();
	UW	off = TOFF_PLIC_THRESHOLD + (hard_id * NUM_PLIC_THRESHOLD);
	sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+off), threshold);
}

/*
 *  PLIC 割込み優先度（ハードウェアの割込み優先度マスク，内部表現）の現在値の読出し
 */
Inline UW
current_ipriority(INTNO intno)
{
	UW	off = TOFF_PLIC_PRIORITY + (intno * sizeof(UW));
	return (UW)sil_rew_mem((UW *)(SYSTEM_PLIC_CTRL+off));
}

/*
 *  PLIC 割込み優先度（ハードウェアの割込み優先度マスク，内部表現）の現在値の設定
 */
Inline void
set_ipriority(INTNO intno, UW priority)
{
	UW	off = TOFF_PLIC_PRIORITY + (intno * sizeof(UW));
	sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+off), priority);
}

/*
 *  割込み要求禁止フラグのセット
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをセットしようとした場合には，falseを返す．
 */
Inline BOOL
disable_int(INTNO intno)
{
	unsigned long hart_id = x_prc_index();
	UW	bitpat = 1 << (intno & 0x1f);
	UW	bitpno = intno >> 5;
	UW	off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE) + (bitpno * 4);

	if(intno > TMAX_INTNO){
		return FALSE;
	}
	sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+off), (sil_rew_mem((UW *)(SYSTEM_PLIC_CTRL+off)) & ~bitpat));
	return TRUE;
}

/*
 *  割込み要求禁止フラグのクリア
 *
 *  割込み属性が設定されていない割込み要求ラインに対して割込み要求禁止
 *  フラグをクリアしようとした場合には，falseを返す．
 */
Inline BOOL
enable_int(INTNO intno)
{
	unsigned long hart_id = x_prc_index();
	UW	bitpat = 1 << (intno & 0x1f);
	UW	bitpno = intno >> 5;
	UW	off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE) + (bitpno * 4);

	if(intno > TMAX_INTNO){
		return FALSE;
	}
	sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+off),
			(sil_rew_mem((UW *)(SYSTEM_PLIC_CTRL+off)) | bitpat));
	return TRUE;
}

/*
 *  割込み要求のクリア
 */
Inline void
clear_int(INTNO intno)
{
}

/*
 *  割込み要求のチェック
 */
Inline BOOL
probe_int(INTNO intno)
{
	unsigned long hart_id = x_prc_index();
	UW	off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE) + (intno >> 3);

	return((sil_reb_mem((UB *)(SYSTEM_PLIC_CTRL+off)) & (1 << (intno & 7))) != 0);
}

/*
 *  割込みハンドラの設定
 */
Inline void
define_inh(INHNO inhno, FP int_entry)
{
	if(inhno > TMAX_INTNO)
		timer_inh(int_entry);
	else
		vector_table[x_prc_index()][inhno] = int_entry;
}

/*
 *  割込みハンドラの出入口処理の生成マクロ
 *
 */
#define INTHDR_ENTRY(inthdr)  extern void inthdr(void)

#define INT_ENTRY(inthdr) inthdr

/*
 *  ターゲットシステム依存の初期化
 */
extern void	sys_initialize(void);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void	sys_exit(void);

/*
 *  イレギュラーなPDIC割込み処理
 */
extern void default_plic_handler(void);

/*
 *  ターゲットシステムの文字出力
 *
 *  システムの低レベルの文字出力ルーチン．ROMモニタ呼出しで実現するこ
 *  とを想定している．
 */

extern void	sys_putc(char c) throw();

/*
 *  割込み要求ラインの属性の設定
 *
 *  SAPPHIRESOCでは，優先度0～3までの設定．
 */
extern void	x_config_int(INTNO intno, BOOL active, PRI intpri);


#endif /* _MACRO_ONLY */


#endif /* _SYS_CONFIG_H_ */
