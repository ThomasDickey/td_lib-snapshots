/* $Id: ptypes.h,v 12.2 1993/09/24 19:15:17 dickey Exp $ */

#ifndef	_PTYPES_
#define	_PTYPES_

/*
 * The definitions in this file cover simple cases of bsd4.x/system5 porting,
 * as well as definitions useful for linting (e.g., void-casts, malloc pseudo-
 * functions).
 *
 * Define any of the following before including this file to turn on special
 * definitions:
 *	ACC_PTYPES	<sys/file.h>
 *	CHR_PTYPES	<ctype.h>
 *	DIR_PTYPES	<dirent.h>
 *	PWD_PTYPES	<pwd.h>
 *	SIG_PTYPES	<signal.h>
 *	STR_PTYPES	<string.h>
 *	WAI_PTYPES	<wait.h>
 *
 * We include <stdio.h> here so that 'sprintf()' lints properly.
 * The <sys/types.h> file is needed so that we can include <sys/stat.h>
 * Distinguish bsd4.x from system5 by the presence of sockets.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

	/* useful stuff for <sys/stat.h> */
#define	STAT	struct	stat
#define	isDIR(mode)	((mode & S_IFMT) == S_IFDIR)
#define	isFILE(mode)	((mode & S_IFMT) == S_IFREG)
#ifdef	S_IFLNK
#define	isLINK(mode)	((mode & S_IFMT) == S_IFLNK)
#endif

#ifndef	P_tmpdir
#define	P_tmpdir	"/usr/tmp"
#ifdef	L_tmpnam
#ifdef _SYS_STDSYMS_INCLUDED
#define	apollo_sr10
#endif
#else
#define	L_tmpnam	32
#ifdef	apollo
#define	apollo_sr10	/* ...must be independent of '__STDC__' for lint */
#endif	/* apollo */
#endif	/* L_tmpnam */
#endif	/* P_tmpdir */

#ifdef	apollo
#ifdef	CLASSIC_SYS5_FUNC
#define	SYSTEM5		/* apollo sr10.x sys5 */
#endif
#ifdef	_DECL_FUNC	/* apollo sr10.3 */
#ifndef	lint		/* too many void *'s */
#define	HAS_STDLIB 1
#endif
#endif
#endif

#ifndef HAS_STDLIB
#if defined(sun) || defined(vms)
#define	HAS_STDLIB 1
#else
#define HAS_STDLIB 0
#endif
#endif

#if defined(sun) || defined(SYSTEM5)
#define HAS_UNISTD 1
#endif

#if HAS_STDLIB
#include	<stdlib.h>
#endif

#if HAS_UNISTD
#include	<unistd.h>
#endif

#ifndef	S_IFLNK
#define	lstat	stat
#endif	/* S_IFLNK */

/*
 * Definition which is true iff we use function-prototypes
 */
#undef	PROTOTYPES
#if	defined(vms) || (defined(__STDC__) && !defined(LINTLIBRARY))
#define	PROTOTYPES	1
#endif

/*
 * Definitions for files which are combined lint-library/function-prototype
 * declarations (e.g., "td_lib.h"):
 */
#ifdef	LINTLIBRARY
#define	_fn1(t,v,a)	v
#define	_fnx(t,v,a)	_fn1(t,v,a),
#define	_ar0
#define	_ar1(t,v)	v
#define	_arx(t,v)	_ar1(t,v),
#define	_dcl(t,v)	t v;
#define	_ret		{return(0);}
#define	_nul		{}
#else	/* !LINTLIBRARY */
#if	PROTOTYPES	/* function prototypes */
#define	_fn1(t,v,a)	t (*v)a
#define	_fnx(t,v,a)	_fn1(t,v,a),
#define	_ar0		void
#define	_ar1(t,v)	t v
#define	_arx(t,v)	_ar1(t,v),
#define	_dcl(t,v)
#define	_ret		;
#define	_nul		;
#else	/* -- old-style declarations */
#define	_fn1(t,v,a)
#define	_fnx(t,v,a)
#define	_ar0
#define	_ar1(t,v)
#define	_arx(t,v)
#define	_dcl(t,v)
#define	_ret		;
#define	_nul		;
#endif	/* PROTOTYPES */
#endif	/* LINTLIBRARY */

/*
 * Define special macros to represent the "..." ellipsis
 */
#if	PROTOTYPES
#define	_DOTS	...
#define	_CDOTS	,...
#else
#define	_DOTS
#define	_CDOTS
#endif

/*
 * Macros to use with actual procedures to make them use prototypes if the
 * compiler supports it:
 */
