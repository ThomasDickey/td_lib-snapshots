#ifndef	lint
static	char	Id[] = "$Id: walktree.c,v 10.0 1991/10/17 07:53:58 ste_cm Rel $";
#endif

/*
 * Title:	walktree.c (link/directory tree)
 * Author:	T.E.Dickey
 * Created:	31 Aug 1988
 * Modified:
 *		03 Oct 1991, converted to ANSI
 *		
 *		Revision 8.1  91/05/15  09:58:45  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *		
 *		Revision 4.1  89/09/06  15:20:27  dickey
 *		use 'getwd()' definition from "ptypes.h"
 *		
 * Function:	Given a pathname, this successively invokes a user-supplied
 *		function, with the following arguments:
 *
 *			path	- the current working directory.
 *			name	- the leafname of the individual file (the
 *				  current working directory is set accordingly).
 *			sp	- pointer to a 'stat' structure if 'stat()'
 *				  succeeded on the file.
 *			ok_acc	- set to 0 if user-specified access is permitted
 *				  on the file, -1 if not.
 *			level	- the current directory-nesting level
 *
 *		The user-function may suppress further nesting by returning
 *		a "-1".
 *
 *		Names are passed to the user-function in alphabetically-sorted
 *		order, on conclusion of the scan for each directory.
 *
 * Arguments:	path	- the current working directory (ignored if level=0)
 *		name	- the initial pathname to scan.
 *		func	- the user-supplied function
 *		type	- access, specified in character-string form (r,w,x).
 *			  Directories are checked for access even if no access
 *			  is specified.
 *		level	- the current nesting level, which is incremented on
 *			  entry to each directory.  This is made an argument
 *			  so that 'walktree()' need have no local state.
 *
 * Returns:	The number of items in the tree which both succeed with 'stat()'
 *		and with the user-specified access.
 */

#define		ACC_PTYPES
#define		DIR_PTYPES		/* include directory-stuff */
#define		STR_PTYPES
#include	"ptypes.h"

/************************************************************************
 *	local definitions						*
 ************************************************************************/

typedef	char	*PTR;
	/*ARGSUSED*/
	def_DOALLOC(PTR)

#define	CHUNK	127	/* 1 less than a power of 2 */
#define	v_ALLOC(v,n,s)	v = DOALLOC(v, PTR, ((++n)|CHUNK)+1);\
			v[n-1] = txtalloc(s)

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

/*
 * Given a name in the current directory, see what it is.  If it is a directory,
 * nest to a new level.
 */
walktree(path, name, func, type, level)
char	*path;
char	*name;
int	(*func)();
char	*type;
int	level;
{
	int	total	= 0,
		ok_acc	= -1,
		mode	= 0;
	struct	stat	sb,
			*sb_	= &sb;
	register unsigned num;
	PTR		*vec;
	char		old_wd[BUFSIZ];
	char		new_wd[BUFSIZ];
	DIR		*dp;
	struct	direct	*de;

	if (stat(name, sb_) >= 0) {
		mode = (sb.st_mode & S_IFMT);
		ok_acc = 0;
		if (mode == S_IFDIR)
			ok_acc |= (R_OK | X_OK);
		if (type) {
			if (strchr(type, 'r'))	ok_acc |= R_OK;
			if (strchr(type, 'w'))	ok_acc |= W_OK;
			if (strchr(type, 'x'))	ok_acc |= X_OK;
		}
		if (ok_acc != 0) {
			if ((ok_acc = access(name, ok_acc)) >= 0)
				total++;
		} else
			total++;
	} else
		sb_ = 0;

	if (level == 0)		/* do this once, to initialize */
		path = getwd(old_wd);
	else			/* ...and inherit otherwise */
		(void)strcpy(old_wd, path);

	if (((*func)(path, name, sb_, ok_acc, level) >= 0)
	&&  (ok_acc >= 0)
	&&  (mode == S_IFDIR)) {
		if ((chdir(name) >= 0)
		&&  (dp = opendir("."))) {
			abspath(strcpy(new_wd, "."));
			num = 0;
			vec = 0;
			while (de = readdir(dp)) {
				if (dotname(de->d_name))	continue;
				v_ALLOC(vec,num,de->d_name);
			}
			(void)closedir(dp);
			if (num != 0) {
				qsort((PTR)vec, (LEN_QSORT)num,
					sizeof(PTR), compare);
				while (num-- != 0)
					total += walktree(new_wd, vec[num],
						func, type, level+1);
				dofree((PTR)vec);
			}
		}
		(void)chdir(old_wd);
	}
	return (total);
}

/************************************************************************
 *	test driver							*
 ************************************************************************/

#ifdef	TEST
static	int	lines;

static
display(path, name, sp, ok_acc, level)
char	*path;
char	*name;
struct	stat	*sp;
{
	++lines;
	PRINTF("%4d:%c\t", lines, (ok_acc < 0) ? '?' : ' ');
	while (level-- > 0)
		PRINTF("|--%c", (level > 0) ? '-' : ' ');
	if ((sp != 0)
	&&  ((sp->st_mode & S_IFMT) == S_IFDIR))
		PRINTF("%s%c\n", name, name[strlen(name)-1] == '/' ? ' ' : '/');
	else
		PRINTF("%s\n", name);
	return(ok_acc);
}

static
do_arg(name,type)
char	*name;
char	*type;
{
	PRINTF("** path = %s\n", name);
	lines	= 0;
	PRINTF("** total= %d\n", walktree((PTR)0, name,display,type,0));
}

/*ARGSUSED*/
_MAIN
{
	register int j;
	char	*opts = "wrx";
	char	need[BUFSIZ];
	int	len = 0;

	while ((j = getopt(argc, argv, opts)) != EOF)
		switch (j) {
		case 'w':
		case 'r':
		case 'x':
			need[len++] = j;
			break;
		default:
			FPRINTF(stderr, "usage: walktree [-%s] [dir [...]]\n",
				opts);
			(void)exit(FAIL);
		}

	need[len] = EOS;
	PRINTF("** mode = %s\n", need);
	if (optind < argc) {
		for (j = optind; j < argc; j++)
			do_arg(argv[j],need);
	} else
		do_arg(".",need);

	(void)exit(SUCCESS);
	/*NOTREACHED*/
}
#endif
