#ifndef	NO_IDENT
static	char	Id[] = "$Id: edittree.c,v 8.2 1993/12/01 19:38:36 tom Exp $";
#endif

/*
 * Title:	edittree.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * Modified:
 *		01 Dec 1993, ifdefs.
 *		22 Sep 1993, gcc warnings
 *		29 Apr 1993, sort _all_ leaves before doing translation.
 *		20 Nov 1992, added 3rd arg to _FNX macros.
 *		11 Dec 1991, added 'links' argument.  Process entire list of
 *			     files per-directory to avoid possible conflict
 *			     with temporary-files in current-directory.  Also,
 *			     pass-in 'sb' argument to 'func()' with name.
 *
 * Function:	Invokes a single-line editing function on a tree of files.
 *
 *		On VMS, we use this also to hide the fact that DCL does not
 *		do wildcard expansion.  Hence, the "recur" option may be
 *		false simply to do in-directory scanning.
 *
 *		Note that this assumes that we do directory-recursion in a
 *		unix-style, rather than the "..." wildcard that VMS provides.
 */
#define	DIR_PTYPES
#define	STR_PTYPES
#include	"portunix.h"
#include	"td_qsort.h"
#include	<errno.h>

typedef	char	*PTR;
	/*ARGSUSED*/
	def_DOALLOC(PTR)

#define	CHUNK	127	/* 1 less than a power of 2 */
#define	v_ALLOC(v,n,s)	v = DOALLOC(v, PTR, ((++n)|CHUNK)+1);\
			v[n-1] = txtalloc(s)

#define	TELL		FPRINTF(stderr,

#ifdef	TEST
#define	TELL_FILE(name)	TELL "%d\t%s => %s\n", changes, nesting, name);
#define	TELL_DIR(name)	TELL "%d\t%s directory %s\n", changes, nesting, name); 
static	int	editfile(
		_ARX(char *,	n)
		_FNX(int,	func,	(_ARX(FILE*,o) _ARX(FILE*,i) _AR1(STAT*,s)))
		_AR1(STAT *,	s)
			)
		_DCL(char *,	n)
		_DCL(int,	(*f)())
		_DCL(STAT *,	s)
		{ return 1;}
#else
#define	TELL_FILE(name)
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

int	edittree(
	_ARX(char *,	oldname)
	_FNX(int,	func,	(_ARX(FILE *,o) _ARX(FILE *,i) _AR1(STAT *,s)))
	_ARX(int,	recur)
	_AR1(int,	links)
		)
	_DCL(char *,	oldname)
	_DCL(int,	(*func)())
	_DCL(int,	recur)
	_DCL(int,	links)
{
	auto	DIR	*dirp;
	auto	DIRENT	*dp;
	auto	int	changes = 0;
	auto	int	next	= recur ? recur+1 : 0;
	auto	STAT	sb;
	auto	unsigned num;
	auto	PTR	*vec;
	auto	char	newname[MAXPATHLEN];
	auto	char	oldpath[MAXPATHLEN];
	auto	char	*newpath;

#ifdef	TEST
	static	char	stack[]	= ". . . . . . . ";
	auto	char	*nesting = &stack[sizeof(stack)-(recur*2)];
#endif

	if (LOOK(oldname, &sb) < 0) {
		errno = ENOENT;		/* bypass vms-bug */
		perror(oldname);
		return(0);
	}

	if (_OPENDIR(oldname,sb.st_mode)) {
		TELL_DIR(oldname);

		if (getwd(oldpath) == 0) {
			perror("(getwd)");
			return(0);
		}
		newpath = EDITDIR_ARG;
#ifdef	vms
		if (vms_iswild(oldname))
			newpath = oldname;
		else
#endif
		if (chdir(DIR2PATH(oldname)) < 0) {
			perror(oldname);
			return(0);
		}

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
						if (next)
							changes += edittree(vec[num], func, next, links);
					} else if (isFILE(sb.st_mode)) {
						TELL_FILE(vec[num]);
						changes += editfile(vec[num], func, &sb);
					}
				}
				dofree((PTR)vec);
			}
		}
		(void)chdir(oldpath);
	} else {
		TELL_FILE(oldname);
		changes += editfile(oldname, func, &sb);
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
		fputs(buffer, ofp);
	return (1);
}

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
	auto	 int	links = FALSE;

	for (j = 1; j < argc; j++) {
		if (!strcmp(argv[j], "-r"))
			recur = TRUE;
		else if (!strcmp(argv[j], "-L"))
			links = TRUE;
		else
			PRINTF("count = %d\n", edittree(argv[j],do_copy,recur, links));
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
#endif	/* TEST */