#if	PROTOTYPES
#define	_FN1(t,v,a)	t (*v)a
#define	_AR1(t,v)	t v
#define	_AR0		void
#define	_DCL(t,v)
#else
#define	_FN1(t,v,a)	v
#define	_AR1(t,v)	v
#define	_AR0
#define	_DCL(t,v)	t v;
#endif

#define	_FNX(t,v,a)	_FN1(t,v,a),
#define	_ARX(t,v)	_AR1(t,v),

#define	_ONE(t,v)	(_AR1(t,v)) _DCL(t,v)

/*
 * Declare argument for 'exit()' and '_exit()':
 */
#ifdef	vms
#include	<stsdef.h>
#define	SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#define	FAIL	(STS$M_INHIB_MSG | STS$K_ERROR)
#else	/* unix */
#define	SUCCESS	(0)		/* if no error */
#define	FAIL	(1)		/* if any error */
#endif	/* vms/unix */

/*
 * Declare functions which are int (or implicit) in some systems, but explicitly
 * void in system5.  Also, declare nuisance-types which are unsigned/int,
 * depending on the system:
 */
#ifdef	SYSTEM5
#define	V_OR_I		void
#define	LEN_QSORT	unsigned
#define	LEN_READ	unsigned
#else
#ifdef	vms
#define	V_OR_I		void
#define	LEN_QSORT	int
#define	LEN_READ	int
#define	LEN_FREAD	size_t
#else	/* unix */
#define	LEN_QSORT	int
#define	LEN_READ	int
#if	defined(__STDC__) || defined(apollo)
#define	LEN_FREAD	size_t
#else
#define	LEN_FREAD	int
#endif	/* apollo */
#endif	/* vms/unix */
#endif	/* SYSTEM5 */

#ifdef	sun
#define	V_OR_I		int
#define	V_OR_I2		void
#endif

#ifdef	apollo_sr10
#define	V_OR_I		void
#endif

#ifndef	V_OR_I
#define	V_OR_I		/**/
#endif

#ifndef	V_OR_I2
#define	V_OR_I2		V_OR_I
#endif

#ifdef	__STDC__
#define	V_OR_P		void *
#else
#define	V_OR_P		char *
#endif

/******************************************************************************
 * Useful external-definitions                                                *
 ******************************************************************************/
#ifndef	LINTLIBRARY

#if	defined(SYSTEM5) || defined(vms)
#define	getwd(p)	getcwd(p,sizeof(p)-2)
extern	char	*getcwd();
#else	/* !SYSTEM5 */
extern	char	*getwd(_ar1(char *,p));
#if	defined(unix) && !defined(apollo) && !defined(__GNUC__)	/* bsd4.x on SunOs? */
extern	char	*sprintf(_arx(char *,fmt) _DOTS);
#endif
#endif

#if	!HAS_STDLIB
extern	V_OR_I	_exit(_ar1(int,code));
extern	V_OR_I2	exit(_ar1(int,code));
extern	V_OR_I	free(_ar1(char *,s));
extern	V_OR_P	calloc(_arx(size_t,nel) _ar1(size_t,size));
extern	V_OR_P	malloc(_ar1(size_t,size));
extern	V_OR_P	realloc(_arx(V_OR_P,ptr) _ar1(size_t,size));
#endif
#ifndef	vms
#if	!defined(apollo_sr10)
extern	V_OR_I2	perror (_ar1(char *,s));
extern	V_OR_I	rewind (_ar1(FILE *,s));
#endif

extern	char *	getenv (_ar1(char *,s));
extern	int	mkstemp(_ar1(char *,s));
extern	char *	mktemp (_ar1(char *,s));
extern	long	strtol(
		_arx(char *,	s)
		_arx(char **,	d)
		_ar1(int,	base));
extern	time_t	time   (_ar1(time_t *,t));

#endif	/* !vms */

extern	int	getopt(
		_arx(int,	argc)
		_arx(char **,	argv)
		_ar1(char *,	opts));
extern	char *	optarg;
extern	int	optind;

#ifdef	unix
#ifdef	apollo_sr10
extern	uid_t	getuid(), geteuid();
extern	gid_t	getgid(), getegid();
#else
#if	!defined(NBBY)
typedef	int	uid_t;
typedef	int	gid_t;
#endif	/* SunOs 3.5 (fixed in SunOs 4.0) */
#endif

/*
 * Common definitions that aren't declared properly
 */
