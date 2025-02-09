/*
 * Title:	walktree.c (link/directory tree)
 * Author:	T.E.Dickey
 * Created:	31 Aug 1988
 * Modified:
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		17 Nov 1992, modified _FNX macro.
 *		05 Feb 1992, missed prototype on main-function
 *		03 Oct 1991, converted to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		06 Sep 1989, use 'getwd()' definition from "ptypes.h"
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
#define		DIR_PTYPES	/* include directory-stuff */
#define		STR_PTYPES
#include	"ptypes.h"
#include	"td_qsort.h"

MODULE_ID("$Id: walktree.c,v 12.19 2025/01/07 00:31:55 tom Exp $")

/************************************************************************
 *	local definitions						*
 ************************************************************************/

typedef char *PTR;
#define	CHUNK	127		/* 1 less than a power of 2 */
#define	v_ALLOC(v,n,s)	v = DOALLOC(v, PTR, ((++n)|CHUNK)+1);\
			v[n-1] = txtalloc(s)

/************************************************************************
 *	public entrypoints						*
 ************************************************************************/

/*
 * Given a name in the current directory, see what it is.  If it is a directory,
 * nest to a new level.
 */
int
walktree(const char *patharg,
	 const char *namearg,
	 int (*func) (WALK_FUNC_ARGS),
	 const char *type,
	 int level)
{
    int total = 0;
    int ok_acc = -1;
    int mode = 0;
    Stat_t sb, *sb_ = &sb;
    unsigned num;
    PTR *vec;
    char old_wd[MAXPATHLEN];
    char new_wd[MAXPATHLEN];
    DIR *dp;
    DirentT *de;

    if (((level == 0) || (strlen(patharg) < MAXPATHLEN))
	&& strlen(namearg) < MAXPATHLEN) {

	if (stat(namearg, sb_) >= 0) {
	    mode = (int) (sb.st_mode & S_IFMT);
	    ok_acc = 0;
	    if (mode == S_IFDIR)
		ok_acc |= (R_OK | X_OK);
	    if (type) {
		if (strchr(type, 'r'))
		    ok_acc |= R_OK;
		if (strchr(type, 'w'))
		    ok_acc |= W_OK;
		if (strchr(type, 'x'))
		    ok_acc |= X_OK;
	    }
	    if (ok_acc != 0) {
		if ((ok_acc = access(namearg, ok_acc)) >= 0)
		    total++;
	    } else {
		total++;
	    }
	} else {
	    sb_ = NULL;
	}

	if (level == 0) {	/* do this once, to initialize */
	    patharg = getwd(old_wd);
	} else {		/* ...and inherit otherwise */
	    (void) strcpy(old_wd, patharg);
	}

	if (((*func) (patharg, namearg, sb_, ok_acc, level) >= 0)
	    && (ok_acc >= 0)
	    && (mode == S_IFDIR)) {
	    if ((chdir(namearg) >= 0)
		&& (dp = opendir(".")) != NULL) {
		abspath(strcpy(new_wd, "."));
		num = 0;
		vec = NULL;
		while ((de = readdir(dp)) != NULL) {
		    if (dotname(de->d_name))
			continue;
		    v_ALLOC(vec, num, de->d_name);
		}
		(void) closedir(dp);
		if (num != 0) {
		    qsort((PTR) vec, (size_t) num, sizeof(PTR), cmp_qsort);
		    while (num-- != 0)
			total += walktree(new_wd, vec[num],
					  func, type, level + 1);
		    dofree((PTR) vec);
		}
	    }
	    if (chdir(old_wd) != 0)
		failed(old_wd);
	}
    }
    return (total);
}

/************************************************************************
 *	test driver							*
 ************************************************************************/

#ifdef	TEST
static
void do_arg(const char *name, const char *type);

static int
WALK_FUNC(display)
{
    (void) path;

    /* filter out source-tree artifacts to make tests regress */
    if (!strcmp(name, ".@-RCS"))
	return -1;
    if (!strcmp(name, "RCS/"))
	return -1;
    if (!strcmp(name, "RCS"))
	return -1;

    PRINTF("%c\t", (readable < 0) ? '?' : ' ');
    while (level-- > 0)
	PRINTF("|--%c", (level > 0) ? '-' : ' ');
    if ((sp != 0)
	&& ((sp->st_mode & S_IFMT) == S_IFDIR))
	PRINTF("%s%c\n", name, isSlash(name[strlen(name) - 1]) ? ' ' : PATH_SLASH);
    else
	PRINTF("%s\n", name);
    return (readable);
}

static void
do_arg(const char *name, const char *type)
{
    PRINTF("** path = %s\n", name);
    PRINTF("** total= %d\n", walktree((PTR) 0, name, display, type, 0));
}

/*ARGSUSED*/
_MAIN
{
    int j;
    const char *opts = "wrx";
    char need[BUFSIZ];
    int len = 0;

    while ((j = getopt(argc, argv, opts)) != EOF)
	switch (j) {
	case 'w':
	case 'r':
	case 'x':
	    need[len++] = (char) j;
	    break;
	default:
	    FPRINTF(stderr, "usage: walktree [-%s] [dir [...]]\n",
		    opts);
	    (void) exit(FAIL);
	}

    need[len] = EOS;
    PRINTF("** mode = %s\n", need);
    if (optind < argc) {
	for (j = optind; j < argc; j++)
	    do_arg(argv[j], need);
    } else {
	do_arg(".", need);
    }

    (void) exit(SUCCESS);
    /*NOTREACHED */
}
#endif
