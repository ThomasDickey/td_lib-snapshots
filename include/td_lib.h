/* $Id: td_lib.h,v 12.0 1993/04/26 15:30:01 ste_cm Rel $ */

/*
 * Combined lint-library/function prototype definitions for TD_LIB common
 * library.
 */

#ifndef		_COMMON_H_
#define		_COMMON_H_
#ifndef		_PTYPES_
#include "ptypes.h"
#endif		/* _PTYPES_ */

	/* MAIN-PROGRAM *********************************************** */
		failed(
			_ar1(char *,	s)
			)
			_dcl(char *,	s)
			_nul

	/* GENERIC-LIBRARY ******************************************** */

#ifdef	CUR_PTYPES
#include "td_curse.h"
#endif

	/* abspath.c -------------------------------------------------- */
#ifdef	unix
		abshome(
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_nul

		abspath(
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_nul
#endif	/* unix */

	/* acc_mode.c ------------------------------------------------- */
	char *	access_mode(
			_ar1(int,	mode)
			)
			_dcl(int,	mode)
			_ret

	/* add2fnam.c ------------------------------------------------- */
	int	add2fname(
			_arx(char *,	fname)
			_ar1(char *,	suffix)
			)
			_dcl(char *,	fname)
			_dcl(char *,	suffix)
			_ret

	/* arg2file.c ------------------------------------------------- */
	int	argv2file(
			_arx(char **,	vec)
			_arx(char *,	name)
			_ar1(char *,	mode)
			)
			_dcl(char **,	vec)
			_dcl(char *,	name)
			_dcl(char *,	mode)
			_ret

	/* bldarg.c --------------------------------------------------- */
		bldarg (
			_arx(int,	argc)
			_arx(char **,	argv)
			_ar1(char *,	string)
			)
			_dcl(int,	argc)
			_dcl(char **,	argv)
			_dcl(char *,	string)
			_nul

	/* catarg.c --------------------------------------------------- */
		catarg (
			_arx(char *,	dst)
			_ar1(char *,	src)
			)
			_dcl(char *,	dst)
			_dcl(char *,	src)
			_nul

	void	catarg2 (
			_arx(char *,	dst)
			_arx(char *,	opt)
			_ar1(char *,	arg)
			)
			_dcl(char *,	dst)
			_dcl(char *,	opt)
			_dcl(char *,	arg)
			_nul

	/* catchall.c ------------------------------------------------- */
#ifdef	SIG_PTYPES
		catchall (
			_fn1(SIG_T,	func,	(/*patch:SIG_T?*/))
			)
			_dcl(SIG_T,	(*func)())
			_nul
#endif

	/* copyback.c ------------------------------------------------- */
	int	copyback(
			_arx(FILE *,	fp)
			_arx(char *,	name)
			_arx(int,	mode)
			_ar1(int,	lines)
			)
			_dcl(FILE *,	fp)
			_dcl(char *,	name)
			_dcl(int,	mode)
			_dcl(int,	lines)
			_ret

	/* cutoff.c --------------------------------------------------- */
#ifdef	unix
	time_t	cutoff(
			_arx(int,	argc)
			_ar1(char **,	argv)
			)
			_dcl(int,	argc)
			_dcl(char **,	argv)
			_ret

	/* denode.c --------------------------------------------------- */
	char *	denode (
			_arx(char *,	path)
			_arx(char *,	node)
			_ar1(int *,	opt)
			)
			_dcl(char *,	path)
			_dcl(char *,	node)
			_dcl(int *,	opt)
			_ret
#endif	/* unix */

	/* dftenv.c --------------------------------------------------- */
	char *	dftenv(
			_arx(char *,	value)
			_ar1(char *,	name)
			)
			_dcl(char *,	value)
			_dcl(char *,	name)
			_ret

	/* doalloc.c -------------------------------------------------- */
	char *	doalloc(
			_arx(char *,	oldp)
			_ar1(unsigned,	len)
			)
			_dcl(char *,	oldp)
			_dcl(unsigned,	len)
			_ret

	void	dofree(
			_ar1(char *,	oldp)
			)
			_dcl(char *,	oldp)
			_nul

	void	show_alloc(_ar0)
			_nul

	/* dotcmp.c --------------------------------------------------- */
	int	dotcmp (
			_arx(char *,	s1)
			_ar1(char *,	s2)
			)
			_dcl(char *,	s1)
			_dcl(char *,	s2)
			_ret

	/* dotname.c -------------------------------------------------- */
	int	dotname (
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_ret

	/* dumpchr.c -------------------------------------------------- */
		dumpchr(
			_arx(FILE *,	fp)
			_ar1(int,	c)
			)
			_dcl(FILE *,	fp)
			_dcl(int,	c)
			_nul

	/* execute.c -------------------------------------------------- */
	int	execute(
			_arx(char *,	verb)
			_ar1(char *,	args)
			)
			_dcl(char *,	verb)
			_dcl(char *,	args)
			_ret

	/* file2arg.c ------------------------------------------------- */
	int	file2argv(
			_arx(char *,	name)
			_ar1(char ***,	vec)
			)
			_dcl(char *,	name)
			_dcl(char ***,	vec)
			_ret

	/* file2mem.c ------------------------------------------------- */
	char *	file2mem(
			_ar1(char *,	name)
			)
			_dcl(char *,	name)
			_ret

	/* filecopy.c ------------------------------------------------- */
#ifdef	unix
	int	filecopy(
			_arx(char *,	src)
			_arx(char *,	dst)
			_ar1(int,	copy)
			)
			_dcl(char *,	src)
			_dcl(char *,	dst)
			_dcl(int,	copy)
			_ret

	/* filesize.c ------------------------------------------------- */
	off_t	filesize(
			_ar1(char *,	name)
			)
			_dcl(char *,	name)
			_ret

	/* for_admn.c ------------------------------------------------- */
	int	for_admin2(
			_fnx(int,	func,	(_AR0))
			_arx(int,	the_uid)
			_ar1(int,	the_gid)
			)
			_dcl(int,	(*func)())
			_dcl(int,	the_uid)
			_dcl(int,	the_gid)
			_ret

	int	for_admin(
			_fn1(int,	func,	(_AR0))
			)
			_dcl(int,	(*func)())
			_ret

	/* for_user.c ------------------------------------------------- */
	int	for_user2(
			_fnx(int,	func,	(_AR0))
			_arx(int,	the_uid)
			_ar1(int,	the_gid)
			)
			_dcl(int,	(*func)())
			_dcl(int,	the_uid)
			_dcl(int,	the_gid)
			_ret

	int	for_user(
			_fn1(int,	func,	(_AR0))
			)
			_dcl(int,	(*func)())
			_ret
#endif	/* unix */

	/* fp2argv.c -------------------------------------------------- */
	int	fp2argv(
			_arx(FILE *,	fp)
			_arx(char ***,	argv_)
			_fn1(void,	trace,	(_AR1(char *,s)))
			)
			_dcl(FILE *,	fp)
			_dcl(char ***,	argv_)
			_dcl(void,	(*trace)())
			_ret

	/* ftype.c ---------------------------------------------------- */
	char *	ftype(
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_ret

	/* ftype2.c --------------------------------------------------- */
#ifdef	unix
	char *	ftype2(
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_ret

	/* gethome.c -------------------------------------------------- */
	char *	gethome(_ar0)
			_ret

	/* getuser.c -------------------------------------------------- */
	char *	getuser(_ar0)
			_ret
#endif	/* unix */

	/* gid2s.c ---------------------------------------------------- */
	char *	gid2s(
			_ar1(int,	gid)
			)
			_dcl(int,	gid)
			_ret

	/* interact.c ------------------------------------------------- */
	int	interactive(_ar0)
			_ret

	/* is_spath.c ------------------------------------------------- */
	int	is_subpath(
			_arx(char *,	ref)
			_ar1(char *,	tst)
			)
			_dcl(char *,	ref)
			_dcl(char *,	tst)
			_ret

	/* istextfl.c ------------------------------------------------- */
	int	istextfile(
			_ar1(char *,	name)
			)
			_dcl(char *,	name)
			_ret

	/* lsbycols.c ------------------------------------------------- */
		list_by_cols(
			_arx(char **,	listp)
			_arx(int,	sizep)
			_ar1(int,	num)
			)
			_dcl(char **,	listp)
			_dcl(int,	sizep)
			_dcl(int,	num)
			_nul

	/* logch.c ---------------------------------------------------- */
	int	decode_logch(
			_arx(char **,	buffer)
			_ar1(int *,	count_)
			)
			_dcl(char **,	buffer)
			_dcl(int *,	count_)
			_ret

	void	encode_logch(
			_arx(char *,	buffer)
			_arx(int *,	count_)
			_ar1(int,	c)
			)
			_dcl(char *,	buffer)
			_dcl(int *,	count_)
			_dcl(int,	c)
			_nul

	/* ltostr.c --------------------------------------------------- */
	char *	ltostr(
			_arx(char *,	bfr)
			_arx(long,	value)
			_ar1(int,	base)
			)
			_dcl(char *,	bfr)
			_dcl(long,	value)
			_dcl(int,	base)
			_ret

	/* makeargv.c ------------------------------------------------- */
	int	makeargv(
			_arx(char **,	argv)
			_arx(int,	maxarg)
			_arx(char *,	dst)
			_ar1(char *,	src)
			)
			_dcl(char **,	argv)
			_dcl(int,	maxarg)
			_dcl(char *,	dst)
			_dcl(char *,	src)
			_ret

	/* mem2file.c ------------------------------------------------- */
	int	mem2file(
			_arx(char *,	blob)
			_arx(char *,	name)
			_ar1(char *,	mode)
			)
			_dcl(char *,	blob)
			_dcl(char *,	name)
			_dcl(char *,	mode)
			_ret

	/* modechar.c ------------------------------------------------- */
	int	modechar(
			_ar1(unsigned,	mode)
			)
			_dcl(unsigned,	mode)
			_ret

	/* name2s.c --------------------------------------------------- */
#ifdef	unix
	int	name2s(
			_arx(char *,	bfr)
			_arx(int,	maxbfr)
			_arx(char *,	name)
			_ar1(int,	opt)
			)
			_dcl(char *,	bfr)
			_dcl(int,	maxbfr)
			_dcl(char *,	name)
			_dcl(int,	opt)
			_ret

	/* newzone.c -------------------------------------------------- */
	int	localzone;

		newzone(
			_arx(int,	hours)
			_arx(int,	minutes)
			_ar1(int,	apres)
			)
			_dcl(int,	hours)
			_dcl(int,	minutes)
			_dcl(int,	apres)
			_nul

		oldzone(_ar0)
			_nul

	/* next_ver.c ------------------------------------------------- */
		next_version(
			_arx(char *,	dst)
			_ar1(char *,	src)
			)
			_dcl(char *,	dst)
			_dcl(char *,	src)
			_nul

	/* packdate.c ------------------------------------------------- */
	long	gmt_offset(
			_ar1(time_t,	t)
			)
			_dcl(time_t,	t)
			_ret

	long	packdate (
			_arx(int,	year)
			_arx(int,	mon)
			_arx(int,	day)
			_arx(int,	hour)
			_arx(int,	min)
			_ar1(int,	s)
			)
			_dcl(int,	year)
			_dcl(int,	mon)
			_dcl(int,	day)
			_dcl(int,	hour)
			_dcl(int,	min)
			_dcl(int,	s)
			_ret

	/* padedit.c -------------------------------------------------- */
	int	padedit(
			_arx(char *,	name)
			_arx(int,	readonly)
			_ar1(char *,	editor)
			)
			_dcl(char *,	name)
			_dcl(int,	readonly)
			_dcl(char *,	editor)
			_ret
#endif	/* unix */

	/* pathcat.c -------------------------------------------------- */
	char *	pathcat(
			_arx(char *,	dst)
			_arx(char *,	p)
			_ar1(char *,	n)
			)
			_dcl(char *,	dst)
			_dcl(char *,	p)
			_dcl(char *,	n)
			_ret

	/* pathcmp.c -------------------------------------------------- */
#ifdef	unix
	int	pathcmp(
			_arx(char *,	p1)
			_ar1(char *,	p2)
			)
			_dcl(char *,	p1)
			_dcl(char *,	p2)
			_ret

	/* pathhead.c ------------------------------------------------- */
	char *	pathhead(
			_arx(char *,	path)
			_ar1(STAT *,	s)
			)
			_dcl(char *,	path)
			_dcl(STAT *,	s)
			_ret
#endif	/* unix */

	/* pathleaf.c ------------------------------------------------- */
	char *	pathleaf(
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_ret

	/* relpath.c -------------------------------------------------- */
#ifdef	unix
	char *	relpath(
			_arx(char *,	dst)
			_arx(char *,	cwd)
			_ar1(char *,	src)
			)
			_dcl(char *,	dst)
			_dcl(char *,	cwd)
			_dcl(char *,	src)
			_ret

	/* revert.c --------------------------------------------------- */
	int	revert(
			_ar1(char *,	msg)
			)
			_dcl(char *,	msg)
			_ret

	/* s2gid.c ---------------------------------------------------- */
	int	s2gid(
			_ar1(char *,	s)
			)
			_dcl(char *,	s)
			_ret

	/* s2uid.c ---------------------------------------------------- */
	int	s2uid(
			_ar1(char *,	s)
			)
			_dcl(char *,	s)
			_ret
#endif	/* unix */

	/* sameleaf.c ------------------------------------------------- */
	int	sameleaf(
			_arx(char *,	path)
			_ar1(char *,	leaf)
			)
			_dcl(char *,	path)
			_dcl(char *,	leaf)
			_ret

#ifdef	unix
	/* savesuid.c ------------------------------------------------- */
	int	saves_uid(_ar0)
			_nul

	/* scr_size.c ------------------------------------------------- */
	int	scr_size(
			_ar1(int *,	lc)
			)
			_dcl(int *,	lc)
			_ret

	/* setmtime.c ------------------------------------------------- */
	int	setmtime(
			_arx(char *,	name)
			_ar1(time_t,	mtime)
			)
			_dcl(char *,	name)
			_dcl(time_t,	mtime)
			_ret
#endif	/* unix */

	/* shoarg.c --------------------------------------------------- */
	char 	*bldcmd (
			_arx(char *,	dst)
			_arx(char *,	src)
			_ar1(size_t,	len)
			)
			_dcl(char *,	dst)
			_dcl(char *,	src)
			_dcl(size_t,	len)
			_ret

	char 	*bldcmd2 (
			_arx(char *,	dst)
			_arx(char *,	src1)
			_arx(char *,	src2)
			_ar1(size_t,	len)
			)
			_dcl(char *,	dst)
			_dcl(char *,	src1)
			_dcl(char *,	src2)
			_dcl(size_t,	len)
			_ret

		shoarg (
			_arx(FILE *,	fp)
			_arx(char *,	command)
			_ar1(char *,	args)
			)
			_dcl(FILE *,	fp)
			_dcl(char *,	command)
			_dcl(char *,	args)
			_nul

	/* showuids.c ------------------------------------------------- */
	void	show_uids(
			_ar1(FILE *,	fp)
			)
			_dcl(FILE *,	fp)
			_nul

	/* stat_dir.c ------------------------------------------------- */
	int	stat_dir(
			_arx(char *,	path)
			_ar1(STAT *,	sb)
			)
			_dcl(char *,	path)
			_dcl(STAT *,	sb)
			_ret

	/* stat_fil.c ------------------------------------------------- */
	int	stat_file(
			_arx(char *,	path)
			_ar1(STAT *,	sb)
			)
			_dcl(char *,	path)
			_dcl(STAT *,	sb)
			_ret

	/* stralloc.c ------------------------------------------------- */
	char *	stralloc(
			_ar1(char *,	string)
			)
			_dcl(char *,	string)
			_ret

		strfree(
			_ar1(char *,	string)
			)
			_dcl(char *,	string)
			_nul

	/* strbcmp.c -------------------------------------------------- */
	int	strbcmp(
			_arx(char *,	a)
			_ar1(char *,	b)
			)
			_dcl(char *,	a)
			_dcl(char *,	b)
			_ret

	/* strclean.c ------------------------------------------------- */
	int	strclean(
			_ar1(char *,	string)
			)
			_dcl(char *,	string)
			_ret

	/* strcount.c ------------------------------------------------- */
	int	strcount(
			_arx(char *,	s)
			_ar1(int,	c)
			)
			_dcl(char *,	s)
			_dcl(int,	c)
			_ret

	/* strtrim.c -------------------------------------------------- */
	char *	strtrim(
			_ar1(char *,	string)
			)
			_dcl(char *,	string)
			_ret

	/* strucmp.c -------------------------------------------------- */
	int	strucmp(
			_arx(char *,	a)
			_ar1(char *,	b)
			)
			_dcl(char *,	a)
			_dcl(char *,	b)
			_ret

	/* strucpy.c -------------------------------------------------- */
	char *	strucpy(
			_arx(char *,	a)
			_ar1(char *,	b)
			)
			_dcl(char *,	a)
			_dcl(char *,	b)
			_ret

	char *	strlcpy(
			_arx(char *,	a)
			_ar1(char *,	b)
			)
			_dcl(char *,	a)
			_dcl(char *,	b)
			_ret

	/* strwcmp.c -------------------------------------------------- */
	int	strwcmp(
			_arx(char *,	wild)
			_ar1(char *,	name)
			)
			_dcl(char *,	wild)
			_dcl(char *,	name)
			_ret

	/* track_wd.c ------------------------------------------------- */
		track_wd(
			_ar1(char *,	path)
			)
			_dcl(char *,	path)
			_nul

	/* trimpath.c ------------------------------------------------- */
	char *	trimpath(
			_arx(char *,	path)
			_ar1(char *,	cwd)
			)
			_dcl(char *,	path)
			_dcl(char *,	cwd)
			_ret

	/* txtalloc.c ------------------------------------------------- */
	char *	txtalloc(
			_ar1(char *,	string)
			)
			_dcl(char *,	string)
			_ret

	void	txtfree(
			_ar1(char *,	string)
			)
			_dcl(char *,	string)
			_nul

	void	free_txtalloc(_ar0)
			_nul

	/* uid2s.c ---------------------------------------------------- */
#ifdef	apollo_sr10
	int	len_passwd;
	char	**vec_passwd;
#endif
	char *	uid2s(
			_ar1(int,	uid)
			)
			_dcl(int,	uid)
			_ret

	/* usercopy.c ------------------------------------------------- */
#ifdef	unix
	int	usercopy(
			_arx(char *,	src)
			_ar1(char *,	dst)
			)
			_dcl(char *,	src)
			_dcl(char *,	dst)
			_ret

	/* userprot.c ------------------------------------------------- */
	int	userprot(
			_arx(char *,	name)
			_arx(int,	mode)
			_ar1(time_t,	mtime)
			)
			_dcl(char *,	name)
			_dcl(int,	mode)
			_dcl(time_t,	mtime)
			_ret
#endif	/* unix */

	/* vecalloc.c ------------------------------------------------- */
	char	**vecalloc(
			_ar1(unsigned,	len)
			)
			_dcl(unsigned,	len)
			_ret

		vecfree(
			_ar1(char **,	v)
			)
			_dcl(char **,	v)
			_nul

	/* vecedit.c -------------------------------------------------- */
	char **	vecedit (
			_arx(char **,	text)
			_ar1(char **,	script)
			)
			_dcl(char **,	text)
			_dcl(char **,	script)
			_ret

	/* veclen.c --------------------------------------------------- */
	int	veclen (
			_ar1(char **,	vector)
			)
			_dcl(char **,	vector)
			_ret

	/* vercmp.c --------------------------------------------------- */
	int	vercmp (
			_arx(char *,	s1)
			_arx(char *,	s2)
			_ar1(int,	wild)
			)
			_dcl(char *,	s1)
			_dcl(char *,	s2)
			_dcl(int,	wild)
			_ret

	/* viewfile.c ------------------------------------------------- */
#ifdef	unix
	int	view_file(
			_arx(char *,	fname)
			_ar1(int,	readonly)
			)
			_dcl(char *,	fname)
			_dcl(int,	readonly)
			_ret
#endif	/* unix */

	/* walkback.c ------------------------------------------------- */
	void	walkback(
			_ar1(char *,	program)
			)
			_dcl(char *,	program)
			_nul

	/* walktree.c ------------------------------------------------- */
#ifdef	unix

#define	WALK_FUNC_ARGS	\
			_ARX(char *,	path)\
			_ARX(char *,	name)\
			_ARX(STAT *,	sp)\
			_ARX(int,	readable)\
			_AR1(int,	level)

#define	WALK_FUNC(f)	f(WALK_FUNC_ARGS)\
			_DCL(char *,	path)\
			_DCL(char *,	name)\
			_DCL(STAT *,	sp)\
			_DCL(int,	readable)\
			_DCL(int,	level)

	int	walktree(
			_arx(char *,	p)
			_arx(char *,	n)
			_fnx(int,	f,	(WALK_FUNC_ARGS))
			_arx(char *,	m)
			_ar1(int,	lvl)
			)
			_dcl(char *,	p)
			_dcl(char *,	n)
			_dcl(int,	(*f)())
			_dcl(char *,	m)
			_dcl(int,	lvl)
			_ret

	/* which.c ---------------------------------------------------- */
	int	which(
			_arx(char *,	bfr)
			_arx(int,	len)
			_arx(char *,	find)
			_ar1(char *,	dot)
			)
			_dcl(char *,	bfr)
			_dcl(int,	len)
			_dcl(char *,	find)
			_dcl(char *,	dot)
			_ret
#endif	/* unix */

#endif		/* _COMMON_H_ */
