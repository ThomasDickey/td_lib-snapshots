/*
 * Function:	Translate file-mode to obtain the basic type-character
 *		(e.g., the first character shown by "ls -l").
 */
#include	<sys/types.h>
#include	<sys/stat.h>

modechar(mode)
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
	case S_IFLNK:	c = 'l';	break;
	default:	c = '?';
	}
	return(c);
}
