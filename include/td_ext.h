/* $Id: td_ext.h,v 12.5 1994/07/10 22:22:07 tom Exp $ */

#ifndef	TD_EXT_H
#define	TD_EXT_H

/*
 * This file provides missing extern/prototype declarations for external
 * functions used in TD_LIB and applications that are built with that library. 
 * The list of functions is ad hoc, to cover the systems on which I've ported
 * that happen to not have a prototype (or extern) declared in any include
 * file.  It is not intended to be a complete list of functions.
 *
 * There are three sources for the definitions that are used:
 *
 *	1) man-pages
 *	2) lint-libraries
 *	3) the Posix standard
 *
 * When there is a difference, the Posix standard is used as the final
 * authority; however some additional definitions are used to facilitate
 * linting the applications.
 */

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

/******************************************************************************
 * Useful external-definitions                                                *
 ******************************************************************************/

#if	TIMEZONE_DECLARED
extern	long	timezone;
#endif

#if	HAVE_SYS_ERRLIST
extern	char	*sys_errlist[];
extern	int	sys_nerr;
#endif

#if	HAVE_LTOSTR
#define	ltostr	td_ltostr	/* rename my version */
#endif

/******************************************************************************
 * <ctype.h> definitions                                                      *
 ******************************************************************************/
#ifdef	CHR_PTYPES

#if HAVE_TOLOWER && NEED_TOLOWER
extern	int	tolower   ARGS((int c));
#endif /* HAVE_TOLOWER */
#if HAVE_TOUPPER && NEED_TOUPPER
extern	int	toupper   ARGS((int c));
#endif /* HAVE_TOUPPER */

#endif	/* CHR_PTYPES */

/******************************************************************************
 * <dirent.h> definitions                                                     *
 ******************************************************************************/
#ifdef	DIR_PTYPES

#if HAVE_OPENDIR && NEED_OPENDIR
extern	DIR	*opendir  ARGS((const char *name));
#endif /* HAVE_OPENDIR */
#if HAVE_CLOSEDIR && NEED_CLOSEDIR
extern	int	closedir  ARGS((DIR * dirp));
#endif /* HAVE_CLOSEDIR */
#if HAVE_READDIR && NEED_READDIR
extern	DirentT *readdir  ARGS((DIR * dirp));
#endif /* HAVE_READDIR */
#if HAVE_REWINDDIR && NEED_REWINDDIR
extern	void	rewinddir ARGS((DIR * dirp));
#endif /* HAVE_REWINDDIR */
#if HAVE_SEEKDIR && NEED_SEEKDIR
extern	void	seekdir   ARGS((DIR * dirp, off_t pos));
#endif /* HAVE_SEEKDIR */
#if HAVE_TELLDIR && NEED_TELLDIR
extern	off_t	telldir   ARGS((DIR * dirp));
#endif /* HAVE_TELLDIR */

#endif	/* DIR_PTYPES */

/******************************************************************************
 * <errno.h> definitions                                                      *
 ******************************************************************************/
#if	HAVE_ERRNO && NEED_ERRNO
extern	int	errno;
#endif

/******************************************************************************
 * <fcntl.h> definitions                                                      *
 ******************************************************************************/
#ifdef	OPN_PTYPES

#if HAVE_CREAT && NEED_CREAT
extern	int	creat	 ARGS((const char *f, mode_t m));
#endif /* HAVE_CREAT */
#if HAVE_OPEN && NEED_OPEN
extern	int	open	 ARGS((const char *f, int m, ...));
#endif /* HAVE_OPEN */

#endif	/* OPN_PTYPES */

/******************************************************************************
 * <grp.h> definitions                                                        *
 ******************************************************************************/
#ifdef	GRP_PTYPES

#if HAVE_GRP_H

#if HAVE_GETGRNAM && NEED_GETGRNAM
extern	struct group * getgrnam ARGS((const char *name));
#endif /* HAVE_GETGRNAM */
#if HAVE_GETGRGID && NEED_GETGRGID
extern	struct group * getgrgid ARGS((gid_t gid));
#endif /* HAVE_GETGRGID */
#if HAVE_GETGRENT && NEED_GETGRENT
extern	struct group * getgrent ARGS((void));
#endif /* HAVE_GETGRENT */
#if HAVE_SETGRENT && NEED_SETGRENT
extern	V_OR_I         setgrent ARGS((void));
#endif /* HAVE_SETGRENT */
#if HAVE_ENDGRENT && NEED_ENDGRENT
extern	V_OR_I         endgrent ARGS((void));
#endif /* HAVE_ENDGRENT */

