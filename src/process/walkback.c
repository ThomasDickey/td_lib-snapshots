#ifndef	lint
static	char	Id[] = "$Id: walkback.c,v 11.1 1992/11/18 07:24:02 dickey Exp $";
#endif

/*
 * Author:	T.E.Dickey
 * Created:	18 Nov 1992, from 'doalloc.c'
 * Modified:
 *
 * Function:	Generate a walkback (if possible!)
 */

#include	"ptypes.h"

void	walkback(_AR0)
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
