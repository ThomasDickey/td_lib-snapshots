/* $Header: /users/source/archives/td_lib.vcs/include/RCS/td_lib.h,v 3.0 1989/06/05 13:21:31 ste_cm Rel $ */

/*
 * Combined lint-library/function prototype definitions for CM_TOOLS COMMON
 * library.
 */

#ifndef		_PTYPES_
#include "ptypes.h"
#endif		_PTYPES_

	/* MAIN-PROGRAM *********************************************** */
		failed(
			_AR1(char *,	s)
			)
			_DCL(char *,	s)
			_NUL

	/* GENERIC-LIBRARY ******************************************** */

	/* abspath.c -------------------------------------------------- */
#ifdef	unix
		abspath(
			_AR1(char *,	path)
			)
			_DCL(char *,	path)
			_NUL
#endif	unix

	/* add2fname.c ------------------------------------------------ */
	int	add2fname(
			_ARX(char *,	fname)
			_AR1(char *,	suffix)
			)
			_DCL(char *,	fname)
			_DCL(char *,	suffix)
			_RET

	/* argv2file.c ------------------------------------------------ */
	int	argv2file(
			_ARX(char **,	vec)
			_ARX(char *,	name)
			_AR1(char *,	mode)
			)
			_DCL(char **,	vec)
			_DCL(char *,	name)
			_DCL(char *,	mode)
			_RET

	/* beep.c ----------------------------------------------------- */
		beep()
			_NUL

	/* bldarg.c --------------------------------------------------- */
		bldarg (
			_ARX(int,	argc)
			_ARX(char **,	argv)
			_AR1(char *,	string)
			)
			_DCL(int,	argc)
			_DCL(char **,	argv)
			_DCL(char *,	string)
			_NUL

	/* blip.c ----------------------------------------------------- */
		blip(
			_AR1(int,	c)
			)
			_DCL(int,	c)
			_NUL

	/* catarg.c --------------------------------------------------- */
		catarg (
			_ARX(char *,	dst)
			_AR1(char *,	src)
			)
			_DCL(char *,	dst)
			_DCL(char *,	src)
			_NUL

	/* catchall.c ------------------------------------------------- */
		catchall (
			_FN1(int,	func)
			)
			_DCL(int,	(*func)())
			_NUL

	/* cmdch.c ---------------------------------------------------- */
#ifdef	CUR_PTYPES
	int	cmdch(
			_AR1(int *,	cntp)
			)
			_DCL(int *,	cntp)
			_RET
#endif	CUR_PTYPES

	/* copyback.c ------------------------------------------------- */
	int	copyback(
			_ARX(FILE *,	fp)
			_ARX(char *,	name)
			_ARX(int,	mode)
			_AR1(int,	lines)
			)
			_DCL(FILE *,	fp)
			_DCL(char *,	name)
			_DCL(int,	mode)
			_DCL(int,	lines)
			_RET

	/* cutoff.c --------------------------------------------------- */
#ifdef	unix
	time_t	cutoff(
			_ARX(int,	argc)
			_AR1(char **,	argv)
			)
			_DCL(int,	argc)
			_DCL(char **,	argv)
			_RET

	/* denode.c --------------------------------------------------- */
	char	*denode (
			_ARX(char *,	path)
			_ARX(char *,	node)
			_AR1(int *,	opt)
			)
			_DCL(char *,	path)
			_DCL(char *,	node)
			_DCL(int *,	opt)
			_RET
#endif	unix

	/* doalloc.c -------------------------------------------------- */
	char	*doalloc(
			_ARX(char *,	oldp)
			_AR1(unsigned,	len)
			)
			_DCL(char *,	oldp)
			_DCL(unsigned,	len)
			_RET

		dofree(
			_AR1(char *,	oldp)
			)
			_DCL(char *,	oldp)
			_NUL

	/* dotcmp.c --------------------------------------------------- */
	int	dotcmp (
			_ARX(char *,	s1)
			_AR1(char *,	s2)
			)
			_DCL(char *,	s1)
			_DCL(char *,	s2)
			_RET

	/* dotname.c -------------------------------------------------- */
	int	dotname (
			_AR1(char *,	path)
			)
			_DCL(char *,	path)
			_RET

	/* dumpchr.c -------------------------------------------------- */
		dumpchr(
			_ARX(FILE *,	fp)
			_AR1(int,	c)
			)
			_DCL(FILE *,	fp)
			_DCL(int,	c)
			_NUL

	/* dumpwin.c -------------------------------------------------- */
