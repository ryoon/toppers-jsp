
#ifndef _SYS_DEFS_H_
#define _SYS_DEFS_H_

/*	Values	*/

#define TARGET_CPU_STR		"SapphireSoC"

/* endian */
#define SIL_ENDIAN      0		/* LITTLE ENDIAN */

/*	Address		*/
#define RAM_BASE        0xF9002000
#define RAM_SIZE        (128*1024)
#define VECTOR_SIZE		(4*8)
#define CLOAD_ADDRESS   0xF9000000

/*
 *  微少時間待ちのための定義
 *  　本当はクロック周波数に依存する。
 */
#define    SIL_DLY_TIM1     1
#define    SIL_DLY_TIM2     110

/*	Default Values	*/

#define MAX_INT_NUM     32
#define MAX_PRIORITY    256
#define DEF_DUMP_LENGTH	256
#define MCAUSE_INT          0x80000000
#define MCAUSE_CAUSE        0x7FFFFFFF

#define MonitorVector   &trap_entry
#define UserVector      RAM_BASE
#define NOT_INITDECT
#define NOT_DEFVECTORTYPE

#ifndef _MACRO_ONLY
/*
 *  コンパイラ依存のデータ型の定義
 */
#define	_int8_		char		/* 8ビットの整数型	*/
#define	_int16_		short		/* 16ビットの整数型	*/
#define	_int32_		int			/* 32ビットの整数型	*/
#define	_int64_		long long	/* 64ビットの整数型	*/

/*
 *  コンパイラの拡張機能のためのマクロ定義
 */
#define	Inline          static inline

typedef	int                 ID;			/* オブジェクトのID番号 */
typedef	int                 ER;			/* エラーコード */
typedef int                 bool_t;		/* 真偽値 */

#ifndef NULL
#define NULL                0			/* 無効ポインタ */
#endif /* NULL */

#ifndef true
#define true                1			/* 真 */
#endif /* true */

#ifndef false
#define false               0			/* 偽 */
#endif /* false */

/*
 *  SDMMC拡張エラー定義
 */
#define E_OK                0			/* 正常終了 */
#define E_SYS               (-5)		/* システムエラー */
#define E_PAR               (-17)		/* パラメータエラー */
#define E_OBJ               (-41)		/* オブジェクト状態エラー */
#define E_TMOUT             (-50)		/* ポーリング失敗またはタイムアウト */
#define E_SDCOM             (-80)		/* コマンドエラー */

typedef void (*function_ptr_t) (void);

#define plic_set_vector(n, f)   (plic_interrupt_handlers[0] = f)
#define Enable()                set_csr(mstatus, MSTATUS_MIE)
#define Disable()               clear_csr(mstatus, MSTATUS_MIE)

extern void SetVector(void *addr);
extern int sys_printf(const char * format);
extern void sys_putc(char c);
extern int apl_printf(const char * format);
extern void apl_puthex(unsigned int c, unsigned int val);
extern void apl_putchar(char c);
extern void trap_entry(void);

extern void plic_init (void);
extern void plic_set_threshold (unsigned long threshold);
extern void plic_enable_interrupt(unsigned int source);
extern void plic_disable_interrupt (unsigned int source);
extern void plic_set_priority (unsigned int source, unsigned int priority);

extern void sil_dly_nse(unsigned int cnt);
extern function_ptr_t plic_interrupt_handlers[];

#endif	/* _MACRO_ONLY */

#include "sapphire-soc.h"
#include "encoding.h"

#define SYSCLK_FREQUENCY    SYSTEM_CLINT_HZ	/*  SYSCLK clock frequency expressed in Hz */

/*	Serial Channels		*/

#define MAX_SERIAL		1
#define DEF_MON_SERIAL	0


/* serial definitions */

#define DEFAULT_SPEED		115200

#endif	/* SYS_DEFS_H_ */

