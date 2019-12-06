/* $Id: dyn_str.h,v 12.6 2019/12/06 01:06:59 tom Exp $
 *
 *	Functions which manage dynamic-strings.
 */

#ifndef	DYN_STR_H
#define	DYN_STR_H
/* *INDENT-OFF* */

typedef	struct	{
	char	*text;
	size_t	cur_length,
		max_length;
	} DYN;

#ifdef ARGV_INTERNAL
struct _argv ARGV;
#else
typedef struct {
	size_t	cur_length;
	size_t	max_length;
	DYN *	the_command;
	DYN **	the_strings;
	char **	the_array;
} ARGV;
#endif

#define	APPEND	(void) dyn_append
#define	CATARG	(void) dyn_catarg
#define	CATARG2	(void) dyn_catarg2

	/* dyn_app.c -------------------------------------------------- */
	DYN *	dyn_append(
		DYN *	p,
		const char *	s
		)
		;

	/* dyn_appc.c ------------------------------------------------- */
	DYN *	dyn_append_c(
		DYN *	p,
		int	c
		)
		;

	/* dyn_argv.c ------------------------------------------------- */
	ARGV *	argv_init(void)
		;

	ARGV *	argv_init1(
		const char *command
		)
		;

	ARGV *	argv_init2(
		const char *command,
		const char *executable
		)
		;

	void	argv_append(
	    	ARGV **	p,
		const char *value
		)
		;

	void	argv_append2(
	    	ARGV **	p,
		const char *option,
		const char *value
		)
		;

	ARGV *	argv_merge(
	    	ARGV **	target,
		ARGV *	source
		)
		;

	char **	argv_values(
		ARGV *	p
		)
		;

	DYN *	argv_flatten(
		ARGV *	p
		)
		;

	void	argv_free(
		ARGV **	p
		)
		;

	/* dyn_cat.c -------------------------------------------------- */
	DYN *	dyn_catarg(
		DYN *	p,
		const char *	arg
		)
		;

	DYN *	dyn_catarg2(
		DYN *	p,
		const char *	opt,
		const char *	value
		)
		;

	/* dyn_copy.c ------------------------------------------------- */
	DYN *	dyn_copy(
		DYN *	p,
		const char *	s
		)
		;

	/* dyn_str.c -------------------------------------------------- */
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

/* *INDENT-ON* */
#endif	/* DYN_STR_H */
