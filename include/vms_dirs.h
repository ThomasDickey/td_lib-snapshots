/*
 * VMS definitions to interface to unix-like DIRECTORY(3) procedures.
 */
#ifndef	_UNIX_DIR
#include	<stdlib.h>
#include	<rms.h>
#ifndef	$DESCRIPTOR
#include	<descrip.h>
#endif	$DESCRIPTOR
#include	<stsdef.h>

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

extern	DIR		*opendir(char *filename);
extern	struct direct	*readdir(DIR *dirp);
extern	long		telldir(DIR *dirp);
extern			seekdir(DIR *dirp);
extern			rewinddir(DIR *dirp);
extern			closedir(DIR *dirp);
#define	_UNIX_DIR
#endif	_UNIX_DIR
