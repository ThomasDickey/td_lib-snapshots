#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/port2vms/RCS/name2vms.c,v 1.1 1989/03/02 11:38:03 dickey Exp $";
#endif	lint

/*
 * Title:	name2vms.c
 * Author:	T.E.Dickey
 * Created:	29 Sep 1988
 * Modified:
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

#define	STR_PTYPES
#include	"portunix.h"
#include	<ctype.h>
extern	char	*getenv();

static	int	leaf_dot;   /* counts dots found in a particular leaf */

/*
 * If we have a dot in one of the first two character positions, force that to
 * a dollar sign.  Otherwise, keep the first dot in each unix leaf as a dot in
 * the resulting vms name.
 */
#define	PREFIX(s)   (s[0] == '.' || s[1] == '.') ? -1 : 0

static	char
translate(c)
{
	if (isalpha(c))
		c = toupper(c);
	else if (c == '.') {
		if (leaf_dot++)
			c = '$';
	} else if (!strchr("0123456789_-", c))
		c = '$';
	return (c);
}

static
leading_uc(dst, src)
char	*dst, *src;
{
	auto	char	*base = dst;
	register int	c;

	while ((c = *src) && c != '/') {
		if (isalpha(c)) {
			if (islower(c))
				return (0);
		} else if (!strchr("0123456789$_", c))
			return (0);
		*dst++ = c;
		*dst   = EOS;
		src++;
	}
	if ((*base) && (dst = getenv(base))) {
		c = strlen(base);
		while (isspace(*dst))	dst++;
		(void)strcpy(base, dst);
		return (c);
	}
	return (0);
}

char *
name2vms(dst, src)
char	*dst, *src;
{
	char	tmp[MAXPATHLEN],
		token[MAXPATHLEN],
		*t,
		*s = strcpy(tmp, src),	/* ... to permit src == dst */
		*d = dst,
		c  = '?';
	auto	int	bracket	= FALSE,	/* true when "[" passed. */
			on_top	= FALSE,	/* true when no "[." lead */
			node	= FALSE,	/* true when node found */
			device	= FALSE,	/* true when device found */
			len;

	/*
	 * If VMS 'getenv()' is given an upper-case name, it assumes that
	 * it corresponds to a logical device assignment.  As a special
	 * case, if we have a leading token of this form, translate it.
	 */
	if (len = leading_uc(token,s)) {
		s  += len;
		len = strlen(strcpy(d, token));
		while (len > 1 && d[len-1] == ' ')
			len--;
		if (*s) {		/* text follows leading token */
			s++;		/* skip (assumed) '/' */
			if ((len > 1)
			&&  (d[len-1] == ':'))
				on_top = TRUE;
			else if (strchr(s, '/')) {	/* must do a splice */
				if ((len > 2)
				&&  (d[len-1] == ']')) {
					bracket++;
					if (d[len-2] == '.')
						/* rooted-device ? */
						len -= 2;
					else
						len--;
				}
			}
		}
		d[len] = EOS;
		if (t = strchr(d, ':')) {
			if (t[1] == ':') {
				node = TRUE;
				if (t = strchr(t+2, ':'))
					device = TRUE;
			} else
				device = TRUE;
		}
		d  += len;
	}

	/* look for node-name in VMS-format */
	if (!node
	&&  (t = strchr(s, '!'))
	&&  (t[1] == '/' || t[1] == EOS)) {
		while (s < t)
			*d++ = *s++;
		*d++ = ':';
		*d++ = ':';
		s++;		/* skip over '!' */
	}

	/* look for device-name, indicated by a leading '/' */
	if (!device
	&&  (*s == '/')) {
		if ((t = strchr(++s, '/')) == 0)
			t = s + strlen(s);
		while (s < t)
			*d++ = *s++;
		*d++ = ':';
	}

	/* permit leading "./" to simplify cases in which we concatenate */
	if (!strncmp(s, "./", 2))
		s += 2;

	/* translate repeated leading "../" */
	while (!strncmp(s, "../", 3)) {
		s += 3;
		if (!bracket++)
			*d++ = '[';
		*d++ = '-';
	}

	if (strchr(s, '/')) {
		if (!bracket++)
			*d++ = '[';
		if (*s == '/')
			s++;
		else if (!on_top)
			*d++ = '.';
		on_top++;
		leaf_dot = PREFIX(s);
		while (c = *s++) {
			if (c == '/') {
		    		leaf_dot = PREFIX(s);
				if (strchr(s, '/'))
					*d++ = '.';
				else {
					break;
				}
			} else
				*d++ = translate(c);
		}
	}
	if (bracket)
		*d++ = ']';
	if (c != EOS && *s) {
		leaf_dot = PREFIX(s);
		while (c = *s++)
			*d++ = translate(c);
		if (!leaf_dot)
			*d++ = '.';
		*d++ = ';';
	}
	*d = EOS;
	return (dst);
}

#ifdef	TEST
do_test(argc, argv)
char	*argv[];
{
	register int j;
	char	buffer[MAXPATHLEN];

	if (argc <= 1) {
		static	char	*array[] = {
				"name",
				"name.type",
				"name.type.junk",
				"x/y",
				"x/y/z",
				"/x",
				"/x/y",
				"/x/y/",
				"/x.y",
				"x.y.z",
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
		do_test(sizeof(array)/sizeof(array[0]), array);
	} else {
		for (j = 1; j < argc; j++) {
			name2vms(buffer, argv[j]);
			printf("%d)\t\"%s\" => \"%s\"\n", j, argv[j], buffer);
		}
	}
}

main(argc, argv)
char	*argv[];
{
	do_test(argc, argv);
	exit(SUCCESS);
}
#endif	TEST