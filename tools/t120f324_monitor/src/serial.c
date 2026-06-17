/*
 *  rommon Simple Monitor
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2006 by GJ Business Division RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2016-2019 by TOPPERS PROJECT Educational Working Group.
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
 *  @(#) $Id: $
 */

/*
 *	rommon Serial Functions
 */
#include "comdefs.h"
#include <sil.h>
#include "serial.h"

#ifndef WDOG_RESET
#define WDOG_RESET()
#endif

/*
 *	Seiral Data
 */
#if !defined(NOINT_UARTRX)
char *SCIRXWritePtr[MAX_SERIAL];
char *SCIRXReadPtr[MAX_SERIAL];
char *SCIRXBuffBase[MAX_SERIAL];
char *SCIRXBuffLimit[MAX_SERIAL];
char SCIRXBuffer[MAX_SERIAL][DEF_SCIRX_BUFF_SIZE];
volatile unsigned char SCIRXBuffCount[MAX_SERIAL];
#endif	/* NOINT_UARTRX */

#if !defined(NOINT_UARTTX)
char *SCITXWritePtr[MAX_SERIAL];
char *SCITXReadPtr[MAX_SERIAL];
char *SCITXBuffBase[MAX_SERIAL];
char *SCITXBuffLimit[MAX_SERIAL];
char SCITXBuffer[MAX_SERIAL][DEF_SCITX_BUFF_SIZE];
volatile unsigned char SCITXBuffCount[MAX_SERIAL];
#endif /* NOINT_UARTTX */

volatile unsigned char SCIRXFlowCntrl[MAX_SERIAL];
volatile unsigned char SCITXFlowCntrl[MAX_SERIAL];
unsigned char SCIRXXOFF[MAX_SERIAL];

/*
 *	Initialize Serial Channel
 *
 *		Input:	er3: Serial Channel #
 */

void InitSerial(int id)
{
#if !defined(NOINT_UARTRX)
	SCIRXBuffCount[id] = 0;
	SCIRXBuffBase[id]  = &SCIRXBuffer[id][0];
	SCIRXReadPtr[id]   = &SCIRXBuffer[id][0];
	SCIRXWritePtr[id]  = &SCIRXBuffer[id][0];
	SCIRXBuffLimit[id] = &SCIRXBuffer[id+1][0];
#endif	/* NOINT_UARTRX */

#if !defined(NOINT_UARTTX)
	SCITXBuffCount[id] = 0;
	SCITXBuffBase[id]  = &SCITXBuffer[id][0];
	SCITXReadPtr[id]   = &SCITXBuffer[id][0];
	SCITXWritePtr[id]  = &SCITXBuffer[id][0];
	SCITXBuffLimit[id] = &SCITXBuffer[id+1][0];
#endif	/* NOINT_UARTTX */
	SCIRXFlowCntrl[id] = 0;
	SCIRXXOFF[id]      = 0;
	SCITXFlowCntrl[id] = 0;
	InitSIO(id);
}

#if !defined(NOINT_UARTRX)
/*
 *	SCI Common Intrrupt Handler: Recive Data Full
 *  Input: id: Serial Channel #
 */
void ComInterRX(int id)
{
	unsigned short val;
	char           c;

	val = GetRxData(id);
	if(CHKRXDATA & val){
		c = val & 0xff;
#ifndef USE_HARDFLOW
		if(c == ('S'-'@')){
			SCITXFlowCntrl[id] = c;
		}
		else if(c == ('Q'-'@')){
			SCITXFlowCntrl[id] = 0;
#if !defined(NOINT_UARTTX)
			EnableITX(id);
#endif	/* NOINT_UARTTX */
		}
		else{
#endif	/* !USE_HARDFLOW */
			*SCIRXWritePtr[id]++ = c;
			if(SCIRXWritePtr[id] >= SCIRXBuffLimit[id])
				SCIRXWritePtr[id] = SCIRXBuffBase[id];
			if(SCIRXBuffCount[id] < DEF_SCIRX_BUFF_SIZE)
				SCIRXBuffCount[id]++;
			if(SCIRXBuffCount[id] >= SCI_HIGH_WMARK){
				if(SCIRXFlowCntrl[id] == 0){
					SCIRXFlowCntrl[id] = 'S'-'@';
#ifdef USE_HARDFLOW
					SetRTS(id, 0);
#else
					SCIRXXOFF[id] = 'S'-'@';
#endif
#if !defined(NOINT_UARTTX)
					EnableITX(id);
#endif	/* NOINT_UARTTX */
				}
			}
#ifndef USE_HARDFLOW
		}
#endif	/* NOINT_UARTTX */
	}
}
#endif	/* NOINT_UARTRX */