#endif	/* HAVE_GRP_H */

#endif	/* GRP_PTYPES */

/******************************************************************************
 * <pwd.h> definitions                                                        *
 ******************************************************************************/
#ifdef	PWD_PTYPES

#if HAVE_PWD_H

#if HAVE_GETPWNAM && NEED_GETPWNAM
extern	struct passwd *	getpwnam ARGS((const char * name));
#endif /* HAVE_GETPWNAM */
#if HAVE_GETPWUID && NEED_GETPWUID
extern	struct passwd *	getpwuid ARGS((uid_t uid));
#endif /* HAVE_GETPWUID */
#if HAVE_GETPWENT && NEED_GETPWENT
extern	struct passwd *	getpwent ARGS((void));
#endif /* HAVE_GETPWENT */
#if HAVE_SETPWENT && NEED_SETPWENT
extern	V_OR_I		setpwent ARGS((void));
#endif /* HAVE_SETPWENT */
#if HAVE_ENDPWENT && NEED_ENDPWENT
extern	V_OR_I		endpwent ARGS((void));
#endif /* HAVE_ENDPWENT */

#endif	/* HAVE_PWD_H */

#endif	/* PWD_PTYPES */

/******************************************************************************
 * <stdio.h> definitions                                                      *
 ******************************************************************************/

#if HAVE_FCLOSE && NEED_FCLOSE
extern	int	fclose   ARGS((FILE *s));
#endif /* HAVE_FCLOSE */
#if HAVE_FFLUSH && NEED_FFLUSH
extern	int	fflush   ARGS((FILE *s));
#endif /* HAVE_FFLUSH */
#if HAVE_FGETC && NEED_FGETC
extern	int	fgetc    ARGS((FILE *s));
#endif /* HAVE_FGETC */
#if HAVE_FPRINTF && NEED_FPRINTF
extern	int	fprintf  ARGS((FILE *s, const char *f, ...));
#endif /* HAVE_FPRINTF */
#if HAVE_FPUTC && NEED_FPUTC
extern	int	fputc    ARGS((int c, FILE *s));
#endif /* HAVE_FPUTC */
#if HAVE_FPUTS && NEED_FPUTS
extern	int	fputs    ARGS((const char *b, FILE *s));
#endif /* HAVE_FPUTS */
#if HAVE_FREAD && NEED_FREAD
extern	size_t	fread    ARGS((void *b, size_t sz, size_t n, FILE *s));
#endif /* HAVE_FREAD */
#if HAVE_FSCANF && NEED_FSCANF
extern	int	fscanf   ARGS((FILE *s, const char *f, ...));
#endif /* HAVE_FSCANF */
#if HAVE_FSEEK && NEED_FSEEK
extern	int	fseek    ARGS((FILE *s, long, int));
#endif /* HAVE_FSEEK */
#if HAVE_FWRITE && NEED_FWRITE
extern	size_t	fwrite   ARGS((const void *b, size_t sz, size_t n, FILE *s));
#endif /* HAVE_FWRITE */
#if HAVE_PERROR && NEED_PERROR
extern	V_OR_I2	perror   ARGS((const char *s));
#endif /* HAVE_PERROR */
#if HAVE_PRINTF && NEED_PRINTF
extern	int	printf   ARGS((const char *f, ...));
#endif /* HAVE_PRINTF */
#if HAVE_PUTS && NEED_PUTS
extern	int	puts     ARGS((const char *s));
#endif /* HAVE_PUTS */
#if HAVE_RENAME && NEED_RENAME
extern	int	rename   ARGS((const char *s, const char *d));
#endif /* HAVE_RENAME */
#if HAVE_REWIND && NEED_REWIND
extern	V_OR_I	rewind   ARGS((FILE *s));
#endif /* HAVE_REWIND */
#if HAVE_SETBUF && NEED_SETBUF
extern	void	setbuf   ARGS((FILE *s, char *b));
#endif /* HAVE_SETBUF */
#if HAVE_SPRINTF && NEED_SPRINTF
extern	int	sprintf  ARGS((char *b, const char *f, ...));
#endif /* HAVE_SPRINTF */
#if HAVE_SSCANF && NEED_SSCANF
extern	int	sscanf	 ARGS((const char *b, const char *s, ...));
#endif /* HAVE_SSCANF */
#if HAVE_UNGETC && NEED_UNGETC
extern	int	ungetc	 ARGS((int, FILE *));
#endif /* HAVE_UNGETC */

