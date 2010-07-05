/*
 * Title:	lastrev.c (find last revision & date)
 * Author:	T.E.Dickey
 * Created:	03 Aug 1994, from 'dedscan.c'
 * Modified:
 *		15 Oct 2009, add svnlast().
 *		07 Mar 2004, remove K&R support, indent'd.
 *		25 Apr 2003, add cvslast().
 *		30 May 1998, compile with g++
 *		15 Feb 1998, guard against use of non-configured modules.
 *
 * Function:	Combines calls to rcslast, sccslast, cmv_last.
 */

#define STR_PTYPES
#include	"ptypes.h"
#include	"cmv_defs.h"
#include	"rcsdefs.h"
#include	"sccsdefs.h"

MODULE_ID("$Id: lastrev.c,v 12.13 2010/07/04 16:43:58 tom Exp $")

#if defined(CMV_PATH) && !(defined(RCS_PATH) || defined(SCCS_PATH))
#undef CMV_PATH
#endif

/*
 * This is driven by an environment variable, but ultimately should be done
 * via ".dedrc"
 */
#if defined(RCS_PATH) || defined(SCCS_PATH) || defined(CVS_PATH) || defined(SVN_PATH)
typedef enum TrySCCS {
    DontTry, TrySccs, TryRcs, TryCvs, TrySvn, TryCmVision
} TRY;

#define MAX_ORDER 10

static TRY
try_order(int tried)
{
    static int num_order;
    static TRY vec_order[MAX_ORDER];

    char temp[BUFSIZ];
    char *s;

    if (num_order == 0) {
	char *env = getenv("DED_CM_LOOKUP");
	if (env != 0) {
	    env = strlwrcpy(temp, env);
	} else {
	    env = strcpy(temp, "rcs");
#ifdef SCCS_PATH
	    (void) strcat(temp, ",sccs");
#endif
#ifdef CVS_PATH
	    (void) strcat(temp, ",cvs");
#endif
#ifdef SVN_PATH
	    (void) strcat(temp, ",svn");
#endif
	}
	while ((s = strtok(env, ",")) != 0) {
	    if (!strcmp(s, "rcs")) {
		vec_order[num_order++] = TryRcs;
	    } else if (!strcmp(s, "sccs")) {
		vec_order[num_order++] = TrySccs;
	    } else if (!strcmp(s, "cmv")) {
		vec_order[num_order++] = TryCmVision;
	    } else if (!strcmp(s, "cvs")) {
		vec_order[num_order++] = TryCvs;
	    } else if (!strcmp(s, "svn")) {
		vec_order[num_order++] = TrySvn;
	    }
	    env = 0;
	    if (num_order + 1 >= MAX_ORDER)
		break;
	}

	vec_order[num_order++] = DontTry;	/* end-marker */
    }
    if (tried >= num_order)
	tried = num_order;
    return vec_order[tried];
}

#define	LAST(p)	p(working_dir, filename, vers_ptr, time_ptr, lock_ptr)

void
lastrev(const char *working_dir,
	const char *filename,
	const char **vers_ptr,
	time_t * time_ptr,
	const char **lock_ptr)
{
    int n;
    TRY tried;

    for (n = 0; (tried = try_order(n)) != DontTry; n++) {
#ifdef	RCS_PATH
	if (tried == TryRcs) {
	    LAST(rcslast);
	}
#endif
#ifdef	SCCS_PATH
	if (tried == TrySccs) {
	    LAST(sccslast);
	}
#endif
#ifdef	CMV_PATH
	if (tried == TryCmVision) {
	    LAST(cmv_last);
	}
#endif
#ifdef	CVS_PATH
	if (tried == TryCvs) {
	    LAST(cvslast);
	}
#endif
#ifdef	SVN_PATH
	if (tried == TrySvn) {
	    LAST(svnlast);
	}
#endif
	if (*time_ptr != 0
	    || (*vers_ptr != 0 && *vers_ptr[0] != '?')
	    || (*lock_ptr != 0 && *lock_ptr[0] != '?'))
	    break;
    }
}
#endif /* RCS_PATH || SCCS_PATH */
