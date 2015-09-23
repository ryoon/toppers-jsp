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
 *  @(#) $Id: hal_msg.h,v 1.1 2000/11/14 16:31:38 takayuki Exp $
 */
#ifndef __HAL_MSG_H
#define __HAL_MSG_H

#include "jsp_stddef.h"
#include "vwindows.h"

#define HALMSG_MESSAGE WM_USER

#define HALMSG_DISPATCH		0x0001
#define HALMSG_DESTROY		0x0002
#define HALMSG_INTERRUPT	0x0003

#define HALMSG_EXECUTEPROCEDURE 0x0101

#define HALTIM_AUTOINT		0xffff0000

extern HANDLE PrimaryThreadHandle;

Inline void
HALDispatchRequest(void * tcb)
{
	SendMessage(PrimaryThreadHandle,HALMSG_MESSAGE,HALMSG_DISPATCH,(LPARAM)tcb);
}

Inline void
HALDestroyRequest(void * tcb)
{
	PostMessage(PrimaryThreadHandle,HALMSG_MESSAGE,HALMSG_DESTROY,(LPARAM)tcb);
	ExitThread(0);
}

Inline void
HALInterruptRequest(unsigned int intno)
{
	PostMessage(PrimaryThreadHandle,HALMSG_MESSAGE,HALMSG_INTERRUPT,(LPARAM)intno);
}

Inline void
HALInterruptRequestAndWait(unsigned int intno)
{
	SendMessage(PrimaryThreadHandle,HALMSG_MESSAGE,HALMSG_INTERRUPT,(LPARAM)intno);
}

Inline void
HALExecuteProcedure(void * func, void * param)
{
	void * _workofHALExecuteProcedure[2] = { func, param };
	SendMessage(PrimaryThreadHandle,HALMSG_MESSAGE,HALMSG_EXECUTEPROCEDURE,(LPARAM)_workofHALExecuteProcedure);
}

#endif