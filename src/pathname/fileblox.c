/*
 * Title:	fileblox.c (file-blocks total)
 * Author:	T.E.Dickey
 * Created:	13 Jul 1994
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *
 * Function:	returns the total number of blocks used by a file, counting
 *		both direct and indirect blocks.
 *
 *		The algorithm for computing the number of blocks of a SYS5 file
 *		is derived from the discussion on pages 64-72 of "The Design of
 *		the UNIX Operating System", by Maurice J.  Bach, Prentice-Hall
 *		1986.
 */
#define ERR_PTYPES
#include <ptypes.h>

MODULE_ID("$Id: fileblox.c,v 12.11 2014/07/22 13:51:29 tom Exp $")

#ifdef SYS_UNIX
#if !defined(STAT_HAS_ST_BLOCKS)

#ifdef __hpux			/* incompatible def */
#undef  NINDIR
#undef  BSIZE
#endif

#ifndef	NINDIR			/* should be in <sys/param.h> */
#define	BSIZE		1024	/* size of indirect-block, in bytes */
#define	NINDIR		(BSIZE/sizeof(daddr_t))
#endif

#define NDIR		10	/* # of direct-block addresses in inode */

#define	frac(a,b)	((a) + (b) - 1) / (b)

long
fileblocks(const Stat_t * sb)
{
    long bytes = sb->st_size;
    long blocks = frac(bytes, 512);
    long c = blocks - NDIR;

    if (c > 0) {
	blocks += frac(c, NINDIR);
	if (c > NINDIR) {
	    blocks++;
	    c -= (NINDIR * NINDIR);
	    if (c > 0) {
		blocks += 1 + frac(c, NINDIR);
	    }
	}
    }
    return (blocks);
}
#else
void dummy_fileblox(void);
void
dummy_fileblox(void)
{
}
#endif /* !STAT_HAS_ST_BLOCKS */
#endif /* SYS_UNIX */

#ifdef	TEST
static void
do_file(char *name)
{
    Stat_t sb;
    if (stat_file(name, &sb) >= 0) {
	printf("%8lu >%8ld %s\n",
	       (unsigned long) sb.st_size,
	       (long) fileblocks(&sb),
	       name);
    } else if (errno != EISDIR) {
	perror(name);
    }
}

_MAIN
{
    int n;
    if (argc > 1) {
	for (n = 0; n < argc; n++)
	    do_file(argv[n]);
    } else {
	char buffer[BUFSIZ];
	while (fgets(buffer, sizeof(buffer), stdin)) {
	    strtrim(buffer);
	    do_file(buffer);
	}
    }
    exit(EXIT_SUCCESS);
}
#endif /* TEST */
