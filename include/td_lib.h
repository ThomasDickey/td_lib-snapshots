/* $Id: td_lib.h,v 12.35 2019/12/01 22:20:49 tom Exp $ */

/*
 * Combined lint-library/function prototype definitions for TD_LIB common
 * library.
 */

#ifndef		TD_LIB_H
#define		TD_LIB_H
#ifndef		PTYPES_H
#include <ptypes.h>
#endif		/* PTYPES_H */

	/* MAIN-PROGRAM *********************************************** */
	void	failed(
			const char * s
			)
			GCC_NORETURN ;

	/* GENERIC-LIBRARY ******************************************** */

#ifdef	CUR_PTYPES
#include <td_curse.h>
#endif

	/* abspath.c -------------------------------------------------- */
#if	defined(SYS_UNIX) || defined(MSDOS)
	void	abshome(
			char *	path
			)
			;

	void	abspath(
			char *	path
			)
			;
#endif	/* SYS_UNIX or MSDOS */

	/* acc_mode.c ------------------------------------------------- */
	char *	access_mode(
			int	mode
			)
			;

	/* add2fnam.c ------------------------------------------------- */
	int	add2fname(
			char *	fname,
			char *	suffix
			)
			;

	/* arg2file.c ------------------------------------------------- */
	int	argv2file(
			char **	vec,
			char *	name,
			char *	mode
			)
			;

	/* blip.c ----------------------------------------------------- */
	void	blip(
			int	c
			)
			;

	/* bldarg.c --------------------------------------------------- */
	int	bldarg (
			int	argc,
			char **	argv,
			char *	string
			)
			;

	/* catarg.c --------------------------------------------------- */
	void	catarg (
			char *	dst,
			const char *	src
			)
			;

	void	catarg2 (
			char *	dst,
			const char *	opt,
			const char *	arg
			)
			;

	/* catchall.c ------------------------------------------------- */
#ifdef	SIG_PTYPES
	void	catchall (
			SIG_T (*func)	(SIGNAL_ARGS)
			)
			;
#endif

	/* copyback.c ------------------------------------------------- */
	int	copyback(
			FILE *	fp,
			char *	name,
			int	mode,
			int	lines_to_copy
			)
			;

	/* cutoff.c --------------------------------------------------- */
#ifdef	SYS_UNIX
	time_t	cutoff(
			int	argc,
			char **	argv
			)
			;
#endif	/* SYS_UNIX */

	/* denode.c --------------------------------------------------- */
#if	defined(SYS_UNIX) || defined(MSDOS)
	char *	denode (
			char *	path,
			char *	node,
			int *	opt
			)
			;
#endif	/* SYS_UNIX */

	/* dftenv.c --------------------------------------------------- */
	const char *	dftenv(
			const char *	value,
			const char *	name
			)
			;

	/* dlettree.c ------------------------------------------------- */
#ifndef	LINTLIBRARY
	extern	void	fail      (const char * text);
	extern	int	deletefile(const char * name);
	extern	int	deletedir (const char * name);
#endif

	int	deletetree(
			const char *	path,
			int		recur
			)
			;

	/* doalloc.c -------------------------------------------------- */
	void *	doalloc(
			void *		oldp,
			size_t		len
			)
			;

	void	dofree(
			void *		oldp
			)
			;

	void	show_alloc(void)
			;

	/* dotcmp.c --------------------------------------------------- */
	int	dotcmp (
			const char * s1,
			const char * s2
			)
			;

	/* dotname.c -------------------------------------------------- */
	int	dotname (
			const char *	path
			)
			;

	/* dumpchr.c -------------------------------------------------- */
	void	dumpchr(
			FILE *	fp,
			int	c
			)
			;

	/* editfile.c ------------------------------------------------- */
	int	editfile(
			const char *	name,
			int (*func) (FILE* o, FILE* i, Stat_t* s),
			Stat_t *	sb
			)
			;

	/* edittree.c ------------------------------------------------- */
	int	edittree(
			const char *	name,
			int (*func) (FILE* o, FILE* i, Stat_t* s),
			int		recur,
			int		links
			)
			;

	/* egress.c --------------------------------------------------- */
	int	egress(
			char *	path,
			int	mode
			)
			;

	/* execute.c -------------------------------------------------- */
	int	execute(
			const char *	verb,
			const char *	args
			)
			;
	int	executev(
			char **		args
			)
			;

	/* file2arg.c ------------------------------------------------- */
	int	file2argv(
			const char *	name,
			char ***	vec
			)
			;

	/* file2mem.c ------------------------------------------------- */
	char *	file2mem(
			const char *	name
			)
			;

	/* fileblox.c ------------------------------------------------- */
