#ifndef	lint
static	char	Id[] = "$Id: dftenv.c,v 9.0 1991/05/15 10:03:16 ste_cm Rel $";
#endif

/*
 * Title:	dftenv.c (getenv with default)
 * Author:	T.E.Dickey
 * Created:	08 Aug 1989
 *
 * Function:	Performs a 'getenv()' and returns the environment value if it
 *		is defined, otherwise the value supplied by the user as an arg.
 */

extern	char	*getenv();

char *
dftenv(value, name)
char	*value, *name;
{
	register char	*s;
	if ((s = getenv(name)) == 0)
		s = value;
	return (s);
}
