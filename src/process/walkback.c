#ifndef	lint
static	char	Id[] = "$Id: walkback.c,v 11.2 1992/11/19 15:12:20 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'doalloc.c'
 * Modified:
 *
 * Function:	Generate a walkback (if possible!)
 */

#include	"ptypes.h"

void	walkback(
	_AR1(char *,	program))
	_DCL(char *,	program)
{
#ifdef	apollo
	char	msg[80];
	FORMAT(msg, "/com/tb %d", getpid());
	FFLUSH(stdout);
	FFLUSH(stderr);
	(void)system(msg);
#endif	/* apollo */
#ifdef	vms
	*((char *)0) = 0;	/* patch */
#endif	/* vms */
}
