/*
 * Title:	file2mem.c (file to argv-conversion)
 * Author:	T.E.Dickey
 * Created:	11 May 1989
 * Modified:
 *		12 Dec 2014, fix memory leak (coverity).
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
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

#define	ERR_PTYPES
#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: file2mem.c,v 12.13 2025/01/07 00:29:46 tom Exp $")

char *
file2mem(const char *name)
{
    FILE *fp;
    int j;
    size_t expected;		/* expected file-size */
    size_t length;
    off_t offset;
    char *blob;

    if (!strcmp(name, "-")) {
	if ((fp = tmpfile()) == NULL)
	    return (NULL);
	length = 0;
	while ((j = getchar()) != EOF) {
	    length++;
	    (void) fputc(j, fp);
	    if (ferror(fp)) {
		(void) fclose(fp);
		return (NULL);
	    }
	}
	expected = length;
	(void) rewind(fp);
    } else {
	/*
	 * Determine how large the file is, and allocate enough space
	 * to read it in a single chunk.  Assume a nominal line-size
	 * so that we will cut the average time on realloc.
	 */
	if ((offset = filesize(name)) < 0)
	    return (NULL);
	length = (size_t) offset;
#ifdef	vms
	/* on vms, 'stat()' returns size in terms of blocks */
	expected = length & ~511;
#else /* SYS_UNIX */
	expected = length;
#endif /* vms/SYS_UNIX */

	if ((fp = fopen(name, "r")) == NULL)
	    return (NULL);
    }

    /*
     * If the file is too big, we cannot allocate space...
     */
#ifdef	MSDOS
    if (length > 0xffffL) {
	fclose(fp);
	return (0);
    }
#endif

    /*
     * We now have a file-pointer open on a file of known length, which
     * we can read into memory using a single 'malloc()'
     */
    blob = doalloc((char *) 0, (length + 2));
    if (blob == NULL) {
	(void) fclose(fp);
	return (NULL);
    }
    errno = 0;			/* in case system does not flag actual err */
    length = fread(blob, sizeof(char), (size_t) length, fp);
    (void) fclose(fp);

    /*
     * Ensure that we read the entire file.
     */
    if (length < expected) {
	int save = errno;
	free(blob);
	if (!save)
	    errno = EFBIG;
	return (NULL);
    }
    blob[length] =		/* allow length+2 for 'file2argv()' call */
	blob[length + 1] = EOS;

    return (blob);
}

#ifdef	TEST
_MAIN
{
    int j;
    char *blob;
    char *name;

    for (j = 1; j < argc; j++) {
	if ((blob = file2mem(name = argv[j])) != NULL) {
	    PRINTF("file: %s (%d chars)\n", name, strlen(blob));
	    PRINTF("%s<EOF>\n", blob);
	    dofree(blob);
	} else {
	    perror(name);
	}
    }
    (void) exit(SUCCESS);
    /*NOTREACHED */
}
#endif
