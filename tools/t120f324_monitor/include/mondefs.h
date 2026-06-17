/*
 *  rommon Simple Monitor
 * 
 *  Copyright (C) 1999-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2006-2015 by GJ Business Division
 *                   RICOH COMPANY,LTD. JAPAN
 *  Copyright (C) 2016-2018 by TOPPERS PROJECT Educational Working Group.
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

#ifndef _MONDEFS_H_
#define _MONDEFS_H_

/*
 *	mondefs.h -- rom Monitor Core Definitions
 */

/*	Prompt		*/

#define MONITOR_PROMPT		':'

/*	Command		*/

#define MONCMD_ENTER_LONG   (('e' << 8) | 'l')
#define MONCMD_ENTER_WORD   (('e' << 8) | 'w')
#define MONCMD_ENTER_BYTE   (('e' << 8) | 'b')
#define MONCMD_DUMP_LONG    (('d' << 8) | 'l')
#define MONCMD_DUMP_WORD    (('d' << 8) | 'w')
#define MONCMD_DUMP_BYTE    (('d' << 8) | 'b')
#define MONCMD_FILL_BYTE    (('f' << 8) | 'b')
#define MONCMD_LOAD         (('l' << 8) | 'd')
#define MONCMD_GO           (('g' << 8) | 'o')
#define MONCMD_FL           (('f' << 8) | 'l')
#define MONCMD_LS           (('l' << 8) | 's')

/*	Error Code	*/

#define ERR_CHECK_SUM		0xf1

#ifndef _MACRO_ONLY
/*
 *	External Data
 */
extern const unsigned short CommandString[];
extern const char HelpStrings[];
#ifndef NOT_DEFVECTORTYPE
extern unsigned long MonitorVector[];
extern unsigned long UserVector[];
#endif

extern char  *AddressPointer[];
extern char  *DumpPointer[];
extern unsigned char CommandMode[];
extern unsigned char Status[];

/*
 *	External Functions
 */
extern void SendPrompt(int id);
extern void BeforeInit(void);
extern void AfterInit(void);
extern int  VolumeInit(void);

extern void EnterByteCmd(int id, char **ps);
extern void EnterWordCmd(int id, char **ps);
extern void EnterLongCmd(int id, char **ps);
extern void DumpByteCmd(int id, char **ps);
extern void DumpWordCmd(int id, char **ps);
extern void DumpLongCmd(int id, char **ps);
extern void FillByteCmd(int id, char **ps);
extern void LoadCommand(int id, char **v);
extern void GoCommand(int id, char **ps);
extern void HelpCmd(int id, char **ps);
extern void ParseCmdCore(int id);
extern void MonSendPrompt(int id);

extern unsigned long load_file(int id, unsigned long addr, unsigned long dsize, char *path);
extern void disp_dir(int id, char *path);

#endif	/* _MACRO_ONLY */
#endif	/* of #ifndef _MONDEFS_H_ */
