/*
 * Title:	vms2name.c
 * Author:	T.E.Dickey
 * Created:	02 Nov 1988
 * Modified:
 *		24 May 2010, fix clang --analyze warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs, TurboC warnings.
 *		22 Sep 1993, gcc warnings
 *		29 Apr 1993, added 'copying' to case-conversions.
 *		20 Nov 1992, use prototypes
 *		15 Sep 1989, added IMakefile and AMakefile cases.
 *		25 May 1989, handle special case of "[]".  Lowercase entire
 *			     pathname.  Generate ".." cases.
 *		13 Apr 1989, added special cases for leaf-editing which assume
 *			     that we got "s$" prefix from "s." (sccs), "$v"
 *			     suffix from ",v" (RCS), and "$" prefix from "."
 *			     (special unix "invisible" files).
 *		02 Mar 1989, added built-in test case.  Test for presence of
 *			     node and device specifications, and translate them
 *			     into VMS convention.  Still, be smart enough about
 *			     relative pathnames (e.g., "[.a]b") that we don't
 *			     automatically put the node and device in result.
 *		22 Feb 1989, added special cases of uppercase names.
 *		09 Jan 1989, trim trailing "." if suffix is null.
 *
 * Function:	Translates a VMS-style name into a unix-style name.
 *
 * Assumes:	The VMS-style name is syntactically correct.
 *		There is enough room in the output buffer to store the name.
 *
 * Notes:	This will translate a string which is either entirely a VMS
 *		pathname to a unix-style name, or one which is unix-style
 *		except for the leaf-name.  It looks explicitly for the ']'
 *		to determine where to put '/' marks.  This feature is used by
 *		vms-tar.
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: vms2name.c,v 12.10 2010/07/10 00:12:09 tom Exp $")

#define	LOWER(p)	((isalpha(UCH(*p)) && isupper(UCH(*p))) ? LowerMacro(*p) : *p)

static struct {
    int len;			/* number of chars to uppercase */
    const char *name;		/* lowercase string to check for */
} uc_names[] = {

    {
	1, "makefile"
    },
    {
	2, "amakefile"
    },
    {
	2, "imakefile"
    },
    {
	6, "readme"
    },
    {
	256, "read.me"
    },
    {
	256, "copying"
    },
    {
	256, "copyright"
    }
};

