/*
 * VMS-definitions for supporting unix/vms port
 */

#include	"ptypes.h"		/* CM_TOOLS common */

#undef	SUCCESS
#undef	FAIL

#define	MAXPATHLEN	BUFSIZ		/* defined in <stdio.h> */

#define	isDIR(m)	((m & S_IFMT) == S_IFDIR)
#define	isFILE(m)	((m & S_IFMT) == S_IFREG)

#ifdef	vms

#include	<stdlib.h>		/* defines lots of useful stuff */
#include	<stsdef.h>
#define	SUCCESS	(STS$M_INHIB_MSG | STS$K_SUCCESS)
#define	FAIL	(STS$M_INHIB_MSG | STS$K_ERROR)

typedef	long	daddr_t;

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

#include	<sys/time.h>		/* defines 'struct timeval' */

#define	_OPENDIR(s,m)	(isDIR(m))
#define	OPENDIR_ARG	"."

#endif	vms/unix

/*
 * Library procedures for unix/vms compatability
 */
int	deletetree(
		_ARX(char *,		path)
		_AR1(int,		recur)
		)
		_DCL(char *,		path)
		_DCL(int,		recur)
		_RET

char *	dir2path(
		_AR1(char *,		src)
		)
		_DCL(char *,		src)
		_RET

int	editfile(
		_ARX(char *,		name)
		_FN1(int,		func)
		)
		_DCL(char *,		name)
		_DCL(int,		(*func)())
		_RET

int	edittree(
		_ARX(char *,		name)
		_FNX(int,		func)
		_AR1(int,		recur)
		)
		_DCL(char *,		name)
		_DCL(int,		(*func)())
		_DCL(int,		recur)
		_RET

#ifdef	vms
char *	name2vms(
		_ARX(char *,		dst)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET
#else	unix
#define	name2vms(d,s)	strcpy(d,s)
#endif	vms/unix

char *	path2dir(
		_AR1(char *,		src)
		)
		_DCL(char *,		src)
		_RET

#ifdef	vms
char *	path2vms(
		_ARX(char *,		dst)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET
#else	unix
#define	path2vms(d,s)	strcpy(d,s)
#endif	vms/unix

int	s2uid(
		_AR1(char *,		name)
		)
		_DCL(char *,		name)
		_RET

#ifdef	vms
	time2vms(
		_ARX(long *,		vms_time)
		_AR1(time_t,		unix_time)
		)
		_DCL(long *,		vms_time)
		_DCL(time_t,		unix_time)
		_NUL
#endif	vms

int	transtree(
		_ARX(char *,		path)
		_FNX(int,		func)
		_AR1(int,		recur)
		)
		_DCL(char *,		path)
		_DCL(int,		(*func)())
		_DCL(int,		recur)
		_RET

char *	uid2s(
		_AR1(int,		uid)
		)
		_DCL(int,		uid)
		_RET

int	utimes(
		_ARX(char *,		filespec)
		_AR1(struct timeval *,	tv)
		)
		_DCL(char *,		filespec)
		_DCL(struct timeval *,	tv)
		_NUL

#ifdef	vms
char *	vms2name(
		_ARX(char *,		dst)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET
#else	unix
#define	vms2name(d,s)	strcpy(d,s)
#endif	vms/unix

#ifdef	vms
time_t	zone2vms(
		_AR1(time_t,		reference)
		)
		_DCL(time_t,		reference)
		_RET
#else	unix
#define	zone2vms(n)	((time_t)0)
#endif	vms/unix
