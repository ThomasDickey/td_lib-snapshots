/* @(#)ptypes.h	1.5 88/08/12 09:34:20 */

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

#ifndef	S_IFSOCK
#define	SYSTEM5
#endif	S_IFSOCK

/*
 * Declare functions which are int (or implicit) in some systems, but explicitly
 * void in system5:
 */
#ifdef	SYSTEM5
#define	V_OR_I	void
#else
#define	V_OR_I
#endif

extern	V_OR_I	_exit();
extern	V_OR_I	exit();
extern	V_OR_I	free();
extern	V_OR_I	perror();
extern	V_OR_I	qsort();
extern	V_OR_I	rewind();

/*
 * Miscellaneous useful definitions for readability
 */
#ifndef	TRUE
#define	TRUE	(1)
#define	FALSE	(0)
#endif	TRUE

#define	EOS	'\0'

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
extern	char	*doalloc();
#define	def_DOALLOC(t)
#define	def_ALLOC(t)
#define	DOALLOC(p,t,n)	(t *)doalloc((char *)p,sizeof(t)*(n))
#define	ALLOC(t,n)	DOALLOC(0,t,n)
#endif	lint

/*
 * System5 does not provide the directory manipulation procedures in bsd4.x;
 * define macros so we can use the bsd4.x names:
 */
#ifdef	DIR_PTYPES
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
#endif	DIR_PTYPES

/*
 * System5 curses does not define the 'screen' structure
 */
#ifdef	CUR_PTYPES
#ifdef	SYSTEM5
struct	screen	{ int dummy; };
#else	SYSTEM5
typedef char	chtype;		/* sys5-curses data-type */
#endif	SYSTEM5
#include	<curses.h>
#endif	CUR_PTYPES
