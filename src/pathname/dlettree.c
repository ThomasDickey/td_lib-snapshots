#ifndef	lint
static	char	Id[] = "$Id: dlettree.c,v 3.0 1989/09/15 08:39:09 ste_cm Rel $";
#endif	lint

/*
 * Title:	deletetree.c
 * Author:	T.E.Dickey
 * Created:	17 Nov 1988
 * Function:	Deletes a tree of files a la "rm -rf".
 *
 *		On VMS, we use this also to hide the fact that DCL does not
 *		do wildcard expansion.  Hence, the "recur" option may be
 *		false simply to do in-directory scanning.
 *
 *		Note that this assumes that we do directory-recursion in a
 *		unix-style, rather than the "..." wildcard that VMS provides.
 */

#define		DIR_PTYPES
#define		STR_PTYPES
#include	"portunix.h"
#include	<errno.h>

#define	TELL		fprintf(stderr,

#ifdef	TEST
#define	fail		perror
#define	TELL_(s)	changes,nesting,s
#define	TELL_FILE(name)	TELL "%d\t%s => %s\n", TELL_(name))
#define	TELL_DIR(name)	TELL "%d\t%s (directory) %s\n", TELL_(name));
#define	TELL_SCAN(name)	TELL "%d\t%s scan directory %s\n", TELL_(name))
static	deletedir()	{ return 1;}
static	deletefile()	{ return 1;}
#else	TEST
#define	TELL_FILE(name)
#define	TELL_SCAN(name)
#define	TELL_DIR(name)
#endif	TEST


deletetree(oldname,recur)
char	*oldname;
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	int		changes = 0;
	auto	struct stat	sb;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(recur*2)-1];
#endif	TEST

	if (stat(oldname, &sb) < 0) {
		errno = ENOENT;		/* bypass vms-bug */
		fail(oldname);
		return(0);
	}

	if (_OPENDIR(oldname,sb.st_mode)) {
		TELL_SCAN(oldname);
		if (getwd(oldpath) == 0) {
			fail("(getwd)");
			return(0);
		}
		if (chdir(DIR2PATH(oldname)) < 0) {
			fail(oldname);
			return(0);
		}

		if (dirp = opendir(OPENDIR_ARG)) {
			while (dp = readdir(dirp)) {
				(void)strcpy(newname, dp->d_name);
#ifndef	vms
				if (dotname(newname))	continue;
#endif	vms
				if (lstat(newname, &sb) < 0) {
					fail(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					if (!recur)
						continue;
					changes += deletetree(newname, recur+1);
					changes += deletedir(newname);
					TELL_DIR(newname);
				} else {	/* file, link, etc. */
					TELL_FILE(newname);
					changes += deletefile(newname);
				}
			}
			closedir(dirp);
		}
		(void)chdir(oldpath);
	} else {
		TELL_FILE(oldname);
		changes += deletefile(oldname);
	}
	return (changes);
}

#ifdef	TEST
do_test(argc, argv)
char	*argv[];
{
	register int	j;
	auto	 int	recur = FALSE;

	for (j = 1; j < argc; j++) {
		if (!strcmp(argv[j], "-r"))
			recur = TRUE;
		else
			printf("count = %d\n", deletetree(argv[j],recur));
	}
}

main(argc, argv)
char	*argv[];
{
	if (argc > 1)
		do_test(argc, argv);
	else {
		static	char	*tbl[] = {
					"?",
#ifdef	vms
					"[-]", "-r", "[-]"
#else	unix
					"..",  "-r", ".."
#endif	vms/unix
					};
		do_test(sizeof(tbl)/sizeof(tbl[0]), tbl);
	}
	exit(SUCCESS);
}
#endif	TEST
