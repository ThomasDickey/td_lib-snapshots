/*
 * Title:	lastrev.c (find last revision & date)
 * Author:	T.E.Dickey
 * Created:	03 Aug 1994, from 'dedscan.c'
 * Modified:
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

MODULE_ID("$Id: lastrev.c,v 12.9 2003/04/26 10:47:26 tom Exp $")

#if defined(CMV_PATH) && !(defined(RCS_PATH) || defined(SCCS_PATH))
#undef CMV_PATH
#endif

/*
 * This is driven by an environment variable, but ultimately should be done
 * via ".dedrc"
 */
#if	defined(RCS_PATH) || defined(SCCS_PATH)
typedef	enum TrySCCS { DontTry, TrySccs, TryRcs, TryCvs, TryCmVision } TRY;

#define MAX_ORDER 10

static	TRY	try_order(
	_AR1(int,	tried))
	_DCL(int,	tried)
{
	static	int	 num_order;
	static	TRY vec_order[MAX_ORDER];

	auto	char	temp[BUFSIZ];
	auto	char	*s;

	if (num_order == 0) {
		char	*env = getenv("DED_CM_LOOKUP");
		if (env != 0) {
			env = strlwrcpy(temp, env);
		} else {
			env = strcpy(temp, "rcs");
#ifdef SCCS_PATH
			(void)strcat(temp, ",sccs");
#endif
#ifdef CVS_PATH
			(void)strcat(temp, ",cvs");
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
			}
			env = 0;
			if (num_order + 1 >= MAX_ORDER)
				break;
		}

		vec_order[num_order++] = DontTry; /* end-marker */
	}
	if (tried >= num_order)
		tried = num_order;
	return vec_order[tried];
}

#define	LAST(p)	p(working_dir, filename, vers_ptr, time_ptr, lock_ptr)

void	lastrev(
	_ARX(char *,	working_dir)
	_ARX(char *,	filename)
	_ARX(char **,	vers_ptr)
	_ARX(time_t *,	time_ptr)
	_AR1(char **,	lock_ptr)
		)
	_DCL(char *,	working_dir)
	_DCL(char *,	filename)
	_DCL(char **,	vers_ptr)
	_DCL(time_t *,	time_ptr)
	_DCL(char **,	lock_ptr)
{
	int	n;
	TRY	tried;
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
		if (*time_ptr != 0
		 || (*vers_ptr != 0 && *vers_ptr[0] != '?')
		 || (*lock_ptr != 0 && *lock_ptr[0] != '?'))
		 break;
	}
}
#endif	/* RCS_PATH || SCCS_PATH */
