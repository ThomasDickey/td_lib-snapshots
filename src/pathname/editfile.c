#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/editfile.c,v 1.1 1988/10/05 16:24:02 dickey Exp $";
#endif	lint

/*
 * Title:	editfile.c
 * Author:	T.E.Dickey
 * Created:	03 Oct 1988
 * Function:	Invokes a single-line editing function on a file.  If no changes
 *		are made to the file, we discard the new file and keep the old.
 *
 *		On VMS we can directly create a new version of a file without
 *		having to delete the old one first.  On unix we must copy to
 *		another name first.
 */
#include	"portunix.h"
#include	<stdio.h>
#include	<string.h>

#define	TELL	fprintf(stderr,

editfile(oldname,func)
char	*oldname;
int	(*func)();
{
	auto	FILE	*ifp = fopen(oldname, "r");
	auto	FILE	*ofp;
	auto	int	changes = 0;
	auto	char	newname[MAXPATHLEN];
#ifdef	vms
	auto	char	*s;
	(void)strcpy(newname, oldname);
	if (s = strrchr(newname, ';'))	s[1] = '\0';
	else				(void)strcat(newname, ";");
#else
	(void)strcat(newname, "$");	/* fake a unique name */
#endif	vms

	if ((ifp != 0)
	&&  (ofp = fopen(newname, "w"))) {
		TELL "** edit \"%s\" => \"%s\"\n", oldname, newname);
		changes += (*func)(ofp, ifp);
		(void)fclose(ifp);
		(void)fclose(ofp);
		if (changes == 0) {
			TELL "** no change made\n");
			(void)unlink(newname);
		}
#ifndef	vms
		else {
			if (link(newname, oldname) >= 0)
				(void)unlink(newname);
		}
#endif	vms
		if (changes > 0) {
			TELL "** %d change(s) made\n", changes);
		}
	}
	return (changes);
}

#ifdef	TEST
static
do_copy(ofp, ifp)
FILE	*ofp, *ifp;
{
	char	buffer[BUFSIZ];
	while (fgets(buffer, sizeof(buffer), ifp))
		fputs(buffer, ofp);
	return (1);
}

main(argc, argv)
char	*argv[];
{
	register int	j;

	for (j = 1; j < argc; j++)
		editfile(argv[j], do_copy);
	exit(SUCCESS);
}
#endif	TEST
