/* $Id: ptypes.h,v 12.35 1995/01/31 00:38:26 tom Exp $ */

#ifndef	PTYPES_H
#define	PTYPES_H

#if HAVE_CONFIG_H
#include "config.h"
#endif

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
 *	OPN_PTYPES	<fcntl.h>
 *	PWD_PTYPES	<pwd.h>
 *	SIG_PTYPES	<signal.h>
 *	STR_PTYPES	<string.h>
 *	TIM_PTYPES	<time.h>
 *	TRM_PTYPES	<termio.h>
 *	WAI_PTYPES	<wait.h>
 *
 * We include <stdio.h> here so that 'sprintf()' lints properly.
 * The <sys/types.h> file is needed so that we can include <sys/stat.h>
 * Distinguish bsd4.x from system5 by the presence of sockets.
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#if HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif

	/* useful stuff for <sys/stat.h> */
#define	Stat_t	struct	stat	/* STAT conflicts with 'dist' package */
#define	isDIR(mode)	((mode & S_IFMT) == S_IFDIR)
#define	isFILE(mode)	((mode & S_IFMT) == S_IFREG)
#ifdef	S_IFLNK
#define	isLINK(mode)	((mode & S_IFMT) == S_IFLNK)
#endif

	/* known special systems */
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
#ifdef	lint		/* too many void *'s */
#undef	HAVE_STDLIB_H
#endif
#endif

#if	defined(__TURBOC__) && !defined(MSDOS)
#define	MSDOS
#endif

	/* types that aren't always defined */
#ifdef	MSDOS
typedef	long	off_t;
typedef	short	dev_t;
typedef	short	ino_t;
#endif

#ifdef	vms
#define	PATHLIST_SEP ','
#define PATH_DELIMS  ":[]"
#define isSlash(c)   not_implemented
#endif

#ifdef	MSDOS
#define PATHLIST_SEP ';'
#define PATH_DELIMS  "\\/"
#define PATH_SLASH   '\\'
#define isSlash(c)   ((c) == PATH_SLASH || (c) == '/')
#endif

#ifndef	PATHLIST_SEP
#define	PATHLIST_SEP ':'
#define PATH_DELIMS  "/"
#define PATH_SLASH   '/'
#define	isSlash(c)   ((c) == PATH_SLASH)
#endif

#if	defined(__hpux) || defined(__svr4__) || defined(linux)
#define	SYSTEM5
#endif

#if	!defined(MSDOS) && !defined(vms) && !defined(unix)
#define	unix		/* it's close enough for me */
#endif

#ifdef	MSDOS
#undef  SYSTEM5
#define STDC_HEADERS 1	/* ...at least for TurboC */
#define HAVE_FCNTL_H 1
#define	HAVE_STDLIB_H 1
#define	HAVE_STDARG_H 1
#undef  HAVE_UNISTD_H
#define HAVE_TIMEZONE 1
#define HAVE_NEW_TOKEN_SPLICE 1
#define HAVE_NEW_TOKEN_QUOTE  1
#include <io.h>		/* for 'chmod()' */
#endif

#if defined(vms)	/* VAX/VMS 5.3 */
#define	HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1
#undef  HAVE_UNISTD_H
#define HAVE_OLD_TOKEN_SPLICE 1
#define HAVE_OLD_TOKEN_QUOTE  1
#endif

#if HAVE_STDLIB_H
#include	<stdlib.h>
#endif

#if HAVE_UNISTD_H
#include	<unistd.h>
#endif

#if !HAVE_LSTAT
#define	lstat	stat
#endif

/*
 * Definition which is true iff we use function-prototypes
 */
#undef	PROTOTYPES
#if	defined(vms) || defined(__TURBOC__) || (defined(__STDC__) && !defined(LINTLIBRARY))
#define	PROTOTYPES	1
#endif
/*FIXME: make a test for prototypes */

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
 * The ARGS() macro is used for general extern-prototypes
 */
