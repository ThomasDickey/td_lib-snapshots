#ifndef	lint
static	char	Id[] = "$Id: strucmp.c,v 12.0 1991/10/04 07:42:27 ste_cm Rel $";
#endif

/*
 * Title:	strucmp.c
 * Created:	05 Oct 1988
 * Modified:
 *		03 Oct 1991, converted to ANSI
 * Function:	Compares two strings ignoring case.
 */

#include	"ptypes.h"
#include	<ctype.h>

strucmp(
_ARX(register char *,	a)
_AR1(register char *,	b)
	)
_DCL(register char *,	a)
_DCL(register char *,	b)
{
	register int	x, y;

	for (;;) {
		if ((x = *a++) != (y = *b++)) {
			if (x == 0 || y == 0)
				break;
			if (isalpha(x) && islower(x))	x = toupper(x);
			if (isalpha(y) && islower(y))	y = toupper(y);
			if (x != y)
				break;
		} else if (x == 0)
			break;
	}
	return (x - y);
}

#ifdef	TEST
_MAIN
{
	register int	j, k;
	static	 char	*tag[] = { "<", "==", ">" };

	for (j = 1; j < argc; j++) {
		k = strucmp(argv[1], argv[j]);
		if (k < 0) k = -1;
		if (k > 0) k = 1;
		PRINTF("%s %s %s\n", argv[1], tag[k+1], argv[j]);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
