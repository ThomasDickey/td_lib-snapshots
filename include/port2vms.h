/*
 * $Id: port2vms.h,v 5.2 1991/12/10 09:13:10 dickey Exp $
 *
 * VMS-definitions for supporting unix/vms port
 */

#ifndef	_PORTUNIX_
#define	_PORTUNIX_

#include	"ptypes.h"		/* CM_TOOLS common */

#ifndef	MAXPATHLEN
#define	MAXPATHLEN	BUFSIZ		/* defined in <stdio.h> */
#endif

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

#endif	/* vms/unix */

/*
 * Library procedures for unix/vms compatability
 */
extern	int	deletetree(
		_arx(char *,		path)
		_ar1(int,		recur)
		)
		_dcl(char *,		path)
		_dcl(int,		recur)
		_ret

extern	char *	dir2path(
		_ar1(char *,		src)
		)
		_dcl(char *,		src)
		_ret

extern	int	editfile(
		_arx(char *,		name)
		_fn1(int,		func)
		)
		_dcl(char *,		name)
		_dcl(int,		(*func)())
		_ret

extern	int	edittree(
		_arx(char *,		name)
		_fnx(int,		func)
		_ar1(int,		recur)
		)
		_dcl(char *,		name)
		_dcl(int,		(*func)())
		_dcl(int,		recur)
		_ret

extern	char *	gid2s(
		_ar1(int,		gid)
		)
		_dcl(int,		gid)
		_ret

extern	char *	name2vms(
		_arx(char *,		dst)
		_ar1(char *,		src)
		)
		_dcl(char *,		dst)
		_dcl(char *,		src)
		_ret

extern	char *	path2dir(
		_ar1(char *,		src)
		)
		_dcl(char *,		src)
		_ret

extern	char *	path2vms(
		_arx(char *,		dst)
		_ar1(char *,		src)
		)
		_dcl(char *,		dst)
		_dcl(char *,		src)
		_ret

extern	int	s2gid(
		_ar1(char *,		name)
		)
		_dcl(char *,		name)
		_ret

extern	int	s2uid(
		_ar1(char *,		name)
		)
		_dcl(char *,		name)
		_ret

#ifdef	vms
extern	void	time2vms(
		_arx(long *,		vms_time)
		_ar1(time_t,		unix_time)
		)
		_dcl(long *,		vms_time)
		_dcl(time_t,		unix_time)
		_nul
#endif	/* vms */

extern	void	transtree(
		_arx(char *,		path)
		_fnx(int,		func)
		_arx(int,		recur)
		_ar1(int,		links)
		)
		_dcl(char *,		path)
		_dcl(int,		(*func)())
		_dcl(int,		recur)
		_dcl(int,		links)
		_nul

extern	char *	uid2s(
		_ar1(int,		uid)
		)
		_dcl(int,		uid)
		_ret

extern	int	utimes(
		_arx(char *,		filespec)
		_ar1(struct timeval *,	tv)
		)
		_dcl(char *,		filespec)
		_dcl(struct timeval *,	tv)
		_ret

extern	char *	vms2name(
		_arx(char *,		dst)
		_ar1(char *,		src)
		)
		_dcl(char *,		dst)
		_dcl(char *,		src)
		_ret

extern	int	vms_iswild(
		_ar1(char *,		name)
		)
		_dcl(char *,		name)
		_ret

extern	char *	vms_pathcat(
		_arx(char *,		dst)
		_arx(char *,		p)
		_ar1(char *,		n)
		)
		_dcl(char *,		dst)
		_dcl(char *,		p)
		_dcl(char *,		n)
		_ret

extern	char *	vms_relpath(
		_arx(char *,		dst)
		_arx(char *,		cwd)
		_ar1(char *,		src)
		)
		_dcl(char *,		dst)
		_dcl(char *,		cwd)
		_dcl(char *,		src)
		_ret

extern	time_t	zone2vms(
		_ar1(time_t,		reference)
		)
		_dcl(time_t,		reference)
		_ret

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
