#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/vms_dirs.c,v 1.1 1988/09/28 15:07:13 dickey Exp $";
#endif	lint

/*
 * Title:	unixdir.c
 * Author:	T.E.Dickey
 * Created:	28 Sep 1988
 *
 * Function:	This implements (in a crude way!) the unix-compatible directory
 *		scanning procedures.
 */
#include	"unixdir.h"

#define	zfab	dirp->dd_fab
#define	znam	dirp->dd_nam
#define	zrsa	dirp->dd_ret.d_name
#define	zesa	dirp->dd_esa

DIR *
opendir(filename)
char	*filename;
{
	DIR	*dirp = calloc(1, sizeof(DIR));
	long	status;

	if (dirp == 0)
		return (0);

	zfab = cc$rms_fab;
	zfab.fab$l_fop = FAB$M_NAM;
	zfab.fab$l_nam = &znam;		/* FAB => NAM block	*/
	zfab.fab$l_dna = "*.*;*";	/* Default-selection	*/
	zfab.fab$b_dns = strlen(zfab.fab$l_dna);

	zfab.fab$l_fna = filename;
	zfab.fab$b_fns = strlen(filename);

	znam = cc$rms_nam;
	znam.nam$b_ess = NAM$C_MAXRSS;
	znam.nam$l_esa = zesa;
	znam.nam$b_rss = NAM$C_MAXRSS;
	znam.nam$l_rsa = zrsa;

	if (sys$parse(&zfab) != RMS$_NORMAL) {
		closedir(dirp);
		dirp = 0;
	}
	return (dirp);
}

struct direct *
readdir(dirp)
DIR	*dirp;
{
	if (sys$search(&zfab) == RMS$_NORMAL) {
		zrsa[znam.nam$b_rsl] = '\0';
		return (&(dirp->dd_ret));
	}
	return (0);
}

long
telldir(dirp)
DIR	*dirp;
{
	/* not implemented */
}

seekdir(dirp, loc)
DIR	*dirp;
{
	/* not implemented */
}

rewinddir(dirp)
DIR	*dirp;
{
	/* not implemented */
}

closedir(dirp)
DIR	*dirp;
{
	cfree(dirp);
}

#ifdef	TEST
#include	<stdio.h>

main(argc, argv)
char	*argv[];
{
	DIR	*dirp;
	struct	direct	*dp;

	register int	j;
	for (j = 1; j < argc; j++) {
		printf("%d:%s\n", j, argv[j]);
		if (dirp = opendir(argv[j])) {
			while (dp = readdir(dirp))
				printf("\t%s\n", dp->d_name);
			closedir(dirp);
		}
	}
}
#endif	TEST
