/* $Id: ptypes.h,v 5.3 1989/12/07 13:46:35 dickey Exp $ */

#ifndef	_PTYPES_
#define	_PTYPES_

/*
 * The definitions in this file cover simple cases of bsd4.x/system5 porting,
 * as well as definitions useful for linting (e.g., void-casts, malloc pseudo-
 * functions).
 *
 * We include <stdio.h> here so that 'sprintf()' lints properly.
 * The <sys/types.h> file is needed so that we can include <sys/stat.h>
 * Distinguish bsd4.x from system5 by the presence of sockets.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef	S_IFSOCK
#if	S_IFSOCK == S_IFLNK
#define	SYSTEM5		/* apollo sr10.x sys5 */
#endif
#else			/* no sockets, assume bsd4.x */
#define	SYSTEM5
#endif

#ifndef	S_IFLNK
#define	lstat	stat
#endif	S_IFLNK

#ifdef	SYSTEM5
#define	getwd(p)	getcwd(p,sizeof(p)-2)
extern	char	*getcwd();
#else	/* !SYSTEM5 */
extern	char	*getwd();
#ifdef	unix		/* bsd4.x on SunOs? */
extern	char	*sprintf();
#endif
#endif

/*
 * Definitions for files which are combined lint-library/function-prototype
 * declarations (e.g., "common.h"):
 */
#ifdef	LINTLIBRARY
#define	_FN1(t,v)	v
#define	_FNX(t,v)	_FN1(t,v),
#define	_AR1(t,v)	v
#define	_ARX(t,v)	_AR1(t,v),
#define	_DCL(t,v)	t v;
#define	_RET		{return(0);}
#define	_NUL		{}
#else	!LINTLIBRARY
#ifdef	vms
#define	PROTOTYPES
#endif	vms
#ifdef	__STDC__
#define	PROTOTYPES
#endif	__STDC__
#ifdef	PROTOTYPES	/* function prototypes */
#define	FNC_PTYPES
#define	_FN1(t,v)	t (*v)()
#define	_FNX(t,v)	_FN1(t,v),
#define	_AR1(t,v)	t v
#define	_ARX(t,v)	_AR1(t,v),
#define	_DCL(t,v)
#define	_RET		;
#define	_NUL		;
#else	!PROTOTYPES	/* old-style declarations */
#define	_FN1(t,v)
#define	_FNX(t,v)
#define	_AR1(t,v)
#define	_ARX(t,v)
#define	_DCL(t,v)
#define	_RET		;
#define	_NUL		;
#endif	PROTOTYPES
#endif	LINTLIBRARY

/*
 * Declare argument for 'exit()' and '_exit()':
 */
#ifdef	vms
#include	<stdlib.h>	/* defines lots of useful stuff */
#include	<stsdef.h>
#define	SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#define	FAIL	(STS$M_INHIB_MSG | STS$K_ERROR)
#else	unix
#define	SUCCESS	(0)		/* if no error */
#define	FAIL	(1)		/* if any error */
#endif	vms/unix

/*
 * Declare functions which are int (or implicit) in some systems, but explicitly
 * void in system5.  Also, declare nuisance-types which are unsigned/int,
 * depending on the system:
 */
#ifdef	SYSTEM5
#define	V_OR_I	void
#define	LEN_QSORT	unsigned
#define	LEN_READ	unsigned
#else
#ifdef	vms
#define	V_OR_I		int
#define	LEN_QSORT	int
#define	LEN_READ	int
#else	unix
#define	V_OR_I
#define	LEN_QSORT	int
#define	LEN_READ	int
#endif	vms/unix
#endif	SYSTEM5

/* the type of return-value from "signal()" */
#if	defined(apollo) || !defined(NBBY)
#define	SIGS_T		int
#else
#define	SIGS_T		void
#endif

extern	V_OR_I	_exit();
extern	V_OR_I	exit();
#ifndef	vms
extern	V_OR_I	free();
#endif	vms
extern	V_OR_I	perror();
extern	V_OR_I	qsort();
#ifndef	vms
#ifndef	__STDC__
extern	V_OR_I	rewind();
#endif	__STDC__
#endif

