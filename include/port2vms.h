/*
 * $Id: port2vms.h,v 12.11 2025/01/07 00:03:50 tom Exp $
 *
 * VMS-definitions for supporting unix/vms port
 */

#ifndef	PORT2VMS_H
#define	PORT2VMS_H

#include	<ptypes.h>	/* CM_TOOLS common */

#ifndef	MAXPATHLEN
#define	MAXPATHLEN	BUFSIZ	/* defined in <stdio.h> */
#endif

#ifdef	vms

typedef long daddr_t;

#include	<time.h>
typedef struct timeval {
    long tv_sec;
    long tv_usec;
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

#else /* SYS_UNIX or MSDOS */

#define	_OPENDIR(s,m)	(isDIR(m))
#define	OPENDIR_ARG	"."
#define	EDITDIR_ARG	"."

#endif /* vms/SYS_UNIX/MSDOS */

extern char *dir2path(
			 const char *src
);

#ifndef		TD_LIB_H
extern char *gid2s(
		      int gid
);
#endif

extern char *name2vms(
			 char *dst,
			 const char *src
);

extern char *path2dir(
			 const char *src
);

extern char *path2vms(
			 char *dst,
			 const char *src
);

#ifdef	vms
extern int rmsio_open(
			 char *name_,
			 char *mode_);

extern int rmsio_open2(
			  char *name_,
			  char *dft_,
			  char *mode_);

extern int rmsio_read(
			 int fd,
			 char *bfr,
			 int maxbfr);

extern int rmsio_write(
			  int fd,
			  char *bfr,
			  int maxbfr);

extern int rmsio_close(
			  int fd);

extern void rmsio_perror(
			    char *s);

extern int rmsio_size(
			 int fd);
#endif

#ifndef		TD_LIB_H
extern int s2gid(
		    char *name
);
#endif

#ifndef		TD_LIB_H
extern int s2uid(
		    char *name
);
#endif

#ifdef	vms
extern void time2vms(
			long *vms_time,
			time_t unix_time
);
#endif /* vms */

#ifndef		TD_LIB_H
extern char *uid2s(
		      int uid
);
#endif

#ifdef	vms
extern int utimes(
		     char *filespec,
		     struct timeval *tv
);
#endif /* vms */

extern char *vms2name(
			 char *dst,
			 const char *src
);

extern int vms_iswild(
			 const char *name
);

extern char *vms_pathcat(
			    char *dst,
			    char *p,
			    char *n
);

extern char *vms_relpath(
			    char *dst,
			    char *cwd,
			    char *src
);

extern time_t zone2vms(
			  time_t reference
);

/* conversions from unix-form to native system */
#ifdef	vms
#define	NAME2SYS(dst,src)	name2vms(dst,src)
#define	PATH2SYS(dst,src)	path2vms(dst,src)
#define	SYS2NAME(dst,src)	vms2name(dst,src)
#define	ZONE2SYS(n)		zone2vms(n)
#else /* SYS_UNIX */
#define	NAME2SYS(dst,src)	strcpy(dst,src)
#define	PATH2SYS(dst,src)	strcpy(dst,src)
#define	SYS2NAME(dst,src)	strcpy(dst,src)
#define	ZONE2SYS(n)		((time_t)0)
#endif /* vms/SYS_UNIX */

/* conversions that assume native-system is VMS */
#ifdef	vms
#define	DIR2PATH(path)		dir2path(path)
#define	PATH2DIR(path)		path2dir(path)
#else /* SYS_UNIX */
#define	DIR2PATH(path)		path
#define	PATH2DIR(path)		path
#endif /* vms/SYS_UNIX */

#endif /* PORT2VMS_H */
