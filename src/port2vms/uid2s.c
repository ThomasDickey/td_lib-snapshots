/*
 * Title:	uid2s.c
 * Author:	T.E.Dickey
 * Created:	15 Dec 1988
 * Modified:
 *		07 Mar 2004, remove K&R support, indent'd.
 *		01 Dec 1993, ifdefs.
 *		20 Nov 1992, use prototypes
 *		18 Oct 1991, use macro _MAIN
 *		20 May 1991, apollo sr10.3 cpp complains about endif-tags
 *		16 Nov 1989, rewrote, adding gid2s, s2gid, vms_uid2s functions
 *		14 Mar 1989, extended test-driver
 *		 9 Jan 1989 added 's2uid()' function
 *
 * Function:	Given a unix-style uid, finds the corresponding username string
 *		and returns a pointer to it.
 *
 *		The corresponding inverse translation is bundled with this
 *		module on VMS since no password support is provided.
 *
 *		The corresponding functions for group-id are bundled here also
 *		because the VMS system call which returns user-name does not
 *		distinguish (except by use of "-1" for group) between these
 *		and group-names.
 */

#define	STR_PTYPES
#include	"port2vms.h"

MODULE_ID("$Id: uid2s.c,v 12.3 2004/03/07 22:03:45 tom Exp $")

#ifdef	vms
#include	<ssdef.h>

typedef unsigned long LID;

#define	ID_MASK		0xffff
#define	GID_SHIFT	16

#define	_ID2UID(n)	(n & ID_MASK)
#define	_ID2GID(n)	((n >> GID_SHIFT) & ID_MASK)

#define	VEC2UID(n)	_ID2UID(vec[n].id)
#define	VEC2GID(n)	_ID2GID(vec[n].id)

#define	is_GROUP(n)	((vec[n].id & ID_MASK) == ID_MASK)
#define	is_MATCH(n,s)	(!strucmp(vec[n].name,s))

typedef struct {
    LID id;
    char *name;
} LIST;

static LIST *vec;		/* list of uid/gid values */
static int numvec;		/* number of items in 'vec[]' */

#ifdef	TEST
static char *
show(int id)
{
    static char buffer[80];
    sprintf(buffer, "%o,%o", _ID2GID(id), _ID2UID(id));
    return (buffer);
}
#endif

static void
build_vec(void)
{
    int j;

    long st;
    unsigned amount;
    char buffer[80];

    long contxt = 0;
    LID id = -1;
    short namlen = 0;
    LID resid = 0;
    long attrib = 0;
    $DESCRIPTOR(nambuf, buffer);

    /*
     * Unix-style uid's are mapped into VMS id's by gluing in the gid field.
     * If the caller does not provide both, we must do a scan to find the
     * name assuming that there is a single gid value (usually the case!).
     *
     * This saves the id's returned so that successive calls will be faster.
     */
    if (vec == 0) {
	while ((st = sys$idtoasc(
				    id,
				    &namlen,
				    &nambuf,
				    &resid,
				    0 /*&attrib */ ,
				    &contxt)) == SS$_NORMAL) {
	    buffer[namlen] = '\0';
	    j = numvec;
	    amount = (++numvec) * sizeof(LIST);
	    vec = doalloc(vec, amount);
	    vec[j].id = resid;
	    vec[j].name = stralloc(buffer);
#ifdef	TEST
	    printf("id = %s, name = %s\n", show(vec[j].id), vec[j].name);
#endif
	}
	if (st != SS$_NOSUCHID)
	    sys$finish_rdb(contxt);
    }
}

/*
 * Translate a uid to a string, returning a pointer to text-constant.
 * Note that on VAX/VMS we may want to lookup the ordered pair [gid,uid].
 */
char *
uid2s(int uid)
{
    char buffer[80];
    int j;
    int gid = _ID2GID(uid);

    build_vec();
    (void) strcpy(buffer, "?");
    uid = _ID2UID(uid);
    if (uid != ID_MASK) {
	for (j = 0; j < numvec; j++) {
	    if (VEC2UID(j) == uid
		&& (gid == 0 || VEC2GID(j) == gid)) {
		(void) strcpy(buffer, vec[j].name);
		break;
	    }
	}
    }
    return (txtalloc(buffer));
}

/*
 * Translate a gid to a string, returning a pointer to text-constant
 */
char *
gid2s(int gid)
{
    char buffer[80];
    int j;

    build_vec();
    (void) strcpy(buffer, "?");
    if (gid != ID_MASK) {
	gid = (gid << GID_SHIFT) | ID_MASK;
	for (j = 0; j < numvec; j++) {
	    if (gid == vec[j].id) {
		(void) strcpy(buffer, vec[j].name);
		break;
	    }
	}
    }
    return (txtalloc(buffer));
}

/*
 * Translate a string back to a uid, returning a negative number iff not found
 */
int
s2uid(char *name)
{
    int j;

    build_vec();
    for (j = 0; j < numvec; j++) {
	if (!is_GROUP(j)
	    && is_MATCH(j, name))
	    return (VEC2UID(j));
    }
    return (-1);
}

/*
 * Translate a string back to a gid, returning a negative number iff not found
 */
int
s2gid(char *name)
{
    int j;

    build_vec();
    for (j = 0; j < numvec; j++) {
	if (is_GROUP(j)
	    && is_MATCH(j, name))
	    return (VEC2GID(j));
    }
    return (-1);
}

/*
 * Special VMS-only entrypoint to translate id to string
 */
char *
vms_uid2s(int id)
{
    int uid = _ID2UID(id);
    int gid = _ID2GID(id), bracket = ((gid & 0100000) == 0);
    char tmp[BUFSIZ];

    *tmp = EOS;
    if (bracket) {
	strcat(tmp, "[");
	if (gid != 1) {
	    strcat(tmp, gid2s(gid));
	    strcat(tmp, ",");
	}
    }
    strcat(tmp, uid2s(id));
    if (bracket)
	strcat(tmp, "]");
    return (txtalloc(tmp));
}

#ifdef	TEST
/*ARGSUSED*/
_MAIN
{
    int uid = getuid();
    int gid = getgid();
    int j, k;
    char buffer[L_cuserid];
    extern char *cuserid();

    printf("lookup (cuserid=%s)\n", cuserid(buffer));
    printf("uid %s => '%s'\n", show(uid), uid2s(uid));
    printf("uid %s => '%s'\n", show(uid), uid2s(uid));
    printf("gid %s => '%s'\n", show(gid), gid2s(gid));
    printf("invert => %s\n", show(s2uid(cuserid(buffer))));

    for (j = 1; j < argc; j++) {
	if (!strucmp(argv[j], "-t")) {
	    for (k = 0; k < numvec; k++) {
		printf("%s = %s\n",
		       show(vec[k].id),
		       vms_uid2s(vec[k].id));
	    }
	} else {
	    uid = s2uid(argv[j]);
	    printf("uid of %s is %s\n", argv[j], show(uid));
	}
    }
    exit(SUCCESS);
    /*NOTREACHED */
}
#endif /* TEST */

#endif /* vms */
