#ifndef	lint
static	char	Id[] = "$Id: modechar.c,v 9.0 1991/05/15 10:04:15 ste_cm Rel $";
#endif

/*
 * Title:	modechar.c (mode/char convert)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * $Log: modechar.c,v $
 * Revision 9.0  1991/05/15 10:04:15  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:04:15  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/07/25  09:07:28  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/07/25  09:07:28  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/07/25  09:07:28  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/07/25  09:07:28  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/07/25  09:07:28  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  09:07:28  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  89/05/11  12:52:20  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.2  89/05/11  12:52:20  dickey
 *		show fifo's as "p"; added hack for apollo to ensure this works.
 *		
 *		Revision 2.1  89/04/20  16:03:07  dickey
 *		port to VMS
 *		
 *		Revision 2.0  87/11/24  13:43:43  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 * Function:	Translate file-mode to obtain the basic type-character
 *		(e.g., the first character shown by "ls -l").
 */
#include	<sys/types.h>
#include	<sys/stat.h>

#ifdef	apollo
#define	S_IFFIFO	010000
#endif

modechar(mode)
unsigned mode;
{
register int	c;
	switch (mode & S_IFMT) {
	case S_IFDIR:	c = 'd';	break;
	case S_IFCHR:	c = 'c';	break;
	case S_IFBLK:	c = 'b';	break;
	case S_IFREG:	c = '-';	break;
#ifdef	S_IFSOCK
	case S_IFSOCK:	c = 's';	break;
#ifdef	S_IFFIFO
#undef	S_IFFIFO	/* fix apollo SR10 conflict */
#endif
#endif
#ifdef	S_IFLNK
	case S_IFLNK:	c = 'l';	break;
#endif
#ifdef	S_IFFIFO
	case S_IFFIFO:	c = 'p';	break;
#endif
	default:	c = '?';
	}
	return(c);
}
