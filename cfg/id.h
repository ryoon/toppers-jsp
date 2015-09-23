/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000,2001 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の条件のいずれかを満たす場合に限り，本ソフトウェア（本ソフトウェ
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
 *  @(#) $Id: id.h,v 1.5 2001/11/12 14:59:27 takayuki Exp $
 */

//------------------------------------------------------------
// ID - class definition
//
//   class ID is a type for managing objects' identifier and
//		assigning suitable identifier automatically for new
//		object.

#ifndef __ID_H

#include <set>

class ID
{
protected:
	long LowerBound;	//IDs which equal upper or lower bound are available
	long UpperBound;	
	std::set<long> AssignedIdentifier;

public:
	ID(const long Size = 255);
	explicit ID(long Lower, unsigned long Size = 255);
	explicit ID(long Lower, long Upper = 255);

	operator const bool(void);				//Availability check
	operator const unsigned long(void);	//Count of items
	bool operator [](long);							//Status of each item

	bool Assign(long);
	bool Resign(long);
	
	bool IsAssigned(long);
	long GetMaxID(void);
	long GetMinID(void);
	unsigned long GetCount(void);

	long GetNewID(void);		//Method GetNewID reports a suitable new identifier, but never reserves one.

	bool IsAvailable(void);		//This method checks up on the availablity of these IDs with ITRON ID numbering policy.
};

inline ID::operator const bool (void)
{	return this->IsAvailable();	}

inline ID::operator const unsigned long(void)
{	return this->AssignedIdentifier.size();	}

inline bool ID::operator [] (long value)
{	return this->AssignedIdentifier.find(value) != this->AssignedIdentifier.end();	}

inline bool ID::IsAssigned (long value)
{	return this->AssignedIdentifier.find(value) != this->AssignedIdentifier.end();	}

inline unsigned long ID::GetCount(void)
{	return AssignedIdentifier.size();	}

#endif

//---
// Degined and Programmed by S.D.F. in Oct,2000