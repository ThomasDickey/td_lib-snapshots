/* $Id: td_ext.h,v 12.1 1994/06/25 14:09:28 tom Exp $ */

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

#if	HAVE_LTOSTR
#define	ltostr	td_ltostr	/* rename my version */
#endif

/******************************************************************************
 * <ctype.h> definitions                                                      *
 ******************************************************************************/
#ifdef	CHR_PTYPES

extern	int	tolower   ARGS((int c));
extern	int	toupper   ARGS((int c));

#endif	/* CHR_PTYPES */

/******************************************************************************
 * <dirent.h> definitions                                                     *
 ******************************************************************************/
#ifdef	DIR_PTYPES

extern	DIR	*opendir  ARGS((const char *name));
extern	int	closedir  ARGS((DIR * dirp));
extern	DirentT *readdir  ARGS((DIR * dirp));
extern	void	rewinddir ARGS((DIR * dirp));
extern	void	seekdir   ARGS((DIR * dirp, off_t pos));
extern	off_t	telldir   ARGS((DIR * dirp));

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

extern	int	creat	 ARGS((const char *f, mode_t m));
extern	int	open	 ARGS((const char *f, int m, ...));

#endif	/* OPN_PTYPES */

/******************************************************************************
 * <grp.h> definitions                                                        *
 ******************************************************************************/
#ifdef	GRP_PTYPES

#if HAVE_GRP_H

extern	struct group * getgrnam ARGS((const char *name));
extern	struct group * getgrgid ARGS((gid_t gid));
extern	struct group * getgrent ARGS((void));
extern	V_OR_I         setgrent ARGS((void));
extern	V_OR_I         endgrent ARGS((void));

#endif	/* HAVE_GRP_H */

#endif	/* GRP_PTYPES */

/******************************************************************************
 * <pwd.h> definitions                                                        *
 ******************************************************************************/
#ifdef	PWD_PTYPES

#if HAVE_PWD_H

extern	struct passwd *	getpwnam ARGS((const char * name));
extern	struct passwd *	getpwuid ARGS((uid_t uid));
extern	struct passwd *	getpwent ARGS((void));
extern	V_OR_I		setpwent ARGS((void));
extern	V_OR_I		endpwent ARGS((void));

#endif	/* HAVE_PWD_H */

#endif	/* PWD_PTYPES */

/******************************************************************************
 * <stdio.h> definitions                                                      *
 ******************************************************************************/

extern	int	fclose   ARGS((FILE *s));
extern	int	fflush   ARGS((FILE *s));
extern	int	fgetc    ARGS((FILE *s));
extern	int	fprintf  ARGS((FILE *s, const char *f, ...));
extern	int	fputc    ARGS((int c, FILE *s));
extern	int	fputs    ARGS((const char *b, FILE *s));
extern	size_t	fread    ARGS((void *b, size_t sz, size_t n, FILE *s));
extern	int	fscanf   ARGS((FILE *s, const char *f, ...));
extern	int	fseek    ARGS((FILE *s, long, int));
extern	size_t	fwrite   ARGS((const void *b, size_t sz, size_t n, FILE *s));
extern	V_OR_I2	perror   ARGS((const char *s));
extern	int	printf   ARGS((const char *f, ...));
extern	int	puts     ARGS((const char *s));
extern	int	rename   ARGS((const char *s, const char *d));
extern	V_OR_I	rewind   ARGS((FILE *s));
extern	void	setbuf   ARGS((FILE *s, char *b));
extern	int	sprintf  ARGS((char *b, const char *f, ...));
extern	int	sscanf	 ARGS((const char *b, const char *s, ...));
extern	int	ungetc	 ARGS((int, FILE *));

/******************************************************************************
 * <stdlib.h> definitions                                                     *
 ******************************************************************************/

extern	V_OR_P	calloc  ARGS((size_t nel, size_t size));
extern	V_OR_I2	exit    ARGS((int code));
extern	V_OR_I	free    ARGS((void *s));
extern	char *	getenv  ARGS((const char *s));
extern	V_OR_P	malloc  ARGS((size_t size));
extern	V_OR_P	realloc ARGS((V_OR_P ptr, size_t size));
extern	long	strtol  ARGS((const char *s, char **d, int base));
extern	int	system  ARGS((const char *s));

/******************************************************************************
 * <string.h> definitions                                                     *
 ******************************************************************************/
#ifdef	STR_PTYPES

extern	char *	strchr	ARGS((const char * s, int c));
extern	char *	strrchr	ARGS((const char * s, int c));
extern	char *	strtok	ARGS((char * s, const char * t));

#endif	/* STR_PTYPES */

/******************************************************************************
 * <sys/wait.h> definitions                                                   *
 ******************************************************************************/
#ifdef	WAI_PTYPES

extern	pid_t	wait    ARGS((WAIT_ARG_TYPE *p));

#endif	/* WAI_PTYPES */

/******************************************************************************
 * <time.h> definitions                                                       *
 ******************************************************************************/
#ifdef	TIM_PTYPES

extern	time_t	time    ARGS((time_t *t));

#endif	/* TIM_PTYPES */

/******************************************************************************
 * <unistd.h> definitions                                                     *
 ******************************************************************************/

extern	V_OR_I	_exit   ARGS((int code));
extern	int	fork	ARGS((void));
extern	char	*getcwd ARGS((char *p, size_t n));
extern	gid_t	getegid ARGS((void));
extern	uid_t	geteuid ARGS((void));
extern	gid_t	getgid  ARGS((void));
extern	uid_t	getuid  ARGS((void));

/******************************************************************************
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *+ NON-POSIX definitions                                                    +*
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 ******************************************************************************/

extern	int	_filbuf	 ARGS((FILE *s));
extern	int	_flsbuf	 ARGS((int n, FILE *s));

extern	void	bzero	 ARGS((V_OR_P p, int n));

extern	int	getopt   ARGS((int argc, char * const *argv, const char *opts));
extern	char *	optarg;
extern	int	optind;

extern	char *	getwd    ARGS((char *p));
extern	int	ioctl	 ARGS((int f, int m, caddr_t p));
extern	int	lstat	 ARGS((const char *f, Stat_t *s));
extern	int	mkstemp  ARGS((char *s));
extern	char *	mktemp   ARGS((char *s));
extern	int	pclose	 ARGS((FILE *s));
extern	FILE	*popen   ARGS((const char *name, const char *mode));
extern	int	putenv	 ARGS((const char *s));
extern	int	readlink ARGS((const char *f, char *b, size_t n));
extern	int	setegid	 ARGS((gid_t g));
extern	void	setlinebuf ARGS((FILE *s));
extern	int	setrgid	 ARGS((uid_t u));
extern	int	setruid	 ARGS((uid_t u));
extern	int	symlink	 ARGS((const char *s, const char *d));
extern	int	tgetent	 ARGS((char *b, char *s));
extern	int	tgetnum	 ARGS((char *s));
extern	int	vfork	 ARGS((void));

#endif	/* TD_EXT_H */
