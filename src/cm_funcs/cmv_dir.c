/*
 * Title:	cmv_dir.c (returns name of CmVision-directory)
 * Author:	T.E.Dickey
 * Created:	02 Aug 1994, from 'sccs_dir.c'
 * Modified:
 *		01 May 2020, coverity warnings.
 *		11 Dec 2019, clang warnings.
 *		27 Dec 2014, coverity warnings.
 *		07 Mar 2004, remove K&R support, indent'd.
 *		25 Apr 2003, split-out samehead.c, add check on return-value.
 *		21 Aug 1998, get_cmv_lock now returns binary-file mod-times.
 *		06 Jul 1995, mods to check for the level of the given
 *			     vault-directory
 *		14 Mar 1995, mods to support release-branches
 *		18 Feb 1995, version-string ends with either space or semicolon
 *		31 Jan 1995, lock-owner must be "?", not null
 *		27 Jan 1995, quick fix for binary-file version parsing
 *		05 Jan 1995, fix an infinite loop when parsing multiple archives
 *		29 Sep 1994, automatically purge the cached r-curr only when
 *			     the actual file changes.
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
 */

#define	CHR_PTYPES
#define	STR_PTYPES
#include "ptypes.h"
#include "cmv_defs.h"
#include "dyn_str.h"

MODULE_ID("$Id: cmv_dir.c,v 12.41 2025/01/07 00:03:17 tom Exp $")

/******************************************************************************/

#define	typealloc(type)	(type *)doalloc((char *)0, sizeof(type))

#ifdef DEBUG
#define Debug(p) fprintf p
#else
#define Debug(p)		/* nothing */
#endif

/******************************************************************************/
#define	WORKING	struct	Working
WORKING {
    WORKING *next;
    char *working;
};

#define	CMFILE	struct	CmFile
CMFILE {
    CMFILE *next;
    char *internal;		/* internal file-leafname (e.g., f-1234) */
    char *external;		/* external file-leafname (e.g., Makefile) */
    char *lockedby;		/* name of lock-owner, if non-null */
    char *revision;		/* ...and current-revision number */
    time_t modtime;		/* modification, if binary */
};

#define	CMTREE	struct	CmTree
CMTREE {
    CMTREE *children;
    CMTREE *siblings;
    CMFILE *filelist;
    char *internal;		/* internal leaf-name */
    char *external;		/* external leaf-name */
    char *fullpath;
    long marktime;		/* ...from reading r-curr */
    int level;			/* ...level of 'internal' name */
};

#define	VAULTS	struct	Vaults
VAULTS {
    VAULTS *next;
    char *archive;		/* directory-path of the CmVision vault */
    int level;			/* ...its level */
    CMTREE *cmtree;		/* vault's internal structure */
    WORKING *working;		/* working trees associated with vault */
};

static int initialized;
static char *CmvVault;
static VAULTS *VaultList;

/******************************************************************************/

/*
 * Allocate a CMTREE node and insert default values
 */
static CMTREE *
NewCmTree(const char *pathname)
{
    CMTREE *p = typealloc(CMTREE);

    p->filelist = NULL;
    p->children =
	p->siblings = NULL;
    p->fullpath = txtalloc(pathname);
    p->internal =
	p->external = txtalloc("");
    p->marktime = 0;
    return p;
}

static char *
PathCat(const char *head, const char *tail)
{
    char temp[MAXPATHLEN];
    size_t have = strlen(head);
    size_t want = strlen(tail);

    if (have + want + 2 < MAXPATHLEN) {

	strcpy(temp, head);
	temp[have++] = PATH_SLASH;
	strcpy(temp + have, tail);
	tail = temp;
    }
    return txtalloc(tail);
}

/*
 * Returns a relative path for the given internal leaf name
 */
static char *
NewInternal(CMTREE * parent, const char *internal)
{
    if (parent != NULL && parent->internal[0] != EOS) {
	internal = PathCat(parent->internal, internal);
    }
    return txtalloc(internal);
}

/*
 * Returns a relative path for the given internal leaf name
 */
