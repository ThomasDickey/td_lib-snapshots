/*
 * Title:	file2argv.c (file to argv-conversion)
 * Author:	T.E.Dickey
 * Created:	06 Apr 1989
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		05 Mar 1990, lint
 *		12 Dec 1989, lint (SunOs 4.0.3)
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

MODULE_ID("$Id: file2arg.c,v 12.11 2025/01/07 00:29:46 tom Exp $")

#define	AVG_LINE	25	/* nominal line-length */
#define	AMOUNT(n)	(size_t)(n)

int
file2argv(const char *name, char ***vec)
{
    int j;
    int length;
    int newlines;		/* number of newlines found */
    int lines;			/* number of newlines found */
    char **p, *blob;
    char *s, *d;

    *vec = NULL;		/* in case we find an error */
    if ((blob = file2mem(name)) == NULL)
	return (-1);

    for (s = blob, lines = 0; *s; ++s)
	if (*s == '\n')
	    lines++;
    length = (int) (s - blob);
    newlines = lines;
    if (length > 0 && blob[length - 1] != '\n')
	lines++;

    /*
     * Reallocate space for the file counting the nulls inserted
     * after each newline.
     */
    if ((blob = doalloc(blob, AMOUNT(length + lines + 1))) == NULL)
	return (-1);

    /*
     * Allocate space for the vector pointing to the strings
     */
    *vec = p = vecalloc((size_t) (lines + 1) * sizeof(char *));

    /*
     * Recopy the blob onto itself, putting a null after each newline.
     */
    for (j = newlines, s = blob + length, d = s + newlines; d != s;) {
	if (*s == '\n') {
	    p[j--] = d + 1;
	    *d-- = EOS;
	}
	*d-- = *s--;
    }
    p[lines] = NULL;		/* always supply a null-pointer */
    p[0] = blob;

    return (lines);
}

#ifdef	TEST
_MAIN
{
    int j, k;
    int lines;
    char **vec;
    char *name;

    for (j = 1; j < argc; j++) {
	if ((lines = file2argv(name = argv[j], &vec)) > 0) {
	    PRINTF("file: %s (%d lines)\n", name, lines);
	    for (k = 0; vec[k]; k++)
		PRINTF("%d:\t%s", k + 1, vec[k]);
	    dofree(vec[0]);	/* frees the blob */
	    dofree((char *) vec);
	    PRINTF("<EOF>\n");
	} else {
	    perror(name);
	}
    }
    (void) exit(SUCCESS);
    /*NOTREACHED */
}
#endif
