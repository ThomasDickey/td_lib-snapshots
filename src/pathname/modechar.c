#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/modechar.c,v 3.0 1989/05/11 12:52:20 ste_cm Rel $";
#endif	lint

/*
 * Title:	modechar.c (mode/char convert)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * $Log: modechar.c,v $
 * Revision 3.0  1989/05/11 12:52:20  ste_cm
 * BASELINE Mon Jun 19 13:27:01 EDT 1989
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
#endif	S_IFSOCK
#ifdef	S_IFLNK
	case S_IFLNK:	c = 'l';	break;
#endif	S_IFLNK
#ifdef	S_IFFIFO
	case S_IFFIFO:	c = 'p';	break;
#endif
	default:	c = '?';
	}
	return(c);
}
