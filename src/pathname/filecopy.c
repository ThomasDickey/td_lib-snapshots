#ifndef	lint
static	char	sccs_id[] = "@(#)filecopy.c	1.2 88/08/30 16:11:21";
#endif	lint

/*
 * Title:	filecopy.c (copy a file)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		30 Aug 1988, redundant 'chmod()' is needed on Apollo SR9.7 if
 *			     we create a file with 0444 mode.
 *
 * Function:	Copy a file, setting the destination's mode and modification
 *		time from the source.
 *
 * Arguments:	src	- the name of the source file.
 *		dst	- the name of the destination file.
 *		copy	- TRUE if we copy contents of the file.  This flag is
 *			  set FALSE when we simply want a dummy name (cf: the
 *			  setuid hack for 'checkout').
 *
 * Returns:	-1 if no copy was made, with errno set.  We set the following
 *		codes:
 *
 *		EISDIR - either source or destination exists, but is not a file
 *		EEXIST - the source and destination are identical.
 */

#include	"ptypes.h"
#include	<sys/file.h>
#include	<errno.h>
extern	int	errno;

filecopy(src, dst, copy)
char	*src;
char	*dst;
{
	struct	stat	sb1, sb2;
	register int	count,
			fi,
			fo;

	errno = 0;		/* reset so we can use as return-code */
	if (stat(src, &sb1) < 0) {
		if (copy)
			return (-1);
		else {		/* ...make destination non-existent too */
			(void)unlink(dst);
			return (0);
		}
	}
	if ((sb1.st_mode & S_IFMT) != S_IFREG) {
		errno = EISDIR;
		return (-1);
	}
	if (stat(dst, &sb2) >= 0) {
		if ((sb1.st_mode & S_IFMT) != S_IFREG) {
			errno = EISDIR;
			return (-1);
		}
		if ((sb1.st_dev == sb2.st_dev)
		&&  (sb1.st_ino == sb2.st_ino)) {
			errno = EEXIST;
			return (-1);
		}
	}

	if ((fi = open(src, O_RDONLY, 0)) >= 0) {
		int	mode = sb1.st_mode & 0777;
		if (((unlink(dst) >= 0) || (errno == ENOENT))
		&&  ((fo = open(dst, O_CREAT | O_WRONLY, mode)) >= 0)) {

			errno = 0;	/* reset after 'unlink()' */
			if (copy) {
				char	bfr[BUFSIZ];

				while ((count = read(fi, bfr, sizeof(bfr))) > 0)
					if (write(fo, bfr, count) < 0)
						break;
			}
			(void)close(fo);
			(void)chmod(dst, mode);
			(void)setmtime(dst, sb1.st_mtime);
		}
		(void)close(fi);
	}

	return (errno ? -1 : 0);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	char	*src, *dst;

	if (argc > 2) {
		printf("src=%s, dst=%s\n", src = argv[1], dst = argv[2]);
		if (filecopy(src, dst, TRUE) < 0)
			perror("copy");
	}
}
#endif	TEST
