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
 *  @(#) $Id: sys_defs.h 2246 2025-11-20 17:59:05Z roi $
 */

#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

/*
 *  プロセッサタイプ
 */
#define RP2350A

/*
 *  ターゲットを識別するためのマクロの定義
 */
#define TOPPERS_RASPBERRYPI_PICO2				/* システム略称 */

/*
 *  起動メッセージのターゲットボード名
 */
#define	TARGET_NAME	"CORTEX-M33 - RASPBERRYPI_PICO2"

/* カーネル起動時のメッセージ   */
#define COPYRIGHT_SYS \
"Copyright (C) 2017-2025 by TOPPERS PROJECT Educational Working Group.\n"

/*
 * プロセッサ数
 */
#ifndef TNUM_PRCID
#define TNUM_PRCID	1
#endif /* TNUM_PRCID */

/*
 *  スタック定義
 */
#define STACKTOP	0x20082000U	/* 非タスクコンテキスト用のスタックの初期値 */
#define PSTACKSIZE	4096U
#define HEAP_TOP	(STACKTOP - (PSTACKSIZE*TNUM_PRCID))

/*
 *  タイムティックの定義
 */
#define	TIC_NUME	1u		/* タイムティックの周期の分子 */
#define	TIC_DENO	1u		/* タイムティックの周期の分母 */

/*
 *  プロセッサのエンディアン
 */
#define	SIL_ENDIAN	SIL_ENDIAN_LITTLE	/* リトルエンディアン */

/*
 *  RP2350ID定義
 */
#define BINARY_INFO_MARKER_START   0x7188ebf2
#define BINARY_INFO_MARKER_END     0xe71aa390
#define PICOBIN_BLOCK_MARKER_START 0xffffded3
#define PICOBIN_BLOCK_MARKER_END   0xab123579
#define PICOBIN_BLOCK_ITEM_1BS_ENTRY_POINT 0x44
#define PICOBIN_BLOCK_ITEM_2BS_IGNORED   0xfe
#define PICOBIN_BLOCK_ITEM_2BS_LAST      0xff

/*
 *  バイナリ情報用のID定義
 */
#define BINARY_INFO_TYPE_ID_AND_INT                 5
#define BINARY_INFO_TYPE_ID_AND_STRING              6

#define BINARY_INFO_ID_RP_PROGRAM_NAME              0x02031c86
#define BINARY_INFO_ID_RP_PROGRAM_VERSION_STRING    0x11a9bc3a
#define BINARY_INFO_ID_RP_PROGRAM_BUILD_DATE_STRING 0x9da22254

#define BINARY_INFO_ID_RP_BINARY_END                0x68f465de

/*
 *  コア判定アドレス
 */
#define TADR_SIO_CPUID      0xD0000000


#ifndef _MACRO_ONLY

/*
 *  INTNO型と割込みの禁止/許可
 */

typedef	INT	INTNO;			/* 割込み番号（cpu_configで利用） */

typedef void (*func_t)(void);

/*
 *  ロックの型(マルチプロセッサのみ)
 */
typedef	UINT	LOCK;		/* ロックの型 */

/*
 *  バイナリ情報用の型定義
 */
typedef struct __attribute__((packed)) _data_info {
	UH type;
	UH tag;
	UW id;
	UW value;
} data_info_t;

typedef struct __attribute__((packed)) string_info_t {
	UH type;
	UH tag;
	UW id;
	const char * value;
} string_info_t;

/*
 *  ターゲットシステム依存のサービスコール
 */

typedef	UD 	SYSUTIM;		/* 性能評価用システム時刻 */
extern	ER	vxget_tim(SYSUTIM *pk_sysutim) throw();

/*
 *  システムの停止処理
 */
Inline void
kernel_abort()
{
	while(1);
}

#endif /* _MACRO_ONLY */

#endif /* _SYS_DEFS_H_ */
