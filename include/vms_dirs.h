/*
 * $Id: vms_dirs.h,v 5.0 1989/09/14 14:57:43 ste_cm Rel $
 *
 * VMS definitions to interface to unix-like DIRECTORY(3) procedures.
 */
#ifndef	_UNIXDIR_
#define	_UNIXDIR_
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
#else	unix
#include <sys/dir.h>
#endif	vms/unix

DIR *		opendir(
		_AR1(char *,	filename)
		)
		_DCL(char *,	filename)
		_RET

struct direct *	readdir(
		_AR1(DIR *,	dirp)
		)
		_DCL(DIR *,	dirp)
		_RET

long		telldir(
		_AR1(DIR *,	dirp)
		)
		_DCL(DIR *,	dirp)
		_RET

		seekdir(
		_ARX(DIR *,	dirp)
		_AR1(long,	loc)
		)
		_DCL(DIR *,	dirp)
		_DCL(long,	loc)
		_NUL

		rewinddir(
		_AR1(DIR *,	dirp)
		)
		_DCL(DIR *,	dirp)
		_NUL

		closedir(
		_AR1(DIR *,	dirp)
		)
		_DCL(DIR *,	dirp)
		_NUL
#endif	_UNIXDIR_
