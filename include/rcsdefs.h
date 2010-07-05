/* $Id: rcsdefs.h,v 12.12 2010/07/05 11:09:27 tom Exp $ */

#ifndef	RCSDEFS_H
#define	RCSDEFS_H

#define	RCS_VERSION	5	/* current version of rcs we are built over */
			/* version 5 defines "-M" option of 'ci' */
			/* version 4 defines "-d" option of 'ci' */

/*
 * State information for scanning RCS-file header
 */
#define	S_HEAD		0
#define	S_ACCESS	1
#define	S_BRANCH	20	/* (from rcs 3.0) */
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

/* RCS uses 2-digit dates internally for files before 2000, 4-digits after */
#define	FMT_DATE	"%d.%02d.%02d.%02d.%02d.%02d"

#define NULL_FUNC (int (*)())0

typedef	void	(*RcsparseStr)(int);

/*
 * If the environment variable RCS_DEBUG is defined, we activate additional
 * tracing (e.g., in 'rcsedit.c')
 */
#define	RCS_DEBUG	(rcs_debug())

/*
 * Useful external-definitions
 */
#include <deltree.h>

	/* rcs_debug.c ----------------------------------------------- */
	int	rcs_debug(void)
			;

	/* rcs_dir.c ------------------------------------------------- */
	char *	rcs_dir(
			const char *	working_dir,
			const char *	pathname
			)
			;

	/* rcsargpair.c ---------------------------------------------- */
	int	rcs_working(
			char *	name,
			Stat_t *	sb
			)
			;

	int	rcs_archive(
			char *	name,
			Stat_t *	sb
			)
			;

	int	rcs_located(
			char *	name,
			Stat_t *	sb
			)
			;

	int	rcsargpair(
			int	item,
			int	argc,
			char **	argv
			)
			;

	/* rcsedit.c ------------------------------------------------- */
	int	rcsopen(
			const char *	name,
			int		show,
			int		readonly
			)
			;

	char *	rcsread(
			char *	s,
			int	code
			)
			;

	void	rcsedit (
			char *	oldname,
			char *	newname
			)
			;

	void	rcsclose(void)
			;

	char *	rcsparse_num(
			char *	d,
			char *	s
			)
			;

	char *	rcsparse_id(
			char *	d,
			char *	s
			)
			;

	char *	rcsparse_str(
			char *	s,
			RcsparseStr	f
			)
			;

	/* rcskeys.c ------------------------------------------------- */
	int	rcskeys(
			char *	s
			)
			;

	/* rcslast.c ------------------------------------------------- */
	void	rcslast(
			const char *	wd,
			const char *	n,
			const char **	v_,
			time_t *	t_,
			const char **	l_
			)
			;

	/* rcsload.c ------------------------------------------------- */
	DELTREE *rcsload(
			char *	name,
			int	full,
			int	load,
			int	verbose
			)
			;

	void	rcsunload(
			DELTREE *	p
			)
			;

	/* rcslocks.c ------------------------------------------------ */
	char *	rcslocks(
			char *	s,
			char *	who,
			char *	rev
			)
			;

	/* rcsname.c -------------------------------------------------- */
	char *	rcs2name(
			const char *	name,
			int		full
			)
			;

	char *	name2rcs(
			const char *	name,
			int		full
			)
			;

	/* rcspath.c -------------------------------------------------- */
	char *	rcspath(
			const char *	name
			)
			;

	/* rcspermit.c ------------------------------------------------ */
	int	rcspermit(
			const char *	path,
			char *		base,
			const char **	accflag
			)
			;

	/* rcssymbols.c ---------------------------------------------- */
	char *	rcssymbols(
			char *		s,
			char *		dst,
			const char *	src
			)
			;

	/* rcstemp.c -------------------------------------------------- */
	char *	rcstemp(
			char *	name,
			int	copy
			)
			;

	/* rcstime.c -------------------------------------------------- */
	void	time2rcs(
			char *	buffer,
			time_t	when
			)
			;

	time_t	rcs2time(
			char *	buffer
			)
			;

	/* samebranch.c ----------------------------------------------- */
	int	samebranch(
			const char *	rev1,
			const char *	rev2
			)
			;

	/* vcs_file.c ------------------------------------------------- */
	char *	vcs_file(
			const char *	path,
			char *		name,
			int		temp
			)
			;

#ifdef CVS_PATH
	/* cvslast.c -------------------------------------------------- */
	void	cvslast(
			const char *	wd,
			const char *	n,
			const char **	v_,
			time_t *	t_,
			const char **	l_
			)
			;

#endif
#ifdef SVN_PATH
	/* svnlast.c -------------------------------------------------- */
	void	svnlast(
			const char *	wd,
			const char *	n,
			const char **	v_,
			time_t *	t_,
			const char **	l_
			)
			;

#endif

#endif	/* RCSDEFS_H */
