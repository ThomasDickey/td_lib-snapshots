/*
 * VMS-definitions for supporting unix/vms port
 */

#include	<stdlib.h>	    /* defines lots of useful stuff */
#include	<stdio.h>
#define	MAXPATHLEN  BUFSIZ	    /* defined in <stdio.h> */

#define	isDIR(m)	((m & S_IFMT) == S_IFDIR)
#define	isFILE(m)	((m & S_IFMT) == S_IFREG)

#ifdef	vms

#include	<stsdef.h>
#define	SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#define	FAIL	(STS$M_INHIB_MSG | STS$K_ERROR)

#include	<types.h>
typedef	long	daddr_t;

#include	<stat.h>

#include	<time.h>
typedef	struct	timeval {
	long	tv_sec;
	long	tv_usec;
	};

#define	bzero(p,len)	memset(p,0,len)
#define	bcopy(s,d,n)	memcpy(d,s,n)
#define	rmdir		remove
#define	unlink		remove

#define	_OPENDIR(s,m)	(isDIR(m) || strchr(s,'*') || strchr(s,'%'))
#define	OPENDIR_ARG	"*.*;*"

#include	<descrip.h>
		/* (try doing strings w/o descriptors!) */

#else	unix

#define	SUCCESS	(0)
#define	FAIL	(1)

#include	<sys/types.h>
#include	<sys/time.h>	    /* defines 'struct timeval' */

#define	_OPENDIR(s,m)	(isDIR(m))
#define	OPENDIR_ARG	"."

#endif	vms/unix

#ifdef	vms
#define	__STDC__	/* not really, but supports function prototypes */
#endif

#ifdef	__STDC__
#define	_FNA(t,v)	t (*v)()
#define	_ARG(t,v)	t v
#define	_DCL(t,v)
#define	_RET		;
#define	_NUL		;
#else
#define	_FNA(t,v)	v
#define	_ARG(t,v)	v
#define	_DCL(t,v)	t v;
#define	_RET		{return(0);}
#define	_NUL		{}
#endif

/*
 * Library procedures for unix/vms compatability
 */
int	deletetree(
		_ARG(char *,		path),
		_ARG(int,		recur)
		)
		_DCL(char *,		path)
		_DCL(int,		recur)
		_RET

char *	dir2path(
		_ARG(char *,		src)
		)
		_DCL(char *,		src)
		_RET

char *	doalloc(
		_ARG(char *,		oldp),
		_ARG(unsigned,		amount)
		)
		_DCL(char *,		oldp)
		_DCL(unsigned,		amount)
		_RET

int	editfile(
		_ARG(char *,		name),
		_FNA(int,		func)
		)
		_DCL(char *,		name)
		_DCL(int,		(*func)())
		_RET

int	edittree(
		_ARG(char *,		name),
		_FNA(int,		func),
		_ARG(int,		recur)
		)
		_DCL(char *,		name)
		_DCL(int,		(*func)())
		_DCL(int,		recur)
		_RET

#ifdef	vms
char *	name2vms(
		_ARG(char *,		dst),
		_ARG(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET
#else	unix
#define	name2vms(d,s)	strcpy(d,s)
#endif	vms/unix

char *	path2dir(
		_ARG(char *,		src)
		)
		_DCL(char *,		src)
		_RET

#ifdef	vms
char *	path2vms(
		_ARG(char *,		dst),
		_ARG(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET
#else	unix
#define	path2vms(d,s)	strcpy(d,s)
#endif	vms/unix

char *	pathcat(
		_ARG(char *,		dst),
		_ARG(char *,		dirname),
		_ARG(char *,		filname)
		)
		_DCL(char *,		dst)
		_DCL(char *,		dirname)
		_DCL(char *,		filname)
		_RET

int	s2uid(
		_ARG(char *,		name)
		)
		_DCL(char *,		name)
		_RET

char *	stralloc(
		_ARG(char *,		s)
		)
		_DCL(char *,		s)
		_RET

int	strucmp(
		_ARG(char *,		dst),
		_ARG(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET

	strucpy(
		_ARG(char *,		dst),
		_ARG(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_NUL

#ifdef	vms
	time2vms(
		_ARG(long *,		vms_time),
		_ARG(time_t,		unix_time)
		)
		_DCL(long *,		vms_time)
		_DCL(time_t,		unix_time)
		_NUL
#endif	vms

int	transtree(
		_ARG(char *,		path),
		_FNA(int,		func),
		_ARG(int,		recur)
		)
		_DCL(char *,		path)
		_DCL(int,		(*func)())
		_DCL(int,		recur)
		_RET

char *	uid2s(
		_ARG(int,		uid)
		)
		_DCL(int,		uid)
		_RET

int	utimes(
		_ARG(char *,		filespec),
		_ARG(struct timeval *,	tv)
		)
		_DCL(char *,		filespec)
		_DCL(struct timeval *,	tv)
		_NUL

#ifdef	vms
char *	vms2name(
		_ARG(char *,		dst),
		_ARG(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET
#else	unix
#define	vms2name(d,s)	strcpy(d,s)
#endif	vms/unix

#ifdef	vms
time_t	zone2vms(
		_ARG(time_t,		reference)
		)
		_DCL(time_t,		reference)
		_RET
#else	unix
#define	zone2vms(n)	((time_t)0)
#endif	vms/unix
