/* @(#)ptypes.h	1.2 88/05/19 10:16:48 */

#include <sys/types.h>
#include <sys/stat.h>

#ifndef	S_IFSOCK
#define	SYSTEM5
#endif	S_IFSOCK

/* declare 'exit()' properly, for lint ... */
#ifdef	SYS3_LLIB
extern	int	exit();
#else	SYS3_LLIB
#ifdef	SYSTEM5
extern	void	exit();
#else	SYSTEM5
extern		exit();
#endif	SYSTEM5
#endif	SYS3_LLIB
