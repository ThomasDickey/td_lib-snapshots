/* $Id: cmv_defs.h,v 12.5 1998/08/20 20:51:33 tom Exp $ */

#ifndef	CMV_DEFS_H
#define	CMV_DEFS_H

	/*
	 * The CmVision vault-tree contains three visible types of files:
	 *
	 *	s-curr
	 *		contains a list of directory leaf names in internal
	 *		form, two semicolons (?) and the external leaf name.
	 *
	 *		Example:
	 *			d-1385;;tools
	 *	r-curr
	 *		contains a list of file leaf names in internal format,
	 *		the string ";/;" (?), the external leaf name, ";" and
	 *		the current release version.
	 *
	 *		Example
	 *			f-3001;/;jb_error.c;4.1
	 *	f-xxxx
	 *		an SCCS (or RCS?) file containing the actual delta
	 *		history.
	 *
	 */

	/* cmv_dir.c ------------------------------------------------- */
	char *	cmv_dir(
			_arx(char *,	working_dir)
			_ar1(char *,	pathname)
			)
			_dcl(char *,	working_dir)
			_dcl(char *,	pathname)
			_ret

	char *	cmv_file(
			_arx(char *,	working_dir)
			_ar1(char *,	pathname)
			)
			_dcl(char *,	working_dir)
			_dcl(char *,	pathname)
			_ret

	void	get_cmv_lock(
			_arx(char *,	working_dir)
			_arx(char *,	pathname)
			_arx(char **,	lockedby)
			_arx(char **,	revision)
			_ar1(time_t *,	t_)
			)
			_dcl(char *,	working_dir)
			_dcl(char *,	pathname)
			_dcl(char *,	lockedby)
			_dcl(char *,	revision)
			_dcl(time_t *,	t_)
			_nul

	void	purge_cmv_dir(
			_arx(char *,	working_dir)
			_ar1(char *,	pathname)
			)
			_dcl(char *,	working_dir)
			_dcl(char *,	pathname)
			_nul

	/* cmv_dbug.c ------------------------------------------------- */
	int	cmv_debug(_ar0)
			_ret

	/* cmv_last.c ------------------------------------------------- */
	void	cmv_last(
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

	/* lastrev.c -------------------------------------------------- */
	void	lastrev(
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

#endif	/* CMV_DEFS_H */
