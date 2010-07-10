/* $Id: ptypes.h,v 12.55 2010/07/09 23:11:07 tom Exp $ */

#ifndef	PTYPES_H
#define	PTYPES_H

#if defined(HAVE_CONFIG_H)
#include <td_config.h>
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
 *	GRP_PTYPES	<grp.h>
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

#if defined(HAVE_SYS_PARAM_H)
#include <sys/param.h>
#endif

#ifdef	NO_IDENT
#define	MODULE_ID(s)	/*nothing*/
#else
#define	MODULE_ID(s)	static const char Id[] = s;
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

#if defined(MSDOS) || defined(__EMX__)
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

#if	!defined(MSDOS) && !defined(vms)
#define	SYS_UNIX		/* it's close enough for me */
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

#if defined(HAVE_STDLIB_H)
#include	<stdlib.h>
#endif

#if defined(HAVE_UNISTD_H)
#include	<unistd.h>
#endif

#if !defined(HAVE_LSTAT)
#define	lstat	stat
#endif

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
#else	/* SYS_UNIX */
#define	LEN_QSORT	int
#define	LEN_READ	int
#if	defined(__STDC__) || defined(apollo)
#define	LEN_FREAD	size_t
#else
#define	LEN_FREAD	int
#endif	/* apollo */
#endif	/* vms/SYS_UNIX */
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

#define UCH(c)	((unsigned char)(c))

/*
 * Functions we (usually) ignore the return value from:
 */
#define	PRINTW	(void)printw
#define	PRINTF	(void)printf
#define	FPRINTF	(void)fprintf
#define	FORMAT	(void)sprintf
#define	FFLUSH	(void)fflush
#define	FCLOSE	(void)fclose

#define	DOALLOC(p,t,n)	(t *)doalloc((char *)p,sizeof(t)*(n))
#define	ALLOC(t,n)	DOALLOC(0,t,n)

/******************************************************************************
 * Define symbols used in 'access()' function                                 *
 ******************************************************************************/
#ifdef	ACC_PTYPES

#ifdef	SYS_UNIX
#  if defined(HAVE_SYS_FILE_H)
#    include	<sys/file.h>
#  endif
#endif	/* vms/SYS_UNIX */

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

#if defined(STDC_HEADERS) || defined(HAVE_TOLOWER)
# define LowerMacro(c) tolower(c)
# define UpperMacro(c) toupper(c)
#else
# if defined(_tolower)
#  define LowerMacro(c) _tolower(c)
#  define UpperMacro(c) _toupper(c)
# endif
#endif

#if defined(STDC_HEADERS)
#define LowerCase(c) c = LowerMacro(c)
#define UpperCase(c) c = UpperMacro(c)
#else
#define LowerCase(c) if (isascii(c) && isupper(c)) c = LowerMacro(c)
#define UpperCase(c) if (isascii(c) && islower(c)) c = UpperMacro(c)
#endif

#if !defined(toascii) && !defined(HAVE_TOASCII)
#define	toascii(c) (c & 0x7f)		/* e.g., 7-bit ASCII */
#endif

#ifndef isascii
#define	isascii(c) (c == toascii(c))	/* e.g., 7-bit ASCII */
#endif

#endif	/* CHR_PTYPES */

/******************************************************************************
 * Define macros for directory-scanning                                       *
 ******************************************************************************/
#ifdef	DIR_PTYPES

#if defined(HAVE_DIRENT_H)
#  include	<dirent.h>
#  define NAMLEN(dirent) strlen((dirent)->d_name)
#else
#  define dirent direct
#  define NAMLEN(dirent) ((dirent)->d_namlen)
#  if defined(HAVE_SYS_NDIR_H)
#    include	<sys/ndir.h>
#    if defined(HAVE_SYS_DIR_H)
#      include	<sys/dir.h>
#    else
#      if defined(HAVE_NDIR_H)
#        include	<ndir.h>
#      else
#        if defined(vms) || defined(VMS)
#          include <unixdir.h>	/* get this from PORTUNIX */
#        else
#          if defined(MSDOS)
#            include <dir.h>
#          else			/* ...must be an antique unix clone */
#            define	DIR	FILE
#            define	opendir(n)	fopen(n,"r")
#            define	readdir(fp)	(fread(&dbfr, sizeof(dbfr), 1, fp)\
						? &dbfr\
						: (struct direct *)0)
