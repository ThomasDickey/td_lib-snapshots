#if	!defined(NO_IDENT)
static	char	Id[] = "$Id: filecopy.c,v 12.5 1994/07/16 15:23:47 tom Exp $";
#endif

/*
 * Title:	filecopy.c (copy a file)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * Modified:
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		04 Oct 1991, conversion to ANSI
 *		12 Sep 1991, removed redundant def for 'errno' (VMS C 3.2)
 *		13 Sep 1988, use 'catchall()'
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

#define	OPN_PTYPES
#define	SIG_PTYPES
#include	"ptypes.h"
#include	<errno.h>

int	filecopy(
	_ARX(char *,	src)
	_ARX(char *,	dst)
	_AR1(int,	copy)
		)
	_DCL(char *,	src)
	_DCL(char *,	dst)
	_DCL(int,	copy)
{
	Stat_t	sb1, sb2;
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

	catchall(SIG_IGN);
	if ((fi = open(src, O_RDONLY, 0)) >= 0) {
		int	mode = sb1.st_mode & 0777;
		if (((unlink(dst) >= 0) || (errno == ENOENT))
		&&  ((fo = open(dst, O_CREAT | O_WRONLY, mode)) >= 0)) {

			errno = 0;	/* reset after 'unlink()' */
			if (copy) {
				char	bfr[BUFSIZ];

				while ((count = read(fi, bfr, sizeof(bfr))) > 0)
					if (write(fo, bfr, (LEN_READ)count) < 0)
						break;
			}
			(void)close(fo);
			(void)chmod(dst, (mode_t)mode);
			(void)setmtime(dst, sb1.st_mtime);
		}
		(void)close(fi);
	}
	catchall(SIG_DFL);

	return (errno ? -1 : 0);
}

#ifdef	TEST
_MAIN
{
	char	*src, *dst;

	if (argc > 2) {
		PRINTF("src=%s, dst=%s\n", src = argv[1], dst = argv[2]);
		if (filecopy(src, dst, TRUE) < 0)
			perror("copy");
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
