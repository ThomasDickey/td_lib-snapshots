/* $Id: sccsdefs.h,v 8.0 1990/05/08 14:11:44 ste_cm Rel $ */

#ifndef	_SCCSDEFS_H_
#define	_SCCSDEFS_H_


#define	SCCS_DIR	"SCCS"
#define	SCCS_PREFIX	"s."

	/* sccs_dir.c ------------------------------------------------- */
	char *	sccs_dir()
			_RET

	/* sccslast.c ------------------------------------------------- */
		sccslast(
			_ARX(char *,	wd)
			_ARX(char *,	n)
			_ARX(char **,	v_)
			_ARX(time_t *,	t_)
			_AR1(char **,	l_)
			)
			_DCL(char *,	wd)
			_DCL(char *,	n)
			_DCL(char **,	v_)
			_DCL(time_t *,	t_)
			_DCL(char **,	l_)
			_NUL

	/* sccsname.c ------------------------------------------------- */
	char *	sccs2name(
			_ARX(char *,	name)
			_AR1(int,	full)
			)
			_DCL(char *,	name)
			_DCL(int,	full)
			_RET

	char *	name2sccs(
			_ARX(char *,	name)
			_AR1(int,	full)
			)
			_DCL(char *,	name)
			_DCL(int,	full)
			_RET

#endif	_SCCSDEFS_H_