#ifdef	CUR_PTYPES
		dumpwin(
			_ARX(WINDOW *,	w)
			_AR1(char *,	s)
			)
			_DCL(WINDOW *,	w)
			_DCL(char *,	s)
			_NUL

	/* erasechar.c ------------------------------------------------ */
#ifdef	erasechar
#undef	erasechar
#endif	erasechar
		char	erasechar(
			)
			_RET
#endif	CUR_PTYPES

	/* execute.c -------------------------------------------------- */
#ifdef	unix
	int	execute(
			_ARX(char *,	verb)
			_AR1(char *,	args)
			)
			_DCL(char *,	verb)
			_DCL(char *,	args)
			_RET
#endif	unix

	/* file2argv.c ------------------------------------------------ */
	int	file2argv(
			_ARX(char *,	name)
			_AR1(char ***,	vec)
			)
			_DCL(char *,	name)
			_DCL(char ***,	vec)
			_RET

	/* file2mem.c ------------------------------------------------- */
	char *	file2mem(
			_AR1(char *,	name)
			)
			_DCL(char *,	name)
			_RET

	/* filecopy.c ------------------------------------------------- */
#ifdef	unix
	int	filecopy(
			_ARX(char *,	src)
			_ARX(char *,	dst)
			_AR1(int,	copy)
			)
			_DCL(char *,	src)
			_DCL(char *,	dst)
			_DCL(int,	copy)
			_RET

	/* for_user.c ------------------------------------------------- */
	int	for_user(
			_FN1(int,	func)
			)
			_DCL(int,	(*func)())
			_RET
#endif	unix

	/* fp2argv.c -------------------------------------------------- */
	int	fp2argv(
			_ARX(FILE *,	fp)
			_AR1(char ***,	argv_)
			)
			_DCL(FILE *,	fp)
			_DCL(char ***,	argv_)
			_RET

	/* ftype.c ---------------------------------------------------- */
#ifdef	unix
	char	*ftype(
			_AR1(char *,	path)
			)
			_DCL(char *,	path)
			_RET

	/* ftype2.c --------------------------------------------------- */
	char	*ftype2(
			_AR1(char *,	path)
			)
			_DCL(char *,	path)
			_RET

	/* gethome.c -------------------------------------------------- */
	char	*gethome()
			_RET

	/* getuser.c -------------------------------------------------- */
	char	*getuser()
			_RET
#endif	unix

	/* gid2s.c ---------------------------------------------------- */
	char	*gid2s(
			_AR1(int,	gid)
			)
			_DCL(int,	gid)
			_RET

	/* killchar.c ------------------------------------------------- */
#ifdef	CUR_PTYPES
#ifdef	killchar
#undef	killchar
#endif	killchar
	char	killchar()
			_RET
#endif	CUR_PTYPES

	/* list_by_cols.c --------------------------------------------- */
		list_by_cols(
			_ARX(char **,	listp)
			_ARX(int,	sizep)
			_AR1(int,	num)
			)
			_DCL(char **,	listp)
			_DCL(int,	sizep)
			_DCL(int,	num)
			_NUL

	/* ltostr.c --------------------------------------------------- */
	char	*ltostr(
			_ARX(char *,	bfr)
			_ARX(long,	value)
			_AR1(int,	base)
			)
			_DCL(char *,	bfr)
			_DCL(long,	value)
			_DCL(int,	base)
			_RET

	/* makeargv.c ------------------------------------------------- */
	int	makeargv(
			_ARX(char **,	argv)
			_ARX(int,	maxarg)
			_ARX(char *,	dst)
			_AR1(char *,	src)
			)
			_DCL(char **,	argv)
			_DCL(int,	maxarg)
			_DCL(char *,	dst)
			_DCL(char *,	src)
			_RET

	/* mem2file.c ------------------------------------------------- */
	int	mem2file(
			_ARX(char *,	blob)
			_ARX(char *,	name)
			_AR1(char *,	mode)
			)
			_DCL(char *,	blob)
			_DCL(char *,	name)
			_DCL(char *,	mode)
			_RET

	/* modechar.c ------------------------------------------------- */
	int	modechar(
			_AR1(unsigned,	mode)
			)
			_DCL(unsigned,	mode)
			_RET

	/* name2s.c --------------------------------------------------- */
