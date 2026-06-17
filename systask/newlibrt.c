/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Takagi Nobuhisa
 * 
 *  上記著作権者は，以下の (1)～(4) の条件か，Free Software Foundation 
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
 *  @(#) $Id: newlibrt.c,v 1.2 2004/09/17 09:11:44 honda Exp $
 */
#include <stdio.h>
#include <sys/stat.h>
#include <stddef.h>
#include <reent.h>
#include <errno.h>
#include "../kernel/jsp_kernel.h"
#include "serial.h"

#ifndef	HEAP_TOP
#define	HEAP_TOP		~0UL
#endif

/*
 *  システムログ用スピンロック設定
 */
Inline void
malloc_spn_lock(void)
{
#ifdef KERNEL_TOOL_SPNLOCK
	LOCK tool_lock = (LOCK)KERNEL_TOOL_SPNLOCK;
	x_acquire_lock(&tool_lock);
#endif /* KERNEL_TOOL_SPNLOCK */
}

/*
 *  システムログ用スピンロック解除
 */
Inline void
malloc_spn_unlock(void)
{
#ifdef KERNEL_TOOL_SPNLOCK
	LOCK tool_lock = (LOCK)KERNEL_TOOL_SPNLOCK;
	x_release_lock(&tool_lock);
#endif /* KERNEL_TOOL_SPNLOCK */
}

/*
 *	For malloc (Newlib)
 */
static int cxxrt_dis_dsp;

void __malloc_lock(struct _reent *ptr)
{
	if (!iniflg || sns_dsp())
		return;
	dis_dsp();
	malloc_spn_lock();
	cxxrt_dis_dsp = 1;
}

void __malloc_unlock(struct _reent *ptr)
{
	if (!cxxrt_dis_dsp)
		return;
	cxxrt_dis_dsp = 0;
	malloc_spn_unlock();
	ena_dsp();
}

void *_sbrk_r(struct _reent *ptr, ptrdiff_t nbyte)
{
	extern char end;
	static char *heap_ptr = &end;
	static char *heap_end = (char*)HEAP_TOP;
	char *base;

	if (!iniflg)
	{
		volatile char _auto;
		if (&end < &_auto && &_auto < heap_end)
			heap_end = (char*)&_auto;
	}

	if (heap_ptr + nbyte + 32 > heap_end)
	{
		ptr->_errno = errno = ENOMEM;
		return (void*)-1;
	}

	base = heap_ptr;
	heap_ptr += nbyte;
	return base;
}

/*
 *  For stdio
 */
#ifndef	_SMALL_STDIO_

#define STDIO_STDIN_FD	0
#define STDIO_STDOUT_FD	1
#define STDIO_STDERR_FD	2

Weak int _read(int handle, char *buffer, int length)
{
    if (handle == STDIO_STDIN_FD) {
        length = serial_rea_dat(LOGTASK_PORTID, buffer, length);
    }
	return length;
}

Weak int _write(int handle, char *buffer, int length)
{
    if (handle == STDIO_STDOUT_FD || handle == STDIO_STDERR_FD) {
		length = serial_wri_dat(LOGTASK_PORTID, buffer, length);
    }
	return length;
}

Weak int _open(const char *fn, int oflag, ...)
{
    return -1;
}

Weak int _close(int fd)
{
	return -1;
}

Weak off_t _lseek(int fd, off_t pos, int whence)
{
	return -1;
}

Weak int _fstat(int fd, struct stat *buf)
{
	return -1;
}

Weak int _isatty(int fd)
{
    return fd == STDIO_STDIN_FD || fd == STDIO_STDOUT_FD || fd == STDIO_STDERR_FD;
}

#endif /* _SMALL_STDIO_ */

/*
 *	For program termination
 */
#define	ATEXIT_MAX	(32 + 1)

static void (*atexit_table[ATEXIT_MAX])(void);
static int atexit_num;

/*
 * abort時に最初に呼出されるフック
 * raise(SIGABRT)を呼出すようにすれば、ホスト環境との互換をとることが可能になる。
 */
void (*_atabort)(void) = 0;

int atexit(void (*func)(void))
{
	int result = -1;
	int sync = iniflg;

	if (sync)
		loc_cpu();

	if (atexit_num < ATEXIT_MAX)
	{
		atexit_table[atexit_num++] = func;
		result = 0;
	}

	if (sync)
		unl_cpu();

	return result;
}

void NoReturn Weak _exit(int status)
{
	sys_exit();
}

void exit(int status)
{
	if (iniflg)
		kernel_exit();
	_exit(status);
}

void Weak abort(void)
{
	syslog(LOG_EMERG, "abort()!");
	if (_atabort != 0)
		(*_atabort)();
	if (iniflg)
		kernel_abort();
	_exit(3);
}

/*
 *	For software term hooks
 */
void software_term_hook(void)
{
	int i;

	for (i = atexit_num - 1; i >= 0; i--)
		(*atexit_table[i])();
}

