/* $Id: deltree.h,v 10.0 1991/09/17 07:34:13 ste_cm Rel $ */

#ifndef	_DELTREE_H_
#define	_DELTREE_H_

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

#endif	/* _DELTREE_H_ */