#if	PROTOTYPES
#define	ARGS(p) p
#else
#define	ARGS(p) ()
#endif

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

	/* shorthand for single-argument */
#define	_ONE(t,v)	(_AR1(t,v)) _DCL(t,v)
#define _one(t,a)	(_ar1(t,a))

/*
 * Declare functions which are int (or implicit) in some systems, but explicitly
 * void in system5.  Also, declare nuisance-types which are unsigned/int,
 * depending on the system:
 */
#ifdef	SYSTEM5
#define	V_OR_I		void
#define	LEN_QSORT	unsigned
#define	LEN_READ	unsigned
#define	LEN_FREAD	size_t
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

#if	defined(sun) && !defined(V_OR_I)
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

#if	defined(__STDC__) || defined(__TURBOC__)
#define	V_OR_P		void *
#else
#define	V_OR_P		char *
#endif

/*
 * Miscellaneous useful definitions for readability
 */
#if	!defined(TRUE) || (TRUE != 1)
#undef  TRUE
#define	TRUE	(1)
#endif

#if	!defined(FALSE) || (FALSE != 0)
#undef  FALSE
#define	FALSE	(0)
#endif

#define	EOS	'\0'

#define	SIZEOF(v)	(sizeof(v)/sizeof(v[0]))

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

#ifdef	unix
#  if HAVE_SYS_FILE_H
#    include	<sys/file.h>
#  endif
#endif	/* vms/unix */

#ifdef	MSDOS
#  include <io.h>
#endif

#ifndef F_OK
#  define	F_OK	0	/* file-exists */
#  define	X_OK	1	/* executable */
#  define	W_OK	2	/* writeable */
#  define	R_OK	4	/* readable */
#endif

#endif	/* ACC_PTYPES */

/******************************************************************************
 * character definitions                                                      *
 ******************************************************************************/
#ifdef	CHR_PTYPES
#include <ctype.h>

#if STDC_HEADERS || HAVE_TOLOWER
# define LowerMacro(c) tolower(c)
# define UpperMacro(c) toupper(c)
#else
# if defined(_tolower)
#  define LowerMacro(c) _tolower(c)
#  define UpperMacro(c) _toupper(c)
# endif
#endif

#if STDC_HEADERS
#define LowerCase(c) c = LowerMacro(c)
#define UpperCase(c) c = UpperMacro(c)
#else
#define LowerCase(c) if (isascii(c) && isupper(c)) c = LowerMacro(c)
#define UpperCase(c) if (isascii(c) && islower(c)) c = UpperMacro(c)
#endif

#ifndef isascii
#define	isascii(c) (!(c & ~0x7f))	/* e.g., 7-bit ASCII */
#endif

#endif	/* CHR_PTYPES */

/******************************************************************************
 * Define macros for directory-scanning                                       *
 ******************************************************************************/
#ifdef	DIR_PTYPES

#ifdef	vms
#  include	"unixdir.h"	/* get this from PORTUNIX */
#else
#  if defined(MSDOS)
#    include <dir.h>
#  else	/* unix */
#    if defined(HAVE_DIRENT_H)
#      include	<dirent.h>
#      define	direct	dirent	/* so <sys/dir.h> looks like <dirent.h> */
#      define   d_namlen d_reclen
#    else
#      if defined(HAVE_SYS_DIR_H)
#        include	<sys/dir.h>
#      else
#        if defined(HAVE_SYS_NDIR_H)
#          include	<sys/ndir.h>
#        else			/* ...must be an antique unix clone */
#          define	DIR	FILE
#          define	opendir(n)	fopen(n,"r")
#          define	readdir(fp)	(fread(&dbfr, sizeof(dbfr), 1, fp)\
						? &dbfr\
						: (struct direct *)0)
#          define	closedir(fp)	FCLOSE(fp)
static	struct	direct	dbfr;
#       endif		/* SYSNDIR/... */
#     endif		/* SYSDIR/... */
#   endif		/* DIRENT/... */
# endif			/* MSDOS/unix */
#endif			/* vms/MSDOS/unix */

