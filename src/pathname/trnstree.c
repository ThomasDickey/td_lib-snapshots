#ifndef	lint
static	char	what[] = "$Id: trnstree.c,v 2.1 1989/07/26 13:18:23 dickey Exp $";
#endif	lint

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
extern	char	*getcwd();

transtree(oldname,func,recur)
char	*oldname;
int	(*func)();
{
	auto	DIR		*dirp;
	auto	struct	direct	*dp;
	auto	struct stat	sb;
	auto	char		newname[MAXPATHLEN];
	auto	char		oldpath[MAXPATHLEN];

	if (stat(oldname, &sb) < 0) {
		errno = ENOENT;		/* bypass vms-bug */
		perror(oldname);
	}

	if (_OPENDIR(oldname,sb.st_mode)) {
		if (dirp = opendir(oldname)) {
			while (dp = readdir(dirp)) {
				(void)strcpy(newname, dp->d_name);
				if (stat(newname, &sb) < 0) {
					perror(newname);
					continue;
				}
				if (isDIR(sb.st_mode)) {
					if (!recur)
						continue;
					if (getcwd(oldpath,sizeof(oldpath)-2)
					&&  chdir(DIR2PATH(newname)) >= 0) {
						transtree(
							OPENDIR_ARG,
							func,
							recur+1);
						(void)chdir(oldpath);
					} else
						perror(newname);
				} else if (isFILE(sb.st_mode)) {
					(*func)(newname);
				}
			}
			closedir(dirp);
		}
	} else if (isFILE(sb.st_mode)) {
		(*func)(oldname);
	}
}