#if defined(__GNUC__) && defined(sun)
extern	int	_filbuf	(FILE *);
extern	int	_flsbuf	(int, FILE *);
extern	void	bzero	(char *, int);
extern	int	creat	(char *, int);
extern	int	fclose	(FILE *);
extern	int	fflush	(FILE *);
extern	int	fgetc	(FILE *);
extern	int	fork	(void);
extern	int	fprintf (FILE *, char *, ...);
extern	int	fputc	(int, FILE *);
extern	int	fputs	(char *, FILE *);
extern	int	fread	(char *, int, int, FILE *);
extern	int	fscanf	(FILE *, char *, ...);
extern	int	fseek	(FILE *, long, int);
extern	int	fwrite	(char *, int, int, FILE *);
extern	int	setegid	(gid_t);
extern	int	setlinebuf(FILE *);
extern	int	setruid	(uid_t);
extern	int	setrgid	(uid_t);
extern	int	ioctl	(int, int, caddr_t);
extern	int	lstat	(char *, STAT *);
extern	int	open	(char *, int, int);
extern	int	pclose	(FILE *);
extern	int	printf	(char *, ...);
extern	int	putenv	(char *);
extern	int	puts	(char *);
extern	int	readlink(char *, char *, int);
extern	int	rename	(char *, char *);
extern	int	sscanf	(char *, ...);
extern	void	setbuf	(FILE *, char *);
extern	int	symlink	(char *, char *);
extern	int	system	(char *);
extern	int	tgetent	(char *, char *);
extern	int	tgetnum	(char *);
extern	int	ungetc	(int, FILE *);
extern	int	vfork	(void);
#endif

#endif	/* unix */

#endif	/* !LINTLIBRARY */

/*
 * Miscellaneous useful definitions for readability
 */
#ifndef	TRUE
#define	TRUE	(1)
#define	FALSE	(0)
#endif	/* TRUE */

#define	EOS	'\0'

#define	SIZEOF(v)	(sizeof(v)/sizeof(v[0]))

#ifndef	MAXPATHLEN
#define	MAXPATHLEN	256
#endif

#define NULL_FUNC (int (*)())0

#ifndef NO_LEAKS
#define NO_LEAKS 1
#endif

/*
 * Functions we (usually) ignore the return value from:
 */
#define	PRINTW	(void)printw
#define	PRINTF	(void)printf
#define	FPRINTF	(void)fprintf
#define	FORMAT	(void)sprintf
#define	FFLUSH	(void)fflush
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
#else	/* lint */
#define	def_DOALLOC(t)
#define	def_ALLOC(t)
#define	DOALLOC(p,t,n)	(t *)doalloc((char *)p,sizeof(t)*(n))
#define	ALLOC(t,n)	DOALLOC(0,t,n)
#endif	/* lint */

/******************************************************************************
 * Define symbols used in 'access()' function                                 *
 ******************************************************************************/
#ifdef	ACC_PTYPES
#ifdef	vms
#define	F_OK	0	/* file-exists */
#define	X_OK	1	/* executable */
#define	W_OK	2	/* writeable */
#define	R_OK	4	/* readable */
#else	/* unix */
#ifndef	SYSTEM5
#include	<sys/file.h>
#endif	/* SYSTEM5 */
#endif	/* vms/unix */
#endif	/* ACC_PTYPES */

/******************************************************************************
 * character definitions                                                      *
 ******************************************************************************/
#ifdef	CHR_PTYPES
#include <ctype.h>

#if defined(__GNUC__) && defined(sun)
extern	int	tolower(int);
extern	int	toupper(int);
#endif

#if defined(tolower) || defined(sun)
#define LowerMacro(c) tolower(c)
#define UpperMacro(c) toupper(c)
#endif

#ifdef _tolower
#define LowerMacro(c) _tolower(c)
#define UpperMacro(c) _toupper(c)
#endif

#ifndef LowerCase
#define LowerCase(c) if (isascii(c) && isupper(c)) c = LowerMacro(c)
#define UpperCase(c) if (isascii(c) && islower(c)) c = UpperMacro(c)
#endif

#endif	/* CHR_PTYPES */

/******************************************************************************
 * System5 does not provide the directory manipulation procedures in bsd4.x;  *
 * define macros so we can use the bsd4.x names:                              *
 ******************************************************************************/
#ifdef	DIR_PTYPES
#ifdef	vms
#include	"unixdir.h"	/* get this from PORTUNIX */
#else	/* unix */
#ifdef	sun			/* SunOs 4.1 */
#include	<dirent.h>
#define	direct	dirent
#else				/* apollo & other old bsd's */
#include	<sys/dir.h>
#endif
#ifdef	SYSTEM5
#define	DIR	FILE
#define	opendir(n)	fopen(n,"r")
#define	readdir(fp)	(fread(&dbfr, sizeof(dbfr), 1, fp)\
				? &dbfr\
				: (struct direct *)0)
