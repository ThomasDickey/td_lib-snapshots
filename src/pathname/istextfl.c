/*
 * Title:	istextfile.c (is this a text-file?)
 * Author:	T.E.Dickey
 * Created:	24 Oct 1989
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	Test a given file to see if if contains characters which are
 *		not normally considered printable ASCII.  If so (or if the
 *		file cannot be opened), return false.
 *
 *		If the file is empty, true is returned.
 */

#include	"ptypes.h"
#include	<ctype.h>

MODULE_ID("$Id: istextfl.c,v 12.3 1993/10/29 17:35:25 tom Exp $")

int	istextfile(
	_AR1(char *,	name))
	_DCL(char *,	name)
{
	auto	FILE	*fp;
	auto	char	bfr[BUFSIZ];
	register int	n, c;

	if ((fp = fopen(name, "r")) != 0) {
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
