/*
 * $Id: vms_dirs.h,v 12.3 2004/03/07 21:45:49 tom Exp $
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
			char *	filename
			)
			;

extern	struct direct *	readdir(
			DIR *	dirp
			)
			;

extern	long		telldir(
			DIR *	dirp
			)
			;

extern	void		seekdir(
			DIR *	dirp,
			long	loc
			)
			;

extern	void		rewinddir(
			DIR *	dirp
			)
			;

extern	void		closedir(
			DIR *	dirp
			)
			;

#endif	VMS_DIRS_H
