#ifndef	lint
static	char	Id[] = "$Id: edittree.c,v 7.0 1992/07/16 16:41:12 ste_cm Rel $";
#endif

/*
 * Title:	edittree.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
 * Modified:
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
static	editfile(n,f,s)	char *n; int (*f)(); STAT *s; { return 1;}
#else
#define	TELL_FILE(name)
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

edittree(
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
	auto	int		changes = 0;
	auto	int		next	= recur ? recur+1 : 0;
	auto	STAT		sb;
	auto	unsigned	num;
	auto	PTR		*vec;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];
	auto	char		*newpath;

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(recur*2)];
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
					if (!next)
						continue;
					changes += edittree(newname, func, next, links);
				} else if (isFILE(sb.st_mode)) {
					v_ALLOC(vec,num,newname);
				}
			}
			closedir(dirp);
			if (num != 0) {
				qsort((PTR)vec, (LEN_QSORT)num,
					sizeof(PTR), compare);
				while (num-- != 0) {
					if (LOOK(vec[num], &sb) < 0) {
						perror(vec[num]);
						continue;
					}
					TELL_FILE(vec[num]);
					changes += editfile(vec[num], func, &sb);
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
do_copy(ofp, ifp)
FILE	*ofp, *ifp;
{
	char	buffer[BUFSIZ];
	while (fgets(buffer, sizeof(buffer), ifp))
		fputs(buffer, ofp);
	return (1);
}

do_test(argc, argv)
char	*argv[];
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
		do_test(sizeof(tbl)/sizeof(tbl[0]), tbl);
	}
	exit(SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
