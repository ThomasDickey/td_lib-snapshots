#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/file2mem.c,v 2.1 1989/05/11 12:19:58 dickey Exp $";
#endif	lint

/*
 * Title:	file2mem.c (file to argv-conversion)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 *
 * Function:	Reads a file into memory, returning a pointer to the result.
 *
 * Arguments:	name -	the name of the file to read from.  If "-", then we
 *			read from the standard input.
 *
 * Returns:	a pointer to the file, as loaded into memory (or null if an
 *		error occurred).
 */

#include "ptypes.h"
#include <errno.h>
extern	FILE	*tmpfile();
extern	int	errno;			/* ...not always in <errno.h> */

char	*
file2mem(name)
char	*name;
{
	auto	 struct stat sb;
	auto	 FILE	*fp;
	auto	 int	j,
			expected,	/* expected file-size */
			length;
	auto	 char	*blob;

	if (!strcmp(name, "-")) {
		if (!(fp = tmpfile()))
			return (0);
		length = 0;
		while ((j = getchar()) != EOF) {
			length++;
			(void)fputc(j, fp);
			if (ferror(fp)) {
				(void)fclose(fp);
				return (0);
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
			return (0);
		if ((sb.st_mode & S_IFMT) != S_IFREG) {
			errno = EISDIR;
			return (0);
		}
		length = sb.st_size;
#ifdef	vms
		/* on vms, 'stat()' returns size in terms of blocks */
		expected = length & ~511;
#else	unix
		expected = length;
#endif	vms/unix

		if (!(fp = fopen(name, "r")))
			return (0);
	}

	/*
	 * We now have a file-pointer open on a file of known length, which
	 * we can read into memory using a single 'malloc()'
	 */
	blob = malloc((unsigned)(length + 2));
	if (blob == 0) {
		(void)fclose(fp);
		return (0);
	}
	errno = 0;		/* in case system does not flag actual err */
	length = fread(blob, sizeof(char), length, fp);
	(void)fclose(fp);

	/*
	 * Ensure that we read the entire file.
	 */
	if (length < expected) {
		if (!errno)	errno = EFBIG;
		return (0);
	}
	blob[length] =		/* allow length+2 for 'file2argv()' call */
	blob[length+1] = EOS;

	return (blob);
}


#ifdef	TEST
#define	PRINTF	(void)printf
main(argc, argv)
char	*argv[];
{
	register int	j, k;
	auto	 int	lines;
	auto	 char	*blob;
	auto	 char	*name;

	for (j = 1; j < argc; j++) {
		if (blob = file2mem(name = argv[j])) {
			PRINTF("file: %s (%d chars)\n", name, strlen(blob));
			PRINTF("%s<EOF>\n", blob);
			free(blob);
		} else {
			perror(name);
		}
	}
	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	TEST
