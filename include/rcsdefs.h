/* $Id: rcsdefs.h,v 12.3 1994/06/25 14:15:37 tom Exp $ */

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

#define	FMT_DATE	"%02d.%02d.%02d.%02d.%02d.%02d"

#define NULL_FUNC (int (*)())0

typedef	void	(*RcsparseStr)(
#if PROTOTYPES
			int	c
#endif
		);

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
	int	rcs_debug(_ar0)
			_ret

	/* rcs_dir.c ------------------------------------------------- */
	char *	rcs_dir(_ar0)
			_ret

	/* rcsargpair.c ---------------------------------------------- */
	int	rcs_working(
			_arx(char *,	name)
			_ar1(Stat_t *,	sb)
			)
			_dcl(char *,	name)
			_dcl(Stat_t *,	sb)
			_ret

	int	rcs_archive(
			_arx(char *,	name)
			_ar1(Stat_t *,	sb)
			)
			_dcl(char *,	name)
			_dcl(Stat_t *,	sb)
			_ret

	int	rcs_located(
			_arx(char *,	name)
			_ar1(Stat_t *,	sb)
			)
			_dcl(char *,	name)
			_dcl(Stat_t *,	sb)
			_ret

	int	rcsargpair(
			_arx(int,	this)
			_arx(int,	argc)
			_ar1(char **,	argv)
			)
			_dcl(int,	this)
			_dcl(int,	argc)
			_dcl(char **,	argv)
			_ret

	/* rcsedit.c ------------------------------------------------- */
	int	rcsopen(
			_arx(char *,	name)
			_arx(int,	show)
			_ar1(int,	readonly)
			)
			_dcl(char *,	name)
			_dcl(int,	show)
			_dcl(int,	readonly)
			_ret

	char *	rcsread(
			_arx(char *,	s)
			_ar1(int,	code)
			)
			_dcl(char *,	s)
			_dcl(int,	code)
			_ret

	void	rcsedit (
			_arx(char *,	old)
			_ar1(char *,	new)
			)
			_dcl(char *,	old)
			_dcl(char *,	new)
			_nul

	void	rcsclose(_ar0)
			_nul

	char *	rcsparse_num(
			_arx(char *,	d)
			_ar1(char *,	s)
			)
			_dcl(char *,	d)
			_dcl(char *,	s)
			_ret

	char *	rcsparse_id(
			_arx(char *,	d)
			_ar1(char *,	s)
			)
			_dcl(char *,	d)
			_dcl(char *,	s)
			_ret

	char *	rcsparse_str(
			_arx(char *,	s)
			_ar1(RcsparseStr,f)
			)
			_dcl(char *,	s)
			_dcl(RcsparseStr,f)
			_ret

	/* rcskeys.c ------------------------------------------------- */
	int	rcskeys(
			_ar1(char *,	s)
			)
			_dcl(char *,	s)
			_ret

	/* rcslast.c ------------------------------------------------- */
	void	rcslast(
			_arx(char *,	wd)
			_arx(char *,	n)
			_arx(char **,	v_)
			_arx(time_t *,	t_)
			_ar1(char **,	l_)
			)
			_dcl(char *,	wd)
			_dcl(char *,	n)
			_dcl(char **,	v_)
			_dcl(time_t *,	t_)
			_dcl(char **,	l_)
			_nul

	/* rcsload.c ------------------------------------------------- */
	DELTREE *rcsload(
			_arx(char *,	name)
			_arx(int,	full)
			_arx(int,	load)
			_ar1(int,	verbose)
			)
			_dcl(char *,	name)
			_dcl(int,	full)
			_dcl(int,	load)
			_dcl(int,	verbose)
			_ret

	void	rcsunload(
			_ar1(DELTREE *,	p)
			)
			_dcl(DELTREE *,	p)
			_nul

	/* rcslocks.c ------------------------------------------------ */
	char *	rcslocks(
			_arx(char *,	s)
			_arx(char *,	who)
			_ar1(char *,	rev)
			)
			_dcl(char *,	s)
			_dcl(char *,	who)
			_dcl(char *,	rev)
			_ret

	/* rcsname.c -------------------------------------------------- */
	char *	rcs2name(
			_arx(char *,	name)
			_ar1(int,	full)
			)
			_dcl(char *,	name)
			_dcl(int,	full)
			_ret

	char *	name2rcs(
			_arx(char *,	name)
			_ar1(int,	full)
			)
			_dcl(char *,	name)
			_dcl(int,	full)
			_ret

	/* rcspath.c -------------------------------------------------- */
	char *	rcspath(
			_ar1(char *,	name)
			)
			_dcl(char *,	name)
			_ret

	/* rcspermit.c ------------------------------------------------ */
	int	rcspermit(
			_arx(char *,	path)
			_arx(char *,	base)
			_ar1(char **,	accflag)
			)
			_dcl(char *,	path)
			_dcl(char *,	base)
			_dcl(char **,	accflag)
			_ret

	/* rcssymbols.c ---------------------------------------------- */
	char *	rcssymbols(
			_arx(char *,	s)
			_arx(char *,	dst)
			_ar1(char *,	src)
			)
			_dcl(char *,	s)
			_dcl(char *,	dst)
			_dcl(char *,	src)
			_ret

	/* rcstemp.c -------------------------------------------------- */
	char *	rcstemp(
			_arx(char *,	name)
			_ar1(int,	copy)
			)
			_dcl(char *,	name)
			_dcl(int,	copy)
			_ret

	/* rcstime.c -------------------------------------------------- */
	void	time2rcs(
			_arx(char *,	buffer)
			_ar1(time_t,	when)
			)
			_dcl(char *,	buffer)
			_dcl(time_t,	when)
			_nul

	time_t	rcs2time(
			_ar1(char *,	buffer)
			)
			_dcl(char *,	buffer)
			_ret

	/* samebranch.c ----------------------------------------------- */
	int	samebranch(
			_arx(char *,	rev1)
			_ar1(char *,	rev2)
			)
			_dcl(char *,	rev1)
			_dcl(char *,	rev2)
			_ret

	/* vcs_file.c ------------------------------------------------- */
	char *	vcs_file(
			_arx(char *,	path)
			_arx(char *,	name)
			_ar1(int,	temp)
			)
			_dcl(char *,	path)
			_dcl(char *,	name)
			_dcl(int,	temp)
			_ret

#endif	/* RCSDEFS_H */
