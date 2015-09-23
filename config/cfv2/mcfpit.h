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
 *  @(#) $Id: sh7615frt.h,v 1.5 2005/07/06 00:45:07 honda Exp $
 */

/*
 *	タイマドライバ
 *  FRTを使用
 */

#ifndef _MCFPIT_H_
#define _MCFPIT_H_

/*
 *   タイマへの入力クロックの分周比設定
 *   	f/8でカウント
 */
#define TCR_CKS         0x0

/*
 *   タイマに供給されるクロック周波数[kHz]
 *      60MHz/2 = 30MHz
 */
#define	TIMER_CLOCK		(25000)

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
 *  レジスタの設定値
 */
#define MCF_PIT_PCSR_EN   (0x0001u)
#define MCF_PIT_PCSR_RLD  (0x0002u)
#define MCF_PIT_PCSR_PIF  (0x0004u)
#define MCF_PIT_PCSR_PIE  (0x0008u)
#define MCF_PIT_PCSR_OVW  (0x0010u)
#define MCF_PIT_PCSR_DBG  (0x0020u)
#define MCF_PIT_PCSR_DOZE (0x0040u)

/*
 *  コントロールレジスタのアドレス
 */
#define MCF_PIT0_PCSR   (VH*)(IPSBAR + 0x00150000)
#define MCF_PIT0_PMR    (VH*)(IPSBAR + 0x00150002)
#define MCF_PIT0_PCNTR  (VH*)(IPSBAR + 0x00150004)


#ifndef _MACRO_ONLY

/*
 *  タイマスタート
 */
Inline void
mcf_timer_start ()
{
     sil_wrh_mem (MCF_PIT0_PCSR, sil_reh_mem (MCF_PIT0_PCSR) | MCF_PIT_PCSR_EN);
}

/*
 *  タイマ一時停止
 */
Inline void
mcf_timer_stop ()
{
    	/* タイマを停止 	  */
	sil_wrh_mem (MCF_PIT0_PCSR, sil_reh_mem (MCF_PIT0_PCSR) & ~MCF_PIT_PCSR_EN);
}
/*
 *  タイマ割込み要求のクリア
 *	TCRレジスタのIMFAビットは1回読み出した後に０を書き込む
 */
Inline void
mcf_timer_int_clear ()
{
	/* 割り込み要求をクリア     */
	sil_wrh_mem (MCF_PIT0_PCSR, sil_reh_mem (MCF_PIT0_PCSR));
}


/*
 *  タイマの起動処理
 *
 *  タイマを初期化し，周期的なタイマ割込み要求を発生させる．
 */
Inline void
mcf_timer_initialize ()
{
	CLOCK cyc = TO_CLOCK (TIC_NUME, TIC_DENO);

	/*
	 *  タイマ関連の設定
	 */
	mcf_timer_stop ();			/* タイマ停止 */
	/* タイマ上限値のチェック */
	assert (cyc <= MAX_CLOCK);
       /*
         *  タイマ割込みの割込みレベルを設定し，要求をクリアした後，
         *  マスクを解除する．
         */
        sil_wrh_mem (MCF_PIT0_PCSR, (VH)(MCF_PIT_PCSR_OVW | MCF_PIT_PCSR_PIF | MCF_PIT_PCSR_RLD));
        sil_wrh_mem (MCF_PIT0_PMR, (VH)CLOCK_PER_TICK);
        sil_wrh_mem (MCF_PIT0_PCNTR,0x0000);
        sil_wrh_mem (MCF_PIT0_PCSR, sil_reh_mem(MCF_PIT0_PCSR) | MCF_PIT_PCSR_PIE);	//割り込み許可
}

/*
 *  タイマの停止処理
 *
 *  タイマの動作を停止させる．
 */
Inline void
mcf_timer_terminate ()
{
	/*
	 *  タイマの動作を停止する．
	 */
	mcf_timer_stop();
        mcf_timer_int_clear ();
	/*
	 *  タイマ割込みをマスクし，要求をクリアする．
	 */
	sil_wrh_mem (MCF_PIT0_PCSR, sil_reh_mem(MCF_PIT0_PCSR) & ~MCF_PIT_PCSR_PIE);	//割り込み不許可
	sil_wrh_mem (MCF_PIT0_PCNTR,0x0000);
	sil_wrh_mem (MCF_PIT0_PMR,0x0000); 
        sil_wrw_mem (MCF_INTC_IMRH(MCF_INTC0), sil_rew_mem(MCF_INTC_IMRH(MCF_INTC0)) | (1 << (TBIT_TT0)));
}

/*
 *  タイマの現在値の読出し
 */
Inline CLOCK
mcf_timer_get_current ()
{
	CLOCK	clk;
        
	clk = TO_CLOCK(TIC_NUME, TIC_DENO) - sil_reh_mem(MCF_PIT0_PCNTR);
	return (clk);
}

/*
 *  タイマ割込み要求のチェック
 */
Inline BOOL
mcf_timer_fetch_interrupt ()
{
        return (sil_reh_mem (MCF_PIT0_PCSR) & MCF_PIT_PCSR_PIF);
}

#endif /* _MACRO_ONLY */
#endif /* _MCFPIT_H_ */