#ifdef	unix
	int	name2s(
			_ARX(char *,	bfr)
			_ARX(int,	maxbfr)
			_ARX(char *,	name)
			_AR1(int,	opt)
			)
			_DCL(char *,	bfr)
			_DCL(int,	maxbfr)
			_DCL(char *,	name)
			_DCL(int,	opt)
			_RET

	/* newzone.c -------------------------------------------------- */
	int	localzone;

		newzone(
			_ARX(int,	hours)
			_ARX(int,	minutes)
			_AR1(int,	apres)
			)
			_DCL(int,	hours)
			_DCL(int,	minutes)
			_DCL(int,	apres)
			_NUL

		oldzone()
			_NUL

	/* packdate.c ------------------------------------------------- */
	long	packdate (
			_ARX(int,	year)
			_ARX(int,	mon)
			_ARX(int,	day)
			_ARX(int,	hour)
			_ARX(int,	min)
			_AR1(int,	s)
			)
			_DCL(int,	year)
			_DCL(int,	mon)
			_DCL(int,	day)
			_DCL(int,	hour)
			_DCL(int,	min)
			_DCL(int,	s)
			_RET

	/* padedit.c -------------------------------------------------- */
	int	padedit(
			_ARX(char *,	name)
			_ARX(int,	readonly)
			_AR1(char *,	editor)
			)
			_DCL(char *,	name)
			_DCL(int,	readonly)
			_DCL(char *,	editor)
			_RET
#endif	unix

	/* pathcat.c -------------------------------------------------- */
	char *	pathcat(
			_ARX(char *,	dst)
			_ARX(char *,	p)
			_AR1(char *,	n)
			)
			_DCL(char *,	dst)
			_DCL(char *,	p)
			_DCL(char *,	n)
			_RET

	/* pathcmp.c -------------------------------------------------- */
#ifdef	unix
	int	pathcmp(
			_ARX(char *,	p1)
			_AR1(char *,	p2)
			)
			_DCL(char *,	p1)
			_DCL(char *,	p2)
			_RET

	/* pathhead.c ------------------------------------------------- */
	char *	pathhead(
			_ARX(char *,	path)
			_AR1(struct stat *, s)
			)
			_DCL(char *,	path)
			_DCL(struct stat *, s)
			_RET
#endif	unix

	/* pathleaf.c ------------------------------------------------- */
	char *	pathleaf(
			_AR1(char *,	path)
			)
			_DCL(char *,	path)
			_RET

	/* rawgets.c -------------------------------------------------- */
#ifdef	CUR_PTYPES
		rawgets (
			_ARX(char *,	bfr)
			_ARX(int,	size)
			_AR1(int,	newline)
			)
			_DCL(char *,	bfr)
			_DCL(int,	size)
			_DCL(int,	newline)
			_NUL

	/* rawterm.c -------------------------------------------------- */
		rawterm()
			_NUL

	/* resizewin.c ------------------------------------------------ */
	int	resizewin()
			_RET
#endif	CUR_PTYPES

	/* revert.c --------------------------------------------------- */
#ifdef	unix
		revert(
			_AR1(char *,	msg)
			)
			_DCL(char *,	msg)
			_NUL

	/* s2gid.c ---------------------------------------------------- */
	int	s2gid(
			_AR1(char *,	s)
			)
			_DCL(char *,	s)
			_RET

	/* s2uid.c ---------------------------------------------------- */
	int	s2uid(
			_AR1(char *,	s)
			)
			_DCL(char *,	s)
			_RET
#endif	unix

	/* sameleaf.c ------------------------------------------------- */
	int	sameleaf(
			_ARX(char *,	path)
			_AR1(char *,	leaf)
			)
			_DCL(char *,	path)
			_DCL(char *,	leaf)
			_RET

	/* savewin.c -------------------------------------------------- */
#ifdef	CUR_PTYPES
		savewin()
			_NUL

		lastwin(
			_ARX(int,	redo)
			_AR1(int,	top)
			)
			_DCL(int,	redo)
			_DCL(int,	top)
			_NUL

		unsavewin(
			_ARX(int,	redo)
			_AR1(int,	top)
			)
			_DCL(int,	redo)
			_DCL(int,	top)
			_NUL
#endif	CUR_PTYPES

	/* sccs_dir.c ------------------------------------------------- */