#            define	closedir(fp)	FCLOSE(fp)
static	struct	direct	dbfr;
#          endif		/* MSDOS/SYS_UNIX */
#        endif			/* vms/MSDOS/SYS_UNIX */
#      endif			/* NDIR/... */
#    endif			/* SYS_DIR/... */
#  endif			/* SYS_NDIR/... */
#endif				/* DIRENT/... */

#define	DirentT struct	dirent

#endif	/* DIR_PTYPES */

/******************************************************************************
 * defines externals used in error-reporting/handling                         *
 ******************************************************************************/
#ifdef	ERR_PTYPES

#include <errno.h>

# if defined(DECL_SYS_ERRLIST)
extern char *sys_errlist[];
extern int sys_nerr;
# endif

#endif

/******************************************************************************
 * defines externals used in group-file routines                              *
 ******************************************************************************/
#ifdef	GRP_PTYPES

#if defined(HAVE_GRP_H)
#include <grp.h>
#endif	/* HAVE_GRP_H */

#endif	/* GRP_PTYPES */

/******************************************************************************
 * defines externals used in low-level I/O routines                           *
 ******************************************************************************/
#ifdef	OPN_PTYPES

#if defined(HAVE_FCNTL_H)
#  include <fcntl.h>
#else
#  if defined(HAVE_SYS_FCNTL_H)
#    include <sys/fcntl.h>
#  endif
#endif

#ifndef O_BINARY
#define O_BINARY 0	/* nonstandard, used in OS/2 EMX and Cygwin */
#endif

#endif	/* OPN_PTYPES */

/******************************************************************************
 * defines externals used in password-file routines                           *
 ******************************************************************************/
#ifdef	PWD_PTYPES

#if defined(HAVE_PWD_H)
#include <pwd.h>
#endif	/* HAVE_PWD_H */

#endif	/* PWD_PTYPES */

/******************************************************************************
 * defines the type of return-value from "signal()"                           *
 ******************************************************************************/
#ifdef	SIG_PTYPES

#undef CS
#define CS broken_GLIBC_CS	/* glibc 2.1.3 (mis)defines a lot of stuff */

#include <signal.h>

#undef CS

#undef	SIG_T
#undef	SIGNAL_FUNC
#undef	DCL_SIGNAL

#define	SIG_T	RETSIGTYPE	/* obsolete */

#if defined(SIG_ARGS_VARYING)
#  define SIGNAL_ARGS int sig, ...
#  define SIGNAL_args int sig, ...
#endif

#ifndef SIGNAL_ARGS
#  define SIGNAL_ARGS int sig
#  define SIGNAL_args int sig
#endif

#define	SIGNAL_FUNC(f)		RETSIGTYPE f (SIGNAL_ARGS)

#define	DCL_SIGNAL(func)	RETSIGTYPE	(*func)(SIGNAL_args)

#if defined(SIG_IGN_REDEFINEABLE)
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