#define	DirentT struct	direct

#endif	/* DIR_PTYPES */

/******************************************************************************
 * defines externals used in error-reporting/handling                         *
 ******************************************************************************/
#ifdef	ERR_PTYPES

#include <errno.h>

# if HAVE_SYS_ERRLIST
extern char *sys_errlist[];
extern int sys_nerr;
# endif

#endif

/******************************************************************************
 * defines externals used in low-level I/O routines                           *
 ******************************************************************************/
#ifdef	OPN_PTYPES

#if HAVE_FCNTL_H
#  include <fcntl.h>
#else
#  if HAVE_SYS_FCNTL_H
#    include <sys/fcntl.h>
#  endif
#endif

#endif	/* OPN_PTYPES */

/******************************************************************************
 * defines externals used in password-file routines                           *
 ******************************************************************************/
#ifdef	PWD_PTYPES

#if HAVE_PWD_H
#include <pwd.h>
#endif	/* HAVE_PWD_H */

#endif	/* PWD_PTYPES */

/******************************************************************************
 * defines the type of return-value from "signal()"                           *
 ******************************************************************************/
#ifdef	SIG_PTYPES

#include <signal.h>

#undef	SIG_T
#undef	SIGNAL_FUNC
#undef	DCL_SIGNAL

#define	SIG_T	RETSIGTYPE	/* obsolete */

#if SIG_ARGS_VARYING
#  define SIGNAL_ARGS _AR1(int,sig) _CDOTS
#  define SIGNAL_args _ar1(int,sig) _CDOTS
#endif

#ifndef SIGNAL_ARGS
#  define SIGNAL_ARGS _AR1(int,sig)
#  define SIGNAL_args _ar1(int,sig)
#endif

#define	SIGNAL_FUNC(f)		RETSIGTYPE f (SIGNAL_ARGS) _DCL(int,sig)

#define	DCL_SIGNAL(func)	RETSIGTYPE	(*func)(SIGNAL_args)

#if SIG_IGN_REDEFINEABLE
# undef  SIG_DFL
# undef  SIG_IGN
# undef  SIG_ERR
# define SIG_DFL	((RETSIGTYPE (*)(SIGNAL_ARGS))0)
# define SIG_IGN	((RETSIGTYPE (*)(SIGNAL_ARGS))1)
# define SIG_ERR	((RETSIGTYPE (*)(SIGNAL_ARGS))-1)
#endif

#ifndef	NSIG		/* e.g., MSDOS */
#define NSIG 32		/* (a guess?) */
#endif

#endif	/* SIG_PTYPES */

/******************************************************************************
 * Define string-procedures                                                   *
 ******************************************************************************/
#ifdef	STR_PTYPES

#if STDC_HEADERS || HAVE_STRING_H
#  include <string.h>
/* An ANSI string.h and pre-ANSI memory.h might conflict.  */
#  if !STDC_HEADERS && HAVE_MEMORY_H
#    include <memory.h>
#  endif /* not STDC_HEADERS and HAVE_MEMORY_H */
#  define bcopy(s, d, n) memcpy ((d), (s), (n))
#  define bcmp(s1, s2, n) memcmp ((s1), (s2), (n))
#  if !HAVE_BZERO
#    define bzero(s, n) memset ((s), 0, (n))
#  endif
#else /* not STDC_HEADERS and not HAVE_STRING_H */
#  include <strings.h>
#  define strchr index
#  define strrchr rindex
/* memory.h and strings.h conflict on some systems.  */
#endif /* not STDC_HEADERS and not HAVE_STRING_H */

#endif	/* STR_PTYPES */

/******************************************************************************
 * Handles proper include for <time.h>                                        *
 ******************************************************************************/
#ifdef	TIM_PTYPES

#ifdef TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else
#  ifdef HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else
#    include <time.h>
#  endif
#endif