#define	closedir(fp)	FCLOSE(fp)
static	struct	direct	dbfr;
#endif	/* SYSTEM5 */
#endif	/* vms/unix */
#define	DIRENT	struct	direct
#endif	/* DIR_PTYPES */

/******************************************************************************
 * defines externals used in password-file routines                           *
 ******************************************************************************/
#ifdef	PWD_PTYPES

#include <pwd.h>

extern	struct passwd *	getpwnam(_ar1(char *,name));
extern	struct passwd *	getpwuid(_ar1(int,uid));

extern	struct passwd *	getpwent(_ar0);
extern	V_OR_I		setpwent(_ar0);
extern	V_OR_I		endpwent(_ar0);

#endif	/* PWD_PTYPES */

/******************************************************************************
 * defines the type of return-value from "signal()"                           *
 ******************************************************************************/
#ifdef	SIG_PTYPES

#include <signal.h>

#undef	SIG_T
#undef	SIGNAL_FUNC
#undef	DCL_SIGNAL

#define	SIG_T	void

#ifdef	apollo
#  ifdef	__SIG_HANDLER_T
#    define	SIGNAL_ARGS _AR1(int,sig) _CDOTS
#    define	SIGNAL_args _ar1(int,sig) _CDOTS
#  else	/* sr10.2 or lower */
#    undef	SIG_T
#    define	SIG_T	int
#  endif
#endif

#ifndef SIGNAL_ARGS
#  define SIGNAL_ARGS _AR1(int,sig)
#  define SIGNAL_args _ar1(int,sig)
#endif

#ifndef	SIGNAL_FUNC
#  define	SIGNAL_FUNC(f)	SIG_T f (SIGNAL_ARGS) _DCL(int,sig)
#endif

#define	DCL_SIGNAL(func)	SIG_T	(*func)(SIGNAL_args)

#if defined(__GNUC__)
# undef  SIG_DFL
# undef  SIG_IGN
# define SIG_DFL	(SIG_T (*)(SIGNAL_ARGS))0
# define SIG_IGN	(SIG_T (*)(SIGNAL_ARGS))1
#endif

#endif	/* SIG_PTYPES */

/******************************************************************************
 * Define string-procedures                                                   *
 ******************************************************************************/
#ifdef	STR_PTYPES
#ifndef LINTLIBRARY
#include	<string.h>
#if	!defined(SYSTEM5) && !defined(vms) && !defined(apollo) && !defined(sun)
#define	strchr	index
#define	strrchr	rindex
#endif	/* SYSTEM5 */
extern	char *	strchr (_arx(char *,s) _ar1(int,c));
extern	char *	strrchr(_arx(char *,s) _ar1(int,c));
extern	char *	strtok (_arx(char *,s) _ar1(char *,t));
#endif
#endif	/* STR_PTYPES */

/******************************************************************************
 * defines the argument-type for "wait()"                                     *
 ******************************************************************************/
#ifdef	WAI_PTYPES
#ifdef	vms
#include <processes.h>
#else
#include <sys/wait.h>
#endif

#undef	ARG_WAIT
#undef	DCL_WAIT

#if	defined(sun) || defined(apollo_sr10) || defined(SYSTEM5)
#define	ARG_WAIT(status)	((int *)&status)
#endif
#ifdef	SYSTEM5
#define	DCL_WAIT(status)	int status
#endif

#ifndef	ARG_WAIT
#define	ARG_WAIT(status)	((union wait *)&status)
#endif
#ifndef	DCL_WAIT
#define	DCL_WAIT(status)	union wait status
#endif

#ifdef	SYSTEM5
#define	W_RETCODE(status)	((status >> 8) & 0xff)
#else	/* !SYSTEM5 */
#define	W_RETCODE(status)	(status.w_retcode)
#endif	/* SYSTEM5/!SYSTEM5 */

#endif	/* WAI_PTYPES */

/******************************************************************************
 * Definitions of procedures in TD_LIB common library                         *
 ******************************************************************************/
#ifndef	LINTLIBRARY
#include <td_lib.h>

#ifndef	lint
extern	void	main(_arx(int,argc) _arx(char **,argv) _ar1(char **,envp));
#endif	/* lint */
#define	_MAIN\
	void	main(_ARX(int,argc) _ARX(char **,argv) _AR1(char **,envp))\
		     _DCL(int,argc) _DCL(char **,argv) _DCL(char **,envp)

#endif	/* LINTLIBRARY */

#endif	/* _PTYPES_ */
