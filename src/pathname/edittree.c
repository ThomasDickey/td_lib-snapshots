#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/edittree.c,v 1.1 1988/11/22 09:55:28 dickey Exp $";
#endif	lint

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
static	editfile()	{ return 1;}
#else	TEST
#define	TELL_FILE(name)
#define	TELL_DIR(name)
#endif	TEST


edittree(oldname,func,recur)
char	*oldname;
int	(*func)();
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	int		changes = 0;
	auto	int		next	= recur ? recur+1 : 0;
	auto	struct stat	sb;
	auto	char		newname[BUFSIZ];

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(next*2)+1];
#endif	TEST

	if (stat(oldname, &sb) < 0) {
		errno = ENOENT;		/* bypass vms-bug */
		perror(oldname);
		return(0);
	}

	if (_OPENDIR(oldname,sb.st_mode)) {
		TELL_DIR(oldname);

		if (dirp = opendir(oldname)) {
			while (dp = readdir(dirp)) {
				(void)strcpy(newname, dp->d_name);
				if (stat(newname, &sb) < 0) {
					perror(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					if (!next)
						continue;
#ifdef	vms
					(void)strcat(strcpy(newname,
							dir2path(newname)),
						"*.*;");
#else	unix
					(void)strcat(newname, "/*");
#endif	vms or unix
					changes += edittree(newname, func, next);
				} else if (isFILE(sb.st_mode)) {
					TELL_FILE(newname);
					changes += editfile(newname, func);
				}
			}
			closedir(dirp);
		}
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

main(argc, argv)
char	*argv[];
{
	register int	j, k;

	for (j = 1; j < argc; j++)
		printf("count = %d\n", edittree(argv[j], do_copy,1));
	exit(SUCCESS);
}
#endif	TEST
