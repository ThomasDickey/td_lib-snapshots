/* $Id: sccsdefs.h,v 12.3 1994/06/25 14:15:25 tom Exp $ */

#ifndef	SCCSDEFS_H
#define	SCCSDEFS_H

#define	SCCS_DIR	"SCCS"
#define	SCCS_PREFIX	"s."

	/* sccs_dir.c ------------------------------------------------- */
	char *	sccs_dir(_ar0)
			_ret

	/* sccsdbug.c ------------------------------------------------- */
	int	sccs_debug(_ar0)
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

#endif	/* SCCSDEFS_H */
