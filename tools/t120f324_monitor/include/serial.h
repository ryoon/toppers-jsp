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

#ifndef _SERIAL_H_
#define _SERIAL_H_

/*
 *	serial.h -- H8 Serial Definitions
 */

/*	Default Values	*/

#define DEF_SCITX_BUFF_SIZE	80				/* default SCI Transmit buffer size */
#define DEF_SCIRX_BUFF_SIZE	80				/* default SCI Recive   buffer size */
#define DEFAULT_LINE_SIZE	(DEF_SCIRX_BUFF_SIZE - 1)	/* default line size (without terminator) */

/*	Parameters	*/

#define SCI_LOW_WMARK		(DEF_SCIRX_BUFF_SIZE / 3)
#define SCI_HIGH_WMARK		(SCI_LOW_WMARK * 2)
#define CHKRXDATA           0xff00

/*  Logical Serial id */

#define	DUMMY_MON_SERIAL	-1

#ifndef _MACRO_ONLY
/*
 *	External Functions
 */

extern void InitSerial(int id);
extern void SendChar(int id, char c);
extern char ReciveChar(int id);
extern void ComInterRX(int id);
extern void ComInterTX(int id);
extern void ComInterCTS(int id, int val);

extern void InitSIO(int id);
extern unsigned short GetRxData(int id);
extern void EnableITX(int id);
extern void DisableITX(int id);
extern void SendTXWait(int id);
extern void SendTX(int id, char c);
extern void SetRTS(int id, int val);

#endif	/* _MACRO_ONLY */

#endif	/* of #ifndef _SERIAL_H_ */
