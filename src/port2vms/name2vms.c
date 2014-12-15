/*
 * Title:	name2vms.c
 * Author:	T.E.Dickey
 * Created:	29 Sep 1988
 * Modified:
 *		12 Dec 2014, tell coverity the expected buffer-sizes.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		25 Aug 1994, allow ".type" names as-is, and always uppercase.
 *		01 Dec 1993, ifdefs, TurboC warnings.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes
 *		01 Jun 1989, if 2nd-char in name is a '.' and 1st is not, only
 *			     change 2nd to a '$' if we expect that the name
 *			     might be an SCCS-file, for example.
 *		02 Mar 1989, added more test cases, changed (to work with new
 *			     'vms2name()') so that it recognizes node and device
 *			     names in form compatible with VMS/unix scheme.
 *		07 Dec 1988, supply trailing "." so that COPY will work
 *			     consistently when given result as destination.
 *			     Added 'getenv()'-based logical translation.
 *		30 Nov 1988, corrected handling of "./" and "../" prefixes
 *		28 Oct 1988, translate names beginning with "/" to top-level on
 *			     the current device.  Return destination buffer
 *			     address.
 *
 * Function:	Translates a unix-style name into a VMS-style name.
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: name2vms.c,v 12.9 2014/12/13 00:30:37 tom Exp $")

static int leaf_dot;		/* counts dots found in a particular leaf */
static int leaf_ver;		/* set if we found a DECshell version */

/*
 * If we have a dot in the second character position, force that to a dollar
 * sign.  Otherwise, keep the first dot in each unix leaf as a dot in the
 * resulting vms name.
 */
#define	ONE_CHAR_DOT	"sp"	/* e.g., for SCCS */

static int
prefix(const char *s)
{
    if (s[0] != EOS
	&& s[1] == '.'
	&& s[2] != EOS
	&& strchr(ONE_CHAR_DOT, s[0]))
	return (-1);
    return (0);
}

static char
translate(int c)
{
    if (isalpha(c)) {
	if (islower(c))
	    c = UpperMacro(c);
    } else if (c == '.') {
	if (leaf_dot++)
	    c = '$';
    } else if (!strchr("0123456789_-", c)) {
	c = '$';
    }
    return (char) (c);
}

static int
leading_uc(char dst[MAXPATHLEN], const char *src)
{
    char *base = dst;
    int c;

    while ((c = *src) != EOS && c != '/') {
	if (isalpha(c)) {
	    if (islower(c))
		return (0);
	} else if (!strchr("0123456789$_", c))
	    return (0);
	*dst++ = (char) c;
	*dst = EOS;
	src++;
    }
    *dst = EOS;
    if ((*base) && (dst = getenv(base)) != 0) {
	c = (int) strlen(base);
	while (isspace(UCH(*dst)))
	    dst++;
	(void) strcpy(base, dst);
	return (c);
    }
    return (0);
}

