/*
 * Title:	rcs_dir.c (returns name of RCS-directory)
 * Author:	T.E.Dickey
 * Created:	02 Sep 1988
 * Modified:
 *		11 Dec 2001, make this a clone of sccs_dir() to implement
 *			     $RCS_VAULT.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, converted to ANSI
 *
 *		Revision 8.1  91/05/15  10:04:43  dickey
 *		apollo sr10.3 cpp complains about tag in #endif
 *
 * Function:	Encapsulates the name of the RCS directory, permitting the
 *		user to override our default by assigning an environment
 *		variable "RCS_DIR".
 */

#define STR_PTYPES
#include "ptypes.h"
#include "rcsdefs.h"

MODULE_ID("$Id: rcs_dir.c,v 12.4 2001/12/11 13:55:53 tom Exp $")

#define	WORKING	struct	Working
	WORKING	{
	WORKING	*next;
	char	*working;
	};

#define	VAULTS	struct	Vaults
	VAULTS	{
	VAULTS	*next;
	char	*archive;
	WORKING	*working;
	};

static	int	initialized;
static	char	*RcsDir;
static	char	*RcsVault;
static	VAULTS	*VaultList;

/******************************************************************************/

/*
 * We put stuff on the end of the linked list to preserve a natural ordering
 * of the search path.
 */
static
VAULTS	*add_archive(
	_AR1(char *,	pathname))
	_DCL(char *,	pathname)
{
	if (*pathname != EOS) {
		VAULTS	*p, *q, *r;
		for (p = VaultList, q = 0; p != 0; q = p, p = p->next)
			;
		r = (VAULTS *)doalloc((char *)0, sizeof(VAULTS));
		r->next = 0;
		r->archive = txtalloc(pathname);
		r->working = 0;
		if (q == 0)
			VaultList = r;
		else
			q->next = r;
		return r;
	}
	return 0;
}

static
void	add_working(
	_ARX(VAULTS *,	list)
	_AR1(char *,	pathname)
		)
	_DCL(VAULTS *,	list)
	_DCL(char *,	pathname)
{
	if (*pathname != EOS) {
		WORKING *p, *q, *r;
		for (p = list->working, q = 0; p != 0; q = p, p = p->next)
			;
		r = (WORKING *)doalloc((char *)0, sizeof(WORKING));
		r->next = 0;
		r->working = txtalloc(pathname);
		if (q == 0)
			list->working = r;
		else
			q->next = r;
	}
}

/******************************************************************************/
static
void	Initialize(_AR0)
{
	initialized = TRUE;
	RcsDir = getenv("RCS_DIR");
	if (RcsDir == 0)
		RcsDir = "RCS";
	RcsDir = txtalloc(RcsDir);

	RcsVault = getenv("RCS_VAULT");
	if (RcsVault != 0) {
		register char *s;
		char	*next, *eqls;
		int	at_next, at_eqls;
		VAULTS	*p;

		for (s = RcsVault; *s != EOS; s = next) {
			next = strchr(s, PATHLIST_SEP);
			if (next == 0)
				next = s + strlen(s);
			at_next = *next;
			*next = EOS;

			eqls = strchr(s, '=');
			if (eqls == 0)
				eqls = next;
			at_eqls = *eqls;
			*eqls = EOS;

			if ((p = add_archive(s)) != 0) {
				while (eqls != next) {
					*eqls = at_eqls;
					s = eqls + 1;

					eqls = strchr(s, '=');
					if (eqls == 0)
						eqls = next;
					at_eqls = *eqls;
					*eqls = EOS;

					add_working(p, s);
				}
			}
			if ((*next = at_next) != EOS)
				next++;
		}
		RcsVault = txtalloc(RcsVault);
	}
}

/******************************************************************************/
/*patch:pathname?*/
static
/*
 * Compare two pathnames, returning the length of the matching portion,
 * limited to a pathname separator.  Note that 'strchr()' can use a null
 * character for the second argument.
 */
#define	isPath(c)	(strchr(PATH_DELIMS,c) != 0)

int	samehead(
	_ARX(char *,	path1)
	_AR1(char *,	path2)
		)
	_DCL(char *,	path1)
	_DCL(char *,	path2)
{
	int	match = 0;
	register int	n;

	for (n = 0; ; n++) {
		if (isPath(path1[n]) && isPath(path2[n]))
			match = n;
		if (path1[n] == EOS || path2[n] == EOS)
			break;
		if (path1[n] != path2[n])
			break;
	}
	return match;
}

/******************************************************************************/
char *	rcs_dir(
	_ARX(char *,	working_directory)
	_AR1(char *,	filename)
		)
	_DCL(char *,	working_directory)
	_DCL(char *,	filename)
{
	char	*name;
	int	vault	= FALSE;
	int	n;

	if (!initialized)
		Initialize();

	name = RcsDir;
	if (filename != 0 && RcsVault != 0) {
		Stat_t	sb;
		char	temp[MAXPATHLEN];
		VAULTS	*p, *max_p = 0;
		WORKING	*q, *max_q = 0;
		int	max_n = 0;

		/*
		 * If we're given the name of a file, compute its directory.
		 * If we're given a directory name, use it.
		 */
		abspath(pathcat(temp, working_directory, filename));
		if (stat_dir(temp, &sb) < 0)
			(void)strcpy(temp, pathhead(temp, &sb));

		/*
		 * Now, search the RCS_VAULT variable for a working directory
		 * that matches the beginning of the string we've got in
		 * 'temp[]'.  If we find that match, substitute the remainder
		 * to obtain the archive directory path.  If we find an archive
		 * without a working directory in RCS_VAULT, use this iff no
		 * prior match is found.
		 */
		for (p = VaultList; p != 0; p = p->next) {
			if ((n = samehead(temp, p->archive)) > 0) {
				if (n > max_n) {
					max_p = p;
					max_q = p->working;
					max_n = n;
					vault = TRUE;
				}
			}
			for (q = p->working; q != 0; q = q->next) {
				if ((n = samehead(temp, q->working)) > 0) {
					if (n > max_n) {
						max_p = p;
						max_q = q;
						max_n = n;
						vault = FALSE;
					}
				}
			}
		}

		/*
		 * Construct the correspond archive-directory name.
		 * If this isn't the same as the last value that I returned,
		 * update my buffer.
		 */
		if (max_n > 0) {	/* we found a match */
			char	archive[MAXPATHLEN];

			if (vault) {
				/*
				 * FIXME: if we found the path was in a vault
				 * directory, use it as is (we are assuming
				 * the working directory corresponds to this
				 * vault!)
				 */
				name = txtalloc(temp);
			} else {
				if (temp[max_n] != EOS)
					max_n++;
				pathcat(
					archive,
					pathcat(
						archive,
						strcpy(archive, max_p->archive),
						temp + max_n),
					name);
				name = txtalloc(archive);
			}
		}
	}

	return (name);
}
