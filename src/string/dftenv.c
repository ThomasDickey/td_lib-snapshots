#ifndef	lint
static	char	what[] = "$Id: dftenv.c,v 8.0 1989/08/08 13:59:32 ste_cm Rel $";
#endif	lint

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
