#ifndef	lint
static	char	Id[] = "$Id: filecopy.c,v 9.0 1991/05/15 10:02:18 ste_cm Rel $";
#endif

/*
 * Title:	filecopy.c (copy a file)
 * Author:	T.E.Dickey
 * Created:	30 Aug 1988
 * $Log: filecopy.c,v $
 * Revision 9.0  1991/05/15 10:02:18  ste_cm
 * BASELINE Mon Jun 10 10:09:56 1991 -- apollo sr10.3
 *
 *		Revision 8.1  91/05/15  10:02:18  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 8.0  89/07/25  08:58:40  ste_cm
 *		BASELINE Mon Aug 13 15:06:41 1990 -- LINCNT, ADA_TRANS
 *		
 *		Revision 7.0  89/07/25  08:58:40  ste_cm
 *		BASELINE Mon Apr 30 09:54:01 1990 -- (CPROTO)
 *		
 *		Revision 6.0  89/07/25  08:58:40  ste_cm
 *		BASELINE Thu Mar 29 07:37:55 1990 -- maintenance release (SYNTHESIS)
 *		
 *		Revision 5.0  89/07/25  08:58:40  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.0  89/07/25  08:58:40  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 3.1  89/07/25  08:58:40  dickey
 *		recompiled with apollo SR10 -- mods for function prototypes
 *		
 *		Revision 3.0  88/09/13  08:57:27  ste_cm
 *		BASELINE Mon Jun 19 13:27:01 EDT 1989
 *		
 *		Revision 2.0  88/09/13  08:57:27  ste_cm
 *		BASELINE Thu Apr  6 09:45:13 EDT 1989
 *		
 *		Revision 1.4  88/09/13  08:57:27  dickey
 *		sccs2rcs keywords
 *		
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

#include	"ptypes.h"
#include	<sys/file.h>
#include	<sys/signal.h>
#include	<errno.h>
extern	int	errno;

filecopy(src, dst, copy)
char	*src;
char	*dst;
int	copy;
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

	catchall(SIG_IGN);
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
	catchall(SIG_DFL);

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
#endif
