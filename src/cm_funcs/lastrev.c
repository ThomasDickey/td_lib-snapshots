/*
 * Title:	lastrev.c (find last revision & date)
 * Author:	T.E.Dickey
 * Created:	03 Aug 1994, from 'dedscan.c'
 * Modified:
 *
 * Function:	Combines calls to rcslast, sccslast, cmv_last.
 */

#define STR_PTYPES
#include	"ptypes.h"
#include	"cmv_defs.h"
#include	"rcsdefs.h"
#include	"sccsdefs.h"

MODULE_ID("$Id: lastrev.c,v 12.5 1994/10/06 00:02:27 tom Exp $")

/*
 * This is driven by an environment variable, but ultimately should be done
 * via ".dedrc"
 */
#if	defined(RCS_PATH) || defined(SCCS_PATH)
typedef	enum TrySCCS { DontTry, TrySccs, TryRcs, TryCmVision } TRY;

static	TRY	try_order(
	_AR1(int,	try))
	_DCL(int,	try)
{
	static	int	 num_order;
	static	TRY vec_order[10];

	auto	char	temp[BUFSIZ];
	auto	char	*s;

	if (num_order == 0) {
		char	*env = getenv("DED_CM_LOOKUP");
		if (env != 0) {
			env = strlcpy(temp, env);
		} else {
			env = strcpy(temp, "rcs");
#ifdef SCCS_PATH
			(void)strcat(temp, ",sccs");
#endif
		}
		while ((s = strtok(env, ",")) != 0) {
			if (!strcmp(s, "rcs")) {
				vec_order[num_order++] = TryRcs;
			} else if (!strcmp(s, "sccs")) {
				vec_order[num_order++] = TrySccs;
			} else if (!strcmp(s, "cmv")) {
				vec_order[num_order++] = TryCmVision;
			}
			env = 0;
		}

		vec_order[num_order++] = DontTry; /* end-marker */
	}
	if (try >= num_order)
		try = num_order;
	return vec_order[try];
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
	TRY	try;
	for (n = 0; (try = try_order(n)) != DontTry; n++) {
#ifdef	RCS_PATH
		if (try == TryRcs) {
			LAST(rcslast);
		}
#endif
#ifdef	SCCS_PATH
		if (try == TrySccs) {
			LAST(sccslast);
		}
#endif
#if defined(CMV_PATH) && (defined(RCS_PATH) || defined(SCCS_PATH))
		if (try == TryCmVision) {
			LAST(cmv_last);
		}
#endif
		if (*time_ptr != 0
		 || *vers_ptr[0] != '?'
		 || *lock_ptr[0] != '?')
		 break;
	}
}
#endif	/* RCS_PATH || SCCS_PATH */