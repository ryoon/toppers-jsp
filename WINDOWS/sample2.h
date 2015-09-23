/*
 *  @(#) $Id: sample2.h,v 1.1 2000/11/14 16:29:22 takayuki Exp $
 */

/* 
 *  サンプルプログラム(2)のヘッダファイル
 */

#define MAIN_PRIORITY	5		/* メインタスクの優先度 */
					/* HIGH_PRIORITY より高くすること */

#define HIGH_PRIORITY	9		/* 並列に実行されるタスクの優先度 */
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

#define	STACK_SIZE	8192		/* タスクのスタックサイズ */

extern void	task(VP_INT tskno);
extern void	main_task(VP_INT exinf);
