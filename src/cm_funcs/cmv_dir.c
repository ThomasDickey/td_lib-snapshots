#ifndef	NO_IDENT
static	char	Id[] = "$Id: cmv_dir.c,v 12.7 1994/09/29 22:39:01 tom Exp $";
#endif

/*
 * Title:	cmv_dir.c (returns name of CmVision-directory)
 * Author:	T.E.Dickey
 * Created:	02 Aug 1994, from 'sccs_dir.c'
 * Modified:
 *		
 * Function:	Encapsulates the name of the CMV directory.
 *
 *		The environment-variable CMV_VAULT must be defined.  It is not
 *		a subdirectory.  The caller supplies his working directory and
 *		the name of the file that we use as a reference), we determine
 *		the name of the corresponding CMV directory.
 *
 *		The CMV_VAULT variable consists of one or more pathname
 *		assignments separated by colons, e.g.,
 *		
 *			/arc/src1=/work/src1:/arc2=/work2=/work3:/arc/misc
 *
 *		We assume that the caller doesn't modify our return value;
 *		this lets us use 'txtalloc()'.
 *
 * FIXME:	this doesn't seem to pick up filenames at the top-level
 * FIXME:	this doesn't seem to handle multiple vaults
 */

#define	STR_PTYPES
#include "ptypes.h"
#include "cmv_defs.h"

/******************************************************************************/
#ifdef	lint
#define	typealloc(type)	(type *)0
#else
#define	typealloc(type)	(type *)doalloc((char *)0, sizeof(type))
#endif

/******************************************************************************/
#define	WORKING	struct	Working
	WORKING	{
	WORKING	*next;
	char	*working;
	};

#define	CMFILE	struct	CmFile
	CMFILE	{
	CMFILE	*next;
	char	*internal;	/* internal file-leafname (e.g., f-1234) */
	char	*external;	/* external file-leafname (e.g., Makefile) */
	char	*lockedby;	/* name of lock-owner, if non-null */
	char	*revision;	/* ...and current-revision number */
	};

#define	CMTREE	struct	CmTree
	CMTREE	{
	CMTREE	*cmparent;
	CMTREE	*children;
	CMTREE	*siblings;
	CMFILE	*filelist;
	char	*internal;	/* internal leaf-name */
	char	*external;	/* external leaf-name */
	char	*fullpath;
	long	marktime;	/* ...from reading r-curr */
	};

#define	VAULTS	struct	Vaults
	VAULTS	{
	VAULTS	*next;
	char	*archive;	/* directory-path of the CmVision vault */
	CMTREE	*cmtree;	/* vault's internal structure */
	WORKING	*working;	/* working trees associated with vault */
	};

static	int	initialized;
static	char	*CmvVault;
static	VAULTS	*VaultList;

static	void	Initialize(_ar0);

/******************************************************************************/

/*
 * Allocate a CMTREE node and insert default values
 */
static
CMTREE *NewCmTree(
	_AR1(char *,	pathname))
	_DCL(char *,	pathname)
{
	CMTREE	*p = typealloc(CMTREE);

	p->filelist = 0;
	p->children =
	p->siblings = 0;
	p->fullpath = txtalloc(pathname);
	p->internal =
	p->external = txtalloc("");
	p->marktime = 0;
	return p;
}

/*
 * Returns a relative path for the given internal leaf name
 */
static
char *	NewInternal(
	_ARX(CMTREE *,	parent)
	_AR1(char *,	internal)
		)
	_DCL(CMTREE *,	parent)
	_DCL(char *,	internal)
{
	if (parent != 0 && parent->internal[0] != EOS) {
		char	temp[MAXPATHLEN];
		FORMAT(temp, "%s%c%s", parent->internal, PATH_SLASH, internal);
		internal = temp;
	}
	return txtalloc(internal);
}

/*
 * Returns a relative path for the given internal leaf name
 */
static
char *	NewExternal(
	_ARX(CMTREE *,	parent)
	_AR1(char *,	external)
		)
	_DCL(CMTREE *,	parent)
	_DCL(char *,	external)
{
	if (parent != 0 && parent->external[0] != EOS) {
		char	temp[MAXPATHLEN];
		FORMAT(temp, "%s%c%s", parent->external, PATH_SLASH, external);
		external = temp;
	}
	return txtalloc(external);
}

/*
 * Read the entries in the s-curr file in the given directory, adding them
 * to the children of the given CMTREE node.
 */
static
void	read_s_curr(
	_ARX(char *,	archive)	/* directory that may contain s-curr */
	_AR1(CMTREE *,	parent)		/* parent node to populate */
		)
	_DCL(char *,	archive)
	_DCL(CMTREE *,	parent)
{
	char	temp[MAXPATHLEN];
	char	internal[BUFSIZ];
	char	external[BUFSIZ];
	FILE	*fp;
	CMTREE	*p, *q;

	if ((fp = fopen(pathcat(temp, archive, "s-curr"), "r")) != 0) {
		while (fgets(temp, sizeof(temp), fp) != 0) {
			char *s = strchr(strtrim(temp), ';');
			if (s == 0)
				continue;
			*s++ = EOS;
			(void)strcpy(internal, temp);
			if ((s = strchr(s, ';')) == 0)
				continue;
			(void)strcpy(external, s+1);
			p = NewCmTree(pathcat(temp, archive, internal));
			p->internal = NewInternal(parent, internal);
			p->external = NewExternal(parent, external);
			if ((q = parent->children) != 0)
				p->siblings = q;
			parent->children = p;
		}
		FCLOSE(fp);
	}
}

