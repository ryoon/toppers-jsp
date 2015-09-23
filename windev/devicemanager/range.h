/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，以下の条件を満たす場合に限り，本ソフトウェア（本ソ
 *  フトウェアを改変したものを含む．以下同じ）を使用・複製・改変・再配
 *  布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアをバイナリコードの形または機器に組み込んだ形で利
 *      用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (3) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: range.h,v 1.1 2001/02/23 09:34:31 takayuki Exp $
 */

#ifndef __RANGE_H
#define __RANGE_H

class Range
{
protected:
	unsigned long Base;
	unsigned long Length;

public:
	Range(unsigned long b, unsigned long l) : Base(b), Length(l) {};
	Range(unsigned long b) : Base(b), Length(1) {};
	Range(const Range & src) : Base(src.Base), Length(src.Length) {};

	bool operator < (const Range & cmp) const
	{	return Base+(Length-1) < cmp.Base;	};

	bool IsInsideOf(const Range & cmp) const
	{	return (cmp.Base <= Base) && (cmp.Base + cmp.Length >= Base + Length); };

	bool IsIncludedIn(const Range &cmp) const
	{	return !(*this < cmp) && !(cmp < *this);	};

	unsigned long GetBase(void) const
	{	return Base;	};

	unsigned long GetLength(void) const
	{	return Length;	};

};

#endif
