#ifndef	lint
static	char	Id[] = "$Id: trnstree.c,v 5.0 1991/05/20 17:17:36 ste_cm Rel $";
#endif

/*
 * Title:	transtree.c
 * Author:	T.E.Dickey
 * Created:	12 Jan 1989
 * Modified:
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


transtree(oldname,func,recur)
char	*oldname;
int	(*func)();
int	recur;
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
do_file(name)
char	*name;
{
}

main(argc, argv)
char	*argv[];
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
}
#endif
