/*
 * Title:	getopt.c
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		04 Dec 1994, ifdefs.
 *		20 Nov 1992, use prototypes
 *
 * SYSTEM5-like 'getopt()'
 */

#define	STR_PTYPES
#include "ptypes.h"

MODULE_ID("$Id: getopt.c,v 12.7 2014/12/28 01:11:01 tom Exp $")

#if !defined(HAVE_GETOPT)

#ifdef	SYS_UNIX
#define ERR(s, c)	if(opterr){\
	char	bfr[BUFSIZ]; char *p = bfr;\
	p += strlen(strcat(strcpy(bfr,argv[0]), s));\
	*p++ = c; *p++ = '\n';\
	(void)write(2,bfr,(unsigned)(p-bfr));}
#else
#define ERR(s, c)	if(opterr){\
	(void) fputs(argv[0], stderr);\
	(void) fputs(s, stderr);\
	(void) fputc(c, stderr);\
	(void) fputc('\n', stderr);}
#endif

int opterr = 1;
int optind = 1;
int optopt;
char *optarg;

int
getopt(int argc,
       char *const *argv,
       const char *opts)
{
    static int sp = 1;
    int c;
    char *cp;

    if (sp == 1)
	if (optind >= argc ||
	    argv[optind][0] != '-' || argv[optind][1] == '\0')
	    return (EOF);
	else if (strcmp(argv[optind], "--") == NULL) {
	    optind++;
	    return (EOF);
	}
    optopt = c = argv[optind][sp];
    if (c == ':' || (cp = strchr(opts, c)) == NULL) {
	ERR(": illegal option -- ", c);
	if (argv[optind][++sp] == '\0') {
	    optind++;
	    sp = 1;
	}
	return ('?');
    }
    if (*++cp == ':') {
	if (argv[optind][sp + 1] != '\0')
	    optarg = &argv[optind++][sp + 1];
	else if (++optind >= argc) {
	    ERR(": option requires an argument -- ", c);
	    sp = 1;
	    return ('?');
	} else
	    optarg = argv[optind++];
	sp = 1;
    } else {
	if (argv[optind][++sp] == '\0') {
	    sp = 1;
	    optind++;
	}
	optarg = NULL;
    }
    return (c);
}
#endif /* !HAVE_GETOPT */

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    (void) argc;
    (void) argv;
    exit(EXIT_FAILURE);
    /*NOTREACHED */
}
#endif /* TEST */