/*
 * Find the internal pathname, given the vault and the external name relative
 * to the top of the working tree.
 */
static
CMTREE *FindInternalDir(
	_ARX(CMTREE *,	parent)
	_AR1(char *,	external)
		)
	_DCL(CMTREE *,	parent)
	_DCL(char *,	external)
{
	CMTREE	*p;
	size_t	len1 = strlen(external);
	size_t	len2;

	if (len1 == 0)
		return parent;
	p = parent->children;
	if (p == 0) {
		read_s_curr(parent->fullpath, parent);
		p = parent->children;
	}
	/* if not at the current level, recur */
	while (p != 0) {
		if (!strcmp(p->external, external))
			return p;
		len2 = strlen(p->external);
		if (len2 <= len1
		 && !strncmp(p->external, external, len2)
		 && (external[len2] == PATH_SLASH)) {
			return FindInternalDir(p, external);
		}
		p = p->siblings;
	}

	return 0;
}

/******************************************************************************/
/*
 * Read the entries in the r-curr file in the given directory, adding them to
 * the file-list of the given CMTREE node.
 *
 * Fields are separated by semicolons:
 *	internal;description;external;version
 *
 * An empty description is usually a "/".  The first word of a nonempty
 * description is the user who's locked the file for changes.
 */
static
void	read_r_curr(
	_AR1(CMTREE *,	parent))
	_DCL(CMTREE *,	parent)
{
	char	temp[BUFSIZ];
	FILE	*fp;
	CMFILE	*p;
	Stat_t	sb;

	if (stat(pathcat(temp, parent->fullpath, "r-curr"), &sb) < 0)
		return;
	if (parent->marktime == sb.st_mtime)
		return;
	parent->marktime = sb.st_mtime;

	/* discard the previous contents of the cache */
	while (parent->filelist != 0) {
		p = parent->filelist;
		parent->filelist = p->next;
		/* the members are all 'txtalloc()' heap: don't free */
		dofree((char *)p);
	}

	if ((fp = fopen(temp, "r")) != 0) {
		while (fgets(temp, sizeof(temp), fp) != 0) {
			char	*external;
			char	*internal;
			char	*description;
			char	*s;
			char	*d;

			internal = strtrim(temp);
			if ((s = strchr(temp, ';')) == 0)
				continue;
			*s++ = EOS;

			description = s;
			if ((s = strchr(s, ';')) == 0)
				continue;
			*s++ = EOS;

			external = s;
			if ((d = strchr(s, ';')) == 0)
				continue;
			*d++ = EOS;	/* version follows */

			/* chop off the first word to get the lock-owner */
			if ((s = strchr(description, ' ')) != 0) {
				*s = EOS;
				description = txtalloc(description);
			} else {
				description = 0;
			}

			p = typealloc(CMFILE);
			p->lockedby = description;
			p->revision = txtalloc(d);
			p->internal = NewInternal(parent, internal);
			p->external = NewExternal(parent, external);
			p->next = parent->filelist;
			parent->filelist = p;
		}
		FCLOSE(fp);
	}
}

