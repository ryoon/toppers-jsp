/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2000-2003 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2002-2004 by Hokkaido Industrial Research Institute, JAPAN
 *  Copyright (C) 2010 by Cronus Computer Works, JAPAN
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
 *  @(#) $Id
 */

/*
 *	タイマドライバ
 *  CMTを使用
 */

#ifndef _SH726XCMT_H_
#define _SH726XCMT_H_

/*
 *   タイマへの入力クロックの分周比設定
 *   	f/8でカウント
 */
//#define TCR_CKS         0x0   /* sys_def.h 内へ移動 */

/*
 *   タイマに供給されるクロック周波数[kHz]
 */
#if TCR_CKS == 0
#define TIMER_CLOCK     ((PCLOCK / 1000) / 8)
#elif TCR_CKS == 1
#define TIMER_CLOCK     ((PCLOCK / 1000) / 32)
#elif TCR_CKS == 2
#define TIMER_CLOCK     ((PCLOCK / 1000) / 128)
#elif TCR_CKS == 3
#define TIMER_CLOCK     ((PCLOCK / 1000) / 512)
#endif


/*
 *  タイマ値の内部表現の型
 */
typedef UH CLOCK;

/*
 *  タイマ値の内部表現とミリ秒・μ秒単位との変換
 *
 */
#define	TO_CLOCK(nume, deno)	(TIMER_CLOCK * (nume) / (deno))
#define	TO_USEC(clock)		((clock) * 1000 / TIMER_CLOCK)
#define CLOCK_PER_TICK	((CLOCK) TO_CLOCK(TIC_NUME, TIC_DENO))

/*
 *  設定できる最大のタイマ周期（単位は内部表現）
 */
#define	MAX_CLOCK	((CLOCK) 0xffff)

/*
 *  タイマの現在値を割込み発生前の値とみなすかの判断
 */
#define	GET_TOLERANCE	100		/* 処理遅れの見積り値（単位は内部表現） */
#define	BEFORE_IREQ(clock) \
		((clock) >= TO_CLOCK(TIC_NUME, TIC_DENO) - GET_TOLERANCE)

/*
 *  コントロールレジスタのアドレス
 */
#ifndef CMSTR
#define CMSTR	(VH *)0xfffec000
#define CMCSR_0	(VH *)0xfffec002
#define CMCNT_0	(VH *)0xfffec004
#define CMCOR_0	(VH *)0xfffec006
#endif


#ifndef _MACRO_ONLY
/*
 *  タイマスタート
 */
Inline void
sh2_timer_start ()
{
	sil_wrh_mem (CMSTR, sil_reh_mem (CMSTR) | 0x0001);
}

/*
 *  タイマ一時停止
 */
Inline void
sh2_timer_stop ()
{
    	/* タイマを停止 	  */
	sil_wrh_mem (CMSTR, sil_reh_mem (CMSTR) & ~0x0001);
}

/*
 *  タイマ割込み要求のクリア
 *	TCRレジスタのIMFAビットは1回読み出した後に０を書き込む
 */
Inline void
sh2_timer_int_clear ()
{
	/* 割り込み要求をクリア     */
	sil_wrh_mem (CMCSR_0, sil_reh_mem (CMCSR_0) & ~0x0080);
				/* CMFコンペアマッチフラグのクリア */
}


/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
sh2_timer_initialize ()
{
	CLOCK cyc = TO_CLOCK (TIC_NUME, TIC_DENO);

#if defined(SH7203) || defined(SH7263)
	sil_wrb_mem (STBCR4, (sil_reb_mem(STBCR4)& ~0x04));
#endif
#if defined(SH7262) || defined(SH7264)
	sil_wrb_mem (STBCR7, (sil_reb_mem(STBCR7)& ~0x04));
#endif
	/*
	 *  タイマ関連の設定
	 */
	sh2_timer_stop ();			/* タイマ停止 */
	/* タイマ上限値のチェック */
	assert (cyc <= MAX_CLOCK);

	/*  分周比設定  clock 1/8  、割り込み禁止 */
    sil_wrh_mem(CMCSR_0,0x0000);
	/*  CMCOR0レジスタ設定（カウンタ目標値）      */
	sil_wrh_mem (CMCOR_0,(VH)CLOCK_PER_TICK);
	/* カウンタをクリア             */
	sil_wrh_mem (CMCNT_0,0x0000);
	sil_wrh_mem (CMCSR_0, sil_reh_mem(CMCSR_0) | 0x0040);	/* 割り込み許可 */
}

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
Inline void
sh2_timer_terminate ()
{
	sh2_timer_stop ();			/* タイマを停止     */
	sh2_timer_int_clear ();		/* 割り込み要求をクリア */

	sil_wrh_mem (CMCSR_0, sil_reh_mem(CMCSR_0) & ~0x0040);	//割り込み不許可
	sil_wrh_mem (CMCNT_0,0x0000);
	sil_wrh_mem (CMCOR_0,0x0000);
#if defined(SH7203) || defined(SH7263)
	sil_wrb_mem (STBCR4, (sil_reb_mem(STBCR4) | 0x04));
#endif
#if defined(SH7262) || defined(SH7264)
	sil_wrb_mem (STBCR7, (sil_reb_mem(STBCR7) | 0x04));
#endif
}

/*
 *  タイマの現在値の読出し
 *　　　タイマの動作を一時的に停止し，タイマ値を読み出す．
 */
Inline CLOCK
sh2_timer_get_current ()
{
	CLOCK	clk;
/* SH1と同じ */
	sh2_timer_stop ();			/*  タイマ停止  */

	/*  本来は待ち時間を入れるべき  */

	clk = sil_reh_mem(CMCNT_0);
	sh2_timer_start();			/*  タイマスタート  */
	
	return(clk);
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
sh2_timer_fetch_interrupt ()
{
	return (sil_reh_mem (CMCSR_0) & 0x0080);
}

#endif /* _MACRO_ONLY */
#endif /* _SH726XCMT_H_ */
