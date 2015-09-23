/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
 *  Copyright (C) 2008-     by Monami Software Limited Partnership, JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
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
 *  @(#) $Id: i386.h,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

#ifndef	_I386_H_
#define	_I386_H_

/* Segment type */
#define	I386_TYPE_SEG_DATA       (0x00) /* 0000	データセグメント */
#define	I386_TYPE_SEG_EXPAND     (0x04) /* 0100 エクスパンドダウン */
#define	I386_TYPE_SEG_WRITEABLE  (0x02) /* 0010 書き込み可 */
#define	I386_TYPE_SEG_ACCESSED   (0x01) /* 0001 アクセスされたか？ */
#define	I386_TYPE_SEG_CODE       (0x08) /* 1000 コードセグメント */

#define	I386_TYPE_SEG_CONFORMING (0x04) /* 0100 コンフォーミング */
#define	I386_TYPE_SEG_READABLE   (0x02) /* 0010 読み取り可 */
#define	I386_TYPE_SEG_ACCESSED   (0x01) /* 0001 アクセスされたか？ */

/* Gate type */
#define	I386_TYPE_GATE_INTR		0xe
#define	I386_TYPE_GATE_TRAP		0xf

/* Segment descriptor */
typedef struct __i386_segment_descriptor {
	unsigned	limit_L:16;
	unsigned	base_L:24;
	unsigned	type:4;
	unsigned	s:1;
	unsigned	dpl:2;
	unsigned	p:1;
	unsigned	limit_H:4;
	unsigned	avl:1;
	unsigned	x:1;
	unsigned	db:1;
	unsigned	g:1;
	unsigned	base_H:8;
}   __attribute__ ((packed)) SEGDESC;

/* Gate descriptor */
typedef struct __i386_gate_descriptor {
	unsigned long offset_L:16;
	unsigned long selector:16;
	unsigned long copy:8;
	unsigned long type:4;
	unsigned long s:1;
	unsigned long dpl:2;
	unsigned long p:1;
	unsigned long offset_H:16;
}   __attribute__ ((packed)) GATEDESC;

typedef	struct {
	unsigned	limit:16;
	unsigned	base:32;
}  __attribute__ ((packed)) DESCPTR;

#endif	/* _I386_H_ */