#ifdef	unix
#if	defined(apollo) && defined(__STDC__)
extern	uid_t	getuid(), geteuid();
extern	gid_t	getgid(), getegid();
#else
#if	!defined(NBBY)
typedef	int	uid_t;
typedef	int	gid_t;
#endif	/* SunOs 3.5 (fixed in SunOs 4.0) */
#endif
#endif	unix

/*
 * Miscellaneous useful definitions for readability
 */
#ifndef	TRUE
#define	TRUE	(1)
#define	FALSE	(0)
#endif	TRUE

#define	EOS	'\0'

#define NULL_FUNC (int (*)())0

/*
 * Functions we (usually) ignore the return value from:
 */
#define	PRINTW	(void)printw
#define	PRINTF	(void)printf
#define	FPRINTF	(void)fprintf
#define	FORMAT	(void)sprintf
#define	FCLOSE	(void)fclose

/*
 * Define pseudo-functions to lint memory-allocator:
 */
#ifdef	lint
#define	def_DOALLOC(t)	/*ARGSUSED*/ static t *def_doalloc(p,n)\
						t *p; unsigned n;\
						{return(0);}
#define	def_ALLOC(t)	/*ARGSUSED*/ static t *def_alloc(n)\
						unsigned n;\
						{return(0);}
#define	DOALLOC(p,t,n)	def_doalloc(p,n)
#define	ALLOC(t,n)	def_alloc(n)
#else	lint
#define	def_DOALLOC(t)
#define	def_ALLOC(t)
#define	DOALLOC(p,t,n)	(t *)doalloc((char *)p,sizeof(t)*(n))
#define	ALLOC(t,n)	DOALLOC(0,t,n)
#endif	lint

#ifndef	vms
extern	char	*doalloc();
extern	char	*malloc(), *realloc();
#endif	vms

/*
 * System5 does not provide the directory manipulation procedures in bsd4.x;
 * define macros so we can use the bsd4.x names:
 */
#ifdef	DIR_PTYPES
#ifdef	vms
#include	"unixdir.h"	/* get this from PORTUNIX */
#else	unix
#include	<sys/dir.h>
#ifdef	SYSTEM5
#define	DIR	FILE
#define	opendir(n)	fopen(n,"r")
#define	readdir(fp)	(fread(&dbfr, sizeof(dbfr), 1, fp)\
				? &dbfr\
				: (struct direct *)0)
#define	closedir(fp)	FCLOSE(fp)
static	struct	direct	dbfr;
#endif	SYSTEM5
#endif	vms/unix
#endif	DIR_PTYPES

/*
 * System5 curses does not define the 'screen' structure
 */
#ifdef	CUR_PTYPES
#ifdef	SYSTEM5
struct	screen	{ int dummy; };
#else	SYSTEM5
typedef char	chtype;		/* sys5-curses data-type */
#ifndef	erasechar
extern	char	erasechar();
#endif	erasechar
#ifndef	killchar
extern	char	killchar();
#endif	killchar
#endif	SYSTEM5
#include	<curses.h>
#endif	CUR_PTYPES

/*
 * Define symbols used in 'access()' function
 */
#ifdef	ACC_PTYPES
#ifdef	vms
#define	F_OK	0	/* file-exists */
#define	X_OK	1	/* executable */
#define	W_OK	2	/* writeable */
#define	R_OK	4	/* readable */
#else	unix
#ifdef	SYSTEM5
#include	<unistd.h>
#else	SYSTEM5
#include	<sys/file.h>
#endif	SYSTEM5
#endif	vms/unix
#endif	ACC_PTYPES

/*
 * Define string-procedures
 */
#ifdef	STR_PTYPES
#include	<string.h>
#ifndef	SYSTEM5
#define	strchr	index
#define	strrchr	rindex
#endif	SYSTEM5
extern	char	*strchr();
extern	char	*strrchr();
#endif	STR_PTYPES

/*
 * Definitions of procedures in CM_TOOLS common library
 */
#ifdef	FNC_PTYPES
#include "common.h"
#endif	FNC_PTYPES

#endif	_PTYPES_
