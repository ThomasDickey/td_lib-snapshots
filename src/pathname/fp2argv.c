#ifndef	lint
static	char	Id[] = "$Id: fp2argv.c,v 9.1 1991/06/20 08:09:22 dickey Exp $";
#endif

/*
 * Title:	fp2argv (file-pointer to argv-list)
 * Author:	T.E.Dickey
 * Created:	18 Jul 1988
 * Modified:
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
extern	char	*stralloc();

	/*ARGSUSED*/
	def_DOALLOC(char *)
#define	CHUNK	32

fp2argv(fp, argv_, trace)
FILE	*fp;
char	***argv_;
void	(*trace)();
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
main(argc, argv)
char	*argv[];
{
	char	**list;
	register int j;

	fp2argv(stdin, &list);
	for (j = 0; list[j]; j++)
		printf("%5d:\t%s", j+1, list[j]);
}

failed(s)
char	*s;
{
	perror(s);
	exit(1);
}
#endif
