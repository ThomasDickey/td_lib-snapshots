#ifndef	lint
static	char	sccs_id[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/fp2argv.c,v 8.0 1988/08/10 12:27:48 ste_cm Rel $";
#endif	lint

/*
 * Title:	fp2argv (file-pointer to argv-list)
 * Author:	T.E.Dickey
 * Created:	18 Jul 1988
 * $Log: fp2argv.c,v $
 * Revision 8.0  1988/08/10 12:27:48  ste_cm
 * BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *
 *		Revision 7.0  88/08/10  12:27:48  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  88/08/10  12:27:48  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  88/08/10  12:27:48  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  88/08/10  12:27:48  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.0  88/08/10  12:27:48  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/08/10  12:27:48  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.6  88/08/10  12:27:48  dickey
 *		sccs2rcs keywords
 *		
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
#include	<stdio.h>
extern	char	*stralloc();

	/*ARGSUSED*/
	def_DOALLOC(char *)
#define	CHUNK	32

fp2argv(fp, argv_)
FILE	*fp;
char	***argv_;
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
#endif	TEST
