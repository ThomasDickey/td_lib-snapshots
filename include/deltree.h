/* $Id: deltree.h,v 12.1 1994/06/25 14:16:23 tom Exp $ */

#ifndef	DELTREE_H
#define	DELTREE_H

#ifndef	_arx
#include "ptypes.h"
#endif

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
			_arx(char **,	names)
			_arx(int,	load)
			_ar1(int,	verbose)
			)
			_dcl(char **,	names)
			_dcl(int,	load)
			_dcl(int,	verbose)
			_ret

	void	diffunload(
			_ar1(DELTREE *,	p)
			)
			_dcl(DELTREE *,	p)
			_nul

#endif	/* DELTREE_H */
