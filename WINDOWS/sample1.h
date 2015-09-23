/*
 *  @(#) $Id: sample1.h,v 1.1 2000/11/14 16:29:22 takayuki Exp $
 */

/* 
 *  サンプルプログラム(1)のヘッダファイル
 */

#define TASK_LOOP	(0x2ffffff)		/* タスク内でのループ回数 */
#define TEX_LOOP	500000		/* 例外処理ルーチン内でのループ回数 */

#define MAIN_PRIORITY	5		/* メインタスクの優先度 */
					/* HIGH_PRIORITY より高くすること */

#define HIGH_PRIORITY	9		/* 並列に実行されるタスクの優先度 */
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

#define	STACK_SIZE	8192		/* タスクのスタックサイズ */

extern void	task(VP_INT tskno);
extern void	tex_routine(TEXPTN texptn, VP_INT tskno);
extern void	main_task(VP_INT exinf);

extern void	cyclic_handler(VP_INT exinf);
