/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: sys_defs.h,v 1.2 2002/04/05 09:01:29 takayuki Exp $
 */

/*
 *	ターゲットシステムに依存する定義
 *    M3A-2131G50 M32102 66MHz
 */

#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

#define M3A2131G50

/*
 * メモリマップドレジスタ
 */

#define LED 0xef1027

	/* 割込み関連のレジスタ */
#define ICUISTS		0xeff004
#define ICUIREQ0	0xeff008
#define ICUIREQ1	0xeff00c
#define ICUSBICR	0xeff018
#define ICUIMASK	0xeff01c

#define __ICUCR(x)	ICUCR_##x
#define _ICUCR(x)	__ICUCR(x)
#define ICUCR_INT	0xeff200
#define ICUCR_MFT	0xeff23c
#define ICUCR_SIO	0xeff2bc
#define ICUCR(x,y)	(_ICUCR(x) + (0x4*y))


	/* タイマ関連レジスタ */
#define MFTCR			0xefc000
#define MFTRPR			0xefc004
#define MFTMOD(x)		(0xefc100 + (x * 0x100))
#define MFTBOS(x)		(0xefc104 + (x * 0x100))
#define MFTCUT(x)		(0xefc108 + (x * 0x100))
#define MFTRLD(x)		(0xefc10c + (x * 0x100))
#define MFTMCMPRLD(x)	(0xefc110 + (x * 0x100))


	/* シリアル関連レジスタ */
#define SIOCR(x)	(0xefd000 + (x * 0x100))
#define SIOMOD0(x)	(0xefd004 + (x * 0x100))
#define SIOMOD1(x)	(0xefd008 + (x * 0x100))
#define SIOSTS(x)	(0xefd00c + (x * 0x100))
#define SIOTRCR(x)	(0xefd010 + (x * 0x100))
#define SIOBAUR(x)	(0xefd014 + (x * 0x100))
#define SIORBAUR(x)	(0xefd018 + (x * 0x100))
#define SIOTXB(x)	(0xefd01c + (x * 0x100))
#define SIORXB(x)	(0xefd020 + (x * 0x100))


	/* プログラマブルI/Oポート関連レジスタ */
#define PIEN		0xef1000
#define PDATA(x)	(0xef1020+x)
#define PDIR(x)		(0xef1040+x)
#define PMOD(x)		(0xef1060+(x*2))
#define PODCR(x)	(0xef1080+(x*2))


	/* 外部バスコントローラ */
#define BSELCR(x)	(0xef5000 + (x*4))

	/* SDRAMコントローラ */
#define SDRF0		0xef6000
#define SDRF1		0xef6004
#define SDIR0		0xef6008
#define SDIR1		0xef600c
#define SDBR		(0xef6010)
#define SDADR(x)	(0xef6020 + (x*32))
#define SDER(x)		(0xef6024 + (x*32))
#define SDTR(x)		(0xef6028 + (x*32))
#define SDMOD(x)	(0xef602c + (x*32))


	/* CPU動作モード関連のレジスタ */
#define CPUCLKCR	0xef4000
#define CLKMOD		0xef4004
#define PLLCR		0xef4008

/*
 *  INTNO型と割込みの禁止/許可
 */
#ifndef _MACRO_ONLY

typedef	unsigned char INTNO;		/* 割込み番号 */

#endif /* _MACRO_ONLY */

/*
 *  ターゲットシステム依存のサービスコール
 */
#ifndef _MACRO_ONLY

typedef	unsigned long SYSUTIM;	/* 性能評価用システム時刻 */

//extern ER	vxget_tim(SYSUTIM *pk_sysutim);

#endif /* _MACRO_ONLY */

/*
 *  タイムティックの定義
 */
#define	TIC_NUME	1		/* タイムティックの周期の分子 */
#define	TIC_DENO	1		/* タイムティックの周期の分母 */

#endif /* _SYS_DEFS_H_ */
