#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/file2arg.c,v 2.1 1989/04/12 15:16:16 dickey Exp $";
#endif	lint

/*
 * Title:	file2argc.c (file to argv-conversion)
 * Author:	T.E.Dickey
 * Created:	06 Apr 1989
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

#ifdef	vms
#include "portunix.h"
#else	unix
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
extern	char	*malloc();
extern	char	*realloc();
#define	SUCCESS	0
#endif	vms/unix

#include <errno.h>
extern	FILE	*tmpfile();
extern	int	errno;			/* ...not always in <errno.h> */

#define	AVG_LINE	25		/* nominal line-length */
#define	AMOUNT(n)	(unsigned)(n)

file2argv(name, vec)
char	*name;
char	***vec;
{
	auto	 struct stat sb;
	auto	 FILE	*fp;
	auto	 int	j,
			expected,	/* expected file-size */
			length,
			newlines,	/* number of newlines found */
			lines;		/* number of newlines found */
	auto	 char	**p, *blob;
	register char	*s, *d;

	*vec = 0;	/* in case we find an error */

	if (!strcmp(name, "-")) {
		if (!(fp = tmpfile()))
			return (-1);
		length = 0;
		while ((j = getchar()) != EOF) {
			length++;
			(void)fputc(j, fp);
			if (ferror(fp)) {
				(void)fclose(fp);
				return (-1);
			}
		}
		expected = length;
		(void)rewind(fp);
	} else {
		/*
		 * Determine how large the file is, and allocate enough space
		 * to read it in a single chunk.  Assume a nominal line-size
		 * so that we will cut the average time on realloc.
		 */
		if (stat(name, &sb) < 0)
			return (-1);
		if ((sb.st_mode & S_IFMT) != S_IFREG) {
			errno = EISDIR;
			return (-1);
		}
		length = sb.st_size;
#ifdef	vms
		/* on vms, 'stat()' returns size in terms of blocks */
		expected = length & ~511;
#else	unix
		expected = length;
#endif	vms/unix

		if (!(fp = fopen(name, "r")))
			return (-1);
	}

	/*
	 * We now have a file-pointer open on a file of known length, which
	 * we can read into memory using a single 'malloc()'
	 */
	blob = malloc(AMOUNT(length + (length / AVG_LINE) + 2));
	if (blob == 0) {
		(void)fclose(fp);
		return (-1);
	}
	errno = 0;		/* in case system does not flag actual err */
	length = fread(blob, sizeof(char), length, fp);
	(void)fclose(fp);

	/*
	 * Ensure that we read the entire file.
	 */
	if (length < expected) {
		if (!errno)	errno = EFBIG;
		return (-1);
	}

	/*
	 * Count the number of lines in the file.  Note that the last
	 * "line" may not really have a newline.
	 */
	blob[length]   =
	blob[length+1] = '\0';

	for (s = blob, lines = 0; *s; ++s)
		if (*s == '\n')
			lines++;
	newlines = lines;
	if (length > 0 && blob[length-1] != '\n')
		lines++;

	/*
	 * Reallocate space for the file counting the nulls inserted
	 * after each newline.
	 */
	if (!(blob = realloc(blob, AMOUNT(length + lines + 1))))
		return (-1);

	/*
	 * Allocate space for the vector pointing to the strings
	 */
	*vec = p = (char **)malloc((lines + 1) * sizeof(char *));

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
			free(vec[0]);	/* frees the blob */
			free((char *)vec);
			PRINTF("<EOF>\n");
		} else {
			perror(name);
		}
	}
	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	TEST
