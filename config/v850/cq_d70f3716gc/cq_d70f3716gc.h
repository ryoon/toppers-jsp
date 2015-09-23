/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 */

#ifndef	_CQ_D70F3716GC_H_
#define	_CQ_D70F3716GC_H_
/*
 *	プロセッサに依存する定義（V850ES/JG2用）
 */

/*
 * 内蔵RAM
 */
#define	IRAM_TOP	0x03FF9000		/* 内蔵RAMの先頭アドレス */
#define IRAM_SIZE	0x6000			/* 内蔵RAMの大きさ 24Kbyte */
/*
 * 内蔵FlashROM
 */
#define	IROM_TOP	0x0000000		/* 内蔵FlashROMの先頭アドレス */
#define IROM_SIZE	0x40000			/* 内蔵FlashROMの大きさ 256Kbyte */


/* 
 * V850ES/JG2のIOレジスタ定義
 */
#define PRCMD		(0xFFFFF1FC)	/* Command Register */
#define VSWC		(0xFFFFF06E)	/* System Wait Control Register */
#define OCDM		(0xFFFFF9FC)	/* OnChip Debug Mode Register */
#define WDTM2		(0xFFFFF6D0)	/* Watchdog Debug Mode Register2 */

/*
 * クロック関連
 */
#define PCC			(0xFFFFF828)	/* Processor Clock Control Register */
#define PLLCTL		(0xFFFFF82C)	/* PLL コントロール・レジスタ */

/*
 * LED関連
 */
/* ポートCT */
#define PCT			(0xFFFFF00A)	/* PCT Register vector */
#define PMCT		(0xFFFFF02A)	/* PCT Mode Register vector */
#define PMCCT		(0xFFFFF04A)	/* PCT Mode Control Register vector */

#define LED_BIT		(0x40)			/* LED */

/*
 * 16bit Interval Timer M
 */
#define TM0CTL0		(0xFFFFF690)
#define TM0CMP0		(0xFFFFF694)

/*
 * UART0関連
 */
/* ポート */
#define PMC3		(0xFFFFF446)
#define PFC3		(0xFFFFF466)
#define PFCE3L		(0xFFFFF706)

/* UART0 */
#define UA0CTL0		(0xFFFFFA00)	/* UARTA0 制御レジスタ0 */
#define UA0OPT0		(0xFFFFFA03)	/* UARTA0 オプション制御レジスタ0 */
#define UA0STR		(0xFFFFFA04)	/* UARTA0 状態レジスタ */
#define UA0RX		(0xFFFFFA06)	/* UARTA0 受信データレジスタ */
#define UA0TX		(0xFFFFFA07)	/* UARTA0 送信データレジスタ */

/* ボーレート */
#define UA0CTL1		(0xFFFFFA01)	/* UARTA0 制御レジスタ1 */
#define UA0CTL2		(0xFFFFFA02)	/* UARTA0 制御レジスタ2 */

/*
 * IntC
 */
#define TM0EQIC0	(0xFFFFF150)	/* TM0EQ0 割込み制御レジスタ */
#define UA0RIC		(0xFFFFF162)	/* UART0 受信完了割込み制御レジスタ */
#define UA0TIC		(0xFFFFF164)	/* UART0 送信許可割込み制御レジスタ */
#define IMR2		(0xFFFFF104)	/* 割込みマスクレジスタ2 */
#define IMR2H		(0xFFFFF105)	/* 割込みマスクレジスタ2H */
#define IMR2L		(0xFFFFF104)	/* 割込みマスクレジスタ2L */
#define ISPR		(0xFFFFF1FA)	/* インサービス・プライオリティ・レジスタ */

#endif	/* _CQ_D70F3716GC_H_ */