/******************************************************************************
 * <stdlib.h> definitions                                                     *
 ******************************************************************************/

#if HAVE_CALLOC && NEED_CALLOC
extern	V_OR_P	calloc  ARGS((size_t nel, size_t size));
#endif /* HAVE_CALLOC */
#if HAVE_EXIT && NEED_EXIT
extern	V_OR_I2	exit    ARGS((int code));
#endif /* HAVE_EXIT */
#if HAVE_FREE && NEED_FREE
extern	V_OR_I	free    ARGS((void *s));
#endif /* HAVE_FREE */
#if HAVE_GETENV && NEED_GETENV
extern	char *	getenv  ARGS((const char *s));
#endif /* HAVE_GETENV */
#if HAVE_MALLOC && NEED_MALLOC
extern	V_OR_P	malloc  ARGS((size_t size));
#endif /* HAVE_MALLOC */
#if HAVE_REALLOC && NEED_REALLOC
extern	V_OR_P	realloc ARGS((V_OR_P ptr, size_t size));
#endif /* HAVE_REALLOC */
#if HAVE_STRTOL && NEED_STRTOL
extern	long	strtol  ARGS((const char *s, char **d, int base));
#endif /* HAVE_STRTOL */
#if HAVE_SYSTEM && NEED_SYSTEM
extern	int	system  ARGS((const char *s));
#endif /* HAVE_SYSTEM */

/******************************************************************************
 * <string.h> definitions                                                     *
 ******************************************************************************/
#ifdef	STR_PTYPES

#if HAVE_STRCHR && NEED_STRCHR
extern	char *	strchr	ARGS((const char * s, int c));
#endif /* HAVE_STRCHR */
#if HAVE_STRERROR && NEED_STRERROR
extern	char *	strerror ARGS((int errnum));
#endif /* HAVE_STRERROR */
#if HAVE_STRRCHR && NEED_STRRCHR
extern	char *	strrchr	ARGS((const char * s, int c));
#endif /* HAVE_STRRCHR */
#if HAVE_STRTOK && NEED_STRTOK
extern	char *	strtok	ARGS((char * s, const char * t));
#endif /* HAVE_STRTOK */

#endif	/* STR_PTYPES */

/******************************************************************************
 * <sys/wait.h> definitions                                                   *
 ******************************************************************************/
#ifdef	WAI_PTYPES

#if HAVE_WAIT && NEED_WAIT
extern	pid_t	wait    ARGS((WAIT_ARG_TYPE *p));
#endif /* HAVE_WAIT */

#endif	/* WAI_PTYPES */

/******************************************************************************
 * <time.h> definitions                                                       *
 ******************************************************************************/
#ifdef	TIM_PTYPES

#if HAVE_TIME && NEED_TIME
extern	time_t	time    ARGS((time_t *t));
#endif /* HAVE_TIME */

#endif	/* TIM_PTYPES */

/******************************************************************************
 * <unistd.h> definitions                                                     *
 ******************************************************************************/

#if HAVE__EXIT && NEED__EXIT
extern	V_OR_I	_exit   ARGS((int code));
#endif /* HAVE__EXIT */
#if HAVE_FORK && NEED_FORK
extern	int	fork	ARGS((void));
#endif /* HAVE_FORK */
#if HAVE_GETCWD && NEED_GETCWD
extern	char	*getcwd ARGS((char *p, size_t n));
#endif /* HAVE_GETCWD */
#if HAVE_GETEGID && NEED_GETEGID
extern	gid_t	getegid ARGS((void));
#endif /* HAVE_GETEGID */
#if HAVE_GETEUID && NEED_GETEUID
extern	uid_t	geteuid ARGS((void));
#endif /* HAVE_GETEUID */
#if HAVE_GETGID && NEED_GETGID
extern	gid_t	getgid  ARGS((void));
#endif /* HAVE_GETGID */
#if HAVE_GETUID && NEED_GETUID
extern	uid_t	getuid  ARGS((void));
#endif /* HAVE_GETUID */

