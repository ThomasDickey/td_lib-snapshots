#ifndef	lint
static	char	Id[] = "$Id: fp2argv.c,v 10.0 1991/10/04 16:38:04 ste_cm Rel $";
#endif

/*
 * Title:	fp2argv (file-pointer to argv-list)
 * Author:	T.E.Dickey
 * Created:	18 Jul 1988
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *		20 Jun 1991, added trace-arg for 'ded'
 *
 * Function:	Reads a file via standard I/O and allocates an argv-like
 *		structure containing the file in memory.
 *
 * Arguments:	fp	the file pointer via which to read
 *		argv_	the address of the pointer to the resulting structure.
 *
 * Returns:	the number of lines allocated.
 */

#include	"ptypes.h"

	/*ARGSUSED*/
	def_DOALLOC(char *)
#define	CHUNK	32

fp2argv(
_ARX(FILE *,	fp)
_ARX(char ***,	argv_)
_FN1(void,	trace)
	)
_DCL(FILE *,	fp)
_DCL(char ***,	argv_)
_DCL(void,	(*trace)())
{
	register char **vec = 0;
	register int  lines = 0;
	register int  have  = 0;
	char	buffer[BUFSIZ];

	while (fgets(buffer, sizeof(buffer), fp)) {
		unsigned need	= (++lines | (CHUNK-1)) + 1;
		if (need != have) {
			vec  = DOALLOC(vec, char *, need);
			have = need;
		}
		vec[lines-1] = stralloc(buffer);
		if (trace != 0)
			(*trace)(buffer);
	}
	if (lines == 0)
		vec = DOALLOC(vec, char *, 1);
	vec[lines]   = 0;
	*argv_ = vec;
	return (lines);
}

#ifdef	TEST
_MAIN
{
	char	**list;
	register int j;

	fp2argv(stdin, &list, 0);
	for (j = 0; list[j]; j++)
		PRINTF("%5d:\t%s", j+1, list[j]);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