char *
vms2name(char *dst, const char *src)
{
    size_t j;
    int uc_len = 0;
    char current[MAXPATHLEN];
    int need_dev = FALSE, have_dev = FALSE, rcs_suffix = FALSE;
    char tmp[MAXPATHLEN];
    char *output = dst;
    char *base = tmp;
    char *s = strcpy(tmp, src);	/* ... to permit src == dst */
    char *d;

    if ((s = strchr(s, ';')) != NULL)	/* trim off version */
	*s = EOS;

    /* look for node specification */
    if ((s = strchr(base, ':')) != 0
	&& (s[1] == ':')) {
	while (base < s) {
	    *dst++ = (char) LOWER(base);
	    base++;
	}
	*dst++ = '!';
	base += 2;
	need_dev = TRUE;
    }

    /*
     * Look for device specification.  If not found, see if the path must
     * begin at the top of the device.  In this case, it would be ambiguous
     * if no device is supplied.
     */
    if ((s = strchr(base, ':')) != NULL) {
	*dst++ = '/';
	while (base < s) {
	    *dst++ = (char) LOWER(base);
	    base++;
	}
	base++;			/* skip over ":" */
	have_dev = TRUE;
    } else if (need_dev
	       || ((base[0] == '[')
		   && (base[1] != '-')
		   && (base[1] != '.')
		   && (base[1] != ']'))) {	/* must supply a device */
	const char *a = getwd(current);
	char *b;

	if (a == 0)
	    a = ":";

	b = strchr(a, '?');
	if ((b != 0)
	    && (b[1] == ':')) {	/* skip over node specification */
	    a = b + 2;
	    b = strchr(a, ':');
	}
	if (b != 0) {
	    *dst++ = '/';	/* begin the device */
	    while (a < b) {
		*dst++ = (char) LOWER(a);
		a++;
	    }
	    have_dev = TRUE;
	}			/* else, no device in getwd! */
    }

    /* translate directory-syntax */
    if ((s = strchr(base, '[')) != NULL) {
	if (s[1] == ']') {
	    *dst++ = '.';
	    if (s[2] != EOS)
		*dst++ = '/';
	    s += 2;
	    d = s;
	} else if (s[1] == '.') {
	    if (have_dev)
		*dst++ = '/';
	    s += 2;
	    d = s;
	} else if (s[1] == '-' && strchr("-.]", s[2])) {
	    s++;
	    while (*s == '-') {
		s++;
		*dst++ = '.';
		*dst++ = '.';
		if (*s == '.' && (s[1] == '-' || s[1] == ']'))
		    /* allow "-.-" */
		    s++;
		if (*s == '-')
		    *dst++ = '/';
	    }
	    d = s;
	} else {
	    d = s;
	    *s++ = '/';
	}
	/* expect s points to the last token before ']' */
	while (*s && *s != ']') {
	    if (*s == '.')
		*s = '/';
	    s++;
	}
	if (*s)
	    *s = s[1] ? '/' : EOS;
    } else {
	if (have_dev)
	    *dst++ = '/';
	d = base;
    }

    /*
     * Ensure that we map consistently to lowercase
     * 'd' points to the beginning of character-string which needs only
     * case-conversion (i.e., after ":" and "]").
     */
    for (s = dst; *d; s++, d++) {
	if (isalpha(UCH(*d)) && isupper(UCH(*d)))
	    *s = tolower(*d);
	else {
	    *s = *d;
	    if (*s == '.' && d[1] == EOS)
		*s = EOS;
	}
    }
    *s = EOS;

    /* check for uppercase names, by convention */
    if (!(s = strrchr(dst, '/')))
	s = dst;
    else
	s++;

    if (*s == '$') {
	*s++ = '.';
    } else if (s[0] == 's' && s[1] == '$') {
	s[1] = '.';
	s += 2;
    } else if (((j = strlen(s)) > 2)
	       && (s[j - 1] == 'v')
	       && (s[j - 2] == '$')) {
	rcs_suffix = TRUE;
	s[j - 2] = EOS;
    }

    if (!strncmp(s, "readme.", (size_t) 7))
	uc_len = 6;
    for (j = 0; j < SIZEOF(uc_names); j++) {
	if (!strcmp(s, uc_names[j].name)) {
	    uc_len = uc_names[j].len;
	    break;
	}
    }
    while (uc_len-- > 0 && *s) {
	if (isalpha(UCH(*s)))
	    *s = UpperMacro(*s);
	s++;
    }
    if (rcs_suffix)
	(void) strcat(dst, ",v");
    return (output);
}

#ifdef	TEST
static void
dotest(int argc, const char **argv)
{
    int j;
    char buffer[MAXPATHLEN];

    for (j = 1; j < argc; j++) {
	vms2name(buffer, argv[j]);
	printf("%d)\t%s => %s\n", j, argv[j], buffer);
    }
}

/*ARGSUSED*/
_MAIN
{
    char current[MAXPATHLEN];

    if (getcwd(current, sizeof(current)) == 0)
	strcpy(current, ".");

    printf("current directory = \"%s\"\n", current);
    if (argc > 1) {
	dotest(argc, (const char **) argv);
    } else {
	static const char *testv[] =
	{
	    "A",
	    "[A]",
	    "[A.B]",
	    "[.A]",
	    "[.A.B]",
	    "[A]B",
	    "[.A]B",
	    "[.A.B]C",
	    "[]",
	    "[]A",
	    "[-]",
	    "[-]A",
	    "[--]",
	    "[--]A",
	    "[-.]",
	    "[-.]A",
	    "[-.-]",
	    "[-.-]A",
	    "[-.A]",
	    "[-A]",
	    "[-]bin/tar.exe;26",	/* mixed-mode */
	    "$CSHRC",
	    "S$README",		/* SCCS */
	    "README$V",		/* RCS  */
	    "READ.ME",
	    "READ.ME;-1",
	    "README.TXT",
	    "README.",
	    "DEV:[A]",
	    "DEV:[.A]",
	    "NODE::DEV:A",
	    "NODE::DEV:[A]",
	    "NODE::A",
	    "NODE::[A]",
	    "NODE::[A.B]"
	};
	dotest(SIZEOF(testv), testv);
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
