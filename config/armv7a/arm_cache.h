/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2013 by Embedded and Real-Time Systems Laboratory
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
 */
#ifndef ARM_CACHE_H
#define ARM_CACHE_H

#ifndef _MACRO_ONLY

/*
 *  コプロを用いたルーチン
 */

/*
 *  Data Synchronization Barrier
 *  ・先に発行された read と write の終了を待つ
 *  ・キャッシュ，ブランチプリディクション，TLBの操作の終了を待つ
 *  ・この命令の後に書かれた命令は実行されない
 */
Inline void
data_sync_barrier(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 *  Data Memory Barrier
 *  ・プログラムの記述に従って，先に書かれた命令でのメモリアクセスが
 *     終了するまで待つ．
 *
 */
Inline void
data_memory_barrier(void)
{
	CP15_DATA_MEMORY_BARRIER();
}

/*
 *  プリフェッチバッファをクリア
 */
Inline void
pbuffer_flash(void)
{
	CP15_PBUFFER_FLUSH();
}


/*
 *  ブランチプリディクターの全無効化
 */
Inline void
bpi_invalidate_all(void)
{
	CP15_BPI_INVALIDATEALL(0);
	CP15_DATA_SYNC_BARRIER();
	CP15_PBUFFER_FLUSH();
}

/*
 *  ブランチプリディクターを有効に
 */
Inline void
btac_enable(void)
{
	UW	tmp;

	CP15_CONTROL_READ(tmp);
	tmp |= CP15_CONTROL_Z_BIT;
	CP15_CONTROL_WRITE(tmp);
}

extern void invalidate_dcache_all();
extern void clean_invalidate_dcache_all();
extern void clean_dcache_all();

/*
 *  Dキャッシュの無効化
 */
Inline void
dcache_invalidate(void)
{
	invalidate_dcache_all();
}

/*
 *  Dキャッシュのクリーンと無効化
 */
Inline void
dcache_clean_and_invalidate(void)
{
	clean_invalidate_dcache_all();
}

/*
 *  Dキャッシュのクリーン
 */
Inline void
dcache_clean(void)
{
	clean_dcache_all();
}

/*
 *  Iキャッシュの無効化
 */
Inline void
icache_invalidate(void)
{
	CP15_ICACHE_INVALIDATE();
	CP15_BRANCHP_INVALIDATE();
	CP15_DATA_SYNC_BARRIER();
	CP15_PBUFFER_FLUSH();
}

/*
 *  I/Dキャッシュを両方を有効に
 */
extern void cache_enable(void);

/*
 *  I/Dキャッシュを両方を無効に
 */
extern void cache_disable(void);

/*
 *  Iキャッシュを有効に
 */
extern void icache_enable(void);

/*
 *  Iキャッシュを無効に
 */
extern void icache_disable(void);

/*
 *  Dキャッシュを有効に
 */
extern void dcache_enable(void);

/*
 *  Dキャッシュを無効に
 */
extern void dcache_disable(void);

/*
 *  L2キャッシュのフラッシュ（各依存部で定義）
 */
extern void outer_cache_flush(void);

/*
 *  MMU関連
 */

/*
 *  TLBの無効化
 */
Inline void
invalidate_unfied_tlb(void)
{
	CP15_DATA_SYNC_BARRIER();
}

/*
 * 変換テーブルへの設定内容
 * va   : 仮想アドレス
 * pa   : 物理アドレス
 * size : サイズ
 * s    : 共有指定
 * tex  : C Bとの組み合わせで変化
 * ap   : アクセス権
 * c    : キャッシュ
 * b    : バッファ
 */
typedef struct{
	VP_INT	va;
	VP_INT	pa;
	UW	size;
	UB	ns;
	UB	s;
	UB	tex;
	UB	ap;
	UB	c;
	UB	b;
}MEMORY_ATTRIBUTE;

/*
 *  MMUの初期化
 */
extern void mmu_init(void);

/*
 *  MMUによるメモリのマッピング
 */
extern void mmu_map_memory(MEMORY_ATTRIBUTE *m_attribute);

#endif /* _MACRO_ONLY */
#endif /* ARM_CACHE_H */
