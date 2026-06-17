/*
 *  rommon Simple Monitor
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2006-2015 by GJ Business Division
 *                   RICOH COMPANY,LTD. JAPAN
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

#ifndef _COMDEFS_H_
#define _COMDEFS_H_
#include <sys_defs.h>

/*
 *	comdefs.h -- rom Monitor Common Definitions
 */

/*	Prompt		*/

#define ERROR_PROMPT		'?'

/*	Command Mode	*/

#define CMDMODE_ADDR		'a'
#define CMDMODE_ENTER		'e'
#define CMDMODE_DUMP		'd'

/*	Data Size	*/

#define DATA_SIZE_BYTE		1
#define DATA_SIZE_WORD		2
#define DATA_SIZE_LONG		4

/*	Command		*/

#define COMMAND_END		'.'
#define CMDSTR_HELP		('?' << 8)	/* help */

/*	Error Code	*/

#define ERR_NO_COMMAND		0x01

#ifndef _MACRO_ONLY
/*
 *  Type Definitions
 */
#ifdef _int8_
typedef	_int8_				VB;		/* 型が定まらない8ビットの値 */
#endif /* _int8_ */
#ifdef _int16_
typedef	unsigned _int16_ 	UH;		/* 符号無し16ビット整数 */
typedef	_int16_				VH;		/* 型が定まらない16ビットの値 */
#endif /* _int16_ */
typedef	unsigned _int32_	UW;		/* 符号無し32ビット整数 */
typedef	_int32_				VW;		/* 型が定まらない32ビットの値 */
typedef	void				*VP;	/* 型が定まらないものへのポインタ */
typedef unsigned int		UINT;	/* 自然なサイズの符号無し整数 */


/*
 *	External Functions
 */

#if defined(NOT_DEFVECTORTYPE) || defined(USE_EXECDISABLE)
extern void execDisable(void);
#else
#define execDisable()
#endif
extern void SetVector(void *v);

extern unsigned char Byte2HexASCII(unsigned char c);
extern unsigned long HexASCII2Long(char **ps, int size);
extern void SendCharCount(char c, int count);
extern void SkipBlanks(char **ps);
extern char *ReciveLine(int sid, int rid);
extern void InitRecLine(int id);

#endif	/* _MACRO_ONLY */
#endif	/* of #ifndef _COMDEFS_H_ */
