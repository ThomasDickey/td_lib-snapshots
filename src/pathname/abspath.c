#ifndef	lint
static	char	Id[] = "$Id: abspath.c,v 5.2 1989/12/08 08:58:48 dickey Exp $";
#endif	lint

/*
 * Author:	T.E.Dickey
 * Created:	17 Sep 1987
 * $Log: abspath.c,v $
 * Revision 5.2  1989/12/08 08:58:48  dickey
 * apollo sr10.1 provides get-name call with correct char-case
 *
 *		Revision 5.1  89/12/08  08:45:32  dickey
 *		added a set of default-cases for test-driver
 *		
 *		Revision 5.0  89/09/06  15:17:52  ste_cm
 *		BASELINE Fri Oct 27 12:27:25 1989 -- apollo SR10.1 mods + ADA_PITS 4.0
 *		
 *		Revision 4.1  89/09/06  15:17:52  dickey
 *		use "ptypes.h" to consolidate definitions
 *		
 *		Revision 4.0  88/09/09  07:56:04  ste_cm
 *		BASELINE Thu Aug 24 09:38:55 EDT 1989 -- support:navi_011(rel2)
 *		
 *		Revision 1.9  88/09/09  07:56:04  dickey
 *		sccs2rcs keywords
 *		
 *		09 Sep 1988, corrected case in which current directory ends in
 *			     "/".
 *		16 May 1988, 'getcwd()' is not as portable as 'getwd()'.
 *		05 May 1988, make "/tmp" translate ok on Apollo (must provide
 *			     missing node-name).  Also, translate csh-like "~".
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

#define	STR_PTYPES
#include	"ptypes.h"
#ifdef	apollo
#ifdef	__STDC__
#include	<apollo/base.h>
#include	<apollo/name.h>
#else	/* !__STDC__ */
#include	</sys/ins/base.ins.c>
#include	</sys/ins/name.ins.c>
#include	<ctype.h>
#endif	/* __STDC__ */
#endif	/* apollo */

#include	<pwd.h>
extern	char	*getenv();

#define	MAXPATHLEN	BUFSIZ

#ifdef	apollo
#define	TOP	2			/* permit 2 leading /'s */
#else
#define	TOP	1
#endif	apollo

extern	char	*denode();

static	char	nodestr[MAXPATHLEN];	/* will hold nodename, if any */
static	int	nodelen = -1;

#ifdef	apollo
static
apollo_name(path)
char	*path;
{
#ifdef	__STDC__			/* sr10.x */
	auto	status_$t	st;
	auto	unsigned short	out_len;

	name_$get_path_lc(path, strlen(path), BUFSIZ, path, &out_len, &st);
	if (st.all != status_$ok)
		out_len = 0;
	path[out_len] = EOS;
#else					/* sr9.x */
register char	*s, *d = path;
name_$pname_t	in_name, out_name;
short		in_len,
		out_len;
status_$t	st;

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
	}
#endif					/* apollo sr10.x vs sr9.x */
}
#endif	apollo

/*
 * Concatenate two pathnames to make a longer one.
 */
static
precat(prefix, string)
char	*prefix, *string;
{
register char *s;
char	tmp[MAXPATHLEN];
	s = strcpy(tmp, prefix) + strlen(prefix);
	if (*string)
		(void)strcat(strcat(s, "/"), string);
	(void)strcpy(string, tmp);
	*s = '\0';
}

/************************************************************************
 *	main procedure							*
 ************************************************************************/
abspath(path)
char	*path;
{
register char *s, *d = path;

	if (nodelen < 0) {	/* 'getwd()' is expensive... */
		(void)getwd(nodestr);
		(void)denode(nodestr,nodestr,&nodelen);
	}

	/*
	 * Check for references to someone's home directory in csh-style.
	 */
	if (*d == '~') {	/* my home directory */
		s = d+1;
		if ((*s == '\0') || (*s++ == '/')) {
			while (*d++ = *s++);
			precat(getenv("HOME"), d = path);
		} else {	/* someone else's home */
		extern	 struct passwd *getpwnam();	/* cf: apollo sys5 */
		register struct passwd *p;
		char	user[MAXPATHLEN];
			if (s = strchr(strcpy(user, d+1), '/'))
				*s++ = '\0';
			else
				s = d + strlen(d);
			if (p = getpwnam(user)) {
				while (*d++ = *s++);
				precat(p->pw_dir, d = path);
			}
			/* else no such home directory! */
		}
	}

	/*
	 * Convert special Apollo names.  This is necessary because the
	 * 'readlink()' call does not return a fully-resolved pathname,
	 * but may begin with "`".
	 */
#ifdef	apollo
	if ((*d == '`')
	||  (*d == '\\'))
		apollo_name(d);
#endif	apollo

	/*
	 * Strip altos-style nodename prefix, if applicable
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
	if (*path == '/') {
#ifdef	apollo
	static
	char	thisnode[MAXPATHLEN];
		if (path[1] != '/') {
			if (!thisnode[0])
				apollo_name(strcpy(thisnode, "/"));
			precat(thisnode, path);
		}
#else	apollo
		;
#endif	apollo
	} else if (*path) {
	char	cwdpath[MAXPATHLEN];
		d = getwd(cwdpath);
		s = path;
		if (*s == '.')
			if (s[1] == '\0' || s[1] == '/')
				s++;		/* absorb "." */
		d += strlen(cwdpath);
		if (d[-1] != '/')		/* add "/" iff we need it */
			(void)strcat(d, "/");
		(void)strcat(d, s);
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
do_test(argc,argv)
char	*argv[];
{
	register int	j;
	auto	 char	bfr[MAXPATHLEN];

	for (j = 1; j < argc; j++) {
		PRINTF("%d: %s\n", j, strcpy(bfr, argv[j]));
		abspath(bfr);
		PRINTF("=>  %s\n", bfr);
	}
}

main(argc, argv)
char	*argv[];
{
	if (argc > 1)
		do_test(argc, argv);
	else {
		static	char	*tbl[] = {
				 "?"
				,"."
				,".."
				,".././.."
				,"../."
				,"./.."
				,"/"
#ifdef	apollo
				,"`node_data"
#endif
				,"/tmp"
				};
		do_test(sizeof(tbl)/sizeof(tbl[0]), tbl);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	TEST
