/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 * 
 *  ╛х╡н├°║ю╕в╝╘д╧бд░╩▓╝д╬╛Є╖ядЄ╦■д┐д╣╛ь╣чд╦╕┬дъбд╦▄е╜е╒е╚ежезевб╩╦▄е╜
 *  е╒е╚ежезевдЄ▓■╩╤д╖д┐дтд╬дЄ┤▐дрбе░╩▓╝╞▒д╕б╦дЄ╗╚═╤бж╩г└╜бж▓■╩╤бж║╞╟█
 *  ╔█б╩░╩▓╝бд═°═╤д╚╕╞д╓б╦д╣дыд│д╚дЄ╠╡╜■д╟╡Ў┬·д╣дыбе
 *  (1) ╦▄е╜е╒е╚ежезевдЄе╜б╝е╣е│б╝е╔д╬╖┴д╟═°═╤д╣ды╛ь╣чд╦д╧бд╛х╡нд╬├°║ю
 *      ╕в╔╜╝ибдд│д╬═°═╤╛Є╖ядкдшд╙▓╝╡нд╬╠╡╩▌╛┌╡м─ъдмбдд╜д╬д▐д▐д╬╖┴д╟е╜б╝
 *      е╣е│б╝е╔├цд╦┤▐д▐дьд╞дддыд│д╚бе
 *  (2) ╦▄е╜е╒е╚ежезевдЄе╨еде╩еъе│б╝е╔д╬╖┴д▐д┐д╧╡б┤яд╦┴╚д▀╣■дєд└╖┴д╟═°
 *      ═╤д╣ды╛ь╣чд╦д╧бд╝бд╬ддд║дьдлд╬╛Є╖ядЄ╦■д┐д╣д│д╚бе
 *    (a) ═°═╤д╦╚╝дже╔енехесеєе╚б╩═°═╤╝╘е▐е╦ехевеыд╩д╔б╦д╦бд╛х╡нд╬├°║ю
 *        ╕в╔╜╝ибдд│д╬═°═╤╛Є╖ядкдшд╙▓╝╡нд╬╠╡╩▌╛┌╡м─ъдЄ╖╟║▄д╣дыд│д╚бе
 *    (b) ═°═╤д╬╖┴┬╓дЄбд╩╠д╦─ъдсды╩¤╦бд╦дшд├д╞бд╛х╡н├°║ю╕в╝╘д╦╩є╣Ёд╣ды
 *        д│д╚бе
 *  (3) ╦▄е╜е╒е╚ежезевд╬═°═╤д╦дшдъ─╛└▄┼кд▐д┐д╧┤╓└▄┼кд╦└╕д╕дыдддлд╩ды┬╗
 *      │▓длдщдтбд╛х╡н├°║ю╕в╝╘дЄ╠╚└╒д╣дыд│д╚бе
 * 
 *  ╦▄е╜е╒е╚ежезевд╧бд╠╡╩▌╛┌д╟─є╢бд╡дьд╞дддыдтд╬д╟двдыбе╛х╡н├°║ю╕в╝╘д╧бд
 *  ╦▄е╜е╒е╚ежезевд╦┤╪д╖д╞бдд╜д╬┼м═╤▓─╟╜└ндт┤▐дсд╞бддддлд╩ды╩▌╛┌дт╣╘дя
 *  д╩ддбед▐д┐бд╦▄е╜е╒е╚ежезевд╬═°═╤д╦дшдъ─╛└▄┼кд▐д┐д╧┤╓└▄┼кд╦└╕д╕д┐дд
 *  длд╩ды┬╗│▓д╦┤╪д╖д╞дтбдд╜д╬└╒╟ддЄ╔щдяд╩ддбе
 * 
 *  @(#) $Id: linux_sigio.c,v 1.1 2000/11/14 16:27:57 honda Exp $
 */


#include <jsp_services.h>
#include "../kernel/jsp_kernel.h"
#include "../kernel/queue.h"
#include "kernel_id.h"
#include <signal.h>
#include <linux_sigio.h>


/*
 *  SIGIO$BDLCN%$%Y%s%H%-%e!<(B
 */

static QUEUE	sigio_queue;

/*
 *  SIGIO$BDLCN%O%s%I%i(B
 *
 *  SIGIO$BDLCN=hM}%?%9%/$r5/>2$9$k!%(B
 */

void
linux_sigio_handler()
{
	syscall(iwup_tsk(TSK_LINUX_SIGIO));
}

/*
 *  $B%N%s%V%m%C%-%s%0(BI/O $B%b%8%e!<%k5/F0%k!<%A%s(B
 *
 *  SIGIO$BDLCN%$%Y%s%H%-%e!<$r=i4|2=!%(BSIGIO$BDLCN=hM}%?%9%/$H%;%^%U%)$r@8(B
 *  $B@.!%(BSIGIO$BDLCN%O%s%I%i$r@_Dj!%(B
 */

static int	initflag = 0;		/* $B=i4|2=:Q%U%i%0(B */

void
linux_sigio_initialize(VP_INT exinf)
{
    
	if (!initflag) {
		queue_initialize(&sigio_queue);
		initflag = 1;
	}
}

/*
 *  SIGIO$BDLCN=hM}%?%9%/K\BN(B
 */

void
linux_sigio_task(void)
{
	QUEUE	*q;
	SIGIOEB	*event;

	while (slp_tsk() == E_OK) {
		/*
		 *  SIGIO$BDLCN%$%Y%s%H%-%e!<Cf$N3F%$%Y%s%H%V%m%C%/$N%3!<(B
		 *  $B%k%P%C%/%k!<%A%s$r8F$S$@$9!%%3!<%k%P%C%/%k!<%A%s$,(B
		 *  0 $B0J30$rJV$7$?>l9g!$$=$N%$%Y%s%H%V%m%C%/$r:o=|$9$k!%(B
		 */
		syscall(wai_sem(SEM_LINUX_SIGIO));
		q = sigio_queue.next;
		while (q != &sigio_queue) {
			event = ((SIGIOEB *) q);
			q = q->next;
			if ((*(event->callback))(event->arg)) {
				queue_delete((QUEUE *) &(event->queue));
			}
		}
		syscall(sig_sem(SEM_LINUX_SIGIO));
	}
	ext_tsk();
}

/*
 *  SIGIO$BDLCN%$%Y%s%H$r(BSIGIO$BDLCN%$%Y%s%H%-%e!<$XA^F~(B
 *
 *  $B%7%9%F%`%*%V%8%'%/%H$K%"%/%;%9$9$k$?$a$K!$3HD%(BSVC$B%O%s%I%i$H$7$FF0(B
 *  $B:n$5$;$F$$$k!%(B
 */

ER
enqueue_sigioeb(SIGIOEB *event)
{
	syscall(wai_sem(SEM_LINUX_SIGIO));
	queue_insert_prev(&sigio_queue , (QUEUE *) &(event->queue));
	syscall(sig_sem(SEM_LINUX_SIGIO));
	return(E_OK);
}


/*
 *   $B%7%9%F%`5/F0;~EPO?MQ(B
 *   $B%7%9%F%`5/F0;~!"=i4|2=%k!<%A%s$OHs%?%9%/%3%s%F%-%9%H$+$i(B
 *   $B8F$P$l$k$?$a(Benqueue_sigioeb()$B$G%;%^%U%)$,;HMQ$G$-$J$$(B
 */

ER
enqueue_sigioeb_initialize(SIGIOEB *event)
{
	queue_insert_prev(&sigio_queue , (QUEUE *) &(event->queue));
	return(E_OK);
}






























