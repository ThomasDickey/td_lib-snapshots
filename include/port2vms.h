/*
 * $Id: port2vms.h,v 12.6 1995/10/31 23:22:05 tom Exp $
 *
 * VMS-definitions for supporting unix/vms port
 */

#ifndef	PORT2VMS_H
#define	PORT2VMS_H

#include	"ptypes.h"		/* CM_TOOLS common */

#ifndef	MAXPATHLEN
#define	MAXPATHLEN	BUFSIZ		/* defined in <stdio.h> */
#endif

#ifdef	vms

typedef	long	daddr_t;

#include	<time.h>
typedef	struct	timeval {
	long	tv_sec;
	long	tv_usec;
	};

#define	rmdir		remove
#define	unlink		remove

#define	_OPENDIR(s,m)	(isDIR(m) || vms_iswild(s))
#define	OPENDIR_ARG	"*.*;*"
#define	EDITDIR_ARG	"*.*;"

/*#ifndef	$DESCRIPTOR*/
#include	<descrip.h>
/*#endif*/
		/* (try doing strings w/o descriptors!) */

#else	/* unix or MSDOS */

#define	_OPENDIR(s,m)	(isDIR(m))
#define	OPENDIR_ARG	"."
#define	EDITDIR_ARG	"."

#endif	/* vms/unix/MSDOS */

extern	char *	dir2path(
		_ar1(char *,		src)
		)
		_dcl(char *,		src)
		_ret

#ifndef		TD_LIB_H
extern	char *	gid2s(
		_ar1(int,		gid)
		)
		_dcl(int,		gid)
		_ret
#endif

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

#ifdef	vms
extern	int	rmsio_open (
		_arx(char *,	name_)
		_ar1(char *,	mode_));

extern	int	rmsio_open2 (
		_arx(char *,	name_)
		_arx(char *,	dft_)
		_ar1(char *,	mode_));

extern	int	rmsio_read (
		_arx(int,	fd)
		_arx(char *,	bfr)
		_ar1(int,	maxbfr));

extern	int	rmsio_write (
		_arx(int,	fd)
		_arx(char *,	bfr)
		_ar1(int,	maxbfr));

extern	int	rmsio_close (
		_ar1(int,	fd));

extern	void	rmsio_perror (
		_ar1(char *,	s));

extern	int	rmsio_size (
		_ar1(int,	fd));
#endif

#ifndef		TD_LIB_H
extern	int	s2gid(
		_ar1(char *,		name)
		)
		_dcl(char *,		name)
		_ret
#endif

#ifndef		TD_LIB_H
extern	int	s2uid(
		_ar1(char *,		name)
		)
		_dcl(char *,		name)
		_ret
#endif

#ifdef	vms
extern	void	time2vms(
		_arx(long *,		vms_time)
		_ar1(time_t,		unix_time)
		)
		_dcl(long *,		vms_time)
		_dcl(time_t,		unix_time)
		_nul
#endif	/* vms */

#ifndef		TD_LIB_H
extern	char *	uid2s(
		_ar1(int,		uid)
		)
		_dcl(int,		uid)
		_ret
#endif

#ifdef	vms
extern	int	utimes(
		_arx(char *,		filespec)
		_ar1(struct timeval *,	tv)
		)
		_dcl(char *,		filespec)
		_dcl(struct timeval *,	tv)
		_ret
#endif	/* vms */

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
 
#endif	/* PORT2VMS_H */
