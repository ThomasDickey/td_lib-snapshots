#ifndef	lint
static	char	Id[] = "$Id: file2mem.c,v 12.0 1992/09/02 15:19:51 ste_cm Rel $";
#endif

/*
 * Title:	file2mem.c (file to argv-conversion)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 * Modified:
 *		04 Oct 1991, conversion to ANSI
 *		13 Sep 1991, use 'filesize()'
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *
 * Function:	Reads a file into memory, returning a pointer to the result.
 *
 * Arguments:	name -	the name of the file to read from.  If "-", then we
 *			read from the standard input.
 *
 * Returns:	a pointer to the file, as loaded into memory (or null if an
 *		error occurred).
 */

#define	STR_PTYPES
#include "ptypes.h"
#include <errno.h>
extern	FILE	*tmpfile();

char	*
file2mem(
_AR1(char *,	name))
_DCL(char *,	name)
{
	auto	 FILE	*fp;
	auto	 int	j;
	auto	 off_t	expected,	/* expected file-size */
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
		if ((length = filesize(name)) < 0)
			return (0);
#ifdef	vms
		/* on vms, 'stat()' returns size in terms of blocks */
		expected = length & ~511;
#else	/* unix */
		expected = length;
#endif	/* vms/unix */

		if (!(fp = fopen(name, "r")))
			return (0);
	}

	/*
	 * We now have a file-pointer open on a file of known length, which
	 * we can read into memory using a single 'malloc()'
	 */
	blob = doalloc((char *)0, (unsigned)(length + 2));
	if (blob == 0) {
		(void)fclose(fp);
		return (0);
	}
	errno = 0;		/* in case system does not flag actual err */
	length = fread(blob, sizeof(char), (LEN_FREAD)length, fp);
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
_MAIN
{
	register int	j, k;
	auto	 int	lines;
	auto	 char	*blob;
	auto	 char	*name;

	for (j = 1; j < argc; j++) {
		if (blob = file2mem(name = argv[j])) {
			PRINTF("file: %s (%d chars)\n", name, strlen(blob));
			PRINTF("%s<EOF>\n", blob);
			dofree(blob);
		} else {
			perror(name);
		}
	}
	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
