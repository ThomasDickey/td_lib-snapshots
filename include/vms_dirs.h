/*
 * $Id: vms_dirs.h,v 12.2 2001/05/15 00:56:52 tom Exp $
 *
 * VMS definitions to interface to unix-like DIRECTORY(3) procedures.
 */
#ifndef	VMS_DIRS_H
#define	VMS_DIRS_H

#ifdef	vms
#include	<rms.h>
#ifndef	$DESCRIPTOR
#include	<descrip.h>
#endif	$DESCRIPTOR
#ifndef	$VMS_STATUS_SUCCESS
#include	<stsdef.h>
#endif	$VMS_STATUS_SUCCESS

/* *** nominal contents of "dir.h" */
#define	MAXPATHLEN  NAM$C_MAXRSS

typedef	struct	direct {
	unsigned long d_ino;
	short	d_reclen;
	short	d_namlen;
	char	d_name[NAM$C_MAXRSS];		/* result: SYS$SEARCH */
	};

typedef	struct	_dirdesc {
	struct	direct	dd_ret;
	struct	FAB	dd_fab;
	struct	NAM	dd_nam;
	char		dd_esa[NAM$C_MAXRSS];	/* expanded: SYS$PARSE */
	} DIR;
#else	/* SYS_UNIX */
#include <sys/dir.h>
#endif	/* vms/SYS_UNIX */

extern	DIR *		opendir(
			_ar1(char *,	filename)
			)
			_dcl(char *,	filename)
			_ret

extern	struct direct *	readdir(
			_ar1(DIR *,	dirp)
			)
			_dcl(DIR *,	dirp)
			_ret

extern	long		telldir(
			_ar1(DIR *,	dirp)
			)
			_dcl(DIR *,	dirp)
			_ret

extern	void		seekdir(
			_arx(DIR *,	dirp)
			_ar1(long,	loc)
			)
			_dcl(DIR *,	dirp)
			_dcl(long,	loc)
			_nul

extern	void		rewinddir(
			_ar1(DIR *,	dirp)
			)
			_dcl(DIR *,	dirp)
			_nul

extern	void		closedir(
			_ar1(DIR *,	dirp)
			)
			_dcl(DIR *,	dirp)
			_nul

#endif	VMS_DIRS_H
