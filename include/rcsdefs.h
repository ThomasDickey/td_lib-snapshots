/* $Id: rcsdefs.h,v 9.0 1991/05/15 09:00:59 ste_cm Rel $ */

#ifndef	_RCSDEFS_H_
#define	_RCSDEFS_H_

/*
 * State information for scanning RCS-file header
 */
#define	S_HEAD		0
#define	S_ACCESS	1
#define	S_SYMBOLS	2
#define	S_LOCKS		3
#define	S_COMMENT	4
#define	S_STRICT	5

#define	S_VERS		99	/* <version_string>		*/
#define	S_DATE		6
#define	S_AUTHOR	7
#define	S_STATE		8
#define	S_BRANCHES	9
#define	S_NEXT		10

#define	S_DESC		11
#define	S_LOG		12
#define	S_TEXT		13

#define	S_FAIL		999

#define	RCS_DIR		"RCS"
#define	RCS_SUFFIX	",v"

#define	FMT_DATE	"%02d.%02d.%02d.%02d.%02d.%02d"

#define NULL_FUNC (int (*)())0

/*
 * If the environment variable RCS_DEBUG is defined, we activate additional
 * tracing (e.g., in 'rcsedit.c')
 */
#define	RCS_DEBUG	(rcs_debug())

/*
 * Note: since 'rcsload()' uses 'txtalloc()' to allocate the '.revision' and
 * '.parent' strings; we can do equality-comparison simply by comparing the
 * pointers.
 */
#ifdef	S_IFMT
typedef	struct	{
		char	*revision;
		char	*parent;
		time_t	tstamp;		/* checkin time		*/
		int	num_lines;	/* total lines in file	*/
		int	num_added;	/* lines added		*/
		int	num_deleted;	/* lines deleted	*/
	} RCSTREE;
extern	RCSTREE *rcsload();
#endif	/* S_IFMT */

/*
 * Useful external-definitions
 */
extern	char *	getenv();

	/* rcs_debug.c ----------------------------------------------- */
	int	rcs_debug()
			_RET

	/* rcs_dir.c ------------------------------------------------- */
	char *	rcs_dir()
			_RET

	/* rcsedit.c ------------------------------------------------- */
	int	rcsopen(
			_ARX(char *,	name)
			_AR1(int,	show)
			)
			_DCL(char *,	name)
			_DCL(int,	show)
			_RET

	char *	rcsread(
			_AR1(char *,	s)
			)
			_DCL(char *,	s)
			_RET

		rcsedit (
			_ARX(char *,	where)
			_ARX(char *,	old)
			_AR1(char *,	new)
			)
			_DCL(char *,	where)
			_DCL(char *,	old)
			_DCL(char *,	new)
			_NUL

		rcsclose()
			_NUL

	char *	rcsparse_num(
			_ARX(char *,	d)
			_AR1(char *,	s)
			)
			_DCL(char *,	d)
			_DCL(char *,	s)
			_RET

	char *	rcsparse_id(
			_ARX(char *,	d)
			_AR1(char *,	s)
			)
			_DCL(char *,	d)
			_DCL(char *,	s)
			_RET

	char *	rcsparse_str(
			_ARX(char *,	s)
			_FN1(int,	f)
			)
			_DCL(char *,	s)
			_DCL(int,	(*f)())
			_RET

	/* rcskeys.c ------------------------------------------------- */
	int	rcskeys(
			_AR1(char *,	s)
			)
			_DCL(char *,	s)
			_RET

	/* rcslast.c ------------------------------------------------- */
		rcslast(
			_ARX(char *,	wd)
			_ARX(char *,	n)
			_ARX(char **,	v_)
			_ARX(time_t *,	t_)
			_AR1(char **,	l_)
			)
			_DCL(char *,	wd)
			_DCL(char *,	n)
			_DCL(char **,	v_)
			_DCL(time_t *,	t_)
			_DCL(char **,	l_)
			_NUL

	/* rcsload.c ------------------------------------------------- */
	RCSTREE *rcsload(
			_ARX(char *,	name)
			_AR1(int,	verbose)
			)
			_DCL(char *,	name)
			_DCL(int,	verbose)
			_RET

		rcsunload(
			_AR1(RCSTREE *,	p)
			)
			_DCL(RCSTREE *,	p)
			_NUL

	/* rcslocks.c ------------------------------------------------ */
	char *	rcslocks(
			_ARX(char *,	s)
			_ARX(char *,	who)
			_AR1(char *,	rev)
			)
			_DCL(char *,	s)
			_DCL(char *,	who)
			_DCL(char *,	rev)
			_RET

	/* rcsname.c -------------------------------------------------- */
	char *	rcs2name(
			_ARX(char *,	name)
			_AR1(int,	full)
			)
			_DCL(char *,	name)
			_DCL(int,	full)
			_RET

	char *	name2rcs(
			_ARX(char *,	name)
			_AR1(int,	full)
			)
			_DCL(char *,	name)
			_DCL(int,	full)
			_RET

	/* rcspath.c -------------------------------------------------- */
	char *	rcspath(
			_AR1(char *,	name)
			)
			_DCL(char *,	name)
			_RET

	/* rcspermit.c ------------------------------------------------ */
	int	rcspermit(
			_ARX(char *,	path)
			_AR1(char *,	base)
			)
			_DCL(char *,	path)
			_DCL(char *,	base)
			_RET

	/* rcstemp.c -------------------------------------------------- */
	char *	rcstemp(
			_ARX(char *,	name)
			_AR1(int,	copy)
			)
			_DCL(char *,	name)
			_DCL(int,	copy)
			_RET

	/* vcs_file.c ------------------------------------------------- */
	char *	vcs_file(
			_ARX(char *,	path)
			_ARX(char *,	name)
			_AR1(int,	temp)
			)
			_DCL(char *,	path)
			_DCL(char *,	name)
			_DCL(int,	temp)
			_RET

#endif	/* _RCSDEFS_H_ */