#define	MINUTE	60
#define	HOUR	(60*MINUTE)
#define	DAY	(24*HOUR)
#define	YEAR	(365*DAY)

#if HAVE_TIMEZONE && !TIMEZONE_DECLARED && !LINTLIBRARY
extern	long	timezone;
#endif

#endif

/******************************************************************************
 * Handles proper include for <termios.h>                                     *
 ******************************************************************************/
#ifdef	TRM_PTYPES

#if HAVE_TERMIOS_H && HAVE_TCGETATTR
#  include <termios.h>
#  define USING_TERMIOS_H 1
#  define TermioT struct termios
#  define GetTerminal(p) tcgetattr(0, p)
#  define SetTerminal(p) tcsetattr(0, TCSAFLUSH, p)
#else
#  if HAVE_TERMIO_H
#    include <termio.h>
#    define USING_TERMIO_H 1
#    define TermioT struct termio
#    if HAVE_TCGETATTR
#      define GetTerminal(p) tcgetattr(0, p)
#      define SetTerminal(p) tcsetattr(0, TCSAFLUSH, p)
#    else
#      define GetTerminal(p) ioctl(0, TCGETA, p)
#      define SetTerminal(p) ioctl(0, TCSETAF, p)
#    endif
#  else
#    if HAVE_SGTTY_H
#      include <sgtty.h>		/* for stty/gtty functions */
#      define USING_SGTTY_H 1
#      define TermioT struct sgttyb
#      define GetTerminal(p) gtty(0, p)
#      define SetTerminal(p) stty(0, p)
#    endif
#  endif
#endif

#endif	/* TRM_PTYPES */

/******************************************************************************
 * defines the argument-type for "wait()"                                     *
 ******************************************************************************/
#ifdef	WAI_PTYPES

#ifdef	vms
#include <processes.h>
#endif

#ifdef HAVE_SYS_WAIT_H
#include <sys/wait.h>
#endif

#if WAIT_USES_UNION
# define	WAIT_ARG_TYPE	union wait
#else
# define	WAIT_ARG_TYPE	int
#endif

#define	ARG_WAIT(status)	((int *)&status)
#define	DCL_WAIT(status)	int status

#ifdef WEXITSTATUS
# define	W_RETCODE(status)	WEXITSTATUS(status)
#else
# if WAIT_USES_UNION
#  define	W_RETCODE(status)	(status.w_retcode)
# else
#  define	W_RETCODE(status)	((status >> 8) & 0xff)
# endif
#endif

#endif	/* WAI_PTYPES */

/******************************************************************************
 * Definitions of procedures in TD_LIB common library                         *
 ******************************************************************************/
#ifndef	LINTLIBRARY
#include <td_ext.h>
#include <td_lib.h>

#ifndef	lint
extern	int	main(_arx(int,argc) _ar1(char **,argv));
#endif	/* lint */
#define	_MAIN\
	int	main(_ARX(int,argc) _AR1(char **,argv))\
		     _DCL(int,argc) _DCL(char **,argv)

#endif	/* LINTLIBRARY */

/* This may be defined in one of the system includes, e.g., <sys/param.h> */
#ifndef	MAXPATHLEN
#define	MAXPATHLEN	256
#endif

/*
 * Declare argument for 'exit()' and '_exit()' (usually in <stdlib.h>):
 */
#ifndef	EXIT_FAILURE
# ifdef	vms
#  include	<stsdef.h>
#  define	EXIT_SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#  define	EXIT_FAILURE	(STS$M_INHIB_MSG | STS$K_ERROR)
# else	/* unix */
#  define	EXIT_SUCCESS	(0)		/* if no error */
#  define	EXIT_FAILURE	(1)		/* if any error */
# endif	/* vms/unix */
#endif

#define FAIL    EXIT_FAILURE
#define SUCCESS EXIT_SUCCESS

#endif	/* PTYPES_H */
