#ifndef	lint
static	char	Id[] = "$Id: editfile.c,v 8.0 1992/11/20 10:59:49 ste_cm Rel $";
#endif

/*
 * Title:	editfile.c
 * Author:	T.E.Dickey
 * Created:	03 Oct 1988
 * Modified:
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
#include	"portunix.h"

#define	TELL	FPRINTF(stderr,

#ifdef	vms
#define	NEWVER(name)	(name)
#else	/* unix */
extern	char	*mktemp();
#define	NEWVER(name)	mktemp(name)
#endif	/* vms/unix */

int	editfile(
	_ARX(char *,	oldname)
	_FNX(int,	func,	(_ARX(FILE*,o) _ARX(FILE*,i) _AR1(STAT*,s)))
	_AR1(STAT *,	sb)
		)
	_DCL(char *,	oldname)
	_DCL(int,	(*func)())
	_DCL(STAT *,	sb)
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
#else	/* unix */
	static	char	newname[] = "fileXXXXXX";
#endif	/* vms/unix */

	if ((ifp != 0)
	&&  (ofp = fopen(NEWVER(newname), "w")) ) {
		TELL "** edit \"%s\" => \"%s\"\n", oldname, newname);
		changes += (*func)(ofp, ifp, sb);
		(void)fclose(ifp);
		(void)fclose(ofp);
		if (changes == 0) {
			TELL "** no change made\n");
			(void)unlink(newname);
		}
#ifndef	vms
		else {
			if (rename(newname, oldname) >= 0)
				(void)unlink(newname);
			else {
				perror(newname);
				return(0);
			}
		}
#endif
		if (changes > 0) {
			TELL "** %d change(s) made\n", changes);
		}
	}
	return (changes);
}

#ifdef	TEST
static
int	do_copy(
	_ARX(FILE *,	ofp)
	_ARX(FILE *,	ifp)
	_AR1(STAT *,	sb)
		)
	_DCL(FILE *,	ofp)
	_DCL(FILE *,	ifp)
	_DCL(STAT *,	sb)
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
		editfile(argv[j], do_copy, (STAT *)0);
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