#if	defined(SYS_UNIX)
#if	defined(STAT_HAS_ST_BLOCKS)
#define	fileblocks(sb)	(sb)->st_blocks
#else
	long	fileblocks(
			const Stat_t *	sb
			)
			;
#endif
#endif

	/* filecopy.c ------------------------------------------------- */
#if	defined(SYS_UNIX) || defined(MSDOS)
	int	filecopy(
			const char *	src,
			const char *	dst,
			int	copy
			)
			;

	/* filesize.c ------------------------------------------------- */
	off_t	filesize(
			const char *	name
			)
			;
#endif	/* SYS_UNIX or MSDOS */

	/* fleaf.c ---------------------------------------------------- */
	char *	fleaf_delim(
			const char *	path
			)
			;
	char *	fleaf(
			const char *	path
			)
			;

	/* fleaf14.c -------------------------------------------------- */
	char *	fleaf14(
			char *	path
			)
			;

	/* for_admn.c ------------------------------------------------- */
#ifdef	SYS_UNIX
	int	for_admin2(
			int (*func)	(void),
			uid_t		the_uid,
			gid_t		the_gid
			)
			;

	int	for_admin(
			int (*func)	(void)
			)
			;

	/* for_user.c ------------------------------------------------- */
	int	for_user2(
			void (*func)	(void),
			uid_t		the_uid,
			gid_t		the_gid
			)
			;

	int	for_user(
			void (*func)	(void)
			)
			;
#endif	/* SYS_UNIX */

	/* fp2argv.c -------------------------------------------------- */
	int	fp2argv(
			FILE *	fp,
			char ***	argv_,
			void (*tfunc)	(char * s)
			)
			;

	/* ftype.c ---------------------------------------------------- */
	char *	ftype(
			char *	path
			)
			;

	/* ftype2.c --------------------------------------------------- */
#ifdef	SYS_UNIX
	char *	ftype2(
			char *	path
			)
			;

	/* gethome.c -------------------------------------------------- */
	char *	gethome(void)
			;

	/* getuser.c -------------------------------------------------- */
	char *	getuser(void)
			;
#endif	/* SYS_UNIX */

	/* gid2s.c ---------------------------------------------------- */
	char *	gid2s(
			gid_t	gid
			)
			;

	/* in_group.c ------------------------------------------------- */
#ifdef SYS_UNIX
	int	in_group(
			gid_t	gid
			)
			;
#endif

	/* interact.c ------------------------------------------------- */
	int	interactive(void)
			;

	/* is_spath.c ------------------------------------------------- */
	int	is_subpath(
			char *	ref,
			char *	tst
			)
			;

	/* istextfl.c ------------------------------------------------- */
	int	istextfile(
			const char *	name
			)
			;

	/* lsbycols.c ------------------------------------------------- */
	void	list_by_cols(
			const char **	listp,
			int		sizep,
			int		num
			)
			;

	/* logch.c ---------------------------------------------------- */
	int	decode_logch(
			char **	buffer,
			int *	count_
			)
			;

	void	encode_logch(
			char *	buffer,
			int *	count_,
			int	c
			)
			;

	/* ltostr.c --------------------------------------------------- */
	char *	l2str(
			char *	bfr,
			long	value,
			int	base
			)
			;

	/* makeargv.c ------------------------------------------------- */
	int	makeargv(
			char **		argv,
			int		maxarg,
			char *		dst,
			const char *	src
			)
			;

	/* mem2file.c ------------------------------------------------- */
	int	mem2file(
			char *	blob,
			char *	name,
			char *	mode
			)
			;

	/* modechar.c ------------------------------------------------- */
	int	modechar(
			unsigned	mode
			)
			;

	/* name2s.c --------------------------------------------------- */
	int	name2s(
			char *		bfr,
			int		maxbfr,
			const char *	name,
			int		opt
			)
			;

	/* newzone.c -------------------------------------------------- */
