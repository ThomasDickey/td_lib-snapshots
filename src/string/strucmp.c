#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/string/RCS/strucmp.c,v 8.0 1988/10/05 14:55:33 ste_cm Rel $";
#endif	lint

/*
 * Title:	strucmp.c
 * Created:	05 Oct 1988
 * Function:	Compares two strings ignoring case.
 */

#include	<ctype.h>

strucmp(a,b)
register char	*a, *b;
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
main(argc, argv)
char	*argv[];
{
	register int	j, k;
	static	 char	*tag[] = { "<", "==", ">" };

	for (j = 1; j < argc; j++) {
		k = strucmp(argv[1], argv[j]);
		if (k < 0) k = -1;
		if (k > 0) k = 1;
		printf("%s %s %s\n", argv[1], tag[k+1], argv[j]);
	}
}
#endif	TEST
