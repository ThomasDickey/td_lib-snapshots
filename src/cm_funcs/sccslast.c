#ifndef	lint
static	char	sccs_id[] = "@(#)sccslast.c	1.2 87/01/23 08:59:31";
#endif	lint

/*
 * Created:	20 Oct 1986
 * Function:	Lookup the last sccs-delta date, and its release.version number
 *		for 'fl'.
 */

#include	<stdio.h>
#include	<sys/types.h>
extern	char	*strcpy();

extern	long	packdate();

#define	MAXPATH	256

/*
 * Set the release.version and date values iff we find a legal sccs-file at
 * 'path[]'.
 */
static	trysccs (path, rels_, vers_, date_)
char	*path;
unsigned char *rels_, *vers_;
time_t	*date_;
{
FILE	*fp = fopen(path, "r");
int	gotten = 0;

	if (fp) {
	char	bfr[BUFSIZ];
		while (fgets(bfr, sizeof(bfr), fp)) {
			if (!gotten) {
				if (strncmp(bfr, "\001h", 2))	break;
				gotten++;
			}
			if (!strncmp(bfr, "\001d D ", 4)) {
			int	rel, ver, yy, mm, dd, hr, mn, sc;
				if (sscanf(bfr+4, "%d.%d %d/%d/%d %d:%d:%d ",
					&rel, &ver,
					&yy, &mm, &dd,
					&hr, &mn, &sc) != 8)	break;
				*rels_ = rel;
				*vers_ = ver;
				*date_ = packdate (1900+yy, mm, dd, hr, mn, sc);
				break;
			}
		}
		(void) fclose(fp);
	}
}

sccslast (path, rels_, vers_, date_)
char	*path;
unsigned char *rels_, *vers_;
time_t	*date_;
{
char	name[MAXPATH+1];
register char *s;

	*rels_ = *vers_ = 0;
	*date_ = 0;

	/*
	 * First, find the actual filename in the 'path[]' variable.  If the
	 * name begins with 's.', assume that this may be an sccs-file, and
	 * open it.
	 */
	s = path + strlen(path);
	while (s > path) {
		if (s[-1] == '/')	break;
		s--;
	}
	if (!strncmp(s, "s.", 2)) {
		trysccs(path, rels_, vers_, date_);
		if (*date_)		return;
	}

	/*
	 * If the original path was not that of an sccs-file, look in the
	 * standard place (i.e., <directory>/sccs/s.<name>)
	 */
	(void) strcpy (name, path);
	(void) sprintf (&name[s-path], "sccs/s.%s", s);
	trysccs(name, rels_, vers_, date_);
}
