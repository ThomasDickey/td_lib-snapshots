#ifndef	lint
static	char	Id[] = "$Id: edittree.c,v 5.0 1991/05/20 17:16:10 ste_cm Rel $";
#endif

/*
 * Title:	edittree.c
 * Author:	T.E.Dickey
 * Created:	06 Oct 1988
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

#define	isDIR(m)	((m & S_IFMT) == S_IFDIR)
#define	isFILE(m)	((m & S_IFMT) == S_IFREG)

#define	TELL		fprintf(stderr,

#ifdef	TEST
#define	TELL_FILE(name)	TELL "%d\t%s => %s\n", changes, nesting, name);
#define	TELL_DIR(name)	TELL "%d\t%s directory %s\n", changes, nesting, name); 
static	editfile(n,f)	char *n; int (*f)(); { return 1;}
#else
#define	TELL_FILE(name)
#define	TELL_DIR(name)
#endif

#ifdef	vms
#define	EDITDIR_ARG	"*.*;"
#else	/* unix */
#define	EDITDIR_ARG	"."
#endif	/* vms/unix */

edittree(oldname,func,recur)
char	*oldname;
int	(*func)();
int	recur;
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	int		changes = 0;
	auto	int		next	= recur ? recur+1 : 0;
	auto	struct stat	sb;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];
	auto	char		*newpath;

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(recur*2)];
#endif

	if (stat(oldname, &sb) < 0) {
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
					if (!next)
						continue;
					changes += edittree(newname, func, next);
				} else if (isFILE(sb.st_mode)) {
					TELL_FILE(newname);
					changes += editfile(newname, func);
				}
			}
			closedir(dirp);
		}
		(void)chdir(oldpath);
	} else {
		TELL_FILE(oldname);
		changes += editfile(oldname, func);
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

	for (j = 1; j < argc; j++) {
		if (!strcmp(argv[j], "-r"))
			recur = TRUE;
		else
			printf("count = %d\n", edittree(argv[j],do_copy,recur));
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
#else	/* unix */
					"..",  "-r", ".."
#endif	/* vms/unix */
					};
		do_test(sizeof(tbl)/sizeof(tbl[0]), tbl);
	}
	exit(SUCCESS);
}
#endif	/* TEST */
