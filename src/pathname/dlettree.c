#ifndef	lint
static	char	what[] = "$Header: /users/source/archives/td_lib.vcs/src/pathname/RCS/dlettree.c,v 1.1 1988/12/12 07:40:30 dickey Exp $";
#endif	lint

/*
 * Title:	deletetree.c
 * Author:	T.E.Dickey
 * Created:	17 Nov 1988
 * Function:	Deletes a tree of files a la "rm -rf".
 *
 *		On VMS, we use this also to hide the fact that DCL does not
 *		do wildcard expansion.  Hence, the "recur" option may be
 *		false simply to do in-directory scanning.
 *
 *		Note that this assumes that we do directory-recursion in a
 *		unix-style, rather than the "..." wildcard that VMS provides.
 */

#define		DIR_PTYPES
#define		STR_PTYPES
#include	"portunix.h"
#include	<errno.h>
extern	char	*getcwd();

#define	TELL		fprintf(stderr,

#ifdef	TEST
#define	fail		perror
#define	TELL_(s)	changes,nesting,s
#define	TELL_FILE(name)	TELL "%d\t%s => %s\n", TELL_(name));
#define	TELL_DIR(name)	TELL "%d\t%s (directory) %s\n", TELL_(name)); 
#define	TELL_SCAN(name)	TELL "%d\t%s scan directory %s\n", TELL_(name)); 
static	deletedir()	{ return 1;}
static	deletefile()	{ return 1;}
#else	TEST
#define	TELL_FILE(name)
#define	TELL_SCAN(name)
#define	TELL_DIR(name)
#endif	TEST


deletetree(oldname,recur)
char	*oldname;
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	int		changes = 0;
	auto	struct stat	sb;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];

#ifdef	TEST
	static	char		stack[]	= ". . . . . . . ";
	auto	char		*nesting = &stack[sizeof(stack)-(recur*2)+1];
#endif	TEST

	if (stat(oldname, &sb) < 0) {
		errno = ENOENT;		/* bypass vms-bug */
		fail(oldname);
		return(0);
	}

	if (_OPENDIR(oldname,sb.st_mode)) {
		TELL_SCAN(oldname);

		if (dirp = opendir(oldname)) {
			while (dp = readdir(dirp)) {
				(void)strcpy(newname, dp->d_name);
				if (stat(newname, &sb) < 0) {
					fail(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					if (!recur)
						continue;
					if (getcwd(oldpath,sizeof(oldpath)-2)
					&&  chdir(dir2path(newname)) >= 0) {
						changes += deletetree(
							OPENDIR_ARG,
							recur+1);
						(void)chdir(oldpath);
						changes += deletedir(newname);
						TELL_DIR(newname);
					} else
						fail(newname);
				} else if (isFILE(sb.st_mode)) {
					TELL_FILE(newname);
					changes += deletefile(newname);
				}
			}
			closedir(dirp);
		}
	} else {
		TELL_FILE(oldname);
		changes += deletefile(oldname);
	}
	return (changes);
}

#ifdef	TEST
main(argc, argv)
char	*argv[];
{
	register int	j, k;

	for (j = 1; j < argc; j++)
		printf("count = %d\n", deletetree(argv[j],0));
	exit(SUCCESS);
}
#endif	TEST
