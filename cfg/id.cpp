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
 *  @(#) $Id: id.cpp,v 1.6 2001/11/12 14:59:27 takayuki Exp $
 */


//------------------------------------------------------------
// ID - class implementation
//
//   class ID is a type for managing objects' identifier and
//		assigning suitable identifier automatically for new
//		object.

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include "id.h"
#include <algorithm>

using namespace std;

ID::ID(const long Size)
{
	LowerBound = 1;
	UpperBound = Size -1 + LowerBound;
	AssignedIdentifier.clear();
}

ID::ID(long Lower, unsigned long Size)
{
	LowerBound = Lower;
	UpperBound = Size -1 + LowerBound;
	AssignedIdentifier.clear();
}

ID::ID(long Lower, long Upper)
{
	if(Lower > Upper)
	{
		Lower ^= Upper;
		Upper ^= Lower;
		Lower ^= Upper;
	}
	LowerBound = Lower;
	UpperBound = Upper;
	AssignedIdentifier.clear();
}

bool ID::Assign(long value)
{
	if(value < LowerBound || value > UpperBound)
		return false;

	return (AssignedIdentifier.insert(value)).second;
}

	//METHOD Resign
	// value : The value of ID you want remove
	// Result: Execution status (True means success)
bool ID::Resign(long value)
{
	if(value < LowerBound || value > UpperBound)
		return false;

	AssignedIdentifier.erase(value);
	return true;
}

	//METHOD GetMinID
	// Result: Maximum value of ID existed
long ID::GetMaxID(void)
{
	set<long>::iterator maxvalue;

	maxvalue = max_element(AssignedIdentifier.begin(),AssignedIdentifier.end());
	if(maxvalue == AssignedIdentifier.end())
		return LowerBound -1;

	return *maxvalue;
}

	//METHOD GetMinID
	// Result: Minimum value of ID existed 
long ID::GetMinID(void)
{
	set<long>::iterator minvalue;

	minvalue = min_element(AssignedIdentifier.begin(),AssignedIdentifier.end());
	if(minvalue == AssignedIdentifier.end())
		return UpperBound + 1;

	return *minvalue;
}

	//METHOD GetNewID
	// Result: Suitable New ID
long ID::GetNewID(void)
{
	long prev;
	set<long>::iterator scope;

	if(AssignedIdentifier.size()==0)
		return LowerBound;

	scope = AssignedIdentifier.end();
	scope --;
	if(*scope-LowerBound+1 == (signed)AssignedIdentifier.size())
	{
		if(*scope+1>UpperBound)
			return LowerBound -1;
		else
			return *scope+1;
	}

	scope = AssignedIdentifier.begin();
	if(*scope != LowerBound)
		return LowerBound;
	
	prev = *scope;
	scope++;
	while(scope != AssignedIdentifier.end() )
	{
		if(prev+1<*scope)
			return prev+1;
		prev = *scope;
		scope++;
	}

	return LowerBound-1;
}

	//METHOD IsAvailable
	//  Result: Status whether this array is available or not
bool ID::IsAvailable(void)
{
	long prev;
	set<long>::iterator work;

		//Empty check (Empty ID set is not bad, so result should be true.)
	if(AssignedIdentifier.size() == 0)
		return true;

		//Smallest ID should be equal to lower bound
	work = AssignedIdentifier.begin();
	if(*work != LowerBound)
		return false;

		//The array of IDs must not include any cracks
	prev = *work;
	work ++;
	while(work != AssignedIdentifier.end() )
	{
		if(*work != prev +1)
			return false;
		
		prev = *work;
		work++;
	}

	return true;
}


//---
// Degined and Programmed by S.D.F. in Oct,2000
