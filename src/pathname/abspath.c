#ifndef	lint
static	char	sccs_id[] = "@(#)abspath.c	1.1 87/09/17 12:18:03";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Created:	17 Sep 1987
 * Modified:
 *
 * Function:	Modify the argument so that it contains an absolute (beginning
 *		with '/') pathname.  Relative paths may include the following
 *		cases (no embedded slashes):
 *			xx
 *			.
 *			./xx
 *		The ".." notation may be embedded at any point (we recursively
 *		strip it).
 */

#include	<stdio.h>
extern	char	*getcwd(),
		*strcat(),
		*strcpy();

#define	MAXPATHLEN	BUFSIZ

extern	char	*denode();

static	char	nodestr[MAXPATHLEN];	/* will hold nodename, if any */
static	int	nodelen = -1;

abspath(path)
char	*path;
{
register char *s, *d = path;

	if (nodelen < 0) {	/* 'getcwd()' is expensive... */
		(void)getcwd(nodestr,sizeof(nodestr)-2);
		(void)denode(nodestr,nodestr,&nodelen);
	}

	/*
	 * Strip nodename prefix, if applicable
	 */
	if (nodelen > 0) {
		if (strcmp(d, nodestr)) {
			if (d != (s = denode(d, nodestr, (int *)0)))
				while (*d++ = *s++);
		} else
			(void)strcpy(d, "/");
	}

	/*
	 * Convert this to an absolute path somehow
	 */
	if (*path == '/')
		;
	else if (*path) {
	char	cwdpath[MAXPATHLEN];
		(void)getcwd(cwdpath,sizeof(cwdpath)-2);
		s = path;
		if (*s == '.')
			if (s[1] == '\0' || s[1] == '/')
				s++;		/* absorb "." */
		(void)strcat(strcat(cwdpath, "/"), s);
		(void)strcpy(path,denode(cwdpath, nodestr, (int *)0));
	}

	/*
	 * Trim out repeated '/' marks
	 */
	for (s = d = path; *s; s++) {
		if (*s == '/') {
			while (s[1] == '/')
				s++;
		} else if (*s == '.') {
			if (s > path && s[-1] == '/') {
				if (s[1] == '\0')
					break;
				else if (s[1] == '/') {
					s++;
					continue;
				}
			}
		}
		*d++ = *s;
	}

	/*
	 * Trim trailing '/' marks
	 */
	while (d > path+1)
		if (d[-1] == '/')	d--;
		else			break;
	*d = '\0';

	/*
	 * Trim out ".." constructs
	 */
	for (s = path; *s; s++) {
		if (s[0] == '.' && s[1] == '.')
			if ((s > path && s[-1] == '/')
			&&  (s[2] == '\0' || s[2] == '/')) {
				d = s+2;
				if (s > path+1) {
					s -= 2;
					while (s > path && *s != '/') s--;
					if (s == path &&  !*d) s++;
				} else if (*d)
					s--;
				while (*s++ = *d++);
				s = path;	/* rescan */
			}
	}
}

#ifdef	TEST
main(argc,argv)
char	*argv[];
{
extern	void	exit();
register int j;
char	bfr[MAXPATHLEN];
	for (j = 1; j < argc; j++) {
		(void)printf("%d: %s\n", j, strcpy(bfr, argv[j]));
		abspath(bfr);
		(void)printf("=>  %s\n", bfr);
	}
	exit(0);
	/*NOTREACHED*/
}
#endif	TEST