static char *
NewExternal(CMTREE * parent, const char *external)
{
    if (parent != NULL && parent->external[0] != EOS) {
	external = PathCat(parent->external, external);
    }
    return txtalloc(external);
}

/*
 * CmVision stores analogous information to the s-curr file in the top-level
 * directories.  We use these also for navigating.
 */
static char *
parts_list(char *result, const char *archive, int level)
{
    const char *leaf;

    switch (level) {
    case 0:
	leaf = "PROJ-list";
	break;
    case 1:
	leaf = "CPCI-list";
	break;
    default:
	leaf = "s-curr";
	break;
    }

    if ((strlen(archive) + strlen(leaf) + 3) < MAXPATHLEN)
	pathcat(result, archive, leaf);
    else
	result = NULL;

    return result;
}

/*
 * Read the entries in the s-curr file in the given directory, adding them
 * to the children of the given CMTREE node.
 */
static void
read_s_curr(CMTREE * parent)	/* parent node to populate */
{
    char temp[MAXPATHLEN];
    char internal[MAXPATHLEN];
    char external[MAXPATHLEN];
    FILE *fp;
    CMTREE *p, *q;
    char *fname = parts_list(temp, parent->fullpath, parent->level);

    if ((fname != NULL) && (fp = fopen(fname, "r")) != NULL) {
	while (fgets(temp, (int) sizeof(temp), fp) != NULL) {
	    char *s = strchr(strtrim(temp), ';');
	    if (s == NULL)
		continue;
	    *s++ = EOS;
	    (void) strcpy(internal, temp);
	    if ((s = strchr(s, ';')) == NULL)
		continue;
	    (void) strcpy(external, s + 1);
	    if ((s = strchr(external, ';')) != NULL)
		*s = EOS;
	    p = NewCmTree(pathcat(temp, parent->fullpath, internal));
	    p->level = parent->level + 1;
	    p->internal = NewInternal(parent, internal);
	    p->external = NewExternal(parent, external);
	    if ((q = parent->children) != NULL)
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
static CMTREE *
FindInternalDir(CMTREE * parent, const char *external)
{
    CMTREE *p;
    size_t len1 = strlen(external);
    size_t len2;

    if (len1 == 0)
	return parent;		/* ambiguous, but better than nothing */
    p = parent->children;
    if (p == NULL) {
	read_s_curr(parent);
	p = parent->children;
    }
    /* if not at the current level, recur */
    while (p != NULL) {
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

    return NULL;
}

/*
 * Binary files are simply snapshots whose date is stored in the r-curr file.
 */
static time_t
cmv_date(const char *src)
{
    time_t result = 0;

    if (src != NULL
	&& (src = strchr(src, '\001')) != NULL) {
	int owner, group, prot;
	long modtime;
	if (sscanf(++src, "O%d:G%d:P%d:M%ld:",
		   &owner, &group, &prot, &modtime) == 4)
	    result = modtime;
    }

    return result;
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
static void
read_r_curr(CMTREE * parent)
{
    char temp[MAXPATHLEN];
    FILE *fp;
    CMFILE *p;
    Stat_t sb;

    if (stat(pathcat(temp, parent->fullpath, "r-curr"), &sb) < 0)
	return;
    if (parent->marktime == sb.st_mtime)
	return;
    parent->marktime = sb.st_mtime;

    /* discard the previous contents of the cache */
    while (parent->filelist != NULL) {
	p = parent->filelist;
	parent->filelist = p->next;
	/* the members are all 'txtalloc()' heap: don't free */
	dofree((char *) p);
    }

    if ((fp = fopen(temp, "r")) != NULL) {
	while (fgets(temp, (int) sizeof(temp), fp) != NULL) {
	    char *external;
	    char *internal;
	    char *description;
	    char *s;
	    char *d;

	    internal = strtrim(temp);
	    if ((s = strchr(temp, ';')) == NULL)
		continue;
	    *s++ = EOS;

	    description = s;
	    if ((s = strchr(s, ';')) == NULL)
		continue;
	    *s++ = EOS;

	    external = s;
	    if ((d = strchr(s, ';')) == NULL)
		continue;
	    *d++ = EOS;		/* version follows */

	    /* chop off the first word to get the lock-owner */
	    if ((s = strchr(description, ' ')) != NULL) {
		*s = EOS;
	    } else {
		description = txtalloc("");
	    }
	    description = txtalloc(description);

	    p = typealloc(CMFILE);
	    p->lockedby = description;
	    p->modtime = 0;
	    /* some revision-fields, for binary files, contain
	     * other info.
	     */
	    if ((s = strpbrk(d, " ;")) != NULL)
		*s++ = EOS;
	    p->revision = txtalloc(d);
	    p->internal = txtalloc(internal);
	    p->external = NewExternal(parent, external);
	    p->modtime = cmv_date(s);
	    p->next = parent->filelist;
	    parent->filelist = p;
	}
	FCLOSE(fp);
    }
}

static CMFILE *
FindInternalFile(CMTREE * parent, const char *external)
{
    CMFILE *p;
    if (parent->filelist == NULL)
	read_r_curr(parent);
    for (p = parent->filelist; p != NULL; p = p->next) {
	if (!strcmp(p->external, external))
	    return p;
    }
    return NULL;
}

/******************************************************************************/

/*
 * Test existence of a parts-list file at a given level
 */
static int
part_exists(const char *pathname, int level)
{
    char full[MAXPATHLEN];
    struct stat sb;
    int rc;
    char *fname = parts_list(full, pathname, level);

    rc = ((fname != NULL) && (stat_file(fname, &sb) == 0));
    return rc;
}

/*
 * Infer the level of a vault directory by the presence of parts-list files.
 */
static int
level_of(const char *pathname)
{
    int level;
    int found;
    char part[MAXPATHLEN];

    for (level = 0, found = FALSE; level <= 2; level++) {
	if (part_exists(part, level)) {
	    found = TRUE;
	    break;
	}
    }
    if (found) {

	if (level == 2) {
	    DYN *p = dyn_alloc((DYN *) 0, strlen(pathname) + 1);
	    (void) strcpy(p->text, pathname);
	    do {
		*strrchr(p->text, '/') = EOS;
		if (!part_exists(p->text, level + 1))
		    break;
		level++;
	    } while (*part != EOS);
	    dyn_free(p);
	}
    } else {
	level = -1;
    }
    return level;
}

/******************************************************************************/

/*
 * We put stuff on the end of the linked list to preserve a natural ordering
 * of the search path.
 */
static VAULTS *
add_archive(const char *pathname)
{
    if (*pathname != EOS) {
	VAULTS *p, *q, *r;
	for (p = VaultList, q = NULL; p != NULL; q = p, p = p->next) ;
	r = typealloc(VAULTS);
	r->next = NULL;
	r->archive = txtalloc(pathname);
	r->working = NULL;
	r->cmtree = NewCmTree(pathname);
	r->cmtree->level =
	    r->level = level_of(pathname);
	if (q == NULL)
	    VaultList = r;
	else
	    q->next = r;
	return r;
    }
    return NULL;
}

static void
add_working(VAULTS * list, char *pathname)
{
    if (*pathname != EOS) {
	WORKING *p, *q, *r;
	for (p = list->working, q = NULL; p != NULL; q = p, p = p->next) ;
	r = typealloc(WORKING);
	r->next = NULL;
	r->working = txtalloc(pathname);
	if (q == NULL)
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
    CmvVault = getenv("CMV_VAULT");
    if (CmvVault != NULL) {
	char *s;
	char *next, *eqls;
	int at_next, at_eqls;
	VAULTS *p;

	for (s = CmvVault; *s != EOS; s = next) {
	    next = strchr(s, PATHLIST_SEP);
	    if (next == NULL)
		next = s + strlen(s);
	    at_next = *next;
	    *next = EOS;

	    eqls = strchr(s, '=');
	    if (eqls == NULL)
		eqls = next;
	    at_eqls = *eqls;
	    *eqls = EOS;

	    if ((p = add_archive(s)) != NULL) {
		while (eqls != next) {
		    *eqls = (char) at_eqls;
		    s = eqls + 1;

		    eqls = strchr(s, '=');
		    if (eqls == NULL)
			eqls = next;
		    at_eqls = *eqls;
		    *eqls = EOS;

		    add_working(p, s);
		}
	    }
	    if ((*next = (char) at_next) != EOS)
		next++;
	}
	CmvVault = txtalloc(CmvVault);
    }
}

/******************************************************************************/

/*
 * Skip backwards in the given substring to the previous path separator.  If
 * the entire leaf looks like a CmVision internal name (e.g., d-<number>),
 * return the pointer to that path separator.  Otherwise, return null.
 */
static char *
UpInternalDir(char *first, char *last)
{
    int state = 3;

    Debug((stderr, "Up(%s) %s\n", first, last));
    while (--last >= first) {
	if (isdigit(UCH(*last))) {
	    if (state != 3)
		break;
	} else if (*last == '-') {
	    if (state-- != 3)
		break;
	} else if (*last == 'd') {
	    if (state-- != 2)
		break;
	} else if (isSlash(*last)) {
	    if (state-- != 1)
		break;
	    Debug((stderr, "=> %s\n", last));
	    return last;
	} else {
	    break;
	}
    }
    return NULL;
}

/*
 * Links are (apparently) always relative to the second level of the CmVision
 * vault.  Strip off extra levels from the archive path so we'll be able to
 * append the internal name correctly.
 *
 * For example, /vault/d-1234/d-3456 becomes /vault/d-1234.
 */
static void
StripToTop(char *dst, char *src)
{
    char *base = strcpy(dst, src);
    char *last = base + strlen(base);
    char *ptr1;
    char *ptr2;

    if ((ptr1 = UpInternalDir(base, last)) != NULL) {
	while (last > base) {
	    if ((ptr2 = UpInternalDir(base, ptr1)) == NULL) {
		*last = EOS;
		return;
	    }
	    last = ptr1;
	    ptr1 = ptr2;
	}
    }
}

/******************************************************************************/
static VAULTS *
LookupVault(const char *working_directory,
	    const char *filename,
	    char *result)
{
    Stat_t sb;
    VAULTS *p, *max_p = NULL;
    WORKING *q;
    int max_n = 0;

    Debug((stderr, "LookupVault\n"));
    Debug((stderr, "...dir:%s\n", working_directory));
    Debug((stderr, "..file:%s\n", filename));

    if (!initialized)
	Initialize();
    if (CmvVault == NULL || filename == NULL)
	return NULL;

    /*
     * If we're given the name of a file, compute its directory.  If we're
     * given a directory name, use it.
     */
    abspath(pathcat(result, working_directory, filename));
    if (stat_dir(result, &sb) < 0)
	(void) strcpy(result, pathhead(result, &sb));

    Debug((stderr, "..test:%s\n", result));

    /*
     * Now, search the CMV_VAULT variable for a working directory that
     * matches the beginning of the string we've got in 'result[]'.  If we
     * find that match, substitute the remainder to obtain the archive
     * directory path.  If we find an archive without a working directory
     * in CMV_VAULT, use this iff no prior match is found.
     */
    for (p = VaultList; p != NULL; p = p->next) {
	for (q = p->working; q != NULL; q = q->next) {
	    int n;
	    if ((n = samehead(result, q->working)) > 0
		&& n >= (int) strlen(q->working)) {
		if (n > max_n) {
		    Debug((stderr, "..same:%s\n", q->working));
		    max_p = p;
		    max_n = n;
		}
	    }
	}
    }
    if (max_n > 0) {
	int j;
	int level = max_p->level;
	if (result[max_n] != EOS) {
	    max_n++;
	    level++;
	}
	for (j = 0; (result[j] = result[j + max_n]) != EOS; j++)
	    if (isSlash(result[j]))
		level++;;
	Debug((stderr, "=>%s\n", result));
    }
    return (max_n > 0) ? max_p : NULL;
}

/******************************************************************************/
char *
cmv_dir(const char *working_directory, const char *filename)
{
    char *name = NULL;
    char temp[MAXPATHLEN];
    VAULTS *max_p = LookupVault(working_directory, filename, temp);

    if (max_p != NULL) {	/* we found a match */
	char archive[MAXPATHLEN];
	CMTREE *it = FindInternalDir(max_p->cmtree, temp);

	if (it != NULL
	    && (strlen(max_p->archive) + strlen(it->internal) + 3) < sizeof(archive)) {
	    (void) pathcat(
			      archive,
			      strcpy(archive, max_p->archive),
			      it->internal);
	    name = txtalloc(archive);
	}
    }

    return (name);
}

/******************************************************************************/
char *
cmv_file(const char *working_directory,
	 const char *filename)
{
    char *name = NULL;
    char temp[MAXPATHLEN];
    VAULTS *max_p = LookupVault(working_directory, filename, temp);

    if (max_p != NULL) {	/* we found a match */
	char archive[MAXPATHLEN];
	CMTREE *p;
	CMFILE *q;

	if ((p = FindInternalDir(max_p->cmtree, temp)) != NULL) {
	    char *my_leaf = pathleaf(filename);
	    if (strlen(temp) + strlen(my_leaf) + 3 >= sizeof(temp)) {
		;		/* give up */
	    } else if ((q = FindInternalFile(p,
					     pathcat(temp, temp, my_leaf)))
		       != NULL) {
		*archive = EOS;
		if (q->internal[0] == PATH_SLASH) {
		    StripToTop(archive, max_p->archive);
		    if (strlen(archive) + strlen(q->internal) < sizeof(archive)) {
			(void) strcat(archive, q->internal);
		    } else {
			*archive = EOS;
		    }
		} else if ((strlen(max_p->archive)
			    + strlen(p->internal)
			    + strlen(q->internal)
			    + 5) < sizeof(archive)) {
		    (void) pathcat(
				      archive,
				      pathcat(
						 archive,
						 strcpy(archive, max_p->archive),
						 p->internal),
				      q->internal);
		}
		if (*archive != EOS) {
		    name = txtalloc(archive);
		}
	    }
	}
    }

    return (name);
}

/******************************************************************************/
void
get_cmv_lock(const char *working_directory,
	     const char *filename,
	     const char **lockedby,
	     const char **revision,
	     time_t *modtime)
{
    char temp[MAXPATHLEN];
    VAULTS *max_p = LookupVault(working_directory, filename, temp);

    *lockedby =
	*revision = "?";
    *modtime = 0;
    if (max_p != NULL) {	/* we found a match */
	CMTREE *p;
	CMFILE *q;

	if ((p = FindInternalDir(max_p->cmtree, temp)) != NULL
	    && (q = FindInternalFile(p, pathcat(temp, temp,
						pathleaf(filename)))) != NULL) {
	    *lockedby = q->lockedby;
	    *revision = q->revision;
	    *modtime = q->modtime;
	}
    }
}

/******************************************************************************/
void
purge_cmv_dir(const char *working_directory, const char *filename)
{
    char temp[MAXPATHLEN];
    VAULTS *max_p = LookupVault(working_directory, filename, temp);

    if (max_p != NULL) {	/* we found a match */
	CMTREE *it = FindInternalDir(max_p->cmtree, temp);

	if (it != NULL) {
	    read_r_curr(it);
	}
    }
}

/******************************************************************************/
#ifdef	TEST
_MAIN
{
    int n;
    for (n = 1; n < argc; n++) {
	const char *s = cmv_dir(".", argv[n]);
	const char *t = cmv_file(".", argv[n]);
	if (s == 0)
	    s = "?";
	if (t == 0)
	    t = "?";
	PRINTF("%d:%d %s\n", n, argc - 1, argv[n]);
	PRINTF("\tdir  '%s'\n", s);
	PRINTF("\tfile '%s'\n", t);
    }
    exit(EXIT_SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */
