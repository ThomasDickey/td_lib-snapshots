#ifndef	NO_IDENT
#endif

/*
 * Title:	fileblox.c (file-blocks total)
 * Author:	T.E.Dickey
 * Created:	13 Jul 1994
 * Modified:
 *
 * Function:	returns the total number of blocks used by a file, counting
 *		both direct and indirect blocks.
 *
 *		The algorithm for computing the number of blocks of a SYS5 file
 *		is derived from the discussion on pages 64-72 of "The Design of
 *		the UNIX Operating System", by Maurice J.  Bach, Prentice-Hall
 *		1986.
 */

#include <ptypes.h>
#undef STAT_HAS_ST_BLOCKS	/* patch */
#undef fileblocks		/* patch */

#ifdef unix
#if !STAT_HAS_ST_BLOCKS

#define	BSIZE		1024	/* size of indirect-block, in bytes */
#define	ISIZE		(BSIZE/sizeof(long))
#define NDIR		10	/* # of direct-block addresses in inode */

#define	frac(a,b)	((a) + (b) - 1) / (b)

long	fileblocks (
	_AR1(Stat_t *,	sb))
	_DCL(Stat_t *,	sb)
{
	register long	blocks = frac(sb->st_size, BSIZE);
	register long	c = blocks - NDIR;

	if (c > (ISIZE*ISIZE)) {
		/* 1 triple with up to ISIZE double indirect */
		blocks += (1)
			+ (1 + frac((c - (1+ISIZE)), ISIZE))
			+ (1 + frac((c - (1+ISIZE+ISIZE*ISIZE)), ISIZE*ISIZE));
	} else if (c > ISIZE) {
		/* 1 double with up to ISIZE indirect */
		blocks += (1)
			+ (1 + frac((c - (1+ISIZE)), ISIZE));
	} else if (c > 0) {
		/* 1 indirect with up to ISIZE direct */
		blocks += (1);
	}
	return (blocks);
}
#endif /* !STAT_HAS_ST_BLOCKS */
#endif	/* unix */