#ifdef	SYS_UNIX
extern	int	localzone;

	void	newzone(
			int	hours,
			int	minutes,
			int	apres
			)
			;

	void	oldzone(void)
			;
#endif	/* SYS_UNIX */
	/* next_ver.c ------------------------------------------------- */
	void	next_version(
			char *	dst,
			char *	src
			)
			;

	/* packdate.c ------------------------------------------------- */
	long	gmt_offset(
			time_t	t
			)
			;

	long	packdate (
			int	year,
			int	mon,
			int	day,
			int	hour,
			int	min,
			int	s
			)
			;

	/* padedit.c -------------------------------------------------- */
#ifdef	SYS_UNIX
	int	padedit(
			const char *	name,
			int		readonly,
			const char *	editor
			)
			;
#endif	/* SYS_UNIX */

	/* pathcat.c -------------------------------------------------- */
#if	defined(SYS_UNIX) || defined(MSDOS)
	char *	pathcat2(
			char * dst,
			const char * p,
			const char * n
			)
			;

	char *	pathcat(
			char * dst,
			const char * p,
			const char * n
			)
			;

	/* pathcmp.c -------------------------------------------------- */
	int	pathcmp(
			const char *	p1,
			const char *	p2
			)
			;

	/* pathhead.c ------------------------------------------------- */
	char *	pathhead(
			const char *	path,
			Stat_t *	s
			)
			;

	/* pathleaf.c ------------------------------------------------- */
	char *	pathleaf(
			const char *	path
			)
			;

	/* relpath.c -------------------------------------------------- */
	char *	relpath(
			char *		dst,
			const char *	cwd,
			const char *	src
			)
			;
#endif	/* SYS_UNIX or MSDOS */

	/* revert.c --------------------------------------------------- */
#ifdef	SYS_UNIX
	int	revert(
			const char *	msg
			)
			;

	/* s2gid.c ---------------------------------------------------- */
	int	s2gid(
			const char *	s
			)
			;

	/* s2uid.c ---------------------------------------------------- */
	int	s2uid(
			const char *	s
			)
			;
#endif	/* SYS_UNIX */

	/* samehead.c ------------------------------------------------- */
	int	samehead(
			const char *	path,
			const char *	leaf
			)
			;

	/* sameleaf.c ------------------------------------------------- */
	int	sameleaf(
			const char *	path,
			const char *	leaf
			)
			;

	/* savesuid.c ------------------------------------------------- */
#ifdef	SYS_UNIX
	int	saves_uid(void)
			;

	/* scr_size.c ------------------------------------------------- */
	int	scr_size(
			int *	lc
			)
			;
#endif	/* SYS_UNIX */

	/* setmtime.c ------------------------------------------------- */
	int	setmtime(
			const char *	name,
			time_t		mtime,
			time_t		atime
			)
			;

	/* shoarg.c --------------------------------------------------- */
	char 	*bldcmd (
			char *		dst,
			const char *	src,
			size_t		len
			)
			;

	char 	*bldcmd2 (
			char *	dst,
			const char *	src1,
			const char *	src2,
			size_t	len
			)
			;

	void	shoarg (
			FILE *	fp,
			const char *	command,
			const char *	args
			)
			;

	/* showuids.c ------------------------------------------------- */
	void	show_uids(
			FILE *	fp
			)
			;

	/* stat_dir.c ------------------------------------------------- */
	int	stat_dir(
			const char *	path,
			Stat_t *	sb
			)
			;

	/* stat_fil.c ------------------------------------------------- */
	int	stat_file(
			const char *	path,
			Stat_t *	sb
			)
			;

	/* stralloc.c ------------------------------------------------- */
	char *	stralloc(
			const char *	string
			)
			;

	void	strfree(
			char *	string
			)
			;

	/* strbcmp.c -------------------------------------------------- */
	int	strbcmp(
			const char *	a,
			const char *	b
			)
			;

	/* strclean.c ------------------------------------------------- */
	int	strclean(
			char *	string
			)
			;

	/* strcount.c ------------------------------------------------- */
	int	strcount(
			char *	s,
			int	c
			)
			;

	/* strerror.c ------------------------------------------------- */
