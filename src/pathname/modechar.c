#ifndef	lint
static	char	Id[] = "$Id: modechar.c,v 12.1 1993/09/21 18:54:04 dickey Exp $";
#endif

/*
 * Title:	modechar.c (mode/char convert)
 * Author:	T.E.Dickey
 * Created:	18 Nov 1987
 * Modified:
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		25 Jul 1989, recompiled with apollo SR10 -- mods for function
 *			     prototypes
 *		11 May 1989, show fifo's as "p"; added hack for apollo to
 *			     ensure this works.
 *		20 Apr 1989, port to VMS
 *		
 * Function:	Translate file-mode to obtain the basic type-character
 *		(e.g., the first character shown by "ls -l").
 */

#include	"ptypes.h"

#ifdef	apollo
#define	S_IFFIFO	010000
#endif

int	modechar(
	_AR1(unsigned,	mode))
	_DCL(unsigned,	mode)
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
