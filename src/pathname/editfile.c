/*
 * Title:	editfile.c
 * Author:	T.E.Dickey
 * Created:	03 Oct 1988
 * Modified:
 *		21 Aug 1994, argument of mktemp is volatile on Linux.
 *		01 Dec 1993, ifdefs, TurboC warnings.
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, added 3rd arg to _FNX macros.
 *		13 Dec 1991, pass 'sb' arg down to 'func', from initial stat on
 *			     the input-file.
 *		24 Apr 1989, port to unix using 'mktemp()'
 *
 * Function:	Invokes a single-line editing function on a file.  If no changes
 *		are made to the file, we discard the new file and keep the old.
 *
 *		On VMS we can directly create a new version of a file without
 *		having to delete the old one first.  On unix we must copy to
 *		another name first.
 */

#define		STR_PTYPES
#include	"ptypes.h"

MODULE_ID("$Id: editfile.c,v 12.4 2001/05/15 00:59:32 tom Exp $")

#ifdef	vms
#define	NEWVER(name)	(name)
#else	/* SYS_UNIX */
#define	NEWVER(name)	mktemp(name)
#endif	/* vms/SYS_UNIX */

int	editfile(
	_ARX(char *,	oldname)
	_FNX(int,	func,	(_ARX(FILE*,o) _ARX(FILE*,i) _AR1(Stat_t*,s)))
	_AR1(Stat_t *,	sb)
		)
	_DCL(char *,	oldname)
	_DCL(int,	(*func)())
	_DCL(Stat_t *,	sb)
{
	auto	FILE	*ifp = fopen(oldname, "r");
	auto	FILE	*ofp;
	auto	int	changes = 0;
#ifdef	vms
	auto	char	newname[MAXPATHLEN];
	auto	char	*s;
	(void)strcpy(newname, oldname);
	if (s = strrchr(newname, ';'))	s[1] = '\0';
	else				(void)strcat(newname, ";");
#else	/* SYS_UNIX */
	char	newname[MAXPATHLEN];
	(void)strcpy(newname, "fileXXXXXX");
#endif	/* vms/SYS_UNIX */

	if ((ifp != 0)
	&&  (ofp = fopen(NEWVER(newname), "w")) != 0) {
		FPRINTF(stderr, "** edit \"%s\" => \"%s\"\n", oldname, newname);
		changes += (*func)(ofp, ifp, sb);
		(void)fclose(ifp);
		(void)fclose(ofp);
		if (changes == 0) {
			FPRINTF(stderr, "** no change made\n");
			(void)unlink(newname);
		}
#ifndef	vms
		else {
			if (rename(newname, oldname) >= 0) {
				(void)unlink(newname);
			} else {
				perror(newname);
				return(0);
			}
		}
#endif
		if (changes > 0) {
			FPRINTF(stderr, "** %d change(s) made\n", changes);
		}
	} else {
		perror(newname);
	}
	return (changes);
}

#ifdef	TEST
static
int	do_copy(
	_ARX(FILE *,	ofp)
	_ARX(FILE *,	ifp)
	_AR1(Stat_t *,	sb)
		)
	_DCL(FILE *,	ofp)
	_DCL(FILE *,	ifp)
	_DCL(Stat_t *,	sb)
{
	char	buffer[BUFSIZ];
	while (fgets(buffer, sizeof(buffer), ifp))
		if (fputs(buffer, ofp) < 0) {
			perror("do_copy");
			return (0);
		}
	fputs("LAST\n", ofp);
	return (1);
}

_MAIN
{
	register int	j;

	for (j = 1; j < argc; j++)
		editfile(argv[j], do_copy, (Stat_t *)0);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
