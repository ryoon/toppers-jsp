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
 *  @(#) $Id: sys_config.c 2246 2025-12-09 10:59:26Z roi $
 */

/*
 *		ターゲット依存モジュール（SAPPHIRESOC用）
 */

#include "jsp_kernel.h"
#include <soc.h>
#include <hw_serial.h>		/* デバックシリアルコントローラ関係 */

#define TOFF_CLINT_MTIME    0xBFF8

FP vector_table[TNUM_PRCID][TMAX_INTNO+1];
UD compare_time;
W  molecule;

/*
 *  MACHINE割込みから割込みハンドラを呼び出す
 */
static void
machine_interrupt(unsigned long mcause, void *p_excinf)
{
	unsigned long hart_id = x_prc_index();
	UW	int_num = sil_rew_mem((UW *)(SYSTEM_PLIC_CTRL+TOFF_PLIC_CLAIM+(hart_id * NUM_PLIC_THRESHOLD)));
	UB	threshold = (UB)current_ithreshold();
	void (*volatile handler)(void);
	UB	priority;

	if(int_num == 0)	/* 割込み番号0の場合は未処理 */
		return;
	else if(int_num <= TMAX_INTNO){
		if((handler = (void (*volatile)(void))vector_table[hart_id][int_num]) != NULL){
			priority = current_ipriority(int_num);
			set_ithreshold((UW)priority);
			set_csr(mie, kernel_mie);
			sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+TOFF_PLIC_CLAIM+(hart_id * NUM_PLIC_THRESHOLD)), int_num);
			handler();
			clear_csr(mie, KERNEL_MIE);
			set_ithreshold((UW)threshold);
		}
	}
	else{
		syslog(LOG_EMERG, "Illegal global interrupt NO = %d", int_num);
		sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+TOFF_PLIC_CLAIM+(hart_id * NUM_PLIC_THRESHOLD)), int_num);
	}
}

/*
 *  ターゲット依存の初期化
 */
void
sys_initialize(void)
{
	unsigned long hart_id = x_prc_index();
	UW	i, off, len, hi, lo;

	/*
	 *  バナー出力用のシリアル初期化
	 */
	sio_init();

	/*
	 *  割込み関連の初期化
	 *
	 *  割込みレベルテーブルをHIに設定する．
	 *  PLICに対して割込み禁止、レベルを0、スレシュホールドを0に設定する．
	 */
	off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE);
	len = (TMAX_INTNO + 8) / 8;
	for(i = 0 ; i < len ; i += 4){
		sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL + off + i), 0);
	}

	/*
	 *  割込み優先度を0に設定
	 */
	len = (TMAX_INTNO + 1) * sizeof(UW);
	for(i = 0 ; i < len ; i++){
		sil_wrb_mem((UB *)(SYSTEM_PLIC_CTRL+TOFF_PLIC_PRIORITY+i), 0);
	}
	/*
	 *  デフォルト割込みハンドラの設定
	 */
	for(i = 0 ; i <= TMAX_INTNO ; i++){
		vector_table[x_prc_index()][i] = default_plic_handler;
	}

	/*
	 *  割込みスレシュホールドを0に設定
	 */
	off = (TOFF_PLIC_THRESHOLD + (hart_id * NUM_PLIC_THRESHOLD));
	sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL+off), 0);

	/*
	 *  MACHINE割込み設定
	 */
	machine_inh(IRQ_MACHINE_EXTERNAL, (FP)machine_interrupt);

	/*
	 *  タイマーコンペア値設定
	 */
	hi = sil_rew_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIME+4));
	lo = sil_rew_mem((UW *)(SYSTEM_CLINT_CTRL+TOFF_CLINT_MTIME));
	compare_time = ((UD)hi << 32) | lo;
}

/*
 *  ターゲット依存の終了処理
 */
void
sys_exit(void)
{
	unsigned long hart_id = x_prc_index();
	UW	i, off, len;

	/*
	 *  すべての割込みをマスクする．
	 */
	off = TOFF_PLIC_ENABLE + (hart_id * NUM_PLIC_ENABLE);
	len = (TMAX_INTNO + 8) / 8;
	for(i = 0 ; i < len ; i += 4){
		sil_wrw_mem((UW *)(SYSTEM_PLIC_CTRL + off + i), 0);
	}

	/*
	 *  開発環境依存の終了処理
	 */
	while(1);
}

/*
 *  イレギュラーなPDIC割込み処理
 */
void
default_plic_handler(void)
{
	syslog(LOG_EMERG, "\nUnregistered plic Interrupt occurs.");
	sys_exit();
}

/*
 *  システムログの低レベル出力のための文字出力
 */
void
sys_putc(char c)
{
	if (c == '\n') {
		sio_snd_chr_pol('\r');
	}
	sio_snd_chr_pol(c);
}

/*
 *  割込み要求ラインの属性の設定
 *
 *  JSPカーネルでの利用を想定して，パラメータエラーはアサーションでチェッ
 *  クしている．cfg_intサービスコールを設ける場合には，エラーを返すよう
 *  にすべきであろう．
 */
void
x_config_int(INTNO intno, BOOL active, PRI intpri)
{
	assert(intno < NUM_MACHNE_INTNO);
	assert(TMIN_INTPRI <= intpri && intpri <= TMAX_INTPRI);

	/*
	 *  割込みのマスク
	 *
	 *  割込みを受け付けたまま，レベルトリガ／エッジトリガの設定や，割
	 *  込み優先度の設定を行うのは危険なため，割込み属性にかかわらず，
	 *  一旦マスクする．
	 */
	(void) disable_int(intno);

	/*
	 *  割込み優先度の設定
	 */
	set_ipriority(intno, intpri);

	/*
	 *  割込みのマスク解除（必要な場合）
 	 */
	if (active) {
		(void) enable_int(intno);
	}
}
