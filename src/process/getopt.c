#ifndef	NO_IDENT
static	char	Id[] = "$Id: getopt.c,v 12.1 1994/08/21 21:43:23 tom Exp $";
#endif

/*
 * Title:	getopt.c
 * Modified:
 *		04 Dec 1994, ifdefs.
 *		20 Nov 1992, use prototypes
 *
 * SYSTEM5-like 'getopt()'
 */

#define	STR_PTYPES
#include "ptypes.h"

#if !HAVE_GETOPT

#ifdef	unix
#define ERR(s, c)	if(opterr){\
	char	bfr[BUFSIZ]; register char *p = bfr;\
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

int	opterr = 1;
int	optind = 1;
int	optopt;
char	*optarg;

int	getopt(
	_ARX(int,	argc)
	_ARX(char **,	argv)
	_AR1(char *,	opts)
		)
	_DCL(int,	argc)
	_DCL(char **,	argv)
	_DCL(char *,	opts)
{
	static int sp = 1;
	register int c;
	register char *cp;

	if(sp == 1)
		if(optind >= argc ||
		   argv[optind][0] != '-' || argv[optind][1] == '\0')
			return(EOF);
		else if(strcmp(argv[optind], "--") == NULL) {
			optind++;
			return(EOF);
		}
	optopt = c = argv[optind][sp];
	if(c == ':' || (cp=strchr(opts, c)) == NULL) {
		ERR(": illegal option -- ", c);
		if(argv[optind][++sp] == '\0') {
			optind++;
			sp = 1;
		}
		return('?');
	}
	if(*++cp == ':') {
		if(argv[optind][sp+1] != '\0')
			optarg = &argv[optind++][sp+1];
		else if(++optind >= argc) {
			ERR(": option requires an argument -- ", c);
			sp = 1;
			return('?');
		} else
			optarg = argv[optind++];
		sp = 1;
	} else {
		if(argv[optind][++sp] == '\0') {
			sp = 1;
			optind++;
		}
		optarg = NULL;
	}
	return(c);
}
#endif	/* !HAVE_GETOPT */
