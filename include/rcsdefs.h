/* $Id: rcsdefs.h,v 9.8 1991/10/03 10:23:30 dickey Exp $ */

#ifndef	_RCSDEFS_H_
#define	_RCSDEFS_H_

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
			_ar1(char *,	s)
			)
			_dcl(char *,	s)
			_ret

		rcsedit (
			_arx(char *,	where)
			_arx(char *,	old)
			_ar1(char *,	new)
			)
			_dcl(char *,	where)
			_dcl(char *,	old)
			_dcl(char *,	new)
			_nul

		rcsclose(_ar0)
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
			_fn1(int,	f)
			)
			_dcl(char *,	s)
			_dcl(int,	(*f)())
			_ret

	/* rcskeys.c ------------------------------------------------- */
	int	rcskeys(
			_ar1(char *,	s)
			)
			_dcl(char *,	s)
			_ret

	/* rcslast.c ------------------------------------------------- */
		rcslast(
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

		rcsunload(
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
			_ar1(char *,	base)
			)
			_dcl(char *,	path)
			_dcl(char *,	base)
			_ret

	/* rcstemp.c -------------------------------------------------- */
	char *	rcstemp(
			_arx(char *,	name)
			_ar1(int,	copy)
			)
			_dcl(char *,	name)
			_dcl(int,	copy)
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

#endif	/* _RCSDEFS_H_ */
