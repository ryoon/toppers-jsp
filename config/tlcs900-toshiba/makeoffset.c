/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2006-2010 by Witz Corporation, JAPAN
 * 
 *  The above copyright holders grant permission gratis to use,
 *  duplicate, modify, or redistribute (hereafter called use) this
 *  software (including the one made by modifying this software),
 *  provided that the following four conditions (1) through (4) are
 *  satisfied.
 * 
 *  (1) When this software is used in the form of source code, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be retained in the source code without modification.
 * 
 *  (2) When this software is redistributed in the forms usable for the
 *      development of other software, such as in library form, the above
 *      copyright notice, this use conditions, and the disclaimer shown
 *      below must be shown without modification in the document provided
 *      with the redistributed software, such as the user manual.
 * 
 *  (3) When this software is redistributed in the forms unusable for the
 *      development of other software, such as the case when the software
 *      is embedded in a piece of equipment, either of the following two
 *      conditions must be satisfied:
 * 
 *    (a) The above copyright notice, this use conditions, and the
 *           disclaimer shown below must be shown without modification in
 *        the document provided with the redistributed software, such as
 *        the user manual.
 * 
 *    (b) How the software is to be redistributed must be reported to the
 *        TOPPERS Project according to the procedure described
 *        separately.
 * 
 *  (4) The above copyright holders and the TOPPERS Project are exempt
 *      from responsibility for any type of damage directly or indirectly
 *      caused from the use of this software and are indemnified by any
 *      users or end users of this software from any and all causes of
 *      action whatsoever.
 * 
 *  THIS SOFTWARE IS PROVIDED "AS IS." THE ABOVE COPYRIGHT HOLDERS AND
 *  THE TOPPERS PROJECT DISCLAIM ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, ITS APPLICABILITY TO A PARTICULAR
 *  PURPOSE. IN NO EVENT SHALL THE ABOVE COPYRIGHT HOLDERS AND THE
 *  TOPPERS PROJECT BE LIABLE FOR ANY TYPE OF DAMAGE DIRECTLY OR
 *  INDIRECTLY CAUSED FROM THE USE OF THIS SOFTWARE.
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改変・
 *  再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作権
 *      表示，この利用条件および下記の無保証規定が，そのままの形でソース
 *      コード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使用
 *      できる形で再配布する場合には，再配布に伴うドキュメント（利用者マ
 *      ニュアルなど）に，上記の著作権表示，この利用条件および下記の無保
 *      証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使用
 *      できない形で再配布する場合には，次のいずれかの条件を満たすこと．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに報
 *        告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損害
 *      からも，上記著作権者およびTOPPERSプロジェクトを免責すること．また，
 *      本ソフトウェアのユーザまたはエンドユーザからのいかなる理由に基づ
 *      く請求からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者およ
 *  びTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的に対す
 *  る適合性も含めて，いかなる保証も行わない．また，本ソフトウェアの利用
 *  により直接的または間接的に生じたいかなる損害に関しても，その責任を負
 *  わない．
 * 
 *  @(#) $Id: makeoffset.c,v 1.1 2006/04/10 08:19:25 honda Exp $
 */


/*
 *	offset.inc (もしくは offset.h) について
 *
 *	  offset.inc は構造体の特定の要素が構造体の先頭から何バイト目に
 *	存在するかということや, 特定のビットフィールドが先頭から
 *	何バイト先の何ビット目に存在するか, という情報を
 *	アセンブリ言語ファイルに与えるために存在している.
 *
 *	  offset.inc ファイルの中身は, 単なるシンボル定義ファイルである.
 *	シンボルといっても定義するシンボルの名称にはパターンがあり,
 *	大きく分けて次の3種類に分類できる.
 *
 *	<構造体名>_<要素名>	(例) TCB_sp, TCB_pc, TCB_enatex など
 *	<構造体名>_<要素名>_bit	(例) TCB_enatex_bit など
 *	<構造体名>_<要素名>_mask (例) TCB_enatex_mask など
 *
 *	(例)
 *		TCB_texptn		equ		14
 *		TCB_sp			equ		20
 *		TCB_pc			equ		24
 *		TCB_enatex		equ		12
 *		TCB_enatex_bit	equ		14
 *		TCB_enatex_mask	equ		0x4000
 *
 *	  一番はじめは特定の要素が構造体の先頭から何バイト先にあるか
 *	ということを示す数値につけるラベルの名称.
 *	  二番目は特定のビットフィールド中のビットが, 下位から数えて
 *	何ビット目にあるかということを示す数値につけるラベルの名称. 
 *	0から始まる数値で表現する. 最も下位のビットは第0ビットである.
 *	  三番目は先ほどの <構造体名>_<要素名>_bit とも関連するが,
 *	特定のビットフィールド中のビットのマスク値の名称.
 *	*_bit と *_mask の値の間には次の関係がある.
 *		(*_mask) == (1 << *_bit)
 *	(例) TCB_enatex_bit が 14 の時, TCB_enatex_mask は 0x4000
 *
 *	  ちなみにTLCS-900L1依存部の実装では, 特定ビットのチェックなどに
 *	ビット命令アドレッシングを使用しているので *_mask というラベルは
 *	使用していない.
 */

#include "jsp_kernel.h"
#include "task.h"


void makeoffset(void);

void
makeoffset()
{
	Asm("; BEGIN TCB_texptn\n");
	(((TCB *)0)->texptn) |= (TEXPTN)1;
	Asm("; END");

	Asm("; BEGIN TCB_sp\n");
	(((TCB *)0)->tskctxb.xsp) = (VP)1;
	Asm("; END");

	Asm("; BEGIN TCB_pc\n");
	(((TCB *)0)->tskctxb.pc)();
	Asm("; END");

	Asm("; BEGIN TCB_enatex\n");
	(((TCB *)0)->enatex) = 1;
	Asm("; END");
}

