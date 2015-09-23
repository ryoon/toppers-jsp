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
 *  @(#) $Id: sample2.c,v 1.5 2001/11/12 14:54:57 takayuki Exp $
 */


/*
 * TOPPERS/JSP サンプルプログラム  - 5人の哲学者による哲学者問題 -
 *    デバイスエミュレーション デモ
 *         Last update : 23rd, February, 2001
 */

#include <jsp_services.h>
#include "kernel_id.h"
#include "device.h"
#include "debugout.h"

void task(VP_INT exinf)
{
	ID tskid = (ID)exinf ;

	while(1==1)
	{
		set_flg(1,0x10 | (tskid-1));	/* 空腹 */
		if(pol_sem(tskid) == E_OK)
		{
			if(pol_sem( 1 + (tskid + 4 - 1) % 5 ) == E_OK)
			{
				set_flg(1,0x20 | (tskid-1));	/* 食事 */
				dly_tsk(1000 * tskid);

				set_flg(1,0x40 | (tskid-1));	/* 満腹 */
				sig_sem( 1 + (tskid + 4 - 1) % 5 );
				sig_sem(tskid);
				dly_tsk(1000 * tskid);
				continue;
			}else
				sig_sem(tskid);
		}
		dly_tsk(1000 * tskid);
	}
}

void main_task(VP_INT exinf)
{
	char patternchar[5] = {'.','-','O','.','z'};
	FLGPTN ptn;
	char buffer[6] = "-----";


	serial_ioctl(0, (IOCTL_CRLF | IOCTL_RAW | IOCTL_IXON | IOCTL_IXOFF));
	syslog_1(LOG_NOTICE,"Sample task starts (exinf = %d)", exinf);
	
	act_tsk(1);
	act_tsk(2);
	act_tsk(3);
	act_tsk(4);
	act_tsk(5);

	do{
		wai_flg(1, 0xf0, TWF_ORW, &ptn);
		if((ptn & 0xf) >= 5)
			continue;

		OnDevice DeviceWrite(100,4,&ptn);

		if( buffer[ptn & 0xf] != patternchar[(ptn >> 4) & 0xf] )
		{
			buffer[ptn & 0xf] = patternchar[(ptn >> 4) & 0xf];
			syslog_5(LOG_NOTICE,"%c%c%c%c%c",buffer[0],buffer[1],buffer[2],buffer[3],buffer[4]);
			DebugOut(buffer);
			DebugOut("\n");
		}
	}while(1==1);
}
