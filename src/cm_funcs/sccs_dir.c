/*
 * Title:	sccs_dir.c (returns name of SCCS-directory)
 * Author:	T.E.Dickey
 * Created:	02 Sep 1988
 * Modified:
 *		24 May 2010, fix clang --analyze warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		25 Apr 2003, split-out samehead.c, add check on return-value.
 *		26 Mar 2002, force paths to lowercase on cygwin.
 *		21 Jul 2000, mods to support checkup -o & $SCCS_VAULT
 *		30 Jun 2000, fix infinite loop if PATHLIST_SEP is found.
 *		13 Jul 1994, added SCCS_VAULT interpretation.
 *		29 Oct 1993, ifdef-ident
 *		21 Sep 1993, gcc-warnings
 *		03 Oct 1991, conversion to ANSI
 *		15 May 1991, apollo sr10.3 cpp complains about tag in #endif
 *		22 Jun 1990, changed default value from "sccs" to "SCCS" since
 *			     this seems to be the currently more-common usage.
 *		
 * Function:	Encapsulates the name of the SCCS directory, permitting the
 *		user to override our default by assigning an environment
 *		variable "SCCS_DIR".
 *
 *		If the environment-variable SCCS_VAULT is defined, we assume
 *		that the SCCS directory is not a subdirectory.  In this case
 *		(if the caller supplies his working directory and the name of
 *		the file that we use as a reference), we determine the name of
 *		the corresponding SCCS directory.
 *
 *		The SCCS_VAULT variable consists of one or more pathname
 *		assignments separated by colons, e.g.,
 *		
 *			/arc/src1=/work/src1:/arc2=/work2=/work3:/arc/misc
 *
 *		We assume that the caller doesn't modify our return value;
 *		this lets us use 'txtalloc()'.
 *
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "sccsdefs.h"

MODULE_ID("$Id: sccs_dir.c,v 12.12 2010/05/24 22:25:22 tom Exp $")

#define	WORKING	struct	Working
WORKING {
    WORKING *next;
    char *working;
};

#define	VAULTS	struct	Vaults
VAULTS {
    VAULTS *next;
    char *archive;
    WORKING *working;
};

static int initialized;
static char *SccsDir;
static char *SccsVault;
static VAULTS *VaultList;

#ifdef __CYGWIN__
static char *
path_alloc(char *s)
{
    char temp[1024];
    strlwrcpy(temp, s);
    return txtalloc(temp);
}
#else
#define path_alloc(s) txtalloc(s)
#endif

/******************************************************************************/

/*
 * We put stuff on the end of the linked list to preserve a natural ordering
 * of the search path.
 */
static VAULTS *
add_archive(char *pathname)
{
    if (*pathname != EOS) {
	VAULTS *p, *q, *r;
#ifdef __CYGWIN__
	char temp[1024];
	if (*pathname == '"') {
	    size_t len;
	    pathname = strlwrcpy(temp, pathname + 1);
	    len = strlen(pathname);
	    if (len != 0 && pathname[len - 1] == '"')
		pathname[len - 1] = EOS;
	} else {
	    pathname = strlwrcpy(temp, pathname);
	}
#endif
	for (p = VaultList, q = 0; p != 0; q = p, p = p->next) ;
	r = (VAULTS *) doalloc((char *) 0, sizeof(VAULTS));
	r->next = 0;
	r->archive = path_alloc(pathname);
	r->working = 0;
	if (q == 0)
	    VaultList = r;
	else
	    q->next = r;
	return r;
    }
    return 0;
}

static void
add_working(VAULTS * list, char *pathname)
{
    if (*pathname != EOS) {
	WORKING *p, *q, *r;
	for (p = list->working, q = 0; p != 0; q = p, p = p->next) ;
	r = (WORKING *) doalloc((char *) 0, sizeof(WORKING));
	r->next = 0;
	r->working = path_alloc(pathname);
	if (q == 0)
	    list->working = r;
	else
	    q->next = r;
    }
}

/******************************************************************************/
static void
Initialize(void)
{
    initialized = TRUE;
    SccsDir = getenv("SCCS_DIR");
    if (SccsDir == 0)
	SccsDir = "SCCS";
    SccsDir = path_alloc(SccsDir);

    SccsVault = getenv("SCCS_VAULT");
    if (SccsVault != 0) {
	char *s;
	char *next, *eqls;
	int at_next, at_eqls;
	VAULTS *p;

	for (s = SccsVault; *s != EOS; s = next) {
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
	SccsVault = path_alloc(SccsVault);
    }
}

/******************************************************************************/
char *
sccs_dir(char *working_directory, char *filename)
{
    char *name;
    int vault = FALSE;
    int n;

    if (!initialized)
	Initialize();

    name = SccsDir;
    if (filename != 0 && SccsVault != 0) {
	Stat_t sb;
	char temp[MAXPATHLEN];
	VAULTS *p, *max_p = 0;
	WORKING *q;
	int max_n = 0;

	/*
	 * If we're given the name of a file, compute its directory.
	 * If we're given a directory name, use it.
	 */
	abspath(pathcat(temp, working_directory, filename));
	if (stat_dir(temp, &sb) < 0)
	    (void) strcpy(temp, pathhead(temp, &sb));

	/*
	 * Now, search the SCCS_VAULT variable for a working directory
	 * that matches the beginning of the string we've got in
	 * 'temp[]'.  If we find that match, substitute the remainder
	 * to obtain the archive directory path.  If we find an archive
	 * without a working directory in SCCS_VAULT, use this iff no
	 * prior match is found.
	 */
	for (p = VaultList; p != 0; p = p->next) {
	    if ((n = samehead(temp, p->archive)) > 0
		&& n >= (int) strlen(p->archive)) {
		if (n > max_n) {
		    max_p = p;
		    max_n = n;
		    vault = TRUE;
		}
	    }
	    for (q = p->working; q != 0; q = q->next) {
		if ((n = samehead(temp, q->working)) > 0
		    && n >= (int) strlen(q->working)) {
		    if (n > max_n) {
			max_p = p;
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
	    char archive[MAXPATHLEN];

	    if (vault) {
		/*
		 * FIXME: if we found the path was in a vault
		 * directory, use it as is (we are assuming
		 * the working directory corresponds to this
		 * vault!)
		 */
		name = path_alloc(temp);
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
		name = path_alloc(archive);
	    }
	}
    }

    return (name);
}
