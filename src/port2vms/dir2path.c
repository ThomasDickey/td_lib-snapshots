#ifndef	lint
static	char	Id[] = "$Id: dir2path.c,v 5.0 1991/05/20 17:14:58 ste_cm Rel $";
#endif

/*
 * Title:	dir2path.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 *
 * Function:	Convert a VMS directory-name into the name of the corresponding
 *		path.
 *
 *		Note that this returns a pointer to a static buffer which is
 *		overwritten by each call.
 */

#define	STR_PTYPES
#include	"portunix.h"
#include	<ctype.h>

static
insert(dst, src)
char	*dst, *src;
{
	register int	c,d;

	for (d = strlen(src), c = strlen(dst); c >= 0; c--)
		dst[c+d] = dst[c];

	while (c = *src++)
		*dst++ = c;
}

char *
dir2path(src)
char	*src;
{
	static	char	buffer[MAXPATHLEN];
	register char	*s, *t;

	(void)strucpy(buffer, src);
	if (t = strrchr(buffer, '.')) {
		while ((s = strrchr(buffer, ';')) == 0)
			(void)strcat(buffer, ";1");
		if (! s[1])
			(void)strcat(buffer, "1");
		if (!strncmp(t, ".DIR;1", 6)) {
			*t = '\0';
			if (s = strrchr(buffer, ']')) {
				*s = '.';
			} else if (s = strrchr(buffer, ':')) {
				insert(s+1, "[");
			} else
				insert(buffer, "[.");
			(void)strcat(buffer, "]");
		}
	}
	return (buffer);
}

#ifdef	TEST
main(argc,argv)
char	*argv[];
{
	register int	j;
	for (j = 1; j < argc; j++)
		printf("%s => %s\n", argv[j], dir2path(argv[j]));
}
#endif
