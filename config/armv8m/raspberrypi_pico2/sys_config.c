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
 *  @(#) $Id: sys_config.c 2246 2025-11-06 18:33:12Z roi $
 */

/*
 *		ターゲット依存モジュール（Raspberry Pi Pico2用）
 */

#include "jsp_kernel.h"
#include <hw_timer.h>		/* システム・タイマー関係 */
#include <hw_interpro.h>	/* プロセッサ間割込み関係 */
#include <hw_serial.h>		/* デバックシリアルコントローラ関係 */

/*
 *  ベクターテーブル
 */
__attribute__ ((section(".vector"))) 
const FP vector_table[TNUM_PRCID][NUM_VECTORS] = {
  {
	(FP)STACKTOP,			/* 0 */
	(FP)(start),			/* 1 */
	(FP)(cpu_exc_entry),	/* 2 */
	(FP)(cpu_exc_entry),	/* 3 */
	(FP)(cpu_exc_entry),	/* 4 */
	(FP)(cpu_exc_entry),	/* 5 */
	(FP)(cpu_exc_entry),	/* 6 */
	(FP)(cpu_exc_entry),	/* 7 */
	(FP)(cpu_exc_entry),	/* 8 */
	(FP)(cpu_exc_entry),	/* 9 */
	(FP)(cpu_exc_entry),	/* 10 */
	(FP)(cpu_exc_entry),	/* 11 */
	(FP)(cpu_exc_entry),	/* 12 */
	(FP)(cpu_exc_entry),	/* 13 */
	(FP)(pendsvc_handler),	/* 14 */
 	(FP)(cpu_int_entry),	/* 15 */
 	(FP)(cpu_int_entry),	/* 0 */
 	(FP)(cpu_int_entry),	/* 1 */
 	(FP)(cpu_int_entry),	/* 2 */
 	(FP)(cpu_int_entry),	/* 3 */
 	(FP)(cpu_int_entry),	/* 4 */
 	(FP)(cpu_int_entry),	/* 5 */
 	(FP)(cpu_int_entry),	/* 6 */
 	(FP)(cpu_int_entry),	/* 7 */
 	(FP)(cpu_int_entry),	/* 8 */
 	(FP)(cpu_int_entry),	/* 9 */
 	(FP)(cpu_int_entry),	/* 10 */
 	(FP)(cpu_int_entry),	/* 11 */
 	(FP)(cpu_int_entry),	/* 12 */
 	(FP)(cpu_int_entry),	/* 13 */
 	(FP)(cpu_int_entry),	/* 14 */
 	(FP)(cpu_int_entry),	/* 15 */
 	(FP)(cpu_int_entry),	/* 16 */
 	(FP)(cpu_int_entry),	/* 17 */
 	(FP)(cpu_int_entry),	/* 18 */
 	(FP)(cpu_int_entry),	/* 19 */
 	(FP)(cpu_int_entry),	/* 20 */
 	(FP)(cpu_int_entry),	/* 21 */
 	(FP)(cpu_int_entry),	/* 22 */
 	(FP)(cpu_int_entry),	/* 23 */
 	(FP)(cpu_int_entry),	/* 24 */
 	(FP)(cpu_int_entry),	/* 25 */
 	(FP)(cpu_int_entry),	/* 26 */
 	(FP)(cpu_int_entry),	/* 27 */
 	(FP)(cpu_int_entry),	/* 28 */
 	(FP)(cpu_int_entry),	/* 29 */
 	(FP)(cpu_int_entry),	/* 30 */
 	(FP)(cpu_int_entry),	/* 31 */
 	(FP)(cpu_int_entry),	/* 32 */
 	(FP)(cpu_int_entry),	/* 33 */
 	(FP)(cpu_int_entry),	/* 34 */
 	(FP)(cpu_int_entry),	/* 35 */
 	(FP)(cpu_int_entry),	/* 36 */
 	(FP)(cpu_int_entry),	/* 37 */
 	(FP)(cpu_int_entry),	/* 38 */
 	(FP)(cpu_int_entry),	/* 39 */
 	(FP)(cpu_int_entry),	/* 40 */
 	(FP)(cpu_int_entry),	/* 41 */
 	(FP)(cpu_int_entry),	/* 42 */
 	(FP)(cpu_int_entry),	/* 43 */
 	(FP)(cpu_int_entry),	/* 44 */
 	(FP)(cpu_int_entry),	/* 45 */
 	(FP)(cpu_int_entry),	/* 46 */
 	(FP)(cpu_int_entry),	/* 47 */
 	(FP)(cpu_int_entry),	/* 48 */
 	(FP)(cpu_int_entry),	/* 49 */
 	(FP)(cpu_int_entry),	/* 50 */
 	(FP)(cpu_int_entry)		/* 51 */
  },
#if TNUM_PRCID > 1
  {
	(FP)STACKTOP-PSTACKSIZE, /* 0 */
	(FP)(start),			/* 1 */
	(FP)(cpu_exc_entry),	/* 2 */
	(FP)(cpu_exc_entry),	/* 3 */
	(FP)(cpu_exc_entry),	/* 4 */
	(FP)(cpu_exc_entry),	/* 5 */
	(FP)(cpu_exc_entry),	/* 6 */
	(FP)(cpu_exc_entry),	/* 7 */
	(FP)(cpu_exc_entry),	/* 8 */
	(FP)(cpu_exc_entry),	/* 9 */
	(FP)(cpu_exc_entry),	/* 10 */
	(FP)(cpu_exc_entry),	/* 11 */
	(FP)(cpu_exc_entry),	/* 12 */
	(FP)(cpu_exc_entry),	/* 13 */
	(FP)(pendsvc_handler),	/* 14 */
 	(FP)(cpu_int_entry),	/* 15 */
 	(FP)(cpu_int_entry),	/* 0 */
 	(FP)(cpu_int_entry),	/* 1 */
 	(FP)(cpu_int_entry),	/* 2 */
 	(FP)(cpu_int_entry),	/* 3 */
 	(FP)(cpu_int_entry),	/* 4 */
 	(FP)(cpu_int_entry),	/* 5 */
 	(FP)(cpu_int_entry),	/* 6 */
 	(FP)(cpu_int_entry),	/* 7 */
 	(FP)(cpu_int_entry),	/* 8 */
 	(FP)(cpu_int_entry),	/* 9 */
 	(FP)(cpu_int_entry),	/* 10 */
 	(FP)(cpu_int_entry),	/* 11 */
 	(FP)(cpu_int_entry),	/* 12 */
 	(FP)(cpu_int_entry),	/* 13 */
 	(FP)(cpu_int_entry),	/* 14 */
 	(FP)(cpu_int_entry),	/* 15 */
 	(FP)(cpu_int_entry),	/* 16 */
 	(FP)(cpu_int_entry),	/* 17 */
 	(FP)(cpu_int_entry),	/* 18 */
 	(FP)(cpu_int_entry),	/* 19 */
 	(FP)(cpu_int_entry),	/* 20 */
 	(FP)(cpu_int_entry),	/* 21 */
 	(FP)(cpu_int_entry),	/* 22 */
 	(FP)(cpu_int_entry),	/* 23 */
 	(FP)(cpu_int_entry),	/* 24 */
 	(FP)(cpu_int_entry),	/* 25 */
 	(FP)(cpu_int_entry),	/* 26 */
 	(FP)(cpu_int_entry),	/* 27 */
 	(FP)(cpu_int_entry),	/* 28 */
 	(FP)(cpu_int_entry),	/* 29 */
 	(FP)(cpu_int_entry),	/* 30 */
 	(FP)(cpu_int_entry),	/* 31 */
 	(FP)(cpu_int_entry),	/* 32 */
 	(FP)(cpu_int_entry),	/* 33 */
 	(FP)(cpu_int_entry),	/* 34 */
 	(FP)(cpu_int_entry),	/* 35 */
 	(FP)(cpu_int_entry),	/* 36 */
 	(FP)(cpu_int_entry),	/* 37 */
 	(FP)(cpu_int_entry),	/* 38 */
 	(FP)(cpu_int_entry),	/* 39 */
 	(FP)(cpu_int_entry),	/* 40 */
 	(FP)(cpu_int_entry),	/* 41 */
 	(FP)(cpu_int_entry),	/* 42 */
 	(FP)(cpu_int_entry),	/* 43 */
 	(FP)(cpu_int_entry),	/* 44 */
 	(FP)(cpu_int_entry),	/* 45 */
 	(FP)(cpu_int_entry),	/* 46 */
 	(FP)(cpu_int_entry),	/* 47 */
 	(FP)(cpu_int_entry),	/* 48 */
 	(FP)(cpu_int_entry),	/* 49 */
 	(FP)(cpu_int_entry),	/* 50 */
 	(FP)(cpu_int_entry)		/* 51 */
  }
#endif /* TNUM_PRCID */
};

UW SystemFrequency;

/*
 *  ターゲット依存の初期化
 */
void
sys_initialize(void)
{
	UINT	idx = x_prc_index();

	/*
	 *  システムクロック設定
	 */
	SystemFrequency = get_Clock(CLK_SYS);

#if TNUM_PRCID > 1
	if (idx == 0) {
		/*
		 *  コア１起動
		 */
		mprc_initialize(start, (UW *)(STACKTOP-PSTACKSIZE), (UW)vector_table[1]);
	}
#endif /* TNUM_PRCID > 1 */

	/*
	 *  バナー出力用のシリアル初期化
	 */
	sio_init();

	/*
	 *  プロセッサ間割込み初期化(コア1を先に初期化しなければならない)
	 */
	interpro_init();
}

/*
 *  ターゲット依存の終了処理
 */
void
sys_exit(void)
{
	/*
	 *  開発環境依存の終了処理
	 */
	while(1);
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
			Asm("SEV");
		}
		/* send a command */
		while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_RDY) == 0);
		sil_wrw_mem((VP)(TADR_SIO_FIFO_WR), c);
		Asm("SEV");

		/* receive a response */
		while((sil_rew_mem((VP)(TADR_SIO_FIFO_ST)) & SIO_FIFO_ST_VLD) == 0){
			Asm("WFE");
		}
		if(c == sil_rew_mem((VP)(TADR_SIO_FIFO_RD)))
			i++;
    }
}