#if !defined(HAVE_STRERROR)
	char *	strerror(
			int	err
			)
			;
#endif

	/* strtrim.c -------------------------------------------------- */
	char *	strtrim(
			char *	string
			)
			;

	/* strucmp.c -------------------------------------------------- */
	int	strucmp(
			const char *	a,
			const char *	b
			)
			;

	/* strucpy.c -------------------------------------------------- */
	char *	strucpy(
			char *		a,
			const char *	b
			)
			;

	char *	strlwrcpy(
			char *		a,
			const char *	b
			)
			;

	/* strwcmp.c -------------------------------------------------- */
	int	strwcmp(
			const char *	wild,
			const char *	name
			)
			;

	/* track_wd.c ------------------------------------------------- */
	void	track_wd(
			const char *	path
			)
			;

	/* transtree.c ------------------------------------------------ */
	void	transtree(
			char *		path,
			int (*func)(char* n, Stat_t* s),
			int		recur,
			int		links
			)
			;

	/* trimpath.c ------------------------------------------------- */
	char *	trimpath(
			char *		path,
			const char *	cwd
			)
			;

	/* txtalloc.c ------------------------------------------------- */
	char *	txtalloc(
			const char *	string
			)
			;

	void	txtfree(
			char *		string
			)
			;

	void	free_txtalloc(void)
			;

	/* uid2s.c ---------------------------------------------------- */
#ifdef	apollo_sr10
	int	len_passwd;
	char	**vec_passwd;
#endif
	char *	uid2s(
			uid_t	uid
			)
			;

	/* usercopy.c ------------------------------------------------- */
#ifdef	SYS_UNIX
	int	usercopy(
			const char *	src,
			const char *	dst
			)
			;

	/* userprot.c ------------------------------------------------- */
	int	userprot(
			char *	name,
			int	mode,
			time_t	mtime
			)
			;
#endif	/* SYS_UNIX */

	/* vecalloc.c ------------------------------------------------- */
	char	**vecalloc(
			size_t	len
			)
			;

	void	vecfree(
			char **	v
			)
			;

	/* vecedit.c -------------------------------------------------- */
	char **	vecedit (
			char **	text,
			char **	script
			)
			;

	/* veclen.c --------------------------------------------------- */
	int	veclen (
			char **	vector
			)
			;

	/* vercmp.c --------------------------------------------------- */
	int	vercmp (
			const char * s1,
			const char * s2,
			int	wild
			)
			;

	/* viewfile.c ------------------------------------------------- */
#ifdef	SYS_UNIX
	int	view_file(
			char *	fname,
			int	readonly
			)
			;
#endif	/* SYS_UNIX */

	/* walkback.c ------------------------------------------------- */
#ifdef	SYS_UNIX
	void	walkback(
			char *	program
			)
			;
#endif	/* SYS_UNIX */

	/* walktree.c ------------------------------------------------- */
#if	defined(SYS_UNIX) || defined(MSDOS)

#define	WALK_FUNC_ARGS	\
			const char *	path,\
			const char *	name,\
			Stat_t *	sp,\
			int		readable,\
			int		level

#define	WALK_FUNC(f)	f(WALK_FUNC_ARGS)

	int	walktree(
			const char *	p,
			const char *	n,
			int (*f)	(WALK_FUNC_ARGS),
			const char *	m,
			int	lvl
			)
			;

	/* which.c ---------------------------------------------------- */
	int	which(
			char *		bfr,
			unsigned	len,
			const char *	find,
			const char *	dot
			)
			;
#endif	/* SYS_UNIX */

#endif	/* TD_LIB_H */
