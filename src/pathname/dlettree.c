#ifndef	lint
static	char	Id[] = "$Id: dlettree.c,v 8.1 1993/09/22 17:55:30 dickey Exp $";
#endif

/*
 * Title:	deletetree.c
 * Author:	T.E.Dickey
 * Created:	17 Nov 1988
 * Modified:
 *		22 Sep 1993, gcc warnings
 *		20 Nov 1992, use prototypes
 *
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

#define	TELL		FPRINTF(stderr,

#ifdef	TEST
#define	fail		perror
#define	TELL_(s)	changes,nesting,s
#define	TELL_FILE(name)	TELL "%d\t%s => %s\n", TELL_(name))
#define	TELL_DIR(name)	TELL "%d\t%s (directory) %s\n", TELL_(name));
#define	TELL_SCAN(name)	TELL "%d\t%s scan directory %s\n", TELL_(name))
static	int	deletedir   (_AR1(char *,s))	_DCL(char *,s) { return 1;}
static	int	deletefile  (_AR1(char *,s))	_DCL(char *,s) { return 1;}
#else
#define	TELL_FILE(name)
#define	TELL_SCAN(name)
#define	TELL_DIR(name)
#endif


int	deletetree(
	_ARX(char *,	oldname)
	_AR1(int,	recur)
		)
	_DCL(char *,	oldname)
	_DCL(int,	recur)
{
	auto	DIR	*dirp;
	auto	DIRENT	*dp;
	auto	int	changes = 0;
	auto	STAT	sb;
	auto	char	newname[MAXPATHLEN];
	auto	char	oldpath[MAXPATHLEN];
	auto	char	*newpath;
	auto	int	old_mode;
	auto	int	ok	= TRUE;

#ifdef	TEST
	static	char	stack[]	= ". . . . . . . ";
	auto	char	*nesting = &stack[sizeof(stack)-(recur*2)-1];
#endif

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
		newpath = OPENDIR_ARG;
#ifdef	vms
		if (vms_iswild(oldname))
			newpath = oldname;
		else
#endif
		if (chdir(DIR2PATH(oldname)) < 0) {
			fail(oldname);
			return(0);
		}
		old_mode = sb.st_mode;	/* save, so we know to delete-dir */

		if ((dirp = opendir(newpath)) != NULL) {
			while ((dp = readdir(dirp)) != NULL) {
				(void)strcpy(newname, dp->d_name);
#ifndef	vms
				if (dotname(newname))	continue;
#endif
				if (lstat(newname, &sb) < 0) {
					fail(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					auto	int	k;
					if (!recur)
						continue;
					if ((k = deletetree(newname,recur+1))
							>= 0)
						changes += k;
					else
						ok = FALSE;
				} else {	/* file, link, etc. */
					TELL_FILE(newname);
					if (deletefile(newname))
						changes++;
					else
						ok = FALSE;
				}
			}
			closedir(dirp);
		}
		(void)chdir(oldpath);
		if (ok && isDIR(old_mode)) {
			changes += deletedir(oldname);
			TELL_DIR(DIR2PATH(oldname));
		}
	} else {
		TELL_FILE(oldname);
		if (deletefile(oldname))
			changes++;
		else
			ok = FALSE;
	}
	return (ok ? changes : -1);
}

#ifdef	TEST
static
void	do_test(
	_ARX(int,	argc)
	_AR1(char **,	argv)
		)
	_DCL(int,	argc)
	_DCL(char **,	argv)
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

/*ARGSUSED*/
_MAIN
{
	if (argc > 1)
		do_test(argc, argv);
	else {
		static	char	*tbl[] = {
					"?",
#ifdef	vms
					"[-]", "-r", "[-]"
#else	/* unix */
					"..",  "-r", ".."
#endif	/* vms/unix */
					};
		do_test(SIZEOF(tbl), tbl);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
