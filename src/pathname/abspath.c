/*
 * Author:	T.E.Dickey
 * Created:	17 Sep 1987
 * Modified:
 *		11 Oct 2022, gcc warnings
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Nov 2000, modified to work with OS/2 EMX.
 *		10 Jan 1996, corrected handling of "~foo/bar/bar" in abshome.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		13 Nov 1992, removed redundant 'index' macro.
 *		04 Oct 1991, conversion to ANSI
 *		22 Jul 1991, moved MAXPATHLEN def to "ptypes.h"
 *		04 Apr 1991, try to recover in case 'getwd()' fails.
 *		24 Apr 1990, added entrypoint 'abshome()', which we use when
 *			     tilde-expansion is sufficient.
 *		08 Dec 1989, apollo sr10.1 provides get-name call with correct
 *			     char-case.  Added a set of default-cases for test-
 *			     driver.
 *		06 Sep 1989, use "ptypes.h" to consolidate definitions
 *		09 Sep 1988, corrected case in which current directory ends in
 *			     slash.
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

#define	CHR_PTYPES
#define	PWD_PTYPES
#define	STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: abspath.c,v 12.17 2025/01/07 00:29:46 tom Exp $")

#ifdef	apollo
#ifdef	apollo_sr10
#ifdef	lint
#define	const
#ifndef	index
#define	index	Index
#endif
#endif /* lint */
#include	<apollo/base.h>
#include	<apollo/name.h>
#else /* !apollo_sr10 */
#include	</sys/ins/base.ins.c>
#include	</sys/ins/name.ins.c>
#endif /* apollo_sr10 */
#endif /* apollo */

#ifdef	apollo
#define	TOP	2		/* permit 2 leading /'s */
#else
#define	TOP	1
#endif /* apollo */

static char slash[] =
{PATH_SLASH, EOS};		/* "/"   */

#ifdef	SYS_UNIX
static char nodestr[MAXPATHLEN];	/* will hold nodename, if any */
static int nodelen = -1;
#endif

#ifdef	apollo
static
apollo_name(char *path)
{
#ifdef	apollo_sr10		/* sr10.x */
    status_$t st;
    unsigned short out_len;

    name_$get_path_lc(
			 path, (unsigned short) strlen(path),
			 BUFSIZ, path, &out_len, &st);
    if (st.all != status_$ok)
	out_len = 0;
    path[out_len] = EOS;
#else /* sr9.x */
    char *s, *d = path;
    name_$pname_t in_name, out_name;
    short in_len, out_len;
    status_$t st;

    in_len = strlen(strcpy(in_name, d));
    name_$get_path(in_name, in_len, out_name, out_len, st);

    if (st.all == status_$ok) {
	s = out_name;
	s[out_len] = EOS;
	/* Convert AEGIS name to UNIX name */
	while (*d = *s++) {
	    if (*d == ':') {
		*d = *s++;
	    } else if (*d == '#') {
		char tmp[3];
		int hex;

		(void) strncpy(tmp, s, 2);
		tmp[2] = EOS;
		s += strlen(tmp);
		sscanf(tmp, "%x", &hex);
		*d = hex;
	    } else {
		LowerCase(*d);
	    }
	    d++;
	}
    }
#endif /* apollo sr10.x vs sr9.x */
}
#endif /* apollo */

/*
 * Concatenate two pathnames to make a longer one.
 */
static void
precat(const char *prefix, char string[MAXPATHLEN])
{
    char *s;
    char tmp[MAXPATHLEN];
    size_t need = strlen(prefix);

    if (need + 1 < sizeof(tmp)) {
	s = strcpy(tmp, prefix) + strlen(prefix);
	if (*string)
	    (void) strcat(strcat(s, slash), string);
	(void) strcpy(string, tmp);
	*s = EOS;
    }
}

/************************************************************************
 *	main procedures							*
 ************************************************************************/
#if	defined(SYS_UNIX) || defined(MSDOS)
void
abshome(char *path)
{
    char *s, *d = path;

    if (*d == '~') {		/* my home directory */
	s = d + 1;
	if ((*s == EOS) || isSlash(*s)) {
	    if (*s != EOS) {
		s++;
	    }
	    while ((*d++ = *s++) != EOS) {
		;
	    }
	    precat(getenv("HOME"), path);
	} else {		/* someone else's home */
#ifdef	MSDOS
	    while ((*d++ = *s++) != EOS) {
		;
	    }
	    precat("/users", path);
#else
	    struct passwd *p;
	    char user[MAXPATHLEN];

	    if (strlen(d) < sizeof(user)) {
		for (s = strcpy(user, d + 1); *s != EOS; s++) {
		    if (isSlash(*s)) {
			*s++ = EOS;
			break;
		    }
		}
		if ((p = getpwnam(user)) != NULL) {
		    while ((*d++ = *s++) != EOS) {
			;
		    }
		    if ((strlen(p->pw_dir) + strlen(path)) < MAXPATHLEN) {
			precat(p->pw_dir, path);
		    }
		}
		/* else no such home directory! */
	    }
#endif
	}
    }
#ifdef __CYGWIN__
    strlwrcpy(path, path);
#endif
}
#endif

void
abspath(char *path)
{
    char *base = path;
    char *s, *d = base;

#ifdef	SYS_UNIX
    if (nodelen < 0) {		/* 'getwd()' is expensive... */
	if (getwd(nodestr))
	    (void) denode(nodestr, nodestr, &nodelen);
    }
#endif /* SYS_UNIX */

    /*
     * Check for references to someone's home directory in csh-style.
     */
    abshome(base);

    /*
     * Skip DOS-style device
     */
#if defined(MSDOS) || defined(__EMX__)
    if (isalpha(*base) && base[1] == ':') {
	path += 2;
	d = path;
    }
#endif /* MSDOS */

    /*
     * Convert special Apollo names.  This is necessary because the
     * 'readlink()' call does not return a fully-resolved pathname,
     * but may begin with "`".
     */
#ifdef	apollo
    if ((*d == '`')
	|| (*d == '\\'))
	apollo_name(d);
#endif /* apollo */

    /*
     * Strip altos-style nodename prefix, if applicable
     */
#ifdef	SYS_UNIX
    if (nodelen > 0) {
	if (strcmp(d, nodestr)) {
	    if (d != (s = denode(d, nodestr, (int *) 0)))
		while ((*d++ = *s++) != EOS) ;
	} else {
	    (void) strcpy(d, slash);
	}
    }
#endif

    /*
     * Convert this to an absolute path somehow
     */
    if (isSlash(*path)) {
#ifdef	apollo
	static char thisnode[MAXPATHLEN];

	if (!isSlash(path[1])) {
	    if (!thisnode[0])
		apollo_name(strcpy(thisnode, slash));
	    precat(thisnode, path);
	}
#else /* !apollo */
	;
#endif /* apollo */
    } else if (*path) {
	char cwdpath[MAXPATHLEN];

	if ((d = getwd(cwdpath)) != NULL) {
	    s = path;
	    if (*s == '.')
		if (s[1] == EOS || isSlash(s[1]))
		    s++;	/* absorb "." */
	    d += strlen(cwdpath);
	    if (!isSlash(d[-1]))	/* add slash iff we need it */
		(void) strcat(d, slash);
	    (void) strcat(d, s);
#if defined(MSDOS) || defined(__EMX__)
	    (void) strcpy(base, cwdpath);
#else
	    (void) strcpy(base, denode(cwdpath, nodestr, (int *) 0));
#endif /* MSDOS */
	}
    }

    /*
     * Trim out repeated slash-marks
     */
    for (s = d = path; *s; s++) {
	if (isSlash(*s)) {
	    if (s > (path + TOP - 1))	/* provide for leading "//" */
		while (isSlash(s[1]))
		    s++;
	} else if (*s == '.') {
	    if (s > path && isSlash(s[-1])) {
		if (s[1] == EOS)
		    break;
		else if (isSlash(s[1])) {
		    s++;
		    continue;
		}
	    }
	}
	*d++ = *s;
    }

    /*
     * Trim trailing slash-marks
     */
    while (d > path + TOP)
	if (isSlash(d[-1]))
	    d--;
	else
	    break;
    *d = EOS;

    /*
     * Trim out ".." constructs
     */
    for (s = path; *s; s++) {
	if (s[0] == '.' && s[1] == '.')
	    if ((s > path && isSlash(s[-1]))
		&& (s[2] == EOS || isSlash(s[2]))) {
		d = s + 2;
		if (s > (path + TOP)) {
		    s -= 2;
		    while (s > path && !isSlash(*s))
			s--;
		    if (s == path && !*d)
			s++;
		} else if (*d)
		    s--;
		while ((*s++ = *d++) != EOS) ;
		s = path;	/* rescan */
	    }
    }
#ifdef __CYGWIN__
    strlwrcpy(base, base);
#endif
}

#ifdef	TEST
static void
do_test(int argc, const char **argv)
{
    int j;
    char bfr[MAXPATHLEN];

    for (j = 1; j < argc; j++) {
	PRINTF("%d: %s\n", j, strcpy(bfr, argv[j]));
	abspath(bfr);
	PRINTF("=>  %s\n", bfr);
    }
}

_MAIN
{
    if (argc > 1) {
	const char **params = (const char **) argv;
	do_test(argc, params);
    } else {
	static const char *tbl[] =
	{
	    "?"
	    ,"."
	    ,".."
	    ,".././.."
	    ,"../."
	    ,"./.."
	    ,"/"
	    ,"/tmp"
#ifdef	apollo			/* put these on the end */
	    ,"`node_data"
#endif
	};
	do_test(SIZEOF(tbl), tbl);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
