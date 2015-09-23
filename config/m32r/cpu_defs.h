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
 *  @(#) $Id: cpu_defs.h,v 1.2 2002/04/05 09:01:40 takayuki Exp $
 */

/*
 *	プロセッサに依存する定義（M32R用）
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

#define M32R

	/* 特に何も無いときにPSWの値 */
#define DEFAULT_PSW	0x0007c0c0	//ユーザスタック + 割込み許可 + 割込みマスク7

	/* 例外要因番号 */
#define NUM_EXCEPTION 20

#define EXC_RESET	0
#define EXC_SBI		1
#define EXC_RIE		2
#define EXC_AE		3
#define EXC_TRAP00	4
#define EXC_TRAP01	5
#define EXC_TRAP02	6
#define EXC_TRAP03	7
#define EXC_TRAP04	8
#define EXC_TRAP05	9
#define EXC_TRAP06	10
#define EXC_TRAP07	11
#define EXC_TRAP08	12
#define EXC_TRAP09	13
#define EXC_TRAP10	14
#define EXC_TRAP11	15
#define EXC_TRAP12	16
#define EXC_TRAP13	17
#define EXC_TRAP14	18
#define EXC_TRAP15	19

	/* 割込み要因番号 */
#define INT_INT0	1
#define INT_INT1	2
#define INT_INT2	3
#define INT_INT3	4
#define INT_INT4	5
#define INT_INT5	6
#define INT_INT6	7
#define INT_MFT0	16
#define INT_MFT1	17
#define INT_MFT2	18
#define INT_MFT3	19
#define INT_MFT4	20
#define INT_MFT5	21
#define INT_DMA0	32
#define INT_SIO0RCV	48
#define INT_SIO0XMT	49
#define INT_SIO1RCV	50
#define INT_SIO1XMT	51
#define INT_SIO2RCV	52
#define INT_SIO2XMT	53
#define INT_SIO3RCV	54
#define INT_SIO3XMT	55
#define INT_SIO4RCV	56
#define INT_SIO4XMT	57


#ifndef _MACRO_ONLY

typedef	UINT		INHNO;		/* 割込みハンドラ番号 */
typedef	UB			IPR;		/* 割込み優先度 */
typedef	UINT		EXCNO;		/* CPU例外ハンドラ番号 */

#endif /* _MACRO_ONLY */
#endif /* _CPU_DEFS_H_ */
