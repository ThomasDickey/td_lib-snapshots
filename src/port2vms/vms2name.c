#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/vms2name.c,v 1.1 1989/04/13 09:04:37 dickey Exp $";
#endif	lint

/*
 * Title:	vms2name.c
 * Author:	T.E.Dickey
 * Created:	02 Nov 1988
 * Modified:
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
 */

#define	STR_PTYPES
#include	"portunix.h"
#include	<ctype.h>
extern	char	*getcwd();

static	struct	{
		int	len;	/* number of chars to uppercase */
		char	*name;	/* lowercase string to check for */
	} uc_names[] = {
		1,	"makefile",
		256,	"readme",
		256,	"read.me",
		256,	"copyright"
	};

char *
vms2name(dst, src)
char	*dst, *src;
{
	register int j, uc_len = 0;
	auto	char	current[MAXPATHLEN];
	auto	int	need_dev = FALSE,
			have_dev = FALSE,
			rcs_suffix= FALSE;
	char	tmp[MAXPATHLEN],
		*base = tmp,
		*s = strcpy(tmp, src),	/* ... to permit src == dst */
		*d;

	if (s = strchr(s, ';')) {	/* trim off version */
		*s = EOS;
		s = tmp;
	}

	/* look for node specification */
	if ((s = strchr(base, ':'))
	&&  (s[1] == ':')) {
		while (base < s)
			*dst++ = *base++;
		*dst++ = '!';
		base += 2;
		need_dev = TRUE;
	}

	/*
	 * Look for device specification.  If not found, see if the path must
	 * begin at the top of the device.  In this case, it would be ambiguous
	 * if no device is supplied.
	 */
	if (s = strchr(base, ':')) {
		*dst++ = '/';
		while (base < s)
			*dst++ = *base++;
		base++;			/* skip over ":" */
		have_dev = TRUE;
	} else if (need_dev
	||	  ((base[0] == '[')
	&&	   (base[1] != '.'))) {	/* must supply a device */
		register char	*a = getcwd(current, sizeof(current)),
				*b = strchr(a ? a : "?", ':');
		if ((b != 0)
		&&  (b[1] == ':')) {	/* skip over node specification */
			a = b + 2;
			b = strchr(a, ':');
		}
		if (b != 0) {
			*dst++ = '/';	/* begin the device */
			while (a < b)
				*dst++ = *a++;
			have_dev = TRUE;
		}			/* else, no device in getcwd! */
	}

	/* translate directory-syntax */
	if (s = strchr(base, '[')) {
		if (s[1] == '.') {
			if (have_dev)
				*dst++ = '/';
			s += 2;
			d = s;
		} else {
			d = s;
			*s++ = '/';
		}
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

	/* ensure that we map consistently to lowercase */
	for (s = dst; *d; s++, d++) {
		if (isalpha(*d) && isupper(*d))
			*s = _tolower(*d);
		else {
			*s = *d;
			if (*s == '.' && d[1] == EOS)
			    *s = EOS;
		}
	}
	*s = EOS;

	/* check for uppercase names, by convention */
	if (!(s = strrchr(dst, '/')))	s = dst; else s++;

	if (*s == '$') {
		*s++ = '.';
	} else if (s[0] == 's' && s[1] == '$') {
		s[1] = '.';
		s += 2;
	} else if (	((j = strlen(s)) > 2)
		    &&	(s[j-1] == 'v')
		    &&	(s[j-2] == '$')) {
		rcs_suffix = TRUE;
		s[j-2] = EOS;
	}

	if (!strncmp(s, "readme.", 7))
		uc_len = strlen(s);
	for (j = 0; j < sizeof(uc_names)/sizeof(uc_names[0]); j++) {
		if (!strcmp(s, uc_names[j].name)) {
			uc_len = uc_names[j].len;
			break;
		}
	}
	while (uc_len-- > 0 && *s) {
		if (isalpha(*s))
			*s = _toupper(*s);
		s++;
	}
	if (rcs_suffix)
		(void)strcat(dst, ",v");
	return (dst);
}

#ifdef	TEST
dotest(argc, argv)
char	*argv[];
{
	register int j;
	char	buffer[MAXPATHLEN];

	for (j = 1; j < argc; j++) {
		vms2name(buffer, argv[j]);
		printf("%d)\t%s => %s\n", j, argv[j], buffer);
	}
}

main(argc, argv)
char	*argv[];
{
	auto	char	current[MAXPATHLEN];

	(void)getcwd(current, sizeof(current));
	printf("current directory = \"%s\"\n", current);
	if (argc > 1)
		dotest(argc, argv);
	else {
		static	char	*testv[] = {
				"a",
				"[a]",
				"[a.b]",
				"[.a]",
				"[.a.b]",
				"[a]b",
				"[.a]b",
				"[.a.b]c",
				"$cshrc",
				"s$readme",	/* sccs */
				"readme$v",	/* RCS  */
				"read.me",
				"read.me;-1",
				"readme.txt",
				"readme.",
				"dev:[a]",
				"dev:[.a]",
				"node::dev:a",
				"node::dev:[a]",
				"node::a",
				"node::[a]",
				"node::[a.b]"
				};
		dotest(sizeof(testv)/sizeof(testv[0]), testv);
	}
}
#endif	TEST
