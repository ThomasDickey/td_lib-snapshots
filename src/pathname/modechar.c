#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/modechar.c,v 2.1 1989/04/20 16:03:07 dickey Exp $";
#endif	lint

/*
 * Title:	modechar.c (mode/char convert)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * $Log: modechar.c,v $
 * Revision 2.1  1989/04/20 16:03:07  dickey
 * port to VMS
 *
 *		Revision 2.0  87/11/24  13:43:43  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.3  87/11/24  13:43:43  dickey
 *		sccs2rcs keywords
 *		
 *
 * Function:	Translate file-mode to obtain the basic type-character
 *		(e.g., the first character shown by "ls -l").
 */
#include	<sys/types.h>
#include	<sys/stat.h>

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
	default:	c = '?';
	}
	return(c);
}
