/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 *
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2002 by Monami software, Limited Partners.
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
 *  @(#) $Id: i386.h,v 1.3 2002/04/06 12:23:01 honda Exp $
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
	unsigned	base_L:24 __attribute__ ((packed));
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
} SEGDESC;

/* Gate descriptor */
typedef struct __i386_gate_descriptor {
	unsigned	offset_L:16;
	unsigned	selector:16;
	unsigned	copy:8;
	unsigned	type:4;
	unsigned	s:1;
	unsigned	dpl:2;
	unsigned	p:1;
	unsigned	offset_H:16;
} GATEDESC;

typedef	struct {
	unsigned	limit:16;
	unsigned	base:32 __attribute__ ((packed));
} DESCPTR;

#endif	/* _I386_H_ */
