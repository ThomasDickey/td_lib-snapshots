/*
 * $Id: port2vms.h,v 5.0 1991/05/20 17:19:07 ste_cm Rel $
 *
 * VMS-definitions for supporting unix/vms port
 */

#ifndef	_PORTUNIX_
#define	_PORTUNIX_

#include	"ptypes.h"		/* CM_TOOLS common */

#define	MAXPATHLEN	BUFSIZ		/* defined in <stdio.h> */

#define	isDIR(m)	((m & S_IFMT) == S_IFDIR)
#define	isFILE(m)	((m & S_IFMT) == S_IFREG)

#ifdef	vms

typedef	long	daddr_t;

#include	<time.h>
typedef	struct	timeval {
	long	tv_sec;
	long	tv_usec;
	};

#ifndef	getwd
#define	getwd(p)	getcwd(p,sizeof(p)-2)	/* patch: ptypes.h (5.0) */
extern	char		*getcwd();
#endif	/* getwd */

#define	bzero(p,len)	memset(p,0,len)
#define	bcopy(s,d,n)	memcpy(d,s,n)
#define	rmdir		remove
#define	unlink		remove

#define	_OPENDIR(s,m)	(isDIR(m) || vms_iswild(s))
#define	OPENDIR_ARG	"*.*;*"

#ifndef		$DESCRIPTOR
#include	<descrip.h>
#endif
		/* (try doing strings w/o descriptors!) */

#else	/* unix */

#include	<sys/time.h>		/* defines 'struct timeval' */

#define	_OPENDIR(s,m)	(isDIR(m))
#define	OPENDIR_ARG	"."

extern	char	*malloc();

#endif	/* vms/unix */

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

char *	gid2s(
		_AR1(int,		gid)
		)
		_DCL(int,		gid)
		_RET

char *	name2vms(
		_ARX(char *,		dst)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET

char *	path2dir(
		_AR1(char *,		src)
		)
		_DCL(char *,		src)
		_RET

char *	path2vms(
		_ARX(char *,		dst)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET

int	s2gid(
		_AR1(char *,		name)
		)
		_DCL(char *,		name)
		_RET

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
#endif	/* vms */

	transtree(
		_ARX(char *,		path)
		_FNX(int,		func)
		_AR1(int,		recur)
		)
		_DCL(char *,		path)
		_DCL(int,		(*func)())
		_DCL(int,		recur)
		_NUL

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

char *	vms2name(
		_ARX(char *,		dst)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		src)
		_RET

int	vms_iswild(
		_AR1(char *,		name)
		)
		_DCL(char *,		name)
		_RET

char *	vms_pathcat(
		_ARX(char *,		dst)
		_ARX(char *,		p)
		_AR1(char *,		n)
		)
		_DCL(char *,		dst)
		_DCL(char *,		p)
		_DCL(char *,		n)
		_RET

char *	vms_relpath(
		_ARX(char *,		dst)
		_ARX(char *,		cwd)
		_AR1(char *,		src)
		)
		_DCL(char *,		dst)
		_DCL(char *,		cwd)
		_DCL(char *,		src)
		_RET

time_t	zone2vms(
		_AR1(time_t,		reference)
		)
		_DCL(time_t,		reference)
		_RET

/* conversions from unix-form to native system */
#ifdef	vms
#define	NAME2SYS(dst,src)	name2vms(dst,src)
#define	PATH2SYS(dst,src)	path2vms(dst,src)
#define	SYS2NAME(dst,src)	vms2name(dst,src)
#define	ZONE2SYS(n)		zone2vms(n)
#else	/* unix */
#define	NAME2SYS(dst,src)	strcpy(dst,src)
#define	PATH2SYS(dst,src)	strcpy(dst,src)
#define	SYS2NAME(dst,src)	strcpy(dst,src)
#define	ZONE2SYS(n)		((time_t)0)
#endif	/* vms/unix */

/* conversions that assume native-system is VMS */
#ifdef	vms
#define	DIR2PATH(path)		dir2path(path)
#define	PATH2DIR(path)		path2dir(path)
#else	/* unix */
#define	DIR2PATH(path)		path
#define	PATH2DIR(path)		path
#endif	/* vms/unix */
 
#endif	/* _PORTUNIX_ */
