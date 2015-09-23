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
 *  @(#) $Id: cpu_defs.h,v 1.1 2004/07/21 02:49:36 monaka Exp $
 */

/*
 *	プロセッサに依存する定義（i386用）
 */

#ifndef _CPU_DEFS_H_
#define _CPU_DEFS_H_

#define IA32

#ifndef _MACRO_ONLY

typedef	UINT		EXCNO;		/* CPU例外ハンドラ番号 */

#define SIL_ENDIAN SIL_ENDIAN_LITTLE

#define SIL_REV_ENDIAN_H(data) ((VH)((((UH)(data) & 0xff) << 8) | (((UH)(data) >> 8) & 0xff)))

#define SIL_REV_ENDIAN_W(data) ((VW)((((UW)(data) & 0xff) << 24) | (((UW)(data) & 0xff00) << 8) | (((UW)(data) >> 8) & 0xff00) | (((UW)(data) >> 24) & 0xff)))

Inline VB sil_reb_iop(VP iop) {
	VB data;
	Asm("inb %%dx, %%al" : "=a"(data) : "d"(iop));
	return data;
}

Inline VH sil_reh_iop(VP iop) {
	VH data;
	Asm("inw %%dx, %%ax" : "=a"(data) : "d"(iop));
	return data;
}

Inline VW sil_rew_iop(VP iop) {
	VW data;
	Asm("inl %%dx, %%eax" : "=a"(data) : "d"(iop));
	return data;
}

#define sil_reh_lep(iop) sil_reh_iop(iop)
#define sil_rew_lep(iop) sil_rew_iop(iop)

Inline VH sil_reh_bep(VP iop) {
	return SIL_REV_ENDIAN_H(sil_reh_iop(iop));
}

Inline VW sil_rew_bep(VP iop) {
	return SIL_REV_ENDIAN_W(sil_rew_iop(iop));
}

Inline void sil_wrb_iop(VP iop, VB data) {
	Asm("outb %%al, %%dx" : : "a"(data), "d"(iop));
}

Inline void sil_wrh_iop(VP iop, VH data) {
	Asm("outw %%ax, %%dx" : : "a"(data), "d"(iop));
}

Inline void sil_wrw_iop(VP iop, VW data) {
	Asm("outl %%eax, %%dx" : : "a"(data), "d"(iop));
}

#define sil_wrh_lep(iop, data) sil_wrh_iop(iop, data)
#define sil_wrw_lep(iop, data) sil_wrw_iop(iop, data)

Inline void sil_wrh_bep(VP iop, VH data) {
	sil_wrh_iop(iop, SIL_REV_ENDIAN_H(data));
}

Inline void sil_wrw_bep(VP iop, VW data) {
	sil_wrw_iop(iop, SIL_REV_ENDIAN_W(data));
}

#endif /* _MACRO_ONLY */
#endif /* _CPU_DEFS_H_ */
