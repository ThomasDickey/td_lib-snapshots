/* $Id: dyn_str.h,v 12.3 2004/03/07 21:46:52 tom Exp $
 *
 *	Functions which manage dynamic-strings.
 */

#ifndef	DYN_STR_H
#define	DYN_STR_H

typedef	struct	{
	char	*text;
	size_t	cur_length,
		max_length;
	} DYN;

#define	APPEND	(void) dyn_append
#define	CATARG	(void) dyn_catarg
#define	CATARG2	(void) dyn_catarg2

	/* dyn_append.c ----------------------------------------------- */
	DYN *	dyn_append(
		DYN *	p,
		char *	s
		)
		;

	/* dyn_append_c.c --------------------------------------------- */
	DYN *	dyn_append_c(
		DYN *	p,
		int	c
		)
		;

	/* dyn_catarg.c ----------------------------------------------- */
	DYN *	dyn_catarg(
		DYN *	p,
		char *	arg
		)
		;

	DYN *	dyn_catarg2(
		DYN *	p,
		char *	opt,
		char *	value
		)
		;

	/* dyn_copy.c ------------------------------------------------- */
	DYN *	dyn_copy(
		DYN *	p,
		char *	s
		)
		;

	/* dyn_string.c ----------------------------------------------- */
	DYN *	dyn_alloc(
		DYN *	p,
		size_t	len
		)
		;

	DYN *	dyn_free(
		DYN *	p
		)
		;

	void	dyn_init(
		DYN **	p,
		size_t	len
		)
		;

	char *	dyn_string(
		DYN *	p
		)
		;

	size_t	dyn_length(
		DYN *	p
		)
		;

#endif	/* DYN_STR_H */
