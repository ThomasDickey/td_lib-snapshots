#ifndef	lint
static	char	Id[] = "$Id: istextfl.c,v 9.0 1991/05/15 09:26:28 ste_cm Rel $";
#endif

/*
 * Title:	istextfile.c (is this a text-file?)
 * Author:	T.E.Dickey
 * Created:	24 Oct 1989
 *
 * Function:	Test a given file to see if if contains characters which are
 *		not normally considered printable ASCII.  If so (or if the
 *		file cannot be opened), return false.
 *
 *		If the file is empty, true is returned.
 */

#include	"ptypes.h"
#include	<ctype.h>

istextfile(name)
char	*name;
{
	auto	FILE	*fp;
	auto	char	bfr[BUFSIZ];
	register int	n, c;

	if (fp = fopen(name, "r")) {
		while ((n = fread(bfr, sizeof(char), sizeof(bfr), fp)) > 0) {
			while (n-- > 0) {
				c = bfr[n];
				if (!isascii(c)
				||  (!isprint(c) && !isspace(c))) {
					FCLOSE(fp);
					return (FALSE);
				}
			}
		}
		FCLOSE(fp);
		return (TRUE);
	}
	return (FALSE);
}
