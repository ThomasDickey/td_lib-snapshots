/*
 * Title:	unixdir.c
 * Author:	T.E.Dickey
 * Created:	28 Sep 1988
 * Modified:
 *		01 Dec 1993, ifdefs.
 *
 * Function:	This implements (in a crude way!) the unix-compatible directory
 *		scanning procedures.
 */
#define		DIR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: vms_dirs.c,v 12.2 1994/08/21 18:39:51 tom Exp $")

#ifdef	vms
#define	zfab	dirp->dd_fab
#define	znam	dirp->dd_nam
#define	zrsa	dirp->dd_ret.d_name
#define	zesa	dirp->dd_esa

DIR *
opendir(
_AR1(char *,	filename))
_DCL(char *,	filename)
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
readdir(
_AR1(DIR *,	dirp))
_DCL(DIR *,	dirp)
{
	if (sys$search(&zfab) == RMS$_NORMAL) {
		zrsa[znam.nam$b_rsl] = '\0';
		return (&(dirp->dd_ret));
	}
	return (0);
}

long
telldir(
_AR1(DIR *,	dirp))
_DCL(DIR *,	dirp)
{
	/* not implemented */
}

void
seekdir(
_ARX(DIR *,	dirp)
_AR1(long,	loc)
	)
_DCL(DIR *,	dirp)
_DCL(long,	loc)
{
	/* not implemented */
}

void
rewinddir(
_AR1(DIR *,	dirp))
_DCL(DIR *,	dirp)
{
	/* not implemented */
}

void
closedir(
_AR1(DIR *,	dirp))
_DCL(DIR *,	dirp)
{
	cfree(dirp);
}

#ifdef	TEST
#include	<stdio.h>

/*ARGSUSED*/
_MAIN
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
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */

#endif	/* vms */
