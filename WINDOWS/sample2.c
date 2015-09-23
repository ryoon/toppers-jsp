/*
 *  @(#) $Id: sample2.c,v 1.3 2000/11/14 17:24:36 takayuki Exp $
 */

/*
 * TOPPERS/JSP サンプルプログラム  - 5人の哲学者による哲学者問題 -
 *         Last update : 13th, November, 2000
 */

#include <jsp_services.h>
#include "kernel_id.h"

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
	syslog(LOG_NOTICE,"Sample task starts (exinf = %d)", exinf);
	
	act_tsk(1);
	act_tsk(2);
	act_tsk(3);
	act_tsk(4);
	act_tsk(5);

	do{
		wai_flg(1, 0xf0, TWF_ORW, &ptn);
		if((ptn & 0xf) >= 5)
			continue;

		if( buffer[ptn & 0xf] != patternchar[(ptn >> 4) & 0xf] )
		{
			buffer[ptn & 0xf] = patternchar[(ptn >> 4) & 0xf];
			syslog(LOG_NOTICE,buffer);
		}
	}while(1==1);
}