#if !defined(NOINT_UARTTX)
/*
 *	SCI Common Intrrupt Handler: Transmit Data Empty
 *  Input:	id: Serial Channel #
 */
void ComInterTX(int id)
{
	char c;

	if((c = SCIRXXOFF[id]) != 0){
		SCIRXXOFF[id] = 0;
		SendTX(id, c);
	}
	else if(SCITXBuffCount[id] > 0){
		c = *SCITXReadPtr[id]++;
		if(SCITXReadPtr[id] >= SCITXBuffLimit[id])
			SCITXReadPtr[id] = SCITXBuffBase[id];
		SendTX(id, c);
		SCITXBuffCount[id]--;
	}
	else
		DisableITX(id);
}
#endif	/* NOINT_UARTTX */

#ifdef USE_HARDFLOW
void ComInterCTS(int id, int val)
{
	if(val == 0)
		SCITXFlowCntrl[id] = 1;
	else{
#if !defined(NOINT_UARTTX)
		if(SCITXFlowCntrl[id] != 0 && SCITXBuffCount[id] > 0)
			EnableITX(id);
#endif
		SCITXFlowCntrl[id] = 0;
	}
}
#endif	/* USE_HARDFLOW */

/*
 *	Send Character
 *  Input:  arg1: Serial Channel #
 *          arg2: Character sent
 */

void SendChar(int id, char c)
{
#if !defined(NOINT_UARTTX)
	if(id < 0)
		return;
	while(SCITXBuffCount[id] >= DEF_SCITX_BUFF_SIZE){}

	*SCITXWritePtr[id]++ = c;
	if(SCITXWritePtr[id] >= SCITXBuffLimit[id])
		SCITXWritePtr[id] = SCITXBuffBase[id];
	Disable();
	SCITXBuffCount[id]++;
	if(SCITXFlowCntrl[id] == 0){
		EnableITX(id);
	}
	Enable();
#else	/* NOINT_UARTTX */
	char c2;

	if(id < 0)
		return;
	while(SCITXFlowCntrl[id] != 0){
		sil_dly_nse(1000);
	}
	if((c2 = SCIRXXOFF[id]) != 0){
		SCIRXXOFF[id] = 0;
		SendTXWait(id);
		SendTX(id, c2);
	}
	SendTXWait(id);
	SendTX(id, c);
#endif	/* NOINT_UARTTX */
}

/*
 *	Recive Character
 *  Input:  arg1: Serial Channel #
 *  Output:	Character received, 0 if no data
 */
char ReciveChar(int id)
{
	char c = 0;
	WDOG_RESET();
#if !defined(NOINT_UARTRX)
	if(SCIRXBuffCount[id] > 0){
		Disable();
		SCIRXBuffCount[id]--;
		Enable();

		if(SCIRXBuffCount[id] <= SCI_LOW_WMARK && SCIRXFlowCntrl[id]){
			SCIRXFlowCntrl[id] = 0;
			Disable();
#ifdef USE_HARDFLOW
			SetRTS(id, 1);
#else
			SCIRXXOFF[id] = 'Q'-'@';
#endif
#ifndef NOINT_UARTTX
			EnableITX(id);
#endif	/* NOINT_UARTTX */
			Enable();
		}
		c = *SCIRXReadPtr[id]++;
		if(SCIRXReadPtr[id] >= SCIRXBuffLimit[id])
			SCIRXReadPtr[id] = SCIRXBuffBase[id];
	}
#else	/* NOINT_UARTRX */
	unsigned short tmp = GetRxData(id);
	if(tmp & CHKRXDATA)
		c = (unsigned char)tmp;
#endif	/* NOINT_UARTRX */
	return c;
}