/******************************************************************************
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *+ NON-POSIX definitions                                                    +*
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 ******************************************************************************/

#if HAVE__FILBUF && NEED__FILBUF
extern	int	_filbuf	 ARGS((FILE *s));
#endif /* HAVE__FILBUF */
#if HAVE__FLSBUF && NEED__FLSBUF
extern	int	_flsbuf	 ARGS((int n, FILE *s));
#endif /* HAVE__FLSBUF */

#if HAVE_BZERO && NEED_BZERO
extern	void	bzero	 ARGS((V_OR_P p, int n));
#endif /* HAVE_BZERO */

#if HAVE_GETOPT && NEED_GETOPT
extern	int	getopt   ARGS((int argc, char * const *argv, const char *opts));
#endif /* HAVE_GETOPT */
extern	char *	optarg;
extern	int	optind;

#if HAVE_GETWD && NEED_GETWD
extern	char *	getwd    ARGS((char *p));
#endif /* HAVE_GETWD */
#if HAVE_IOCTL && NEED_IOCTL
extern	int	ioctl	 ARGS((int f, int m, ...));
#endif /* HAVE_IOCTL */
#if HAVE_LSTAT && NEED_LSTAT
extern	int	lstat	 ARGS((const char *f, Stat_t *s));
#endif /* HAVE_LSTAT */
#if HAVE_MEMMOVE && NEED_MEMMOVE
extern	void *	memmove  ARGS((void *dest, const void *src, size_t n));
#endif /* HAVE_MEMMOVE */
#if HAVE_MKSTEMP && NEED_MKSTEMP
extern	int	mkstemp  ARGS((char *s));
#endif /* HAVE_MKSTEMP */
#if HAVE_MKTEMP && NEED_MKTEMP
extern	char *	mktemp   ARGS((char *s));
#endif /* HAVE_MKTEMP */
#if HAVE_PCLOSE && NEED_PCLOSE
extern	int	pclose	 ARGS((FILE *s));
#endif /* HAVE_PCLOSE */
#if HAVE_POPEN && NEED_POPEN
extern	FILE	*popen   ARGS((const char *name, const char *mode));
#endif /* HAVE_POPEN */
#if HAVE_PUTENV && NEED_PUTENV
extern	int	putenv	 ARGS((const char *s));
#endif /* HAVE_PUTENV */
#if HAVE_READLINK && NEED_READLINK
extern	int	readlink ARGS((const char *f, char *b, size_t n));
#endif /* HAVE_READLINK */
#if HAVE_SETEGID && NEED_SETEGID
extern	int	setegid	 ARGS((gid_t g));
#endif /* HAVE_SETEGID */
#if HAVE_SETLINEBUF && NEED_SETLINEBUF
extern	void	setlinebuf ARGS((FILE *s));
#endif /* HAVE_SETLINEBUF */
#if HAVE_SETRGID && NEED_SETRGID
extern	int	setrgid	 ARGS((uid_t u));
#endif /* HAVE_SETRGID */
#if HAVE_SETRUID && NEED_SETRUID
extern	int	setruid	 ARGS((uid_t u));
#endif /* HAVE_SETRUID */
#if HAVE_SYMLINK && NEED_SYMLINK
extern	int	symlink	 ARGS((const char *s, const char *d));
#endif /* HAVE_SYMLINK */
#if HAVE_TGETENT && NEED_TGETENT
extern	int	tgetent	 ARGS((char *b, char *s));
#endif /* HAVE_TGETENT */
#if HAVE_TGETNUM && NEED_TGETNUM
extern	int	tgetnum	 ARGS((char *s));
#endif /* HAVE_TGETNUM */
#if HAVE_VFORK && NEED_VFORK
extern	int	vfork	 ARGS((void));
#endif /* HAVE_VFORK */

#if !HAVE_GETWD
#define	getwd(p)	getcwd(p,MAXPATHLEN)
#endif

#endif	/* TD_EXT_H */
