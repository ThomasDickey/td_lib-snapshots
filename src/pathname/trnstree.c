#ifndef	NO_IDENT
static	char	Id[] = "$Id: trnstree.c,v 12.1 1994/08/21 19:36:36 tom Exp $";
#endif

/*
 * Title:	transtree.c
 * Author:	T.E.Dickey
 * Created:	12 Jan 1989
 * Modified:
 *		01 Dec 1993, ifdefs.
 *		22 Sep 1993, gcc warnings
 *		29 Apr 1993, sort _all_ leaves before translating
 *		20 Nov 1992, added 3rd arg to _FNX macros.
 *		11 Dec 1991, added 'links' argument.  Process entire list of
 *			     files per-directory to avoid possible conflict
 *			     with temporary-files in current-directory.  Also,
 *			     pass-in 'sb' argument to 'func()' with name.
 *		18 Oct 1991, converted to ANSI
 *
 * Function:	Translates files in a tree.
 */

#define		DIR_PTYPES
#define		ERR_PTYPES
#define		STR_PTYPES
#include	"port2vms.h"
#include	"td_qsort.h"

typedef	char	*PTR;
	/*ARGSUSED*/
	def_DOALLOC(PTR)

#define	CHUNK	127	/* 1 less than a power of 2 */
#define	v_ALLOC(v,n,s)	v = DOALLOC(v, PTR, ((++n)|CHUNK)+1);\
			v[n-1] = txtalloc(s)

#ifdef	TEST
#define	TELL_(s)	nesting,s
#define	TELL_FILE(name)	FPRINTF(stderr, "%s => %s\n", TELL_(name))
#define	TELL_DIR(name)	FPRINTF(stderr, "%s (directory) %s\n", TELL_(name));
#define	TELL_SCAN(name)	FPRINTF(stderr, "%s scan directory %s\n", TELL_(name))
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

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*ARGSUSED*/
void	transtree(
	_ARX(char *,	oldname)
	_FNX(int,	func,	(_ARX(char *,name) _AR1(Stat_t *,s)))
	_ARX(int,	recur)
	_AR1(int,	links)
		)
	_DCL(char *,	oldname)
	_DCL(int,	(*func)())
	_DCL(int,	recur)
	_DCL(int,	links)
{
	auto	DIR	*dirp;
	auto	DirentT	*dp;
	auto	Stat_t	sb;
	auto	unsigned num;
	auto	PTR	*vec;
	auto	char	newname[MAXPATHLEN];
	auto	char	oldpath[MAXPATHLEN];
	auto	char	*newpath;

#ifdef	TEST
	static	char	stack[]	= ". . . . . . . ";
	auto	char	*nesting = &stack[sizeof(stack)-(recur*2)-1];
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

		if ((dirp = opendir(newpath)) != NULL) {
			num = 0;
			vec = 0;
			while ((dp = readdir(dirp)) != NULL) {
				(void)strcpy(newname, dp->d_name);
#ifndef	vms
				if (dotname(newname))	continue;
#endif
				v_ALLOC(vec,num,newname);
			}
			closedir(dirp);
			if (num != 0) {
				qsort((PTR)vec, (LEN_QSORT)num,
					sizeof(PTR), cmp_qsort);
				while (num-- != 0) {
					if (LOOK(vec[num], &sb) < 0) {
						perror(vec[num]);
						continue;
					}
					if (isDIR(sb.st_mode)) {
						if (recur)
							transtree(vec[num],
								func,
								recur+1,
								links);
					} else if (isFILE(sb.st_mode)) {
						TELL_FILE(vec[num]);
						(*func)(vec[num], &sb);
					}
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
int	do_file(
	_ARX(char *,	name)
	_AR1(Stat_t *,	sb)
		)
	_DCL(char *,	name)
	_DCL(Stat_t *,	sb)
{
	return 1;
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
