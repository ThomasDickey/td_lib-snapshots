#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/curses/RCS/blip.c,v 8.0 1988/08/15 09:54:15 ste_cm Rel $";
#endif	lint

/*
 * Title:	blip.c (write a single character to screen)
 * Author:	T.E.Dickey
 * Created:	11 Aug 1988, from 'beep()'
 * $Log: blip.c,v $
 * Revision 8.0  1988/08/15 09:54:15  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  88/08/15  09:54:15  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/15  09:54:15  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/15  09:54:15  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/15  09:54:15  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/15  09:54:15  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/15  09:54:15  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  88/08/15  09:54:15  dickey
 *		sccs2rcs keywords
 *		
 *		15 Aug 1988, lint
 */

#include	<stdio.h>

blip(c)
{
#ifdef	putc
	c = putc((unsigned char)c, stderr);
#else
	c = putc(c, stderr);
#endif
	if (c != EOF)
		(void)fflush(stderr);
}
