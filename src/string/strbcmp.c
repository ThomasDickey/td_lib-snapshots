#ifndef	lint
static	char	Id[] = "$Id: strbcmp.c,v 9.1 1991/10/04 07:37:13 dickey Exp $";
#endif

/*
 * Title:	strbcmp.c (string-blank-compare)
 * Author:	T.E.Dickey
 * Created:	07 Apr 1989
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *
 * Function:	compare two strings, treating sequences of embedded whitespace
 *		as a single blank, and ignoring trailing whitespace.  This is
 *		essentially the comparison performed by "diff -b".
 *
 * Returns:	the lexical difference between the two strings (0 if equal,
 *		positive if the first string would sort before the second,
 *		otherwise negative).
 */

#include	"ptypes.h"
#include	<ctype.h>

#define	SKIP(p)	while (isspace(*p))	p++;

strbcmp(
_ARX(register char *,	a)
_AR1(register char *,	b)
	)
_DCL(register char *,	a)
_DCL(register char *,	b)
{
	register int	cmp;

	while (*a && *b) {
		if (isspace(*a) && isspace(*b)) {
			SKIP(a);
			SKIP(b);
		} else if (cmp = (*a++ - *b++))
			return (cmp);
	}
	SKIP(a);
	SKIP(b);
	return (*a - *b);
}

#ifdef	TEST
_MAIN
{
	extern	time_t	time();
	auto	time_t	now;
	static	char	*tbl[] = {
				"ab",
				" ab",
				"a b",
				"ab ",
				" ab ",
				"a b "
			};
	register int	j, k, cmp;

#define	LOOP(j)	for (j = 0; j < sizeof(tbl)/sizeof(tbl[0]); j++)
#define	CMP(f)	((cmp = f(tbl[j],tbl[k])) ? (cmp > 0 ? ">" : "<") : "=")
	LOOP(j) {
		printf("\n");
		LOOP(k) {
			printf("\"%s\"\t\"%s\"\t%s (blank) %s (normal)\n",
				tbl[j], tbl[k],
				CMP(strbcmp),
				CMP(strcmp));
		}
	}
	now = time(0);
	for (cmp = 0; cmp < 20000; cmp++)
		LOOP(j) {
			LOOP(k) {
				(void)strbcmp(tbl[j], tbl[k]);
			}
		}
	printf("# %d seconds\n", time(0) - now);
	(void)exit(0);
	/*NOTREACHED*/
}
#endif
