/* $Id: deltree.h,v 12.3 2004/03/07 21:46:48 tom Exp $ */

#ifndef	DELTREE_H
#define	DELTREE_H

/*
 * This structure contains a list of all deltas loaded via 'rcsload()'.
 *
 * Note: since 'rcsload()' uses 'txtalloc()' to allocate the '.revision' and
 * '.parent' strings; we can do equality-comparison simply by comparing the
 * pointers.
 */
typedef	struct	{
		char	*revision;
		char	*author;
		char	*parent;
		time_t	tstamp;		/* checkin time		*/
		int	num_lines;	/* total lines in file	*/
		int	num_added;	/* lines added		*/
		int	num_deleted;	/* lines deleted	*/
		char	*buffer;	/* loaded-file (raw)	*/
		char	**vector;	/* pointers to lines	*/
		char	*logged;	/* history-comment	*/
	} DELTREE;

	DELTREE *diffload(
			char **	names,
			int	load,
			int	verbose
			)
			;

	void	diffunload(
			DELTREE *	p
			)
			;

#endif	/* DELTREE_H */