#if defined(STDC_HEADERS) || defined(HAVE_STRING_H)
#  include <string.h>
/* An ANSI string.h and pre-ANSI memory.h might conflict.  */
#  if !defined(STDC_HEADERS) && defined(HAVE_MEMORY_H)
#    include <memory.h>
#  endif /* not STDC_HEADERS and HAVE_MEMORY_H */
#  ifndef bcopy
#    define bcopy(s, d, n) memcpy ((d), (s), (n))
#  endif
#  ifndef bcmp
#    define bcmp(s1, s2, n) memcmp ((s1), (s2), (n))
#  endif
#  if !defined(HAVE_BZERO) && !defined(bzero)
#    define bzero(s, n) memset ((s), 0, (n))
#  endif
#else /* not STDC_HEADERS and not HAVE_STRING_H */
#  include <strings.h>
#  ifndef strchr
#    define strchr index
#  endif
#  ifndef strrchr
#    define strrchr rindex
#  endif
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

#if defined(HAVE_TIMEZONE) && !defined(TIMEZONE_DECLARED) && !defined(LINTLIBRARY)
extern	long	timezone;
#endif

#if defined(HAVE_UTIME_H)
#include <utime.h>
#endif

#endif	/* TIM_PTYPES */

/******************************************************************************
 * Handles proper include for <termios.h>                                     *
 ******************************************************************************/
#ifdef	TRM_PTYPES

#if defined(HAVE_TERMIOS_H) && defined(HAVE_TCGETATTR)
#  include <termios.h>
#  define USING_TERMIOS_H 1
#  define TermioT struct termios
#  define GetTerminal(p) tcgetattr(0, p)
#  define SetTerminal(p) tcsetattr(0, TCSAFLUSH, p)
#else
#  if defined(HAVE_TERMIO_H)
#    include <termio.h>
#    define USING_TERMIO_H 1
#    define TermioT struct termio
#    if defined(HAVE_TCGETATTR)
#      define GetTerminal(p) tcgetattr(0, p)
#      define SetTerminal(p) tcsetattr(0, TCSAFLUSH, p)
#    else
#      define GetTerminal(p) ioctl(0, TCGETA, p)
#      define SetTerminal(p) ioctl(0, TCSETAF, p)
#    endif
#  else
#    if defined(HAVE_SGTTY_H)
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

#if defined(WAIT_USES_UNION)
# define	WAIT_ARG_TYPE	union wait
#else
# define	WAIT_ARG_TYPE	int
#endif

#define	ARG_WAIT(status)	((int *)&status)
#define	DCL_WAIT(status)	int status

#ifdef WEXITSTATUS
# define	W_RETCODE(status)	WEXITSTATUS(status)
#else
# if defined(WAIT_USES_UNION)
#  define	W_RETCODE(status)	(status.w_retcode)
# else
#  define	W_RETCODE(status)	((status >> 8) & 0xff)
# endif
#endif

#endif	/* WAI_PTYPES */

#ifndef GCC_NORETURN
#define GCC_NORETURN /* nothing */
#endif

#ifndef GCC_PRINTF
#define GCC_PRINTF /* nothing */
#endif

#ifndef GCC_PRINTFLIKE
#define GCC_PRINTFLIKE(fmt,var) /* nothing */
#endif

#ifndef GCC_SCANF
#define GCC_SCANF /* nothing */
#endif

#ifndef GCC_SCANFLIKE
#define GCC_SCANFLIKE(fmt,var) /* nothing */
#endif

#ifndef GCC_UNUSED
#define GCC_UNUSED /* nothing */
#endif

/******************************************************************************
 * Definitions of procedures in TD_LIB common library                         *
 ******************************************************************************/
#ifndef	LINTLIBRARY
#include <td_ext.h>
#include <td_lib.h>

#if !(defined(lint) || defined(TESTING_CONFIG_H))
extern	int	main(int argc, char ** argv);
#endif	/* lint */
#define	_MAIN\
	int	main(int argc, char ** argv)

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
# else	/* SYS_UNIX */
#  define	EXIT_SUCCESS	(0)		/* if no error */
#  define	EXIT_FAILURE	(1)		/* if any error */
# endif	/* vms/SYS_UNIX */
#endif

#define FAIL    EXIT_FAILURE
#define SUCCESS EXIT_SUCCESS

#endif	/* PTYPES_H */
