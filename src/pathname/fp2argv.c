#ifndef	lint
static	char	sccs_id[] = "@(#)fp2argv.c	1.3 88/07/27 08:18:05";
#endif	lint

/*
 * Title:	fp2argv (file-pointer to argv-list)
 * Author:	T.E.Dickey
 * Created:	18 Jul 1988
 * Modified:
 *
 * Function:	Reads a file via standard I/O and allocates an argv-like
 *		structure containing the file in memory.
 *
 * Arguments:	fp	the file pointer via which to read
 *		argv_	the address of the pointer to the resulting structure.
 *
 * Returns:	the number of lines allocated.
 */

#include	<stdio.h>
extern	char	*doalloc();
extern	char	*stralloc();

#define	CHUNK	32
#ifdef	lint
/*ARGSUSED*/	char **ALLOC(old,num) char **old; { return(0); }
#else	lint
#define	ALLOC(old,num)	(char **)doalloc((char *)old, sizeof(char *)*num)
#endif	lint

fp2argv(fp, argv_)
FILE	*fp;
char	***argv_;
{
	register char **vec = 0;
	register int  lines = 0;
	register int  have  = 0;
	char	buffer[BUFSIZ];

	while (fgets(buffer, sizeof(buffer), fp)) {
		int	need	= (++lines | (CHUNK-1)) + 1;
		if (need != have) {
			vec  = ALLOC(vec, need);
			have = need;
		}
		vec[lines-1] = stralloc(buffer);
	}
	if (lines == 0)
		vec = ALLOC(vec, 1);
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
#endif	TEST
