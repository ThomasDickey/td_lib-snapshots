#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/path2dir.c,v 4.0 1988/09/30 11:40:35 ste_cm Rel $";
#endif	lint

/*
 * Title:	path2dir.c
 * Author:	T.E.Dickey
 * Created:	30 Sep 1988
 *
 * Function:	Convert a VMS pathname into the name of the corresponding
 *		directory-file.
 *
 *		Note that this returns a pointer to a static buffer which is
 *		overwritten by each call.
 */

#define		STR_PTYPES
#include	"portunix.h"

char *
path2dir(src)
char	*src;
{
	static	char	buffer[MAXPATHLEN];
	register char	*s	= buffer + strlen(strcpy(buffer, src));

	if (s != buffer && *(--s) == ']') {
		(void)strcpy(s, ".DIR");
		while (--s >= buffer) {
			if (*s == '.') {
				*s = ']';
				if (s == buffer+1) {	/* absorb "[]" */
					register char *t = s + 1;
					s = buffer;
					while (*s++ = *t++);
				}
				break;
			}
			if (*s == '[') {		/* absorb "[" */
				register char *t = s + 1;
				while (*s++ = *t++);
				break;
			}
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
		printf("%s => %s\n", argv[j], path2dir(argv[j]));
}
#endif	TEST
