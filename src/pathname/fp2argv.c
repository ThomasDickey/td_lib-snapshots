/*
 * Title:	fp2argv (file-pointer to argv-list)
 * Author:	T.E.Dickey
 * Created:	18 Jul 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		17 Nov 1992, modified _FNX macro.
 *		25 Feb 1992, allow input-records to be arbitrarily wide.
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
#include	"dyn_str.h"

MODULE_ID("$Id: fp2argv.c,v 12.3 1993/10/29 17:35:26 tom Exp $")

	/*ARGSUSED*/
	def_DOALLOC(char *)
#define	CHUNK	32

static
char *
get_line(
_AR1(FILE *,	fp))
_DCL(FILE *,	fp)
{
	static	DYN	*bfr;
	static	char	tmp[] = "?";

	dyn_init(&bfr, BUFSIZ);

	do {
		tmp[0] = fgetc(fp);
		if (feof(fp))
			break;
		(void)dyn_append(bfr, tmp);
		if (tmp[0] == '\n')
			break;
	} while (!ferror(fp));

	return dyn_length(bfr) ? dyn_string(bfr) : 0;
}

int	fp2argv(
	_ARX(FILE *,	fp)
	_ARX(char ***,	argv_)
	_FN1(void,	trace,	(_AR1(char *,s)))
		)
	_DCL(FILE *,	fp)
	_DCL(char ***,	argv_)
	_DCL(void,	(*trace)())
{
	register char **vec = 0;
	register int  lines = 0;
	register int  have  = 0;
	char	*buffer;

	while ((buffer = get_line(fp)) != 0) {
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
		PRINTF("%d:\t%s", j+1, list[j]);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