static
CMFILE *FindInternalFile(
	_ARX(CMTREE *,	parent)
	_AR1(char *,	external))
	_DCL(CMTREE *,	parent)
	_DCL(char *,	external)
{
	CMFILE	*p;
	if (parent->filelist == 0)
		read_r_curr(parent);
	for (p = parent->filelist; p != 0; p = p->next) {
		if (!strcmp(p->external, external))
			return p;
	}
	return 0;
}

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
		r = typealloc(VAULTS);
		r->next = 0;
		r->archive = txtalloc(pathname);
		r->working = 0;
		r->cmtree  = NewCmTree(pathname);
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
		r = typealloc(WORKING);
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
	CmvVault = getenv("CMV_VAULT");
	if (CmvVault != 0) {
		register char *s;
		char	*next, *eqls;
		int	at_next, at_eqls;
		VAULTS	*p;

		for (s = CmvVault; *s != EOS; s = next) {
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
			*next = at_next;
		}
		CmvVault = txtalloc(CmvVault);
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
static
VAULTS *LookupVault(
	_ARX(char *,	working_directory)
	_ARX(char *,	filename)
	_AR1(char *,	result)
		)
	_DCL(char *,	working_directory)
	_DCL(char *,	filename)
	_DCL(char *,	result)
{
	Stat_t	sb;
	VAULTS	*p, *max_p = 0;
	WORKING	*q;
	int	max_n = 0;

	if (!initialized)
		Initialize();
	if (CmvVault == 0 || filename == 0)
		return 0;

	/*
	 * If we're given the name of a file, compute its directory.  If we're
	 * given a directory name, use it.
	 */
	abspath(pathcat(result, working_directory, filename));
	if (stat_dir(result, &sb) < 0)
		(void)strcpy(result, pathhead(result, &sb));

	/*
	 * Now, search the CMV_VAULT variable for a working directory that
	 * matches the beginning of the string we've got in 'result[]'.  If we
	 * find that match, substitute the remainder to obtain the archive
	 * directory path.  If we find an archive without a working directory
	 * in CMV_VAULT, use this iff no prior match is found.
	 */
	for (p = VaultList; p != 0; p = p->next) {
		for (q = p->working; q != 0; q = q->next) {
			int	n;
			if ((n = samehead(result, q->working)) > 0) {
				if (n > max_n) {
					max_p = p;
					max_n = n;
				}
			}
		}
	}
	if (max_n > 0) {
		register int j;
		if (result[max_n] != EOS)
			max_n++;
		for (j = 0; (result[j] = result[j+max_n]) != EOS; j++)
			;
	}
	return (max_n > 0) ? max_p : 0;
}

/******************************************************************************/
char *	cmv_dir(
	_ARX(char *,	working_directory)
	_AR1(char *,	filename)
		)
	_DCL(char *,	working_directory)
	_DCL(char *,	filename)
{
	auto	char	*name	= 0;
	auto	char	temp[MAXPATHLEN];
	auto	VAULTS	*max_p = LookupVault(working_directory, filename, temp);

	if (max_p != 0) {	/* we found a match */
		char	archive[MAXPATHLEN];
		CMTREE	*it = FindInternalDir(max_p->cmtree, temp);

		if (it != 0) {
			(void)pathcat(
				archive,
				strcpy(archive, max_p->archive),
				it->internal);
			name = txtalloc(archive);
		}
	}

	return (name);
}

/******************************************************************************/
char *	cmv_file (
	_ARX(char *,	working_directory)
	_AR1(char *,	filename)
		)
	_DCL(char *,	working_directory)
	_DCL(char *,	filename)
{
	auto	char	*name	= 0;
	auto	char	temp[MAXPATHLEN];
	auto	VAULTS	*max_p = LookupVault(working_directory, filename, temp);

	if (max_p != 0) {	/* we found a match */
		char	archive[MAXPATHLEN];
		CMTREE	*p;
		CMFILE	*q;

		if ((p = FindInternalDir(max_p->cmtree, temp)) != 0
		 && (q = FindInternalFile(p, pathcat(temp, temp, pathleaf(filename)))) != 0) {
			(void)pathcat(
				archive,
				pathcat(
					archive,
					strcpy(archive, max_p->archive),
					p->internal),
				pathleaf(q->internal));
			name = txtalloc(archive);
		}
	}

	return (name);
}

/******************************************************************************/
void	get_cmv_lock (
	_ARX(char *,	working_directory)
	_ARX(char *,	filename)
	_ARX(char **,	lockedby)
	_AR1(char **,	revision)
		)
	_DCL(char *,	working_directory)
	_DCL(char *,	filename)
	_DCL(char **,	lockedby)
	_DCL(char **,	revision)
{
	auto	char	temp[MAXPATHLEN];
	auto	VAULTS	*max_p = LookupVault(working_directory, filename, temp);

	*lockedby = 0;
	*revision = 0;
	if (max_p != 0) {	/* we found a match */
		CMTREE	*p;
		CMFILE	*q;

		if ((p = FindInternalDir(max_p->cmtree, temp)) != 0
		 && (q = FindInternalFile(p, pathcat(temp, temp, pathleaf(filename)))) != 0) {
			*lockedby = q->lockedby;
			*revision = q->revision;
		}
	}
}

/******************************************************************************/
void	purge_cmv_dir(
	_ARX(char *,	working_directory)
	_AR1(char *,	filename)
		)
	_DCL(char *,	working_directory)
	_DCL(char *,	filename)
{
	auto	char	temp[MAXPATHLEN];
	auto	VAULTS	*max_p = LookupVault(working_directory, filename, temp);

	if (max_p != 0) {	/* we found a match */
		CMTREE	*it = FindInternalDir(max_p->cmtree, temp);

		if (it != 0) {
			read_r_curr(it);
		}
	}
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
	int	n;
	for (n = 1; n < argc; n++) {
		char	*s = cmv_dir(".", argv[n]);
		char	*t = cmv_file(".", argv[n]);
		if (s == 0)
			s = "?";
		if (t == 0)
			t = "?";
		PRINTF("%d:%d %s\n", n, argc-1, argv[n]);
		PRINTF("\tdir  '%s'\n", s);
		PRINTF("\tfile '%s'\n", t);
	}
	exit(EXIT_SUCCESS);
	/*NOTREACHED*/
}
#endif	/* TEST */
