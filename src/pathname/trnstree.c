#ifndef	lint
static	char	Id[] = "$Id: trnstree.c,v 5.1 1991/10/18 15:25:14 dickey Exp $";
#endif

/*
 * Title:	transtree.c
 * Author:	T.E.Dickey
 * Created:	12 Jan 1989
 * Modified:
 *		18 Oct 1991, converted to ANSI
 *
 * Function:	Translates files in a tree.
 */

#define		DIR_PTYPES
#define		STR_PTYPES
#include	"portunix.h"
#include	<errno.h>

#define	TELL		fprintf(stderr,

#ifdef	TEST
#define	TELL_(s)	nesting,s
#define	TELL_FILE(name)	TELL "%s => %s\n", TELL_(name))
#define	TELL_DIR(name)	TELL "%s (directory) %s\n", TELL_(name));
#define	TELL_SCAN(name)	TELL "%s scan directory %s\n", TELL_(name))
#else
#define	TELL_FILE(name)
#define	TELL_SCAN(name)
#define	TELL_DIR(name)
#endif


void
transtree(
_ARX(char *,	oldname)
_FNX(int,	func)
_AR1(int,	recur)
	)
_DCL(char *,	oldname)
_DCL(int,	(*func)())
_DCL(int,	recur)
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	struct stat	sb;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];
	auto	char		*newpath;

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(recur*2)-1];
#endif

	if (stat(oldname, &sb) < 0) {
		errno = ENOENT;		/* bypass vms-bug */
		perror(oldname);
	}

	if (_OPENDIR(oldname,sb.st_mode)) {
		TELL_SCAN(oldname);
		if (getwd(oldpath) == 0) {
			perror("(getwd)");
			return;
		}
		newpath = OPENDIR_ARG;
#ifdef	vms
		if (vms_iswild(oldname))
			newpath = oldname;
		else
#endif
		if (chdir(DIR2PATH(oldname)) < 0) {
			perror(oldname);
			return;
		}
		TELL_DIR(DIR2PATH(oldname));

		if (dirp = opendir(newpath)) {
			while (dp = readdir(dirp)) {
				(void)strcpy(newname, dp->d_name);
#ifndef	vms
				if (dotname(newname))	continue;
#endif
				if (lstat(newname, &sb) < 0) {
					perror(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					if (recur)
						transtree(newname,func,recur+1);
				} else if (isFILE(sb.st_mode)) {
					TELL_FILE(newname);
					(*func)(newname);
				}
			}
			closedir(dirp);
		}
		(void)chdir(oldpath);
	} else if (isFILE(sb.st_mode)) {
		TELL_FILE(oldname);
		(*func)(oldname);
	}
}

#ifdef	TEST
static
do_file(
_AR1(char *,	name))
_DCL(char *,	name)
{
}

/*ARGSUSED*/
_MAIN
{
	register int	j;
	auto	 char	*s;
	auto	 int	recur = FALSE;

	for (j = 1; j < argc; j++) {
		if (*(s = argv[j]) == '-') {
			if (*(++s) == 'r')
				recur = TRUE;
		} else
			transtree(s,do_file,recur);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
