/* $Id: td_ext.h,v 12.23 2025/01/07 00:03:50 tom Exp $ */

#ifndef	TD_EXT_H
#define	TD_EXT_H

#ifdef __cplusplus
extern "C" {
#endif

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
#else				/* SYS_UNIX */
#define	LEN_QSORT	int
#define	LEN_READ	int
#if	defined(__STDC__) || defined(apollo)
#define	LEN_FREAD	size_t
#else
#define	LEN_FREAD	int
#endif				/* apollo */
#endif				/* vms/SYS_UNIX */
#endif				/* SYSTEM5 */

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
 * <ctype.h> definitions                                                      *
 ******************************************************************************/
#ifdef	CHR_PTYPES

#if defined(HAVE_TOLOWER) && defined(NEED_TOLOWER)
    extern int tolower(int c);
#endif				/* HAVE_TOLOWER */
#if defined(HAVE_TOUPPER) && defined(NEED_TOUPPER)
    extern int toupper(int c);
#endif				/* HAVE_TOUPPER */

#endif				/* CHR_PTYPES */

/******************************************************************************
 * <dirent.h> definitions                                                     *
 ******************************************************************************/
#ifdef	DIR_PTYPES

#if defined(HAVE_OPENDIR) && defined(NEED_OPENDIR)
    extern DIR *opendir(const char *name);
#endif				/* HAVE_OPENDIR */
#if defined(HAVE_CLOSEDIR) && defined(NEED_CLOSEDIR)
    extern int closedir(DIR * dirp);
#endif				/* HAVE_CLOSEDIR */
#if defined(HAVE_READDIR) && defined(NEED_READDIR)
    extern DirentT *readdir(DIR * dirp);
#endif				/* HAVE_READDIR */
#if defined(HAVE_REWINDDIR) && defined(NEED_REWINDDIR)
    extern void rewinddir(DIR * dirp);
#endif				/* HAVE_REWINDDIR */
#if defined(HAVE_SEEKDIR) && defined(NEED_SEEKDIR)
    extern void seekdir(DIR * dirp, off_t pos);
#endif				/* HAVE_SEEKDIR */
#if defined(HAVE_TELLDIR) && defined(NEED_TELLDIR)
    extern off_t telldir(DIR * dirp);
#endif				/* HAVE_TELLDIR */

#endif				/* DIR_PTYPES */

/******************************************************************************
 * <errno.h> definitions                                                      *
 ******************************************************************************/
#if defined(HAVE_ERRNO) && defined(NEED_ERRNO)
    extern int errno;
#endif

/******************************************************************************
 * <fcntl.h> definitions                                                      *
 ******************************************************************************/
#ifdef	OPN_PTYPES

#if defined(HAVE_CREAT) && defined(NEED_CREAT)
    extern int creat(const char *f, mode_t m);
#endif				/* HAVE_CREAT */
#if defined(HAVE_OPEN) && defined(NEED_OPEN)
    extern int open(const char *f, int m, ...);
#endif				/* HAVE_OPEN */

#endif				/* OPN_PTYPES */

/******************************************************************************
 * <grp.h> definitions                                                        *
 ******************************************************************************/
#ifdef	GRP_PTYPES

#if defined(HAVE_GRP_H)

#if defined(HAVE_GETGRNAM) && defined(NEED_GETGRNAM)
    extern struct group *getgrnam(const char *name);
#endif				/* HAVE_GETGRNAM */
#if defined(HAVE_GETGRGID) && defined(NEED_GETGRGID)
    extern struct group *getgrgid(gid_t gid);
#endif				/* HAVE_GETGRGID */
#if defined(HAVE_GETGRENT) && defined(NEED_GETGRENT)
    extern struct group *getgrent(void);
#endif				/* HAVE_GETGRENT */
#if defined(HAVE_SETGRENT) && defined(NEED_SETGRENT)
    extern V_OR_I setgrent(void);
#endif				/* HAVE_SETGRENT */
#if defined(HAVE_ENDGRENT) && defined(NEED_ENDGRENT)
    extern V_OR_I endgrent(void);
#endif				/* HAVE_ENDGRENT */

#endif				/* HAVE_GRP_H */

#if defined(HAVE_GETGROUPS) && defined(NEED_GETGROUPS)
    extern int getgroups(int size, GETGROUPS_T * list);
#endif				/* HAVE_GETGROUPS */

#endif				/* GRP_PTYPES */

/******************************************************************************
 * <pwd.h> definitions                                                        *
 ******************************************************************************/
#ifdef	PWD_PTYPES

#if defined(HAVE_PWD_H)

#if defined(HAVE_GETPWNAM) && defined(NEED_GETPWNAM)
    extern struct passwd *getpwnam(const char *name);
#endif				/* HAVE_GETPWNAM */
#if defined(HAVE_GETPWUID) && defined(NEED_GETPWUID)
    extern struct passwd *getpwuid(uid_t uid);
#endif				/* HAVE_GETPWUID */
#if defined(HAVE_GETPWENT) && defined(NEED_GETPWENT)
    extern struct passwd *getpwent(void);
#endif				/* HAVE_GETPWENT */
#if defined(HAVE_SETPWENT) && defined(NEED_SETPWENT)
    extern V_OR_I setpwent(void);
#endif				/* HAVE_SETPWENT */
#if defined(HAVE_ENDPWENT) && defined(NEED_ENDPWENT)
    extern V_OR_I endpwent(void);
#endif				/* HAVE_ENDPWENT */

#endif				/* HAVE_PWD_H */

#endif				/* PWD_PTYPES */

/******************************************************************************
 * <stdio.h> definitions                                                      *
 ******************************************************************************/

#if defined(HAVE_FCLOSE) && defined(NEED_FCLOSE)
    extern int fclose(FILE *s);
#endif				/* HAVE_FCLOSE */
#if defined(HAVE_FFLUSH) && defined(NEED_FFLUSH)
    extern int fflush(FILE *s);
#endif				/* HAVE_FFLUSH */
#if defined(HAVE_FGETC) && defined(NEED_FGETC)
    extern int fgetc(FILE *s);
#endif				/* HAVE_FGETC */
#if defined(HAVE_FPRINTF) && defined(NEED_FPRINTF)
    extern int fprintf(FILE *s, const char *f, ...);
#endif				/* HAVE_FPRINTF */
#if defined(HAVE_FPUTC) && defined(NEED_FPUTC)
    extern int fputc(int c, FILE *s);
#endif				/* HAVE_FPUTC */
#if defined(HAVE_FPUTS) && defined(NEED_FPUTS)
    extern int fputs(const char *b, FILE *s);
#endif				/* HAVE_FPUTS */
#if defined(HAVE_FREAD) && defined(NEED_FREAD)
    extern size_t fread(void *b, size_t sz, size_t n, FILE *s);
#endif				/* HAVE_FREAD */
#if defined(HAVE_FSCANF) && defined(NEED_FSCANF)
    extern int fscanf(FILE *s, const char *f, ...);
#endif				/* HAVE_FSCANF */
#if defined(HAVE_FSEEK) && defined(NEED_FSEEK)
    extern int fseek(FILE *s, long, int);
#endif				/* HAVE_FSEEK */
#if defined(HAVE_FWRITE) && defined(NEED_FWRITE)
    extern size_t fwrite(const void *b, size_t sz, size_t n, FILE *s);
#endif				/* HAVE_FWRITE */
#if defined(HAVE_PERROR) && defined(NEED_PERROR)
    extern V_OR_I2 perror(const char *s);
#endif				/* HAVE_PERROR */
#if defined(HAVE_PRINTF) && defined(NEED_PRINTF)
    extern int printf(const char *f, ...);
#endif				/* HAVE_PRINTF */
#if defined(HAVE_PUTS) && defined(NEED_PUTS)
    extern int puts(const char *s);
#endif				/* HAVE_PUTS */
#if defined(HAVE_RENAME) && defined(NEED_RENAME)
    extern int rename(const char *s, const char *d);
#endif				/* HAVE_RENAME */
#if defined(HAVE_REWIND) && defined(NEED_REWIND)
    extern V_OR_I rewind(FILE *s);
#endif				/* HAVE_REWIND */
#if defined(HAVE_SETBUF) && defined(NEED_SETBUF)
    extern void setbuf(FILE *s, char *b);
#endif				/* HAVE_SETBUF */
#if defined(HAVE_SPRINTF) && defined(NEED_SPRINTF)
    extern int sprintf(char *b, const char *f, ...);
#endif				/* HAVE_SPRINTF */
#if defined(HAVE_SSCANF) && defined(NEED_SSCANF)
    extern int sscanf(const char *b, const char *s, ...);
#endif				/* HAVE_SSCANF */
#if defined(HAVE_UNGETC) && defined(NEED_UNGETC)
    extern int ungetc(int, FILE *);
#endif				/* HAVE_UNGETC */

/******************************************************************************
 * <stdlib.h> definitions                                                     *
 ******************************************************************************/

#if defined(HAVE_CALLOC) && defined(NEED_CALLOC)
    extern V_OR_P calloc(size_t nel, size_t size);
#endif				/* HAVE_CALLOC */
#if defined(HAVE_EXIT) && defined(NEED_EXIT)
    extern V_OR_I2 exit(int code);
#endif				/* HAVE_EXIT */
#if defined(HAVE_FREE) && defined(NEED_FREE)
    extern V_OR_I free(void *s);
#endif				/* HAVE_FREE */
#if defined(HAVE_GETENV) && defined(NEED_GETENV)
    extern char *getenv(const char *s);
#endif				/* HAVE_GETENV */
#if defined(HAVE_MALLOC) && defined(NEED_MALLOC)
    extern V_OR_P malloc(size_t size);
#endif				/* HAVE_MALLOC */
#if defined(HAVE_REALLOC) && defined(NEED_REALLOC)
    extern V_OR_P realloc(V_OR_P ptr, size_t size);
#endif				/* HAVE_REALLOC */
#if defined(HAVE_STRTOL) && defined(NEED_STRTOL)
    extern long strtol(const char *s, char **d, int base);
#endif				/* HAVE_STRTOL */
#if defined(HAVE_SYSTEM) && defined(NEED_SYSTEM)
    extern int system(const char *s);
#endif				/* HAVE_SYSTEM */

/******************************************************************************
 * <string.h> definitions                                                     *
 ******************************************************************************/
#ifdef	STR_PTYPES

#if defined(HAVE_STRCHR) && defined(NEED_STRCHR)
    extern char *strchr(const char *s, int c);
#endif				/* HAVE_STRCHR */
#if defined(HAVE_STRERROR) && defined(NEED_STRERROR)
    extern char *strerror(int errnum);
#endif				/* HAVE_STRERROR */
#if defined(HAVE_STRRCHR) && defined(NEED_STRRCHR)
    extern char *strrchr(const char *s, int c);
#endif				/* HAVE_STRRCHR */
#if defined(HAVE_STRTOK) && defined(NEED_STRTOK)
    extern char *strtok(char *s, const char *t);
#endif				/* HAVE_STRTOK */

#endif				/* STR_PTYPES */

/******************************************************************************
 * <sys/wait.h> definitions                                                   *
 ******************************************************************************/
#ifdef	WAI_PTYPES

#if defined(HAVE_WAIT) && defined(NEED_WAIT)
    extern pid_t wait(WAIT_ARG_TYPE * p);
#endif				/* HAVE_WAIT */

#endif				/* WAI_PTYPES */

/******************************************************************************
 * <time.h> definitions                                                       *
 ******************************************************************************/
#ifdef	TIM_PTYPES

#if defined(HAVE_MKTIME) && defined(NEED_MKTIME)
    extern time_t mktime(struct tm *t);
#endif				/* HAVE_MKTEMP */
#if defined(HAVE_TIME) && defined(NEED_TIME)
    extern time_t time(time_t *t);
#endif				/* HAVE_TIME */

#endif				/* TIM_PTYPES */

/******************************************************************************
 * <unistd.h> definitions                                                     *
 ******************************************************************************/

#if defined(HAVE__EXIT) && defined(NEED__EXIT)
    extern V_OR_I _exit(int code);
#endif				/* HAVE__EXIT */
#if defined(HAVE_FORK) && defined(NEED_FORK)
    extern int fork(void);
#endif				/* HAVE_FORK */
#if defined(HAVE_GETCWD) && defined(NEED_GETCWD)
    extern char *getcwd(char *p, size_t n);
#endif				/* HAVE_GETCWD */
#if defined(HAVE_GETEGID) && defined(NEED_GETEGID)
    extern gid_t getegid(void);
#endif				/* HAVE_GETEGID */
#if defined(HAVE_GETEUID) && defined(NEED_GETEUID)
    extern uid_t geteuid(void);
#endif				/* HAVE_GETEUID */
#if defined(HAVE_GETGID) && defined(NEED_GETGID)
    extern gid_t getgid(void);
#endif				/* HAVE_GETGID */
#if defined(HAVE_GETUID) && defined(NEED_GETUID)
    extern uid_t getuid(void);
#endif				/* HAVE_GETUID */
#if defined(HAVE_REALPATH) && defined(NEED_REALPATH)
    extern char *realpath(char *src, char *dst);
#endif				/* HAVE_REALPATH */

/******************************************************************************
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *+ NON-POSIX definitions                                                    +*
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 ******************************************************************************/

#if defined(HAVE__FILBUF) && defined(NEED__FILBUF)
    extern int _filbuf(FILE *s);
#endif				/* HAVE__FILBUF */
#if defined(HAVE__FLSBUF) && defined(NEED__FLSBUF)
    extern int _flsbuf(int n, FILE *s);
#endif				/* HAVE__FLSBUF */

#if defined(HAVE_BZERO) && defined(NEED_BZERO)
    extern void bzero(V_OR_P p, int n);
#endif				/* HAVE_BZERO */

#if defined(HAVE_GETOPT)
#if defined(HAVE_GETOPT_HEADER)
#if defined(NEED_GETOPT_H)
#include <getopt.h>
#endif
#elif defined(NEED_GETOPT)
    extern int getopt(int argc, char *const *argv, const char *opts);
#if defined(NEED_GETOPT)
    extern char *optarg;
    extern int optind;
#endif
#endif
#endif				/* HAVE_GETOPT */

#if defined(HAVE_GETWD)		/*&& NEED_GETWD */
#if defined(HAVE_GETWD) && defined(NEED_GETWD)
    extern char *getwd(char *p);
#endif				/* HAVE_GETWD */
#endif				/* HAVE_GETWD */
#ifdef	TIM_PTYPES
#if defined(HAVE_GETTIMEOFDAY) && defined(NEED_GETTIMEOFDAY)
    extern int gettimeofday(struct timeval *tv, struct timezone *tz);
#endif				/* HAVE_GETTIMEOFDAY */
#endif
#if defined(HAVE_IOCTL) && defined(NEED_IOCTL)
    extern int ioctl(int f, int m, ...);
#endif				/* HAVE_IOCTL */
#if defined(HAVE_LSTAT) && defined(NEED_LSTAT)
    extern int lstat(const char *f, Stat_t * s);
#endif				/* HAVE_LSTAT */
#if defined(HAVE_MEMMOVE) && defined(NEED_MEMMOVE)
    extern void *memmove(void *dest, const void *src, size_t n);
#endif				/* HAVE_MEMMOVE */
#if defined(HAVE_MKSTEMP) && defined(NEED_MKSTEMP)
    extern int mkstemp(char *s);
#endif				/* HAVE_MKSTEMP */
#if defined(HAVE_MKTEMP) && defined(NEED_MKTEMP)
    extern char *mktemp(char *s);
#endif				/* HAVE_MKTEMP */
#if defined(HAVE_PCLOSE) && defined(NEED_PCLOSE)
    extern int pclose(FILE *s);
#endif				/* HAVE_PCLOSE */
#if defined(HAVE_POPEN) && defined(NEED_POPEN)
    extern FILE *popen(const char *name, const char *mode);
#endif				/* HAVE_POPEN */
#if defined(HAVE_PUTENV) && defined(NEED_PUTENV)
    extern int putenv(const char *s);
#endif				/* HAVE_PUTENV */
#if defined(HAVE_READLINK) && defined(NEED_READLINK)
    extern int readlink(const char *f, char *b, size_t n);
#endif				/* HAVE_READLINK */
#if defined(HAVE_SETEGID) && defined(NEED_SETEGID)
    extern int setegid(gid_t g);
#endif				/* HAVE_SETEGID */
#if defined(HAVE_SETLINEBUF) && defined(NEED_SETLINEBUF)
    extern void setlinebuf(FILE *s);
#endif				/* HAVE_SETLINEBUF */
#if defined(HAVE_SETRGID) && defined(NEED_SETRGID)
    extern int setrgid(uid_t u);
#endif				/* HAVE_SETRGID */
#if defined(HAVE_SETRUID) && defined(NEED_SETRUID)
    extern int setruid(uid_t u);
#endif				/* HAVE_SETRUID */
#if defined(HAVE_SYMLINK) && defined(NEED_SYMLINK)
    extern int symlink(const char *s, const char *d);
#endif				/* HAVE_SYMLINK */
#if defined(HAVE_VFORK) && defined(NEED_VFORK)
    extern int vfork(void);
#endif				/* HAVE_VFORK */

#if defined(HAVE_GETCWD)	/* prefer Posix functions */
#define	getwd(p)	getcwd((p), (size_t)MAXPATHLEN)
#endif

#ifdef __cplusplus
}
#endif
#endif				/* TD_EXT_H */
