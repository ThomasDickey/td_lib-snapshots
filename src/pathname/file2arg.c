#ifndef	lint
static	char	Id[] = "$Id: file2arg.c,v 5.1 1989/12/12 09:14:27 dickey Exp $";
#endif	lint

/*
 * Title:	file2argv.c (file to argv-conversion)
 * Author:	T.E.Dickey
 * Created:	06 Apr 1989
 * $Log: file2arg.c,v $
 * Revision 5.1  1989/12/12 09:14:27  dickey
 * lint (SunOs 4.0.3)
 *
 *
 * Function:	Reads a file and converts it to an argv-style array of pointers
 *		to strings.
 *
 * Arguments:	name -	the name of the file to read from.  If "-", then we
 *			read from the standard input.
 *		vec -	the address of a pointer to array-of-pointers to
 *			strings.
 *
 * Returns:	the number of lines read from the input file (or -1 if an error
 *		prevents reading the file).  The return value should never be
 *		exactly zero.
 *
 * Assumptions:	The file contains no embedded nulls.
 *		The newline character ends each (except possibly the last) line.
 */

#include "ptypes.h"
#include <errno.h>
extern	FILE	*tmpfile();
extern	int	errno;			/* ...not always in <errno.h> */
extern	char	*file2mem();
extern	char	**vecalloc();

#define	AVG_LINE	25		/* nominal line-length */
#define	AMOUNT(n)	(unsigned)(n)

file2argv(name, vec)
char	*name;
char	***vec;
{
	auto	 int	j,
			length,
			newlines,	/* number of newlines found */
			lines;		/* number of newlines found */
	auto	 char	**p, *blob;
	register char	*s, *d;

	*vec = 0;	/* in case we find an error */
	if (!(blob = file2mem(name)))
		return (-1);

	for (s = blob, lines = 0; *s; ++s)
		if (*s == '\n')
			lines++;
	length   = s - blob;
	newlines = lines;
	if (length > 0 && blob[length-1] != '\n')
		lines++;

	/*
	 * Reallocate space for the file counting the nulls inserted
	 * after each newline.
	 */
	if (!(blob = doalloc(blob, AMOUNT(length + lines + 1))))
		return (-1);

	/*
	 * Allocate space for the vector pointing to the strings
	 */
	*vec = p = vecalloc((unsigned)((lines + 1) * sizeof(char *)));

	/*
	 * Recopy the blob onto itself, putting a null after each newline.
	 */
	for (j = newlines, s = blob + length, d = s + newlines; d != s; ) {
		if (*s == '\n') {
			p[j--] = d + 1;
			*d-- = '\0';
		}
		*d-- = *s--;
	}
	p[lines] = 0;		/* always supply a null-pointer */
	p[0] = blob;

	return (lines);
}


#ifdef	TEST
#define	PRINTF	(void)printf
main(argc, argv)
char	*argv[];
{
	register int	j, k;
	auto	 int	lines;
	auto	 char	**vec;
	auto	 char	*name;

	for (j = 1; j < argc; j++) {
		if ((lines = file2argv(name = argv[j], &vec)) > 0) {
			PRINTF("file: %s (%d lines)\n", name, lines);
			for (k=0; vec[k]; k++)
				PRINTF("%d:\t%s", k + 1, vec[k]);
			dofree(vec[0]);	/* frees the blob */
			dofree((char *)vec);
			PRINTF("<EOF>\n");
		} else {
			perror(name);
		}
	}
	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	TEST