char *
name2vms(char dst[MAXPATHLEN], const char *src)
{
    char tmp[MAXPATHLEN], token[MAXPATHLEN], *t, *s = strcpy(tmp, src),		/* ... to permit src == dst */
     *d = dst, c = '?';
    int bracket = FALSE;	/* true when "[" passed. */
    int on_top = FALSE;		/* true when no "[." lead */
    int node = FALSE;		/* true when node found */
    int device = FALSE;		/* true when device found */
    int len;

    /*
     * If VMS 'getenv()' is given an upper-case name, it assumes that
     * it corresponds to a logical device assignment.  As a special
     * case, if we have a leading token of this form, translate it.
     */
    if ((len = leading_uc(token, s)) != 0) {
	s += len;
	len = (int) strlen(strcpy(d, token));
	while (len > 1 && d[len - 1] == ' ')
	    len--;
	if (*s) {		/* text follows leading token */
	    s++;		/* skip (assumed) '/' */
	    if ((len > 1)
		&& (d[len - 1] == ':')) {
		on_top = TRUE;
	    } else if (strchr(s, '/')) {	/* must do a splice */
		if ((len > 2)
		    && (d[len - 1] == ']')) {
		    bracket++;
		    if (d[len - 2] == '.')
			/* rooted-device ? */
			len -= 2;
		    else
			len--;
		}
	    }
	}
	d[len] = EOS;
	if ((t = strchr(d, ':')) != NULL) {
	    if (t[1] == ':') {
		node = TRUE;
		if ((t = strchr(t + 2, ':')) != NULL)
		    device = TRUE;
	    } else
		device = TRUE;
	}
	d += len;
    }

    /* look for node-name in VMS-format */
    if (!node
	&& (t = strchr(s, '!')) != 0
	&& (t[1] == '/' || t[1] == EOS)) {
	leaf_dot = prefix(s);
	while (s < t)
	    *d++ = translate(*s++);
	*d++ = ':';
	*d++ = ':';
	s++;			/* skip over '!' */
    }

    /* look for device-name, indicated by a leading '/' */
    if (!device
	&& (*s == '/')) {
	leaf_dot = prefix(++s);
	if ((t = strchr(s, '/')) == 0)
	    t = s + strlen(s);
	while (s < t)
	    *d++ = translate(*s++);
	*d++ = ':';
    }

    /* permit leading "./" to simplify cases in which we concatenate */
    if (!strncmp(s, "./", (size_t) 2))
	s += 2;

    /* translate repeated leading "../" */
    while (!strncmp(s, "../", (size_t) 3)) {
	s += 3;
	if (!bracket++)
	    *d++ = '[';
	*d++ = '-';
    }

    if (strchr(s, '/')) {
	if (!bracket++)
	    *d++ = '[';
	if (*s == '/') {
	    s++;
	} else if (!on_top) {
	    *d++ = '.';
	}
	leaf_dot = prefix(s);
	while ((c = *s++) != EOS) {
	    if (c == '/') {
		leaf_dot = prefix(s);
		if (strchr(s, '/'))
		    *d++ = '.';
		else {
		    break;
		}
	    } else {
		*d++ = translate(c);
	    }
	}
    }
    if (bracket)
	*d++ = ']';
    if (c != EOS && *s) {
	leaf_dot = prefix(s);
	while ((c = *s++) != EOS) {
	    if (c == '.'
		&& (leaf_ver = (strtol(s, &t, 0) && (t != s) && !*t)))
		*d++ = ';';
	    else
		*d++ = translate(c);
	}
	if (!leaf_dot)
	    *d++ = '.';
	if (!leaf_ver)
	    *d++ = ';';
    }
    *d = EOS;
    return (dst);
}

#ifdef	TEST
static void
do_test(int argc, const char **argv)
{
    int j;
    char buffer[MAXPATHLEN];

    if (argc <= 1) {
	static const char *array[] =
	{
	    "name",
	    ".type",
	    "name.type",
	    "name.type.junk",
	    "x/.type",
	    "x/y",
	    "x/y/z",
	    "/x",
	    "/x/y",
	    "/x/y/",
	    "/x.y",
	    "x.y.z",
	    "x.y.1",
	    "x.y.1x",
	    "x.y",
	    "s.x",
	    "s.x.y",		/* SCCS cases */
	    "s.",
	    "x.",
	    "HOME",
	    "HOME/x",
	    "HOME/x/y",
	    "BOOCH_LIB_ROOT",
	    "BOOCH_LIB/ada",
	    "BOOCH_LIB_ROOT/ada",
	    "BOOCH_LIB_ROOT/adalib/",
	    "BOOCH_LIB_ROOT/ada/x",
	    "SYS$DISK",
	    "SYS$DISK/x",
	    "SYS$DISK/x/y",
	    "x;y",
	    "node!",
	    "node!/dev",
	    "node!/dev/file",
	    "REPOS",
	    "REPOS/x"
	};
	do_test(SIZEOF(array), array);
    } else {
	for (j = 1; j < argc; j++) {
	    name2vms(buffer, argv[j]);
	    printf("\"%s\" => \"%s\"\n", argv[j], buffer);
	}
    }
}

/*ARGSUSED*/
_MAIN
{
    do_test(argc, (const char **) argv);
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif
