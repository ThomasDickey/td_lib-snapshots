#ifndef	lint
static	char	Id[] = "$Id: trnstree.c,v 5.2 1991/12/10 09:18:05 dickey Exp $";
#endif

/*
 * Title:	transtree.c
 * Author:	T.E.Dickey
 * Created:	12 Jan 1989
 * Modified:
 *		11 Dec 1991, added 'links' argument.  Process entire list of
 *			     files per-directory to avoid possible conflict
 *			     with temporary-files in current-directory.  Also,
 *			     pass-in 'sb' argument to 'func()' with name.
 *		18 Oct 1991, converted to ANSI
 *
 * Function:	Translates files in a tree.
 */

#define		DIR_PTYPES
#define		STR_PTYPES
#include	"portunix.h"
#include	<errno.h>

typedef	char	*PTR;
	/*ARGSUSED*/
	def_DOALLOC(PTR)

#define	CHUNK	127	/* 1 less than a power of 2 */
#define	v_ALLOC(v,n,s)	v = DOALLOC(v, PTR, ((++n)|CHUNK)+1);\
			v[n-1] = txtalloc(s)

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

#ifdef	S_IFLNK
#define	LOOK(name,sb)	(links ? stat(name,sb) : lstat(name,sb))
#else
#define	LOOK(name,sb)	(stat(name,sb))
#endif

/*
 * Comparison routine for qsort.
 */
static
compare(
_ARX(char **,	p1)
_AR1(char **,	p2)
	)
_DCL(char **,	p1)
_DCL(char **,	p2)
{
	return (-strcmp(*p1, *p2));
}

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*ARGSUSED*/
void
transtree(
_ARX(char *,	oldname)
_FNX(int,	func)
_ARX(int,	recur)
_AR1(int,	links)
	)
_DCL(char *,	oldname)
_DCL(int,	(*func)())
_DCL(int,	recur)
_DCL(int,	links)
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	struct stat	sb;
	auto	unsigned	num;
	auto	PTR		*vec;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];
	auto	char		*newpath;

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(recur*2)-1];
#endif

	if (LOOK(oldname, &sb) < 0) {
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
			num = 0;
			vec = 0;
			while (dp = readdir(dirp)) {
				(void)strcpy(newname, dp->d_name);
#ifndef	vms
				if (dotname(newname))	continue;
#endif
				if (LOOK(newname, &sb) < 0) {
					perror(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					if (recur)
						transtree(newname,
							func,
							recur+1,
							links);
				} else if (isFILE(sb.st_mode)) {
					v_ALLOC(vec,num,newname);
				}
			}
			closedir(dirp);
			if (num != 0) {
				qsort((PTR)vec, (LEN_QSORT)num,
					sizeof(PTR), compare);
				while (num-- != 0) {
					TELL_FILE(vec[num]);
					(*func)(vec[num], &sb);
				}
				dofree((PTR)vec);
			}
		}
		(void)chdir(oldpath);
	} else if (isFILE(sb.st_mode)) {
		TELL_FILE(oldname);
		(*func)(oldname, &sb);
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
	auto	 int	links = FALSE;

	for (j = 1; j < argc; j++) {
		if (*(s = argv[j]) == '-') {
			if (*(++s) == 'r')
				recur = TRUE;
			if (*s == 'L')
				links = TRUE;
		} else
			transtree(s, do_file, recur, links);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
