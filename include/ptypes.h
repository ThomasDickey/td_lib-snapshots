/* @(#)ptypes.h	1.3 88/08/09 09:51:06 */

#include <sys/types.h>
#include <sys/stat.h>

#ifndef	S_IFSOCK
#define	SYSTEM5
#endif	S_IFSOCK

/* declare 'exit()' properly, for lint ... */
#ifdef	SYS3_LLIB
extern	int	exit();
#else	SYS3_LLIB
#ifdef	SYSTEM5
extern	void	exit();
#else	SYSTEM5
extern		exit();
#endif	SYSTEM5
#endif	SYS3_LLIB

/* miscellaneous useful definitions for linting */
#ifndef	TRUE
#define	TRUE	(1)
#define	FALSE	(0)
#endif	TRUE

#define	EOS	'\0'

#define	PRINTF	(void)printf
#define	FORMAT	(void)sprintf

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
