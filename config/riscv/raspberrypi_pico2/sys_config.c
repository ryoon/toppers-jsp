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
 *  @(#) $Id: sys_config.c 2246 2025-12-15 14:52:07Z roi $
 */

/*
 *		ターゲット依存モジュール（Raspberry Pi Pico2 RISC-V用）
 */

#include "jsp_kernel.h"
#include <hw_timer.h>		/* システム・タイマー関係 */
#include <hw_serial.h>		/* デバックシリアルコントローラ関係 */


static FP vector_table[TNUM_PRCID][TMAX_INTNO+1];
const FP *p_int_table[TNUM_PRCID] = {
	vector_table[0],
};

UW SystemFrequency;
UD compare_time;
W  molecule;

/*
 *  ターゲット依存の初期化
 */
void
sys_initialize(void)
{
	UW	i, hi, lo;

	/*
	 *  システムクロック設定
	 */
	SystemFrequency = get_Clock(CLK_SYS);

	/*
	 *  割込みベクタ設定
	 */
	write_csr(mtvec, (UW)ivector + 1);

	/*
	 *  バナー出力用のシリアル初期化
	 */
	sio_init();

	/*
	 *  割込み関連の初期化
	 *
	 *  割込みレベルテーブルをHIに設定する．
	 *  HAZARD3に対して割込み禁止に設定する．
	 */
	hazard3_irqarray_clear(RVCSR_MEIFA, 0, 0xffff);
	hazard3_irqarray_clear(RVCSR_MEIFA, 1, 0xffff);
	hazard3_irqarray_clear(RVCSR_MEIFA, 2, 0xffff);
	hazard3_irqarray_clear(RVCSR_MEIFA, 3, 0xffff);

	/*
	 *  デフォルト割込みハンドラの設定
	 */
	for(i = 0 ; i <= TMAX_INTNO ; i++){
		vector_table[0][i] = default_hazard3_handler;
	}

	/*
	 *  割込みスレシュホールドを0に設定
	 */
	set_ithreshold(0);

	/*
	 *  タイマーコンペア値設定
	 */
	hi = sil_rew_mem((UW *)(TADR_SIO_MTIME+4));
	lo = sil_rew_mem((UW *)(TADR_SIO_MTIME));
	compare_time = ((UD)hi << 32) | lo;
	x_config_int(INTNO_TIMER, TRUE, INTPRI_TIMER);
}

/*
 *  ターゲット依存の終了処理
 */
void
sys_exit(void)
{
	/*
	 *  すべての割込みをマスクする．
	 */
	hazard3_irqarray_clear(RVCSR_MEIFA, 0, 0xffff);
	hazard3_irqarray_clear(RVCSR_MEIFA, 1, 0xffff);
	hazard3_irqarray_clear(RVCSR_MEIFA, 2, 0xffff);
	hazard3_irqarray_clear(RVCSR_MEIFA, 3, 0xffff);

	/*
	 *  開発環境依存の終了処理
	 */
	while(1);
}

/*
 *  イレギュラーな割込み処理
 */
void
default_hazard3_handler(void)
{
	syslog(LOG_EMERG, "\nUnregistered Interrupt occurs.");
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
	assert(intno <= TMAX_INTNO);
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

/*
 *  コア１の実行
 */
void
mprc_initialize(func_t entry, UW *sp, UW vec)
{
    UW	c, i = 0;

    /* Boot CPU1 */

    sil_wrw_mem((VP)(TADR_PSM_FRCE_ON), PSM_FRCE_ON_PROC1);
    while((sil_rew_mem((VP)(TADR_PSM_DONE)) & PSM_DONE_PROC1) == 0);

	/* PROC0割込みは許可していない */

    const UW cmd[] =
            {0, 0, 1, (UW) vec, (UW) sp, (UW) entry};

	while(i < sizeof(cmd) / sizeof(cmd[0])){
		c = cmd[i];
		if(c == 0){
			/* flush garbage */
			while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_VLD) != 0){
				(void)sil_rew_mem((VP)(TADR_SIO_FIFO_RD));
			}
			Asm("slt x0, x0, x1" : : : "memory");
		}
		/* send a command */
		while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_RDY) == 0);
		sil_wrw_mem((VP)(TADR_SIO_FIFO_WR), c);
		Asm("slt x0, x0, x1" : : : "memory");

		/* receive a response */
		while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_VLD) == 0){
			Asm("slt x0, x0, x0" : : : "memory");
		}
		if(c == sil_rew_mem((VP)(TADR_SIO_FIFO_RD)))
			i++;
    }
}
