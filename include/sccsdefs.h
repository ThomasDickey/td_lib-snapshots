/* $Id: sccsdefs.h,v 12.1 1993/09/21 14:02:28 dickey Exp $ */

#ifndef	_SCCSDEFS_H_
#define	_SCCSDEFS_H_


#define	SCCS_DIR	"SCCS"
#define	SCCS_PREFIX	"s."

	/* sccs_dir.c ------------------------------------------------- */
	char *	sccs_dir(_ar0)
			_ret

	/* sccslast.c ------------------------------------------------- */
	void	sccslast(
			_arx(char *,	wd)
			_arx(char *,	n)
			_arx(char **,	v_)
			_arx(time_t *,	t_)
			_ar1(char **,	l_)
			)
			_dcl(char *,	wd)
			_dcl(char *,	n)
			_dcl(char **,	v_)
			_dcl(time_t *,	t_)
			_dcl(char **,	l_)
			_nul

	/* sccsname.c ------------------------------------------------- */
	char *	sccs2name(
			_arx(char *,	name)
			_ar1(int,	full)
			)
			_dcl(char *,	name)
			_dcl(int,	full)
			_ret

	char *	name2sccs(
			_arx(char *,	name)
			_ar1(int,	full)
			)
			_dcl(char *,	name)
			_dcl(int,	full)
			_ret

#endif	/* _SCCSDEFS_H_ */