#ifdef	unix
	char *	sccs_dir()
			_RET

	/* sccslast.c ------------------------------------------------- */
		sccslast(
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

	/* scr_size.c ------------------------------------------------- */
	int	scr_size(
			_AR1(int *,	lc)
			)
			_DCL(int *,	lc)
			_RET

	/* setmtime.c ------------------------------------------------- */
	int	setmtime(
			_ARX(char *,	name)
			_AR1(time_t,	mtime)
			)
			_DCL(char *,	name)
			_DCL(time_t,	mtime)
			_RET
#endif	unix

	/* stralloc.c ------------------------------------------------- */
	char	*stralloc(
			_AR1(char *,	string)
			)
			_DCL(char *,	string)
			_RET

		strfree(
			_AR1(char *,	string)
			)
			_DCL(char *,	string)
			_NUL

	/* strbcmp.c -------------------------------------------------- */
	int	strbcmp(
			_ARX(char *,	a)
			_AR1(char *,	b)
			)
			_DCL(char *,	a)
			_DCL(char *,	b)
			_RET

	/* strclean.c ------------------------------------------------- */
	int	strclean(
			_AR1(char *,	string)
			)
			_DCL(char *,	string)
			_RET

	/* strtrim.c -------------------------------------------------- */
	char *	strtrim(
			_AR1(char *,	string)
			)
			_DCL(char *,	string)
			_RET

	/* strucmp.c -------------------------------------------------- */
	int	strucmp(
			_ARX(char *,	a)
			_AR1(char *,	b)
			)
			_DCL(char *,	a)
			_DCL(char *,	b)
			_RET

	/* strucpy.c -------------------------------------------------- */
		strucpy(
			_ARX(char *,	a)
			_AR1(char *,	b)
			)
			_DCL(char *,	a)
			_DCL(char *,	b)
			_NUL

	/* trimpath.c ------------------------------------------------- */
	char	*trimpath(
			_ARX(char *,	path)
			_AR1(char *,	cwd)
			)
			_DCL(char *,	path)
			_DCL(char *,	cwd)
			_RET

	/* txtalloc.c ------------------------------------------------- */
	char	*txtalloc(
			_AR1(char *,	string)
			)
			_DCL(char *,	string)
			_RET

		txtfree(
			_AR1(char *,	string)
			)
			_DCL(char *,	string)
			_NUL

	/* uid2s.c ---------------------------------------------------- */
	char	*uid2s(
			_AR1(int,	uid)
			)
			_DCL(int,	uid)
			_RET

	/* usercopy.c ------------------------------------------------- */
#ifdef	unix
	int	usercopy(
			_ARX(char *,	src)
			_AR1(char *,	dst)
			)
			_DCL(char *,	src)
			_DCL(char *,	dst)
			_RET

	/* userprot.c ------------------------------------------------- */
	int	userprot(
			_ARX(char *,	name)
			_ARX(int,	mode)
			_AR1(time_t,	mtime)
			)
			_DCL(char *,	name)
			_DCL(int,	mode)
			_DCL(time_t,	mtime)
			_RET
#endif	unix

	/* vecalloc.c ------------------------------------------------- */
	char	**vecalloc(
			_AR1(unsigned,	len)
			)
			_DCL(unsigned,	len)
			_RET

		vecfree(
			_AR1(char **,	v)
			)
			_DCL(char **,	v)
			_NUL

	/* vercmp.c --------------------------------------------------- */
	int	vercmp (
			_ARX(char *,	s1)
			_ARX(char *,	s2)
			_AR1(int,	wild)
			)
			_DCL(char *,	s1)
			_DCL(char *,	s2)
			_DCL(int,	wild)
			_RET

	/* walktree.c ------------------------------------------------- */
#ifdef	unix
	int	walktree(
			_ARX(char *,	p)
			_ARX(char *,	n)
			_FNX(int,	f)
			_ARX(char *,	m)
			_AR1(int,	lvl)
			)
			_DCL(char *,	p)
			_DCL(char *,	n)
			_DCL(int,	(*f)())
			_DCL(char *,	m)
			_DCL(int,	lvl)
			_RET

	/* which.c ---------------------------------------------------- */
	int	which(
			_ARX(char *,	bfr)
			_ARX(int,	len)
			_ARX(char *,	find)
			_AR1(char *,	dot)
			)
			_DCL(char *,	bfr)
			_DCL(int,	len)
			_DCL(char *,	find)
			_DCL(char *,	dot)
			_RET
#endif	unix

	/* win2file.c ------------------------------------------------- */
#ifdef	CUR_PTYPES
	win2file(
			_ARX(WINDOW *,	win)
			_AR1(char *,	file)
			)
			_DCL(WINDOW *,	win)
			_DCL(char *,	file)
			_NUL
#endif	CUR_PTYPES
