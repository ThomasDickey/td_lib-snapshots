#ifndef	lint
static	char	sccs_id[] = "@(#)abspath.c	1.3 88/05/03 12:34:40";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Created:	17 Sep 1987
 * Modified:
 *		03 May 1988, added more Apollo-specific prefixes
 *		23 Nov 1987, to work with Apollo path convention (leading '//')
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

#ifdef	apollo
#include	</sys/ins/base.ins.c>
#include	</sys/ins/name.ins.c>
#include	<ctype.h>
#endif	apollo

#include	<stdio.h>
extern	char	*getcwd(),
		*strcat(),
		*strcpy();

#define	MAXPATHLEN	BUFSIZ

#ifdef	apollo
#define	TOP	2			/* permit 2 leading /'s */
#else
#define	TOP	1
#endif	apollo

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
	 * Convert special Apollo names.  This is necessary because the
	 * 'readlink()' call does not return a fully-resolved pathname,
	 * but may begin with "`".
	 */
#ifdef	apollo
	if ((*d == '`')
	||  (*d == '\\')
	||  (*d == '~')) {
	name_$pname_t	in_name, out_name;
	short		in_len,
			out_len;
	status_$t	st;
	extern	char	*strncpy();

		in_len = strlen(strcpy(in_name, d));
		name_$get_path(in_name, in_len, out_name, out_len, st);

		if (st.all == status_$ok) {
			s = out_name;
			s[out_len] = '\0';
			/* Convert AEGIS name to UNIX name */
			while (*d = *s++) {
				if (*d == ':') {
					*d = *s++;
				} else if (*d == '#') {
				char	tmp[3];
				int	hex;
					(void)strncpy(tmp, s, 2);
					tmp[2] = '\0';
					s += strlen(tmp);
					sscanf(tmp, "%x", &hex);
					*d = hex;
				} else {
					if (isalpha(*d))
						*d = _tolower(*d);
				}
				d++;
			}
			d = path;
		}
	}
#endif	apollo

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
			if (s > (path+TOP-1))	/* provide for leading "//" */
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
	while (d > path+TOP)
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
				if (s > (path+TOP)) {
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
